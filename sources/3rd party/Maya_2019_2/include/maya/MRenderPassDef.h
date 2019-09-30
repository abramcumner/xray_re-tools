#ifndef _MRenderPassDef
#define _MRenderPassDef

//-
// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MRenderPassDef
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MString.h>
#include <maya/MStatus.h>
#include <maya/MPxRenderPassImpl.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderPassRegistry)

//! \ingroup OpenMayaRender
//! \brief Render pass information
/*!
  This class provides a means to access information about a specific render
  pass. Initialize by passing to
  MRenderPassRegistry::getRenderPassDefinition(), or by calling create()
  to generate and register a new pass.
*/

class OPENMAYA_EXPORT MRenderPassDef {
public:
	virtual ~MRenderPassDef();

	MPxRenderPassImpl* getImplementation(
				const MString& renderer,
				MPxRenderPassImpl::PassTypeBit fbType, 
				unsigned int numChannels,
				MStatus* ReturnStatus=NULL);

	MPxRenderPassImpl* getImplementation(
				const MString& renderer,
				MStatus* ReturnStatus=NULL);

	MString getID(MStatus* ReturnStatus=NULL) const;
	MString getName(MStatus* ReturnStatus=NULL) const;
	MString getGroup(MStatus* ReturnStatus=NULL) const;
	MString getDescription(MStatus* ReturnStatus=NULL) const;
    MString getAttributeType(const MString &attributeName, MStatus* ReturnStatus = NULL) const;

	MStatus addFloatParameter(
		const MString &longName,
		const MString &shortName,
        const MString &UIName,
		const float &minValue,
		const float &maxValue,
		const float &defaultValue
		);

	MStatus addDoubleParameter(
		const MString &longName,
		const MString &shortName,
        const MString &UIName,
		const double &minValue,
		const double &maxValue,
		const double &defaultValue
		);

	MStatus addIntParameter(
		const MString &longName,
		const MString &shortName,
        const MString &UIName,
		const int &minValue,
		const int &maxValue,
		const int &defaultValue
		);

	static const char* className();

private:
	friend class MRenderPassRegistry;
	friend class MFnPlugin;

	// can only get instances by querying or creating
	MRenderPassDef();

	void*		instance;
};

// ****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderPassDef */

