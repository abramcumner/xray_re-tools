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
// CLASS:    MPxMaterialInformation
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#ifndef _MPxMaterialInformation
#define _MPxMaterialInformation

#include <maya/MTypes.h>
#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MColor.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMaya MPx
//! \brief Phong shading parameters for an MPxMaterialInformation instance.
/*!
The MaterialInputData class is used to specify the phong shading parameters
to be used with an MPxMaterialInformation instance.
*/
class MaterialInputData
{
public:
	//! Shininess value
    float				shininess;
	//! Ambient color
	MColor				ambient;
	//! Diffuse color
    MColor              diffuse;
	//! Emission color
    MColor              emission;
	//! Specular color
    MColor              specular;

	//! Tells whether the material has transparency or not.
	bool				hasTransparency;
protected:
// No protected members
private:
// No private members
};

// ****************************************************************************
// CLASS DECLARATION (MPxMaterialInformation)

//! \ingroup OpenMaya MPx
//! \brief Material information proxy.
/*!
The MPxMaterialInformation class is a way for users to override the viewport
representation of shaders.  The viewport uses a simple phong model for display
in the viewport.  With this class users can provide their own values for the
phong shading parameters.
*/
class OPENMAYA_EXPORT MPxMaterialInformation
{
public:

	//! Material types. These affect how the material is shaded.
	enum MaterialType
	{
		kSimpleMaterial = 0,	//!< \nop
		kTexture,		//!< \nop
		kOverrideDraw		//!< \nop
	};

	MPxMaterialInformation(MObject & materialNode);

	virtual ~MPxMaterialInformation();

	bool	useMaterialAsTexture();

	// Virtual overrides

	virtual bool materialInfoIsDirty(const MPlug& plug) = 0 ;

	virtual bool connectAsTexture(const MPlug& plug) = 0 ;
	virtual bool textureDisconnected(const MPlug& plug) = 0 ;
	virtual bool computeMaterial(MaterialInputData& data)= 0 ;

protected:
	friend class MFnPlugin;
	//! Default constructor.
	MPxMaterialInformation();

	//! The shader node this material is based on
	MObject				fMaterialNode;

	//! Based on the material type the shader will be rendered
	//! differently in the Maya viewport
	MaterialType		fMaterialType;

	// fInstance Needs to be visible from static non-member methods in MFnPlugin
public:
	void*				fInstance;

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif/* __cplusplus */
#endif/* _MPxMaterialInformation */
