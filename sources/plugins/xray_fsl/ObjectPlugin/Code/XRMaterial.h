#pragma once

// max stuff
#include "Max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

// xrFSL stuff
#include "XRMtlResource.h"
#include "CompilerShadersLib.h"
#include "EngineShadersLib.h"
#include "GameMaterialsLib.h"

// mtl stuff
#include "texmaps.h"


extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define xrFSLMaterial_CLASS_ID	Class_ID(0x9fd6ac09, 0x8f860bc8)

#define NSUBMTL		1 // TODO: number of sub-materials supported by this plugin 
#define NTEXMAPS	1	// number of texmaps

#define PBLOCK_REF	NSUBMTL

/**
 *	
 */
class xrFSLMaterial : public Mtl {
public:


	// Parameter block
	IParamBlock2* pblock;			//ref 0
	Texmaps* m_texMaps;				// ref 1


	Mtl				*submtl[NSUBMTL];  //array of sub-materials
	BOOL			mapOn[NSUBMTL];
	float			spin;
	Interval		ivalid;

	// xr parameters
	BOOL m_bDblSided;

	// xr shaders
	static xrFSL::CompilerShadersLib m_CShadersLib;
	static xrFSL::EngineShadersLib m_EShadersLib;
	static xrFSL::GameMaterialsLib m_GMaterialsLib;



	ParamDlg *CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
	void Update(TimeValue t, Interval& valid);
	Interval Validity(TimeValue t);
	void Reset();

	void NotifyChanged();

	// From MtlBase and Mtl
	void SetAmbient(Color c, TimeValue t);		
	void SetDiffuse(Color c, TimeValue t);		
	void SetSpecular(Color c, TimeValue t);
	void SetShininess(float v, TimeValue t);
	Color GetAmbient(int mtlNum=0, BOOL backFace=FALSE);
	Color GetDiffuse(int mtlNum=0, BOOL backFace=FALSE);
	Color GetSpecular(int mtlNum=0, BOOL backFace=FALSE);
	float GetXParency(int mtlNum=0, BOOL backFace=FALSE);
	float GetShininess(int mtlNum=0, BOOL backFace=FALSE);		
	float GetShinStr(int mtlNum=0, BOOL backFace=FALSE);
	float WireSize(int mtlNum=0, BOOL backFace=FALSE);


	// Shade and displacement calculation
	void Shade(ShadeContext& sc);
	float EvalDisplacement(ShadeContext& sc); 
	Interval DisplacementValidity(TimeValue t); 	

	// SubMaterial access methods
	int NumSubMtls() {return NSUBMTL;}
	Mtl* GetSubMtl(int i);
	void SetSubMtl(int i, Mtl *m);
	TSTR GetSubMtlSlotName(int i);
	TSTR GetSubMtlTVName(int i);

	/*+++++++++ SubTexmap access methods +++++++++++*/
	int NumSubTexmaps() {return NTEXMAPS;}
	Texmap* GetSubTexmap(int i) { return (*m_texMaps)[i].map; };
	void SetSubTexmap(int i, Texmap *m);
	TSTR GetSubTexmapSlotName(int i);
	TSTR GetSubTexmapTVName(int i);
	/*++++++++++ end SubTexmap ++++++++++++++++++++++*/

	BOOL SetDlgThing(ParamDlg* dlg);
	xrFSLMaterial(BOOL loading);


	// Loading/Saving
	IOResult Load(ILoad *iload);
	IOResult Save(ISave *isave);

	//From Animatable
	Class_ID ClassID() {return xrFSLMaterial_CLASS_ID;}		
	SClass_ID SuperClassID() { return MATERIAL_CLASS_ID; }
	void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}

	RefTargetHandle Clone( RemapDir &remap );
	RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
		PartID& partID,  RefMessage message);


	int NumSubs() { return 1+NSUBMTL; }
	Animatable* SubAnim(int i); 
	TSTR SubAnimName(int i);

	// TODO: Maintain the number or references here 
	int NumRefs() { return 1+NSUBMTL; }
	RefTargetHandle GetReference(int i);
	void SetReference(int i, RefTargetHandle rtarg);



	int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
	IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
	IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

	void DeleteThis() { delete this; }
};



class xrFSLMaterialClassDesc : public ClassDesc2 {
public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new xrFSLMaterial(loading); }
	const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
	SClass_ID		SuperClassID() { return MATERIAL_CLASS_ID; }
	Class_ID		ClassID() { return xrFSLMaterial_CLASS_ID; }
	const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

	const TCHAR*	InternalName() { return _T("xrFSLMaterial"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }					// returns owning module handle


};