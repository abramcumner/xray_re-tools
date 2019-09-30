#ifndef _MPxMotionPathNode
#define _MPxMotionPathNode
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//
// CLASS:	MPxMotionPathNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MPxNode.h>
#include <maya/MEulerRotation.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxMotionPathNode)

//! \ingroup OpenMaya MPx
//! \brief Base class for user defined motionPath nodes. 
/*!
   MPxMotionPathNode provides you with the ability to write your own motion
   path classes. A custom motionPath node provides access to Maya's internal
   motionPath engine while at the same time allows you to extend the
   functionality in ways that are not possible with the motionPath node.

   There are two basic ways to work with the methods available in this class.
   The first way is to mimic how Maya's motionPath node works and then
   augment the result. You do this by calling the <b>evaluatePath</b> method
   with values you either read from the input plugs or modified yourself,
   and the method returns the location and orientation on the path.

   The second way of working is to use the individual a-la-carte evaluator
   methods provided in this class to invoke the motionPath engine on a
   piecemeal basis. For example, you might call <b>position()</b> to
   calculate the location on the path, then <b>getVectors()</b> to
   compute the coordinate space vectors, then offset the position along
   those vectors, then call <b>banking()</b> to "tilt" the result. The
   example plug-in "motionPathNode.cpp" found in the API Developer's
   Toolkit provides an example of using the a-la-carte evaluation scheme
   to add sinusoidal offsets to the computed result.

   Here are some examples of how you might use a custom motionPath node:
   <ul>
	<li>Change how the range is interpreted, such as having 0 at the start
		of the path and 100 at the end regardless of the curve parameterisation.
		You could also implement wraparound, cycling, and oscillation when 
		"u" is oustide the range.</li>
	<li>Modify the output of the motionPath engine calculation by adding
		your own custom offsets, rotations and other enhancements based
		on inputs to your custom node.</li>
    <li>If you want a behaviour where an object anticipates the turns
		ahead such as on a roller-coaster (i.e. starts to lean before
		reaching the turn), you do one extra evaluation a certain distance
		ahead on the path then adjust the rotation to point at that
		location.</li>
	<li>To model some objects spaced a certain distance along a path,
		you could input the spacing as a multi attribute, have the "u"
		attribute as the location of the first object, calculate all the
		positions when your <b>compute()</b> method is called, then feed
		the resulting position and orientation into an output multi that
		each geometry connects to.</li>
	<li>Similar to the above, create rows and columns of people marching in
		a parade, with each row turning smartly as the parade rounds a
		corner.</li>
	<li>Adding randomness and simple bounding-box avoidance logic, you could
		model a crowd moving in a general direction along a path but slowly
		dispersing.</li>
	</ul>

   The example plug-in, "motionPathNode.cpp" found in the API Developer's
   Toolkit provides a great starting point for exploring the capabilities
   of the MPxMotionPathNode class.
*/
class OPENMAYAANIM_EXPORT MPxMotionPathNode : public MPxNode
{
public:
			MPxMotionPathNode();
	virtual	~MPxMotionPathNode();

	virtual MPxNode::Type type() const;

	//
	// Inherited attributes
	//

	//! u value
	static MObject uValue;

	//! front twist
	static MObject frontTwist;

	//! up twist
	static MObject upTwist;

	//! side twist
	static MObject sideTwist;

	//! flow node
	static MObject flowNode;

	//! path geometry
	static MObject pathGeometry;

	//! position marker time
	static MObject positionMarkerTime;

	//! orientation marker time
	static MObject orientationMarkerTime;

	//! follow
	static MObject follow;

	//! normal
	static MObject normal;

	//! inverse up
	static MObject inverseUp;

	//! inverse front
	static MObject inverseFront;

	//! front axis
	static MObject frontAxis;

	//! up axis
	static MObject upAxis;

	//! world up type
	static MObject worldUpType;

	//! world up vector
	static MObject worldUpVector;
	static MObject worldUpVectorX;
	static MObject worldUpVectorY;
	static MObject worldUpVectorZ;

	//! world up matrix
	static MObject worldUpMatrix;

	//! bank
	static MObject bank;

	//! bank scale
	static MObject bankScale;

	//! bank threshold
	static MObject bankThreshold;

	//! fraction mode
	static MObject fractionMode;

	//! update orientation markers
	static MObject updateOrientationMarkers;

	//! possible values for <b>worldUpType</b>
	enum worldUpVectorValue
	{
		//! Use the scene up vector as world up
		kUpScene,
		//! Use the object's up vector as world up
		kUpObject,
		//! Use the object's rotation up vector as world up
		kUpObjectRotation,
		//! Use the value of the <b>worldUpVector</b> plug as world up
		kUpVector,
		//! Use the path normal world up
		kUpNormal
    };

	//! The computed world space position
	static MObject allCoordinates;

	//! X-component of the computed world space position
	static MObject xCoordinate;

	//! Y-component of the computed world space position
	static MObject yCoordinate;

	//! Z-component of the computed world space position
	static MObject zCoordinate;

	//! The computed world space orientation matrix
	static MObject orientMatrix;

	//! The computed world space rotation
	static MObject rotate;

	//! Angle of rotation about the X axis
	static MObject rotateX;

	//! Angle of rotation about the Y axis
	static MObject rotateY;

	//! Angle of rotation about the Z axis
	static MObject rotateZ;

	//! The order of rotations for the <b>rotate</b> attribute
	static MObject rotateOrder;

	// A-la-carte access to the individual components of the motionPath
	// evaluator.
	//
	MPoint					position(
									MDataBlock&				data,
									double					f,
									MStatus*				status = NULL ) const;
	MStatus					getVectors(
									MDataBlock&				data,
									double					f,
									MVector&				front,
									MVector&				side,
									MVector&				up,
									const MVector*			worldUp = NULL ) const;
	MQuaternion				banking(
									MDataBlock&				data,
									double					f,
									const MVector&			worldUp,
									double					bankScale,
									double					bankLimit,
									MStatus*				status = NULL ) const;

	// Evaluator that follows the order the motionPath node calculates
	// the result. If you want to change how various aspects of the
	// calculation are performed, combine the individual methods defined
	// in the a-la-carte section above.
	//
	MStatus					evaluatePath(
									MDataBlock&				data,
									double					u,
									double					uRange,
									bool					wraparound,
									double					sideOffset,
									double					upOffset,
									bool					follow,
									bool					inverseFront,
									bool					inverseUp,
									int						frontAxis,
									int						upAxis,
									double					frontTwist,
									double					upTwist,
									double					sideTwist,
									bool					bank,
									double					bankScale,
									double					bankLimit,
									MPoint&					resultPosition,
									MMatrix&				resultOrientation ) const;

	// Utility methods.
	//
	double  				parametricToFractional(
									double					u,
									MStatus*				status = NULL ) const;
	double  				fractionalToParametric(
									double					f,
									MStatus*				status = NULL ) const;
	double					wraparoundFractionalValue(
									double					f,
									MStatus*				status = NULL ) const;
	MMatrix					matrix(
									const MVector&			front,
									const MVector&			side,
									const MVector&			up,
									int						frontAxisIdx = 1,
									int						upAxisIdx = 2,
									MStatus*				status = NULL ) const;

private:
	static void				initialSetup();
	static const char*		className();
	static void*			dataBlock( MDataBlock& );

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxMotionPathNode */
