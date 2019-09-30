#ifndef _MAttributeSpec
#define _MAttributeSpec
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
// CLASS:    MAttributeSpec
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAttributeSpec)

//! \ingroup OpenMaya
//! \brief An attribute specification. 
/*!
Class that encapsulates component/attribute information
for generating selection items.

This class is used by MPxSurfaceShape::matchComponent for validating
attributes specified as strings and converting that specification to
a component object.

The attribute specification ".foo.bar[3].x" would be
expressed like this (using a pseudo-structure description):

\code
    MAttributeSpecArray[0..3] {
        MAttributeSpec[0] {
            name      = "foo"
            dimension = 0
        }
        MAttributeSpec[1] {
            name      = "bar"
            dimension = 1
            MAttributeIndex[0] = {
                type      = kInteger
                isRange   = false
                isBounded = true
                value     = 3
            }
        }
        MAttributeSpec[2] {
            name      = "x"
            dimension = 0
        }
    }
\endcode
*/
class OPENMAYA_EXPORT MAttributeSpec {
public:
	MAttributeSpec();
BEGIN_NO_SCRIPT_SUPPORT:
	MAttributeSpec( const char * name);
END_NO_SCRIPT_SUPPORT:
	MAttributeSpec( const MString & name);
	MAttributeSpec( const MAttributeSpec & other);
	~MAttributeSpec();

public:
	const MString 			name() const;
	int						dimensions() const;
	void					setName( const MString & name);
	void					setDimensions( int value);

public:
	MAttributeSpec &		operator =(const MAttributeSpec & rhs);
	const MAttributeIndex	operator [](int index) const;
	bool					operator==(const MAttributeSpec & other) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	//! \noscript
	MAttributeIndex 		operator[](int index);

END_NO_SCRIPT_SUPPORT:

	static const char* className();

private:
	MAttributeSpec( void* );
	void*	data;
	bool	own;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAttributeSpec */
