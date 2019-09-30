#ifndef _MCustomEvaluatorClusterNode
#define _MCustomEvaluatorClusterNode
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>


// ****************************************************************************
// FORWARD DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN
// ****************************************************************************
// CLASS DECLARATION (MCustomEvaluatorClusterNode)

//! \ingroup OpenMaya
//! \brief Scheduling cluster representing a self-contained cluster of work.

class OPENMAYA_EXPORT MCustomEvaluatorClusterNode
{
public:

    //! Available Scheduling Types
    enum SchedulingType
    { 
        kParallel,			//! Any number of nodes of the same type can run in parallel,
        kSerial,			//! Node should be serialized locally, by chaining all nodes of this flag for sequential execution
        kGloballySerial,	//! Only one node of this type can be run at a time
        kUntrusted,			//! Node will do things that can't be predicted and
                            //! nothing else can be scheduled to execute together with it
        kLast				//! Just to mark the scheduling type count
    };

	virtual ~MCustomEvaluatorClusterNode();

    MPxCustomEvaluator	evaluator(MStatus* ReturnStatus) const;
    MEvaluationGraph    ownerGraph(MStatus* ReturnStatus) const;

    MCustomEvaluatorClusterNode::SchedulingType   schedulingType(MStatus* ReturnStatus = NULL) const;
    void                  evaluate(MStatus* ReturnStatus = NULL) const;
    void                  evaluateNode(const MEvaluationNode& node, MStatus* ReturnStatus) const;
    MObject               asMObject(MStatus* ReturnStatus = NULL) const;
    static const char*	  className();

private:
    friend class MGraphNodeIterator;

    MCustomEvaluatorClusterNode(const void* ownerData);
    const void *fEvaluatorClusterNode;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MCustomEvaluatorClusterNode */