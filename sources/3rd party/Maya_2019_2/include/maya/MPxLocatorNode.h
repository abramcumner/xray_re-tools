#ifndef _MPxLocatorNode
#define _MPxLocatorNode
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
// CLASS:    MPxLocatorNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/M3dView.h>
#include <maya/MBoundingBox.h>
#include <maya/M3dView.h>
#include <maya/MSelectionMask.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxNode)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for user defined locators 
/*!

  MPxLocatorNode allows the creation of user-defined locators.  A
  locator is a DAG shaped that is drawn on the screen, but that has is
  not rendered.  Locators are full dependency nodes and can have
  attributes and a compute method.

  To create a locator, derive off of this class and override the draw method.
  The draw method can be overridden to draw custom geometry using standard
  OpenGL calls.  The other methods of the parent class MPxNode may also
  be overridden to perform dependency node capabilities as well.
*/
class OPENMAYAUI_EXPORT MPxLocatorNode : public MPxNode
{
public:
	MPxLocatorNode();
	virtual ~MPxLocatorNode();
	virtual MPxNode::Type type() const;

	// Methods to overload

	virtual void draw( M3dView & view, const MDagPath & path,
			    	   M3dView::DisplayStyle style, M3dView:: DisplayStatus );
	virtual bool isBounded() const;
	virtual MBoundingBox boundingBox() const;


	// Color methods

	unsigned int         color( M3dView::DisplayStatus displayStatus );

	MColor               colorRGB( M3dView::DisplayStatus displayStatus );

	virtual bool		 excludeAsLocator() const;

	virtual bool		 isTransparent() const;

	virtual bool		 drawLast() const;

	virtual bool useClosestPointForSelection() const;

	virtual MPoint closestPoint(MPoint cursorRayPoint, MVector cursorRayDir) const;

	virtual MSelectionMask getShapeSelectionMask() const;

	// Inherited attributes

	//! under world attribute
	static MObject underWorldObject;
	//! local position attribute
	static MObject localPosition;
		//! X component of localPosition
		static MObject localPositionX;
		//! Y component of localPosition
		static MObject localPositionY;
		//! Z component of localPosition
		static MObject localPositionZ;
	//! world position attribute
	static MObject worldPosition;
	    //! X component of worldPosition
	    static MObject worldPositionX;
	    //! Y component of worldPosition
	    static MObject worldPositionY;
	    //! Z component of worldPosition
	    static MObject worldPositionZ;
	//! local scale attribute
	static MObject localScale;
		//! X component of localScale
		static MObject localScaleX;
		//! Y component of localScale
		static MObject localScaleY;
		//! Z component of localScale
		static MObject localScaleZ;
	//! bounding box attribute
	static MObject nodeBoundingBox;
	    //! bounding box minimum point
	    static MObject nodeBoundingBoxMin;
	        //! X component of boundingBoxMin
	        static MObject nodeBoundingBoxMinX;
	        //! Y component of boundingBoxMin
	        static MObject nodeBoundingBoxMinY;
	        //! Z component of boundingBoxMin
	        static MObject nodeBoundingBoxMinZ;
	    //!  bounding box maximum point
	    static MObject nodeBoundingBoxMax;
	        //! X component of boundingBoxMax
	        static MObject nodeBoundingBoxMaxX;
	        //! Y component of boundingBoxMax
	        static MObject nodeBoundingBoxMaxY;
	        //! Z component of boundingBoxMax
	        static MObject nodeBoundingBoxMaxZ;
	    //!  bounding box size vector
	    static MObject nodeBoundingBoxSize;
	        //! X component of boundingBoxSize
	        static MObject nodeBoundingBoxSizeX;
	        //! Y component of boundingBoxSize
	        static MObject nodeBoundingBoxSizeY;
	        //! Z component of boundingBoxSize
	        static MObject nodeBoundingBoxSizeZ;
	//! object center attribute
	static MObject center;
	    //! X component of boundingBoxCenter
	    static MObject boundingBoxCenterX;
	    //! Y component of boundingBoxCenter
	    static MObject boundingBoxCenterY;
	    //! Z component of boundingBoxCenter
	    static MObject boundingBoxCenterZ;
	//! matrix attribute
	static MObject matrix;
	//! inverse matrix attribute
	static MObject inverseMatrix;
	//! world matrix attribute
	static MObject worldMatrix;
	//! inverse world matrix attribute
	static MObject worldInverseMatrix;
	//! parent matrix attribute
	static MObject parentMatrix;
	//! inverse parent matrix attribute
	static MObject parentInverseMatrix;
	//! visibility attribute
	static MObject visibility;
	//! intermediate object attribute
	static MObject intermediateObject;
	//! template attribute
	static MObject isTemplated;
	//! instances object group info attribute
	static MObject instObjGroups;
	    //! object groups attributes
	    static MObject objectGroups;
	        //! component in object groups attribute
	        static MObject objectGrpCompList;
	        //! group id attribute
	        static MObject objectGroupId;
	        //! group id attribute
	        static MObject objectGroupColor;
	//! controls choice of wireframe dormant object color
	static MObject useObjectColor;
	//! the per object dormant wireframe color
	static MObject objectColor;

	static const char*	    className();

protected:
	// No protected members

private:
	static void				initialSetup();

};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxLocatorNode */
