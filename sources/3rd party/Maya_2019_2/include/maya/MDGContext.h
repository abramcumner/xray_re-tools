#ifndef _MDGContext
#define _MDGContext
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
// CLASS:    MDGContext
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDGContext)

//! \ingroup OpenMaya
//! \brief Dependency graph (DG) context class. 
/*!

  Control the way in which dependency nodes are evaluated.

  DG contexts are used to define the way in which a dependency node is
  going to be evaluated. Examples of such contexts include "normal",
  "at a given time, "for a specific instance", etc.

  MDGContext is mainly used in two places; within methods that trigger
  evaluations, to define what kind of evaluate is being requested, and
  within data blocks (MDataBlock), to identify how the data was
  created.

  There is always the notion of the "current evaluation context". That's
  the one that will be used when no context is specifically mentioned,
  and is the normal state of evaluation.

  \note In the past the current and normal context were the same thing. With
        the advent of background evaluation and caching this is no longer
		necessarily true. Always use the current context, unless you mean to
		deliberately change to a specific context.
*/
class OPENMAYA_EXPORT MDGContext
{
public:

	// Normal
	MDGContext( );

	// Timed
	MDGContext( const MTime & when );

	MDGContext( const MDGContext& in );
	~MDGContext();

	// Default context, "Normal", reflects what is currently on screen
	static		MDGContext	fsNormal;
	bool     	isNormal( MStatus * ReturnStatus = NULL ) const;

	MStatus 	getTime( MTime & ) const;

	MDGContext&	operator =( const MDGContext& other );

	static const char* className();

	// Methods dealing with the current evaluation context
	static	const MDGContext	current();
						bool	isCurrent	(MStatus *ReturnStatus = NULL)	const;
			const MDGContext	makeCurrent	(MStatus *ReturnStatus = NULL)	const;

protected:
	// No protected members

private:
	const void * data;
	friend class MPlug;
	friend class MDataBlock;
	MDGContext( const void* );
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MDGContext */
