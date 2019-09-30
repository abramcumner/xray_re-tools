#ifndef _MPxManipContainer
#define _MPxManipContainer
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
// CLASS:    MPxManipContainer
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MPlug.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MObjectArray.h>
#include <maya/MManipData.h>
#include <maya/MPxManipulatorNode.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxManipContainer)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for user defined manipulator containers 
/*!
MPxManipContainer is the base class for user-defined container
manipulators.  This class is derived from MPxNode since manipulators
in Maya are dependency nodes.

MPxManipContainer is a container manipulator that has at least one
base manipulator. MPxManipContainer has methods for adding the
following base manipulators types to the container:
FreePointTriadManip, DirectionManip, DistanceManip, PointOnCurveManip,
PointOnSurfaceManip, DiscManip, CircleSweepManip, ToggleManip,
StateManip, and CurveSegmentManip.

A container manipulator has one converter which is the interface
between the container's children manipulators and the node plugs they
affect.  The values on the converter that are related to children
manipulators are called converterManipValues, and the values on the
converter that are related to the node plugs are called
converterPlugValues.

The conversion between converterManipValues and converterPlugValues
are performed through conversion callback methods, except when there
is a one-to-one connection between a converterManipValue and a
converterPlugValue.

There are two kinds of conversion callback methods: manipToPlug and
plugToManip.  A plugToManipConversionCallback is used to calculate a
converterManipValue from various converterPlugValues.  This callback
has access to all the converterPlugValues and returns the value of a
converterManipValue.  A manipToPlugConversionCallback is used to
calculate a converterPlugValue from various converterManipValues. This
callback has access to all the converterManipValues and returns the
value of a converterPlugValue.

In Viewport 2.0, all child manipulators will draw automatically. However for
custom drawing (things done in the draw() method for the default viewport),
the plugin must also implement preDrawUI() and drawUI(). Note that selection
relies on the default viewport draw pass so the draw() method must still be
implemented even if the manipulator is not intended for use in the default viewport.

preDrawUI() is called before drawUI() and should be used to prepare and cache data
which will be needed to perform the custom drawing in drawUI().


In order for an MPxManipContainer to be able to run, the manipulator
needs to know that the initialization is finished through the
finishAddingManips method.
*/
class OPENMAYAUI_EXPORT MPxManipContainer : public MPxNode
{
public:
	//! \brief Pointer to a plug-to-manip conversion callback function.
	/*!
	 \param[in] manipIndex Index of the manipulator value to be calculated.
	*/
	typedef MManipData (MPxManipContainer::*plugToManipConversionCallback)(unsigned int manipIndex);

	//! \brief Pointer to a manip-to-plug conversion callback function.
	/*!
	 \param[in] plugIndex Index of the plug value to be calculated.
	*/
	typedef MManipData (MPxManipContainer::*manipToPlugConversionCallback)(unsigned int plugIndex);

	//! Built-in manipulator types.
	enum baseType {
		kFreePointTriadManip,	//!< \nop
		kDirectionManip,	//!< \nop
		kDistanceManip,		//!< \nop
		kPointOnCurveManip,	//!< \nop
		kPointOnSurfaceManip,	//!< \nop
		kDiscManip,		//!< \nop
		kCircleSweepManip,	//!< \nop
		kToggleManip,		//!< \nop
		kStateManip,		//!< \nop
		kCurveSegmentManip,	//!< \nop
		kCustomManip		//!< \nop
	};

	MPxManipContainer();
	virtual ~MPxManipContainer();
	virtual MPxNode::Type type() const;
	static  MStatus		initialize();
	static MPxManipContainer * newManipulator(const MString &manipName,
											  MObject &manipObject,
											  MStatus *ReturnStatus = NULL);

BEGIN_NO_SCRIPT_SUPPORT:
	// Viewport 2.0 manipulator draw overrides
	//! \noscript
	virtual void		preDrawUI( const M3dView &view );
	//! \noscript
	virtual void		drawUI(
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext ) const;
END_NO_SCRIPT_SUPPORT:

	// Methods to overload
	virtual void			draw(M3dView &view,
								 const MDagPath &path,
								 M3dView::DisplayStyle style,
								 M3dView::DisplayStatus status);
	virtual MStatus			connectToDependNode(const MObject &dependNode);

	// Do not put any of these functions in the constructor.
	virtual MStatus			createChildren();

	MDagPath	addFreePointTriadManip		(const MString &manipName,
											 const MString &pointName);
	MDagPath	addDirectionManip			(const MString &manipName,
											 const MString &directionName);
	MDagPath	addDistanceManip			(const MString &manipName,
											 const MString &distanceName);
	MDagPath	addPointOnCurveManip		(const MString &manipName,
											 const MString &paramName);
	MDagPath	addPointOnSurfaceManip		(const MString &manipName,
											 const MString &paramName);
	MDagPath	addDiscManip				(const MString &manipName,
											 const MString &angleName);
	MDagPath	addCircleSweepManip			(const MString &manipName,
											 const MString &angleName);
	MDagPath	addToggleManip				(const MString &manipName,
											 const MString &toggleName);
	MDagPath	addStateManip				(const MString &manipName,
											 const MString &stateName);
	MDagPath	addCurveSegmentManip		(const MString &manipName,
											 const MString &startParamName,
											 const MString &endParamName);
	MDagPath	addRotateManip				(const MString &manipName,
											 const MString &rotationName);
	MDagPath	addScaleManip				(const MString &manipName,
											 const MString &scaleName);
	MStatus	addMPxManipulatorNode		(const MString &manipTypeName,
											 const MString &manipName,
											 MPxManipulatorNode*& proxyManip );
	bool					isManipActive(const MFn::Type& manipType,
											MObject &manipObject);
	MStatus                 finishAddingManips();
    static MStatus          addToManipConnectTable(MTypeId &);
    static MStatus          removeFromManipConnectTable(MTypeId &);

	virtual MManipData plugToManipConversion( unsigned int manipIndex );
	virtual MManipData manipToPlugConversion( unsigned int manipIndex );

	void addPlugToManipConversion( unsigned int manipIndex );
	unsigned int addManipToPlugConversion( MPlug &plug );

	void		addPlugToInViewEditor( const MPlug& plug );

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	void addPlugToManipConversionCallback(
			unsigned int manipIndex,
			plugToManipConversionCallback callback
		);

	//!	NO SCRIPT SUPPORT
	unsigned int addManipToPlugConversionCallback(
			MPlug &plug,
			manipToPlugConversionCallback callback
		);

END_NO_SCRIPT_SUPPORT:

	MStatus	getConverterManipValue(unsigned int manipIndex, unsigned int &value);
	MStatus	getConverterManipValue(unsigned int manipIndex, double &value);
	MStatus	getConverterManipValue(unsigned int manipIndex, double &x, double &y);
	MStatus	getConverterManipValue(unsigned int manipIndex, MPoint &point);
	MStatus	getConverterManipValue(unsigned int manipIndex, MVector &vector);
	MStatus	getConverterManipValue(unsigned int manipIndex, MMatrix &matrix);
	MStatus	getConverterManipValue(unsigned int manipIndex,
								   MEulerRotation &rotation);
	MStatus	getConverterManipValue(unsigned int manipIndex,
								   MTransformationMatrix &xform);

	MStatus	getConverterPlugValue(unsigned int plugIndex, double &value);
	MStatus	getConverterPlugValue(unsigned int plugIndex, double &x, double &y);
	MStatus	getConverterPlugValue(unsigned int plugIndex, MPoint &point);
	MStatus	getConverterPlugValue(unsigned int plugIndex, MVector &vector);
	MStatus	getConverterPlugValue(unsigned int plugIndex, MMatrix &matrix);
	MStatus	getConverterPlugValue(unsigned int plugIndex,
								  MEulerRotation &rotation);

	virtual MStatus	doPress();
	virtual MStatus	doDrag();
	virtual MStatus	doRelease();

    static const char *     className();

protected:
// No protected members

private:
	friend class			::MFnPlugin;
	void *					internalData;
};

OPENMAYA_NAMESPACE_CLOSE

//	These typedefs provide backward compatability for plugins which were
//	written back when the callback types were declared outside of the
//	the class's namespace.
typedef MPxManipContainer::manipToPlugConversionCallback manipToPlugConversionCallback;
typedef MPxManipContainer::plugToManipConversionCallback plugToManipConversionCallback;

#endif /* __cplusplus */
#endif /* _MPxManipContainer */
