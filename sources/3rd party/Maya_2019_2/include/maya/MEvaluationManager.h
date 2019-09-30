#ifndef _MEvaluationManager
#define _MEvaluationManager
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MEvaluationManager
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MDGContext.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MEvaluationManager)

//! \ingroup OpenMaya
//! \brief Class for working with Maya's Evaluation Manager. 
/*!
  The Evaluation Manager is used to evaluate the scene without dirty
  propagation. Its evaluation graph capabilities decouples the data model 
  and evaluation.

  The Evaluation Manager can execute graph evaluation in serial or parallel.
  Parallel execution uses a scheduling graph to provide parallelized
  computation of depend nodes.

  Unless specified, the methods in this class are threadsafe and can be
  performed safely from worker threads.
*/

class OPENMAYA_EXPORT MEvaluationManager
{
public:
    static bool graphConstructionActive	();
	static bool	evaluationManagerActive	( const MDGContext& context );
	
	static bool	evaluationInExecution	( const MDGContext& context );
    
    static const char* className();
private:
    MEvaluationManager();
    ~MEvaluationManager();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MEvaluationManager */

