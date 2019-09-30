#ifndef _MPxGeometryFilter
#define _MPxGeometryFilter
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//
// CLASS:    MPxGeometryFilter
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MSelectionList.h>


// ****************************************************************************
// CLASS DECLARATION (MPxGeometryFilter)



OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaAnim MPx
//! \brief Base class for user-defined deformers 
/*!
 MPxGeometryFilter allows the creation of user-defined deformers.  A deformer
 is a node which takes any number of input geometries, deforms them, and
 places the output into the output geometry attribute.

 If you write a deformer by deriving from MPxGeometryFilter, your deformer will
 derive the benefit of Maya's internal deformer functionality, namely:

   \li The Set Editing Tool and the Set Editor Window can be used to modify the membership of the deformer.
   \li "Has No Effect" mode is implemented in the base class, and disables the node.
   \li When the deformer node is deleted, its inputs and outputs will be correctly reconnected in the dependency graph.
   \li Reordering of the deformer node with other deformers via the Complete List window.
   \li Placement and connection of the deformer node in the dependency graph via the "deformer -type" command. The deformer command also has the standard deformer options such as exclusive membership and parallel mode.


 Deformers are full dependency nodes and can have attributes and a
 deform() method. In general, to derive the full benefit of the Maya
 deformer base class, it is suggested that you do not write your own
 compute() method. Instead, write the deform() method, which is called
 by the MPxGeometryFilter's compute() method. However, there are some
 exceptions when you would instead write your own compute(), namely:

   \li Your node's deformation algorithm depends on the geometry type, which is not available in the deform() method.
   \li Your node's deformation algorithm requires computing all of the output geometries simultaneously.


In the case where you cannot simply override the deform() method, the
following example code shows one possible compute() method
implementation. This compute() example creates an iterator for the deformer
set corresponding to the output geometry being computed. Note that this
sample compute() implementation does not do any deformation, and does
not implement handling of the nodeState attribute. If you do choose to
override compute() in your node, there is no reason to implement the
deform() method, since it will not be called by the base class.

\code
MStatus exampleDeformer::compute(const MPlug& plug, MDataBlock& dataBlock)
{
    MStatus status = MStatus::kUnknownParameter;
 	if (plug.attribute() == outputGeom) {
		// get the input corresponding to this output
		//
		unsigned int index = plug.logicalIndex();
		MObject thisNode = this->thisMObject();
		MPlug inPlug(thisNode,input);
		inPlug.selectAncestorLogicalIndex(index,input);
		MDataHandle hInput = dataBlock.inputValue(inPlug);

		// get the input geometry and input groupId
		//
		MDataHandle hGeom = hInput.child(inputGeom);
		MDataHandle hGroup = hInput.child(groupId);
		unsigned int groupId = hGroup.asLong();
		MDataHandle hOutput = dataBlock.outputValue(plug);
		hOutput.copy(hGeom);

		// do the deformation
		//
		MItGeometry iter(hOutput,groupId,false);
		for ( ; !iter.isDone(); iter.next()) {
			MPoint pt = iter.position();

			//
            // insert deformation code here
			//

			iter.setPosition(pt);
		}
		status = MStatus::kSuccess;
	}
	return status;
}
\endcode

For most deformers, implementing compute() is unnecessary. To create a
deformer, derive from this class and override the deform() method as
demonstrated in the "offsetNode.cpp" example plug-in.  The other
methods of the parent class MPxNode may also be overridden to perform
standard dependency node capabilities.

When implementing the compute method for a
deformer, another consideration is that the input geometry attribute is not
cached. This means that all of the inputs will evaluate each time
MDataBlock::inputArrayValue is called on "inputGeom". If you only want
a single inputGeometry, you can prevent unneeded evaluations by avoiding
calls to MDataBlock.inputArrayValue. For example, use the technique shown in the
above example or use MDataBlock::outputArrayValue.
*/
class OPENMAYAANIM_EXPORT MPxGeometryFilter : public MPxNode
{
public:

	//! Deformation details.
	enum DeformationDetails {
		kDeformsUVs			= (1<<1),			//!< The deformer will deform UVs.
		kDeformsColors		= (1<<2),			//!< The deformer will deform colors.

		kDeformsAll			= (kDeformsUVs|kDeformsColors),
	};

	MPxGeometryFilter();

	virtual ~MPxGeometryFilter();

	virtual MPxNode::Type type() const;

	// Methods to overload

	// deform is called by computePlug when an output geometry
	// value is evaluated
	//

    virtual MStatus        deform(MDataBlock& block,
								  MItGeometry& iter,
								  const MMatrix& mat,
								  unsigned int multiIndex);

	// return the attribute that gets connected to the
	// deformer tool shape
	//
	virtual MObject&    	accessoryAttribute() const;

	// called at creation time so that the node can create and attach any
	// that it needs in order to function
	//
	virtual MStatus			accessoryNodeSetup(MDagModifier& cmd);

	void					setUseExistingConnectionWhenSetEditing(bool state);

	MStatus					setDeformationDetails( unsigned int flags );
	unsigned int			getDeformationDetails( MStatus * ReturnStatus = NULL );

	// called when the set being deformed by this deformer has been modified to
	// add/remove this selection list.
	//
	virtual void			setModifiedCallback( MSelectionList& list,
												 bool listAdded );

	// Inherited attributes
	//! input attribute, multi
	static MObject input;
	//! input geometry attribute
	static MObject inputGeom;
	//! input group id attribute
	static MObject groupId;
	//! geometry output attribute
	static MObject outputGeom;
	//! envelope attribute
	static MObject envelope;

	static const char*	    className();

protected:
// No protected members

private:
	static void				initialSetup();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxGeometryFilter */
