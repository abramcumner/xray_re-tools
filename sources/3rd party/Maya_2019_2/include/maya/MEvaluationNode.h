#ifndef _MEvaluationNode
#define _MEvaluationNode
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
// 
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>
#include <maya/MEvaluationNodeIterator.h>
#include <maya/MObject.h>


// ****************************************************************************
// FORWARD DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MEvaluationNode)

//! \ingroup OpenMaya
//! \sa MEvaluationNodeIterator
//! \brief Provides access to Evaluation Manager node information. 
/*!
	An evaluation node and the connections between them form an evaluation graph.
    Each evaluation context uses a unique evaluation graph with its own set of
    evaluation nodes. Evaluation graphs are scheduled to speed up the operations
    within Maya.  

	This class contains methods for querying which attributes or plugs on a node
	will be set dirty prior to evaluation by the evaluation manager.

	It is sometimes necessary to know the dirty
	state of plugs/attributes if your node is handling custom
	information.  This handling would be done in the MPxNode
	preEvaluation()/postEvaluation() methods.

	If you need to access the list of dirty plugs in the evaluation
	node then use the internal iterator:

		for( MEvaluationNodeIterator nodeIt = theNode.iterator();
			 ! nodeIt.isDone(); nodeIt.next() )
		{
			doSomePlugThing( nodeIt.plug() );
		}
*/
class OPENMAYA_EXPORT MEvaluationNode
{
public:
	~MEvaluationNode();
    MEvaluationNode(const MEvaluationNode& evalNode);
	
    int             parentCount     ()              const;
    MEvaluationNode parent          (int index)     const;
    int             childCount      ()              const;
    MEvaluationNode child           (int index)     const;

    MEvaluationNodeIterator	iterator( MStatus * ReturnStatus = nullptr ) const;
	bool			dirtyPlugExists	(const MObject& attribute, MStatus * ReturnStatus = nullptr ) const;
	MPlug			dirtyPlug		(const MObject& attribute, MStatus * ReturnStatus = nullptr ) const;

    MObject         dependencyNode  (MStatus * ReturnStatus = nullptr) const;
    MDataBlock      datablock       (MStatus * ReturnStatus = nullptr) const;

    void            connect(MEvaluationNode& child, MStatus * ReturnStatus = nullptr);

    static const char*	className();

private:
	friend class MEvaluationNodeIterator;
    friend class MGraphNodeIterator;
    friend class MEvaluationGraph;

	MEvaluationNode();
	MEvaluationNode( const void* );

	const void *fEvaluationNode;
    bool        fGeneric;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MEvaluationNode */
