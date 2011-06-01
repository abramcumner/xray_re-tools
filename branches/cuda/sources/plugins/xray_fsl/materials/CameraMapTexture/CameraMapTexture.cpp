/**********************************************************************
 *<
	FILE: CameraMapTexture.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: Peter Watje

	HISTORY: April 20,2004 created

This is a camera map per pixel tetxure.   It takes an incoming channel and replaces it
with a camera projection.

 
2) Allow different aspect ratio for projection versus render..   
 
3) animation toggle.  support bitmap sequence as a projection.
  
5) semi accurate viewport display
 
6) some sort of baking utility.  


 *>	Copyright (c) 2000, All Rights Reserved.
 **********************************************************************/

#include "CameraMapTexture.h"

#include <IMtlRender_Compatibility.h>
#include <mentalray\imrShader.h>
#include <mentalray\imrShaderTranslation.h>
#include <mentalray\imrAdvancedTranslation.h>
#include <mentalray\mentalrayInMax.h>

#define CAMERAMAPTEXTURE_CLASS_ID	Class_ID(0x60ec7a41, 0xa20dc7e)

#define SCANLINERENDERER_CLASS_ID				Class_ID(SREND_CLASS_ID,0)

#define NSUBTEX		2 // TODO: number of sub-textures supported by this plugin 
					  // 0 is the texture
					  // 1 is the mask


#define PBLOCK_REF	0


enum { cameramaptexture_params };


//TODO: Add enums for various parameters
enum { 
	pb_mapid,		//this is the map channel id
	pb_node,		//this is the node that is the camera, if the node is not a camera the z axis is used for the projection direction with a 45deg fov
	pb_texture,		//this is the texture that will use the camera projection
	pb_backface,	//this is toggle that turns on whether back facing vertices have the projection

	pb_usezbuffer, pb_zbuffer,pb_zfudge,   // this toggles on the ZBuffer mask
										  //  for best results render this without antialiasing
	pb_angle,		//the angle used to determine if a pixel is back facing
	pb_usemask,pb_mask, //this is just a standard mask used to mask the texture
	pb_animated,		//no longer used, but needs to stay here for older file formats
	pb_maskuseprojection //this forces the mask to also use the camera projection otherwise it will use it original UVWs

};

namespace {

	bool GetParamIDByName(ParamID& paramID, const TCHAR* name, IParamBlock2* pBlock) {

		DbgAssert(pBlock != NULL);
		int count = pBlock->NumParams();
		for(int i = 0; i < count; ++i) {
			ParamID id = pBlock->IndextoID(i);
			const ParamDef& paramDef = pBlock->GetParamDef(id);
			if(_tcsicmp(name, paramDef.int_name) == 0) {
				paramID = id;
				return true;
			}
		}

		DbgAssert(false);
		return false;
	}

	template<typename T>
	void TranslateMRShaderParameter(IParamBlock2* paramBlock, TCHAR* paramName, T value) {

		ParamID paramID;
		if(GetParamIDByName(paramID, paramName, paramBlock)) {
			paramBlock->SetValue(paramID, 0, value);
		}
		else {
			DbgAssert(false);
		}
	}
}

class CameraMapTexture : public Texmap, private imrShaderTranslation {
	public:


		// Parameter block
		IParamBlock2	*pblock;	//ref 0

		Interval		ivalid;

		//From MtlBase
		ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
		BOOL SetDlgThing(ParamDlg* dlg);
		void Update(TimeValue t, Interval& valid);
		void Reset();
		Interval Validity(TimeValue t);
		ULONG LocalRequirements(int subMtlNum)
		{
			if ((subMtlNum==0) && (tex)) return tex->LocalRequirements(subMtlNum);
			else if ((subMtlNum==1) && (mask)) return mask->LocalRequirements(subMtlNum);
			return 0;
		}

		//TODO: Return the number of sub-textures
		int NumSubTexmaps() { return NSUBTEX; }
		//TODO: Return the pointer to the 'i-th' sub-texmap
		Texmap* GetSubTexmap(int i) 
		{ 
			Texmap *sm1 = NULL;
			Interval iv;
			if (i==0)
			{
				pblock->GetValue(pb_texture,0,sm1,iv);
				return sm1; 
			}
			else
			{
				pblock->GetValue(pb_mask,0,sm1,iv);
				return sm1; 

			}
		}

		void SetSubTexmap(int i, Texmap *m)
		{
			if (i==0)
				pblock->SetValue(pb_texture,0,m);
			else if (i==1)
				pblock->SetValue(pb_mask,0,m);
		}
		
		TSTR GetSubTexmapSlotName(int i)
		
		{
			Texmap *sm1 = NULL;
			Interval iv;
			if (i==0)
				pblock->GetValue(pb_texture,0,sm1,iv);
			else if (i==1)
				pblock->GetValue(pb_mask,0,sm1,iv);
			TSTR name;
			if (sm1 != NULL)
				name.printf("%s",GetString(IDS_TEXTURE));
			else name.printf("empty");
				return name; 

		}

		//From Texmap
		RGBA EvalColor(ShadeContext& sc);
		float EvalMono(ShadeContext& sc);
		Point3 EvalNormalPerturb(ShadeContext& sc);

		//TODO: Returns TRUE if this texture can be used in the interactive renderer
		BOOL SupportTexDisplay() { return FALSE; }
		void ActivateTexDisplay(BOOL onoff);
		DWORD_PTR GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker);
		
		//TODO: Return anim index to reference index
		int SubNumToRefNum(int subNum) { return subNum; }
		
		
		// Loading/Saving
		IOResult Load(ILoad *iload);
		IOResult Save(ISave *isave);

		//From Animatable
		Class_ID ClassID() {return CAMERAMAPTEXTURE_CLASS_ID;}		
		SClass_ID SuperClassID() { return TEXMAP_CLASS_ID; }
#ifdef LOCALIZED_APL		// naming conflicts for APL languages (only?) -- LOCALIZATION_TODO
		void GetClassName(TSTR& s) {s = _T("Camera Map Per Pixel ");}
#else
		void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}
#endif	// LOCALIZED_APL

		RefTargetHandle Clone( RemapDir &remap );
		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
			PartID& partID,  RefMessage message);


		int NumSubs() { return 1; }
		Animatable* SubAnim(int i); 
		TSTR SubAnimName(int i);

		// TODO: Maintain the number or references here 
		int NumRefs() { return 1; }
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);



		int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

		void DeleteThis() { delete this; }		
		//Constructor/Destructor

		CameraMapTexture();
		~CameraMapTexture();	

		// this takes an existing shade context and creates a new one
		// overding one map channel with the camera projection
		MyShadeContext BuildMyShadeContext(ShadeContext& sc);
		// this returns if a pixel should have mapping applied to it
		BOOL IsPixelHidden(ShadeContext& sc);
		
		// -- from InterfaceServer
		virtual BaseInterface* GetInterface(Interface_ID id);

		//Variables to hold the param block data on update
		INode *cameraNode;			// the camera node
		int mapChannel;				// the map channel to replace
		Texmap *tex;				// the base texture
		Texmap *mask;				// mask to use against the texture
		ObjectState camOState;

		float fov;					// the FOV of the camera
		Matrix3 invCamTM;			// the matrix from world space to camera space

		BOOL backFace;				// whether to remove the back facing samples

		Point3 cameraPos;			// the position of the camera in world space

		BOOL useZBuffer;			// whether to use a zbuffer to mask off samples
		PBBitmap *zBufferPB;		// the zbuffer bitmap param block

		int width, height;			//the width and height of the zbuffer
		float *zbuffer;				//the raw zbuffer data

		float backFudge;			//this is fudge values to let the user relax the definition of what
									//behind/infront of the zbuffer.  Used to fix the problem that the 
									//zbuffer does not have fragment data.

		float angleThreshold;		//this is the angle threshold for determining back facing samples
									//this is in degress and NOT radians 

		BOOL useMask;				// this is just a toggle to determine whether to use the mask or not
		BOOL maskUsesProjection;    // this toggle will force the mask to use the projection also

	private:

		// -- from imrShaderTranslation
		virtual imrShader* GetMRShader(imrShaderCreation& shaderCreation);
		virtual void ReleaseMRShader();
		virtual bool NeedsAutomaticParamBlock2Translation();
		virtual void TranslateParameters(imrTranslation& translationInterface, imrShader* shader, TimeValue t, Interval& valid);
		virtual void GetAdditionalReferenceDependencies(AdditionalDependencyTable& refTargets);
		virtual void TranslateParameters_Advanced(imrAdvancedTranslation& advancedTranslation, TimeValue t, Interval& valid);
};


class CameraMapTextureClassDesc:public ClassDesc2, public IMtlRender_Compatibility_MtlBase {
	public:

	CameraMapTextureClassDesc();

	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new CameraMapTexture(); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return TEXMAP_CLASS_ID; }
	Class_ID		ClassID() { return CAMERAMAPTEXTURE_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("CameraMapTexture"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }				// returns owning module handle

	// -- from IMtlRender_Compability_MtlBase
	virtual bool IsCompatibleWithRenderer(ClassDesc& rendererClassDesc);
};

CameraMapTextureClassDesc::CameraMapTextureClassDesc() {

	IMtlRender_Compatibility_MtlBase::Init(*this);
}

bool CameraMapTextureClassDesc::IsCompatibleWithRenderer(ClassDesc& rendererClassDesc) {

	Class_ID classID = rendererClassDesc.ClassID();

	if((classID == MRRENDERER_CLASSID) || (classID == SCANLINERENDERER_CLASS_ID)) {
		return true;
	}
	else {
		// Return 'true' only if the renderer doesn't implement the compatibility interface.
		// This ensures that we are compatible with all renderers unless they specify the contrary.
		IMtlRender_Compatibility_Renderer* rendererCompatibility = Get_IMtlRender_Compatibility_Renderer(rendererClassDesc);
		return (rendererCompatibility == NULL);
	}
}


static CameraMapTextureClassDesc CameraMapTextureDesc;
ClassDesc2* GetCameraMapTextureDesc() { return &CameraMapTextureDesc; }



static ParamBlockDesc2 cameramaptexture_param_blk ( cameramaptexture_params, _T("params"),  0, &CameraMapTextureDesc, 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, NULL,

	pb_node, 	_T("camera"),		TYPE_INODE, 		0,				IDS_PW_NODE,
		p_ui, 			TYPE_PICKNODEBUTTON, 	IDC_PICK_OBJECT, 
		end, 

	// params
	pb_mapid, 			_T("MapChannel"), 		TYPE_INT, 	P_ANIMATABLE, 	IDS_SPIN, 
		p_default, 		1, 
		p_range, 		0,99, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_INT, IDC_EDIT,	IDC_SPIN, 0.5f, 
		end,

	pb_texture, 	_T("texture"),		TYPE_TEXMAP, 		0,				IDS_TEXTURE,
		p_ui, 			TYPE_TEXMAPBUTTON, 	IDC_PICK_TEXTURE, 
		p_subtexno, 0,
		end, 

	pb_backface,	_T("backFace"), TYPE_BOOL, 0, IDS_BACKFACE,
		p_default,		TRUE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_BACKFACECHECK,
		end,

	pb_usezbuffer,	_T("useZBuffer"), TYPE_BOOL, 0, IDS_USEZBUFFER,
		p_default,		FALSE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_ZBUFFERCHECK,
		end,

	pb_zbuffer, _T("zbuffer"),	TYPE_BITMAP, 0, IDS_ZBUFFER,
		p_ui,			TYPE_BITMAPBUTTON, IDC_PICK_ZBUFFER,
		end,

	// params
	pb_zfudge, 			_T("ZFudge"), 		TYPE_FLOAT, 	P_ANIMATABLE, 	IDS_ZFUDGE, 
		p_default, 		1.0f, 
		p_range, 		0.0f,1000.0f, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_ZFUDGE,	IDC_ZFUDGESPIN, 0.1f, 
		end,

	// params
	pb_angle, 			_T("angleThreshold"), 		TYPE_FLOAT, 	P_ANIMATABLE, 	IDS_ANGLE ,
		p_default, 		90.0f, 
		p_range, 		0.0f,180.0f, 
		p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_ANGLE,	IDC_ANGLESPIN, 0.5f, 
		end,

	pb_mask, 	_T("mask"),		TYPE_TEXMAP, 		0,				IDS_MASK,
		p_ui, 			TYPE_TEXMAPBUTTON, 	IDC_PICK_MASK, 
		p_subtexno, 1,
		end, 

	pb_usemask,	_T("useMask"), TYPE_BOOL, 0, IDS_USEMASK,
		p_default,		FALSE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_MASKCHECK,
		end,

	pb_maskuseprojection,	_T("maskUsesProjection"), TYPE_BOOL, 0, IDS_MASKUSESPROJECTION,
		p_default,		TRUE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_MASKUSEPROJECTIONCHECK,
		end,


	end
	);




//--- CameraMapTexture -------------------------------------------------------
CameraMapTexture::CameraMapTexture()
{
	//TODO: Add all the initializing stuff
	pblock = NULL;
	CameraMapTextureDesc.MakeAutoParamBlocks(this);
	Reset();
}

CameraMapTexture::~CameraMapTexture()
{

}

//From MtlBase
void CameraMapTexture::Reset() 
{

	ivalid.SetEmpty();

}

void CameraMapTexture::Update(TimeValue t, Interval& valid) 
{	
	//TODO: Add code to evaluate anything prior to rendering

//	Interval ivalid;
	ivalid = FOREVER;

	cameraPos = Point3(0.0f,0.0f,0.0f);
	fov = PI/4.0f;
	

	//get our data and store it off so the evals can use it
	//since we dont want to be calling getvalue in the evals
	pblock->GetValue(pb_node,t,cameraNode,ivalid);

	pblock->GetValue(pb_mapid,t,mapChannel,ivalid);

	pblock->GetValue(pb_texture,t,tex,ivalid);
	pblock->GetValue(pb_mask,t,mask,ivalid);
	pblock->GetValue(pb_backface,t,backFace,ivalid);

	pblock->GetValue(pb_usezbuffer,t,useZBuffer,ivalid);

	pblock->GetValue(pb_zbuffer,t,zBufferPB,ivalid);

	pblock->GetValue(pb_zfudge,t,backFudge,ivalid);

	pblock->GetValue(pb_angle,t,angleThreshold,ivalid);
	
	pblock->GetValue(pb_usemask,t,useMask,ivalid);

	pblock->GetValue(pb_maskuseprojection,t,maskUsesProjection,ivalid);



	//update our texture
	if (tex) 
		tex->Update(t,ivalid);

	//upate our mask
	if (mask)
		mask->Update(t,ivalid);
	

	 
	//if we have a camera node extract the camera data
	if (cameraNode)
	{

		camOState = cameraNode->EvalWorldState(t);

		Interval cameraInterval = FOREVER;
		invCamTM = cameraNode->GetObjTMAfterWSM(t, &cameraInterval); //watje Defect 661145  switched to the AfterWSM to include cameras modified by theing like skew etc
//		invCamTM = cameraNode->GetObjectTM(t,&cameraInterval);
		ivalid &= cameraInterval;
		
		cameraPos = invCamTM.GetTrans();

		invCamTM = Inverse(invCamTM);

		if (camOState.obj->SuperClassID() == CAMERA_CLASS_ID) 
			fov = ((CameraObject *)camOState.obj)->GetFOV(t, valid);
	}
		

	zbuffer = NULL;
	//get our zbuffer info needed
	if ((zBufferPB != NULL) && useZBuffer)
	{
		width = zBufferPB->bi.Width();
		height = zBufferPB->bi.Height();

		zBufferPB->Load();
		if (zBufferPB->bm != NULL)
		{
			DWORD type;
			zbuffer = (float *)zBufferPB->bm->GetChannel(BMM_CHAN_Z,type);
		}				
	}

	valid = ivalid;	

}

Interval CameraMapTexture::Validity(TimeValue t)
{
	//TODO: Update ivalid here
	return ivalid;
}

ParamDlg* CameraMapTexture::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) 
{
	IAutoMParamDlg* masterDlg = CameraMapTextureDesc.CreateParamDlgs(hwMtlEdit, imp, this);
	//TODO: Set the user dialog proc of the param block, and do other initialization	
	return masterDlg;	
}

BOOL CameraMapTexture::SetDlgThing(ParamDlg* dlg)
{	
	return FALSE;
}



//From ReferenceMaker
RefTargetHandle CameraMapTexture::GetReference(int i) 
{
	//TODO: Return the references based on the index	
	switch (i) {
		case 0: return pblock;
		}
	return NULL;
}

void CameraMapTexture::SetReference(int i, RefTargetHandle rtarg) 
{
	//TODO: Store the reference handle passed into its 'i-th' reference
	switch(i) {
		case 0:	pblock = (IParamBlock2 *)rtarg; break;
		
	}
}

//From ReferenceTarget 
RefTargetHandle CameraMapTexture::Clone(RemapDir &remap) 
{
	CameraMapTexture *mnew = new CameraMapTexture();
	*((MtlBase*)mnew) = *((MtlBase*)this); // copy superclass stuff
	//TODO: Add other cloning stuff
	BaseClone(this, mnew, remap);

	mnew->ReplaceReference(PBLOCK_REF,remap.CloneRef(pblock));

	return (RefTargetHandle)mnew;
}

	 
Animatable* CameraMapTexture::SubAnim(int i) 
{
	//TODO: Return 'i-th' sub-anim
	switch (i) {
		case 0: return pblock;
		}
	return NULL;
}

TSTR CameraMapTexture::SubAnimName(int i) 
{
	//TODO: Return the sub-anim names
	switch (i) {
		case 0: return GetString(IDS_PARAMS);
		}
	return GetString(IDS_PARAMS);
}

RefResult CameraMapTexture::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
   PartID& partID, RefMessage message ) 
{
	switch (message) {
		case REFMSG_CHANGE:
			{
			ivalid.SetEmpty();
			if (hTarget == pblock)
				{
			// see if this message came from a changing parameter in the pblock,
			// if so, limit rollout update to the changing item and update any active viewport texture
				ParamID changing_param = pblock->LastNotifyParamID();
				cameramaptexture_param_blk.InvalidateUI(changing_param);
				}
			break;
			}
		}
	return(REF_SUCCEED);
	}

IOResult CameraMapTexture::Save(ISave *isave) 
{
	//TODO: Add code to allow plugin to save its data
	return IO_OK;
}

IOResult CameraMapTexture::Load(ILoad *iload) 
{ 
	//TODO: Add code to allow plugin to load its data
	return IO_OK;
}

//this builds our new shade context from an existing one
//and puts in our camera projection into it
MyShadeContext CameraMapTexture::BuildMyShadeContext(ShadeContext& sc)
{
	MyShadeContext myContext;
	myContext.sc = &sc;
	myContext.mode = sc.mode;
	myContext.doMaps = sc.doMaps;
	myContext.filterMaps = sc.filterMaps;
	myContext.shadow = sc.shadow;
	myContext.backFace = sc.backFace;
	myContext.mtlNum = sc.mtlNum;

	myContext.ambientLight = sc.ambientLight;
	myContext.nLights = sc.nLights;
	myContext.rayLevel = sc.rayLevel;

	myContext.xshadeID = sc.xshadeID;
	myContext.atmosSkipLight = sc.atmosSkipLight;
	myContext.globContext = sc.globContext;
	myContext.out = sc.out;

	myContext.mapChannelID = mapChannel;
	// Get the Field of View parameter from the camera we reference
	// Compute the scale factors
	double xScale = -0.5 / (tan(0.5*(double)fov));

	Interface *ip = GetCOREInterface();
	float aspectRatio = ip->GetRendImageAspect();
	float yScale = xScale*aspectRatio;

	// Transform the points into screen space
	float zScale = 1.0f;
	float distance, x, y, z;

	Point3 p = sc.P();
	Point3 cameraSpace = p;

	Point3 worldP = sc.PointTo(sc.P(), REF_WORLD);
	p = worldP * invCamTM;
	x = p.x; 
	y = p.y; 
	z = p.z;

	distance = (float) sqrt(x*x + y*y + z*z);
	myContext.myUVW.x = p.x*xScale/z + 0.5f;
	myContext.myUVW.y = p.y*yScale/z + 0.5f;
	myContext.myUVW.z = distance;

	return myContext;
}

//this checks to see if a pixel should be rendered with the
//projection
BOOL CameraMapTexture::IsPixelHidden(ShadeContext& sc)
{

	BOOL hidden = FALSE;
	//first check the backfacing
	if (backFace)
	{
		//compute the angle between our normal and the vector from our node to camera origin
		Point3 nodePos = sc.PointFrom(cameraPos,REF_WORLD);
		Point3 v = FNormalize(nodePos - sc.P());
		float d = DotProd(sc.Normal(), v);
		float angle = 0.0f;
		if (d != 0.0f)
		{
			angle = acos(d) * 180.0f/PI;
		}
		if (angle > angleThreshold)
		{
			hidden = TRUE;
		}
	}

	//now check the zbuffer if need be
	if ((zbuffer != NULL) && useZBuffer)
	{
		//get the pixel in camera space
		Point3 tp = sc.P();
		//convert to world
		//then to our projection camera
		tp = sc.PointTo(tp, REF_WORLD) * invCamTM;


		double x, y, z;
		x = tp.x; 
		y = tp.y; 
		z = tp.z;

		double px,py;

		//compute where we are in screen space
		double xScale = -0.5 / (tan(0.5*(double)fov));
		Interface *ip = GetCOREInterface();
		float aspectRatio = ip->GetRendImageAspect();
		float yScale = xScale*aspectRatio;

		px = tp.x*xScale/z + 0.5;
		py = tp.y*yScale/z + 0.5;

		py = 1.0f - py;

		//figure out were we are in the bitmap
		int ix = (px * (float)(width-1) + 0.5);
		int iy = (py * (float)(height -1) + 0.5);
		int index = iy * width + ix;

		//if out of the UV space the pixel is hidden we dont tile the map should WE?
		if (px < 0.0f) hidden = TRUE;
		else if (px > 1.0f) hidden = TRUE;

		else if (py < 0.0f) hidden = TRUE;
		else if (py > 1.0f) hidden = TRUE;
		else
		{
			
			z = zbuffer[index];
			float dif = fabs(z - tp.z);
			//see if we arr behind the zbuffer
			if (dif > backFudge)
			{
				//we are behind the mask now comes the hack
				//since we dont have fragment data in the ZBuffer we can't exactly determine if this
				//sample is behind/infront of the mask
				//so we check the neighboring pixels, if they any one of them is infront of the mask
				//the sample is infront of the mask.  This creates a potential 1 pixel errors 
				//but is fine since the altenative is that it will create a halo
				int startX, endX;
				int startY, endY;
				startX = ix-1;
				endX = ix+1;
				startY = iy-1;
				endY = iy+1;

				if (startX < 0) startX = 0;
				if (startY < 0) startY = 0;
				if (endX > (width-1)) endX = (width-1);
				if (endY > (height-1)) endX = (height-1);
				BOOL pass = FALSE;

				for (int j = startY; j <= endY; j++)
				{
					for (int i = startX; i <= endX; i++)
					{
						int index = j * width + i;
						z = zbuffer[index];
						float dif = fabs(z - tp.z);
						if (dif < backFudge)
						{
							pass = TRUE;
							i = endX + 1;
							j = endY + 1;
						}

					}
				}

				if (!pass)
					hidden = TRUE;

			}			
		}
	}

	return hidden;

}

AColor CameraMapTexture::EvalColor(ShadeContext& sc)
{
	//TODO: Evaluate the color of texture map for the context.
	if (tex != NULL)
	{
		
		ObjectState camOState;
		Interval valid;

		MyShadeContext myContext = BuildMyShadeContext(sc);

		if (IsPixelHidden(sc))
			return AColor(0.0f,0.0f,0.0f,0.0f);

		AColor c = tex->EvalColor(myContext);		



		if ((mask) && useMask)
		{
			float m = 0.0f;
			if (maskUsesProjection)
				m = mask->EvalMono(myContext);
			else m = mask->EvalMono(sc);			
			c *= m;
		}
		return c;
	}
	else return AColor (0.0f,0.0f,0.0f,0.0f);
}

float CameraMapTexture::EvalMono(ShadeContext& sc)
{
	if (tex != NULL)
	{
		
		ObjectState camOState;
		Interval valid;

		MyShadeContext myContext = BuildMyShadeContext(sc);

		if (IsPixelHidden(sc))
			return 0.0f;


		float c = tex->EvalMono(myContext);


		if ((mask) && useMask)
		{
			float m = 0.0f;
			if (maskUsesProjection)
				m = mask->EvalMono(myContext);
			else m = mask->EvalMono(sc);			
			c *= m;
		}
		return c;
	}
	return 0.0f;
}

///not sure if this will work
Point3 CameraMapTexture::EvalNormalPerturb(ShadeContext& sc)
{
	if (tex != NULL)
	{
		
		ObjectState camOState;
		Interval valid;

		MyShadeContext myContext = BuildMyShadeContext(sc);


		if (IsPixelHidden(sc))
			return Point3(0.0f,0.f,0.0f);

		Point3 c = tex->EvalNormalPerturb(myContext);


		if ((mask) && useMask)
		{
			float m = 0.0f;
			if (maskUsesProjection)
				m = mask->EvalMono(myContext);
			else m = mask->EvalMono(sc);			
			c *= m;
		}
		return c;
	}	
	return Point3(0, 0, 0);
}


void CameraMapTexture::ActivateTexDisplay(BOOL onoff)
{
	//TODO: Implement this only if SupportTexDisplay() returns TRUE
}

DWORD_PTR CameraMapTexture::GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker)
{
	//TODO: Return the texture handle to this texture map
	return NULL;
}

imrShader* CameraMapTexture::GetMRShader(imrShaderCreation& shaderCreation) {

	// Create the shader and return it
	imrShader* shader = shaderCreation.CreateShader(_T("max_CameraMap"), GetFullName());
	return shader;
}

void CameraMapTexture::ReleaseMRShader() {

}

bool CameraMapTexture::NeedsAutomaticParamBlock2Translation() {

	return false;
}

void CameraMapTexture::TranslateParameters(imrTranslation& translationInterface, imrShader* shader, TimeValue t, Interval& valid) {

	// Get the parameter block in which we need to store the parameter values
	IParamBlock2* paramsPBlock = shader->GetParametersParamBlock();
	if(paramsPBlock != NULL) {

		Interval localValid = FOREVER;

		// Update at the current time 
		Update(t, localValid);

		TranslateMRShaderParameter(paramsPBlock, _T("worldToCameraTM"), invCamTM);
		TranslateMRShaderParameter(paramsPBlock, _T("cameraFOV"), fov);
		TranslateMRShaderParameter(paramsPBlock, _T("cameraPos"), cameraPos);
		TranslateMRShaderParameter(paramsPBlock, _T("map_channel"), mapChannel);
		TranslateMRShaderParameter(paramsPBlock, _T("textureShader"), tex);
		TranslateMRShaderParameter(paramsPBlock, _T("backface"), backFace);
		TranslateMRShaderParameter(paramsPBlock, _T("useZBuffer"), useZBuffer);
		TranslateMRShaderParameter(paramsPBlock, _T("zbufferWidth"), width);
		TranslateMRShaderParameter(paramsPBlock, _T("zbufferHeight"), height);
		TranslateMRShaderParameter(paramsPBlock, _T("zfudge"), backFudge);
		TranslateMRShaderParameter(paramsPBlock, _T("angleThreshold"), angleThreshold);
		TranslateMRShaderParameter(paramsPBlock, _T("maskShader"), mask);
		TranslateMRShaderParameter(paramsPBlock, _T("useMask"), useMask);
		TranslateMRShaderParameter(paramsPBlock, _T("maskUsesProjection"), maskUsesProjection);

		valid &= localValid;
	}
	else {
		// This shouldn't happen
		DbgAssert(false);
	}
}

void CameraMapTexture::GetAdditionalReferenceDependencies(AdditionalDependencyTable& refTargets) {

	// None
}

void CameraMapTexture::TranslateParameters_Advanced(imrAdvancedTranslation& advancedTranslation, TimeValue t, Interval& valid) {

	// Translate the zbuffer, if necessary, by using the mental ray API directly.
	// We translate the zbuffer this way because it is much more efficient that passing through a parameter block
	// array parameter.
	if(advancedTranslation.mi_api_parameter_name(advancedTranslation._mi_mem_strdup("zbuffer"))) {

		advancedTranslation.mi_api_parameter_push(imrAdvancedTranslation::miTRUE);
		if(useZBuffer && (zbuffer != NULL)) {
			int zbufferSize = width * height;
			for(int i = 0; i < zbufferSize; ++i) {
				advancedTranslation.mi_api_new_array_element();
				advancedTranslation.mi_api_parameter_value(imrAdvancedTranslation::miTYPE_SCALAR, &zbuffer[i], NULL, NULL);
			}
		}
		advancedTranslation.mi_api_parameter_pop();
	}
	else {
		DbgAssert(false);
	}
}

BaseInterface* CameraMapTexture::GetInterface(Interface_ID id) {

	if(id == IMRSHADERTRANSLATION_INTERFACE_ID) {
		return static_cast<imrShaderTranslation*>(this);
	}
	else {
		return Texmap::GetInterface(id);
	}
}
