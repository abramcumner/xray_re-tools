#ifndef _MFnClip
#define _MFnClip
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
// CLASS:    MFnClip
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnClip)

//! \ingroup OpenMayaAnim MFn
//! \brief clip function set 
/*!
  Maya uses animation clips as the building block of its nonlinear
  animation system, "Trax". An animation clip allows the user to
  bundle a set of animation curves independent of time so that they
  can be reused multiple times, with different timing then the
  original clip.

  Character sets (MFnCharacter) are integral to Maya's clip architecture,
  since a clip contains whatever attributes of the character were animated
  when the clip was created. For more information on character sets, refer
  to the MFnCharacter documentation.

  Clips in maya can be of two types: source clips and scheduled
  clips. In the Maya UI, source clips are visible in the Visor while
  scheduled clips are visible in Trax. A source clip contains the
  animation curves for the clip. A scheduled clip contains data about
  the placement of an instance of a source clip in the Maya
  timeline. In this context, an "instance" means that the animation
  curves from the source clip are shared by the scheduled
  clip. Scheduled clips never contain their own animation curves, they
  always refer to a source clip's curves.

  For example, if you create a clip called "run" in maya that lasts
  from frames 1-20, a source clip node will be created with a start of
  1, a duration of 19, and dependency graph connections to all of the
  animation curves that make up the "run". If you then place an
  instance of the run clip at frame 5 and another instance of the run
  clip at frame 20, you have 2 scheduled clips: one with a start frame
  of 5 and one with a start frame of 20. As mentioned in the previous
  paragraph, only a single set of animation curves exist for the
  run regardless of the number of times the run is scheduled.

  Trax also allows you to create "blends" between clips, which enable you to
  control the transition between the clips. A blend is represented in the
  dependency graph by an "animBlendInOut" node, which uses an animation curve
  to determine the transition type.

  In the dependency graph, when a character has animation clips, the
  character node will always be connected to a "clipLibrary" node and
  a "clipScheduler" node. The clipLibrary node is connected to all of
  the source clips and their animation curves. The clipScheduler node
  is connected to the scheduled clips and blends. It is the clipScheduler
  that computes the final animation by looking at the placement and overlap
  of the clips and feeding the attribute data back into the character set.

  To create a source clip, the typical steps are:

  <ol>
  <li>If the attributes are not already in a character, create a
  character containing the attributes.
  <li>Create a source clip using MFnClip::createSourceClip
  <li>Attach the clip to a character using
  MFnCharacter::attachSourceToCharacter
  <li>Attach animation curves to the clip using MFnCharacter::addCurveToClip
  </ol>

  To create a scheduled clip so that it appears in the Trax Editor,
  the typical steps are:

  <ol>
  <li>Create an instanced animClip node using
  MFnClip::createInstancedClip, and associate it with the source clip
  that you want to instance.
  <li>Attach the clip instance to the character using
  MFnCharacter::attachInstanceToCharacter
  </ol>
*/

class OPENMAYAANIM_EXPORT MFnClip : public MFnDependencyNode
{
	declareMFn(MFnClip, MFnDependencyNode);

public:
	MObject		createSourceClip ( const MTime& sourceStart,
								   const MTime& sourceDuration,
								   MDGModifier& dgMod,
								   MStatus * ReturnStatus = NULL );
	MObject		createInstancedClip ( MObject& sourceClip,
									  const MTime& start,
									  MDGModifier& dgMod,
									  MStatus * ReturnStatus = NULL,
									  bool absolute = 1,
									  double postCycle = 0.0,
									  double weight = 1.0,
									  double scale = 1.0,
			  						  double preCycle = 0.0 );
	bool 		isInstancedClip(MStatus *ReturnStatus = NULL);
	bool		isPose(MStatus *ReturnStatus = NULL);
	MObject 	sourceClip(MStatus *ReturnStatus = NULL);
	double		getPreCycle(MStatus *ReturnStatus = NULL);
	double		getPostCycle(MStatus *ReturnStatus = NULL);
	// Obsolete
	double		getCycle(MStatus *ReturnStatus = NULL);
	double		getWeight(MStatus *ReturnStatus = NULL);
	double 		getScale(MStatus *ReturnStatus = NULL);
	// Obsolete
	bool 		getAbsolute(MStatus *ReturnStatus = NULL);
	bool 		getEnabled(MStatus *ReturnStatus = NULL);
	MTime		getStartFrame(MStatus *ReturnStatus = NULL);
	MTime		getSourceStart(MStatus *ReturnStatus = NULL);
	MTime		getSourceDuration(MStatus *ReturnStatus = NULL);
	MStatus		getMemberAnimCurves(MObjectArray& curves,
									MPlugArray& associatedAttrs);
	MStatus		getAbsoluteChannelSettings(MIntArray& absoluteChannels);
	int			getTrack(MStatus *ReturnStatus = NULL);
	MStatus		setPoseClip(bool state, MDGModifier* mod = NULL);
	MStatus		setPreCycle(double cycle, MDGModifier* mod = NULL);
	MStatus		setPostCycle(double cycle, MDGModifier* mod = NULL);
	// Obsolete
	MStatus		setCycle(double cycle, MDGModifier* mod = NULL);
	MStatus		setWeight(double wt, MDGModifier* mod = NULL);
	MStatus		setScale(double scale, MDGModifier* mod = NULL);
	// Obsolete
	MStatus		setAbsolute(bool abs, MDGModifier* mod = NULL);
	MStatus		setEnabled(bool val, MDGModifier* mod = NULL);
	MStatus		setStartFrame(const MTime& start, MDGModifier* mod = NULL);
	MStatus		setSourceData(const MTime& start,const MTime& duration,
							  MDGModifier* mod = NULL);
	MStatus		setTrack(int index, MDGModifier* mod = NULL);
	MStatus		setAbsoluteChannelSettings(const MIntArray& absoluteChannels,
										   MDGModifier* mod = NULL);
BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnClip, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnClip */
