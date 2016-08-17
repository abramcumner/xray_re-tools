#include "XRMaterial.h"

// xrFSL stuff
#include "FileSystem.h"
#include "Log.h"



static xrFSLMaterialClassDesc xrFSLMaterialDesc;
ClassDesc2* GetxrFSLMaterialDesc() { return &xrFSLMaterialDesc; }


enum { xrfslmaterial_params };


//TODO: Add enums for various parameters
enum { 
	// 
	xr_dbl_sided,		// check box 2-sided
	xr_eshader,			// combobox engine shader
	xr_cshader,			// combobox compiler shader
	xr_gshader,			// combobox game shader

	pb_spin,
	mtl_mat1,
	mtl_mat1_on,
};


class ShaderPBAccessor : public PBAccessor
{
public:
	void Set(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)    // set from v
	{
/*
		StdMtl2* m = (StdMtl2*)owner;
		switch (id)
		{
			// use item data to unscramble sorted lists
		case std2_shader_type: {
			ClassDesc* pCD = StdMtl2::GetShaderCD(v.i);
			if (pCD && m->GetShaderIndx() != NO_UPDATE )
			{
				m->SwitchShader(pCD); 
				m->shaderId = v.i; 
			}
							   } break;
		case std2_shader_by_name: {
			if( v.s!=NULL ) {
				for (int i = 0; i < StdMtl2::NumShaders(); i++)
				{
					ClassDesc* pCD = StdMtl2::GetShaderCD(i);
					if (_tcsicmp(pCD->ClassName(), v.s) == 0)
					{
						m->pb_shader->SetValue(std2_shader_type, 0, i);
						break;
					}
				}
			}
								  } break;
		case std2_wire:
			m->SetFlag(STDMTL_WIRE, v.i); break;		
		case std2_two_sided:
			m->SetFlag(STDMTL_2SIDE, v.i); break;		
		case std2_face_map:
			m->SetFlag(STDMTL_FACEMAP, v.i); break;
		case std2_faceted:
			m->SetFlag(STDMTL_FACETED, v.i); break;
		}*/

	}

	void Get(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t, Interval& valid)    // get into v
	{
		/*
		StdMtl2* m = (StdMtl2*)owner;
				switch (id)
				{
				case std2_shader_by_name: {
					ClassDesc* pCD = StdMtl2::GetShaderCD(m->shaderId);
					if (pCD)
						v.s = (TCHAR*)pCD->ClassName();
										  } break;
				}*/
		
	}
};

static ShaderPBAccessor shaderPBAccessor;


static void SetComboBoxItems(HWND hWnd, int nIDDlgItem, const std::vector<std::string>& vecItems)
{
	//  load the shader dropdown
	HWND hwndComboBox = GetDlgItem(hWnd, nIDDlgItem);
	SendMessage(hwndComboBox, CB_RESETCONTENT, 0L, 0L);

	std::string sName;

	for (std::vector<std::string>::const_iterator it = vecItems.begin(),
		end = vecItems.end(); it != end; ++it) 
	{
		sName = *it;
		ComboBox_AddString(hwndComboBox, (LPARAM)sName.c_str());
	}

	// Если список имён - пустой, то добавим значение по умолчанию и выберем его
	if (vecItems.empty())
		ComboBox_AddString(hwndComboBox, "default");
	ComboBox_SelectString(hwndComboBox, 0, "default");
}

class xrMaterialDlgProc : public ParamMap2UserDlgProc 
{
public:
	INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		xrFSLMaterial* m = (xrFSLMaterial*)map->GetParamBlock()->GetOwner();
		switch (msg) 
		{
		case WM_INITDIALOG:
			//----------------------------
			// Заполнение combobox-ов
			//----------------------------
			xrFSL::EngineShadersLib EShadersLib;
			if (EShadersLib.load(xrFSL::PA_GAME_DATA, "shaders.xr"))
			{
				SetComboBoxItems(hWnd, IDC_COMBO_ES, EShadersLib.names());
			}
			else
			{
				LOGE("Can't open shaders.xr");
			}

			xrFSL::CompilerShadersLib CShadersLib;
			if (CShadersLib.load(xrFSL::PA_GAME_DATA, "shaders_xrlc.xr"))
			{
				HWND hwndComboBox = GetDlgItem(hWnd, IDC_COMBO_CS);
				SendMessage(hwndComboBox, CB_RESETCONTENT, 0L, 0L);

				for (xrFSL::SCompilerShader_vec_cit it = CShadersLib.shaders().begin(),
					end = CShadersLib.shaders().end(); it != end; ++it) 
				{
					ComboBox_AddString(hwndComboBox, (*it)->sName.c_str());
				}

				// Если список имён - пустой, то добавим значение по умолчанию и выберем его
				if (CShadersLib.shaders().empty())
					ComboBox_AddString(hwndComboBox, "default");
				ComboBox_SelectString(hwndComboBox, 0, "default");
			}
			else
			{
				LOGE("Can't open shaders_xrlc.xr");
			}

			xrFSL::GameMaterialsLib GMaterialsLib;
			if (GMaterialsLib.load(xrFSL::PA_GAME_DATA, "gamemtl.xr"))
			{
				HWND hwndComboBox = GetDlgItem(hWnd, IDC_COMBO_GS);
				SendMessage(hwndComboBox, CB_RESETCONTENT, 0L, 0L);

				for (xrFSL::SGameMaterial_vec_cit it = GMaterialsLib.materials().begin(),
					end = GMaterialsLib.materials().end(); it != end; ++it) 
				{
					ComboBox_AddString(hwndComboBox, (*it)->sName.c_str());
				}

				// Если список имён - пустой, то добавим значение по умолчанию и выберем его
				if (GMaterialsLib.materials().empty())
					ComboBox_AddString(hwndComboBox, "default");
				ComboBox_SelectString(hwndComboBox, 0, "default");
			}
			else
			{
				LOGE("Can't open gamemtl.xr");
			}
/*

			// conditionally set map amount spinner ranges and padlock button images
			LoadStdMtl2Resources();
			int bumpChan = m->stdIDToChannel[ ID_BU ];
			int dispChan = m->stdIDToChannel[ ID_DP ];
			int slevChan = m->stdIDToChannel[ ID_SS ];

			for (int i = 0; i < STD2_NMAX_TEXMAPS; i++) 
			{
				if (i == bumpChan || i == dispChan || i == slevChan)
					map->SetRange(std2_map_amnts, -999, 999, i);
				else 
					map->SetRange(std2_map_amnts, 0, 100, i);
			}

			SetupPadLockButton(hWnd, IDC_LOCK_TEX01, TRUE);
			macroRecorder->Disable();
			{
				KillRefmsg kill( m->killRefmsg );
				m->UpdateLockADTex(TRUE);
			}
			macroRecorder->Enable();
			return TRUE;
			*/
			return TRUE;
		}

		return FALSE;
		
	}

	void DeleteThis() { }
};

static xrMaterialDlgProc xrMtlDlgProc;


static ParamBlockDesc2 xrfslmaterial_param_blk ( xrfslmaterial_params, _T("params"),  0, &xrFSLMaterialDesc, 
												P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
												//rollout
												IDD_PANEL, IDS_PARAMS, 0, 0, &xrMtlDlgProc,
												//-------------
												// xr params
												//-------------
												// 2-sided checkbox
												xr_dbl_sided,	_T("xr_dbl_sided"),		TYPE_BOOL,				0,		IDS_DBL_SIDED,
												p_default,		FALSE,
												p_ui, 			TYPE_SINGLECHEKBOX,		IDC_CHECK_DBL_SIDED,
												p_accessor,		&shaderPBAccessor,
												end,
												// compiler shader combobox
												xr_cshader,		_T("xr_cshader"),		TYPE_INT,			0,		IDS_CSHADER,
												p_default,      3,
												p_ui, 			TYPE_INTLISTBOX,		IDC_COMBO_CS,			0,
												p_accessor,		&shaderPBAccessor,		
												end,

												// params
												pb_spin, 			_T("spin"), 		TYPE_FLOAT, 	P_ANIMATABLE, 	IDS_SPIN, 
												p_default, 		0.1f, 
												p_range, 		0.0f,1000.0f, 
												p_ui, 			TYPE_SPINNER,		EDITTYPE_FLOAT, IDC_EDIT,	IDC_SPIN, 0.01f, 
												end,
												mtl_mat1,			_T("mtl_mat1"),			TYPE_MTL,	P_OWNERS_REF,	IDS_MTL1,
												p_refno,		0,
												p_submtlno,		0,		
												p_ui,			TYPE_MTLBUTTON, IDC_MTL1,
												end,
												mtl_mat1_on,		_T("mtl_mat1_on"),		TYPE_BOOL,		0,				IDS_MTL1ON,
												p_default,		TRUE,
												p_ui,			TYPE_SINGLECHEKBOX, IDC_MTLON1,
												end,

												end
												);





xrFSLMaterial::xrFSLMaterial(BOOL loading) 
{
	for (int i=0; i<NSUBMTL; i++) submtl[i] = NULL;
	pblock = NULL;

	if (!loading) 
		Reset();
}


void xrFSLMaterial::Reset() 
{
	ivalid.SetEmpty();
	for (int i=0; i<NSUBMTL; i++) {
		if( submtl[i] ){ 
			DeleteReference(i);
			submtl[i] = NULL;
		}

	}

	xrFSLMaterialDesc.MakeAutoParamBlocks(this);
}



ParamDlg* xrFSLMaterial::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) 
{
	IAutoMParamDlg* masterDlg = xrFSLMaterialDesc.CreateParamDlgs(hwMtlEdit, imp, this);

	// TODO: Set param block user dialog if necessary
	return masterDlg;
}

BOOL xrFSLMaterial::SetDlgThing(ParamDlg* dlg)
{
	return FALSE;
}

Interval xrFSLMaterial::Validity(TimeValue t)
{
	Interval valid = FOREVER;		

	for (int i=0; i<NSUBMTL; i++) 
	{
		if (submtl[i]) 
			valid &= submtl[i]->Validity(t);
	}

	float u;
	pblock->GetValue(pb_spin,t,u,valid);
	return valid;
}

/*===========================================================================*\
|	Subanim & References support
\*===========================================================================*/

RefTargetHandle xrFSLMaterial::GetReference(int i) 
{
	if (i < NSUBMTL )
		return submtl[i];
	else return pblock;
}

void xrFSLMaterial::SetReference(int i, RefTargetHandle rtarg) 
{
	if (i < NSUBMTL)
		submtl[i] = (Mtl *)rtarg; 
	else pblock = (IParamBlock2 *)rtarg; 
}

TSTR xrFSLMaterial::SubAnimName(int i) 
{
	if (i < NSUBMTL)
		return GetSubMtlTVName(i);
	else return TSTR(_T(""));
}

Animatable* xrFSLMaterial::SubAnim(int i) {
	if (i < NSUBMTL)
		return submtl[i]; 
	else return pblock;
}

RefResult xrFSLMaterial::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
										  PartID& partID, RefMessage message ) 
{
	switch (message) {
		case REFMSG_CHANGE:
			ivalid.SetEmpty();
			if (hTarget == pblock)
			{
				ParamID changing_param = pblock->LastNotifyParamID();
				xrfslmaterial_param_blk.InvalidateUI(changing_param);
			}
			break;

	}
	return REF_SUCCEED;
}

/*===========================================================================*\
|	SubMtl get and set
\*===========================================================================*/

Mtl* xrFSLMaterial::GetSubMtl(int i)
{
	if (i < NSUBMTL )
		return submtl[i];
	return NULL;
}

void xrFSLMaterial::SetSubMtl(int i, Mtl *m)
{
	ReplaceReference(i,m);
	// TODO: Set the material and update the UI	
}

TSTR xrFSLMaterial::GetSubMtlSlotName(int i)
{
	// Return i'th sub-material name 
	return _T(""); 
}

TSTR xrFSLMaterial::GetSubMtlTVName(int i)
{
	return GetSubMtlSlotName(i);
}

/*===========================================================================*\
|	Texmap get and set
\*===========================================================================*/

void xrFSLMaterial::SetSubTexmap(int i, Texmap *m)
{
	assert(i < NTEXMAPS);

	m_texMaps->ReplaceReference(2 * i + 1, m);

	if (m != NULL) 
	{
		//EnableMap(i, TRUE);
		if (m_texMaps->txmap[i].amtCtrl == NULL) 
		{       
			m_texMaps->ReplaceReference(2*i, NewDefaultFloatController());
			m_texMaps->txmap[i].amtCtrl->SetValue(TimeValue(0), &m_texMaps->txmap[i].amount);
		}
	}
	else 
	{
		if (m_texMaps->txmap[i].amtCtrl != NULL)         
			m_texMaps->DeleteReference(2*i);

		//SetTexmapAmt(i, i==ID_BU ? BUMP_DEF_AMT : 1.0f, TimeValue(0));
		//EnableMap(i,FALSE);
	}

	/*
	if (paramDlg)
		paramDlg->UpdateTexmapDisplay(i);
	*/
}

TSTR xrFSLMaterial::GetSubTexmapSlotName(int i)
{
	return _T("Diffuse");
}

TSTR xrFSLMaterial::GetSubTexmapTVName(int i)
{
	// Return i'th sub-texture name 
	return GetSubTexmapSlotName(i);
}



/*===========================================================================*\
|	Standard IO
\*===========================================================================*/

#define MTL_HDR_CHUNK 0x4000

IOResult xrFSLMaterial::Save(ISave *isave) { 
	IOResult res;
	isave->BeginChunk(MTL_HDR_CHUNK);
	res = MtlBase::Save(isave);
	if (res!=IO_OK) return res;
	isave->EndChunk();

	return IO_OK;
}	

IOResult xrFSLMaterial::Load(ILoad *iload) { 
	IOResult res;
	int id;
	while (IO_OK==(res=iload->OpenChunk())) {
		switch(id = iload->CurChunkID())  {
			case MTL_HDR_CHUNK:
				res = MtlBase::Load(iload);
				break;
		}
		iload->CloseChunk();
		if (res!=IO_OK) 
			return res;
	}

	return IO_OK;
}


/*===========================================================================*\
|	Updating and cloning
\*===========================================================================*/

RefTargetHandle xrFSLMaterial::Clone(RemapDir &remap) {
	xrFSLMaterial *mnew = new xrFSLMaterial(FALSE);
	*((MtlBase*)mnew) = *((MtlBase*)this); 
	mnew->ReplaceReference(NSUBMTL,remap.CloneRef(pblock));

	mnew->ivalid.SetEmpty();	
	for (int i = 0; i<NSUBMTL; i++) {
		mnew->submtl[i] = NULL;
		if (submtl[i])
			mnew->ReplaceReference(i,remap.CloneRef(submtl[i]));
		mnew->mapOn[i] = mapOn[i];
	}
	BaseClone(this, mnew, remap);
	return (RefTargetHandle)mnew;
}

void xrFSLMaterial::NotifyChanged() 
{
	NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
}

void xrFSLMaterial::Update(TimeValue t, Interval& valid) 
{	
	if (!ivalid.InInterval(t)) {

		ivalid.SetInfinite();
		pblock->GetValue( mtl_mat1_on, t, mapOn[0], ivalid);
		pblock->GetValue( pb_spin, t, spin, ivalid);

		for (int i=0; i<NSUBMTL; i++) {
			if (submtl[i]) 
				submtl[i]->Update(t,ivalid);
		}
	}
	valid &= ivalid;
}

/*===========================================================================*\
|	Determine the characteristics of the material
\*===========================================================================*/

void xrFSLMaterial::SetAmbient(Color c, TimeValue t) {}		
void xrFSLMaterial::SetDiffuse(Color c, TimeValue t) {}		
void xrFSLMaterial::SetSpecular(Color c, TimeValue t) {}
void xrFSLMaterial::SetShininess(float v, TimeValue t) {}

Color xrFSLMaterial::GetAmbient(int mtlNum, BOOL backFace)
{
	return submtl[0]?submtl[0]->GetAmbient(mtlNum,backFace):Color(0,0,0);
}

Color xrFSLMaterial::GetDiffuse(int mtlNum, BOOL backFace)
{
	return submtl[0]?submtl[0]->GetDiffuse(mtlNum,backFace):Color(0,0,0);
}

Color xrFSLMaterial::GetSpecular(int mtlNum, BOOL backFace)
{
	return submtl[0]?submtl[0]->GetSpecular(mtlNum,backFace):Color(0,0,0);
}

float xrFSLMaterial::GetXParency(int mtlNum, BOOL backFace)
{
	return submtl[0]?submtl[0]->GetXParency(mtlNum,backFace):0.0f;
}

float xrFSLMaterial::GetShininess(int mtlNum, BOOL backFace)
{
	return submtl[0]?submtl[0]->GetShininess(mtlNum,backFace):0.0f;
}

float xrFSLMaterial::GetShinStr(int mtlNum, BOOL backFace)
{
	return submtl[0]?submtl[0]->GetShinStr(mtlNum,backFace):0.0f;
}

float xrFSLMaterial::WireSize(int mtlNum, BOOL backFace)
{
	return submtl[0]?submtl[0]->WireSize(mtlNum,backFace):0.0f;
}


/*===========================================================================*\
|	Actual shading takes place
\*===========================================================================*/

void xrFSLMaterial::Shade(ShadeContext& sc) 
{
	Mtl *sm1 = mapOn[0]?submtl[0]:NULL;
	if (gbufID) sc.SetGBufferID(gbufID);

	if(sm1) sm1->Shade(sc);

	// TODO: compute the color and transparency output returned in sc.out.
}

float xrFSLMaterial::EvalDisplacement(ShadeContext& sc)
{
	Mtl *sm1 = mapOn[0]?submtl[0]:NULL;
	return (sm1)?sm1->EvalDisplacement(sc):0.0f;
}

Interval xrFSLMaterial::DisplacementValidity(TimeValue t)
{
	Mtl *sm1 = mapOn[0]?submtl[0]:NULL;

	Interval iv; iv.SetInfinite();
	if(sm1) iv &= sm1->DisplacementValidity(t);

	return iv;	
}