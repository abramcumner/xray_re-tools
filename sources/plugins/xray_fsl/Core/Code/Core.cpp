#include "stdafx.h"
#include "Core.h"

namespace xrFSL
{

	// This is an example of an exported variable
	xrFSL_CORE_API int nCore=0;

	// This is an example of an exported function.
	xrFSL_CORE_API int fnCore(void)
	{
		return 42;
	}

	// This is the constructor of a class that has been exported.
	// see Core.h for the class definition
	CCore::CCore()
	{
		return;
	}
}