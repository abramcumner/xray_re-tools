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
// DESCRIPTION: Utility Interface (namespace)
// AUTHOR:	Pete Samson - created 13 September 2001
//			David Cunningham - modified 25 November 2005
//***************************************************************************/

#ifndef _UTILINT_H_
#define _UTILINT_H_

#include "utilexp.h"

namespace UtilityInterface {
	//! \brief Returns the base product registry key: "SOFTWARE\<Company Name>\<Product Name>\<version>"
	/*! Returns the base registry key.  The application uses this key as the base for storing registry settings.
		A plugin can choose to store registry settings under the base (preferably in a subkey group), but likely
		will want store its registry settings under its own company root.
		Combines calls to GetRegistrySoftwareBase(), GetCompanyName(), GetProductName() and GetVersion().
		Note: Tailing backslash is not appended.
	\return base-key in this format: "SOFTWARE\<Company Name>\<Product Name>\<version>"
	*/
	UtilExport const TCHAR* GetRegistryKeyBase();	
	//! \brief Returns the base company registry key: "SOFTWARE\<Company Name>"
	/*! Returns the base company registry key.  This key in general should be used to store registry key settings.
		Combines calls to GetRegistrySoftwareBase(), GetCompanyName().
		This is useful when we need to access registry settings for other Autodesk applications.
		Note: Tailing backslash is not appended.
	\return base-key in this format: "SOFTWARE\<Company Name>"
	*/
	UtilExport const TCHAR* GetRegistryCompanyBase();
	//! \brief Returns the software base (i.e. SOFTWARE - should be static from release to release.)  
	/*! Returns the software base (i.e. SOFTWARE - should be static from release to release.)  
	\return Returns the software base (i.e. SOFTWARE - should be static from release to release.)  
	*/
	UtilExport const TCHAR* GetRegistrySoftwareBase();	
	//! \brief Returns the company name (i.e. Autodesk).  Mainly for branding purposes.
	/*! 
	\return Returns the company name (i.e. Autodesk).  Mainly for branding purposes.
	*/
	UtilExport const TCHAR* GetCompanyName();
	//! \brief Returns the product name (i.e. 3dsmax).  
	/*! 
	\return Returns the product name (i.e. 3dsmax).  
	*/
	UtilExport const TCHAR* GetProductName();
	//! \brief Returns the current release-number (major, minor release: #.#).
	/*! 
	\return Returns the current release-number (major, minor release: #.#) 
	*/
	UtilExport const TCHAR* GetCurrentVersion();

	//! \brief Returns the true/false value of an environment variable.
	/*! Returns the true/false value of a specified environment variable, or the specified default
	value if the environment variable does not exist or the first character of its value is
	not one of "yYtT1" (true) or "nNfF0" (false)
	\param[in] pEnvVarName Pointer to a null-terminated string that specifies the name of the environment variable
	\param[in] defaultVal The default value used if the environment variable is not present, or doesn't 
	contain a valid value
	\return Returns the true/false value of the environment variable or default
	*/
	UtilExport bool GetBoolEnvironmentVariable(TCHAR* pEnvVarName, bool defaultVal);

};

#endif // _UTILINT_H_

