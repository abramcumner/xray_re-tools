#ifndef _MAtomic
#define _MAtomic
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MAtomic
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// ****************************************************************************
// CLASS DECLARATION (MAtomic)

//! \ingroup OpenMaya
//! \brief Methods for atomic operations.
/*!
  
    The MAtomic class implements several cross-platform atomic
    operations which are useful when writing a multithreaded
    application. Atomic operations are those that appear to happen as
    a single operation when viewed from other threads.

    As a usage example, during reference counting in an SMP
    environment, it is important to ensure that decrementing and
    testing the counter against zero happens atomically. If coded this
    way:

          if (--counter == 0) {}

    then another thread could modify the value of counter between the
    decrement and the if test. The above code would therefore get the
    wrong value. This class provides a routine to perform the
    decrement and return the previous value atomically, so the above
    snippet could be written as:

          if (MAtomic::preDecrement(&counter) == 0) {}

*/

#if defined(_WIN32)
#include <intrin.h>

#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedExchange) 
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedIncrement) 
#pragma intrinsic(_InterlockedDecrement)

#define FORCE_INLINE __forceinline

#elif defined(__linux__) || defined (__APPLE__)

#define FORCE_INLINE inline __attribute__((always_inline))

// implement inline assembly for Linux 32/64 compatible with gcc and
// icc. gcc has atomic intrinsics but icc does not currently support
// them, so we do it all with assembler.
//
#define MAYA_LINUX_ATOMICS(X)                                                      \
static FORCE_INLINE int atomicCmpXChg(volatile int *value, int comparand, int newValue ) \
{                                                                                  \
    int result;                                                                    \
    __asm__ __volatile__("lock\ncmpxchg" X " %2,%1"                                \
                          : "=a"(result), "=m"(*value)                             \
                          : "q"(newValue), "0"(comparand)                          \
                          : "memory");                                             \
    return result;                                                                 \
}                                                                                  \
static FORCE_INLINE int atomicXAdd(volatile int *value, int addend)                      \
{                                                                                  \
    int result;                                                                    \
    __asm__ __volatile__("lock\nxadd" X " %0,%1"                                   \
                          : "=r"(result), "=m"(*value)                             \
                          : "0"(addend)                                            \
                          : "memory");                                             \
   return result;                                                                  \
}                                                                                  \
static FORCE_INLINE int atomicXChg(volatile int *value, int newValue)                    \
{                                                                                  \
    int result;                                                                    \
    __asm__ __volatile__("lock\nxchg" X " %0,%1"                                   \
                          : "=r"(result), "=m"(*value)                             \
                          : "0"(newValue)                                          \
                          : "memory");                                             \
   return result;                                                                  \
}

MAYA_LINUX_ATOMICS("")

#else
#error Undefined platform
#endif // __linux__


OPENMAYA_MAJOR_NAMESPACE_OPEN

class OPENMAYA_EXPORT MAtomic
{
public:

/*!
   Increment variable, return value after increment
   \param[in] variable Value to be modified

   \return. Variable value after incrementing
*/
static FORCE_INLINE int preIncrement(volatile int *variable)
{
#if defined(_WIN32)
    return (_InterlockedIncrement((long*)variable));
#else 
	return( atomicXAdd( variable, 1 ) + 1 );
#endif
}

/*!
   Increment variable, return value before increment
   \param[in] variable Value to be modified

   \return. Variable value before incrementing
*/
static FORCE_INLINE int postIncrement(volatile int *variable)
{
#if defined(_WIN32)
    return (_InterlockedExchangeAdd((long*)variable,1));
#else
	return( atomicXAdd( variable, 1 ) );
#endif
}

/*!
   Increment variable by incrementValue, return value before increment.
   \param[in] variable Value to be modified
   \param[in] incrementValue Value by which to increment variable

   \return. Variable value before incrementing
*/
static FORCE_INLINE int increment(volatile int *variable, int incrementValue)
{
#if defined(_WIN32)
    return (_InterlockedExchangeAdd((long*)variable, incrementValue));
#else
	return( atomicXAdd( variable, incrementValue ) );
#endif
}

/*!
   Decrement variable, return value after increment
   \param[in] variable Value to be modified

   \return. Variable value after decrementing
*/
static FORCE_INLINE int preDecrement(volatile int *variable)
{
#if defined(_WIN32)
    return (_InterlockedDecrement((long*)variable));
#else
	return( atomicXAdd( variable, -1 ) - 1 );
#endif
}

/*!
   Decrement variable, return value before decrement
   \param[in] variable Value to be modified

   \return. Variable value before decrementing
*/                                                               
static FORCE_INLINE int postDecrement(volatile int *variable)
{
#if defined(_WIN32)
    return (_InterlockedExchangeAdd((long*)variable, -1));
#else
	return( atomicXAdd( variable, -1 ) );
#endif
}

/*!
   Decrement variable by decrementValue, return value before decrement.
   \param[in] variable Value to be modified
   \param[in] decrementValue Value by which to decrement variable

   \return. Variable value before decrementing
*/
static FORCE_INLINE int decrement(volatile int *variable, 
                                 int decrementValue)
{
#if defined(_WIN32)
    return (_InterlockedExchangeAdd((long*)variable, -decrementValue));
#else 
	return( atomicXAdd( variable, -decrementValue ) );
#endif
}

/*!
   Set variable to newValue, return value of variable before set.
   \param[in] variable Value to be modified
   \param[in] newValue Value to which to set variable

   \return. Variable value before set
*/
static FORCE_INLINE int set(volatile int *variable, int newValue)
{
#if defined(_WIN32)
    return (_InterlockedExchange((long*)variable, newValue));
#else
	return atomicXChg( variable, newValue );
#endif
}

/*!
   Compare variable with compareValue and if the values are equal,
   sets *variable equal to swapValue. The result of the comparison is
   returned, true if the compare was sucessful, false otherwise.

   \param[in] variable First value to be compared
   \param[in] compareValue Second value to be compared
   \param[in] swapValue Value to set *variable to if comparison is successful

   \return. True if variable equals compareValue, otherwise false
*/
static FORCE_INLINE int compareAndSwap(volatile int *variable, 
							 int compareValue, 
							 int swapValue)
{
#if defined(_WIN32)
    return (_InterlockedCompareExchange((long*)variable, swapValue, compareValue)
			== compareValue);
#else
	return( atomicCmpXChg( variable, compareValue, swapValue ) == compareValue);
#endif
}

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAtomic */
