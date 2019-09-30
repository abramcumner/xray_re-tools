#ifndef __MPxUITableControl_h
#define __MPxUITableControl_h
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
// CLASS:    MPxUITableControl
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MPxUIControl.h>

// ****************************************************************************
// DECLARATIONS


class QHmayaTable;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxUITableControl)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for creating new spreadsheet controls.
/*!
MPxUITableControl is the base class for creating new spreadsheet like
interfaces.  Every UI control has an associated command that is
'in-charge' of that control.  Therefore, when creating a new table
control, you will need to create an associated
MPxControlCommand. Please refer to the MPxControlCommand documentation
for details on how to use these class together.

At a minimum, when creating a new table control, you must overload
getCell( ... ).  Other methods available for overloading are getLabel(...),
allowEdit( ... ), allowSelection( ... ), and collapseOrExpandRow( ... ).
*/
class OPENMAYAUI_EXPORT MPxUITableControl : public MPxUIControl
{
public:
	MPxUITableControl				( MPxControlCommand & );

	virtual ~MPxUITableControl		();

	//! Types of labels available in the control.
	enum MLabelType {
		kNoLabel,	//!< \nop
		kRowLabel,	//!< \nop
		kColumnLabel,	//!< \nop
		kAllLabels	//!< \nop
	};

	MStatus				redrawLabels( MLabelType lt = kAllLabels );

	MStatus				redrawCells();

	MStatus				addToSelection( unsigned int row, unsigned int col );

	MStatus				removeFromSelection( unsigned int row,
											 unsigned int column );

	MStatus				clearSelection( );

	MStatus				setSelection( unsigned int row, unsigned int column );

	MStatus				setSelection( unsigned int firstRow,
									  unsigned int lastRow,
									  unsigned int firstCol,
									  unsigned int lastCol );

	MStatus				setNumberOfRows( unsigned int count );

	unsigned int		numberOfRows( MStatus *ReturnStatus = NULL );

	MStatus				setNumberOfColumns( unsigned int count );

	unsigned int		numberOfColumns( MStatus *ReturnStatus = NULL ) const;


	bool				suspendUpdates( bool update, MStatus *ReturnStatue = NULL );

	bool				isSelected( unsigned int row, unsigned int col,
									MStatus *status = NULL );

	virtual bool		collapseOrExpandRow( unsigned int row );

	static const char*	className();


protected:
	virtual MString		cellString( unsigned int r, unsigned int c, bool &ValidCell);
	virtual MString		labelString( MLabelType labelType, unsigned int n);

//BEGIN_NO_SCRIPT_SUPPORT:
	//!     NO SCRIPT SUPPORT
	virtual bool		getCell( unsigned int r, unsigned int c, MString &value );
	//!     NO SCRIPT SUPPORT
	virtual void		getLabel( MLabelType labelType, unsigned int n, MString &value );
//END_NO_SCRIPT_SUPPORT:

	virtual bool		allowEdit( ) const ;

	virtual bool		allowSelection( int top, int left, int bottom, int right );

	virtual bool		getCellColor( unsigned int r, unsigned int c, int &red, int &green, int &blue );

private:
	MPxUITableControl();
	MPxUITableControl( const MPxUITableControl & );
	MPxUITableControl & operator=( const MPxUITableControl & );

	friend class MPxControlCommand;
	friend class ::QHmayaTable;

	void *		instance;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* _cplusplus */
#endif /* MPxUITableControl */

