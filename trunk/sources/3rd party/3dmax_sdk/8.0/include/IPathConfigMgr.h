/*	
*		IPathConfigMgr.h - Public interface for modifying application paths.
*
*			Copyright © Autodesk, Inc, 2005.  David Cunningham.
*
*/

#ifndef IPATHCONFIG_H
#define IPATHCONFIG_H

#include "iFnPub.h"

#define PATHCONFIG_INTERFACE   Interface_ID(0x490ddc99, 0xbe8dc96)
//! \brief Provides access to application path configuration functionality.
//! Note that the terms "map paths" and "External Files" (as seen in the UI) are
//! used interchangeably below.
//! This class provides access to all exposed path configuration functionality,
//! including loading and saving of path configuration files.  Should be used
//! in lieu of Interface for path-related operations.
//!
//! To access the single IPathConfigMgr instance, call the static function 
//! IPathConfigMgr::GetPathConfigMgr().
class IPathConfigMgr : public FPStaticInterface
{
public:

	//! \brief Loads a path configuration file.

	//! Loads a path configuration file and returns true if paths are loaded
	//! successfully.
	//! \pre file passed to function is a path configuration file
	//! \post Paths found in file are loaded into application persistently.
	//! \param[in] filename The fully-qualified path of the file to be loaded.
	//! \return true if any paths are loaded
	virtual bool LoadPathConfiguration(const TCHAR* filename) = 0;
	//! \brief Merges a path configuration file.

	//! Merges a path configuration file and returns true if paths are merged
	//! successfully.  A merge differs from a load in that map, xref and plugin
	//! directories are appended to the list instead of the list being overwritten.
	//! File IO paths are simply overwritten, as is the case with load.
	//! \pre file passed to function is a path configuration file
	//! \post Paths found in file are loaded into application persistently.
	//! \param[in] filename The fully-qualified path of the file to be loaded.
	//! \return true if any paths are loaded
	virtual bool MergePathConfiguration(const TCHAR* filename) = 0;
	//! \brief Saves out the current path configuration to the specified file.

	//! \pre Specified path is valid and writable.
	//! \post The current user path configuration is saved to the specified file.
	//! \param[in] filename The fully-qualified path of the file to which paths are saved.
	//! \return true if file is successfully saved out
	virtual bool SavePathConfiguration(const TCHAR* filename) = 0;

	//! \brief Returns the default directory for the specified ID.

	//! \param[in] which APP_XXX_DIR directory ID
	//! \return the default directory
	virtual const TCHAR *GetDir(int which)=0;		
	//! \brief Sets the default directory.

	//! \param[in] which The index of the directory to set.
	//! \param[in] dir The new default direcory.
	//! \return true if the directory is set and valid
	virtual bool SetDir(int which, TCHAR *dir) = 0;
	//! \brief Returns the number of plugin path entries in PLUGIN.INI.

	//! \return the number of plugin path entries.
	virtual int	GetPlugInEntryCount()=0;	
	//! \brief Returns the description string for the given entry number.

	//! \param[in] i entry number
	//! \return the ith description string
	virtual const TCHAR *GetPlugInDesc(int i)=0;
	//! \brief Returns the directory string for the given entry number.

	//! \param[in] i entry number
	//! \return the ith directory string
	virtual const TCHAR *GetPlugInDir(int i)=0;	

	// bitmap path
	//! \brief Returns the number of map / xref directories in path.

	//! \param[in] xref specifies which type of count looking for ( xref = false / map = true)
	//! \return number of dirs in path
	virtual int GetMapDirCount(bool xref)=0;			
	//! \brief Returns the ith bitmap / xref directory in path.

	//! \param[in] i index of path to get
	//! \param[in] xref specifies the directory category from which to get the directory.
	//! If true, the directory should be retrieved from among the xref directories,
	//! otherwise from the map directories.
	//! \return the ith bitmap / xref directory in path
	virtual const TCHAR *GetMapDir(int i, bool xref)=0;		
	//! \brief Adds a map / xref path to the list.

	//! \param[in] dir directory to add
	//! \param[in] xref specifies the directory category to which the new directory 
	//! should be added. If true, the new directory should be added to the xref directories,
	//! otherwise to the map directories
	//! \return true if added successfully
	virtual bool AddMapDir(TCHAR *dir, bool xref)=0;
	//! \brief Adds a map / xref path to the list.

	//! \param[in] dir directory to add
	//! \param[in] update Should the scene be updated after the add?
	//! \param[in] xref specifies the directory category to which the new directory 
	//! should be added. If true, the new directory should be added to the xref directories,
	//! otherwise to the map directories
	virtual bool AddMapDir(TCHAR *dir, int update, bool xref) = 0;	
	//! \brief Deletes a map / xref path

	//! Deletes a map / xref path to the path list. 
	//! \param[in] dir Directory to add.
	//! \param[in] udpate Should the scene be updated with the new path?
	//! \param[in] xref Is this an xref path or a bitmap path?
	//! \return true if Deletes successfully
	virtual bool DeleteMapDir(int i, int update, bool xref) = 0;
	//! \brief Resolves a given path using a predefined path search order.

	/*! This method should be used by plugins to search for an auxiliary file (asset).
	The file is searched for in the following locations, in the order specified below:
	\li The full UNC path and filename specified as the first parameter.
	\li The directory where the current 3ds max file was loaded from.
	\li The map or xref search path.  If filepath is a partial or relative path, then the 
	path is appended to each search path.  In other words, the filename by itself is not searched 
	in the search path.
	\li The directory tree under the directory where the current 3ds max file was loaded from.
	\param [in] filePath Full or partial file path to be used to find a file.
	The filePath must contain at least a file name and extension. The filePath 
	can be a full or relative path. UNC paths are supported. 
	\param [in] xref Specifies which category of search directories should be used.
	If true, xref search directories are used, otherwise map search directories.
	\return The full file path if it was found or the empty string if it wasn't.
	\remarks This method will return a UNC path name if the option controlling this is turned on. 
	*/
	virtual const TSTR GetFullFilePath(const TCHAR* filePath, bool xref) = 0;

	//! \brief Adds a session path

	//! Adds a session path to the path list.  Session paths are not persistent, meaning
	//! they are lost when the application shuts down.
	//! \param[in] dir Directory to add.
	//! \param[in] udpate Should the scene be updated with the new path?
	//! \param[in] xref Is this an xref path or a bitmap path?
	//! \return true if added successfully
	virtual bool AddSessionMapDir(TCHAR *dir, int update, bool xref)=0;
	//! \brief Returns the number of bitmap / xref session directories in path.

	//! \param[in] xref specifies which type of count looking for ( xref = false / map = true)
	//! \return number of session dirs in path
	virtual int GetSessionMapDirCount(bool xref)=0;
	//! \brief Returns the ith bitmap / xref session directory in path.

	//! \param[in] i index of path to get
	//! \param[in] xref specifies which type of count looking for ( xref = false / map = true)
	//! \return the ith bitmap / xref session directory in path
	virtual const TCHAR *GetSessionMapDir(int i, bool xref)=0;
	//! \brief Deletes a session path

	//! Deletes a session path to the path list.  Session paths are not persistent, meaning
	//! they are lost when the application shuts down.
	//! \param[in] dir Directory to add.
	//! \param[in] udpate Should the scene be updated with the new path?
	//! \param[in] xref Is this an xref path or a bitmap path?
	//! \return true if Deletes successfully
	virtual bool DeleteSessionMapDir(int i, int update, bool xref)=0;

	//! \brief Gets the combined total of session and permanent paths.

	//! These methods provide access to the combined list of permanent and temporary (session) map dirs,
	//! therefore the current total set of map directories
	//! \param[in] xref specifies which type of count looking for ( xref = false / map = true)
	//! \return the combined total of session and permanent paths
	virtual int GetCurMapDirCount(bool xref)=0;
	//! \brief Gets the ith map directory.

	//! Gets ith directory.  Session paths are enumerated before permanent paths.
	//! \param[in] i index to return
	//! \param[in] xref specifies which type of count looking for ( xref = false / map = true)
	//! \return the ith directory
	virtual const TCHAR *GetCurMapDir(int i, bool xref)=0;
	
	//! \brief Forces an update on the application.

	//! Forces the application to update itself with the currently
	//! set path.
	//! \param[in] xref Update maps or xrefs?
	virtual void UpdateMapSection(bool xref) = 0;

	//! \brief Returns the name of the .ini file used by max.

	//! \return The name of the .ini file
	//! Returns empty TSTR if locType == LOC_REGISTRY.
	virtual TSTR GetMAXIniFile() = 0; 

	//! \brief Returns the Preferences --> Files [Resolve UNC paths] property

	//! If this flag returns true, then paths will automatically be 
	//! converted to their UNC representation if the path originates from
	//! a mapped drive.
	//! \return If paths are automatically resolved to their UNC equivalents
	virtual bool GetResolveUNC() = 0;
	//! \brief Returns the Preferences --> Files [Resolve UNC paths] property

	//! If this flag is set to true, then paths will automatically be 
	//! converted to their UNC representation if the path originates from
	//! a mapped drive.
	//! \param[in] aFlag If paths should automatically be resolved to their UNC equivalents
	virtual void SetResolveUNC(bool aFlag) = 0;

	//! \brief Returns the single instance of this manager.

	//! Gets the single instance of this class.
	//! \return The single instance of this class.
	static inline IPathConfigMgr* GetPathConfigMgr() { return static_cast<IPathConfigMgr*>(GetCOREInterface(PATHCONFIG_INTERFACE)); }
};




#endif // IPATHCONFIG_H