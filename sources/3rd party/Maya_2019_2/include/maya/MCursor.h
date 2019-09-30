#ifndef _MCursor
#define _MCursor
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
// CLASS:    MCursor
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MCursor)

//! \ingroup OpenMayaUI
//! \brief Manipulate Cursors 
/*!
  The MCursor class implements a cursor class, and is used to pass
  all cursor arguments to Maya API methods.

  The cursor image and mask are stored in xbm format to the constructor
  along with the cursor dimensions and the cursor hotspot.
*/
class OPENMAYAUI_EXPORT MCursor
{
public:
#ifdef _WIN32
	//! Windows-only.
				MCursor( LPCSTR pszResource );
#endif
				MCursor(short width,
						short height,
						short hotSpotX,
						short hotSpotY,
						unsigned char * bits,
						unsigned char * mask );
				MCursor(const MCursor& other);
	        	~MCursor();

	MCursor &	operator=(const MCursor &);
	bool		operator==(const MCursor &) const;
	bool		operator!=(const MCursor &) const;
	//! Maya default cursor, the left arrow.
	static const	MCursor	defaultCursor;
	//! '+' cursor.
	static const	MCursor	crossHairCursor;
	//! '+' cursor with double lines.
	static const	MCursor	doubleCrossHairCursor;
	//! Wedge-shaped arrow pointing left.
	static const	MCursor	editCursor;
	//! Pencil shape.
	static const	MCursor	pencilCursor;
	//! Open hand shaped cursor.
	static const	MCursor	handCursor;

protected:
// No protected members

private:
				MCursor();
				MCursor(const void *);
	const void*	apiData;
	const void* apiData2;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MCursor */
