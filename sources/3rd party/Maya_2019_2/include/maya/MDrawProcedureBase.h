#ifndef _MDrawProcedureBase
#define _MDrawProcedureBase
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
// CLASS:    MDrawProcedureBase
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MDrawProcedureBase)
//
// This class provides an interface through which a plugin can
// implement a class to provide custom hardware drawing effects.
//
// The derived class needs to be registered with using <to be completed>
// Please refer to documentation of <> for more details.
//
// Each procedure has a user defined string name and can be enabled or
// disabled.  There is also a logical "call location" which defines
// when within a rendering loop that the draw procedure will be
// called. Name, enabling and call location must be defined.
//
// Additionally, the procedure can define a desired sequence
// number. All draw procedures are sorted by sequence
// number. Procedures with lower sequence number will be invoked
// first. The number 0 is the lowest supported sequence number and is
// the default.  When more than one procedure has a sequence number,
// the order of registration will determine the invocation order.
//
// All derived classes must over the execute() method. This is the
// method that will be called by the hardware renderer to which the
// procedure is attached.  The call will only be made if the procedure
// is enabled.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDES


#include <maya/MString.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDrawProcedureBase)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
\deprecated
Use MHWRender::MRenderOverride instead.

This class provides an interface through which a plug-in can be writen
to implement a class to provide custom hardware drawing effects.

The derived class can be added, removed, or reordered in a list of
draw procedures used by the hardware renderer.  Please refer to
documentation for MHardwareRenderer for more details.

Each procedure has a user defined string name and can be enabled or
disabled. Name, and enabling methods must be defined.

All derived classes must over the execute() method. This is the method
that will be called by the hardware renderer to which the procedure is
attached. The call will only be made if the procedure is enabled.
*/
class OPENMAYARENDER_EXPORT MDrawProcedureBase
{
public:
	// constructor
	MDrawProcedureBase(const MString &name);

	// destructor
	virtual	~MDrawProcedureBase();

	// Method called to perform drawing effect. Derived classes must implement this.
	virtual bool execute() = 0;

	// Set the procedure to be enabled or not.
	void		setEnabled(bool value);

	// Returns if the procedure is enabled.
	bool		enabled() const ;

	//	Set the name of the procedure
	void		setName( const MString &name );

	// Get the name of the procedure
	const MString &name() const;

protected :
	// Protected default constructor
	MDrawProcedureBase();

	// Name of the procedure
	MString		fName;

	// Enable / disable toggle
	bool		fEnabled;

private:
	// No private members
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDrawProcedureBase */
