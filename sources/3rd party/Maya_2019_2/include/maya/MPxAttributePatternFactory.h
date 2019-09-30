#ifndef _MPxAttributePatternFactory
#define _MPxAttributePatternFactory
//
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
// CLASS:    MPxAttributePatternFactory
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MApiNamespace.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// *****************************************************************************
// CLASS DECLARATION (MPxAttributePatternFactory)

//! \ingroup OpenMaya MPx
//! \brief Base class for user-defined attribute pattern factories.
/*!
	MPxAttributePatternFactory is the parent class of all user defined
	attribute pattern factories.

	This class can be used to implement new kinds of attribute pattern
	factories within Maya that behave in a similar manner to the attribute
	pattern factories included in Maya.
*/
class OPENMAYA_EXPORT MPxAttributePatternFactory
{
public:

	MPxAttributePatternFactory();
	virtual ~MPxAttributePatternFactory();

	// These must provide implementations, there is no default.
	virtual MStatus	createPatternsFromString(const MString&			 patternString,
											 MAttributePatternArray& createdPatterns) const = 0;
	virtual MStatus	createPatternsFromFile	(const MString&			 patternFile,
											 MAttributePatternArray& createdPatterns) const = 0;
	virtual MString				name		()										  const = 0;

	static const char* className();

private:
};

// *****************************************************************************

OPENMAYA_NAMESPACE_CLOSE
#endif /* __cplusplus */
#endif /* _MPxAttributePatternFactory */
