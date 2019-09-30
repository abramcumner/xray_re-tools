#ifndef _MExternalDropData
#define _MExternalDropData
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

#if defined __cplusplus

// ****************************************************************************


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************

//! \ingroup OpenMayaUI
//! \brief Drag-and-drop data, used with MExternalDropCallback.
/*!
MExternalDropData is the data that a drag-and-drop operation carries if dragging from an external application and dropping onto Maya. 

It typically arrives from a MExternalDropCallback callback method.
*/
class OPENMAYAUI_EXPORT MExternalDropData
{
public:
	enum MouseButtons {
		kLeftButton		= 0x00000001,
		kRightButton	= 0x00000002,
		kMidButton		= 0x00000004,
		kMiddleButton	= kMidButton
	};

	enum KeyboardModifiers {
		kNoModifier			= 0x00000000,
		kShiftModifier		= 0x02000000,
		kControlModifier	= 0x04000000,
		kAltModifier		= 0x08000000
	};

	bool hasText() const;
	MString text() const;

	bool hasUrls() const;
	MStringArray urls() const;

	bool hasHtml() const;
	MString html() const;

	bool hasColor() const;
	MColor color() const;

	bool hasImage() const;
	MImage image() const;

	MStringArray formats() const;
	bool hasFormat( const MString& format ) const;

	unsigned int dataSize( const MString& format ) const;
BEGIN_NO_SCRIPT_SUPPORT:
	MStatus copyData( const MString& format, void* buffer, unsigned int bufferSize ) const;
END_NO_SCRIPT_SUPPORT:

	int mouseButtons() const;
	int keyboardModifiers() const;

protected:

private:
	MExternalDropData( const void* data, int mouseButtons, int keyModifiers );

	const void*	fData;
	int fMouseButtons;
	int fKeyModifiers;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MExternalDropData */
