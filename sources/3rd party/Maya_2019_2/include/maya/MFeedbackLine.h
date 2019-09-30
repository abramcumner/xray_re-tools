#ifndef _MFeedbackLine
#define _MFeedbackLine
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
// CLASS:    MFeedbackLine
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// CLASS DECLARATION (MFeedbackLine)

//! \ingroup OpenMayaUI
//! \brief Feedback line 
/*!
The MFeedbackLine class is used to display information back to the user.
The format for the feedback line which indicates the number and type
of the arguments should be set with the setFormat method.
The values of the arguments should be set using the setValue method.
*/
class OPENMAYAUI_EXPORT MFeedbackLine {
public:
	static MStatus	setFormat		(const MString &format);
	static MStatus	setTitle		(const MString &title);
	static MStatus	setValue		(short index, double value);
	static void		clear			();
	static bool		showFeedback	();
	static void		setShowFeedback	(bool showFeedback);

	static const char *className();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFeedbackLine */
