/**********************************************************************
*<
FILE: DllEntry.cpp

DESCRIPTION: Contains the Dll Entry stuff

CREATED BY: 

HISTORY: 

*>	Copyright (c) 2003, All Rights Reserved.
**********************************************************************/
// Import part
#include "ObjectImport.h"
// Export part
#include "ObjectExport.h"
// Material part
#include "XRMaterial.h"

// xrFSL
#include "FileSystem.h"
#include "StringUtils.h"
#include "Log.h"

extern ClassDesc2* GetxrFSLObjectImportDesc();
extern ClassDesc2* GetxrFSLObjectExportDesc();
extern ClassDesc2* GetxrFSLMaterialDesc();

HINSTANCE hInstance;
int controlsInit = FALSE;

// This function is called by Windows when the DLL is loaded.  This 
// function may also be called many times during time critical operations
// like rendering.  Therefore developers need to be careful what they
// do inside this function.  In the code below, note how after the DLL is
// loaded the first time only a few statements are executed.

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	hInstance = hinstDLL;				// Hang on to this DLL's instance handle.
	DisableThreadLibraryCalls(hInstance);

	if (!controlsInit) {
		controlsInit = TRUE;
		InitCustomControls(hInstance);	// Initialize MAX's custom controls
		InitCommonControls();			// Initialize Win95 controls
	}

	return (TRUE);
}

/**
* Init xrFSL subsystems.
*/
static BOOL Init(void)
{
	static bool bInitialized = false;

	if (bInitialized)
	{
		LOGE("Second initialization");
		return false;
	}

	// получим текущую директорию
// 	char szCurrentDirectory[_MAX_PATH];
// 	GetCurrentDirectory(_MAX_PATH, szCurrentDirectory);

	// getting path to max root dir and make from it a path to FS specification file
	//xrFSL::String fs_spec(szCurrentDirectory);

#if MAX_RELEASE < 9000
	xrFSL::String fs_spec(GetCOREInterface()->GetDir(APP_MAXROOT_DIR));	
#else
	xrFSL::String fs_spec(GetCOREInterface()->GetDir(APP_MAX_SYS_ROOT_DIR));
#endif

	xrFSL::StringUtil::AppendPathSeparator(fs_spec);
	fs_spec.append("fs.ltx");

	// initializing FS
	xrFSL::FileSystem& fs = xrFSL::FileSystem::Instance();
	ASSERT_F(fs.Initialize(fs_spec.c_str(), 0), ("File system initialization failed.")); 

	// initializing logger
	fs.ResolvePath(xrFSL::PA_LOGS, "xrFSLMaxTools_objects.log", fs_spec);
	xrFSL::CLog::Instance().Init(fs_spec.c_str());

	LOG("[xrFSL]Object plugin started.");
	
	return bInitialized = true;
}

/**
 *	Releasing xrFSL subsystems.
 */
static BOOL Release(void)
{
// 
}

// This function returns a string that describes the DLL and where the user
// could purchase the DLL if they don't have it.
__declspec( dllexport ) const TCHAR* LibDescription()
{
	return _T("xrFSL MaxTools");
}

// This function returns the number of plug-in classes this DLL
//TODO: Must change this number when adding a new class
__declspec( dllexport ) int LibNumberClasses()
{
	// Initializing xrFSL subsystems
	Init();

	//return 3;	// Object Import/Export & Material
	return 2; // object imp/exp
}

// This function returns the number of plug-in classes this DLL
__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i)
	{
	case 0:			// Import part
		return GetxrFSLObjectImportDesc();
		break;
	case 1:			// Export part
		return GetxrFSLObjectExportDesc();
		break;
	case 2:			// Mtl part
		return GetxrFSLMaterialDesc();
		break;
	default:
		return 0;
		break;
	}
}

// This function returns a pre-defined constant indicating the version of 
// the system under which it was compiled.  It is used to allow the system
// to catch obsolete DLLs.
__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

/**
 *	This function is called early in the processing of your DLL for you to do special initializations.
 */
__declspec( dllexport ) int LibInitialize() 
{
	return Init();
}

/**
 *	This function is called when 3ds Max is finished using your DLL.
 */
// __declspec( dllexport ) int LibShutdown(void)
// {
// 	//
// }

TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

