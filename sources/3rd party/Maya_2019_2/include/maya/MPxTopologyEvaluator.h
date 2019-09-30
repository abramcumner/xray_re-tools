#ifndef _MPxTopologyEvaluator
#define _MPxTopologyEvaluator
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MEvaluationNode.h>
#include <maya/MEvaluationGraph.h>
#include <maya/MEvaluationNodeIterator.h>
#include <maya/MPxCustomEvaluator.h>
#include <maya/MTopologyClusterNode.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// CLASS DECLARATION (MPxTopologyEvaluator)
/*!
    \ingroup OpenMaya
    \brief Topology override evaluator for fine grain evaluation graphs
 
    Evaluation graph is representing evaluation requirements and dependencies
    between them. We coupled granularity of evaluation graph to granularity
    of dependency graph by making smallest execution equal to single dependency
    node. In many cases this causes no trouble and we can use custom evaluators
    to group bunch of nodes together and reduce the number of execution. But
    there are real cases where being able to express single dependency node
    as multiple executions (= multiple evaluation nodes) can give us better
    control about how things are evaluated and unlock even more performance.
    Some of this was possible as part of cluster evaluation, but evaluating
    a single node and scheduling evaluation of cluster evaluation was
    non-trivial and in many cases would just create instabilities.

    New concept was created which allows topology override of evaluation graph.
    This makes possible to have total control over granularity, leverage
    already existing scheduling constraints and scheduling graph. Supports
    partial evaluation for minimal evaluation of cluster content. Finally,
    allows better resource utilization since execution of cluster content
    doesn't have to wait for entire upstream to start and downstream can
    start as soon as dependencies are ready (sort of transparent clusters,
    but much more configurable and with no constraints on granularity).

    \note   This type of evaluator is claiming nodes in the same way
    as any other evaluator (i.e. marking if supported). Evaluation
    is completely done by subgraph nodes, meaning none of evaluation
    methods will be called on this evaluator and this is why they are
    final.
*/

class OPENMAYA_EXPORT MPxTopologyEvaluator
{
public:
    MPxTopologyEvaluator();
	virtual ~MPxTopologyEvaluator();

    bool    active      (MStatus* ReturnStatus=nullptr)					const;
    void    setActive   (bool isNowActive, MStatus* ReturnStatus=nullptr);

    virtual bool	    markIfSupported	(const MEvaluationNode* node); 

    virtual void	    preMakeTopology (const MEvaluationGraph* graph);
    virtual bool	    makeTopology    (MTopologyClusterNode* cluster);
    virtual void	    postMakeTopology(const MEvaluationGraph* graph);

    static const char*	className();

private:
    friend class MTopologyClusterNode;
    friend class MFnPlugin;

    MPxTopologyEvaluator(const void* ownerData);
    const void  *fEvaluator;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MPxTopologyEvaluator */
