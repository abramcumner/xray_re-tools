/**********************************************************************
 *<
	FILE: CameraMapTexture.h

	DESCRIPTION:	Includes for Plugins

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#ifndef __CAMERAMAPTEXTURE__H
#define __CAMERAMAPTEXTURE__H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include "stdmat.h"
#include "imtl.h"
#include "macrorec.h"

#include <RenderElements.h>

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;


//
// Shade Context: passed into Mtls and Texmaps
// This shade context is sued to trick the sub textures into using a new set of UVWs
// I basically overrided the UVW methods and pass the per pixel camera UVWs through them
// and all the other methods just fall back to the original shade context
class MyShadeContext : public ShadeContext  {
	public:

		
	int mapChannelID;  // the map channel that the per pixel camera mapping is using
	Point3 myUVW;      // the replacement UVW for this sample based on the camera projection

	ShadeContext *sc;

	// Reset output & render elements: 
	// param < 0 clears existing elements, leaving nElements unchanged
	// n >= 0 clears n elements & set nElements to n, default is 0
	void ResetOutput( int n = -1 ) { sc->out.Reset(n); }	
	
	 Class_ID ClassID() { return Class_ID(0,0); }  // to distinguish different ShadeContexts.
	 BOOL InMtlEditor() {return sc->InMtlEditor();}	// is this rendering the mtl editor sample sphere?
	 int Antialias() {return sc->Antialias();}
	 int ProjType() { return sc->ProjType();} // returns: 0: perspective, 1: parallel
	 LightDesc* Light(int n) {return sc->Light(n);}	// get the nth light. 
	 TimeValue CurTime(){ return sc->CurTime();}     	// current time value
	 int NodeID() { return sc->NodeID(); }
	 INode *Node() { return sc->Node(); }
	 Object *GetEvalObject() { return sc->GetEvalObject(); } // Returns the evaluated object for this node. 
												   // Will be NULL if object is motion blurred	
	 Point3 BarycentricCoords() { return sc->BarycentricCoords();}  // coords relative to triangular face 
	 int FaceNumber() {return sc->FaceNumber();}			// 
	 Point3 Normal(){return sc->Normal();}			// interpolated surface normal, in cameara coords: affected by SetNormal()
	 void SetNormal(Point3 p) {sc->SetNormal(p);}	// used for perturbing normal
	 Point3 OrigNormal() { return sc->Normal(); } // original surface normal: not affected by SetNormal();
	 Point3 GNormal() {return sc->GNormal();} 		// geometric (face) normal
	 float  Curve() { return sc->Curve(); }   	    // estimate of dN/dsx, dN/dsy
	 Point3 V() {return sc->V();}       		// Unit view vector: from camera towards P 
	 void SetView(Point3 p) { sc->SetView(p);}	// Set the view vector
	 Point3 OrigView() { return sc->V(); } // Original view vector: not affected by SetView();
	 Point3 ReflectVector() {return sc->ReflectVector();}	// reflection vector
	 Point3 RefractVector(float ior){ return sc->RefractVector(ior);}	// refraction vector
	 void SetIOR(float ior) {sc->SetIOR(ior);} // Set index of refraction
	 float GetIOR() { return sc->GetIOR(); } // Get index of refraction
	 Point3 CamPos() {return sc->CamPos(); }			// camera position
	 Point3 P() {return sc->P();}				// point to be shaded;
	 Point3 DP() {return sc->DP();}    		  	// deriv of P, relative to pixel, for AA
	 void DP(Point3& dpdx, Point3& dpdy){sc->DP(dpdx,dpdy); }  // deriv of P, relative to pixel
	 Point3 PObj() {return sc->PObj();}   		  	// point in obj coords
	 Point3 DPObj() {return sc->DPObj();}   	   	// deriv of PObj, rel to pixel, for AA
	 Box3 ObjectBox() {return sc->ObjectBox();}; 	  	// Object extents box in obj coords
	 Point3 PObjRelBox() {return sc->PObjRelBox();}	  	// Point rel to obj box [-1 .. +1 ] 
	 Point3 DPObjRelBox() {return sc->DPObjRelBox();}	  	// deriv of Point rel to obj box [-1 .. +1 ]  
	 void ScreenUV(Point2& uv, Point2 &duv) {sc->ScreenUV(uv,duv);} // screen relative uv (from lower left)
	 IPoint2 ScreenCoord() {return sc->ScreenCoord();} // integer screen coordinate (from upper left)
	 Point2 SurfacePtScreen(){ return sc->SurfacePtScreen(); } // return the surface point in screen coords

	 Point3 UVW(int channel=0) {
									if (channel == mapChannelID)
										return myUVW;
									else return sc->UVW(channel);
								}  			// return UVW coords for point

//need to figure these out
	 Point3 DUVW(int channel=0) 
					{
					if (channel == mapChannelID)
						return Point3(0.001f,0.001f,0.001f);
					return sc->DUVW(channel);
					} 			// return UVW derivs for point
	 void DPdUVW(Point3 dP[3],int channel=0) { sc->DPdUVW(dP,channel);} // Bump vectors for UVW (camera space)

	// BumpBasisVectors: this is going to replace DPdUVW: Now compute bump vectors for 
	//  specific 2D cases only.
	//    axis = AXIS_UV, AXIS_VW, AXIS_WU		
	//    return:     0 => not implemented,  1=> implemented
	// If this function is implemented, it will be used instead of DPdUVW.
	 int BumpBasisVectors(Point3 dP[2], int axis, int channel=0) { return sc->BumpBasisVectors(dP, axis, channel); } 


	 BOOL IsSuperSampleOn(){ return sc->IsSuperSampleOn(); }
	 BOOL IsTextureSuperSampleOn(){ return sc->IsTextureSuperSampleOn(); }
	 int GetNSuperSample(){ return sc->GetNSuperSample(); }
	 float GetSampleSizeScale(){ return sc->GetSampleSizeScale(); }

	// UVWNormal: returns a vector in UVW space normal to the face in UVW space. This can 
	// be CrossProd(U[1]-U[0],U[2]-U[1]), where U[i] is the texture coordinate
	// at the ith vertex of the current face.  Used for hiding textures on
	// back side of objects.
	 Point3 UVWNormal(int channel=0) { return sc->UVWNormal(channel); }  

	// diameter of view ray at this point
	 float RayDiam() { return sc->RayDiam(); } 

	// angle of ray cone hitting this point: gets increased/decreased by curvature 
	// on reflection
	 float RayConeAngle() { return sc->RayConeAngle(); } 

	AColor EvalEnvironMap(Texmap *map, Point3 view) {return sc->EvalEnvironMap(map, view);} //evaluate map with given viewDir
	void GetBGColor(Color &bgcol, Color& transp, BOOL fogBG=TRUE) {sc->GetBGColor(bgcol, transp, fogBG);}    // returns Background color, bg transparency

	// Camera ranges set by user in camera's UI.
	 float CamNearRange() {return sc->CamNearRange();}
	 float CamFarRange() {return sc->CamFarRange();}

	// Transform to and from internal space
	  Point3 PointTo(const Point3& p, RefFrame ito) {return sc->PointTo(p, ito);} 
	  Point3 PointFrom(const Point3& p, RefFrame ifrom) {return sc->PointFrom(p, ifrom);} 
	  Point3 VectorTo(const Point3& p, RefFrame ito) {return sc->VectorTo(p, ito); }; 
	  Point3 VectorFrom(const Point3& p, RefFrame ifrom){return  sc->VectorFrom(p, ifrom);};  
	  Point3 VectorToNoScale(const Point3& p, RefFrame ito){return sc->VectorToNoScale( p, ito);};  
	  Point3 VectorFromNoScale(const Point3& p, RefFrame ifrom){return sc->VectorFromNoScale(p, ifrom); };  

	// After being evaluated, if a map or material has a non-zero GBufID, it should
	// call this routine to store it into the shade context.
	 void SetGBufferID(int gbid) { sc->SetGBufferID(gbid); }

	 FILE* DebugFile() { return sc->DebugFile(); }

	 AColor EvalGlobalEnvironMap(Point3 dir) { return sc->EvalGlobalEnvironMap(dir); }

	 BOOL GetCache(Texmap *map, AColor &c){ return sc->GetCache(map, c); }
	 BOOL GetCache(Texmap *map, float &f) { return sc->GetCache(map, f); }
	 BOOL GetCache(Texmap *map, Point3 &p){ return sc->GetCache(map, p); }
	 void PutCache(Texmap *map, const AColor &c) {sc->PutCache(map, c); }
	 void PutCache(Texmap *map, const float f) {sc->PutCache(map, f);}
	 void PutCache(Texmap *map, const Point3 &p){sc->PutCache(map, p);}
	 void TossCache(Texmap *map) {sc->TossCache(map);}
	// Generic expansion function
	 INT_PTR Execute(int cmd, ULONG_PTR arg1=0, ULONG_PTR arg2=0, ULONG_PTR arg3=0) { return sc->Execute(cmd, arg1,arg2, arg3); } 

	// These are used to prevent self shadowing by volumetric lights
	LightDesc *GetAtmosSkipLight() { return sc->GetAtmosSkipLight(); }
	void SetAtmosSkipLight(LightDesc *lt) { sc->SetAtmosSkipLight(lt); }

	// render element methods
	int NRenderElements() { return sc->NRenderElements();	}
	IRenderElement *GetRenderElement(int n) { return sc->GetRenderElement(n); }

	// diffuse illum utility, computes incoming diffuse illumination, for matte/shadow
	Color DiffuseIllum() {return sc->DiffuseIllum();}

	// used to preshade in different ways
//	virtual IRenderMode GetIRenderMode(){ return IRM_NORMAL; }
//	IRenderMode GetIRenderMode(){ return iRenderMode; }
//	void SetIRenderMode( IRenderMode mode ){ iRenderMode = mode; }

	// NOTE: All of these methods are inline. The inline definitions not
	// declared here are declared in toneop.h

	// Does this tone operator really map physical values to RGB. This method
	// is provided so shaders can determine whether the shading calculations
	// are in physical or RGB space.
	bool IsPhysicalSpace()  {return sc->IsPhysicalSpace();}

	MyShadeContext() {
		mode = SCMODE_NORMAL; nLights = 0; shadow = TRUE;  rayLevel = 0; 
		globContext = NULL; atmosSkipLight = NULL; 
	}


};


#endif // __CAMERAMAPTEXTURE__H
