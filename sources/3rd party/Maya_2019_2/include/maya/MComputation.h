#ifndef _MComputation
#define _MComputation
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
// CLASS:    MComputation
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MComputation)

//! \ingroup OpenMaya
//! \brief Interupt monitor for long computations. 
/*!
An MComputation allows long computations to check for user interrupts.  It is
very simple to use.  Create a new <i>MComputation</i> object and call the
<i>beginComputation</i> method at the beginning of the computation and call
the <i>endComputation</i> method when you finish.  Then, during the computation,
use the <i>isInterruptRequested</i> method to check if the user has requested
that the computation terminate.

<b>Example:</b> (of a simple traversal)

\code
    MComputation computation;
    computation.beginComputation();
    for (int i= 1; i<1000; i++) {
        Computation();  // Some expensive operation
        if  (computation.isInterruptRequested()) break ;
    }
    computation.endComputation();
\endcode

An alternate form of computation with a progress indicator is supported.  In this case,
the <i>beginProgressiveComputation</i> method is called along with the progress methods.

<b>Example:</b> (of a simple progressive traversal)

\code
    MComputation computation;
    computation.beginComputation( true );
    computation.setProgressRange( 0, 100 );
    for (int i= 1; i<1000; i++) {
        Computation();  // Some expensive operation
        if  (computation.isInterruptRequested()) break ;
		computation.setProgress( (int)(i/1000.0 * 100.0) );
    }
    computation.endComputation();
\endcode
*/

class OPENMAYA_EXPORT MComputation {

public:
			MComputation();
	virtual	~MComputation();
    void	beginComputation( bool showProgressBar = false, bool isInterruptable = true, bool useWaitCursor = true );
    bool	isInterruptRequested();
    void	endComputation();

	MStatus	setProgressRange(const int minValue, const int maxValue);
	int progressMin(MStatus* ReturnStatus = NULL) const;
	int progressMax(MStatus* ReturnStatus = NULL) const;

	MStatus	setProgress(const int amount);
	int progress(MStatus* ReturnStatus = NULL) const;

	MStatus	setProgressStatus(const MString&);

    static const char* className();

protected:
// No protected members

private:
	void *f_data;
	bool useProgressBar;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MComputation */
