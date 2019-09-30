#ifndef _MSelectInfo
#define _MSelectInfo
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
// CLASS:    MSelectInfo
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MDrawInfo.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MSelectInfo)

//! \ingroup OpenMayaUI
//! \brief Selection state information used in MPxSurfaceShapeUI::select
/*!
MSelectInfo is used with user defined shape selection and is passed
as an argument to the MPxSurfaceShapeUI::select method.
This class encapsulates all the selection state information for
selected objects.
*/

class OPENMAYAUI_EXPORT MSelectInfo : public MDrawInfo
{
public:
	MSelectInfo();
	MSelectInfo( const MSelectInfo& in );
	~MSelectInfo();

public:

	// Public accessor methods

    M3dView			view();

	bool			singleSelection() const;
	bool			selectClosest() const;
    bool			selectable( MSelectionMask & mask ) const;
    bool			selectableComponent( bool displayed,
										 MSelectionMask & mask ) const;

    void		    selectRect(unsigned int &x, unsigned int &y,
                               unsigned int &width, unsigned int &height) const;
    
    bool			isRay() const;
	MMatrix			getAlignmentMatrix() const;
    void			getLocalRay( MPoint&, MVector & ) const;

    bool			selectForHilite( const MSelectionMask & ) const;

    bool			selectOnHilitedOnly() const;

	int				highestPriority() const;
	void			setHighestPriority( int );

    void			addSelection( const MSelectionList &item,
								  const MPoint &point,
								  MSelectionList &list,
								  MPointArray &points,
								  const MSelectionMask & mask,
								  bool isComponent );

	bool            setSnapPoint( const MPoint& point );

    MDagPath		selectPath() const;

	static const char*	className();

protected:
// No protected members

private:
    MSelectInfo( void* in );
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MSelectInfo */
