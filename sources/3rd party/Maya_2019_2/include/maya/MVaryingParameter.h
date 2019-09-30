#ifndef _MVaryingParameter
#define _MVaryingParameter
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
// CLASS:    MVaryingParameter
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MVaryingParameter)

//! \ingroup OpenMayaRender
//! \brief Geometric data cache.
/*!

  The MVaryingParameter class provides a high-level interface to
  hardware shader varying parameters. By defining your shader's
  varying data through this class, you allow Maya to handle the
  attributes, editing, serialisation, requirements setup, and cache
  management for you in a standard way that ensure you'll be able to
  leverage future performance and functionlity improvements.

  At setup time (either initial load or when the effect/technique is
  changed), your shader simply creates the list of parameters it
  requires, specifying the name, type, semantic of the parameters. At
  render time, you can then use the parameters to directly access the
  appropriate buffers for that surface data.

  If you include a custom Attribute Editor template for your shader
  node, you can include these surface parameters by calling the
  AEhwShaderTemplateParameters script function. The following sample
  code provides a basic template you can modify - however your AE
  template can use as much or as little of this as you like:

\code
global int $yourShaderNameTemplateInitialised = 0;

global proc AEyourShaderNameTemplate ( string $node )
{
	global int $yourShaderNameTemplateInitialised;
	if( $yourShaderNameTemplateInitialised == 0)
	{
		source "AEhwShaderTemplate.mel";
		$yourShaderNameTemplateInitialised = 1;
	}

	AEhwShaderTemplateHeader( $node);

	// Insert custom attributes between the swatch and the parameters here

	AEhwShaderTemplateParameters( $node);

	// Insert custom attributes after the parameters here

	AEhwShaderTemplateFooter( $node);
}
\endcode
*/
class OPENMAYARENDER_EXPORT MVaryingParameter
{
public:
	//! Parameter types.
	typedef enum
	{
		// Warning - make sure you modify the internal version when changing this!

		kInvalidParameter = -1,	//!< \nop
		kStructure = 0,			//!< \nop
		kFloat,					//!< \nop
		kDouble,				//!< \nop
		kChar,					//!< \nop
		kUnsignedChar,			//!< \nop
		kInt16,					//!< \nop
		kUnsignedInt16,			//!< \nop
		kInt32,					//!< \nop
		kUnsignedInt32			//!< \nop
	} MVaryingParameterType;

	//! Parameter semantics (i.e. what the parameter represents).
	typedef enum
	{
		// Warning - make sure you modify the internal version when changing this!

		kNoSemantic = 0,	//!< \nop
		kPosition,		//!< \nop
		kNormal,		//!< \nop
		kTexCoord,		//!< \nop
		kColor,			//!< \nop
		kWeight,		//!< \nop
		kTangent = kWeight + 2,	//!< \nop
		kBinormal,		//!< \nop
	} MVaryingParameterSemantic;

	MVaryingParameter();

	MVaryingParameter( const MString& name,
						MVaryingParameterType type,
						int minDimension = 1,
						int maxDimension= 1,
						MVaryingParameterSemantic semantic = kNoSemantic,
						bool invertTexCoords = false,
                        const MString& semanticName = "");
	MVaryingParameter( const MString& name,
						MVaryingParameterType type,
						int minDimension, 
						int maxDimension, 
						int dimension,
						MVaryingParameterSemantic semantic = kNoSemantic,
						const MString& destinationSet="",
						bool invertTexCoords = false,
                        const MString& semanticName = "");


	~MVaryingParameter();

	const MVaryingParameter& operator=( const MVaryingParameter& other);

	const MString	name() const;

	MVaryingParameter::MVaryingParameterType type() const;

	MVaryingParameter::MVaryingParameterSemantic semantic() const;

	const MString	semanticName() const;

	MString			destinationSet() const;

	int             dimension() const;

	MStatus			getBuffer( MGeometryLegacy& geometry, const void*& data, unsigned int& elements, unsigned int& count) const;

	MVaryingParameter::MVaryingParameterSemantic getSourceType() const;

	MString			getSourceSetName() const;

	MStatus			setSource( MVaryingParameter::MVaryingParameterSemantic type, const MString& setName);

	unsigned int	getElementSize() const;

	unsigned int	getMaximumStride() const;

	MStatus			addElement( const MVaryingParameter& child);

	unsigned int	numElements() const;

	MVaryingParameter getElement( unsigned int i) const;

	MStatus			removeElements();

	unsigned int	getUpdateId() const;
protected:

	MVaryingParameter( void* ptr);

	void*			_ptr;

private:
// No private members

	friend class MVaryingParameterList;
};
OPENMAYA_NAMESPACE_CLOSE
 
#endif
#endif // _MVaryingParameter
