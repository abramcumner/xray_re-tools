#ifndef _MAnimCurveClipboardItemArray
#define _MAnimCurveClipboardItemArray
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
// CLASS:    MAnimCurveClipboardItemArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MAnimCurveClipboardItem.h>
#include <maya/MStatus.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAnimCurveClipboardItemArray)

//! \ingroup OpenMayaAnim
//! \brief  Array of MAnimCurveClipboardItem data type 
/*!
	This class implements an array of MAnimCurveClipboardItems.  Common
	convenience functions are available, and the implementation is compatible
	with the internal Maya implementation so that it can be passed efficiently
	between plugins and internal maya data structures.
*/
class OPENMAYAANIM_EXPORT MAnimCurveClipboardItemArray
{

public:
					MAnimCurveClipboardItemArray();
					MAnimCurveClipboardItemArray(
									const MAnimCurveClipboardItemArray& other );
					MAnimCurveClipboardItemArray( unsigned int initialSize,
								const MAnimCurveClipboardItem &initialValue );
					~MAnimCurveClipboardItemArray();
 	const MAnimCurveClipboardItem&		operator[]( unsigned int index ) const;
 	MStatus			set( const MAnimCurveClipboardItem& element,
						 unsigned int index );
	MStatus			setLength( unsigned int length );
 	unsigned int		length() const;
 	MStatus			remove( unsigned int index );
 	MStatus			insert( const MAnimCurveClipboardItem & element,
							unsigned int index );
 	MStatus			append( const MAnimCurveClipboardItem & element );
 	MStatus			clear();
	void			setSizeIncrement ( unsigned int newIncrement );
	unsigned int		sizeIncrement () const;
	bool			isValid( unsigned int & failedIndex ) const;

BEGIN_NO_SCRIPT_SUPPORT:

    //!	NO SCRIPT SUPPORT
	MAnimCurveClipboardItemArray(
									const MAnimCurveClipboardItem src[],
									unsigned int count );
    //!	NO SCRIPT SUPPORT
	MStatus			get( MAnimCurveClipboardItem array[] ) const;

 	//!	NO SCRIPT SUPPORT
 	MAnimCurveClipboardItem&			operator[]( unsigned int index );

END_NO_SCRIPT_SUPPORT:

	MAnimCurveClipboardItemArray&	operator = ( const MAnimCurveClipboardItemArray& other );
 	MStatus							copy( const MAnimCurveClipboardItemArray& source );
	
	static const char*				className();

protected:
// No protected members

private:
	bool							validate( unsigned int & index,
											  unsigned int rowCount ) const;

 	void*							fArray;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAnimCurveClipboardItemArray */
