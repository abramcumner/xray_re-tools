/**********************************************************************
 *<
	FILE: procmap.cpp

	DESCRIPTION:   textures

	CREATED BY: Dan Silva

	HISTORY: created 15 Feb  1997

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/

#include "procmaps.h"
#include "buildver.h" // orb 01-03-2001 Removing map types
#include "resource.h"
#include "resourceOverride.h"

HINSTANCE hInstance;

/** public functions **/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
   if( fdwReason == DLL_PROCESS_ATTACH )
   {
      hInstance = hinstDLL;
      DisableThreadLibraryCalls(hInstance);
   }
	return(TRUE);
	}


static BOOL InitProcMapDll()
{
	static BOOL InitBool=TRUE;

	if( InitBool )
	{
      // register SXP readers
      RegisterSXPReader(_T("PLANET_I.SXP"), Class_ID(PLANET_CLASS_ID,0));
      RegisterSXPReader(_T("STUCCO_I.SXP"), Class_ID(STUCCO_CLASS_ID,0));
      RegisterSXPReader(_T("SMOKE_I.SXP"), Class_ID(SMOKE_CLASS_ID,0));
      RegisterSXPReader(_T("SPECKL_I.SXP"), Class_ID(SPECKLE_CLASS_ID,0));
      RegisterSXPReader(_T("SPLAT_I.SXP"), Class_ID(SPLAT_CLASS_ID,0));
   #ifndef NO_MAPTYPE_WATER
      RegisterSXPReader(_T("WATER_I.SXP"), Class_ID(WATER_CLASS_ID,0));
   #endif
	  InitBool=FALSE;
    }

   return TRUE;
}


//------------------------------------------------------
// This is the interface to Max:
//------------------------------------------------------

__declspec( dllexport ) const TCHAR *
LibDescription() { return GetString(IDS_LIBDESCRIPTION); }

/// MUST CHANGE THIS NUMBER WHEN ADD NEW CLASS
__declspec( dllexport ) int LibNumberClasses() 
{
#ifndef NO_MAPTYPE_WATER
	return 6;
#else
	return 5;
#endif
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i) {
   InitProcMapDll();

	switch(i) {
		case  0: return GetPlanetDesc();
		case  1: return GetSmokeDesc();
		case  2: return GetSpeckleDesc();
		case  3: return GetStuccoDesc();
#ifndef NO_MAPTYPE_WATER
		case  4: return GetWaterDesc();
		case  5: return GetSplatDesc();
#else
		case  4: return GetSplatDesc();
#endif
		default: return 0;
		}
	}

// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG LibVersion() { return VERSION_3DSMAX; }

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}

// Perform inits that should not be done in DllMain()
__declspec( dllexport ) int LibInitialize()
{
   return InitProcMapDll();
}

TCHAR *GetString(int id)
	{
	static TCHAR buf[256];
	if(hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
	}
