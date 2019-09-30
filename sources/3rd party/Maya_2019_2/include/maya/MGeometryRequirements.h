#ifndef _MGeometryRequirements
#define _MGeometryRequirements
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
// CLASS:    MGeometryRequirementsLegacy
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MGeometryRequirementsLegacy)
//
// The MGeometryRequirementsLegacy class describes the set of geometry data
// elements required by an geometry user. For example, a material can
// specify that is needs position, normal and UV set "a" to render a
// surface.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MUintArray.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGeometryRequirementsLegacy)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
  \deprecated
  Use MHWRender::MGeometryRequirements instead.

  MGeometryRequirementsLegacy stores the collection of
  MGeometryRequirementsData arrays which describe a Maya surface,
  including per-component data such as UV mapping and colour.
*/
class OPENMAYARENDER_EXPORT MGeometryRequirementsLegacy
{
public:
					MGeometryRequirementsLegacy();
					~MGeometryRequirementsLegacy();
					void addPosition(int dimension = 3);
					void addComponentId();
					void addNormal(int dimension = 3);
					void addTexCoord( const MString& uvSetName);
					void addTangent( const MString& uvSetName, int dimension = 3);
					void addBinormal( const MString& uvSetName, int dimension = 3);
					void addColor( const MString& colorSetName);
					void addFaceOffsets();

private:

					MGeometryRequirementsLegacy( void*);
	void*			mRequirements;
	bool			mOwnsRequirements;

	// Hide these so people can't hold onto references by taking
	// copies to the temporary references to these structures passed
	// into populateRequirements
					MGeometryRequirementsLegacy( const MGeometryRequirementsLegacy&);
	const MGeometryRequirementsLegacy& operator=( const MGeometryRequirementsLegacy&);

	friend class MGeometryManager;
	friend class MPxHardwareShader;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MGeometryRequirements */
