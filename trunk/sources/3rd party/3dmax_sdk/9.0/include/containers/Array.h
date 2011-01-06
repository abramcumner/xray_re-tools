//*****************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//*****************************************************************************/
/*==============================================================================

  file:     Array.h
  author:   Daniel Levesque
  created:  27 March 2006
  description:
    Array container.

==============================================================================*/
#ifndef _MAXSDK_ARRAY_H_
#define _MAXSDK_ARRAY_H_

#include "maxheap.h"
#include <memory.h>     
#include <stdlib.h>
#include <new>
#include "../assert1.h"
#include "../utilexp.h"
#include "../coreexp.h"

UtilExport void UtilOutOfMemoryException();
UtilExport void* UtilAllocateMemory(size_t);
UtilExport void UtilDeallocateMemory(void*);

namespace MaxSDK {

template <class T> 
class Array
: public MaxHeapOperators {
public:

	// Type of function to pass to sort().
	// Returns < 0 if elem 1 is smaller, > 0 if elem 1 is greater, or 0 if they're equivalent.
	typedef int( __cdecl *CompareFnc) ( const void *elem1, const void *elem2 );

	Array();
    Array(size_t initUsedLength, const T& defaultVal = T(), size_t initGrowLength = kDefaultGrowthLength);
    Array(const Array<T>&);
    ~Array();

    Array<T>& operator=(const Array<T>&);
    bool operator==(const Array<T>&) const;

    T& operator[](size_t);
    const T& operator[](size_t) const;

    const T& at(size_t index) const;
    T& at(size_t index);

    Array<T>& setAt(size_t index, const T& value);
    Array<T>& setAll(const T& value);

    T& first();
    const T& first() const;
    T& last();
    const T& last() const;

    size_t append(const T& value);
	Array<T>& append(const T* values, size_t count);
    Array<T>& append(const Array<T>& array);

    Array<T>& insertAt(size_t index, const T& value);
	Array<T>& insertAt(size_t index, const T* values, size_t count);

    Array<T>& removeAt(size_t index);
    bool remove(const T& value, size_t start = 0);
    Array<T>& removeFirst();
    Array<T>& removeLast();
    Array<T>& removeAll();
    Array<T>& removeSubArray(size_t startIndex, size_t endIndex);

    bool contains(const T& value, size_t start = 0) const;
    bool find(const T& value, size_t& foundAt, size_t start = 0) const;
    size_t find(const T& value) const;
    size_t findFrom(const T& value, size_t start) const;

    size_t length() const; // Used length.
    bool isEmpty() const;
    size_t lengthUsed() const;
    Array<T>& setLengthUsed(size_t, const T& defaultVal = T());
    size_t lengthReserved() const;
    Array<T>& setLengthReserved(size_t);

    size_t growLength() const;
    Array<T>& setGrowLength(size_t);

    Array<T>& reverse();
    Array<T>& swap(size_t i1, size_t i2);
	void sort(CompareFnc cmp);

    const T* asArrayPtr() const;
    T* asArrayPtr();

	bool isValidIndex(size_t) const;

protected:

	enum {
		kArrayGrowthThreshold = 0x10000,
		kDefaultGrowthLength = 8
	};

    T* mpArray;
    size_t mReservedLen;
    size_t mUsedLen;
    size_t mGrowLen;

	static void	quickSortRecursive(T* data, size_t first, size_t last, CompareFnc cmp);
	static void	handleOutOfMemory();

	static T* ArrayAllocate(size_t len);
	static void ArrayConstruct(T* arrayBegin, size_t len, const T& defaultVal);
	static void ArrayDeAllocate(T* arrayBegin);
	static void ArrayDestruct(T* arrayBegin, size_t len);
	static void ArrayCopy(T* pCopy, const T * pSource, size_t nCount);
	static void ArrayCopyOverlap(T* pCopy, const T * pSource, size_t nCount);
	static void ArrayCopyConstruct(T* pCopy, const T * pSource, size_t nCount);
};

#include "Array.inline.h"
#include "Array.imp.h"

} // namespace MaxSDK

#endif
