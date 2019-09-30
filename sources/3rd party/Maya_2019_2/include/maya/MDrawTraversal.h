#ifndef _MDrawTraversal
#define _MDrawTraversal
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
// CLASS:    MDrawTraversal
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDrawTraversal)

//! \ingroup OpenMayaUI
//! \brief \obsolete
/*!
	\deprecated
	Use MHWRender::MSceneRender and MHWRender::MRenderOverride instead.

	MDrawTraversal is a utility class for interactive drawing. The
	purpose of the class is to traverse through the current scene dag
	and provide a means of accessing a list of visible objects with
	respect to a given frustum specification, and application
	"visibility" criteria.

	The additional "visibility" criteria include:


	\li Visibility overrides set per object .
	\li Visibility overrides set per application (global display settings) .
	\li Visibility overrides set per display layer (global display settings).


	Note that as the traversal is not dependent on a 3d modeling
	viewport.  There are thus no checks for per viewport visibility
	properties such as per viewport dag object type overrides, and
	"isolate select".

	Additionally, render layer visibility checks are not performed, as
	these pertain to software rendering.

	After traversal the list will contain leaf level nodes (shapes),
	which have passed the "visibility" criteria.


	For the actual frustum culling, there is a choice of two culling
	algorithms:


	\li Perform culling tests only when leaf level dag objects (shapes) are
			encountered.
	\li Perform hierarchical culling (cull sub-trees) as they are encountered.


	Note that hierarchical culling may force evaluation of geometry on
	dag objects which are not visible.

	The default is thus to perform leaf level culling only.

	Frustum culling is performed against the bounding boxes of dag
	objects encountered.

	API writers can derive from this class and change the filterNode()
	virtual method to perform custom filtering of objects during
	traversal. That is they will be excluded from the output list. By
	default this class does no additional filtering.
*/
class OPENMAYAUI_EXPORT MDrawTraversal
{
public:

	//! Item status enum
	enum {
		//! Check if item is active
		kActiveItem,
		//! Check if item is templated
		kTemplateItem
	};

    MDrawTraversal();
    virtual ~MDrawTraversal();
    virtual MStatus		traverse();
	virtual bool		filterNode( const MDagPath &traversalItem );

	MStatus				setFrustum( const MDagPath & cameraPath, unsigned int portWidth,
									unsigned int portHeight );
    MStatus				setOrthoFrustum(double left,      double right,
                             			double bottom,    double top,
                             			double nearpt,      double farpt,
                             			const MMatrix & worldXform);
    MStatus				setPerspFrustum(double fovX,      double aspectXY,
                            			double nearDist,  double farDist,
                             			const MMatrix & worldXform);
	MStatus				setFrustum(double left,      double right,
									double bottom,    double top,
									double nearpt,      double farpt,
                             	const MMatrix & worldXform);
    MStatus				setFrustum (const MPoint& nearBottomLeft,
									const MPoint& nearBottomRight,
									const MPoint& nearTopLeft,
									const MPoint& nearTopRight,
									const MPoint& farBottomLeft,
									const MPoint& farBottomRight,
									const MPoint& farTopLeft,
									const MPoint& farTopRight,
									const MMatrix &worldXform);
	bool				frustumValid() const;

	void				setLeafLevelCulling( bool cullAtLeafLevel );
	bool				leafLevelCulling() const;

	void				enableFiltering( bool val );
	bool				filteringEnabled() const;

	//
	// Traversed list methods
	//
	unsigned int		numberOfItems() const;
	MStatus				itemPath(unsigned int itemNumber, MDagPath &path) const;
	bool				itemHasStatus(unsigned int itemNumber, unsigned int test) const;


protected:
	//
	bool				fLeafLevelCulling;
	bool				fFrustumValid;
	bool				fFilteringEnabled;
private:
	void	*fPtr;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* MDrawTraversal */
