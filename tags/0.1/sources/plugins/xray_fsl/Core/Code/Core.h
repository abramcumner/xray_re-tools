/********************************************************************
	created:	2009/03/05
	created:	5:3:2009   1:04
	filename: 	e:\Projects\xrFSL\Core\Code\Core.h
	file path:	e:\Projects\xrFSL\Core\Code
	file base:	Core
	file ext:	h
	author:		Neo][
	
	purpose:	
*********************************************************************/
#pragma once

#include "Prerequisites.h"

namespace xrFSL
{
	// This class is exported from the Core.dll
	class xrFSL_CORE_API CCore {
	public:
		CCore(void);
		// TODO: add your methods here.
	};

	extern xrFSL_CORE_API int nCore;

	xrFSL_CORE_API int fnCore(void);
}