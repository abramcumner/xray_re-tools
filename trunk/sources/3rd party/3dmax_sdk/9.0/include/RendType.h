//**************************************************************************/
// Copyright (c) 1998-2005 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Interface for working with xref that support proxies
// AUTHOR: Attila Szabo - created Aug.23.2005
//***************************************************************************/

#ifndef _REND_TYPE_H
#define _REND_TYPE_H

enum RendType
{ 
	RENDTYPE_NORMAL,
	RENDTYPE_REGION,
	RENDTYPE_BLOWUP,
	RENDTYPE_SELECT,
	RENDTYPE_REGIONCROP,
	// The following 2 types are to NOT be passed into plugin renderers.  There purpose is for passing to
	//  the function Interface::OpenCurRenderer, which converts them into RENDTYPE_REGION and RENDTYPE_REGIONCROP, resp.
	RENDTYPE_REGION_SEL, // do a region render using the bounding rectangle of the selection
	RENDTYPE_CROP_SEL,	// do a crop render using the bounding rectangle of the selection

	RENDTYPE_BAKE_SEL,	// bake textures on selected objects
	RENDTYPE_BAKE_ALL,	// bake textures on all objects
	RENDTYPE_BAKE_SEL_CROP,	// bake textures on selected objects, crop render
};

#endif 
