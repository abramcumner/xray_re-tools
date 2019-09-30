#ifndef _MPlug
#define _MPlug
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
// ****************************************************************************
//
// CLASS:    MPlug
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MDGContext.h>
#include <maya/MObject.h>
#include <maya/MIntArray.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN


// ****************************************************************************
// CLASS DECLARATION (MPlug)

//! \ingroup OpenMaya
//! \brief Create and Access dependency node plugs.
/*!
	MPlug provides methods for creating plugs, and accessing the plugs'
	dependency node and attributes.

	A plug is a point on a dependency node where a particular attribute can be
	connected.  In simple cases the plug and attribute are equivalent.
	When you have array attributes,  however, the plug is more specific
	in that it indicates which of the array elements is to be connected.

	There are two main types of plugs: networked plugs and non-networked plugs.
	Non-networked plugs can be considered user plugs as they are
	created by users and belong to users. Networked plugs can be considered
	dependency node plugs as they are part of the dependency graph and can
	only be referenced by users.

    In every dependency node there is a network or "tree" of plugs indicating
    connections that have been made to attributes of the node. The plugs in
	this tree are known as <b>networked</b> plugs as they belong to the
	dependency node's network.

    <b>Non-networked</b> plugs are plugs that you can create in order to
	establish a new connection to an attribute, or to get or set a value
    on an attribute.  When a connection is made using these plugs, a
	networked version of the plug is created and added to the dependency
    nodes network.

    A <b>networked</b> plug cannot be explicitly created. They are created when
    a connection is established for some attribute of the node.  Since the
	allocation of networked plugs is managed exclusively by Maya, a plugin
	should never reference a networked plug after changes are made to the DG.
	Instead, use a non-networked version of the plug, or get a new reference
	to the networked plug.

    All a <b>non-networked</b> plug knows is how to uniquely describe the
	attribute that it references, in fact the purpose of a non-networked
	plug is to specify, without ambiguity, an attribute of a dependency node.
	A non-networked plug contains an array of array indices that plot the
	path from the root plug to this plug.

    For simple attributes the plug and attribute are equivalent. Compound
    attributes are also unambiguous. A plug that refers to an array
    attribute, however, is more complex as it can refer to the <b>array
    plug</b> or an <b>element plug</b> in the array.

	Several methods are provided for navigating the plug tree. The child()
    method can be used to retrieve the <b>child plugs</b> of a <b>compound
    plug</b>. The elementByLogicalIndex() and elementByPhysicalIndex()
    methods can be used to retrieve the element plugs of an array plug. The
    parent() and array() methods traverse the tree in the opposite
    direction: parent() retrieves a compound plug from a child plug, and
    array() retrieves an array plug from an element plug.

	Since connections to arrays are sparse, element plugs have both logical
    and physical indexes. The logical indexes are used by MEL, and are sparse.
    Physical indexes, on the other hand, are not sparse. It is guaranteed that
    the physical indexes will range from 0 to numElements() - 1. Using the
    physical indexes, iterating over the element plugs in an array is easy:
     \code

       // See note below on numElements() method
       for (i = 0; i < arrayPlug.numElements (); i++)
       {
           MPlug elementPlug = arrayPlug [i];
		   unsigned int logicalIndex = elementPlug.logicalIndex();
           // ...
       }
    \endcode

	This is equivalent to calling elementByPhysicalIndex() since the
	bracket operator uses physical indexes.

	The ancestry of a plug is the tree above the plug: any parents or
	arrays which this plug belongs to. The
	selectAncestorLogicalIndex() method provides quick access to
	element plugs without walking the plug tree. A plug does not need
	to exist in the datablock or plug tree in order to be constructed
	using selectAncestorLogicalIndex(), but once a value is requested or set
	on the plug, it will be added to the datablock if it was not there
	already.  For example: 

	\code
	MPlug plug(node,attribute);
	// See note below on numElements() method
	unsigned int count = plug.numElements();
	if (0 == count) {
	    plug.selectAncestorLogicalIndex(0,attribute);
		plug.setValue(setVal);
		count = plug.numElements(); // count will now be 1 since we added a value

	    plug.selectAncestorLogicalIndex(10,attribute);
		MObject getVal;
		plug.getValue(getVal);
		count = plug.numElements(); // count will now be 2 since we asked for a value
	}
    \endcode

	Once a value has been added to an array plug, it will usually
	remain in the datablock as long as the scene is open. The
	exception is for attributes that have their disconnect behavior
	set to kDelete: data for these attributes will be deleted when
	their connection is broken. When a file is saved, only array
	values that are not at their default values will be written
	out. Therefore, the number of elements in an array plug may change
	when the file is read in again.

	\note
	The numElements() call for array plugs returns a value based
	on the information in the datablock.  It is possible to have
	un-evaluated connected items being excluded from the count since
	these items are not yet in the datablock.  In this case, the
	numElements() method will return the correct answer if the
	evaluateNumElements() call is made first.  But this cannot be
	done in the MPxNode::compute() method of a node.
*/
class OPENMAYA_EXPORT MPlug
{
public:
	MPlug();
	MPlug( const MPlug& in );
	MPlug( const MObject & node, const MObject & attribute );
	virtual	~MPlug();

	MStatus     setAttribute(MObject &attribute);
	MObject		attribute	( MStatus* ReturnStatus = nullptr ) const;
	MObject 	node		( MStatus* ReturnStatus = nullptr ) const;
	MString		name		( MStatus* ReturnStatus = nullptr ) const;
	MString		partialName	(
					bool	 includeNodeName = false,
					bool	 includeNonMandatoryIndices = false,
					bool	 includeInstancedIndices = false,
					bool	 useAlias = false,
					bool	 useFullAttributePath = false,
					bool	 useLongNames = false,
					MStatus* ReturnStatus = nullptr
				) const;

	//! Which values to generate setAttr commands for.
	enum MValueSelector {
		kAll,				//!< All values
		kNonDefault,		//!< Non default values
		kChanged,			//!< Changed values
		kLastAttrSelector	//!< Last value, for counting
	};

	MStatus		getSetAttrCmds(
					MStringArray&	cmds,
					MValueSelector	valueSelector = kAll,
					bool			useLongNames = false
				);

	bool		 isNetworked	( MStatus* ReturnStatus = nullptr ) const;
	bool         isArray		( MStatus* ReturnStatus = nullptr ) const;
	bool         isElement		( MStatus* ReturnStatus = nullptr ) const;
	bool         isCompound		( MStatus* ReturnStatus = nullptr ) const;
	bool         isChild		( MStatus* ReturnStatus = nullptr ) const;
	bool         isProcedural	( MStatus* ReturnStatus = nullptr ) const;
	unsigned int logicalIndex	( MStatus* ReturnStatus = nullptr ) const;
	MStatus	     selectAncestorLogicalIndex( unsigned int index, const MObject &attribute = MObject::kNullObj);

	unsigned int    getExistingArrayAttributeIndices(MIntArray& indices, MStatus* ReturnStatus = nullptr);

	unsigned int    numElements			( MStatus* ReturnStatus = nullptr ) const;
	unsigned int    evaluateNumElements	( MStatus* ReturnStatus = nullptr );
	unsigned int    numChildren			( MStatus* ReturnStatus = nullptr ) const;
	unsigned int    numConnectedElements( MStatus* ReturnStatus = nullptr ) const;
	unsigned int    numConnectedChildren( MStatus* ReturnStatus = nullptr ) const;
	MPlug	child	( MObject& attr, MStatus* ReturnStatus = nullptr ) const;
	MPlug	child	( unsigned int index, MStatus* ReturnStatus = nullptr ) const;
	MPlug	parent	( MStatus* ReturnStatus = nullptr ) const;
	MPlug	array	( MStatus* ReturnStatus = nullptr ) const;

	MPlug	elementByLogicalIndex	( unsigned int logicalIndex, MStatus* ReturnStatus = nullptr) const;
	MPlug	elementByPhysicalIndex	( unsigned int physicalIndex, MStatus* ReturnStatus = nullptr) const;

	MPlug   connectionByPhysicalIndex	( unsigned int physicalIndex, MStatus* ReturnStatus = nullptr) const;
	bool	connectedTo					( MPlugArray & array, bool asDst, bool asSrc, MStatus* ReturnStatus = nullptr ) const;
	bool	isConnected					( MStatus* ReturnStatus = nullptr ) const;
    MPlug 	source						( MStatus* ReturnStatus = nullptr ) const;
    bool	destinations				( MPlugArray& theDestinations, MStatus* ReturnStatus = nullptr) const;
    MPlug 	sourceWithConversion		( MStatus* ReturnStatus = nullptr ) const;
    bool	destinationsWithConversions	( MPlugArray& theDestinations, MStatus* ReturnStatus = nullptr) const;

	bool    isKeyable				( MStatus* ReturnStatus = nullptr ) const;
	MStatus setKeyable				( bool keyable );
	bool    isLocked				( MStatus* ReturnStatus = nullptr ) const;
	MStatus setLocked				( bool locked );
	bool    isChannelBoxFlagSet		( MStatus* ReturnStatus = nullptr ) const;
	MStatus setChannelBox			( bool channelBox );
	bool    isCachingFlagSet		( MStatus* ReturnStatus = nullptr ) const;
	MStatus setCaching				( bool caching );
	bool	isNull					( MStatus* ReturnStatus = nullptr ) const;
	MString info					( MStatus* ReturnStatus = nullptr ) const;
	bool	isFromReferencedFile	( MStatus* ReturnStatus = nullptr ) const;
	bool	isDynamic				( MStatus* ReturnStatus = nullptr ) const;
	bool	isIgnoredWhenRendering	( MStatus* ReturnStatus = nullptr ) const;
	bool	isDefaultValue			( bool forceEval = true, MStatus* ReturnStatus = nullptr ) const;

	//! Plug state returned by isFreeToChange
	enum FreeToChangeState {		
		kFreeToChange = 0,			//!< All tested plugs are free to change
		kNotFreeToChange,			//!< Some of the tested plugs are not free to change
		kChildrenNotFreeToChange	//!< Some of the children are not free to change
	};
	MPlug::FreeToChangeState isFreeToChange(bool checkParents = true,
									   bool checkChildren = true,
									   MStatus* ReturnStatus = nullptr ) const;
	MDataHandle	constructHandle	(MDataBlock&) const;
	void        destructHandle	(MDataHandle&) const;

	// Python Friendly Versions

	// functions to get and set attribute values in the current context
	MObject		asMObject	 ( MStatus * ReturnStatus = nullptr ) const;
	MDataHandle	asMDataHandle( MStatus * ReturnStatus = nullptr ) const;
	double		asDouble	 ( MStatus * ReturnStatus = nullptr ) const;
	float		asFloat		 ( MStatus * ReturnStatus = nullptr ) const;
	MInt64		asInt64		 ( MStatus * ReturnStatus = nullptr ) const;
	int			asInt		 ( MStatus * ReturnStatus = nullptr ) const;
	short		asShort		 ( MStatus * ReturnStatus = nullptr ) const;
	bool		asBool		 ( MStatus * ReturnStatus = nullptr ) const;
	MDistance	asMDistance	 ( MStatus * ReturnStatus = nullptr ) const;
	MAngle		asMAngle	 ( MStatus * ReturnStatus = nullptr ) const;
	MTime		asMTime		 ( MStatus * ReturnStatus = nullptr ) const;
	char		asChar		 ( MStatus * ReturnStatus = nullptr ) const;
	MString     asString	 ( MStatus * ReturnStatus = nullptr ) const;

	MStatus		setMObject		( const MObject & );
	MStatus		setMPxData		( const MPxData * );
	MStatus		setMDataHandle	( const MDataHandle & );
	MStatus		setDouble		( double );
	MStatus		setFloat		( float );
	MStatus		setInt64		( MInt64 );
	MStatus		setInt			( int );
	MStatus		setShort		( short );
	MStatus		setBool			( bool );
	MStatus		setMDistance	( const MDistance & );
	MStatus		setMAngle		( const MAngle & );
	MStatus		setMTime		( const MTime& );
	MStatus		setChar			( char );
	MStatus		setString		( const MString & );

	// Operators
	MPlug&		operator =	( const MPlug& other );
	MPlug		operator[]	( unsigned int physicalIndex ) const;	// index(index)
	bool		operator ==	( const MPlug &other ) const;
	bool		operator ==	( const MObject &other ) const;
	bool		operator !=	( const MPlug &other ) const;
	bool		operator !=	( const MObject &other ) const;
	MStatus		setNumElements	( unsigned int );
	bool		isSource		( MStatus* ReturnStatus = nullptr ) const;
	bool		isDestination	( MStatus* ReturnStatus = nullptr ) const;

	static const char*	className();

BEGIN_NO_SCRIPT_SUPPORT:
	MStatus		getValue( MObject& )	 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( MDataHandle& ) const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( double& )		 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( float& )		 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( MInt64& )		 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( int& )		 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( short& )		 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( bool& )		 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( MDistance& )	 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( MAngle& )		 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( MTime& )		 const; //!<	NO SCRIPT SUPPORT
	MStatus		getValue( char& )		 const;	//!<	NO SCRIPT SUPPORT
	MStatus		getValue( MString& )	 const;	//!<	NO SCRIPT SUPPORT
	//
	MStatus		setValue( const MObject & );	 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( const MPxData * );	 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( const MDataHandle & ); //!<	NO SCRIPT SUPPORT
	MStatus		setValue( double );				 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( float );				 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( MInt64 );				 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( int );				 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( short );				 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( bool );				 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( const MDistance & );	 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( const MAngle& );		 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( const MTime & );		 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( char );				 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( const MString& );		 //!<	NO SCRIPT SUPPORT
	MStatus		setValue( const char* );		 //!<	NO SCRIPT SUPPORT

	//! NO SCRIPT SUPPORT
	MPlug		operator[] ( MObject& attr ) const; // child(attr)

	//! NO SCRIPT SUPPORT
	operator	MObject() const;					// attribute()

	//! NO SCRIPT SUPPORT
	bool		operator!() const;					// false if valid
END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
	MString		getStringHelper	( MStatus* ReturnStatus) const;


	MPlug( const void*, bool );
	const void*	 fPlug;
	bool         ownPlug;

	//**********************************************************************
	// Obsolete variations of methods
public:

	OPENMAYA_DEPRECATED(2018, "Use asMObject(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	MObject		asMObject	 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asMDataHandle(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	MDataHandle	asMDataHandle( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asDouble(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	double		asDouble	 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asFloat(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	float		asFloat		 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asInt64(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	MInt64		asInt64		 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asInt(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	int			asInt		 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asShort(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	short		asShort		 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asBool(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	bool		asBool		 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asMDistance(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	MDistance	asMDistance	 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asMAngle(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	MAngle		asMAngle	 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asMTime(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	MTime		asMTime		 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asChar(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	char		asChar		 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

	OPENMAYA_DEPRECATED(2018, "Use asString(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	MString     asString	 ( const MDGContext&, MStatus * ReturnStatus = nullptr ) const;

BEGIN_NO_SCRIPT_SUPPORT:

	OPENMAYA_DEPRECATED(2018, "Use getValue(MObject&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( MObject&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(MDataHandle&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( MDataHandle&,	MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(double&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( double&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(float&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( float&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(MInt64&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( MInt64&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(int&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( int&,			MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(short&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( short&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(bool&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( bool&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(MDistance&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( MDistance&,	MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(MAngle&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( MAngle&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(MTime&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( MTime&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(char&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus		getValue( char&,		MDGContext& )	const;
	
	OPENMAYA_DEPRECATED(2018, "Use getValue(MString&) to get the value in the current evaluation context. If needed, use MDGContextGuard to switch the context.")
	MStatus     getValue( MString&,		MDGContext& )	const;
END_NO_SCRIPT_SUPPORT:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MPlug */
