#ifndef __MPxManipulatorNode_h
#define __MPxManipulatorNode_h
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
// CLASS:    MPxManipulatorNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MPxNode.h>
#include <maya/MTypes.h>
#include <maya/MGLdefinitions.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MPoint.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxManipulatorNode)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for manipulator creation 
/*!
MPxManipulatorNode is the base class used for creating user-defined
manipulators. This class is derived from MPxNode since manipulators in
Maya are dependency nodes.

An MPxManipulatorNode should implement the following method:

\code
virtual void draw(M3dView &view, const MDagPath &path,M3dView::DisplayStyle style, M3dView::DisplayStatus status);
\endcode

Additionally, several of the following virtuals will need to be defined:
\code
virtual MStatus	doPress( M3dView& view );
virtual MStatus	doDrag( M3dView& view );
virtual MStatus	doRelease( M3dView& view );
\endcode

Implement the following method to properly support undo:
\code
virtual MStatus connectToDependNode(const MObject &dependNode);
\endcode

The draw() method is very important since drawing and picking are
done together.  The colorAndName() method should be called before
drawing a GL component that should be pickable. Several color methods
which return color indexes that Maya use are provided to allow
custom manipulators to have a similar look.

When drawing a GL pickable component, an active name must be set.
Use the glFirstHandle() to get this value from the base class.

To draw the manipulator in Viewport 2.0, the plugin must also implement
preDrawUI() and drawUI(). Note that selection relies on the default
viewport draw pass so the draw() method must still be implemented even
if the manipulator is not intended for use in the default viewport.
preDrawUI() is called before drawUI() and should be used to prepare
and cache data which will be needed to draw the manipulator in drawUI().


A manipulator can be connected to a depend node instead of updating
a node attribute directly in one of the do*() methods.  To connect
to a depend node, you must:

\li Call add*Value() in the postConstructor() of the node
\li Call conectPlugToValue() in connectToDependNode()
\li Call set*Value() in one of the do*() virtuals

This class can work standalone or with MPxManipContainer.
*/
class OPENMAYAUI_EXPORT MPxManipulatorNode  : public MPxNode
{
public:
	MPxManipulatorNode();
	virtual ~MPxManipulatorNode();

	virtual MStatus connectToDependNode(const MObject &dependNode);

BEGIN_NO_SCRIPT_SUPPORT:
	// Viewport 2.0 manipulator draw overrides
	//! \noscript
	virtual void		preDrawUI( const M3dView &view );
	//! \noscript
	virtual void		drawUI(
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext ) const;
END_NO_SCRIPT_SUPPORT:

	virtual void draw(M3dView &view, const MDagPath &path,
				M3dView::DisplayStyle style, M3dView::DisplayStatus status);

	virtual MStatus	doPress( M3dView& view );
	virtual MStatus	doDrag( M3dView& view );
	virtual MStatus	doRelease( M3dView& view );
	virtual MStatus doMove( M3dView& view, bool& refresh );

	MStatus finishAddingManips();

	MStatus	colorAndName( M3dView& view, MGLuint glName, bool glNameIsPickable, short colorIndex ) const;
    OPENMAYA_DEPRECATED(2019, "This function is obsolete, please use 'setHandleColor' instead")
	MStatus shouldDrawHandleAsSelected(unsigned int name, bool&useSelectedColor) const;
	MStatus setHandleColor(MHWRender::MUIDrawManager& drawManager, unsigned int handleName, short colorIndex) const;

	MStatus glFirstHandle( MGLuint &firstHandle );

	MStatus glActiveName( MGLuint &glName );

	MStatus mouseRay( MPoint& linePoint, MVector& lineDirection ) const;
	MStatus mouseRayWorld( MPoint& linePoint, MVector& lineDirection ) const;

	MStatus mousePosition( short& x_pos, short& y_pos );
	MStatus mouseDown( short& x_pos, short& y_pos );
	MStatus mouseUp( short& x_pos, short& y_pos );

	MStatus registerForMouseMove();
	MStatus deregisterForMouseMove();

	MStatus addDoubleValue( const MString& valueName, double defaultValue, int& valueIndex );
	MStatus addPointValue( const MString& valueName, const MPoint& defaultValue, int& valueIndex );
	MStatus addVectorValue( const MString& valueName, const MVector& defaultValue, int& valueIndex );

	MStatus setDoubleValue( int valueIndex, double value );
	MStatus setPointValue( int valueIndex, const MPoint& value );
	MStatus setVectorValue( int valueIndex, const MVector& value );

	MStatus getDoubleValue( int valueIndex, bool previousValue, double& value );
	MStatus getPointValue( int valueIndex, bool previousValue, MPoint& value );
	MStatus getVectorValue( int valueIndex, bool previousValue, MVector& value );

	MStatus connectPlugToValue( const MPlug& plug, int valueIndex, int& plugIndex );

	static MPxManipulatorNode* newManipulator(const MString &manipName,
											  MObject &manipObject,
											  MStatus *ReturnStatus = NULL);

	MStatus addDependentPlug( const MPlug& plug );
	MStatus dependentPlugsReset();

	short mainColor() const;
	short xColor() const;
	short yColor() const;
	short zColor() const;
	short prevColor() const;
	short lineColor() const;
	short dimmedColor() const;
	short selectedColor() const;
	short labelColor() const;
	short labelBackgroundColor() const;

	//! Connected node, message attribute
	static MObject connectedNodes;

	// Internal method
	void *getInstancePtr();
	void setInstancePtr( void *ptr );

	static const char *className();

protected:
// No protected members

private:
	static void initialSetup();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* _cplusplus */
#endif /* MPxManipulatorNode */
