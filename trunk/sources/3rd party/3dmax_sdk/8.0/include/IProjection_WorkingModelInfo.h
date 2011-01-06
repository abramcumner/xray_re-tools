/*==============================================================================

  file:     IProjection_WorkingModelInfo.h

  author:   Daniel Levesque

  created:  19 May 2004

  description:

      Definition of an interface that gives access to basic shade context information
	  on the working model of a bake projection.

	  This interface is to be used along with the bake projection feature, which
	  allows using high-res objects when baking maps for low-res objects.

  modified:	


© 2004 Autodesk
==============================================================================*/

#ifndef _IPROJECTION_WORKINGMODELINFO_H_
#define _IPROJECTION_WORKINGMODELINFO_H_

#include <BaseInterface.h>

class RenderInstance;
class Point3;

//==============================================================================
// class IProjection_WorkingModelInfo
//
// This interface provides access to some shade context information for the
// working model.
//
// When a bake projection is done, all the information in the shade context
// is overriden to represent the intersection point on the projected object.
// If a bake render element still needs access to shade context information
// from the original object (the working model), then this interface may be used.
//
// This interface should be queried from the shade context through GetInterface().
// A NULL pointer will be returned if no projection was done.
//==============================================================================
class IProjection_WorkingModelInfo : public BaseInterface {

public:

	// Query this interface on a shade context
	static IProjection_WorkingModelInfo* GetInterface(ShadeContext& shadeContext);

	// Returns the render instance of the working model
	virtual RenderInstance* GetRenderInstance() = 0;
	// Returns the face number of that hit face
	virtual int GetFaceNumber() = 0;
	// Returns the original interpolated normal
	virtual const Point3& GetOrigNormal() = 0;
	// Returns the barycentric coordinates of the hit point
    virtual void GetBarycentricCoords(Point3& bary) = 0;    
	virtual void GetBarycentricCoords(float& b0, float& b1, float& b2) = 0;
	// Returns whether the backface was hit
	virtual BOOL GetBackFace() = 0;
	// Returns the point that was hit, in camera space
	virtual const Point3& GetPoint() = 0;
	// Get the view direction, in camera space (equivalent to SContext::V())
	virtual const Point3& GetViewDir() = 0;
	// Get the original view direction, in camera space (equivalent to SContext::OrigView())
	virtual const Point3& GetOriginalViewDir() = 0;
	// Get the material number of the hit face
	virtual int GetMaterialNumber() = 0;
	// Get the bump basis vectors of the hit face (equivalent to SContext::BumpBasisVectors())
	virtual void BumpBasisVectors(Point3 dP[2], int axis, int channel) = 0;
	// Get the map basis vectors (aks bump basis vectors) interpolated at the current position
	virtual void TangentBasisVectors(Point3 dP[2], int mapChannel) = 0;

	// -- from BaseInterface
	virtual Interface_ID GetID();
};

#define IPROJECTION_WORKINGMODELINFO_INTERFACE_ID Interface_ID(0x79907259, 0xbf05948)

inline IProjection_WorkingModelInfo* IProjection_WorkingModelInfo::GetInterface(ShadeContext& shadeContext) {

	return static_cast<IProjection_WorkingModelInfo*>(shadeContext.GetInterface(IPROJECTION_WORKINGMODELINFO_INTERFACE_ID));
}

inline Interface_ID IProjection_WorkingModelInfo::GetID() {

	return IPROJECTION_WORKINGMODELINFO_INTERFACE_ID;
}

#endif
