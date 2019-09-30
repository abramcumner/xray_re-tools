#ifndef _MTopologyClusterNode
#define _MTopologyClusterNode
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
#include <maya/MPxNode.h>
#include <maya/MEvaluationNode.h>

// ****************************************************************************
// FORWARD DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN
// ****************************************************************************
// CLASS DECLARATION (MTopologyClusterNode)

//! \ingroup OpenMaya
//! \brief Topology cluster representing a fine grain evaluation graph.

class OPENMAYA_EXPORT MTopologyClusterNode
{
public:
	virtual ~MTopologyClusterNode();

    MPxTopologyEvaluator	evaluator(MStatus* ReturnStatus) const;
    MEvaluationGraph        ownerGraph(MStatus* ReturnStatus) const;


    class OPENMAYA_EXPORT Evaluation_task
    {
    public:
        // just for compilation
        virtual ~Evaluation_task() {}

        virtual void execute() = 0;
        virtual void destroy() = 0;

        static void* allocate    (size_t size);
        static void  deallocate  (void*, size_t size);
    };

    template<typename Body>
    class Evaluation_task_body : public Evaluation_task
    {
    public:
        void execute() override { fBody(); }
        void destroy() override
        {
            this->~Evaluation_task_body();
            Evaluation_task::deallocate(this, sizeof(Evaluation_task_body<Body>));
        }

        static Evaluation_task_body<Body>* construct(const Body &body)
        {
            void* mem = Evaluation_task::allocate(sizeof(Evaluation_task_body<Body>));
            return new(mem) Evaluation_task_body(body);
        }

    private:
        Evaluation_task_body(const Body &body) 
            : fBody(body) { }
        
        ~Evaluation_task_body() override {}

        Body              fBody;	        //!< Function object providing execution "body" for task
    };

    template<typename Task>
    MEvaluationNode     createNode(const char* nodeName, const Task& body, MPxNode::SchedulingType schedulingType, const MEvaluationNode* associateWith, bool associateForEvaluation)
    {
        auto evaluationTask = Evaluation_task_body<Task>::construct(body);
        return createNode_Imp(nodeName, evaluationTask, schedulingType, associateWith, associateForEvaluation);
    }

    MEvaluationNode     createNode(const char* nodeName, const MEvaluationNode& node, MPxNode::SchedulingType schedulingType);

    MEvaluationNode     connectionPort(const MEvaluationNode& outsideNode, const MEvaluationNode& insideNode) const;

    MObject             asMObject(MStatus* ReturnStatus = nullptr) const;
    static const char*	className();

private:
    friend class MGraphNodeIterator;

    MEvaluationNode     createNode_Imp(const char* nodeName, Evaluation_task* taskFn, MPxNode::SchedulingType schedulingType, const MEvaluationNode* associateWith, bool associateForEvaluation);

    MTopologyClusterNode(const void* ownerData);
    const void *fEvaluatorClusterNode;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MTopologyClusterNode */
