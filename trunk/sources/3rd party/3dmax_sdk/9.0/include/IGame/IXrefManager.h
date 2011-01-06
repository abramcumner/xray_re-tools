/**********************************************************************
*<
FILE: IXRefManager.h

DESCRIPTION:	XRef interface for 3DXI

CREATED BY:		Alex Zadorozhny

HISTORY:		11|12|2005


*>	Copyright (c) 2005, All Rights Reserved.
**********************************************************************/
/*!\file IXRefManager.h
\brief XRef interface
*/

#ifndef __IXREFMANAGER__H
#define __IXREFMANAGER__H
#pragma once

#include "Max.h"
#include "IGameStd.h"

class I3DXIXRefManager
{
public:
	
	
	//!Add an XRef object to the scene 
	/*! Add an XRef object to the scene from the specified source by object name
	\param fileName The source 3ds Max file
	\param objName  The name of the object
	\return Pointer to the newly created XRef Object, NULL if not successful. 
	*/

	virtual Object * AddXRefObjectFromFile(TCHAR * fileName, TCHAR * objName) =0;

};


//!External access to the 3DXI XRef manager
IGAMEEXPORT I3DXIXRefManager * GetI3DXIXRefManager();

#endif  ____IXREFMANAGER__H
