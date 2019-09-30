#ifndef adskDataAccessorMaya_h
#define adskDataAccessorMaya_h

#include <maya/adskCommon.h>

#include <set>
#include <string>

namespace adsk {
namespace Data {

class Associations;
class Handle;
class Stream;
class Structure;
class Accessor;

// *****************************************************************************
/*! \namespace adsk::Data::AccessorMaya

	Utility functions to read metadata from Maya files.
*/
namespace AccessorMaya {

METADATA_EXPORT const std::string& getSceneAssociationsName();

// *****************************************************************************
/*! \namespace adsk::Data::AccessorMaya::ExternalContent

	Utility functions to read external content table from Maya files and
	interpret its content.  The external content table is the list of files used
	by a Maya file, and this information is located in a scene-level metadata
	associations (adsk::Data::Associations).

	The underlying mechanism to read this information is adsk::Data::Accessor.

	External content is located in a metadata stream in a file's scene-level
	Associations:

	adsk::Data::Associations {
		adsk::Data::Channel {
			adsk::Data::Stream {
				Item[ 0 ] {
					Node: "The node name"
					Key:  "Key for this item in the node (often attribute name)"
					Unresolved path: "Unresolved path for the content."
					Resolved path: "Content path as resolved on last save."
					Roles: [ "Image", "Background", ... ]
				}
				...
			}
		}
	}

	\li The Associations name in an Accessor is given by
		adsk::Data::AccesorMaya::getSceneAssociationsName.
	\li The channel, stream and various member names are given by the matching
		functions in the ExternalContent namespace.
	\li The node name is the dependency graph node to which the item applies.
	\li The key is the name of the external content for the given node.
		It will often match a node's attribute name, but this is not mandatory.
	\li The unresolved and resolved path members are two versions of the same
		path for the external content.  The unresolved has no token substitution
		performed (may contain environment variables, be relative to the project
		root, etc.).  The resolved path is how the path was resolved the last
		time the scene was saved.
	\li The roles array is a list of roles that this specific external content
		item play in the node's context.  These strings are loosely defined and
		can be used for filtering entries (e.g.: ignoring the files marked as
		"Icon" when sending the scene to a networked renderer).
*/
namespace ExternalContent {

//------------------------------------------------------------------------------

METADATA_EXPORT bool readTable(
	const std::string& fileName,
	Associations&      associationsRet,
	std::string&       errors
);

//------------------------------------------------------------------------------

METADATA_EXPORT const std::string& getStructureName();
METADATA_EXPORT const std::string& getChannelName();
METADATA_EXPORT const std::string& getStreamName();
METADATA_EXPORT const std::string& getNodeMemberName();
METADATA_EXPORT const std::string& getKeyMemberName();
METADATA_EXPORT const std::string& getUnresolvedPathMemberName();
METADATA_EXPORT const std::string& getResolvedPathMemberName();
METADATA_EXPORT const std::string& getRolesMemberName();
METADATA_EXPORT const std::string& getRolesSeparator();

//------------------------------------------------------------------------------

METADATA_EXPORT const Stream* getStream( const Associations& associations );
METADATA_EXPORT Stream* getStream( Associations& associations );
METADATA_EXPORT const Stream* getStream( const Accessor& accessor );
METADATA_EXPORT Stream* getStream( Accessor& accessor );

//------------------------------------------------------------------------------

METADATA_EXPORT void getEntry(
	Handle&                  handle,
	std::string&             nodeNameRet,
	std::string&             keyRet,
	std::string&             unresolvedPathRet,
	std::string&             resolvedPathRet,
	std::set< std::string >& roleSetRet
);
METADATA_EXPORT void setEntry(
	Handle&                        handle,
	const std::string&             nodeName,
	const std::string&             key,
	const std::string&             unresolvedPath,
	const std::string&             resolvedPath,
	const std::set< std::string >& roleSet
);
METADATA_EXPORT void updateEntry(
	Handle&            handle,
	const std::string& newPath
);

} // namespace ExternalContent

} // namespace AccessorMaya
} // namespace Data
} // namespace adsk

//=========================================================
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================

#endif
