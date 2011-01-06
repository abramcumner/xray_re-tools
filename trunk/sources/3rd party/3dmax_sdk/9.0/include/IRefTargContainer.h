/**********************************************************************
 *<
	FILE:  IRefTargContainer.h

	DESCRIPTION:  Defines IRefTargContainer class
		A Reference Maker class that stores a variable list of ReferenceTarget* items.
		All actions performed on this class are undoable.

	CREATED BY: Larry Minton

	HISTORY: created 5/19/04

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/

#ifndef _IREFTARGCONTAINER_H_
#define _IREFTARGCONTAINER_H_

// SClass_ID is REF_TARGET_CLASS_ID
#define REFTARG_CONTAINER_CLASS_ID Class_ID(0xa1692edf, 0xd681173f)

class IRefTargContainer : public ReferenceTarget
{
public:
	virtual int GetNumItems()=0;  // Return the number of items
	virtual ReferenceTarget *GetItem(int i)=0;  // Return the specified item, NULL if out of range
	virtual int AppendItem(ReferenceTarget *ref)=0; // Append item. Returns index of item.
	virtual void SetItem(int i, ReferenceTarget *ref)=0; // Set the specified item.
	virtual int InsertItem(int i, ReferenceTarget *ref)=0; // Insert specified item, index clamped to range. Returns index of item.
	virtual void RemoveItem(int i)=0; // Remove specified item.
};

#endif