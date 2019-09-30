#ifndef _MFnAttribute
#define _MFnAttribute
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
// CLASS:    MFnAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnAttribute)

//! \ingroup OpenMaya MFn
//! \brief Dependency node attribute function set. 
/*!
  MFnAttribute is the function set for dependency node attributes.

  An attribute of a dependency node describes a piece of data that belongs to
  nodes of that type.  For example, a node that makes a sphere might have a
  radius attribute.  A node's attributes describe connections sites on the node
  that can be used in the dependency graph.

  Attributes typically belong to a class of nodes, rather than to an individual
  node.  For example, all sphere nodes have a radius.  Even though a single
  attribute may be shared by all nodes of a given type, each node will have
  its own value for the attribute.

  Attributes are hierarchical.  For example, the translate scale attribute of a
  transform has x, y, and z child attributes.

  By default, attributes are:

  \li Readable.
  \li Writable.
  \li Connectable.
  \li Storable.
  \li Cached.
  \li Not arrays.
  \li Have indices that matter.
  \li Do not use an array builder.
  \li Not keyable.
  \li Not hidden.
  \li Not used as colors.
  \li Not indeterminant.
  \li Set to disconnect behavior kNothing.
*/
class OPENMAYA_EXPORT MFnAttribute : public MFnBase
{
	declareMFn(MFnAttribute, MFnBase);

public:

	//! Attribute behaviour on disconnect.
	enum DisconnectBehavior {
		kDelete,		//!< Delete array element (array attributes only).
		kReset,			//!< Reset the attribute to its default.
		kNothing		//!< Do nothing to the attribute's value.
	};

	bool		isReadable( MStatus* ReturnStatus=NULL ) const;
	bool		isWritable( MStatus* ReturnStatus=NULL ) const;
	bool		isConnectable( MStatus* ReturnStatus=NULL ) const;
	bool		isStorable( MStatus* ReturnStatus=NULL ) const;
	bool		isCached( MStatus* ReturnStatus=NULL ) const;
	bool		isArray( MStatus* ReturnStatus=NULL ) const;
	bool		indexMatters( MStatus* ReturnStatus=NULL ) const;
	bool		isKeyable( MStatus* ReturnStatus=NULL ) const;
	bool		isChannelBoxFlagSet( MStatus* ReturnStatus=NULL ) const;
	bool		isHidden( MStatus* ReturnStatus=NULL ) const;
	bool		isUsedAsColor( MStatus* ReturnStatus=NULL ) const;
	bool		isIndeterminant( MStatus* ReturnStatus=NULL ) const;
	bool		isRenderSource( MStatus* ReturnStatus=NULL ) const;
	bool		isDynamic( MStatus* ReturnStatus=NULL ) const;
    bool        isExtension( MStatus* ReturnStatus=NULL ) const;
	bool		isWorldSpace( MStatus* ReturnStatus=NULL ) const;
	bool		isAffectsWorldSpace( MStatus* ReturnStatus=NULL ) const;
	bool		isUsedAsFilename( MStatus* ReturnStatus=NULL ) const;
	bool		affectsAppearance( MStatus* ReturnStatus=NULL ) const;
	bool		isProxyAttribute( MStatus* ReturnStatus=NULL ) const;

	DisconnectBehavior disconnectBehavior( MStatus* ReturnStatus=NULL ) const;
	bool        usesArrayDataBuilder( MStatus* ReturnStatus=NULL ) const;
	bool     	internal( MStatus* ReturnStatus=NULL ) const;

	MStatus		setReadable( bool state );
	MStatus		setWritable( bool state );
	MStatus		setConnectable( bool state );
	MStatus	 	setStorable( bool state );
	MStatus		setCached( bool state );
	MStatus	 	setArray( bool state );
	MStatus	 	setIndexMatters( bool state );
	MStatus	 	setKeyable( bool state );
	MStatus	 	setChannelBox( bool state );
	MStatus	 	setHidden( bool state );
	MStatus		setUsedAsColor( bool state );
	MStatus		setIndeterminant( bool state );
	MStatus		setRenderSource( bool state );
	MStatus		setWorldSpace( bool state );
	MStatus		setAffectsWorldSpace( bool state );
	MStatus		setUsedAsFilename( bool state );
	MStatus		setAffectsAppearance( bool state );
	MStatus		setProxyAttribute( bool state);

	MStatus     setDisconnectBehavior( DisconnectBehavior behavior );
	MStatus     setUsesArrayDataBuilder( bool state );
	MStatus     setInternal( bool state );
    MStatus     setNiceNameOverride( const MString &localizedName );
    bool        acceptsAttribute( const MFnAttribute& attr, MStatus* ReturnStatus=NULL ) const;
	bool		accepts( MFnData::Type type, MStatus* ReturnStatus=NULL ) const;
	bool		accepts( const MTypeId& id, MStatus* ReturnStatus=NULL ) const;
	MObject		parent(MStatus* ReturnStatus=NULL) const;
    MString     name( MStatus* ReturnStatus=NULL ) const;
    MString     shortName( MStatus* ReturnStatus=NULL ) const;
	MString		getAddAttrCmd(
					bool			useLongName = false,
					MStatus*		status=NULL
				) const;
    MStatus     addToCategory( const MString &category );
    MStatus     removeFromCategory( const MString &category );
    bool        hasCategory( const MString &category ) const;
    MStatus     getCategories( MStringArray &ReturnStatus ) const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnAttribute, MFnBase );

END_NO_SCRIPT_SUPPORT:


protected:
	void * ca[4];
	void setPtr( MPtrBase* );

private:
	virtual bool objectChanged( MFn::Type, MStatus * );
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnAttribute */
