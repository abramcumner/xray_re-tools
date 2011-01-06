/*********************************************************************
 *<
	FILE: sceneTraversal.h

	DESCRIPTION: Interface for plugins controlling display traversal

	CREATED BY:	John Hutchinson

	HISTORY: November 5 2003

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#ifndef __SCENETRAVERSE__
#define __SCENETRAVERSE__

#include "maxheap.h"
#include "maxtypes.h"

#define SCENETRAVERSAL_INTERFACE Interface_ID(0x35380d8c, 0x48e54b8c)

class NodeIterator;
class ViewExp;

// An interface for controlling the display traversal of the scene
class ISceneTraversal : public BaseInterface {
	public:

	typedef enum {
		kDisplay = 0,
		kHitTest = 1,
	} IteratorContext;

      virtual NodeIterator *GetIterator(ViewExp* vpt, IteratorContext context) = 0;
	};

class NodeIterator: public MaxHeapOperators
{
public:
	virtual void DeleteThis()=0;	// used to implement { delete this; }
	virtual void Reset() = 0;
	virtual INode *next() = 0;
};



#endif //__SCENETRAVERSE__
