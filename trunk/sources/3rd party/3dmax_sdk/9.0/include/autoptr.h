//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Automatic pointer supporting resource-acquisition is 
//    initialization technique.
// AUTHOR: Nicolas Desjardins
// DATE: 2006/03/20 
//***************************************************************************/

#ifndef _AUTOPTR_H_
#define _AUTOPTR_H_

#ifndef NULL
#define NULL 0
#endif

namespace MaxSDK
{
namespace Util
{

/*!
 * \brief Automatic memory-ownership pointer supporting "resource acquisition
 * is initialization."
 *
 * This is a standardized implementation of std::auto_ptr from the STL, allowing
 * us to portably include AutoPtrs in the 3ds Max SDK without forcing a compiler
 * or runtime dll version on third-party developers.  std::auto_ptrs are 
 * described in your favourite STL documentation, in Scott Meyers's More 
 * Effective C++ and on pp. 367-368 in Bjarne Stroustrup's The C++ Programming 
 * Language.  This implementation is based on Stroustrup and Meyers.
 *
 * AutoPtrs grant the same mechanics as a regular pointer, so they can be
 * created, assigned and dereferenced much in the same way as a plain old 
 * pointer.  Additionally, AutoPtrs provide "resource acquisition is 
 * initialization" and automatic memory ownership management in two ways:
 * <ol>
 * <li>The object pointed to by the AutoPtr will be implicitly deleted when 
 * the AutoPtr goes out of scope</li>
 * <li>Through "destructive copying" that causes a copied AutoPtr to surrender 
 * ownership to the pointed to object.</li>
 * </ol>
 *
 * This is a very useful technique for making the ownership semantics of a 
 * function very clear.  A function that takes an AutoPtr to an object as a 
 * parameter is stating that it assumes ownership of the pointed to object and
 * that the caller must surrender ownership.  A function that returns an
 * AutoPtr states that the caller must assume ownership of the pointed-to 
 * object.
 *
 * AutoPtrs help prevent memory leaks and lead to exception safe code by 
 * guaranteeing that dynamically allocated objects will be deleted when the
 * AutoPtr goes out of scope.  The following is unsafe code:
 * \code
 * void foo()
 * {
 *    A *a = new A();
 *    // ...
 *    delete a;
 * }
 * \endcode
 *
 * Will leak if any exception is thrown in the "..." part.  Replacing with
 * an AutoPtr guarantees that the memory is cleaned up, since leaving the
 * function through an exception will cause the AutoPtr to go out of scope.
 * \code
 * void foo()
 * {
 *    AutoPtr a(new A());
 *    // ...
 * }
 * \endcode
 *
 * NB: None of AutoPtr's member functions are allowed to throw an 
 * exception.
 *
 * NB2: The destructive copy semantics mean that AutoPtrs are 
 * inappropriate for use in any of the STL's container classes.
 * 
 * NB3: AutoPtr calls delete (and not delete []), therefore it is inappropriate
 * to use AutoPtr to hold dynamically allocated arrays (allocated with new []).
 * It is also inappropriate to use an AutoPtr for an object allocated using 
 * \c malloc. Such an object must be cleaned up using \c free.
 * 
 * NB4: Finally, be careful about passing AutoPtrs across DLL boundaries for
 * DLLs compiled with different compilers or different memory allocation 
 * schemes.  Objects traversing such boundaries must be deallocated on the same 
 * side as they were allocated.  One way to guarantee this is to use 
 * derive from MaxHeapOperators or to override a class's delete operator so
 * that the code that actually calls delete is on the correct side of the 
 * boundary.
 */
template <typename Type>
class AutoPtr
{
private:
	/*!
	 * \brief Helper class used to implement destructive copy semantics and allow
	 * for AutoPtrs to be used as return values.
	 *
	 * You don't really need to know this since the compiler will use 
	 * AutoPtrRefs silently and everything will just work out the way you'd 
	 * expect, but here's an explanation of why AutoPtrRef is needed.
	 *
	 * Values returned by functions are temporaries (unnamed objects that you 
	 * never actually see in your source code).  These then get copied into 
	 * whatever variable is set up to receive them. (See More Effective C++, 
	 * items 19 and 20.)
	 *
	 * Since these temporaries are inaccessible to normal source code, it is
	 * usually a design error for them to be modified, so the compiler makes
	 * them const.  They can't be bound to non-const references, which are
	 * needed for the AutoPtr copy constructor and assignment operator.
	 * Attempting to return and receive an AutoPtr from a function would then
	 * generate a compiler error.
	 * 
	 * The AutoPtrRef and cast operations below exists to allow the compiler 
	 * to implicitly convert from the const AutoPtr returned from a function 
	 * to an AutoPtrRef, which can then be assigned to the receiving AutoPtr 
	 * using the AutoPtr(AutoPtrRef&) constructor.
	 *
	 * (Okay, there's a bit more going on to get around the double-conversion
	 * rule, which Herb Sutter explains as: "[A] subtle interaction in the
	 * overload resolution rules allows the selection of single viable
	 * constructor function, followed by the manufacture of just the right
	 * temporary which lets us slip in a second user-defined conversion (to
	 * auto_ptr_ref<Z>) that is normally forbidden by 12.3/4." From
	 * the thread "What is auto_ptr_ref for?" on comp.lang.c++.moderated.)
	 */
	template<typename OtherType> struct AutoPtrRef
	{
		OtherType *mPtr;
		AutoPtrRef(OtherType *p)  : mPtr(p) {}		
	};
	
	/*!
	 * \brief This prohibits the casual allocation of AutoPtrs on the heap.
	 *
	 * Trying to do so will result in a compile-time error. Not implemented,
	 * so it will also result in a link-time error if a class somehow has
	 * access to it.
	 */
	void * operator new(size_t size);	
	
	/*!
	 * \brief This prohibits the placement initialization of AutoPtrs on the 
	 * heap. 
	 *
	 * Trying to do so will result in a compile-time error. Not 
	 * implemented, so it will also result in a link-time error if a class 
	 * somehow has access to it.
	 */	
	void * operator new(size_t size, void * placement);
	
	/*!
	 * \brief The underlying plain-old pointer.
	 */
	Type *mPtr;

public:
	/*!
	 * \brief Typedef to make the element type a member of the class.
	 */
	typedef Type element_type;

	/*!
	 * \brief Construct, assuming ownership of the pointed-to object.
	 *
	 * \param p Plain pointer to an object - this AutoPtr will assume ownership
	 * of that object.
	 */
	explicit AutoPtr(Type *p = NULL)  : mPtr(p) {}

	/*!
	 * \brief Destructor - automatically cleans up the pointed-to object.
	 */
	~AutoPtr()  { delete mPtr; }

	/*!
	 * \brief Copy construct, assuming ownership from the source AutoPtr.
	 *
	 * \param a Another AutoPtr.  This newly constructed AutoPtr will take
	 * ownership of a's pointed-to object, and subsequently, a will point to NULL.
	 * AutoPtr a may no longer be used to access the pointed-to object after 
	 * being copied.
	 */
	AutoPtr(AutoPtr &a)  : mPtr(a.Release()) {}

	/*!
	 * \brief Conversion copy constructor.  Assumes ownership of any type 
	 * assignable to Type.
	 *
	 * \param a Another AutoPtr.  This newly constructed AutoPtr will take
	 * ownership of a's pointed-to object, and subsequently, a will point to NULL.
	 * AutoPtr a may no longer be used to access the pointed-to object after 
	 * being copied.
	 */
	template <typename OtherType>
	AutoPtr(AutoPtr<OtherType> &a)  : mPtr(a.Release()) {}

	/*!
	 * \brief Assignment, takes over ownership from the source AutoPtr.  Any
	 * existing pointer in this AutoPtr will be deleted.
	 *
	 * \param a Another AutoPtr.  This newly constructed AutoPtr will take
	 * ownership of a's pointed-to object, and subsequently, a will point to NULL.
	 * AutoPtr a may no longer be used to access the pointed-to object after 
	 * being copied.
	 */
	AutoPtr& operator=(AutoPtr &a) 
	{
		if(Get() != a.Get())
		{
			delete mPtr;
			mPtr = a.Release();
		}
		return *this;
	}

	/*!
	 * \brief Conversion assignment, takes over ownership of any type assignable
	 * to type from the source AutoPtr.  Any existing pointer in this AutoPtr
	 * will be deleted.
	 *
	 * \param a Another AutoPtr.  This newly constructed AutoPtr will take
	 * ownership of a's pointed-to object, and subsequently, a will point to NULL.
	 * AutoPtr a may no longer be used to access the pointed-to object after 
	 * being copied.
	 */
	template<typename OtherType>
	AutoPtr& operator=(AutoPtr<OtherType> &a) 
	{
		if(Get() != a.Get())
		{
			delete mPtr;
			mPtr = a.Release();
		}
		return *this;
	}

	/*!
	 * \brief Dereferencing operator - works exactly like a plain pointer's 
	 * operator*.
	 *
	 * \return A reference to the pointed-to object.
	 */
	Type& operator*() const  { return *mPtr; }

	/*!
	 * \brief Pointer-to-member dereferencing operator - works exactly like a 
	 * plain pointer's operator->.
	 *
	 * \return Pointer to member.
	 */
	Type* operator->() const  { return mPtr; }

	/*!
	 * \brief Get the plain pointer back.  This does not affect the ownership.
	 *
	 * This AutoPtr will retain ownership of the pointer, the client must not
	 * delete the pointer.
	 *
	 * \return The contained, plain pointer.
	 */
	Type* Get() const { return mPtr; }

	/*!
	 * \brief Relinquish ownership of the pointed-to object to the caller.
	 *
	 * After a call to release, this AutoPtr will no longer own the pointed-to
	 * object and will point to NULL.  This AutoPtr may no longer be used
	 * to access the pointed-to object.
	 * \return the contained, plain pointer, for which the caller is now
	 * responsible.
	 */
	Type* Release() 
	{
		Type *temp = mPtr;
		mPtr = NULL;
		return temp;
	}

	/*!
	 * \brief Assume ownership of a new object, any existing pointer will be
	 * deleted.
	 *
	 * \param p New pointer whose object this AutoPtr will now own.
	 */
	void Reset(Type *p = NULL) 
	{
		if(p != mPtr)
		{
			delete mPtr;
			mPtr = p;
		}
	}

	/*!
	 * \brief Addition to the textbook interface.  Equivalent to get() == NULL.
	 *
	 *	\return true if this AutoPtr points to NULL.  False, otherwise.
	 */
	bool IsNull() const  { return (NULL == mPtr); }

	/*!
	 * \brief Construct from an AutoPtrRef.
	 *
	 * This may be done implicitly or explicitly.  The Ref object exists to 
	 * avoid temporarily needing to have two AutoPtrs own the same object.
	 * \param AutoPtrRef helper object.
	 */
	AutoPtr(AutoPtrRef<Type> &ref) : mPtr(ref.mPtr) {}

	/*!
	 * \brief Convert to an AutoPtrRef.
	 */
	template<typename OtherType> operator AutoPtrRef<OtherType>() 
	{
		return AutoPtrRef<OtherType>(this->Release());
	}

	/*!
	 * \brief Destructive copy-convert allowing for cast of the pointer type.
	 */
	template<typename OtherType> operator AutoPtr<OtherType>() 
	{
		return AutoPtr<OtherType>(this->Release());
	}
};

} // end of namespace Util
}
#endif