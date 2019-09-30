#ifndef _MItCurveCV
#define _MItCurveCV
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
// CLASS:    MItCurveCV
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// DECLARATIONS

class MPtrBase;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItCurveCV)

//! \ingroup OpenMaya
//! \brief Iterator for NURBS curve CVs. 
/*!
This class is the iterator class for NURBS curve control vertices (CVs).
The iteration can be for a given curve or for a group of CVs.

<b>Example:</b> Translates the CVs for a curve in the X direction (world space).

\code
    MItCurveCV* cvIter = new MItCurveCV( curve, &stat );
    MVector vector(1.0,0.0,0.0);

    if ( MStatus::kSuccess == stat ) {
        for ( ; !cvIter->isDone(); cvIter->next() )
        {
            cvIter->translateBy( vector, MSpace::kWorld );
        }
        cvIter->updateCurve();	// Tell curve is has been changed
    }
    else {
        cerr << "Error creating iterator!" << endl;
    }
\endcode
*/
class OPENMAYA_EXPORT MItCurveCV
{
public:
    MItCurveCV( MObject & curve, MStatus * ReturnStatus = NULL );
    MItCurveCV( const MDagPath & curve,
				MObject & component = MObject::kNullObj,
                MStatus * ReturnStatus = NULL );
    virtual ~MItCurveCV();
    bool        isDone( MStatus * ReturnStatus = NULL ) const;
    MStatus     next();
    MStatus     reset();
    MStatus     reset( MObject & curve );
    MStatus     reset( const MDagPath & curve,
						MObject & component = MObject::kNullObj );
    MPoint      position( MSpace::Space space = MSpace::kObject,
                          MStatus * ReturnStatus = NULL ) const;
    MStatus     setPosition( const MPoint & pt,
							 MSpace::Space space = MSpace::kObject );
    MStatus     translateBy( const MVector & vec,
							 MSpace::Space space = MSpace::kObject );
    int	    index( MStatus * ReturnStatus = NULL ) const;
	// Obsolete
	OPENMAYA_DEPRECATED(2019, "Use MItCurveCV::currentItem instead.")
	MObject		cv( MStatus * ReturnStatus = NULL ) const;
	MObject		currentItem( MStatus * ReturnStatus = NULL ) const;

	bool        hasHistoryOnCreate( MStatus * ReturnStatus = NULL ) const;
    MStatus     updateCurve();

    static const char* className();

protected:
// No protected members

private:
	inline void * updateGeomPtr() const;
    MPtrBase * f_shape;
    MPtrBase * f_geom;
    void *     f_path;
	void *     f_it;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItCurveCV */
