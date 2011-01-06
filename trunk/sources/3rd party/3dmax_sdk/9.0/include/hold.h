/**********************************************************************
 *<
	FILE: hold.h

	DESCRIPTION:

	CREATED BY: Dan Silva

	HISTORY:

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/

#ifndef __HOLD__H__
#define __HOLD__H__

#include "maxheap.h"
#include "strbasic.h"

#define RESTOBJ_DOES_REFERENCE_POINTER		1 // cmd ID for RestoreObj::Execute
#define RESTOBJ_GOING_TO_DELETE_POINTER		2 // cmd ID for RestoreObj::Execute
#define REFTARG_AUTO_DELETED				10 // cmd ID for HoldStore::Execute

#define HOLD_SYSTEM_EMPTY					1 // cmd ID for Hold::Execute - return 1 if hold system is definitely empty
#define HOLD_POINTER_IS_MANAGER				2 // cmd ID for Hold::Execute - return 1 if arg1 is a pointer to the undo manager
#define HOLD_SUPERLEVEL						3 // cmd ID for Hold::Execute - return Hold.superLevel

class HoldStore;
class RestoreObj : public InterfaceServer {
	friend class HoldStore;
	friend class GenUndoObject;
	friend class CheckForFlush;
		RestoreObj *next,*prev;
	public:
		RestoreObj() { next = prev = NULL; }
   		virtual ~RestoreObj() {};
		virtual void Restore(int isUndo)=0;
		virtual void Redo()=0;
		virtual int Size() { return 1; }
		virtual void EndHold() { }
		virtual MSTR Description() { return MSTR(_M("---")); }
		virtual INT_PTR Execute(int cmd, ULONG_PTR arg1=0, ULONG_PTR arg2=0, ULONG_PTR arg3=0) {return -1;}
			// WIN64 Cleanup: Shuler
	};

class Hold : public BaseInterfaceServer {
  	HoldStore *holdStore, *holdList;
	int undoEnabled;
	int superLevel;
	int putCount;
	HoldStore *ResetStore();
	void Init();
	void AddOpToUndoMgr(MCHAR *name,int id);
	public:
		CoreExport Hold();
		CoreExport ~Hold();
		CoreExport void Put(RestoreObj *rob);
		CoreExport void Begin();
		CoreExport void Suspend();	  // temporarly suspend holding
		CoreExport int IsSuspended();
		CoreExport void Resume();    // resume holding
		CoreExport int	Holding();  // are we holding?
		CoreExport int  Restoring(int& isUndo);
		CoreExport int  Redoing();
		CoreExport int  RestoreOrRedoing();
		CoreExport void DisableUndo();  // prevents Undo when Accept is called.
		CoreExport void EnableUndo();
		CoreExport void Restore();  // Restore changes from holdStore. 
		CoreExport void Release();  // Tosses out holdStore. 

		// 3 ways to terminate the Begin()...
		CoreExport void End();  // toss holdStore.
		CoreExport void Accept(int nameID); // record Undo (if enabled), End();
		CoreExport void Accept(MCHAR *name);
		CoreExport void Cancel();   // Restore changes, End() 

		//		
		// Group several Begin-End lists into a single Super-group.
		CoreExport void SuperBegin();
		CoreExport void SuperAccept(int nameID);
		CoreExport void SuperAccept(MCHAR *name);
		CoreExport void SuperCancel();

		// Get the number of times Put() has been called in the current session of MAX
		CoreExport int GetGlobalPutCount();

		CoreExport INT_PTR Execute(int cmd, ULONG_PTR arg1=0, ULONG_PTR arg2=0, ULONG_PTR arg3=0);

	protected:
		friend HoldStore;
		unsigned flags;
		enum {
			kInRestore,
			kInUndo,
			kInRedo
		};
	};



extern CoreExport Hold theHold;

CoreExport void EnableUndoDebugPrintout(BOOL onoff);

// A class to help control the hold system. Create an instance of this class, and when it is
// destructed any hold suspends are resumed. Makes it safe in case a throw occurs after the suspend,
// but before the resume
class HoldSuspend: public MaxHeapOperators {
public:
	HoldSuspend(BOOL suspendNow = TRUE) : suspendCount(0) {
		if (suspendNow) {
			this->Suspend();
		}
	}
	~HoldSuspend()	{
		while(suspendCount > 0) {
			this->Resume();
		}
	}
	void Suspend() {
		if (suspendCount == 0) 
			theHold.Suspend ();
		suspendCount++;
	}
	void Resume() {
		if (suspendCount == 1) 
			theHold.Resume();
		if (suspendCount > 0) 
			suspendCount--;
	}

private:
	int	suspendCount;

};					  



#endif //__HOLD__H__
