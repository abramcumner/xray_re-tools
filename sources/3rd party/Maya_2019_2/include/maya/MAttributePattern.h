#ifndef _MAttributePattern
#define _MAttributePattern
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
// CLASS:    MAttributePattern
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MString.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAttributePattern)

//! \ingroup OpenMaya
//! \brief The pattern of an attribute tree that can be applied to nodes or node classes
/*!
Class that provides a pattern of attributes to be applied to nodes as
dynamic attributes or to node classes as extension attributes.

An attribute pattern is a pattern describing an attribute tree. In the simplest
implementation it can just store an internal copy of the tree to be applied.
The code to apply the attribute patterns will use the rootAttr() and
rootAttrCount() methods to iterate through the list of top level attributes to
be applied.  It presumes that if they are compounds then their children will
already be parented properly.

\code
	for( unsigned int i=0; i < tmpl.rootAttrCount(); ++i )
	{
		applyAttributesToNodes( tmpl.rootAttr(i), someNode );
	}
\endcode

When creating the pattern from inside MPxAttributePatternFactory you add
only the root level attributes, i.e. the attributes without parents. As
a simple example if your pattern factory has data that specifies creating
a pattern with one integer attribute and one compound with three float
children the code would like something like this.

\code
	MStatus status;

	MFnCompoundAttribute parentAttr;
	MObject parentObj1 = parentAttr.create( "parent", "par", &status );

	MFnNumericAttribute childAttrX;
	MFnNumericAttribute childAttrY;
	MFnNumericAttribute childAttrZ;
	MObject childObjX = childAttrX.create( "childX", "cx" MFnNumericData::kFloat, 0, &status );
	MObject childObjY = childAttrY.create( "childY", "cy" MFnNumericData::kFloat, 0, &status );
	MObject childObjZ = childAttrZ.create( "childZ", "cz" MFnNumericData::kFloat, 0, &status );
	parentAttr.addChild( childObjX );
	parentAttr.addChild( childObjY );
	parentAttr.addChild( childObjZ );

	MFnNumericAttribute otherParent;
	MObject parentObj2 = otherParent.create( "intValue", "iv" MFnNumericData::kInt, 0, &status );

	// Note that you only add the top-level attributes to the pattern,
	// not the children.
	MAttributePattern* tmpl = new MAttributePattern("myPattern");
	return ( (tmpl->addRootAttr( parentObj1 ) == MS::kSuccess)
		&&   (tmpl->addRootAttr( parentObj2 ) == MS::kSuccess) );
\endcode

*/
class OPENMAYA_EXPORT MAttributePattern
{
public:
	MAttributePattern	();
	MAttributePattern	( const MString& name );
	MAttributePattern	( const MAttributePattern& rhs );
	MAttributePattern& operator=( const MAttributePattern& rhs );
	bool operator==( const MAttributePattern& rhs ) const;
	~MAttributePattern	();

	static MAttributePattern*	findPattern		(const MString& name);
	static unsigned int			attrPatternCount	();
	static MAttributePattern*	attrPattern		(unsigned int n);

public:
	MString			name			(MStatus* ReturnStatus = NULL) const;
	unsigned int 	rootAttrCount	(MStatus* ReturnStatus = NULL) const;
	MObject	  		rootAttr		(unsigned int idx, MStatus* ReturnStatus = NULL) const;

	MStatus	  		removeRootAttr	(unsigned int idx);
	MStatus	  		removeRootAttr	(const MObject& attr);
	MStatus			addRootAttr		(const MObject& attr);

	static const char* className();

protected:
	void*			fAttributePattern;
	MAttributePattern	( void* mayaAttributePattern );
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAttributePattern */
