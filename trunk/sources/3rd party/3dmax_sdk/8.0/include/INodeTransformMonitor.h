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
// DESCRIPTION: Defines a class for monitoring a ReferenceTarget by holding a weak reference to it
// (IsRealDependency returns false) and passing NotifyRefChanged messages to the owner of the
// class instance. Defines an interface that uses the above class to monitor a node for 
// its PART_TM messages, and defines an interface for getting/setting weak references.
// AUTHOR: Larry.Minton - created May.14.2004
//***************************************************************************/

#ifndef __INODETRANSFORMMONITOR__H
#define __INODETRANSFORMMONITOR__H

#include "iFnPub.h"

//! Superclass for RefTargMonitorRefMaker
#define REFTARGMONITORREFMAKER_SUPERCLASS_ID	0x18f81902

//! ClassID for NodeTransformMonitor
#define NODETRANSFORMMONITOR_CLASS_ID			Class_ID(0x18f81902, 0x19033fd2)

//! Id for INodeTransformMonitor interface
#define IID_NODETRANSFORMMONITOR				Interface_ID(0x18f81902, 0x19033fd2)

//! Id for IWeakReferenceMaker interface
#define IID_IWEAKREFERENCEMAKER					0x18f81902

//! \brief The interface for an owner of a RefTargMonitorRefMaker class.
/*! The owner of a RefTargMonitorRefMaker instance must implement this interface. Messages from
the RefTargMonitorRefMaker instance are passed to the owner through this interface.
\see RefTargMonitorRefMaker, IWeakReferenceMaker
*/
class IRefTargMonitor {
public:
	//! \brief Calls to the RefTargMonitorRefMaker instance's NotifyRefChanged method are passed to its owner through this method. 
	/*! The RefTargMonitorRefMaker owner is responsible for ensuring that an infinite recursion of message passing does not occur. 
	Typically the owner would set a flag while propogating a message, and not propogate a new message if that flag
	is set. 

	Since the owner doesn't hold a reference to the RefTargMonitorRefMaker's watched object, the watched object may not be 
	saved or loaded when the owner is saved or loaded. The owner must handle this case. Typically, the owner would also derive from
	IWeakReferenceMaker, which will result in the watched object being saved and loaded. For an example, see the NodeTransformMonitor 
	class in maxsdk\\samples\\controllers\\nodetransformmonitor.cpp.

	The arguments to this method, other than fromMonitoredTarget, correspond to the arguments of ReferenceMaker::NotifyRefChanged.
	\param changeInt - This is the interval of time over which the message is active.
	\param hTarget - This is the handle of the reference target the message was sent by.
	\param partID - This contains information specific to the message passed in.
	\param message - The message which needs to be handled. 
	\param fromMonitoredTarget - true if message orginated from RefTargMonitorRefMaker's monitored target.
	\returns Typically REF_SUCCEED indicating the message was processed. 
	*/
	virtual RefResult ProcessRefTargMonitorMsg(
						Interval changeInt, 
						RefTargetHandle hTarget, 
						PartID& partID,  
						RefMessage message,
						bool fromMonitoredTarget) = 0;

	//! \brief Calls to the RefTargMonitorRefMaker instance's EnumDependents method are passed to its owner through this method. 
	/*! The RefTargMonitorRefMaker owner is responsible for ensuring that an infinite recursion  does not occur. 
	Typically the owner would set a flag while enumerating dependents, and not start a new enumeration if that flag
	is set. 
	\param dep - the DependentEnumProc.
	\returns 1 to stop the enumeration and 0 to continue. 
	*/
	virtual int ProcessEnumDependents(DependentEnumProc* dep) = 0;
};

//! \brief The interface for an object to specify the ReferenceTargets it is dependent on, but doesn't hold a reference to (i.e., weak references).
/*! The owner of a RefTargMonitorRefMaker-like instance would implement this interface to cause the ReferenceTargets it is dependent on, 
but doesn't hold a reference to, to be saved when the owner is saved, and loaded when the owner is loaded. When the scene is loaded,
SetWeakReference is called to set the weak reference. 

For an example, see the NodeTransformMonitor class in maxsdk\\samples\\controllers\\nodetransformmonitor.cpp.
\see RefTargMonitorRefMaker.
*/
class IWeakReferenceMaker {
public:
	//! \brief The number of weak references. 
	/*! \returns The number of weak references.
	*/
	virtual int NumWeakRefs() = 0;

	//! \brief Retrieve the indexed weak reference. 
	/*! \param i - The virtual array index of the weak reference to get.
	\returns The reference handle of the 'i-th' weak reference.
	*/
	virtual RefTargetHandle GetWeakReference(int i) = 0;

	//! \brief Set the indexed weak reference. 
	/*! \param i - The virtual array index of the weak reference to set.
	\param rtarg - The weak reference.
	*/
	virtual void SetWeakReference(int i, RefTargetHandle rtarg) = 0;

	//! \brief Specifies remapping of weak references on load. 
	/*! This method is used when you have modified a IWeakReferenceMaker to add or delete weak references, and are loading old files. 
	This method is called during the reference mapping process, after the Load() method is called. You determine what version is loading 
	in the Load(), and store the version in a variable which you can look at in RemapWeakRefOnLoad() to determine how to remap 
	weak references. The default implementation of this method just returns the same value it is passed, so you don't need to 
	implement it unless you have added or deleted weak references from your class.  
	\param iref - The input index of the weak reference. 
	\returns The output index of the weak reference.
	*/
	virtual int RemapWeakRefOnLoad(int iref) { return iref; }
};


//! \brief Defines a class for monitoring a ReferenceTarget
/*! Defines a class for monitoring a ReferenceTarget by holding a weak reference to it
(IsRealDependency returns false) and passing NotifyRefChanged messages to the owner of the
class instance. The owner would create an instance of this class passing itself as the 
owner and the ReferenceTarget to watch as the target. The owner must derive from IRefTargMonitor, 
and would typically derive from IWeakReferenceMaker. If the owner derives from IWeakReferenceMaker,
during scene load IWeakReferenceMaker::SetWeakReference is called to set the weak reference. Thus, 
you wouldn't normally call this class's Save or Load methods from the owner if it derives from 
IRefTargMonitor. 
\see IRefTargMonitor, IWeakReferenceMaker
*/
class RefTargMonitorRefMaker: public ReferenceMaker, public PostPatchProc {
private:
	IRefTargMonitor &mOwner;	// the owner of this instance
	RefTargetHandle mpTarget;	// the object being monitored

	//! \brief Private destructor for class instances. Instances should be deleted via DeleteThis()
	~RefTargMonitorRefMaker();

public:

	//! \brief Constructor for class instances.
	/*! \param myOwner - The owner of the instance. The owner is responsible for deleting this instance. The owner must 
	implement the IRefTargMonitor interface.
	\param theTarget - The object instance to be watched.
	*/
	CoreExport RefTargMonitorRefMaker(IRefTargMonitor &myOwner, RefTargetHandle theTarget = NULL);

	//! \brief Set the object being watched.
	/*! \param theTarget - The object instance to be watched.
	*/
	CoreExport void SetRef(RefTargetHandle theTarget);

	//! \brief Get the object being watched.
	/*! \returns The object instance being watched.
	*/
	CoreExport RefTargetHandle GetRef();

	//! \brief Save a pointer to the object being watched. 
	/*! The owner can call this method from its Save method it it wants to persistently watch the object 
	across of load/save. As noted above, if the owner derives from IWeakReferenceMaker, the owner typically
	would not call this method.
	\param isave - The ISave pointer passed to the owner's Save method.
	\returns One of the following values:
	IO_OK - The result was acceptable - no errors.
	IO_ERROR - This is returned if an error occurred.
	*/
	IOResult Save(ISave *isave);

	//! \brief Load a pointer to the object being watched. 
	/*! The owner can call this method from its Load method it it wants to persistently watch the object 
	across of load/save. The owner would create a new RefTargMonitorRefMaker setting itself as an owner, and 
	then call this method using that instance. As noted above, if the owner derives from IWeakReferenceMaker, 
	the owner typically would not call this method.
	\param iload - The ILoad pointer passed to the owner's Load method.
	\returns One of the following values:
	IO_OK - The result was acceptable - no errors.
	IO_ERROR - This is returned if an error occurred.
	*/
	IOResult Load(ILoad *iload);

	//! \brief The PostPatchProc used when cloning. 
	/*! The PostPatchProc is used when cloning since the watched object may or may not have also been cloned.
	When the owner is cloned, it would typically create a new RefTargMonitorRefMaker pointing at the cloned
	watched object. At the time the owner is cloned, the watched object may not have been cloned yet, or it 
	may not be cloned at all. The owner should check to see if the watched object has been cloned already, and
	set the new RefTargMonitorRefMaker to watch it if so. If not, the new RefTargMonitorRefMaker should be set 
	to watch the original watch object, and register this proc via remap.AddPostPatchProc. This proc will check
	after the cloning is complete whether the watch object was cloned, and if so set the clone as the watched 
	object. If it was not cloned, the original object will be watched by the new RefTargMonitorRefMaker.
	\param remap - The RemapDir passed to the owner's Clone method.
	\returns TRUE in all cases
	*/
	int Proc(RemapDir& remap);

	// ReferenceMaker Methods
	//! \brief This method will notify the RefTargMonitorRefMaker of changes to the watched object. 
	/*! Implemented by the system.
	Changes to the watched object cause messages to be passed to this method, and the implementation of this 
	method passes those messages to the owner via the owner's ProcessRefTargMonitorMsg callback
	\returns The value returned from the owner's ProcessRefTargMonitorMsg callback
	*/
	RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message );

	//! \brief Get the number of references instance makes.
	/*! Implemented by the system.
	RefTargMonitorRefMaker makes 1 reference - the watched object.
	\returns The number of references instance makes.
	*/
	int NumRefs();

	//! \brief Get the indexed reference.
	/*! Implemented by the system.
	The watched object should be accessed via GetRef and SetRef. This method is used by the system.
	\returns The watched object.
	*/
	RefTargetHandle GetReference(int i);

	//! \brief Set the indexed reference.
	/*! Implemented by the system.
	Sets the watched object.
	The watched object should be accessed via GetRef and SetRef. This method is used by the system.
	*/
	void SetReference(int i, RefTargetHandle rtarg);

	//! \brief Specifies that this reference to the watched object should not prevent the watched object from being deleted
	/*! Implemented by the system.
	\returns FALSE in all cases
	*/
	BOOL IsRealDependency(ReferenceTarget *rtarg);

	//! \brief Allows a reference target to enumerate all references to it.
	/*! Implemented by the system.
	This method allows a reference target to enumerate all references to it. The implementation of this method passes the 
	DependentEnumProc to the owner via the owner's ProcessEnumDependents callback.
	\returns 1 to stop the enumeration and 0 to continue
	*/
	int EnumDependents(DependentEnumProc* dep);

	// Animtable Methods
	//! \brief The instance's SuperClassID.
	//! \returns The instance's SuperClassID.
	SClass_ID	SuperClassID() { return REFTARGMONITORREFMAKER_SUPERCLASS_ID; }

	//! \brief Delete this instance.
	void DeleteThis();
};

//! RefMessage sent by NodeTransformMonitor's ProcessRefTargMonitorMsg when the monitored node is deleted
//! The hTarg argument is the node being deleted
#define REFMSG_NODETRANSFORMMONITOR_TARGET_DELETED				REFMSG_USER + 0x18f81902

//! \brief The interface for working with NodeTransformMonitor class.
/*! The NodeTransformMonitor class (defined in ctrl.dlc) is used to monitor a node for its REFMSG_CHANGE/PART_TM and 
REFMSG_TARGET_DELETED messages. The NodeTransformMonitor class creates a RefTargMonitorRefMaker instance
pointing at a node, and allows only the REFMSG_TARGET_DELETED message and the REFMSG_CHANGE message when
PartID is PART_TM to propogate to the NodeTransformMonitor's dependendents. To prevent circular message 
loops, the messages are not propogated if that message type is already being propogated.
*/
class INodeTransformMonitor : public FPMixinInterface  {
public:
	//! \brief Retrieves the node being watched.
	/*! \returns INode pointer to the node being watched.
	*/
	virtual INode* GetNode() = 0;
	//! \brief Sets the node being watched.
	/*! \param theNode - Pointer to node to be watched. Can be NULL.
	*/
	virtual void SetNode(INode *theNode) = 0;

	//! \brief Retrieves whether to monitor for REFMSG_CHANGE/PART_TM messages.
	/*! \returns True if to monitor for REFMSG_CHANGE/PART_TM messages.
	*/
	virtual bool GetForwardTransformChangeMsgs() = 0;
	//! \brief Sets whether to monitor for REFMSG_CHANGE/PART_TM messages.
	/*! \param state - True if to monitor for REFMSG_CHANGE/PART_TM messages.
	*/
	virtual void SetForwardTransformChangeMsgs(bool state) = 0;

	// --- Function publishing
	FPInterfaceDesc* GetDesc();    // <-- must implement 
	virtual Interface_ID GetID() { return IID_NODETRANSFORMMONITOR; }

	enum {  
		kfpGetnode, kfpSetnode,
		kfpGetforwardPartTM, kfpSetforwardPartTM,
	};

	BEGIN_FUNCTION_MAP
		PROP_FNS(kfpGetnode, GetNode, kfpSetnode, SetNode, TYPE_INODE);
		PROP_FNS(kfpGetforwardPartTM, GetForwardTransformChangeMsgs, kfpSetforwardPartTM, SetForwardTransformChangeMsgs, TYPE_bool);
	END_FUNCTION_MAP

};

#endif
