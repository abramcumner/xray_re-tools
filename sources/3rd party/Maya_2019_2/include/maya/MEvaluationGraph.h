#ifndef _MEvaluationGraph
#define _MEvaluationGraph
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
#include <maya/MEvaluationNodeIterator.h>
#include <maya/MObject.h>
#include <maya/MDGContext.h>


// ****************************************************************************
// FORWARD DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MEvaluationGraph)

//! \ingroup OpenMaya
//! \brief Evaluation graph object.

/*! 
	The evaluation graph holds information about what in the scene is
	considered to be live (or time dependent). It is constructed
 	out of dirty messages, and has full knowledge of dependencies between 
    live nodes. This is why it can be used to replace regular Maya evaluation 
    with dirty propagation.
 
 	The evaluation graph is linked to DG context and graph construction.
 	This context is fully respected during evaluation.
 
 	Evaluation with the evaluation graph can be done in different modes
 	and depending on settings, a different executor is used to traverse the graph
 	and do complete or partial graph computation. Unlike the DG,
 	the evaluation with evaluation graph is using forward evaluation model. Graph 
    topology is defined by directional connections between evaluation nodes.
*/
class OPENMAYA_EXPORT MEvaluationGraph
{
public:
    virtual ~MEvaluationGraph();

    MDGContext  context             (MStatus* ReturnStatus)  const;
    bool        partialEvaluation   ()  const;

    MEvaluationNode rootNode        ()  const;

    static MEvaluationGraph ownerGraph(const MEvaluationNode& evalNode);

    static const char* className();

private:
    friend class MCustomEvaluatorClusterNode;
    friend class MTopologyClusterNode;

    MEvaluationGraph(const void* ownerData);

    const void *fEvaluationGraph;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MEvaluationGraph */
