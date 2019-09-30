#ifndef _MUniformParameter
#define _MUniformParameter
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
// CLASS:    MUniformParameter
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MString.h>
#include <maya/MPlug.h>

class MUniform;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MUniformParameter)

//! \ingroup OpenMayaRender
//! Uniform parameter.
/*!
The MUniformParameter class provides a high-level interface to
hardware shader uniform parameters. By defining your shader's uniform
parameters through this class, you allow Maya to handle the
attributes, editing, serialisation, and caching for you in a standard
way that ensure you'll be able to leverage future performance and
functionlity improvements.

At setup time (either initial load or when the effect/technique is
changed), your shader simply creates the list of parameters it
requires, specifying the name, type, semantic of the parameters. At
render time, you can then use the parameters to directly access the
appropriate buffers for that surface data.

If you include a custom Attribute Editor template for your shader
node, you can include these surface parameters by calling the
AEhwUniformTemplateParameters script function. The following sample
code provides a basic template you can modify - however your AE
template can use as much or as little of this as you like:

\code
global int $yourUniformNameTemplateInitialised = 0;

global proc AEyourUniformNameTemplate ( string $node )
{
	global int $yourUniformNameTemplateInitialised;
	if( $yourUniformNameTemplateInitialised == 0)
	{
		source "AEhwUniformTemplate.mel";
		$yourUniformNameTemplateInitialised = 1;
	}

	AEhwUniformTemplateHeader( $node);

	// Insert custom attributes between the swatch and the parameters here

	AEhwUniformTemplateParameters( $node);

	// Insert custom attributes after the parameters here

	AEhwUniformTemplateFooter( $node);
}

\endcode
*/
class OPENMAYARENDER_EXPORT MUniformParameter
{
public:
	//! Parameter data types.
	typedef enum
	{
		kTypeUnknown,		//!< \nop
		kTypeBool,		//!< \nop
		kTypeInt,		//!< \nop
		kTypeFloat,		//!< \nop
		kType1DTexture,		//!< \nop
		kType2DTexture,		//!< \nop
		kType3DTexture,		//!< \nop
		kTypeCubeTexture,	//!< \nop
		kTypeEnvTexture,	//!< \nop
		kTypeString,		//!< \nop
		kTypeEnum,		    //!< \nop
		// NOTE - when editing this, be sure to update the internal equivalent
	} DataType;


	//! Parameter semantics (i.e. what the parameter represents).
	typedef enum
	{
		kSemanticUnknown,				//!< \nop
		kSemanticObjectDir,				//!< \nop
		kSemanticWorldDir,				//!< \nop
		kSemanticViewDir,				//!< \nop
		kSemanticProjectionDir,				//!< \nop
		kSemanticObjectPos,				//!< \nop
		kSemanticWorldPos,				//!< \nop
		kSemanticViewPos,				//!< \nop
		kSemanticProjectionPos,				//!< \nop
		kSemanticColor,					//!< \nop
		kSemanticNormal,				//!< \nop
		kSemanticBump,					//!< \nop
		kSemanticEnvironment,				//!< \nop
		kSemanticWorldMatrix,				//!< \nop
		kSemanticWorldInverseMatrix,			//!< \nop
		kSemanticWorldInverseTransposeMatrix,		//!< \nop
		kSemanticViewMatrix,				//!< \nop
		kSemanticViewInverseMatrix,			//!< \nop
		kSemanticViewInverseTransposeMatrix,		//!< \nop
		kSemanticProjectionMatrix,			//!< \nop
		kSemanticProjectionInverseMatrix,		//!< \nop
		kSemanticProjectionInverseTransposeMatrix,	//!< \nop
		kSemanticWorldViewMatrix,			//!< \nop
		kSemanticWorldViewInverseMatrix,		//!< \nop
		kSemanticWorldViewInverseTransposeMatrix,	//!< \nop
		kSemanticWorldViewProjectionMatrix,		//!< \nop
		kSemanticWorldViewProjectionInverseMatrix,	//!< \nop
		kSemanticWorldViewProjectionInverseTransposeMatrix, //!< \nop
		kSemanticColorTexture,				//!< \nop
		kSemanticNormalTexture,				//!< \nop
		kSemanticBumpTexture,				//!< \nop
		kSemanticNormalizationTexture,			//!< \nop
		kSemanticTranspDepthTexture,		//!< \nop
		kSemanticOpaqueDepthTexture,		//!< \nop
		kSemanticTime,					//!< \nop

		kSemanticWorldTransposeMatrix,				//!< \nop
		kSemanticViewTransposeMatrix,				//!< \nop
		kSemanticProjectionTransposeMatrix,				//!< \nop
		kSemanticWorldViewTransposeMatrix,				//!< \nop
		kSemanticWorldViewProjectionTransposeMatrix,		//!< \nop

		kSemanticViewProjectionMatrix,		//!< \nop
		kSemanticViewProjectionInverseMatrix,	//!< \nop
		kSemanticViewProjectionTransposeMatrix,	//!< \nop
		kSemanticViewProjectionInverseTransposeMatrix, //!< \nop

		kSemanticLocalViewer,					//!< \nop
		kSemanticViewportPixelSize,				//!< \nop
		kSemanticBackgroundColor,				//!< \nop
		kSemanticFrameNumber,					//!< \nop

		kSemanticNearClipPlane,					//!< \nop
		kSemanticFarClipPlane,					//!< \nop

		kSemanticHWSPrimitiveBase,				//!< \nop
		kSemanticHWSPrimitiveCountPerInstance,	//!< \nop
		kSemanticHWSObjectLevel,				//!< \nop
		kSemanticHWSFaceLevel,					//!< \nop
		kSemanticHWSEdgeLevel,					//!< \nop
		kSemanticHWSVertexLevel,				//!< \nop
		kSemanticHWSOccluder,					//!< \nop
		kSemanticHWSFrontCCW,					//!< \nop
		kSemanticHWSInstancedDraw,				//!< \nop
		kSemanticHWSHighlighting				//!< \nop
		
		// NOTE - when editing this, be sure to update the internal equivalent
	} DataSemantic;

	MUniformParameter();

	MUniformParameter( const MString& name,
						DataType type,
						DataSemantic semantic,
						unsigned int numRows = 1,
						unsigned int numColumns = 1,
						void* userData = NULL);

	~MUniformParameter();

	const MUniformParameter& operator=( const MUniformParameter& other);

	MString name() const;

	MUniformParameter::DataType type() const;

	MUniformParameter::DataSemantic semantic() const;

	unsigned int numRows() const;

	unsigned int numColumns() const;

	unsigned int numElements() const;

	void* userData() const;

	void setRangeMin(double rangeMin) const;
	void setRangeMax(double rangeMax) const;
	void setSoftRangeMin(double softRangeMin) const;
	void setSoftRangeMax(double softRangeMax) const;

	void setUIHidden(bool hiddenState) const;
	bool UIHidden() const;

	void setEnumFieldNames(const MString& fieldNames);

	void setKeyable(bool keyable) const;
	bool keyable() const;

	bool isATexture() const;

	bool hasChanged( const MGeometryList& iterator) const;
	bool hasChanged( const MHWRender::MDrawContext& context) const;

	const float* getAsFloatArray( const MGeometryList& iterator) const;
	const float* getAsFloatArray( const MHWRender::MDrawContext& context) const;

	void setAsFloatArray( const float* value, unsigned int maxElements) const;

	float getAsFloat( const MGeometryList& iterator) const;
	float getAsFloat( const MHWRender::MDrawContext& context ) const;

	void setAsFloat( float value) const;

	MString getAsString( const MGeometryList& iterator) const;
	MString getAsString( const MHWRender::MDrawContext& context ) const;

	void setAsString( const MString& value) const;

	bool getAsBool( const MGeometryList& iterator) const;
	bool getAsBool( const MHWRender::MDrawContext& context  ) const;

	void setAsBool( bool value) const;

	int getAsInt( const MGeometryList& iterator) const;
	int getAsInt( const MHWRender::MDrawContext& context ) const;

	void setAsInt( int value) const;

	void setUINiceName(const MString& name);

	MPlug getPlug() const;
	MPlug getSource() const;

	void setDirty();

protected:

	MUniformParameter( void* ptr);

	void*			_ptr;

private:
// No private members

	friend class MUniformParameterList;
};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MUniformParameter
