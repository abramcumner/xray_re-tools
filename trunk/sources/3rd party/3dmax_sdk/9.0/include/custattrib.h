/**********************************************************************
 *<
	FILE:  CustAttrib.h

	DESCRIPTION:  Defines CustAttrib class

	CREATED BY: Nikolai Sander

	HISTORY: created 5/25/00

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#ifndef _ICUSTATTRIB_H_
#define _ICUSTATTRIB_H_

#include "maxtypes.h"
#include "plugapi.h"
#include "ref.h"

class ICustAttribContainer;
class IGraphObjectManager;
class IMtlParams;
class ParamDlg;

class CustAttrib: public ReferenceTarget
{
public:
	virtual SClass_ID SuperClassID() { return CUST_ATTRIB_CLASS_ID; }
	virtual TCHAR* GetName(){ return "Custom Attribute";}
	virtual ParamDlg *CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp){return NULL;}
	virtual bool CheckCopyAttribTo(ICustAttribContainer *to) { return true; }
	virtual SvGraphNodeReference SvTraverseAnimGraph(IGraphObjectManager *gom, Animatable *owner, int id, DWORD flags) { return SvStdTraverseAnimGraph(gom, owner, id, flags); }
};

#endif