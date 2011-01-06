//***************************************************************************
// SystemUtilities.h
// A collection of system utilities
// Christer Janson
// Discreet, A division of Autodesk, Inc.
// San Francisco, CA - March 27, 2000

#ifndef SYSTEMUTILITIES_H
#define SYSTEMUTILITIES_H

UtilExport bool	IsDebugging();			// Are we running under a debugger?
UtilExport int	NumberOfProcessors();	// Number of processors in the system.
UtilExport bool	IsWindows9x();			// Are we running on Windows 9x?
UtilExport bool	IsWindows98or2000();	// Are we running on Windows 98 or 2000?
UtilExport int	GetScreenWidth();		// The width of the screen (including multiple monitors)
UtilExport int	GetScreenHeight();		// The height of the screen (including multiple monitors)
UtilExport bool IsGetACPAsian();        // returns true if the current ANSI codepage is for an asian language

// CSIDL functions added 030110  --prs.

UtilExport HRESULT UtilGetFolderPath(HWND hwndOwner,	// just calls SFGetFolderPath()
				int nFolder, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath);

// negative indices, refer to specCSID[] array in systemutilities.cpp

/*! System directory that points to <max data>\maps */
#define APP_MAP_DIR			   -1
#define APP_FOLIAGE_DIR		   -2
#define APP_XREF_DIR		   -3
#define APP_PLUGIN_INI_DIR	   -4
#define APP_STDPLUGS_DIR	   -5
#define APP_PLUGINS_DIR		   -6
#define APP_FILELINK_DIR	   -7
#define APP_CATALOGS_DIR	   -8
#define APP_CUI_SCRIPTS_DIR    -9	// added 030224  --prs.
// xavier robitaille | 03.02.05 | add textures dir. to bitmap paths
#ifdef TEXTURES_DIR_BMP_SEARCH_PATH
#define APP_INT_OFFSET		   -1
#define APP_TEXTURES_DIR	   -10
#else
#define APP_INT_OFFSET		   0
#endif
#define APP_CUI_DIR					-10 + APP_INT_OFFSET
#define APP_FX_DIR					-11 + APP_INT_OFFSET	
#define APP_CUI_WRITE_DIR			-12 + APP_INT_OFFSET
#define APP_MARKET_DEF_WRITE_DIR	-13 + APP_INT_OFFSET
#define APP_LAST_SPEC_DIR			-13 + APP_INT_OFFSET

//! \brief Returns a value indicating whether the application is configured to use User Profiles or not.
/*! The application can be configured to use User Profiles. */
//! \return true if the application uses User Profile directories
UtilExport bool IsUsingProfileDirectories();
//! \brief Returns a value indicating whether the application is configured to use romaing User Profiles or not.
/*!  The application can be configured to use roaming User Profiles.  This value is dependent on 
	 whether the application uses User Profiles at all.  Even if this returns true, if IsUsingProfileDirectories()
	 returns false, then the application will default to the legacy path configuration locations under the application
	 root install directory.  */
//! \return true if the application uses User Profile directories
UtilExport bool IsUsingRoamingProfiles();

UtilExport bool GetSpecDir(int index, const TCHAR *dirName, TCHAR *buf);	// get directory path
UtilExport bool TryCSIDLDir(int csidl, const TCHAR *dirName, TCHAR *buf);	// create directory path

UtilExport BOOL IsSSE();
#ifdef RENDER_VER
UtilExport bool GetAcadCookie(TCHAR *buf, int cbSize);              // VizR: get the cookie for our ACAD vertical.
UtilExport TCHAR *GetAcadProdNameShort();                           // VizR: get the short product name for the ADT or vertical
UtilExport TCHAR *GetAcadLocalRootFolder();                         // VizR: get the LocalRootFolder for the ADT or vertical
#endif // ~RENDER_VER

#endif // SYSTEMUTILITIES_H