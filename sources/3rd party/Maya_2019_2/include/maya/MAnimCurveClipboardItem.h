#ifndef _MAnimCurveClipboardItem
#define _MAnimCurveClipboardItem
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
// CLASS:    MAnimCurveClipboardItem
//
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER HILES


#include <maya/MFnAnimCurve.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

//	CLASS DECLARATION (MAnimCurveClipboardItem)

//! \ingroup OpenMayaAnim
//! \brief	Wrapper for a clipboard item. 
/*!
    This class provides a wrapper for a clipboard item.  Common convenience
    functions are available, and the implementation is compatible with the
    internal Maya implementation so that it can be passed efficiently between
    plugins and internal maya data structures.
*/
class OPENMAYAANIM_EXPORT MAnimCurveClipboardItem
{
public:
					MAnimCurveClipboardItem();
					MAnimCurveClipboardItem( const MAnimCurveClipboardItem &r);
					~MAnimCurveClipboardItem();

	const MObject		animCurve( MStatus * ReturnStatus = NULL ) const;
	MStatus				getAddressingInfo( unsigned int &rowCount,
										   unsigned int &childCount,
										   unsigned int &attributeCount) const;
	const MString &		fullAttributeName( MStatus * ReturnStatus=NULL ) const;
	const MString &		leafAttributeName( MStatus * ReturnStatus=NULL ) const;
	const MString &		nodeName( MStatus * ReturnStatus=NULL ) const;
	MFnAnimCurve::AnimCurveType	animCurveType( MStatus
												* ReturnStatus=NULL ) const;
	MStatus				setAnimCurve( const MObject & curve );
	MStatus				setAddressingInfo( unsigned int rowCount,
										   unsigned int childCount,
										   unsigned int attributeCount );
	MStatus				setNameInfo( const MString & nodeName,
									 const MString & fullName,
									 const MString & leafName );
	MAnimCurveClipboardItem &operator =  (const MAnimCurveClipboardItem &from);
	bool	 				operator == (const MAnimCurveClipboardItem &rhs) const;

	static const char*				className();

protected:
// No protected members

private:
	void *							fCurve;
	MFnAnimCurve::AnimCurveType		fAnimCurveType;

	unsigned int						fRowCount;
	unsigned int						fChildCount;
	unsigned int						fAttrCount;
	MString							fFullAttrName;
	MString							fLeafAttrName;
	MString							fNodeName;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAnimCurveClipboardItem */
