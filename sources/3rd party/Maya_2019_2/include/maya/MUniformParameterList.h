#ifndef _MUniformParameterList
#define _MUniformParameterList
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
// CLASS:    MUniformParameter
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MUniformParameter)

//! \ingroup OpenMayaRender
//! \brief Uniform Parameter.
/*!
  MUniformParameterArray specify the list of uniform shader parameters
  used by a hardware shader, allowing Maya to handle setting up the
  node and user interfaces to the data, the population and access of
  cached data, etc.
*/
class OPENMAYARENDER_EXPORT MUniformParameterList
{
public:

	MUniformParameterList();

	~MUniformParameterList();

	void			append( const MUniformParameter& element);

	int				length() const;

	void			setLength( int n);

	MUniformParameter getElement( int n) const;

	void			setElement( int n, const MUniformParameter& p);

	const MUniformParameterList& operator=( const MUniformParameterList& other);

protected:

	MUniformParameterList( void* ptr, bool ownsData);

	void*			_ptr;
	bool			_ownsData;

private:
// No private members

	friend class MPxHardwareShader;
};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MUniformParameterList
