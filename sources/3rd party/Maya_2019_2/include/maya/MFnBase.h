#ifndef _MFnBase
#define _MFnBase

// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

// ==========================================================================
//
// CLASS:    MFnBase
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MTypes.h>


// ****************************************************************************
// DECLARATIONS

class MPtrBase;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnBase)

//! \ingroup OpenMaya MFn
//! \brief  Function Set Base Class. 
/*!
  Defines the interface for the API RTTI and Maya Object attachment
  methods common to all Function Set Classes.

  Implements the Function Set compatibility test methods for all
  Function Sets.

  The API provides access to Maya Objects as generic MObjects.  Under
  the C++ class scheme MObjects simply have type MObject with no base
  or derived hierarchy.  However, MObjects may be many different types
  of model or scene objects, from Attributes to Transforms, within
  Maya.  This allows many different types of objects to be moved
  across or accessed through the API without undue concern on the part
  of plug-in developers for the type of object being manipulated.

  Since it is impractical to provide every applicable method on
  MObjects, the methods applicable to Maya Objects are encapsulated in
  Function Sets.  Function Sets can be attached to compatible MObjects
  to provide an external interface to methods appropriate to that
  MObject.  All Function Sets provide a method setObject() which
  attaches them to the specified Maya Object.  Additionally, most
  Function Sets provide a version of their constructor which attaches
  the new Function Set to a specified MObject.

  The link between an MObject and the role it performs as a Maya scene
  or model object, as well as the compatibility between MObjects and
  Function Sets is managed by the API Run-Time Type Identification
  (RTTI) system.  The two key concepts of this system are the Maya
  Object type and the Function Set type (both are MFn::Type).  All
  MObjects have one and only one Maya Object type.  All Function Sets
  have one and only one Function Set type.  MObjects may, however, be
  compatible with many types of Function Sets.  This compatibility
  follows the class hierarchy of the Function Sets.  Thus an MObject
  with Maya Object type MFn::kNurbsSurface would be compatible with
  MFnNurbsSurface, MFnDagNode, MFnDependencyNode, MFnNamedObject and
  MFnBase.

  Any MObject can be queried with the MObject::hasFn() method to
  determine if it is comaptible with a given Function Set.

  The purpose of the Base Function Set (MFnBase) is to provide methods for
  determining whether or not a Function Set is compatible with a given Maya
  Object (MObject) type (MFn::Type) or a specific MObject.  See:
                    MFnBase::hasObj(MFn::Type)
							and
					MFnBase::hasObj(const MObject &)

  As well, this Function Set provides virtual methods for determining the type
  of a Function Set under the API RTTI system.
*/
class OPENMAYA_EXPORT MFnBase
{
public:
 	virtual				~MFnBase();
	virtual MFn::Type	type() const;
	bool				hasObj( MFn::Type ) const;
	bool				hasObj( const MObject & ) const;
	MObject				object( MStatus* ReturnStatus = NULL ) const;
 	virtual MStatus		setObject( MObject & object );

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	virtual MStatus		setObject( const MObject & object );

END_NO_SCRIPT_SUPPORT:

	static const char* className();

protected:
	MFnBase();
	void				setPtr( MPtrBase* );
	void				setPtr( const MPtrBase* );
	void				setPtrNull();

	virtual bool		objectChanged( MFn::Type, MStatus * );


 	MPtrBase* 			f_ptr;	    // initialized/set with volatile
 	const MPtrBase*		f_constptr;	// initialized/set with volatile/const

	// Specific for function sets which use internally reference
	// counted objects!  Only those function sets which use these
	// objects need to set this to true.  By default it is set to
	// false.
	//
	// Classes which do not use internally reference counted objects
	// must not tamper with this boolean value. Doing so can cause
	// crashes when dealing with manually deleted objects.
	//
	bool				fIsReferenceCounted;

private:
    MFnBase( const MFnBase &rhs );
	MFnBase & operator=( const MFnBase & );
	MFnBase * operator& () const;
};

// ****************************************************************************
#define declareMinimalMFn( MFnClass )							 	\
	public:														 	\
		virtual MFn::Type type() const;							 	\
		virtual ~MFnClass();									 	\
																 	\
		static const char* className();							 	\
																 	\
	private:													 	\
        MFnClass( const MFnClass &rhs );                            \
		MFnClass & operator=( const MFnClass & );				 	\
		MFnClass * operator& () const

#define declareMFn( MFnClass, MFnParentClass )					 	\
	declareMinimalMFn( MFnClass );								 	\
	public:	        											 	\
		MFnClass() {};											 	\
		MFnClass( MObject & object, MStatus * ReturnStatus = NULL )

#define declareMFnConstConstructor( MFnClass, MFnParentClass ) 		\
		MFnClass( const MObject & object, MStatus * ReturnStatus = NULL )

// Standard MFn class interface *without* the class constructor. This
// is to allow derived MFn classes to optionally specify their own
// custom constructor. For example if they wanted to declare that they
// use internally reference counted objects, they could set fIsReferenceCounted
// to true inside their own custom constructor.
//
#define declareMFnNoVoidConstructor( MFnClass, MFnParentClass )		\
	declareMinimalMFn( MFnClass );									\
	public:															\
		MFnClass( MObject & object, MStatus * ReturnStatus = NULL )

#define declareMFnNoVoidConstructorConstConstructor( MFnClass, MFnParentClass )			\
		MFnClass( const MObject & object, MStatus * ReturnStatus = NULL )

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MFnBase */

