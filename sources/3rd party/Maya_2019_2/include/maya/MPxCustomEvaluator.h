#ifndef _MPxCustomEvaluator
#define _MPxCustomEvaluator
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
#include <maya/MCustomEvaluatorClusterNode.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// CLASS DECLARATION (MPxCustomEvaluator)
//! \ingroup OpenMaya
//! \brief Base class for custom evaluation overrides in the evaluation manager.
/*!

*/

class OPENMAYA_EXPORT MPxCustomEvaluator
{
public:
	MPxCustomEvaluator();
	virtual ~MPxCustomEvaluator();

    bool    active      (MStatus* ReturnStatus=NULL)					const;
    void    setActive   (bool isNowActive, MStatus* ReturnStatus=NULL);

    bool	ready		(MStatus* ReturnStatus=NULL)					const;
	void	setReady	(bool isNowReady, MStatus* ReturnStatus=NULL);

	//! Defines the type of consolidation during cluster creation
	enum ConsolidationType
	{
		kConsolidateNone,		//!< Create single cluster per node/cycle cluster
		kConsolidateSubgraph,	//!< Group nodes to form subgraph cluster, favoring long clusters
		kConsolidateFlood,		//!< Group nodes to form subgraph cluster, favoring big clusters and partitioning speed
	};
	ConsolidationType	consolidation	(MStatus* ReturnStatus=NULL)	const;
	void				setConsolidation(ConsolidationType newType, MStatus* ReturnStatus=NULL);

    virtual bool	markIfSupported		(const MEvaluationNode* node); 

    virtual MCustomEvaluatorClusterNode::SchedulingType schedulingType	(const MCustomEvaluatorClusterNode* cluster);

    virtual void	preEvaluate			(const MEvaluationGraph* graph);
    virtual void	postEvaluate		(const MEvaluationGraph* graph);

    virtual bool	clusterInitialize	(const MCustomEvaluatorClusterNode* cluster);
    virtual bool	clusterReady		(const MCustomEvaluatorClusterNode* cluster);
    virtual void	clusterTerminate	(const MCustomEvaluatorClusterNode* cluster);

    virtual void	clusterEvaluate		(const MCustomEvaluatorClusterNode* cluster);

    static const char*	className();

private:
    friend class MCustomEvaluatorClusterNode;
    friend class MEvaluationNodeIterator;
    friend class MFnPlugin;

    MPxCustomEvaluator(const void* ownerData);
    const void  *fEvaluator;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MPxCustomEvaluator */