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
// DESCRIPTION: Legacy Object Xref SDK
// AUTHOR: Nikolai Sander - created July.07.2000
//***************************************************************************/

#ifndef _IXREF_OBJ_MGR_H_
#define _IXREF_OBJ_MGR_H_

class IXRefObject;

//! \brief Legacy interface for managing object xrefs
/*! \remarks This interface should be considered obsolete. Please use IObjXRefManager8 instead.
Note that this interface does not provide full support for multiple object xref records/links 
created from the same source file. 
\see IObjXRefManager8
*/
class IObjXRefManager : public FPStaticInterface 
{
	public:
	
	// function IDs 
	enum { 
		fnIdAddXRefObject,
		fnIdGetNumXRefObjects,
		fnIdGetXRefObject,
		fnIdGetNumFiles,
		fnIdGetFileName,
		fnIdReloadFile,
		fnIdIsFileUnresolved,
		fnIdIsFileDisabled,
		fnIdGetAllXRefObjects,
	};
	
	virtual IXRefObject *AddXRefObject(TCHAR *fname, TCHAR *obname, int xFlags = 0)=0;

	virtual int GetNumXRefObjects(TCHAR *fname)=0;
	virtual IXRefObject *GetXRefObject(TCHAR *fname, int i)=0;
	
	virtual int GetNumFiles()=0;
	virtual TCHAR *GetFileName(int fidx)=0;
	virtual BOOL ReloadFile(TCHAR *fname)=0;
	
	virtual BOOL IsFileUnresolved(TCHAR *fname)=0;
	virtual BOOL IsFileDisabled(TCHAR *fname)=0;
	virtual void GetAllXRefObjects(Tab<IXRefObject*> &objs)=0;
};

#define OBJXREFMANAGER_INTERFACE Interface_ID(0x7ede1c65, 0x353d271f)
inline IObjXRefManager* GetObjXRefManager () { return (IObjXRefManager*)GetCOREInterface(OBJXREFMANAGER_INTERFACE); }

#endif //_IXREF_OBJ_MGR_H_