#ifndef _MItGeometry
#define _MItGeometry
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
// CLASS:    MItGeometry
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItGeometry)

//! \ingroup OpenMaya
//! \brief Iterator class for geometry data. 
/*!
This class is the iterator class for geometry data, and can be used to
loop over the CVs of NURBS, the points of subds & lattices, and the vertices of
polygonal meshes.

\par Examples:
Simple traversal.

\code
    MItGeometry iter( dagPath );
    for ( ; !iter.isDone(); iter.next() )
    {
        MPoint pt = iter.position();

        // do something with it
    }
\endcode

Traversal of a geometry group within a compute() method.

\code
    MStatus
    exampleIterator::compute(const MPlug& plug, MDataBlock& dataBlock)
    {
        MStatus status;

        if (plug.attribute() == oOutputGeometry) {
            // get the input geometry and input groupId
            MDataHandle hInputGeom = dataBlock.inputValue(inputGeomAttr);
            MDataHandle hGroupId = dataBlock.inputValue(inputGroupIdAttr);

            unsigned int groupId = hGroup.asLong();
            MDataHandle hOutput = dataBlock.outputValue(plug);
            hOutput.copy(hInputGeom);

            // do an iteration where we get each point and set it to a new value
            MItGeometry iter(hOutput,groupId,false);
            for ( ; !iter.isDone(); iter.next()) {
                MPoint pt = iter.position();

                // do something here to modify the point ...

                iter.setPosition(pt);
            }
        } else {
            status = MStatus::kUnknownParameter;
        }        

        return status;
    }
\endcode
*/
class OPENMAYA_EXPORT MItGeometry
{
public:
	MItGeometry( const MDagPath& dagPath,
				 MStatus * ReturnStatus = NULL);
    MItGeometry( const MDagPath& dagPath,
                 MObject & component,
                 MStatus * ReturnStatus = NULL );
    MItGeometry( MObject& dagObject,
                 MStatus * ReturnStatus = NULL );
    MItGeometry( MDataHandle& dataHandle,
                 unsigned int groupId,
                 bool readOnly = true,
                 MStatus * ReturnStatus = NULL );
    MItGeometry( MDataHandle& dataHandle,
                 bool readOnly = true,
                 MStatus * ReturnStatus = NULL );
    MItGeometry( MDataHandle& dataHandle,
                 MObject & component,
                 bool readOnly = true,
                 MStatus * ReturnStatus = NULL );
    virtual ~MItGeometry();
    bool        isDone( MStatus * ReturnStatus = NULL ) const;
    MStatus     next();
    MPoint      position( MSpace::Space space = MSpace::kObject,
                          MStatus * ReturnStatus = NULL ) const;
    MVector     normal( MSpace::Space space = MSpace::kObject,
                          MStatus * ReturnStatus = NULL ) const;
    MStatus     setPosition( const MPoint& point,
                             MSpace::Space space = MSpace::kObject  );
	MWeight		weight( MStatus * ReturnStatus = NULL ) const;
    int	    index( MStatus * ReturnStatus = NULL ) const;
	// obsolete
	OPENMAYA_DEPRECATED(2019, "Use MItGeometry::currentItem instead.")
	MObject		component( MStatus * ReturnStatus = NULL ) const;
	MObject		currentItem( MStatus * ReturnStatus = NULL ) const;
	int		count( MStatus * ReturnStatus = NULL ) const;
	int		exactCount( MStatus * ReturnStatus = NULL );
	MStatus		reset( );

	MStatus		allPositions( MPointArray& points,
							  MSpace::Space space = MSpace::kObject  ) const;
	MStatus		setAllPositions( const MPointArray& points,
								 MSpace::Space space = MSpace::kObject  );

    static const char* className();

protected:
// No protected members

private:
	// internal : called by the dataHandle constructors to set up the iterator
	//
	void 		createIterator(MDataHandle& dataHandle,
							   bool useComponents,
							   void* comps,
							   unsigned int groupId,
							   bool readOnly,
							   MStatus* status);

private:
	void *      f_it;
    void *      f_path;
    void *      f_matrix;
    void *      f_clist;
	bool        f_readOnly;
	bool        f_own;

	MItGeometry( void * ptr );
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItGeometry */
