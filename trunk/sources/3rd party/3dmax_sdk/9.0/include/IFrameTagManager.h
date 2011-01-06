/**********************************************************************
*<
FILE: IFrameTagManager.h

DESCRIPTION: Interface to Frame Tag Manager

CREATED BY: Larry Minton

HISTORY: created 8/23/04

*>	Copyright (c) 2004 Autodesk, All Rights Reserved.
**********************************************************************/

#ifndef __IFRAMETAGMANAGER__
#define __IFRAMETAGMANAGER__

#define FRAMETAGMANAGER_INTERFACE Interface_ID(0x8cd73eea, 0x688b1f11)

class IFrameTagManager : public FPStaticInterface {
	public:
		// get number of frame tags
		virtual int GetTagCount() = 0;

		// get ID for the Ith frame tag in the list of frame tags
		// the list is maintained in sorted order, so a given
		// tag can move around in the list. Each tag has a unique
		// ID, so all access to the tag is via this ID 
		virtual DWORD GetTagID(int i) = 0;

		// get/set the tag's time value. If relative is TRUE, time is relative
		// to the tag this tag is locked to, if any. If false, or this tag is 
		// not locked to another tag, the time is absolute
		virtual TimeValue GetTimeByID(DWORD id, BOOL relative) = 0;
		virtual void SetTimeByID(DWORD id, TimeValue newTime, BOOL relative) = 0;

		// get/set the tag this tag is locked to. An ID value of 0 means not tagged
		// If you attempt to set a lock that would cause a circular dependency, the
		// current lock will not be changed and the method will return FALSE. See CheckForLoop
		// for testing for loops
		virtual DWORD GetLockIDByID(DWORD id) = 0;
		virtual BOOL SetLockIDByID(DWORD id, DWORD newLockID) = 0;

		// get/set the tag's name
		virtual TSTR GetNameByID(DWORD id) = 0;
		virtual void SetNameByID(DWORD id, TSTR &newName) = 0;

		// get/set whether to scale the tag's time when time is scaled
		virtual BOOL GetScaleByID(DWORD id) = 0;
		virtual void SetScaleByID(DWORD id, BOOL newScale) = 0;

		// get the index of the tag in the sorted tag list. A return value of -1
		// means a tag with the given ID was not found
		virtual int FindItem(DWORD id) = 0;

		// finds the first tag in the sorted list, if any, for the given absolute time value
		virtual int FindItemByTime(TimeValue t) = 0;

		// creates a new tag. If lockID is 0, the tag is not locked to another tag
		// The time value is always absolute
		virtual DWORD CreateNewTag(TCHAR *name, TimeValue t, DWORD lockID, BOOL scale) = 0;

		// deletes the specified tag
		virtual void DeleteTag(DWORD id) = 0;

		// deletes all the tags
		virtual void ResetFrameTags() = 0;

		// returns true if tag testID is dependent on tag id
		virtual BOOL CheckForLoop(DWORD testID, DWORD id) = 0;

};

#endif //__IFRAMETAGMANAGER__
