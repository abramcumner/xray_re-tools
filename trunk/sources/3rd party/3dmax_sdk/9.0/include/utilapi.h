/**********************************************************************
*<
	FILE: utilapi.h

	DESCRIPTION: Utility plug-ins interface

	CREATED BY: Rolf Berteig

	HISTORY: 12-23-95 file created

*>	Copyright (c) 1994, All Rights Reserved.
**********************************************************************/

#ifndef __UTILAPI__
#define __UTILAPI__
#include "maxheap.h"

class IUtil: public MaxHeapOperators {
	public:
		// Closes the current utility in the command panel
		virtual void CloseUtility()=0;
	};

// A utility plug-in object
class UtilityObj: public MaxHeapOperators {
	public:
		virtual void BeginEditParams(Interface *ip,IUtil *iu)=0;
		SDK_RESERVED_METHOD(1);	// russom 02/26/01
		virtual void EndEditParams(Interface *ip,IUtil *iu)=0;
		virtual void SelectionSetChanged(Interface *ip,IUtil *iu) {}
		SDK_RESERVED_METHOD(2); // russom 02/26/01
		virtual void DeleteThis()=0;
		virtual void SetStartupParam(TSTR param) {}
	};


#endif //__UTILAPI__

