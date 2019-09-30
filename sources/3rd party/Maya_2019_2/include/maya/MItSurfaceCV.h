#ifndef _MItSurfaceCV
#define _MItSurfaceCV
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
// CLASS:    MItSurfaceCV
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// DECLARATIONS

class MPtrBase;
class TsurfaceCVComponent;
class TcomponentList;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItSurfaceCV)

//! \ingroup OpenMaya
//! \brief NURBS surface CV iterator. 
/*!
Iterator class for NURBS surface CVs.

<b>Example:</b> Translates the CVs for a surface in the X direction (world space).

\code
    MItSurfaceCV cvIter( surface, true, &stat );
    MVector vector(1.0,0.0,0.0);

    if ( MStatus::kSuccess == stat ) {
        for ( ; !cvIter.isDone(); cvIter.nextRow() )
        {
            for ( ; !cvIter.isRowDone(); cvIter.next() )
            {
                cvIter.translateBy( vector, MSpace::kWorld );
            }
        }
        cvIter.updateSurface();    // Tell surface is has been changed
    }
    else {
        cerr << "Error creating iterator!" << endl;
    }
\endcode
*/
class OPENMAYA_EXPORT MItSurfaceCV
{
public:
	MItSurfaceCV( const MObject & surface, bool useURows = true,
				  MStatus * ReturnStatus = NULL );
	MItSurfaceCV( const MDagPath & surface,
				  const MObject & component = MObject::kNullObj,
				  bool useURows = true,
				  MStatus * ReturnStatus = NULL );
	virtual ~MItSurfaceCV();
	bool     isDone( MStatus * ReturnStatus = NULL ) const;
	bool     isRowDone( MStatus * ReturnStatus = NULL ) const;
	MStatus  next();
	MStatus  nextRow();
	MStatus  reset();
	MStatus  reset( const MObject & surface, bool useURows = true );
	MStatus  reset( const MDagPath & surface,
					const MObject & component = MObject::kNullObj,
					bool useURows = true );
	MPoint   position( MSpace::Space space = MSpace::kObject,
					      MStatus * ReturnStatus = NULL ) const;
	MStatus  setPosition( const MPoint & point,
						  MSpace::Space space = MSpace::kObject );
	MStatus  translateBy( const MVector & vector,
						  MSpace::Space space = MSpace::kObject );
	int     index(	MStatus * ReturnStatus = NULL ) const;
	MStatus  getIndex( int& indexU, int& indexV ) const;
	// Obsolete
	OPENMAYA_DEPRECATED(2019, "Use MItSurfaceCV::currentItem instead.")
	MObject  cv( MStatus * ReturnStatus = NULL ) const;
	MObject  currentItem( MStatus * ReturnStatus = NULL ) const;

	bool     hasHistoryOnCreate( MStatus * ReturnStatus = NULL ) const;
	MStatus  updateSurface();

	static const char* 	className();

protected:
// No protected members

private:
	inline void * updateGeomPtr() const;
	MPtrBase *		f_shape;
	MPtrBase *		f_geom;
	void *			f_path;
	unsigned int		f_uindex;
	unsigned int		f_vindex;
	bool			f_in_u;
	void *			f_it;

	TcomponentList *      fCompList;
	TsurfaceCVComponent * fcvComp;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItSurfaceCV */
