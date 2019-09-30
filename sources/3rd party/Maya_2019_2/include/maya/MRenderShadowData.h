#ifndef _MRenderShadowData
#define _MRenderShadowData
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
// CLASS:    MRenderShadowData
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MFloatPoint.h>
#include <maya/MFloatMatrix.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMayaRender
//! \brief Access Rendering Shadow Map Data
/*!

  The shadow map can be changed by instanciating a MRenderCallback and
  overriding shadowCastCallback(). When this is invoked, a
  MRenderShadowData is passed as an argument; the depthMaps and
  midDistMaps members can then be changed by this API. Methods and
  data are provided to assist in transforming back and forth from
  world space to z buffer space. Paint Effects and Fur are two
  examples which use this mechanism to change the shadow map.

  To prevent self shadowing, Maya uses a mid distance map to resolve
  the ambiguity. Details of this technique can be obtained from
  Graphics Gems III, "The Shadow Depth Map Revisited".
*/
class OPENMAYARENDER_EXPORT MRenderShadowData {
public:
						MRenderShadowData();

    void                worldToZbuffer(
                            const MFloatPoint&  worldPoint,
                            MFloatPoint&        screenPoint) const;

    void                zbufferToWorld(
                            const MFloatPoint& screenPoint,
                            MFloatPoint&        worldPoint) const;

    // in data
	//! set if the light has a perspective projection
    bool                perspective;
	//! set if the light generates a depth and midDist map
    bool                useMidDistMap;

    //! Light Types
    enum LightType
    {
        kInvalid,	//!< \nop
        kPoint,		//!< \nop
        kDirectional,	//!< \nop
        kSpot		//!< \nop
    };

	//! the type of light the shadow map is generated for
    LightType           lightType;

	//! the x resolution of shadow map
    unsigned short      shadowResX;
	//! the y resolution of the shadow map
    unsigned short      shadowResY;
	//! position of the light in world space
    MFloatPoint         lightPosition;
	//! projection matrix for the light
    MFloatMatrix        projectionMatrix;
	//! perspective matrix for the light
    MFloatMatrix        perspectiveMatrix;

    // out data
	//! the output depth map
    float               *depthMaps;
	//! the output mid distance map
    float               *midDistMaps;

    // private
    const void*         internalData;

protected:
// No protected members

private:
// No private members
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderShadowData */

