#ifndef _MArrayIteratorTemplate
#define _MArrayIteratorTemplate
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
// CLASS:	MArrayIteratorTemplate
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

#if !defined(SWIG)

template<typename ContainerType, typename ObjectTypeContained>
class MArrayConstIteratorTemplate
{
public:
	// Standards compliant typedefs
	typedef ObjectTypeContained value_type;
	typedef ObjectTypeContained& reference;


	explicit MArrayConstIteratorTemplate(const ContainerType& container, unsigned int index = 0)
		:_container(&const_cast<ContainerType&>(container)),
		_index(index)
	{}
	
	bool operator == (const MArrayConstIteratorTemplate& other)
	{
		bool sameContainers = (other._container == _container);
		return sameContainers && (_index == other._index);
	}

	bool operator != (const MArrayConstIteratorTemplate& other)
	{
		bool sameContainers = (other._container == _container);
		return (sameContainers != true) || (_index != other._index);
	}

	const ObjectTypeContained& operator*()
	{
		return (*_container)[_index];
	}

	const MArrayConstIteratorTemplate& operator++()
	{
		++_index;
		return *this;
	}

	const MArrayConstIteratorTemplate& operator--()
	{
		--_index;
		return *this;
	}

protected:
	MArrayConstIteratorTemplate(ContainerType& container, unsigned int index = 0)
		:_container(&container),
		_index(index)
	{}

protected:
	ContainerType*		_container;
	unsigned int		_index;
};

template <typename ContainerType, typename ObjectTypeContained>
class MArrayIteratorTemplate : public MArrayConstIteratorTemplate<ContainerType, ObjectTypeContained>
{
public:
	// Specify that the fully qualified name of the reference type above is called 'reference' here
	typedef typename MArrayConstIteratorTemplate<ContainerType, ObjectTypeContained>::reference reference;
	
	explicit MArrayIteratorTemplate(ContainerType& _array, unsigned int index = 0)
		: MArrayConstIteratorTemplate<ContainerType, ObjectTypeContained>(_array, index)
	{}

	// Shadow the const T&operator*() in base class.
	// Note that the [] operator in some of the 
	// M*Array classes have a variant that creates 
	// a local copy (e.g. MIntArray). However, the 
	// C++ standard requires that containers return 
	// either a const_reference or a reference, and 
	// in this case, we need a reference
	reference operator*() const
	{
		return (*this->_container)[this->_index];
	}
};

#endif /* SWIG */

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MIntArray */