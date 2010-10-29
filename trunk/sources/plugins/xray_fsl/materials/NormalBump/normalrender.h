/**********************************************************************
 *<
	FILE: normalrender.h

	DESCRIPTION:	Includes for Plugins

	CREATED BY: Ben Lipman

	HISTORY:

 *>	Copyright (c) 2003, All Rights Reserved.
 **********************************************************************/

#ifndef __NORMALRENDER__H
#define __NORMALRENDER__H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include "stdmat.h"
#include "imtl.h"
#include "macrorec.h"
#include "plugapi.h"

#define ALIGN_DWORD	__declspec(align(4))

#define CATALYST

#ifdef CATALYST
#include <IMtlRender_Compatibility.h>
#include <mentalray\mentalrayInMax.h>
#include <mentalray\imrShader.h>
#include <mentalray\imrShaderTranslation.h>
#endif

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define SCANLINERENDERER_CLASS_ID				Class_ID(SREND_CLASS_ID,0)



//===========================================================================
//
// Class Gnormal
//
//===========================================================================

#define GNORMAL_CLASS_ID	Class_ID(0x243e22c6, 0x63f6a014)
#define NSUBTEX		2 
#define PBLOCK_REF	0

#define MTL_HDR_CHUNK 0x4000
#define PARAM2_CHUNK 0x2030

// Values for Gnormal::method
#define TANGENTUV_SPACE		0
#define TANGENTLOCAL_SPACE	4
#define SCREEN_SPACE		2
#define WORLDXYZ_SPACE		3
#define LOCALXYZ_SPACE		1


class Gnormal : public Texmap 
#ifdef CATALYST
	, private imrShaderTranslation 
#endif
{
	public:
		Gnormal();
		~Gnormal();		

		//-- UI and Param Block
		enum { gnormal_params };
		enum { 
			gn_mult_spin, gn_bmult_spin,
			gn_map_normal, gn_map_bump,
			gn_map1on, gn_map2on,
			gn_method,
			gn_flip_red, gn_flip_green, gn_swap_rg,
		};

		void				Reset();
		void				Init();
		void				InvalidateUI();

		inline void			Reorder(AColor &s);
		inline void			Reorder(Point3& p);


		//-- From Animatable
		void				DeleteThis()			{ delete this; }
#ifdef LOCALIZED_APL		// For some reason multibyte data is invalid here -- LOCALIZATION_TODO
		void				GetClassName(TSTR& s)	{ s = _T("Normal Bump"); }
#else
		void				GetClassName(TSTR& s)	{ s = GetString(IDS_CLASS_NAME); }
#endif	// LOCALIZED_APL
		Class_ID			ClassID()				{ return GNORMAL_CLASS_ID;}		
		SClass_ID			SuperClassID()			{ return TEXMAP_CLASS_ID; }

		int					NumSubs()				{ return 1+NSUBTEX; }
		Animatable*			SubAnim(int i); 
		TSTR				SubAnimName(int i);
		int					SubNumToRefNum(int subNum) { return subNum; }

		int					RenderBegin(TimeValue t, ULONG flags=0);
		int					RenderEnd(TimeValue t);

		int					NumParamBlocks()		{ return 1; } // return number of ParamBlocks in this instance
		IParamBlock2*		GetParamBlock(int i)	{ return pblock; } // return i'th ParamBlock
		IParamBlock2*		GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

		//-- From ReferenceMaker
		IOResult			Load(ILoad *iload);
		IOResult			Save(ISave *isave);

		int					NumRefs()				{ return 1+NSUBTEX; }
		RefTargetHandle		GetReference(int i);
		void				SetReference(int i, RefTargetHandle rtarg);

		RefResult			NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
								PartID& partID,  RefMessage message);

		//-- From ReferenceTarget
		RefTargetHandle		Clone( RemapDir &remap );

		//-- From ISubMap
		int					NumSubTexmaps()			{ return NSUBTEX; }
		Texmap*				GetSubTexmap(int i)		{ return subTex[i]; }
		void				SetSubTexmap(int i, Texmap *m);
		TSTR				GetSubTexmapSlotName(int i);

		//-- From MtlBase
		ULONG				LocalRequirements(int subMtlNum);
		void				LocalMappingsRequired(int subMtlNum, BitArray & mapreq, BitArray &bumpreq);

		void				Update(TimeValue t, Interval& valid);
		Interval			Validity(TimeValue t)	{Interval v; Update(t,v); return ivalid;}
		ParamDlg*			CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);

		BOOL				SupportTexDisplay()		{ return FALSE; }
		DWORD_PTR		GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker);
		void				ActivateTexDisplay(BOOL onoff);

		//-- From Texmap
		AColor				EvalColor(ShadeContext& sc);
		float				EvalMono(ShadeContext& sc);
		Point3				EvalNormalPerturb(ShadeContext& sc);

		//-- Mental Ray support
#ifdef CATALYST
	private:
		// -- from InterfaceServer
		BaseInterface*		GetInterface(Interface_ID id);
		// -- from imrShaderTranslation
		imrShader*			GetMRShader(imrShaderCreation& shaderCreation);
		void				ReleaseMRShader();
		bool				NeedsAutomaticParamBlock2Translation();
		void				TranslateParameters(imrTranslation& translationInterface, imrShader* shader, TimeValue t, Interval& valid);
		void				GetAdditionalReferenceDependencies(AdditionalDependencyTable& refTargets);
		virtual bool HasRequirement(Requirement, TimeValue t, Interval& valid, void* arg);
#endif

	protected:
		//user
		float				gnMult, gnBMult;
		BOOL				flip_red, flip_green, swap_rg;
		BOOL				map1on, map2on;

		// Parameter block
		IParamBlock2		*pblock;	//ref 0
		Texmap				*subTex[NSUBTEX]; //array of sub-materials
		Interval			ivalid;
		int					method;
};


//===========================================================================
//
// Class ReaderWriterLock
//
//===========================================================================

class ReaderWriterLock {
	public:
		ReaderWriterLock()	{bits=0;}
		void				EnterRead();
		void				ExitRead();
		void				EnterWrite();
		void				ExitWrite();
	protected:
		// Lower bits [30..0] is the reader count, upper bit [31] is the writer
		ALIGN_DWORD DWORD	bits;
};

#endif // __NORMALRENDER__H
