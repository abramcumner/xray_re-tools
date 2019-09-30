#ifndef _MDGContextGuard
#define _MDGContextGuard
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
// CLASS:    MDGContextGuard
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDGContextGuard)

//! \ingroup OpenMaya
/*!
	\class MDGContextGuard
	\brief	Scope object used to modify current evaluation context in a code block

	Some API methods can have a context passed in to them. In those cases the
	current context will be temporarily changed to the passed context for evaluation
	purposes.

	A better method for longer sequences of evaluation in an alterative context is
	to create a guard in a block of code that makes a bunch of calls, and then use
	the context-free methods.

	\code
		// Example with a temporary context
		some_code_using_current_context();
		{
			MDGContextGuard contextGuard( MDGContext( 3.0 ) );
			myPlug.getValue( myBool );
			myPlug.getValue( myInt );
		}
	\endcode

	This is equivalent to:

	\code
		some_code_using_current_context();
		myPlug.getValue( MDGContext( 3.0 ), myBool );
		myPlug.getValue( MDGContext( 3.0 ), myInt );
	\endcode

	\note Guard objects can be nested
*/
class OPENMAYA_EXPORT MDGContextGuard
{

#if (MAYA_API_VERSION > 20180000)
	char data[24];
#else
	char data[8];	// Maya 2018 API requires 8 bytes storage
#endif

public:
	~MDGContextGuard();
	MDGContextGuard	(const MDGContext& tempContext);

private:
	// Force the one supported constructor
	MDGContextGuard();
	MDGContextGuard(const MDGContextGuard&);
};

OPENMAYA_NAMESPACE_CLOSE

#endif // _MDGContextGuard

