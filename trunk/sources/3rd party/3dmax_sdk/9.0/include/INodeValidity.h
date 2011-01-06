/**********************************************************************
 *<
	FILE:			INodeValidity.h

	DESCRIPTION:	Declare BaseInterface class INodeValidity
											 
	CREATED BY:		MIchael Zyracki

	HISTORY:		created Aug 6, 2004

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/
#ifndef __INODEVALIDITY__H__
#define __INODEVALIDITY__H__

#include "baseinterface.h"


// The interface ID for class INodeValidity
#define NODEVALIDITY_INTERFACE Interface_ID(0x15ce0b65, 0x6acd24ba)

// Provides access to the nodes layer and bylayer bits
class INodeValidity : public BaseInterface 
{
public:
	//get access to the node validity intervals 
	virtual Interval GetObValid() =0;
	virtual Interval GetWSValid() =0;
	virtual Interval GetTMValid() =0;

	Interface_ID	GetID() { return NODEVALIDITY_INTERFACE; }
	LifetimeType	LifetimeControl() { return wantsRelease; }
	void			DeleteInterface() =0;

};
#endif //__INODEVALIDITY__H__