/**********************************************************************
*<
FILE: BMTEX.CPP

DESCRIPTION: BMTEX 2D Texture map.

CREATED BY: Dan Silva

HISTORY:

*>	Copyright (c) 1994, All Rights Reserved.
**********************************************************************/

#include "bmtex.h"
#include "mtlhdr.h"
#include "mtlres.h"
#include "3dsmaxport.h"
#include "notify.h"
#include "macrorec.h"

extern HINSTANCE hInstance;

// Image filtering types
#define FILTER_PYR     0
#define FILTER_SAT     1
#define FILTER_NADA	   2

// Alpha source types
#define ALPHA_FILE 	0
#define ALPHA_RGB	2
#define ALPHA_NONE	3

// End conditions:
#define END_LOOP     0
#define END_PINGPONG 1
#define END_HOLD     2

#define NCOLS 0

#define UVGEN_REF 0
#define PBLOCK_REF 1 
#define TEXOUT_REF 2
#define PBLOCKTIME_REF 3 

// Interface ID used to access the particle bitmaps of the Bitmap texture
#define IID_BITMAPTEX_PARTICLE_BITMAP_LIST 0x3c34ffff

//------------------------------------------------------------------------
// Local definitions
//------------------------------------------------------------------------

namespace {

	Class_ID bmTexClassID(BMTEX_CLASS_ID,0);
	AColor black(0.0f,0.0f,0.0f,0.0f);


	void InvalidNameMsg(const TCHAR* name) {
		if (name==NULL) return;
		TCHAR msg[128];
		wsprintf(msg,_T("Invalid File Name: %s"), name);
		MessageBox(NULL, msg, GetString(IDS_DS_BITMAP_TEXTURE_ERR), MB_TASKMODAL);
	}

	BitmapInfo* GetSubjectInfo( BitmapInfo* srcInfo ) {
		if( srcInfo->GetProxySubject() != NULL )
			return srcInfo->GetProxySubject();
		else 
			return srcInfo;
	}

	// Copy the proxy subject of srcInto into the given subject info, clear proxy request flag on subject info
	void CopySubjectToSubjectInfo( BitmapInfo& subjectInfo, BitmapInfo& srcInfo ) {
		subjectInfo = *GetSubjectInfo( &srcInfo );
		subjectInfo.ResetFlags( MAP_PROXYREQUEST ); // Mark as request for non-proxy
	}

	// Copy the proxy subject of srcInto into the given request info, set proxy request flag on request info
	void CopySubjectToRequestInfo( BitmapInfo& requestInfo, BitmapInfo& srcInfo ) {
		requestInfo = *GetSubjectInfo( &srcInfo );
		requestInfo.SetFlags( MAP_PROXYREQUEST ); // Mark as request for proxy
	}

	void BitmapNonProxy_LoadInto( BitmapInfo& requestInfo, Bitmap** pBitmap, BOOL forceReload ) {
		requestInfo.ResetFlags( MAP_PROXYREQUEST );
		TheManager->LoadInto(&requestInfo, pBitmap, forceReload);
	}

	void BitmapProxy_LoadInto( BitmapInfo& subjectInfo, BitmapInfo& requestInfo, Bitmap** pBitmap, BOOL forceReload ) {
		CopySubjectToRequestInfo( requestInfo, subjectInfo ); // Create a request info for the proxy from our subject info
		TheManager->LoadInto( &requestInfo, pBitmap, forceReload );
		CopySubjectToSubjectInfo( subjectInfo, requestInfo ); // Copy the the updated subject info into our subject info
	}

	Bitmap* BitmapNonProxy_Load( BitmapInfo& requestInfo, BMMRES* status ) {
		requestInfo.ResetFlags( MAP_PROXYREQUEST );
		Bitmap* retval = TheManager->Load(&requestInfo, status);
		return retval;
	}

	Bitmap* BitmapProxy_Load( BitmapInfo& subjectInfo, BitmapInfo& requestInfo, BMMRES* status ) {
		CopySubjectToRequestInfo( requestInfo, subjectInfo ); // Create a request info for the proxy from our subject info
		Bitmap* retval = TheManager->Load( &requestInfo, status );
		CopySubjectToSubjectInfo( subjectInfo, requestInfo ); // Copy the the updated subject info into our subject info
		return retval;
	}

	BMMRES BitmapNonProxy_GoTo( BitmapInfo& requestInfo, Bitmap* map ) {
		requestInfo.ResetFlags( MAP_PROXYREQUEST );
		BMMRES retval = map->GoTo(&requestInfo);
		return retval;
	}

	BMMRES BitmapProxy_GoTo( BitmapInfo& subjectInfo, BitmapInfo& requestInfo, Bitmap* map ) {
		BitmapInfo backupRequestInfo = requestInfo;
		CopySubjectToRequestInfo( requestInfo, subjectInfo ); // Create a request info for the proxy from our subject info
		BMMRES retval = map->GoTo( &requestInfo );
		CopySubjectToSubjectInfo( subjectInfo, requestInfo ); // Copy the the updated subject info into our subject info

		// Restore the original request information if the bitmap doesn't have multiple frames.
		// This is necessary to avoid losing proxy information from the requestInfo
		if(retval == BMMRES_SINGLEFRAME) {
			requestInfo = backupRequestInfo;
		}
		return retval;
	}
}

//------------------------------------------------------------------------
// BMTexPathAccessor -- Methods
//------------------------------------------------------------------------

BMTexPathAccessor::BMTexPathAccessor(BMTex* aTex) : mTex(aTex)	{
	DbgAssert(aTex);
}

BMTexPathAccessor::~BMTexPathAccessor()	{
}

MaxSDK::Util::Path BMTexPathAccessor::GetPath() const {
	return MaxSDK::Util::Path(mTex->GetMapName());
}


void BMTexPathAccessor::SetPath(const MaxSDK::Util::Path& aNewPath)	{
	mTex->pblock->SetValue(bmtex_filename, 0, const_cast<TCHAR*>(aNewPath.GetCStr()));
}

bool BMTexPathAccessor::IsInputAsset() const	{
	return true;
}

int BMTexPathAccessor::GetAssetType() const	{
	return IAssetAccessor::kBitmapAsset;
}

const TCHAR* BMTexPathAccessor::GetAssetDesc() const	{
	return NULL;
}
const TCHAR* BMTexPathAccessor::GetAssetClientDesc() const		{
	return NULL;
}
bool BMTexPathAccessor::IsAssetPathWritable() const	{
	return true;
}


TCHAR		BMTex::s_lastName[MAX_PATH];
ParamDlg*	BMTex::uvGenDlg;	
ParamDlg*	BMTex::texoutDlg;
BMTexDlg*	BMTex::paramDlg;

//------------------------------------------------------------------------
// BMSampler -- Methods
//------------------------------------------------------------------------
BMSampler::BMSampler() 
: bm(NULL), 
ufac(0.0f), 
vfac(0.0f),
fbmw(0.0f),
fbmh(0.0f)
{
}

void BMSampler::Init(BMTex *bmt, Bitmap *bm) {

	if (bm == NULL)
		this->bm = bmt->GetActiveBitmap();
	else this->bm = bm;

	tex = bmt; 
	alphaSource = bmt->alphaSource; 
	if (this->bm) {
		u0 = tex->clipu;
		v0 = tex->clipv;
		u1 = tex->clipu + tex->clipw;
		v1 = tex->clipv + tex->cliph;
		ufac= tex->clipw;
		vfac = tex->cliph;
		bmw = this->bm->Width();
		bmh = this->bm->Height();
		fbmw = float(bmw/*-1*/);  // DS 4/23/99
		fbmh = float(bmh/*-1*/);  // DS 4/23/99
		clipx = int(tex->clipu*fbmw);
		clipy = int(tex->clipv*fbmh);
		fclipw = tex->clipw*fbmw;
		fcliph = tex->cliph*fbmh;
		cliph = int(fcliph);
		ujit = tex->jitter*(1.0f-ufac);
		vjit = tex->jitter*(1.0f-vfac);
	}
}


static inline void Jitter(ShadeContext &sc, float ujit, float vjit, float &ru, float &rv, int iu, int iv) {
	//	srand(((sc.uTile+593)*14693+(sc.vTile+4991)*2517)&0x7fff);
	srand(((iu+593)*14693+(iv+967)*29517)&0x7fff);
	int urnd = rand()&0x7fff;
	rand();	 // these extra calls seem to make the pattern more random
	rand();	 // these extra calls seem to make the pattern more random
	int vrnd = rand()&0x7fff;
	ru = ujit*float(urnd)/float(32767.0f);
	rv = vjit*float(vrnd)/float(32767.0f);
}

int BMSampler::PlaceUV(ShadeContext &sc, float &u, float &v, int iu, int iv) {
	/* Avoid division by zero: make sure we don't have zero.*/
	if((ufac != 0) && (vfac != 0)) {
		if (tex->randPlace) {
			float ru,rv;
			Jitter(sc,ujit,vjit,ru,rv,iu,iv);
			if (u<ru||v<rv||u>ru+ufac||v>rv+vfac) 
				return 0;
			// note jitter ignores u0,v0, effectively setting them to zero
			u = (u-ru)/ufac;
			v = (v-rv)/vfac;  
		}
		else {
			if (u<u0||v<v0||u>u1||v>v1) 
				return 0;
			u = (u-u0)/ufac;
			v = (v-v0)/vfac;
		}
		return 1;
	}
	else {
		return 0;
	}
}

int BMSampler::PlaceUVFilter(ShadeContext &sc, float &u, float &v, int iu, int iv) {

	/* Avoid division by zero: make sure we don't have zero.*/
	if((ufac != 0) && (vfac != 0)) {
		if (tex->randPlace) {
			float ru,rv;
			Jitter(sc,ujit,vjit,ru,rv,iu,iv);
			// note jitter ignores u0,v0, effectively setting them to zero
			u = (u-ru)/ufac;
			v = (v-rv)/vfac;
		}
		else {
			u = (u-u0)/ufac;
			v = (v-v0)/vfac;
		}
		return 1;
	}
	else {
		return 0;
	}
}

AColor BMSampler::Sample(ShadeContext& sc, float u,float v) {
	BMM_Color_fl c;
	int x,y;
	float fu,fv;
	fu = frac(u);
	fv = 1.0f-frac(v);
	if (fu >= 1.0f)
		fu = 0.0f;
	if (fv >= 1.0f)
		fv = 0.0f;
	if (tex->applyCrop) {
		if (tex->placeImage) {
			if (!PlaceUV(sc,fu, fv, int(u), int(v))) 
				return black;
			x = (int)(fu*fbmw);
			y = (int)(fv*fbmh);
		}
		else {
			x = mod(clipx + (int)(fu*fclipw),bmw);
			y = mod(clipy + (int)(fv*fcliph),bmh);
		}
	}
	else {
		x = (int)(fu*fbmw);
		y = (int)(fv*fbmh);
	}
	bm->GetLinearPixels(x,y,1,&c);
	switch(alphaSource) {
		case ALPHA_NONE:  c.a = 1.0f; break;
		case ALPHA_RGB:   c.a = (c.r+c.g+c.b)/3; 
			if(c.a>1)c.a=1;
			else if(c.a<0)c.a=0;
			break;
			//  TBD
			// XPCOL needs to be handled in bitmap for filtering. 
			// Need to open a bitmap with this property.
			//	case ALPHA_XPCOL:  break; 
	}
	return c;
}


AColor BMSampler::SampleFilter(ShadeContext& sc, float u,float v, float du, float dv) {
	BMM_Color_fl c;
	float fu,fv;
	fu = frac(u);
	fv = 1.0f-frac(v);
	if (tex->applyCrop) {
		if (tex->placeImage) {
			if(!PlaceUVFilter(sc,fu, fv, int(u), int(v))) {
				return black;
			}
			du /= ufac;
			dv /= vfac;
			float du2 = 0.5f*du;
			float ua = fu-du2;
			float ub = fu+du2;
			if (ub<=0.0f||ua>=1.0f) return black;
			float dv2 = 0.5f*dv;
			float va = fv-dv2;
			float vb = fv+du2;
			if (vb<=0.0f||va>=1.0f) return black;
			BOOL clip = 0;
			if (ua<0.0f) { ua=0.0f; clip = 1; }
			if (ub>1.0f) { ub=1.0f;	clip = 1; }
			if (va<0.0f) { va=0.0f;	clip = 1; }
			if (vb>1.0f) { vb=1.0f;	clip = 1; }
			bm->GetFiltered(fu,fv,du,dv,&c);
			switch(alphaSource) {
				case ALPHA_NONE:  c.a = 1.0f; break;
				case ALPHA_RGB:   c.a = (c.r+c.g+c.b)/3; 
					if(c.a>1)c.a=1;
					else if(c.a<0)c.a=0;
					break;
			}
			AColor ac(c);
			if (clip) {
				float f = ((ub-ua)/du) * ((vb-va)/dv);
				ac *= f;
			}
			return ac;
		}
		else {
			fu = (u0 + ufac*fu);
			fv = (v0 + vfac*fv);
			du *= ufac;
			dv *= vfac;
			bm->GetFiltered(fu,fv,du,dv,&c);
		}
	}

	else 
		bm->GetFiltered(fu,fv,du,dv,&c);
	switch(alphaSource) {
				case ALPHA_NONE:  c.a = 1.0f; break;
				case ALPHA_RGB:   c.a = (c.r+c.g+c.b)/3; 
					if(c.a>1)c.a=1;
					else if(c.a<0)c.a=0;
					break;
	}
	return c;
}


//------------------------------------------------------------------------
// BMAlphaSampler -- Methods
//------------------------------------------------------------------------

float BMAlphaSampler::SampleMono(ShadeContext& sc, float u,float v) {
	BMM_Color_fl c;
	int x,y;
	float fu,fv;
	fu = frac(u);
	fv = 1.0f-frac(v);
	if (fu >= 1.0f)
		fu = 0.0f;
	if (fv >= 1.0f)
		fv = 0.0f;
	if (tex->applyCrop) {
		if (tex->placeImage) {
			if (!PlaceUV(sc,fu,fv,int(u),int(v))) return 0.0f;
			x = (int)(fu*fclipw);
			y = (int)(fv*fcliph);
		}
		else {
			x = mod(clipx + (int)(fu*fclipw),bmw);
			y = mod(clipy + (int)(fv*fcliph),bmh);
		}
	}
	else {
		x = (int)(fu*fbmw);
		y = (int)(fv*fbmh);
	}
	bm->GetLinearPixels(x,y,1,&c);
	return( c.a);
}

float BMAlphaSampler::SampleMonoFilter(ShadeContext& sc, float u,float v, float du, float dv) {
	BMM_Color_fl c;
	float fu,fv;
	fu = frac(u);
	fv = 1.0f-frac(v);
	if (tex->applyCrop) {
		if (tex->placeImage) {
			if(!PlaceUVFilter(sc,fu, fv, int(u), int(v))) {
				return 0.0f;
			}
			du /= ufac;
			dv /= vfac;
			float du2 = 0.5f*du;
			float ua = fu-du2;
			float ub = fu+du2;
			if (ub<=0.0f||ua>=1.0f) return 0.0f;
			float dv2 = 0.5f*dv;
			float va = fv-dv2;
			float vb = fv+du2;
			if (vb<=0.0f||va>=1.0f) return 0.0f;
			BOOL clip = 0;
			if (ua<0.0f) { ua=0.0f; clip = 1; }
			if (ub>1.0f) { ub=1.0f;	clip = 1; }
			if (va<0.0f) { va=0.0f;	clip = 1; }
			if (vb>1.0f) { vb=1.0f;	clip = 1; }
			bm->GetFiltered(fu,fv,du,dv,&c);
			float alph = c.a;
			if (clip) {
				float f = ((ub-ua)/du) * ((vb-va)/dv);
				alph *= f;
			}
			return alph;
		}
		else {
			fu = frac(u0 + ufac*fu);
			fv = frac(v0 + vfac*fv);
			du *= ufac;
			dv *= vfac;
		}
	}
	bm->GetFiltered(fu,fv,du,dv,&c);
	return( c.a);
}

//------------------------------------------------------------------------
// BMCropper  -- Methods
//------------------------------------------------------------------------

#ifndef NO_MTLEDITOR_BITMAP_CROPPING
void BMCropper::Init(BMTexDlg *txdlg, TimeValue t) {
	dlg = txdlg;
	tex = txdlg->theBMTex;
	u0 = tex->GetClipU(t);
	v0 = tex->GetClipV(t);
	w0 = tex->GetClipW(t);
	h0 = tex->GetClipH(t);
	mode= tex->placeImage;
}

void BMCropper::SetValues(float u, float v, float w, float h, BOOL md) {
	BOOL b = FALSE;
	if (u!=tex->clipu) {
		tex->SetClipU(dlg->ip->GetTime(), u);
		b = TRUE;		
	}
	if (v!=tex->clipv) {
		tex->SetClipV(dlg->ip->GetTime(), v);
		b = TRUE;		
	}
	if (w!=tex->clipw) {
		tex->SetClipW(dlg->ip->GetTime(), w);
		b = TRUE;		
	}
	if (h!=tex->cliph) {
		tex->SetClipH(dlg->ip->GetTime(), h);
		b = TRUE;		
	}

	if (md!=tex->placeImage) {
		tex->placeImage= md;
		b = TRUE;
	}

	if(b) {
		tex->DiscardTexHandle();
		tex->NotifyChanged();
		dlg->UpdateMtlDisplay();
	}
}

void BMCropper::OnClose(){
	dlg->cropping = FALSE;
}
#endif	// !NO_MTLEDITOR_BITMAP_CROPPING

//------------------------------------------------------------------------
// BMTexNotify -- Methods
//------------------------------------------------------------------------

int BMTexNotify::Changed(ULONG flags) {
	//-- File has been reloaded and it must be reflected as such
	if (flags == BMNOTIFY_FLAG_STORAGE_CHANGE) {
		tex->NotifyChanged();
		tex->DiscardTexHandle();
		//-- File has been changed and must be reloaded
		// GG: 02/10/02
	} else if (flags == BMNOTIFY_FLAG_FILE_CHANGE) {
		tex->fnReload();
	}
	return 1;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------

int numBMTexs = 0;
class BMTexClassDesc:public ClassDesc2 {
public:
	int 			IsPublic() { return 1; }
	void *			Create(BOOL loading);
	//	{ 	return new BMTex; }
	const TCHAR *	ClassName() { return GetString(IDS_DS_BITMAP_CDESC); } // mjm - 2.3.99
	SClass_ID		SuperClassID() { return TEXMAP_CLASS_ID; }
	Class_ID 		ClassID() { return bmTexClassID; }
	const TCHAR* 	Category() { return TEXMAP_CAT_2D;  }
	// PW: new descriptor data accessors added.  Note that the 
	//      internal name is hardwired since it must not be localized.
	const TCHAR*	InternalName() { return _T("bitmapTex"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }			// returns owning module handle

};

static BMTexClassDesc bmTexCD;

static int bmFilterType(int filt_type) {
	switch(filt_type) {
		case FILTER_PYR: return BMM_FILTER_PYRAMID;
		case FILTER_SAT: return BMM_FILTER_SUM;
		default: return BMM_FILTER_NONE;
	}
}


ClassDesc* GetBMTexDesc() { return &bmTexCD;  }

class BMTexPBAccessor : public PBAccessor
{
public:
	void Set(PB2Value& val, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)    // set from v
	{
		BMTex* bmt = (BMTex*)owner;
		switch (id)
		{
		case bmtex_bitmap:
			{
				// 425397
				// The code has been removed because "ReloadBitmapAndUpdate" calls the param block to get a pointer to "bmtex_bitmap"
				// The documentation is pretty clear about the fact that PBAccessor::Set is called right before the PB is changed.
				// The bmtex was reloading using the old data. Because the map was getting deleted, the process was acessing deleted memory
				//
				// I changed "NotifyRefChanged" to properly handle the change and reload its data.
				//if (!bmt->loading) {
				//	bmt->ReloadBitmapAndUpdate();
				//	// send message so browser/navigator name gets updated
				//	bmt->NotifyDependents(FOREVER,PART_ALL,REFMSG_SUBANIM_STRUCTURE_CHANGED);
				//	}
				break;
			}
		case bmtex_filename:
			{
				bmt->SetMapName(val.s);
				break;
			}
		case bmtex_filtering:
			//bmt->SetFilterType(val.i);
			bmt->filterType = val.i;
			bmt->m_theBitmap.SetFilter(bmFilterType(val.i));
			break;				
		case bmtex_clipu: {
			float u = val.f;
			float w = bmt->GetClipW(t);
			if (u+w>1.0f) 
				bmt->SetClipW(t,1.0f-u);
			break;
						  }
		case bmtex_clipv: {
			float v = val.f;
			float h = bmt->GetClipH(t);
			if (v+h>1.0f) 
				bmt->SetClipH(t,1.0f-v);
			break;
						  }
		case bmtex_clipw: {
			float w = val.f;
			float u = bmt->GetClipU(t);
			if (u+w>1.0f) 
				bmt->SetClipU(t,1.0f-w);
			break;
						  }
		case bmtex_cliph: {
			float h = val.f;
			float v = bmt->GetClipV(t);
			if (v+h>1.0f) 
				bmt->SetClipV(t,1.0f-h);
			break;
						  }
		}
	}
	void Get(PB2Value& v, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t, Interval &valid)    // get into v
	{
		BMTex* bmt = (BMTex*)owner;
		switch (id)
		{
		case bmtex_filename:
			{
				v.s = bmt->GetMapName();
				break;
			}

		}
	}
};

static BMTexPBAccessor bmtex_accessor;

static ParamBlockDesc2 bmtex_param_blk ( bmtex_params, _T("parameters"),  0, &bmTexCD, P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_BMTEX, IDS_DS_BITMAP_PARAMS, 0, 0, NULL, 
	// params

	bmtex_clipu,	_T("clipu"),   TYPE_FLOAT,			P_ANIMATABLE,	IDS_DS_CLIPU,
		p_default,		0.0,
		p_range,		0.0, 1.0,
		p_ui, 			TYPE_SPINNER, EDITTYPE_FLOAT,  IDC_CLIP_X, IDC_CLIP_XSPIN, 0.001f,
		p_accessor,		&bmtex_accessor,
		end,
	bmtex_clipv,	_T("clipv"),   TYPE_FLOAT,			P_ANIMATABLE,	IDS_DS_CLIPV,
		p_default,		0.0,
		p_range,		0.0, 1.0,
		p_ui, 			TYPE_SPINNER, EDITTYPE_FLOAT,  IDC_CLIP_Y, IDC_CLIP_YSPIN, 0.001f,
		p_accessor,		&bmtex_accessor,
		end,
	bmtex_clipw,	_T("clipw"),   TYPE_FLOAT,			P_ANIMATABLE,	IDS_DS_CLIPW,
		p_default,		0.0,
		p_range,		0.0, 1.0,
		p_ui, 			TYPE_SPINNER, EDITTYPE_FLOAT,  IDC_CLIP_W, IDC_CLIP_WSPIN, 0.001f,
		p_accessor,		&bmtex_accessor,
		end,
	bmtex_cliph,	_T("cliph"),   TYPE_FLOAT,			P_ANIMATABLE,	IDS_DS_CLIPH,
		p_default,		0.0,
		p_range,		0.0, 1.0,
		p_ui, 			TYPE_SPINNER, EDITTYPE_FLOAT,  IDC_CLIP_H, IDC_CLIP_HSPIN, 0.001f,
		p_accessor,		&bmtex_accessor,
		end,
	bmtex_jitter,	_T("jitter"),   TYPE_FLOAT,			P_ANIMATABLE,	IDS_DS_JITTERAMT,
		p_default,		0.0,
		p_range,		0.0, 1.0,
		p_ui, 			TYPE_SPINNER, EDITTYPE_FLOAT,  IDC_JITTER_EDIT, IDC_JITTER_SPIN, 0.01f,
		end,
	bmtex_usejitter,	_T("useJitter"), TYPE_BOOL,			0,		IDS_PW_USEJITTER,
		p_default,		FALSE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_BM_JITTER,
		end,
	bmtex_apply,	_T("apply"), TYPE_BOOL,			0,		IDS_PW_APPLY,
		p_default,		FALSE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_BM_CLIP,
		end,
	bmtex_crop_place, _T("cropPlace"), TYPE_INT,		0,		IDS_PW_CROPPLACE,
		p_default,		0,
		p_range,		0,	1,
		p_ui,			TYPE_RADIO, 2,  IDC_BM_CROP,IDC_BM_PLACE,
		end,

	bmtex_filtering, _T("filtering"), TYPE_INT,		0,		IDS_PW_FILTERING,
		p_default,		0,
		p_range,		0,	2,
		p_ui,			TYPE_RADIO, 3, IDC_FILTER_PYR, IDC_FILTER_SAT,IDC_FILTER_NADA,
		p_accessor,		&bmtex_accessor,
		end,
	bmtex_monooutput, _T("monoOutput"), TYPE_INT,		0,		IDS_PW_MONOOUTPUT,
		p_default,		0,
		p_range,		0,	1,
		p_ui,			TYPE_RADIO, 2, IDC_BMTEX_RGBOUT, IDC_BMTEX_ALPHAOUT,
		end,
	bmtex_rgboutput, _T("rgbOutput"), TYPE_INT,		0,		IDS_PW_RGBOUTPUT,
		p_default,		0,
		p_range,		0,	1,
		p_ui,			TYPE_RADIO, 2, IDC_BMTEX_RGBOUT2, IDC_BMTEX_ALPHAOUT2,
		end,
	bmtex_alphasource, _T("alphaSource"), TYPE_INT,		0,		IDS_PW_ALPHASOURCE,
		p_default,		0,
		p_range,		0,	2,
		p_ui,			TYPE_RADIO, 3, IDC_ALPHA_FILE, IDC_ALPHA_RGB,IDC_ALPHA_NONE,
		end,
	
	bmtex_premultalpha, _T("preMultAlpha"), TYPE_BOOL,		0,		IDS_PW_PREMULT,
		p_default,		TRUE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_ALPHA_PREMULT,
		end,

	bmtex_bitmap, _T("bitmap"),	TYPE_BITMAP, 0, IDS_DS_BITMAP,
		p_ui,			TYPE_BITMAPBUTTON, IDC_BMTEX_NAME,
		p_accessor,		&bmtex_accessor,
		end,
		
	bmtex_coords,		_T("coords"),	TYPE_REFTARG,		P_OWNERS_REF,	IDS_DS_COORDINATES,
		p_refno,		UVGEN_REF, 
		end,
	bmtex_output,		_T("output"),	TYPE_REFTARG,		P_OWNERS_REF,	IDS_DS_TEXOUT,
		p_refno,		TEXOUT_REF, 
		end,

	bmtex_filename,		_T("fileName"), TYPE_FILENAME,		P_TRANSIENT,	IDS_JW_FILENAME,
		p_accessor,		&bmtex_accessor,
		end,

	end
);




class BMTexPBTimeAccessor : public PBAccessor
{
public:
	void Set(PB2Value& val, ReferenceMaker* owner, ParamID id, int tabIndex, TimeValue t)    // set from v
	{
		BMTex* bmt = (BMTex*)owner;
		switch (id)	{
		case bmtex_start:
			bmt->startTime = val.i;
			break;
		case bmtex_playbackrate:
			bmt->pbRate= val.f;
			break;
		case bmtex_endcondition:
			bmt->endCond = val.i;
			break;	
		case bmtex_matidtime:
			bmt->FreeBitmap();
			break;
		}
	}
};

static BMTexPBTimeAccessor bmtextime_accessor;

// aszabo|Oct.30.01 - 312037 UI for Time rollout should be built. 
// Fixing incorrect removal of UI dates back to R3
#ifdef NO_MTLEDITOR_BITMAP_TIME_ROLLOUT
#define TIME_PB_FLAGS P_AUTO_CONSTRUCT
#else	// !NO_MTLEDITOR_BITMAP_TIME_ROLLOUT
#define TIME_PB_FLAGS P_AUTO_CONSTRUCT + P_AUTO_UI
#endif	// !NO_MTLEDITOR_BITMAP_TIME_ROLLOUT

static ParamBlockDesc2 bmtex_time_param_blk ( bmtex_time, _T("time parameters"),  0, &bmTexCD, TIME_PB_FLAGS, PBLOCKTIME_REF, 

#ifndef NO_MTLEDITOR_BITMAP_TIME_ROLLOUT
    //rollout
	IDD_BMTEX_TIME, IDS_DS_TIME_PARAMS, 0, APPENDROLL_CLOSED, NULL, 
#endif	// !NO_MTLEDITOR_BITMAP_TIME_ROLLOUT

	// params
	bmtex_start, _T("startTime"), TYPE_TIMEVALUE,			0,	IDS_PW_STARTTIME,
		p_default,		0,
		p_range,		-1000000,	1000000,
		p_ui, 			TYPE_SPINNER, EDITTYPE_INT,  IDC_BMTEX_START, IDC_BMTEX_START_SPIN, 1.0f,
		p_accessor,		&bmtextime_accessor,
		end,
	bmtex_playbackrate, _T("playBackRate"), TYPE_FLOAT,			0,	IDS_PW_PLAYBACK,
		p_default,		1.0f,
		p_range,		0.0f,	10000.0f,
		p_ui, 			TYPE_SPINNER, EDITTYPE_FLOAT,  IDC_BMTEX_RATE, IDC_BMTEX_RATE_SPIN, 1.0f,
		p_accessor,		&bmtextime_accessor,
		end,
	bmtex_endcondition, _T("endCondition"), TYPE_INT,		0,		IDS_PW_ENDCONDITION,
		p_default,		0,
		p_range,		0,	2,
		p_ui,			TYPE_RADIO, 3, IDC_BMTEX_LOOP, IDC_BMTEX_PINGPONG,IDC_BMTEX_HOLD,
		p_accessor,		&bmtextime_accessor,
		end,

	bmtex_matidtime, _T("tieTimeToMatIDs"), TYPE_BOOL,		0,		IDS_PW_MATIDTIME,
		p_default,		FALSE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_MATIDTIME_CHECK,
		p_accessor,		&bmtextime_accessor,
		end,


	end
	);


//Function Publishing descriptor for Mixin interface
//*****************************************************
static FPInterfaceDesc bitmaptex_interface(
	BITMAPTEX_INTERFACE, _T("bitmapTex"), 0, &bmTexCD, FP_MIXIN,
	bitmaptex_reload, _T("reload"), 0, TYPE_VOID, 0, 0,
	bitmaptex_crop, _T("viewImage"), 0, TYPE_VOID, 0, 0,
	end
	);


void *	BMTexClassDesc::Create(BOOL loading)
{ 	
	AddInterface(&bitmaptex_interface);
	return new BMTex; 
}


FPInterfaceDesc* BitmapTex::GetDesc()
{
	return &bitmaptex_interface;
}



void BMTex::SetEndCondition(int endcond) { 
	pblockTime->SetValue( bmtex_endcondition, 0, endcond);		
	endCond = endcond; 
	//	ParamID changedParam = pblockTime->LastNotifyParamID();
	//	bmtex_time_param_blk.InvalidateUI(changedParam);
}
void BMTex::SetStartTime(TimeValue t) {
	pblockTime->SetValue( bmtex_start, 0, t);		
	startTime = t; 
	//	ParamID changedParam = pblockTime->LastNotifyParamID();
	//	bmtex_time_param_blk.InvalidateUI(changedParam);
}
void BMTex::SetPlaybackRate(float r) { 
	pblockTime->SetValue( bmtex_playbackrate, 0, r);		
	pbRate = r; 
	//	ParamID changedParam = pblockTime->LastNotifyParamID();
	//	bmtex_time_param_blk.InvalidateUI(changedParam);
}


//------------------------------------------------------------------------
// BMTexDlg -- Methods
//------------------------------------------------------------------------
BMTexDlg::BMTexDlg(HWND hwMtlEdit, IMtlParams *imp, BMTex *m) 
: m_displayBitmap(NULL)
{ 
	hwmedit = hwMtlEdit;
	ip = imp;
	hPanel = NULL;
	theBMTex = m; 
	valid  = FALSE;
	isActive  = FALSE;
	cropping = FALSE;
	curTime = imp->GetTime();

	//	hTime = 0;
	curTime = 0;
}

void BMTexDlg::ReloadDialog() {
	Interval valid;
	theBMTex->Update(curTime, valid);
	LoadDialog(FALSE);
}

void BMTexDlg::Update(TimeValue t) {
	Interval valid;
	curTime = t;
	if (!theBMTex->ivalid.InInterval(t)) {
		theBMTex->Update(curTime, valid);
#ifndef DESIGN_VER
		LoadDialog(FALSE);
		InvalidateRect(hPanel,NULL,0);
		if (cropping) {
			theBMTex->StuffCropValues();
		}
#endif // !DESIGN_VER
	}
}


BMTexDlg::~BMTexDlg() {
	RemoveCropImage();
	theBMTex->paramDlg = NULL;
	DLSetWindowLongPtr(hPanel, NULL);
	hPanel =  NULL;

	// This should have been deleted, or else there'll be a memory leak.
	DbgAssert(m_displayBitmap == NULL);	
}

void BMTex::FreeBitmap() {

	m_theBitmap.FreeBitmap();

	for(int i = 0; i < m_theBitmapList.length(); ++i) {
		BitmapHolder& bitmapHolder = m_theBitmapList[i];
		bitmapHolder.FreeBitmap();
	}

	if (texHandle) {
		texHandle->DeleteThis();
		texHandle = NULL;	
	}
	loadFailed = FALSE;
}


void BMTexDlg::BMNameChanged() {
	theBMTex->FreeBitmap();
	theBMTex->NotifyChanged();
	StuffBMNameField(hPanel);
	theBMTex->loadFailed = FALSE;
	BMMRES res = theBMTex->LoadBitmap(ip->GetTime());
	if (res==BMMRES_NODRIVER) 
		InvalidNameMsg(theBMTex->m_bi.Name());
	EnableAlphaButtons(TRUE);
	UpdateMtlDisplay();
}

BOOL BMTexDlg::KeyAtCurTime(int id) { return theBMTex->pblock->KeyFrameAtTime(id,curTime); }

void GetBMName(BitmapInfo& bi, TSTR &fname, BOOL full=FALSE) { 
	TSTR fullName;
	if (bi.Name()[0]==0)
		fullName = bi.Device();
	else 
		fullName =  bi.Name();
	if (full)
		fname = fullName;
	else 
		SplitPathFile(fullName,NULL,&fname);
}

void BMTexDlg::StuffBMNameField(HWND hwndDlg) {

	TSTR fname;
	//	GetBMName(theBMTex->bi,fname,TRUE);
	//	if (iName) {
	//		iName->SetText(fname.data());
	//		iName->SetTooltip(TRUE,fname.data());
	//		}
}

void BMTexDlg::ShowCropImage() {
	if (!theBMTex) return;

	// Disable proxy  mode as we don't want to display a proxy image in the viewer.
	BMTex::ProxyModeDisableGuard proxyModeDisabler(*theBMTex);

	Bitmap *bm = theBMTex->GetActiveBitmap();
	if (!bm) { 
		BMMRES res = theBMTex->LoadBitmap(ip->GetTime());
		bm = theBMTex->GetActiveBitmap();
		if (!bm) return;
	}

	// Create a new instance of the bitmap (pointing to the same storage), in order to ensure it's not
	// deleted while the bitmap is being displayed, when the non-proxy is flushed as
	// proxies are re-enabled. This fixes bug #787907.
	if(m_displayBitmap != NULL) {
		m_displayBitmap->DeleteThis();
		m_displayBitmap = NULL;
	}
	m_displayBitmap = TheManager->NewBitmap();
	if(m_displayBitmap != NULL) {
		m_displayBitmap->SetStorage(bm->Storage());
		m_displayBitmap->SetNotify(this);
		CropCallback* cropCallback = NULL;

#ifndef NO_MTLEDITOR_BITMAP_CROPPING
		cropper.Init(this,curTime);
		cropCallback = &cropper;
#endif	// !NO_MTLEDITOR_BITMAP_CROPPING

		cropping = TRUE;
		// Create an autonomous bitmap to have it deleted automatically when the viewer is closed.
		int junk = m_displayBitmap->Display( GetString(IDS_DS_CROP_TITLE),BMM_CN, TRUE, FALSE, cropCallback);
	}
	else {
		DbgAssert(false);
	}
}

void BMTexDlg::RemoveCropImage() {
	cropping = FALSE;

	// Delete the temporary bitmap that was created for the viewer.
	if(m_displayBitmap != NULL) {
		// UnDisplay() closes the viewer, but disables the "autonomous" mode on bitmaps, so we still
		// need to delete the bitmap, even if it was declared as 'autonomous' when displayed.
		// Need to use a copy of the bitmap pointer, since m_displayBitmap is null'ed in VFBClosed().
		Bitmap* displayBM = m_displayBitmap;
		displayBM->UnDisplay();
		displayBM->DeleteThis();
		m_displayBitmap = NULL;
	}
}

int BMTexDlg::Changed(ULONG flags) {

	// Do nothing

	return 1;
}

void BMTexDlg::VFBClosed() {

	// VFB was closed; the image will be deleted automatically because it was displayed
	// as "autonomous".
	m_displayBitmap = NULL;
}

static int colID[2] = { IDC_CHECK_COL1, IDC_CHECK_COL2 };

void BMTexDlg::EnableAlphaButtons(BOOL isNew) {
	if (hPanel==NULL) return;
	if (theBMTex->m_bi.Flags()&MAP_HAS_ALPHA) {
		EnableWindow(GetDlgItem(hPanel,IDC_ALPHA_FILE), 1);
		EnableWindow(GetDlgItem(hPanel,IDC_ALPHA_PREMULT), 1);
		EnableWindow(GetDlgItem(hPanel,IDC_BMTEX_ALPHAOUT), 1);
		EnableWindow(GetDlgItem(hPanel,IDC_BMTEX_ALPHAOUT2), 1);
		if (isNew) {
			theBMTex->SetAlphaSource(ALPHA_FILE);
		}
	}
	else {
		EnableWindow(GetDlgItem(hPanel,IDC_ALPHA_FILE), 0);
		EnableWindow(GetDlgItem(hPanel,IDC_ALPHA_PREMULT), 0);
		EnableWindow(GetDlgItem(hPanel,IDC_BMTEX_ALPHAOUT), 0);
		EnableWindow(GetDlgItem(hPanel,IDC_BMTEX_ALPHAOUT2), 0);
		if (theBMTex->alphaSource == ALPHA_FILE)
			theBMTex->SetAlphaSource(ALPHA_NONE);
		if (theBMTex->premultAlpha)
			theBMTex->SetPremultAlpha(TRUE);
		if (isNew) {
			if (theBMTex->alphaAsRGB) 
				theBMTex->SetAlphaAsRGB(FALSE);
			if (theBMTex->alphaAsMono) 
				theBMTex->SetAlphaAsMono(FALSE);
		}
	}
}


void BMTexDlg::EnableViewImage() {
	EnableWindow(GetDlgItem(hPanel,IDC_BM_CROP_IMAGE),(_tcslen(theBMTex->m_bi.Name())>0)?1:0); 	
}

INT_PTR BMTexDlg::DlgProc(TimeValue t,IParamMap2 *map,HWND hwndDlg,UINT msg,WPARAM wParam,LPARAM lParam)    
{	
	int id = LOWORD(wParam);
	int code = HIWORD(wParam);
	switch (msg)    {
	case WM_INITDIALOG:
		{
			hPanel = map->GetHWnd();
			EnableAlphaButtons();
			EnableViewImage();
			return TRUE;
		}
		break;
	case WM_COMMAND:  
		switch (id) {
		case IDC_BMTEX_NAME: 
			{
				bmtex_param_blk.InvalidateUI(bmtex_bitmap);

				if(HIWORD(wParam) == BN_CLICKED) {
					// Class ParamMap2UserDlgProc is called _after_ the default processing is complete,
					// meaning that the param block system has already recorded the new name for the bitmap.
					if((map != NULL) && (map->GetParamBlock() != NULL)) {
						ReferenceMaker* pbOwner = map->GetParamBlock()->GetOwner();
						if((pbOwner != NULL) && (pbOwner->ClassID() == bmTexClassID)) {
							BMTex* bmTex = static_cast<BMTex*>(pbOwner);
							bmTex->ShowBitmapProxyPrecacheDialog();
						}
					}
				}
			}
			break;

		case IDC_ALPHA_FILE:
		case IDC_ALPHA_XPCOL:
		case IDC_ALPHA_RGB:
		case IDC_ALPHA_NONE:
			theBMTex->NotifyChanged();
			theBMTex->DiscardTexHandle();
			UpdateMtlDisplay();
			break;
		case IDC_FILTER_NADA:
		case IDC_FILTER_PYR:
		case IDC_FILTER_SAT:
			theBMTex->NotifyChanged();
			break;
		case IDC_BMTEX_RELOAD:
			theBMTex->ReloadBitmap(true);
			BroadcastNotification(NOTIFY_BITMAP_CHANGED, (void *)theBMTex->m_bi.Name());
			//theBMTex->NotifyChanged();
			UpdateMtlDisplay(); // redraw viewports
			Invalidate();  // incase path of map has changed.
			macroRec->OperandSequence(1, mr_prop, _T("bitmaptex.Reload()"),mr_reftarg, theBMTex);

			break;
		case IDC_BMTEX_RGBOUT:
		case IDC_BMTEX_ALPHAOUT:
			theBMTex->NotifyChanged();
			break;
		case IDC_BMTEX_RGBOUT2:
		case IDC_BMTEX_ALPHAOUT2:
			theBMTex->NotifyChanged();
			theBMTex->DiscardTexHandle();
			UpdateMtlDisplay();
			break;
		case IDC_ALPHA_PREMULT:
			theBMTex->NotifyChanged();
			theBMTex->DiscardTexHandle();
			UpdateMtlDisplay();
			break;
		case IDC_BM_CLIP: 
			//					theBMTex->applyCrop = GetCheckBox(hwndDlg,id);
			theBMTex->NotifyChanged();
			theBMTex->DiscardTexHandle();
			UpdateMtlDisplay();
			break;
		case IDC_BM_CROP_IMAGE: 
			ShowCropImage();
			macroRec->OperandSequence(1, mr_prop, _T("bitmaptex.ViewImage()"),mr_reftarg, theBMTex);

			//					macroRecorder->FunctionCall(_T("$.modifiers[#flex].quality"), 0, 0);

			break;
		case IDC_BM_CROP:
		case IDC_BM_PLACE:
			//					CheckRadioButton( hwndDlg, IDC_BM_CROP, IDC_BM_PLACE, id);
			//					theBMTex->placeImage = id==IDC_BM_PLACE?1:0;
			theBMTex->NotifyChanged();
			theBMTex->DiscardTexHandle();
			UpdateMtlDisplay();
			if (cropping) theBMTex->StuffCropValues();
			theBMTex->EnableStuff();
			break;
		case IDC_BM_JITTER:
			//					theBMTex->randPlace =  GetCheckBox(hwndDlg,id);
			theBMTex->NotifyChanged();
			UpdateMtlDisplay();
			theBMTex->EnableStuff();
			break;
		}
		break;

	case WM_PAINT: 	
		if (!valid) {
			valid = TRUE;
			ReloadDialog();
		}
		break;

	case CC_SPINNER_CHANGE: 
		if (!theHold.Holding()) theHold.Begin();
		//			switch (id) {
		theBMTex->NotifyChanged();
		if (cropping) theBMTex->StuffCropValues();
		//need to make as an accesot
		/*
		case IDC_CLIP_XSPIN: {
		float u = 0.0f;//clipUSpin->GetFVal();
		theBMTex->SetClipU(curTime,u);
		//					clipUSpin->SetKeyBrackets(KeyAtCurTime(PB_CLIPU));
		float w = theBMTex->GetClipW(curTime);
		if (u+w>1.0f) {
		w = 1.0f-u;
		theBMTex->SetClipW(curTime,w);
		//						clipWSpin->SetValue(theBMTex->clipw,FALSE);
		//						clipWSpin->SetKeyBrackets(KeyAtCurTime(PB_CLIPW));
		}
		theBMTex->NotifyChanged();
		if (cropping) theBMTex->StuffCropValues();
		}
		break; 	
		case IDC_CLIP_YSPIN: {
		float v = 0.0f;//clipVSpin->GetFVal();
		theBMTex->SetClipV(curTime,v);
		//					clipVSpin->SetKeyBrackets(KeyAtCurTime(PB_CLIPV));
		float h = theBMTex->GetClipH(curTime);
		if (v+h>1.0f) {
		h = 1.0f-v;
		theBMTex->SetClipH(curTime,h);
		//						clipHSpin->SetValue(theBMTex->cliph,FALSE);
		//						clipHSpin->SetKeyBrackets(KeyAtCurTime(PB_CLIPH));
		}
		theBMTex->NotifyChanged();
		if (cropping) theBMTex->StuffCropValues();
		}
		break; 	
		case IDC_CLIP_WSPIN: {
		float w = 0.0f;//clipWSpin->GetFVal();
		theBMTex->SetClipW(curTime,w);
		//					clipWSpin->SetKeyBrackets(KeyAtCurTime(PB_CLIPW));
		float u = theBMTex->GetClipU(curTime);
		if (u+w>1.0f) {
		u = 1.0f-w;
		theBMTex->SetClipU(curTime,u);
		//						clipUSpin->SetValue(theBMTex->clipu,FALSE);
		//						clipUSpin->SetKeyBrackets(KeyAtCurTime(PB_CLIPU));
		}
		theBMTex->NotifyChanged();
		if (cropping) theBMTex->StuffCropValues();
		}
		break; 	
		case IDC_CLIP_HSPIN: {
		float h = 0.0f;//clipHSpin->GetFVal();
		theBMTex->SetClipH(curTime,h);
		//					clipHSpin->SetKeyBrackets(KeyAtCurTime(PB_CLIPH));
		float v = theBMTex->GetClipV(curTime);
		if (v+h>1.0f) {
		v = 1.0f-h;
		theBMTex->SetClipV(curTime,v);
		//						clipVSpin->SetValue(theBMTex->clipv,FALSE);
		//						clipVSpin->SetKeyBrackets(KeyAtCurTime(PB_CLIPV));
		}
		theBMTex->NotifyChanged();
		if (cropping) theBMTex->StuffCropValues();
		}
		break;
		case IDC_JITTER_SPIN:
		//					theBMTex->SetJitter(curTime,jitterSpin->GetFVal());
		//					jitterSpin->SetKeyBrackets(KeyAtCurTime(PB_JITTER));
		theBMTex->NotifyChanged();
		break;
		*/

		//				}
		break;
	case CC_SPINNER_BUTTONDOWN:
		theHold.Begin();
		break;		
	case WM_CUSTEDIT_ENTER:
	case CC_SPINNER_BUTTONUP: 
		if (HIWORD(wParam) || msg==WM_CUSTEDIT_ENTER) theHold.Accept(GetString(IDS_DS_PARAMCHG));
		else theHold.Cancel();
		theBMTex->DiscardTexHandle();
		theBMTex->NotifyChanged();
		UpdateMtlDisplay();
		if (cropping) theBMTex->StuffCropValues();
		break;
	case WM_DESTROY:		
		RemoveCropImage();
		break;
	}
	return FALSE;
}



void BMTexDlg::LoadDialog(BOOL draw) {
	if (theBMTex) {
		Interval valid;
		theBMTex->Update(curTime,valid);
		StuffBMNameField(hPanel);
		CheckRadioButton( hPanel, IDC_ALPHA_FILE, IDC_ALPHA_NONE, IDC_ALPHA_FILE+theBMTex->alphaSource);
		CheckRadioButton( hPanel, IDC_FILTER_PYR, IDC_FILTER_NADA, IDC_FILTER_PYR+theBMTex->filterType);
		CheckRadioButton( hPanel, IDC_BMTEX_RGBOUT, IDC_BMTEX_ALPHAOUT, IDC_BMTEX_RGBOUT+theBMTex->alphaAsMono);
		CheckRadioButton( hPanel, IDC_BMTEX_RGBOUT2, IDC_BMTEX_ALPHAOUT2, IDC_BMTEX_RGBOUT2+theBMTex->alphaAsRGB);
		SetCheckBox( hPanel, IDC_ALPHA_PREMULT, theBMTex->premultAlpha);
		SetCheckBox( hPanel, IDC_BM_CLIP, theBMTex->applyCrop);
		EnableAlphaButtons(FALSE);
	}
}

Class_ID BMTexDlg::ClassID() {

	return bmTexClassID;  
}

void BMTexDlg::SetThing(ReferenceTarget *m) {
	assert (m->ClassID()==bmTexClassID);
	assert (m->SuperClassID()==TEXMAP_CLASS_ID);
	RemoveCropImage();
	if (theBMTex) {
		theBMTex->paramDlg = NULL;
	}
	theBMTex = (BMTex *)m;
	if (theBMTex) theBMTex->paramDlg = this;
	LoadDialog(TRUE);
}

//------------------------------------------------------------------------
// BMTex -- Methods
//------------------------------------------------------------------------



#define BMTEX_VERSION 3

static ParamBlockDescID pbdesc1[] = {
	{ TYPE_FLOAT, NULL, TRUE,1} 	// blur
};   

static ParamBlockDescID pbdesc2[] = {
	{ TYPE_FLOAT, NULL, TRUE,bmtex_clipu }, // clipu
	{ TYPE_FLOAT, NULL, TRUE,bmtex_clipv }, // clipv
	{ TYPE_FLOAT, NULL, TRUE,bmtex_clipw }, // clipw
	{ TYPE_FLOAT, NULL, TRUE,bmtex_cliph }, // cliph
	{ TYPE_FLOAT, NULL, TRUE,bmtex_jitter } // jitter
};

static ParamVersionDesc oldVersions[] = {
	ParamVersionDesc(pbdesc1, 1, 0),
	ParamVersionDesc(pbdesc1, 1, 1),
	ParamVersionDesc(pbdesc2, 4, 2),
	ParamVersionDesc(pbdesc2, 5, 3)
};
/*
static ParamVersionDesc oldVersions[] = {
ParamVersionDesc(pbdesc2, 5, 2)
};
*/
#define NUMOLDVERSIONS 4
//static ParamVersionDesc curVersion(pbdesc2,5,BMTEX_VERSION);

//static int name_id[NPARAMS] = { IDS_DS_CLIPU, IDS_DS_CLIPV, IDS_DS_CLIPW, IDS_DS_CLIPH, IDS_DS_JITTERAMT };

void BMTex::Init() {
	if (uvGen) uvGen->Reset();
	else ReplaceReference( 0, GetNewDefaultUVGen());	
	//	ReplaceReference( 1, CreateParameterBlock( pbdesc2, NPARAMS, BMTEX_VERSION) );	
	if (texout) texout->Reset();
	else ReplaceReference( 2, GetNewDefaultTextureOutput());
	placeImage = FALSE;
	randPlace = FALSE;
	filterType = FILTER_PYR;
	alphaSource = ALPHA_FILE;
	alphaAsMono = FALSE;
	alphaAsRGB = FALSE;
	premultAlpha = TRUE;
	SetClipU(0,0.0f);
	SetClipV(0,0.0f);
	SetClipW(0,1.0f);
	SetClipH(0,1.0f);
	SetJitter(0,1.0f);
	ivalid.SetEmpty();
	clipValid.SetInfinite();
	mTieTimeToMatID = FALSE; // LAM - 4/21/03
	isNew = FALSE;
	rumax = rvmax = -100000.0f;
	rumin = rvmin = +100000.0f;
}

void BMTex::Reset() {
	bmTexCD.Reset(this, TRUE);	// reset all pb2's
	if (!isNew) {
		FreeBitmap();
		NotifyChanged();
		if (paramDlg) paramDlg->UpdateMtlDisplay();
	}
	Init();
}

void BMTex::NotifyChanged() {
	NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
}

#ifdef DESIGN_VER
void BMTex::SetGeoReferenceInfo(BitmapInfo & bi, StdUVGen * uvGen)
{
	TheManager->Execute(BMM_RETRIEVE_GEOREF_DATA,
		ULONG_PTR(bi.Name()), ULONG_PTR(&(uvGen->m_geoRefInfo)), 0);
}
#endif

void BMTex::Notify(NotifyInfo* info) {

	switch(info->intcode) {
		case NOTIFY_BITMAP_CHANGED: 
			{
				TCHAR *nm = (TCHAR *)info->callParam;
				if (nm==NULL || !_tcscmp(nm,m_bi.Name())) {
					NotifyChanged();
				}
				if (nm==NULL) {
					fileNotFound = 0;
				}
#ifdef DESIGN_VER
				if (!_tcscmp(_T("TIF Image File"), m_bi.Device()))
					SetGeoReferenceInfo(m_bi, GetUVGen());
#endif
				break;
			}
		case NOTIFY_PROXY_TEMPORARY_DISABLE_START:
			// Keep track of the number of temporary disables that occur
			++m_numTemporaryDisables;
			if(m_numTemporaryDisables == 1) {
				DisableProxyMode(false);
			}
			break;
		case NOTIFY_PROXY_TEMPORARY_DISABLE_END:
			--m_numTemporaryDisables;
			if(m_numTemporaryDisables == 0) {
				EnableProxyMode(!KeepNonProxyInMemoryAfterRendering());
			}
			break;
		default:
			DbgAssert(false);
			break;
	}
}

void BMTex::BMTexNotify(void *param, NotifyInfo *info) {

	BMTex *bmt = (BMTex *) param;
	if(bmt != NULL) {
		bmt->Notify(info);
	}
	else {
		DbgAssert(false);
	}
}

BMTex::BMTex() 
: m_bi(m_theBitmap.GetBitmapInfo()),
  m_numTemporaryDisables(0),
  m_proxiesWereDisabledForRendering(false)
{
	ip = NULL;
	isParm2 = TRUE;
	paramDlg = NULL;
	pblock = NULL;
	pblockTime = NULL;
	uvGen = NULL;
	texout = NULL;
	texHandle = NULL;
	loadFailed = FALSE;
	startTime = 0;
	pbRate = 1.0f;
	endCond = END_LOOP;
	isNew = TRUE;
	applyCrop = FALSE;
	loading = FALSE;
	loadingOld = FALSE;
	clipValid = FOREVER;
	fileNotFound = FALSE;
	bmTexCD.MakeAutoParamBlocks(this);	// make and intialize paramblock2
	Init();
	bmNotify.SetTex(this);
	rollScroll=0;
	bmWasSet = FALSE;
	RegisterNotification(BMTexNotify, this, NOTIFY_BITMAP_CHANGED);
	RegisterNotification(BMTexNotify, this, NOTIFY_PROXY_TEMPORARY_DISABLE_START);
	RegisterNotification(BMTexNotify, this, NOTIFY_PROXY_TEMPORARY_DISABLE_END);
}

Bitmap* BMTex::GetActiveBitmap() const {

	// We assume that the m_theBitmap is non-null if and only if
	// "tie time to mat ID" is false.
	Bitmap* bitmap = m_theBitmap.GetBitmap();
	if((bitmap == NULL) && !m_theBitmapList.isEmpty()) {
		// Use the last bitmap in the list
		bitmap = m_theBitmapList.last().GetBitmap();
	}

	return bitmap;
}

void BMTex::SetClipU(TimeValue t, float f) { 
	clipu  = f; 
	pblock->SetValue(bmtex_clipu, t, f);
	bmtex_param_blk.InvalidateUI(bmtex_clipu);

}


void BMTex::SetClipV(TimeValue t, float f) { 
	clipv  = f; 
	pblock->SetValue( bmtex_clipv, t, f);		
	bmtex_param_blk.InvalidateUI(bmtex_clipv);
}

void BMTex::SetClipW(TimeValue t, float f) { 
	clipw  = f;
	//	  pblock->SetValue( PB_CLIPW, t, f);		
	pblock->SetValue( bmtex_clipw, t, f);		
	bmtex_param_blk.InvalidateUI(bmtex_clipw);
}

void BMTex::SetClipH(TimeValue t, float f) { 
	cliph  = f; 
	//	pblock->SetValue( PB_CLIPH, t, f);		
	pblock->SetValue( bmtex_cliph, t, f);		
	bmtex_param_blk.InvalidateUI(bmtex_cliph);
}

void BMTex::SetJitter(TimeValue t, float f) { 
	jitter = cliph  = f; 
	//	pblock->SetValue( PB_JITTER, t, f);		
	pblock->SetValue( bmtex_jitter, t, f);	
	bmtex_param_blk.InvalidateUI(bmtex_jitter);
}

Bitmap* BMTex::GetBitmap(TimeValue t) { 

	LoadBitmap(t); 	
	return GetActiveBitmap(); 
}

void BMTex::EnableStuff() {
	if (pblock) {
		IParamMap2 *map = pblock->GetMap();
		pblock->GetValue( bmtex_crop_place, 0, placeImage, FOREVER );
		pblock->GetValue( bmtex_usejitter, 0, randPlace, FOREVER );
		if (map) {
			map->Enable(bmtex_jitter, placeImage);
			map->Enable(bmtex_usejitter, placeImage);
			map->Enable(bmtex_clipu, !(placeImage&&randPlace));
			map->Enable(bmtex_clipv, !(placeImage&&randPlace));
		}
	}
}

void BMTex::SetFilterType(int ft) {

	pblock->SetValue( bmtex_filtering, 0, ft);		
	bmtex_param_blk.InvalidateUI(bmtex_filtering);
}

void BMTex::SetAlphaSource(int as) {
	if (as!=alphaSource) {
		alphaSource =as; 

		InitBitmapSamplers();

		if (alphaSource == ALPHA_FILE) pblock->SetValue( bmtex_alphasource, 0, 0);		
		else if (alphaSource == ALPHA_RGB) pblock->SetValue( bmtex_alphasource, 0, 1);		
		else if (alphaSource == ALPHA_NONE) pblock->SetValue( bmtex_alphasource, 0, 2);		
		bmtex_param_blk.InvalidateUI(bmtex_alphasource);
	}
}

void BMTex::SetMapName(const TCHAR *name, bool isUIAction) { 

	if (name) {
		TCHAR uncname[MAX_PATH];
		if (BMMGetUniversalName(uncname,name))
			m_bi.SetName(uncname); 
		else
			m_bi.SetName(name); 

		PBBitmap bt(m_bi);
		pblock->SetValue(bmtex_bitmap, 0, &bt);
		fileNotFound = FALSE;
		if (TestMtlFlag(MTL_TEX_DISPLAY_ENABLED)) {
			if (ip)
				ip->MtlChanged();
		}

		// Trigger the bitmap proxy pre-caching dialog if the reload occured
		// as the result of a UI command.
		if(isUIAction) {
			ShowBitmapProxyPrecacheDialog();
		}
	}
}

static TimeValue modt(TimeValue x, TimeValue m) {
	TimeValue n = (int)(x/m);
	x -= n*m;
	return (x<0)? x+m : x ;
}

void BMTex::StuffCropValues() {
	pblock->GetValue( bmtex_crop_place, 0, placeImage, FOREVER );

	m_theBitmap.SetCroppingValues(clipu,clipv,clipw,cliph, placeImage );
}

int BMTex::CalcFrame(TimeValue t) {
	TimeValue tm,dur,td;
	int fstart = m_bi.FirstFrame();
	int fend = m_bi.LastFrame();
	int tpf = GetTicksPerFrame();
	tm = TimeValue(float(t-startTime)*pbRate);
	dur = (fend-fstart+1)*GetTicksPerFrame();
	switch (endCond) {
		case END_HOLD:
			if (tm<=0) return fstart;
			if (tm>=dur) return fend;
			return tm/tpf;
		case END_PINGPONG:
			//if ((tm/dur)&1) 
			if ( ((tm>=0) && ((tm/dur)&1)) || ( (tm<0) && !(tm/dur)))
			{
				td = modt(tm,dur);
				return fstart + fend-td/tpf;
			}
			// else fall thrue--
		case END_LOOP:
			td = modt(tm,dur);
			return td/tpf;
	}
	return 0;
}

BMMRES BMTex::LoadBitmap(TimeValue t, bool quiet, bool reload) {
	BOOL silent = FALSE;
	BMMRES status = BMMRES_SUCCESS;

	if (bmWasSet) 
		goto skipall;

	PBBitmap* bitmapPB = NULL;
	pblock->GetValue( bmtex_bitmap, t,bitmapPB, clipValid );
	if ( ((bitmapPB != NULL) && (_tcscmp(m_bi.Name(), bitmapPB->bi.Name())) )   )
		FreeBitmap();

	UpdateBIName();   //DS 2/18/99
	if (m_bi.Name()[0]==0) {
		if (m_bi.Device()[0]==0)
			return BMMRES_NODRIVER;
#ifdef DESIGN_VER //KENNY MERGE
		//SS 10/3/2000: In the Kenny source, this line was indented an additional 
		// level, as if the intention was to include it within the outer if 
		// statement. Since that wasn't the case, I have adjusted indentation.
		// However, if you find a bug here, it could be that this should only be
		// called if bi.Name()[0]==0.
		// Additionally, I don't know if this is a general fix or is specific to
		// VIZ; there were no previous comments.

		// [d.levesque | 10feb2002] I have include this line of code in the if block, 
		// as suggested by Stew's comment. This fixes:
		//   313220 - GAMMA STATE OF IMAGE REVERTS TO USE SYSTEM GAMMA, DOES NOT REMEMBER STATE SET.
		m_bi.ResetCustomFlag(BMM_CUSTOM_FILEGAMMA);
#endif
	}

	BOOL tieTimeToMatID;
	pblockTime->GetValue( bmtex_matidtime, 0, tieTimeToMatID,FOREVER);

	if (GetActiveBitmap()==NULL)
	{
		if (!tieTimeToMatID)
		{
			// Load bitmap
			int bitmapFrame = CalcFrame(t);
			m_bi.SetCurrentFrame(bitmapFrame);
			if(!m_theBitmap.LoadBitmap(status, quiet)) {
				fileNotFound = TRUE;
				return status;
			}
			if (bitmapPB) {
				bitmapPB->bi = m_bi;	  //DS 3/29/99: in case name has changed ( or flags)
			}
			fileNotFound = FALSE; 
			m_theBitmap.SetNotify(&bmNotify);
			m_theBitmap.SetFilter(bmFilterType(filterType));
			// fixup for imported files
#ifdef RENDER_VER
			{
				// Automatically set the alpha settings depending on whether
				// the file has an alpha channel or not. .PSD files have
				// alpha channels, but we are supposed to ignore them.
				LPCTSTR d = m_bi.Name();
				size_t len = _tcslen(d);
				if (len > 4)
					d += len - 4;
				if ((m_bi.Flags() & MAP_HAS_ALPHA) && _tcsicmp(d, ".psd") != 0) {
					SetAlphaAsMono(true);
					SetAlphaSource(ALPHA_FILE);
				}
				else {
					SetAlphaAsMono(false);
					SetAlphaSource(ALPHA_NONE);
				}
			}
#endif

		}

		else
		{
			for (int ct = 0; ct < mysampList.Count(); ct++)
			{
				delete mysampList[ct];
				delete alphasampList[ct];
			}

			int numberFrames = m_bi.NumberFrames();
			m_theBitmapList.setLengthUsed(numberFrames);
			mysampList.SetCount(numberFrames);
			alphasampList.SetCount(numberFrames);
			for (int ct = 0; ct < numberFrames; ct++)
			{
				mysampList[ct] = new BMSampler(); 
				alphasampList[ct] = new BMAlphaSampler();
			}

			int firstFrame = m_bi.FirstFrame();
			int lastFrame = m_bi.LastFrame();


			for (int frameIndex = m_bi.FirstFrame(), ct=0; frameIndex <= m_bi.LastFrame(); frameIndex++)
			{
				if (frameIndex >= numberFrames) { // extend the arrays
					BitmapHolder nullBitmap;
					BMSampler* bmSampler = NULL;
					BMAlphaSampler* bmAlphaSampler = NULL;  
					for(int j=numberFrames; j<=frameIndex; j++) {
						m_theBitmapList.append(nullBitmap);
						bmSampler = new BMSampler();
						mysampList.Append(1, &bmSampler);
						bmAlphaSampler = new BMAlphaSampler();
						alphasampList.Append(1, &bmAlphaSampler);
					}
					numberFrames = frameIndex+1;
				}

				// Load bitmap, with proxy support
				BitmapHolder& currentBitmap = m_theBitmapList[ct];
				// Copy the main bitmap info as it's where our bitmap info lies.
				currentBitmap.GetBitmapInfo() = m_bi;
				currentBitmap.GetBitmapInfo().SetCurrentFrame(frameIndex);
				if(!currentBitmap.LoadBitmap(status, quiet)) {
					fileNotFound = TRUE;
					return status;
				}
				if (bitmapPB) {
					bitmapPB->bi = m_bi;	  //DS 3/29/99: in case name has changed ( or flags)
				}
				fileNotFound = FALSE; 
				m_theBitmap.SetNotify(&bmNotify);
				m_theBitmap.SetFilter(bmFilterType(filterType));

				m_theBitmapList[ct].SetNotify(&bmNotify);
				m_theBitmapList[ct].SetFilter(bmFilterType(filterType));

				ct++;

			}
			// fixup for imported files
			if (!(m_bi.Flags()&MAP_HAS_ALPHA)&&alphaSource==ALPHA_FILE) {
				if (paramDlg) 
					paramDlg->EnableAlphaButtons(FALSE);
				else 
					SetAlphaSource(ALPHA_NONE);
			}
			if (!(m_bi.Flags()&MAP_HAS_ALPHA))			
				SetPremultAlpha(TRUE);   // DS 6/9/00
			if (paramDlg != NULL) {
				paramDlg->EnableAlphaButtons(!reload);
				paramDlg->EnableViewImage();
			}
		}
	}

	if (!tieTimeToMatID) {
		m_bi.SetCurrentFrame(CalcFrame(t));	

		// [dl | 11june2003] Commit the bitmap info to the param block, in case the frame changed
		if(bitmapPB != NULL) {
			bitmapPB->bi = m_bi;
		}

		// Update, with proxy support
		status = m_theBitmap.GotoFrame(true);
	}

#ifdef DESIGN_VER
	if (!_tcscmp(_T("TIF Image File"), m_bi.Device()))
		SetGeoReferenceInfo(m_bi, GetUVGen());
#endif
	//		}

skipall:

	InitBitmapSamplers();

	return status;
}

int BMTex::RenderBegin(TimeValue t, ULONG flags) {

	// Disable proxy for rendering, if necessary

	DbgAssert(!m_proxiesWereDisabledForRendering);

	// Don't disable the proxies if they are already temporarily disabled
	if(m_numTemporaryDisables == 0) {
		bool isMEdit = ((flags & RENDERBEGIN_IN_MEDIT) != 0);
		bool disableProxyMode = ShouldDisableProxiesForRendering(isMEdit);
		if(disableProxyMode) {
			DisableProxyMode(false);
			m_proxiesWereDisabledForRendering = true;
		}
	}

	return 1; 	
}

int BMTex::RenderEnd(TimeValue t) { 

	// Re-enable proxies when done rendering, if they were disabled previously.

	if(m_proxiesWereDisabledForRendering) {
		m_proxiesWereDisabledForRendering = false;
		bool flushNonProxy = !KeepNonProxyInMemoryAfterRendering();
		EnableProxyMode(flushNonProxy);
	}

	return 1; 	
}

int BMTex::LoadMapFiles(TimeValue t) {	
	LoadBitmap(t,1,1);	
	return 1;	
}

void BMTex::RenderBitmap(TimeValue t, Bitmap *bm, float scale3D, BOOL filter) {

	// Disable proxy only when 'filter' is specified, as we assume that 'filter'
	// is equivalent to 'high quality'.
	BMTex::ProxyModeDisableGuard proxyModeDisabler(*this, (filter != 0));

	LoadBitmap(t);
	Bitmap* thebm = GetActiveBitmap();
	if (thebm) 
		bm->CopyImage(thebm, filter?COPY_IMAGE_RESIZE_HI_QUALITY:COPY_IMAGE_RESIZE_LO_QUALITY, 0);
}

Class_ID BMTex::ClassID() {	

	return bmTexClassID; 
}

SClass_ID BMTex::SuperClassID() { 

	return TEXMAP_CLASS_ID; 
}

void BMTex::GetClassName(TSTR& s) { 

	s= GetString(IDS_DS_BITMAP); 
} 

void BMTex::DeleteThis() { 

	delete this; 
}	

void BMTex::ReloadBitmap(bool isUIAction) {
	if(GetActiveBitmap() != NULL) {
		loadFailed = FALSE;
		FreeBitmap(); 
		LoadBitmap(GetCOREInterface()->GetTime(), TRUE, TRUE);

		// Re-load the bitmap in-place, forcing a reload.
		m_theBitmap.LoadInto(true);
	}
	else {
		loadFailed = FALSE;
		TimeValue t;
		t = GetCOREInterface()->GetTime();
		LoadBitmap(t);
	}

	//DS:  10/19/99: update path name on button in case it has changed ( fix for #196020)
	IParamMap2* pmap = pblock->GetMap();
	if (pmap != NULL)
		pmap->SetText(bmtex_bitmap, (char *)m_bi.Name());


	if (paramDlg) 
		paramDlg->EnableAlphaButtons(FALSE);

	// Trigger the bitmap proxy pre-caching dialog if the reload occured
	// as the result pressing the "reload" button in the UI.
	if(isUIAction) {
		ShowBitmapProxyPrecacheDialog();
	}
}

void BMTex::EnumAuxFiles(NameEnumCallback& nameEnum, DWORD flags) {

	if ((flags&FILE_ENUM_CHECK_AWORK1)&&TestAFlag(A_WORK1)) return; // LAM - 4/21/03

	UpdateBIName(); //k2tong|May.17.2006|update the m_bi before an enum to report the most up to date info.
	
	if(flags&FILE_ENUM_ACCESSOR_INTERFACE)	{
		int idx = TheManager->ioList.ResolveDevice(&m_bi);
		
		IEnumAuxAssetsCallback* callback = static_cast<IEnumAuxAssetsCallback*>(&nameEnum);
		if(idx >= 0 && TheManager->ioList[idx].TestCapabilities(BMMIO_IFL))	{
			// handle IFL case
			DeclareBitmapAsset(*callback, BMTexPathAccessor(this), &m_bi, BMM_ASSET_PROXY | BMM_ASSET_GROUP);
			m_bi.EnumAuxFiles(nameEnum, flags);
			callback->EndGroup();
		}
		else	{
			// normal bitmap or unsupported type 
			DeclareBitmapAsset(*callback, BMTexPathAccessor(this), &m_bi, BMM_ASSET_PROXY);
		}
	}
	else	{
		m_bi.EnumAuxFiles(nameEnum,flags);
	}

	BitmapTex::EnumAuxFiles(nameEnum,flags); // LAM - 4/21/03
}


void BMTex::DiscardTexHandle() {
	if (texHandle) {
		texHandle->DeleteThis();
		texHandle = NULL;
	}
}

void BMTex::ActivateTexDisplay(BOOL onoff) {
	if (!onoff) 
		DiscardTexHandle();
}

static BMM_Color_64 black64(0,0,0,0);

BITMAPINFO* BMTex::GetVPDisplayDIB(TimeValue t, TexHandleMaker& thmaker, Interval &valid, BOOL mono, BOOL forceW, BOOL forceH) {   
	LoadBitmap(t);
	valid = clipValid;
	if (m_bi.FirstFrame()!=m_bi.LastFrame())
	{
		// bug :424747
		//the image has multiple frames, the vlidity should be only for the current frame
		// For cases where the sequence has very few frames, it should be valid for more than just the current "t".
		// We have no real
		valid.SetInstant(t);
		//Without this fix, the viewport does not update with the correct image frame. 
		// When GetVPDisplayDIB gets called, some callers look at the validity, others look at the current image frame.
		// When looking at the validity, the viewport was not getting updates.

	}

	Bitmap* thebm = GetActiveBitmap();
	if (thebm==NULL) {
		return NULL;
	}
	BITMAPINFO *bmi = NULL;
	int xflags = premultAlpha?0:EX_MULT_ALPHA;
	if (applyCrop) {
		int w = thebm->Width();
		int h = thebm->Height();
		Bitmap *newBM;
		BitmapInfo bif;
		bif.SetName(_T("y8798734"));
		bif.SetType(BMM_TRUE_32);
		bif.SetFlags(MAP_HAS_ALPHA);
		if (placeImage) {
			int x0,y0,nw,nh;
			int bmw = thmaker.Size();
			int bmh = int(float(bmw)*float(h)/float(w));
			bif.SetWidth(bmw);
			bif.SetHeight(bmh);
			newBM = TheManager->Create(&bif);
			newBM->Fill(0,0,0,0);
			nw = int(float(bmw)*clipw);
			nh = int(float(bmh)*cliph);
			x0 = int(float(bmw-1)*clipu);
			y0 = int(float(bmh-1)*clipv);

			if (nw<1) nw = 1;
			if (nh<1) nh = 1;
			PixelBuf row(nw);

			Bitmap *tmpBM;
			BitmapInfo bif2;
			bif2.SetName(_T("xxxx67878"));
			bif2.SetType(BMM_TRUE_32);
			bif2.SetFlags(MAP_HAS_ALPHA);
			bif2.SetWidth(nw);				
			bif2.SetHeight(nh);
			tmpBM = TheManager->Create(&bif2);
			tmpBM->CopyImage(thebm, COPY_IMAGE_RESIZE_LO_QUALITY, 0);
			BMM_Color_64*  p1 = row.Ptr();
			for (int y = 0; y<nh; y++) {
				tmpBM->GetLinearPixels(0,y, nw, p1);
				if ((mono&&alphaAsMono)||(alphaAsRGB)) {
					for (int ix =0; ix<nw; ix++) 
						p1[ix].r = p1[ix].g = p1[ix].b = p1[ix].a;
				}
				//if (alphaSource==ALPHA_NONE||premultAlpha) {
				if (alphaSource==ALPHA_NONE) {
					for (int ix =0; ix<nw; ix++) 
						p1[ix].a = 0xffff;
				}
				else if (alphaSource==ALPHA_RGB) {
					for (int ix =0; ix<nw; ix++) 
						p1[ix].a = (p1[ix].r+p1[ix].g+p1[ix].b)/3;;
				}
				newBM->PutPixels(x0,y+y0, nw, p1);
			}
			tmpBM->DeleteThis();
			//			texHandle = thmaker.CreateHandle(newBM,uvGen->SymFlags(), xflags);
			bmi = thmaker.BitmapToDIB(newBM,uvGen->SymFlags(), xflags, forceW, forceH);
			newBM->DeleteThis();
		}
		else {
			int x0,y0,nw,nh;
			nw = int(float(w)*clipw);
			nh = int(float(h)*cliph);
			x0 = int(float(w-1)*clipu);
			y0 = int(float(h-1)*clipv);
			if (nw<1) nw = 1;
			if (nh<1) nh = 1;
			bif.SetWidth(nw);
			bif.SetHeight(nh);
			PixelBuf row(nw);
			newBM = TheManager->Create(&bif);
			BMM_Color_64*  p1 = row.Ptr();
			for (int y = 0; y<nh; y++) {
				thebm->GetLinearPixels(x0,y+y0, nw, p1);
				if ((mono&&alphaAsMono)||(alphaAsRGB)) {
					for (int ix =0; ix<nw; ix++) 
						p1[ix].r = p1[ix].g = p1[ix].b = p1[ix].a;
				}
				//					if (alphaSource==ALPHA_NONE||premultAlpha) {
				if (alphaSource==ALPHA_NONE) {
					for (int ix =0; ix<nw; ix++) 
						p1[ix].a = 0xffff;
				}
				else if (alphaSource==ALPHA_RGB) {
					for (int ix =0; ix<nw; ix++) 
						p1[ix].a = (p1[ix].r+p1[ix].g+p1[ix].b)/3;;
				}
				newBM->PutPixels(0,y, nw, p1);
			}
			//			texHandle = thmaker.CreateHandle(newBM,uvGen->SymFlags(), xflags);
			bmi = thmaker.BitmapToDIB(newBM,uvGen->SymFlags(), xflags, forceW, forceH);
			newBM->DeleteThis();
		}
	}
	else {
		if (mono&&alphaAsMono) xflags |= EX_RGB_FROM_ALPHA;
		if (alphaAsRGB) xflags |= EX_RGB_FROM_ALPHA;
		if (alphaSource==ALPHA_NONE) xflags |= EX_OPAQUE_ALPHA;
		if (alphaSource==ALPHA_RGB) xflags |= EX_ALPHA_FROM_RGB;
		//		texHandle = thmaker.CreateHandle(thebm,uvGen->SymFlags(), xflags);
		bmi = thmaker.BitmapToDIB(thebm,uvGen->SymFlags(), xflags, forceW, forceH);
	}
	return bmi;
}

DWORD_PTR BMTex::GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker) {
	if (texHandle&&texTime==CalcFrame(t)&&texValid.InInterval(t)) 
		return texHandle->GetHandle();
	else {
		DiscardTexHandle();  
		texTime = CalcFrame(t);
		texHandle = thmaker.MakeHandle(GetVPDisplayDIB(t,thmaker,texValid));  
		return texHandle->GetHandle();
	}
}

int BMTex::IsHighDynamicRange( ) const
{
	Bitmap* thebm = GetActiveBitmap();
	return thebm == NULL ? false : thebm->IsHighDynamicRange();
}

//----------------------------------------------------------- 

AColor BMTex::EvalColor(ShadeContext& sc) {
	if (!sc.doMaps) return black;
	AColor c;
	if (sc.GetCache(this,c)) 
		return c; 
	IPoint3 sp = sc.ScreenCoord();
	if (gbufID) sc.SetGBufferID(gbufID);
	if (GetActiveBitmap()==NULL) 
		return black;



	if (!mTieTimeToMatID)
	{
		if (alphaAsRGB)	{
			float a = texout->Filter(uvGen->EvalUVMapMono(sc,&alphasamp,filterType!=FILTER_NADA));
			c = AColor(a,a,a,a);
		}
		else {
			c = texout->Filter(uvGen->EvalUVMap(sc,&mysamp,filterType!=FILTER_NADA));
			if (!premultAlpha) c= AColor(c.r*c.a, c.g*c.a, c.b*c.a, c.a);
		}
	}
	else
	{
		int whichFrame = ComputeParticleFrame(sc);
		if (alphaAsRGB)	{
			float a = texout->Filter(uvGen->EvalUVMapMono(sc,alphasampList[whichFrame],filterType!=FILTER_NADA));
			c = AColor(a,a,a,a);
		}
		else {
			c = texout->Filter(uvGen->EvalUVMap(sc,mysampList[whichFrame],filterType!=FILTER_NADA));
			if (!premultAlpha) c= AColor(c.r*c.a, c.g*c.a, c.b*c.a, c.a);
		}
	}
	sc.PutCache(this,c); 
	return c;
}

float BMTex::EvalMono(ShadeContext& sc) {
	if (!sc.doMaps||GetActiveBitmap()==NULL) 
		return 0.0f;
	float f;
	if (sc.GetCache(this,f)) 
		return f; 
	if (gbufID) sc.SetGBufferID(gbufID);

	if (!mTieTimeToMatID)
	{

		if (alphaAsMono) 
			f = texout->Filter(uvGen->EvalUVMapMono(sc,&alphasamp,filterType!=FILTER_NADA));
		else 
			f = texout->Filter(uvGen->EvalUVMapMono(sc,&mysamp,filterType!=FILTER_NADA));
	}
	else
	{
		int whichFrame = ComputeParticleFrame(sc);

		if (alphaAsMono) 
			f = texout->Filter(uvGen->EvalUVMapMono(sc,alphasampList[whichFrame],filterType!=FILTER_NADA));
		else 
			f = texout->Filter(uvGen->EvalUVMapMono(sc,mysampList[whichFrame],filterType!=FILTER_NADA));

	}
	sc.PutCache(this,f); 
	return f;
}

Point3 BMTex::EvalNormalPerturb(ShadeContext& sc) {
	Point3 dPdu, dPdv;
	Point2 dM;
	if (!sc.doMaps) return Point3(0,0,0);
	if (gbufID) sc.SetGBufferID(gbufID);
	if (GetActiveBitmap()==NULL) 
		return Point3(0,0,0);
	if (!mTieTimeToMatID)
	{
		uvGen->GetBumpDP(sc,dPdu,dPdv);  // get bump basis vectors
		if (alphaAsMono) 
			dM =(.01f)*uvGen->EvalDeriv(sc,&alphasamp,filterType!=FILTER_NADA);
		else 
			dM =(.01f)*uvGen->EvalDeriv(sc,&mysamp,filterType!=FILTER_NADA);
	}
	else
	{
		int whichFrame = ComputeParticleFrame(sc);

		if (alphaAsMono) 
			dM =(.01f)*uvGen->EvalDeriv(sc,alphasampList[whichFrame],filterType!=FILTER_NADA);
		else 
			dM =(.01f)*uvGen->EvalDeriv(sc,mysampList[whichFrame],filterType!=FILTER_NADA);
	}

#if 0
	// Blinn's algorithm
	Point3 N = sc.Normal();
	Point3 uVec = CrossProd(N,dPdv);
	Point3 vVec = CrossProd(N,dPdu);
	return texout->Filter(-dM.x*uVec+dM.y*vVec);
#else						 
	return texout->Filter(dM.x*dPdu+dM.y*dPdv);
#endif
}

RefTargetHandle BMTex::Clone(RemapDir &remap) {
	// AF Don't log a restore object during this create
	// the Undo of a new inside a clone is the deletion of the new object.
	// No need to undo back to uninitialized values
	theHold.Suspend();
	BMTex *mnew = new BMTex();
	theHold.Resume();

	*((MtlBase*)mnew) = *((MtlBase*)this);  // copy superclass stuff
	mnew->ReplaceReference(0,remap.CloneRef(uvGen));
	mnew->ReplaceReference(1,remap.CloneRef(pblock));
	mnew->ReplaceReference(2,remap.CloneRef(texout));
	mnew->ReplaceReference(3,remap.CloneRef(pblockTime));
	mnew->filterType  = filterType;
	mnew->alphaSource = alphaSource;
	mnew->alphaAsMono = alphaAsMono;
	mnew->alphaAsRGB  = alphaAsRGB;
	mnew->premultAlpha = premultAlpha;
	mnew->endCond     = endCond;
	mnew->applyCrop   = applyCrop;
	mnew->ivalid.SetEmpty();
	mnew->placeImage  = placeImage;
	mnew->randPlace   = randPlace;
	mnew->applyCrop   = applyCrop;
	mnew->startTime  = startTime;
	mnew->pbRate  = pbRate;
	mnew->m_bi = m_bi;
	BaseClone(this, mnew, remap);
	mnew->LoadBitmap( GetCOREInterface()->GetTime() );
	return (RefTargetHandle)mnew;
}

ParamDlg* BMTex::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) {
	//	BMTexDlg *dm = new BMTexDlg(hwMtlEdit, imp, this);
	//	dm->LoadDialog(TRUE);	
	//	paramDlg = dm;
	//	return dm;
	// JBW: the main difference here is the automatic creation of a ParamDlg by the new
	// ClassDesc2 function CreateParamDlgs().  This mirrors the way BeginEditParams()
	// can be redirected to the ClassDesc2 for automatic ParamMap2 management.  In this 
	// case a special subclass of ParamDlg, AutoMParamDlg, defined in IParamm2.h, is 
	// created.  It can act as a 'master' ParamDlg to which you can add any number of 
	// secondary dialogs and it will make sure all the secondary dialogs are kept 
	// up-to-date and deleted as necessary.  
	// Here you see we create the Coordinate, Gradient and Output ParamDlgs in the desired 
	// order, and then add the Coordinate and Output dlgs as secondaries to the 
	// Gradient master AutoMParamDlg so it will keep them up-to-date automatically

	// create the rollout dialogs
	uvGenDlg = uvGen->CreateParamDlg(hwMtlEdit, imp);	
	IAutoMParamDlg* masterDlg = bmTexCD.CreateParamDlgs(hwMtlEdit, imp, this);
	texoutDlg = texout->CreateParamDlg(hwMtlEdit, imp);
	// add the secondary dialogs to the master
	masterDlg->AddDlg(uvGenDlg);
	masterDlg->AddDlg(texoutDlg);
	paramDlg = new BMTexDlg(hwMtlEdit,imp,this);
	bmtex_param_blk.SetUserDlgProc(paramDlg);
	ip = imp;
	EnableStuff();
	return masterDlg;

}

BMTex::~BMTex() {
	UnRegisterNotification(BMTexNotify, this, NOTIFY_PROXY_TEMPORARY_DISABLE_END);
	UnRegisterNotification(BMTexNotify, this, NOTIFY_PROXY_TEMPORARY_DISABLE_START);
	UnRegisterNotification(BMTexNotify, this, NOTIFY_BITMAP_CHANGED);
	DiscardTexHandle();
	FreeBitmap();
}

BOOL BMTex::SetDlgThing(ParamDlg* dlg)
{
	// JBW: set the appropriate 'thing' sub-object for each
	// secondary dialog
	if (paramDlg) paramDlg->SetThing(this);
	EnableStuff();

	if ((uvGenDlg!= NULL) && (dlg == uvGenDlg))
		uvGenDlg->SetThing(uvGen);
	else if ((texoutDlg!= NULL) && (dlg == texoutDlg))
		texoutDlg->SetThing(texout);
	else 
		return FALSE;
	return TRUE;

}



void BMTex::Update(TimeValue t, Interval& valid) {		

	pblockTime->GetValue( bmtex_matidtime, 0, mTieTimeToMatID,FOREVER);

	if (!ivalid.InInterval(t)) {

		uvGen->SetClipFlag(FALSE);

		ivalid.SetInfinite();
		uvGen->Update(t,ivalid);
		clipValid.SetInfinite();
		pblock->GetValue( bmtex_clipu, t, clipu, clipValid );
		pblock->GetValue( bmtex_clipv, t, clipv, clipValid );
		pblock->GetValue( bmtex_clipw, t, clipw, clipValid );
		pblock->GetValue( bmtex_cliph, t, cliph, clipValid );
		pblock->GetValue( bmtex_jitter, t, jitter, clipValid );

		pblock->GetValue( bmtex_filtering, t, filterType, clipValid );
		pblock->GetValue( bmtex_alphasource, t, alphaSource, clipValid );

		if (alphaSource == 0) 
			alphaSource  = ALPHA_FILE;
		else if (alphaSource == 1) 
			alphaSource  = ALPHA_RGB;
		else if (alphaSource == 2)
			alphaSource  = ALPHA_NONE;

		pblock->GetValue( bmtex_monooutput, t, alphaAsMono, clipValid );
		pblock->GetValue( bmtex_rgboutput, t, alphaAsRGB, clipValid );
		pblock->GetValue( bmtex_premultalpha, t, premultAlpha, clipValid );
		pblock->GetValue( bmtex_usejitter, t, randPlace, clipValid );

		pblock->GetValue( bmtex_crop_place, t, placeImage, clipValid );

		pblock->GetValue( bmtex_apply, t, applyCrop, clipValid );

		pblockTime->GetValue( bmtex_start, t, startTime, clipValid );
		pblockTime->GetValue( bmtex_playbackrate, t, pbRate, clipValid );
		pblockTime->GetValue( bmtex_endcondition, t, endCond, clipValid );

		if (applyCrop) 
			ivalid &= clipValid;
		else 
			clipValid.SetInfinite();

		texout->Update(t,ivalid);

		// [dl | 20aug2003] Set the bitmap frame to be used. This is needed by
		// mental ray to correctly pick up which frame of the bitmap to use.
		// This is only necessary if the bitmap is not using the MtlID as frame
		// number.
		if (!mTieTimeToMatID) {

			// Get the bitmap info from the parameter block
			PBBitmap* bitmapPB = NULL;
			pblock->GetValue( bmtex_bitmap, t,bitmapPB, clipValid );

			// Free the bitmap if the filenames don't match. We need to do this
			// before we overwrite 'bi', otherwise there'll be no way of
			// telling whether the correct bitmap file is loaded.
			if ( ((bitmapPB != NULL) && (_tcscmp(m_bi.Name(), bitmapPB->bi.Name())) )   )
				FreeBitmap();

			// Update 'bi' to be current with the contents of the param block
			UpdateBIName();

			// Calculate the frame number
			m_bi.SetCurrentFrame(CalcFrame(t));

			// Commit the bitmap info to the param block, since mental ray uses
			// the value from the param block.
			if(bitmapPB != NULL) {
				bitmapPB->bi = m_bi;
			}
		}
	}

	if (GetActiveBitmap()&&!bmWasSet) {
		if (m_bi.FirstFrame()!=m_bi.LastFrame())
		{
			ivalid.SetInstant(t);  // force bitmap to be reloaded
		}
	}
	else {
		ivalid.SetInstant(t);  // force bitmap to be reloaded 
	}

	UpdtSampler();
	valid &= ivalid;
}

RefTargetHandle BMTex::GetReference(int i) {
	switch(i) {
		case 0: return uvGen;
		case 1: return pblock;
		case 2:	return texout;
		case 3: return pblockTime;
		default: return NULL;
	}
}

int BMTex::RemapRefOnLoad(int iref) { 
	if (loadingOld) { 
		switch(iref) {
			case 0: return 0;
			case 1: return 2;
			default: assert(0); return 0;
		}
	}
	else return iref;
}

void BMTex::SetReference(int i, RefTargetHandle rtarg) {
	switch(i) {
		case 0: uvGen = (UVGen *)rtarg; break;
		case 1: pblock = (IParamBlock2 *)rtarg; break;
		case 2:	texout = (TextureOutput *)rtarg; break;
		case 3: pblockTime = (IParamBlock2 *)rtarg; break;
		default: break;
	}
}


Animatable* BMTex::SubAnim(int i) {
	switch (i) {
		case 0: return uvGen;
		case 1: return pblock;
		case 2: return texout;
		case 3: return pblockTime;
		default: assert(0); return NULL;
	}
}

TSTR BMTex::GetFullName() {
	TSTR cnm,nm,fname;
	GetClassName(cnm);
	UpdateBIName();   //DS 2/18/99
	GetBMName(m_bi,fname);
	TCHAR *s = fname.Length()>0? fname.data(): cnm.data(); 
	nm.printf(_T("%s (%s)"),GetName().data(), s);
	return nm;
}

TSTR BMTex::SubAnimName(int i) {
	switch (i) {
		case 0: return TSTR(GetString(IDS_DS_COORDINATES));		
		case 1: return TSTR(GetString(IDS_DS_PARAMETERS));		
		case 2: return TSTR(GetString(IDS_DS_OUTPUT));		
		case 3: return TSTR(GetString(IDS_DS_PARAMETERS));		
		default: assert(0); return TSTR(_T(""));
	}
}

RefResult BMTex::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
								  PartID& partID, RefMessage message ) {
	switch (message) {
	case REFMSG_CHANGE:
		{
			ivalid.SetEmpty();
			if (hTarget == pblock)
			{
				// see if this message came from a changing parameter in the pblock,
				// if so, limit rollout update to the changing item and update any active viewport texture
				ParamID changing_param = pblock->LastNotifyParamID();
				bmtex_param_blk.InvalidateUI(changing_param);
				if (changing_param != -1)
					DiscardTexHandle();

				//  435297 : Handling changes here and reload the bitmap
				// The code used to be in PBAccessor::Set for bmtex_bitmap.
				if (changing_param == bmtex_bitmap)
				{
					if (!loading) 
					{
						ReloadBitmapAndUpdate();
						// send message so browser/navigator name gets updated
						NotifyDependents(FOREVER,PART_ALL,REFMSG_SUBANIM_STRUCTURE_CHANGED);
					}
				}
			}



			//SS 10/23/2000: Since code accessing this param block is not available
			// to VIZ anywhere else, it made sense to me to add an ifdef check around
			// this entire if statement.
			if (hTarget == pblockTime)
			{
				// see if this message came from a changing parameter in the pblock,
				// if so, limit rollout update to the changing item and update any active viewport texture
				ParamID changing_param = pblockTime->LastNotifyParamID();
				bmtex_time_param_blk.InvalidateUI(changing_param);
			}

			break;
		}
	case REFMSG_UV_SYM_CHANGE:
		DiscardTexHandle();  
		break;
	}
	return(REF_SUCCEED);
}



//------------------------------------------------------------------------
// IO
//------------------------------------------------------------------------

class BMTexPostLoad : public PostLoadCallback {
public:
	BMTex *chk;
	BMTexPostLoad(BMTex *b) {chk=b;}
	void proc(ILoad *iload) {
		chk->loadingOld = FALSE;
		delete this;
	}
};

#define MTL_HDR_CHUNK 0x4000
#define OLDBMTEX_NAME_CHUNK 0x5001
#define BMTEX_FILTER_CHUNK 0x5002
#define BMTEX_ALPHASOURCE_CHUNK 0x5003
#define BMTEX_NAME_CHUNK 0x5004
#define BMTEX_IO_CHUNK 0x5010
#define BMTEX_START_CHUNK 0x5011
#define BMTEX_RATE_CHUNK 0x5012
#define BMTEX_ALPHA_MONO_CHUNK 0x5013
#define BMTEX_ENDCOND_CHUNK 0x5014
#define BMTEX_ALPHA_RGB_CHUNK 0x5016
#define BMTEX_VERSOLD_CHUNK 0x5021
#define BMTEX_VERSION_CHUNK 0x5022
#define BMTEX_ALPHA_NOTPREMULT_CHUNK 0x5030
#define BMTEX_CLIP_CHUNK 0x5040
#define BMTEX_PLACE_IMAGE_CHUNK 0x5050
#define BMTEX_JITTER_CHUNK 0x5060

IOResult BMTex::Save(ISave *isave) { 
	//	ULONG nb;
	IOResult res;
	// Save common stuff
	isave->BeginChunk(MTL_HDR_CHUNK);
	res = MtlBase::Save(isave);
	if (res!=IO_OK) return res;
	isave->EndChunk();
	return IO_OK;
}	


class BMTex2PostLoad : public PostLoadCallback {
public:
	BMTex *n;
	BOOL isParam2;
	BMTex2PostLoad(BMTex *ns, BOOL b) {n = ns;isParam2 = b;}
	void proc(ILoad *iload) {  

		if (!isParam2)
		{
			macroRecorder->Disable();
			n->pblock->SetValue( bmtex_filtering, 0, n->filterType);
			if (n->alphaSource == ALPHA_FILE) n->alphaSource  = 0;
			else if (n->alphaSource == ALPHA_RGB) n->alphaSource  = 1;
			else if (n->alphaSource == ALPHA_NONE) n->alphaSource  = 2;

			n->pblock->SetValue( bmtex_alphasource, 0, n->alphaSource);


			n->pblock->SetValue( bmtex_monooutput, 0, n->alphaAsMono );
			n->pblock->SetValue( bmtex_rgboutput, 0, n->alphaAsRGB );
			n->pblock->SetValue( bmtex_premultalpha, 0, n->premultAlpha);
			n->pblock->SetValue( bmtex_usejitter, 0, n->randPlace );

			n->pblock->SetValue( bmtex_crop_place, 0, n->placeImage);

			n->pblock->SetValue( bmtex_apply, 0, n->applyCrop);

			PBBitmap bPB(n->m_bi);
			n->loading = TRUE;
			n->pblock->SetValue( bmtex_bitmap, 0, &bPB);
			n->loading = FALSE;

			n->pblockTime->SetValue( bmtex_start, 0, n->startTime);
			n->pblockTime->SetValue( bmtex_playbackrate, 0, n->pbRate);
			n->pblockTime->SetValue( bmtex_endcondition, 0, n->endCond);

			macroRecorder->Enable();

		}
		delete this; 


	} 
};


IOResult BMTex::Load(ILoad *iload) { 
	ULONG nb;
	IOResult res;
	TSTR oldBMName;
	//	iload->RegisterPostLoadCallback(new ParamBlockPLCB(oldVersions, 3, &curVersion, this,1));
	while (IO_OK==(res=iload->OpenChunk())) {
		switch(iload->CurChunkID())  {
			case MTL_HDR_CHUNK:
				res = MtlBase::Load(iload);
				break;
			case BMTEX_VERSOLD_CHUNK:
				loadingOld = TRUE;
				break;
			case BMTEX_FILTER_CHUNK:
				iload->Read(&filterType,sizeof(filterType),&nb);			
				isParm2 = FALSE;
				break;
			case BMTEX_ALPHASOURCE_CHUNK:
				isParm2 = FALSE;
				iload->Read(&alphaSource,sizeof(alphaSource),&nb);			
				break;
			case BMTEX_NAME_CHUNK:
				{
					// This chunk is used for old files only
					isParm2 = FALSE;
					TCHAR *buf;
					if(IO_OK==iload->ReadWStringChunk(&buf)) {
						if((buf != NULL) && (buf[0] != _T('\0'))) {
							oldBMName = buf;
						}
					}
				}
				break;
			case BMTEX_IO_CHUNK:
				isParm2 = FALSE;
				res = m_bi.Load(iload);
				// DS 7/18/96 - This is to handle old files 
				// that had the bi saved with start = end = 0, 
				// which now screws up animation.
				m_bi.SetStartFrame(BMM_UNDEF_FRAME);
				m_bi.SetEndFrame(BMM_UNDEF_FRAME);
				break;
			case BMTEX_START_CHUNK:
				isParm2 = FALSE;
				iload->Read(&startTime,sizeof(startTime),&nb);			
				break;
			case BMTEX_RATE_CHUNK:
				isParm2 = FALSE;
				iload->Read(&pbRate,sizeof(pbRate),&nb);			
				break;
			case BMTEX_ALPHA_MONO_CHUNK:
				isParm2 = FALSE;
				alphaAsMono = TRUE;
				break;
			case BMTEX_ALPHA_RGB_CHUNK:
				isParm2 = FALSE;
				alphaAsRGB = TRUE;
				break;
			case BMTEX_ALPHA_NOTPREMULT_CHUNK:
				isParm2 = FALSE;
				premultAlpha = FALSE;
				break;
			case BMTEX_ENDCOND_CHUNK:
				isParm2 = FALSE;
				iload->Read(&endCond,sizeof(endCond),&nb);			
				break;
			case BMTEX_CLIP_CHUNK:
				isParm2 = FALSE;
				applyCrop = TRUE;
				break;
			case BMTEX_PLACE_IMAGE_CHUNK:
				isParm2 = FALSE;
				placeImage = TRUE;
				break;
			case BMTEX_JITTER_CHUNK:
				isParm2 = FALSE;
				randPlace = TRUE;
				break;
		}
		iload->CloseChunk();
		if (res!=IO_OK) 
			return res;
	}
	if (alphaSource<0||alphaSource>ALPHA_NONE) 
		alphaSource = ALPHA_FILE;


	if (!isParm2)
	{
		ParamBlock2PLCB* plcb = new ParamBlock2PLCB( oldVersions, NUMOLDVERSIONS, &bmtex_param_blk, this, 1);
		iload->RegisterPostLoadCallback(plcb);

		BMTex2PostLoad *bmt2PLCB = new BMTex2PostLoad(this,isParm2);
		iload->RegisterPostLoadCallback(bmt2PLCB);
	}

	if (oldBMName.Length()>0) {
		m_bi.SetName(oldBMName);   // for obsolete files	
		iload->SetObsolete();
	}

	return IO_OK;
}

void BMTex::SetBitmap(Bitmap *bm) {
	FreeBitmap(); 
	m_theBitmap.SetBitmap(bm);
	bmWasSet = (bm != NULL);
}

#define PROPID_SETBITMAP 0x111

int BMTex::SetProperty(ULONG id, void *data)
{
	switch (id) {
	case PROPID_CLEARCACHES:
		FreeBitmap();
		return 1;
	case PROPID_SETBITMAP: 
		SetBitmap((Bitmap *)data);
		return 1;

	default: return 0;
	}
}



void BMTex::BitmapLoadDlg()	{
	HWND hpanel = GetCOREInterface()->GetMAXHWnd();

	// lrr 06/04 
	if (_tcscmp(s_lastName, _T(""))) {
		m_bi.SetName(s_lastName);
	}
	else {
		// Use the images directory as the default dir.
		TCHAR dummy[MAX_PATH];
		_tcscpy(dummy, TheManager->GetDir(APP_IMAGE_DIR));
		// Need to add a dummy filename to this path
		BMMAppendSlash(dummy);
		m_bi.SetName(dummy);
	}

	BOOL res = TheManager->SelectFileInputEx(&m_bi, hpanel, GetString(IDS_DS_SELECT_BMFILE));

	//load into param block
	if (res) {
		PBBitmap bt(m_bi);
		loading = 1;  // disable accessor
		pblock->SetValue(bmtex_bitmap, 0, &bt);
		loading = 0;
		FreeBitmap();   // DS 9/25/00   
		ReloadBitmap(false); // DS 9/25/00  -- this is to fix #254986

		if (m_bi.Name())							//LRR 06/04
			_tcscpy( s_lastName, m_bi.Name() ); 

		ShowBitmapProxyPrecacheDialog();
	}
}

//watje forces the bitmap to reload and view to be redrawn
void BMTex::ReloadBitmapAndUpdate()
{
	FreeBitmap();
	ReloadBitmap(false);
	fileNotFound = FALSE; // DS 4/20/99 
	if (TestMtlFlag(MTL_TEX_DISPLAY_ENABLED)) {
		if (ip)
			ip->MtlChanged();
	}
}


void BMTex::fnReload()
{
	ReloadBitmap(false);
	BroadcastNotification(NOTIFY_BITMAP_CHANGED, (void *)m_bi.Name());
	if (ip) ip->MtlChanged();
	if ((ip) && (paramDlg)) paramDlg->Invalidate();  // incase path of map has changed.

}

void BMTex::fnViewImage()
{
	if ((ip) && (paramDlg)) paramDlg->ShowCropImage();
}

int BMTex::ComputeParticleFrame(ShadeContext& sc)
{

	//	return 0;
	// Evaluate...
	Object *ob = sc.GetEvalObject();
	float u=0.0f;
	if (ob && ob->IsParticleSystem()) {
		ParticleObject *obj = (ParticleObject*)ob;
		TimeValue age  = obj->ParticleAge(sc.CurTime(),sc.mtlNum);
		if (age>=0) 
		{
			if (m_bi.NumberFrames() <= 1) return 0;
			return CalcFrame(age)%m_bi.NumberFrames();
		}

	}
	return 0;

}

void* BMTex::GetInterface(ULONG id) {

	// [dl | 27feb2004] I added this to make it possible for mental ray to
	// retrieve the tab of particle bitmaps.
	if(id == IID_BITMAPTEX_PARTICLE_BITMAP_LIST) {
		MaxSDK::Array<Bitmap*>* particleBitmapList = new MaxSDK::Array<Bitmap*>();
		size_t count = m_theBitmapList.length();
		particleBitmapList->setLengthUsed(count);
		for(size_t i = 0; i < count; ++i) {
			(*particleBitmapList)[i] = m_theBitmapList[i].GetBitmap();
		}
		return particleBitmapList;
	}
	else {
		return Texmap::GetInterface(id);
	}
}

void BMTex::EnableProxyMode(bool deleteNonProxyBitmap) {

	m_theBitmap.EnableProxyMode(deleteNonProxyBitmap);
	size_t count = m_theBitmapList.length();
	for(size_t i = 0; i < count; ++i) {
		BitmapHolder& currentBitmap = m_theBitmapList[i];
		currentBitmap.EnableProxyMode(deleteNonProxyBitmap);
	}

	// Re-initialize the bitmap samplers, since the active bitmap might have changed.
	// The previous active bitmap might even have been deleted.
	InitBitmapSamplers();
}

void BMTex::DisableProxyMode(bool deleteProxyBitmap) {

	m_theBitmap.DisableProxyMode(deleteProxyBitmap);
	size_t count = m_theBitmapList.length();
	for(size_t i = 0; i < count; ++i) {
		BitmapHolder& currentBitmap = m_theBitmapList[i];
		currentBitmap.DisableProxyMode(deleteProxyBitmap);
	}

	InitBitmapSamplers();
}

void BMTex::ShowBitmapProxyPrecacheDialog() {

	const TCHAR* bmName = m_bi.Name();
	if((bmName != NULL) && (bmName[0] != _T('\0'))) {
		// The Bitmap Proxy Pre-Caching dialog is now displayed, if necessary.
		// The  ShowPrecacheDialog() method internally determines whether 
		// it's necessary to display the actual dialog.
		IBitmapProxyManager* proxyManager = GetBitmapProxyManager();
		if((proxyManager != NULL) && proxyManager->GetGlobalProxyEnable()) {
			Tab<TCHAR*> filenames;
			TCHAR* bmNameNonConstHackHackHack = const_cast<TCHAR*>(bmName);
			filenames.Append(1, &bmNameNonConstHackHackHack);
			proxyManager->ShowPrecacheDialog(&filenames);
		}
	}
}

bool BMTex::ShouldDisableProxiesForRendering(bool isMtlEditor) {

	if(isMtlEditor) {
		return false;
	}
	else {
		IBitmapProxyManager* proxyManager = GetBitmapProxyManager();
		if(proxyManager != NULL) {
			IBitmapProxyManager::RenderMode renderMode = proxyManager->GetGlobalProxyRenderMode();
			switch(renderMode) {
				default:
					DbgAssert(false);
					// Fall through...
				case IBitmapProxyManager::kRenderMode_UseProxies:
					return false;
				case IBitmapProxyManager::kRenderMode_UseFullRes_KeepInMemory:
				case IBitmapProxyManager::kRenderMode_UseFullRes_FlushFromMemory:
					return true;
			}
		}
		else {
			DbgAssert(false);
			return false;
		}
	}
}

bool BMTex::KeepNonProxyInMemoryAfterRendering() {

	IBitmapProxyManager* proxyManager = GetBitmapProxyManager();
	if(proxyManager != NULL) {
		IBitmapProxyManager::RenderMode renderMode = proxyManager->GetGlobalProxyRenderMode();
		switch(renderMode) {
			default:
				DbgAssert(false);
				// Fall through...
			case IBitmapProxyManager::kRenderMode_UseProxies:
			case IBitmapProxyManager::kRenderMode_UseFullRes_FlushFromMemory:
				return false;
			case IBitmapProxyManager::kRenderMode_UseFullRes_KeepInMemory:
				return true;
		}
	}
	else {
		DbgAssert(false);
		return false;
	}
}

void BMTex::InitBitmapSamplers() {

	BOOL tieTimeToMatID;
	pblockTime->GetValue( bmtex_matidtime, 0, tieTimeToMatID,FOREVER);

	if (!tieTimeToMatID)
	{
		mysamp.Init(this);
		alphasamp.Init(this);
	}
	else
	{
		for (int i = 0; i < mysampList.Count(); i++)
		{
			mysampList[i]->Init(this,m_theBitmapList[i].GetBitmap());
			alphasampList[i]->Init(this,m_theBitmapList[i].GetBitmap());
		}
	}
}

//==============================================================================
// class BMTex::BMMSilentModeGuard
//==============================================================================

BMTex::BMMSilentModeGuard::BMMSilentModeGuard(bool setSilentMode) 
: m_setSilentMode(setSilentMode)
{
	if(m_setSilentMode) {
		m_previousSilentMode = TheManager->SetSilentMode(true);
	}
}
BMTex::BMMSilentModeGuard::~BMMSilentModeGuard() {
	if(m_setSilentMode) {
		TheManager->SetSilentMode(m_previousSilentMode);
	}
}

//==============================================================================
// class BMTex::BitmapHolder
//==============================================================================

BMTex::BitmapHolder::BitmapHolder() 
: m_nonProxyBitmap(NULL),
  m_proxyBitmap(NULL),
  m_isProxyMode(true)
{
}

BMTex::BitmapHolder::BitmapHolder(const BitmapHolder& other) {

	*this = other;
}

BMTex::BitmapHolder::~BitmapHolder() {
	// The bitmaps should have already been freed by BMTex. I won't free them here (just in case,
	// to avoid double-deleting something), but I'm still putting a debug assert to catch any
	// possible issues.
	DbgAssert((m_proxyBitmap == NULL) && (m_nonProxyBitmap == NULL));
}


BMTex::BitmapHolder& BMTex::BitmapHolder::operator=(const BitmapHolder& other) {

	m_nonProxyBitmap = other.m_nonProxyBitmap;
	m_proxyBitmap = other.m_proxyBitmap;
	m_isProxyMode = other.m_isProxyMode;
	m_nonProxyBitmapInfo = other.m_nonProxyBitmapInfo;
	m_proxyBitmapInfo = other.m_proxyBitmapInfo;

	return *this;
}

bool IsCurrentFrame( Bitmap* bitmap, int frame ) {
	BitmapStorage* storage = NULL;
	if( bitmap!=NULL ) storage = bitmap->Storage();

	if( storage==NULL ) return false;
	BitmapInfo* info = GetSubjectInfo( &(storage->bi) );

	return (info->CurrentFrame() == frame);
}

void BMTex::BitmapHolder::EnableProxyMode(bool deleteNonProxyBitmap) {

	if( m_isProxyMode!=true ) {
		m_isProxyMode = true;

		// Ensure both bitmaps are on the same frame
		// Non-proxy info holds the desired frame, whether in proxy mode or not
		int currentFrame = m_nonProxyBitmapInfo.CurrentFrame();
		if( !IsCurrentFrame( m_proxyBitmap, currentFrame ) ) {
			if( m_proxyBitmap!=NULL )
				BitmapProxy_GoTo( m_nonProxyBitmapInfo, m_proxyBitmapInfo, m_proxyBitmap );
		}
	}

	if(deleteNonProxyBitmap) {
		FreeNonProxyBitmap();
	}
}

void BMTex::BitmapHolder::DisableProxyMode(bool deleteProxyBitmap) {

	if( m_isProxyMode!=false ) {
		m_isProxyMode = false;

		// Ensure both bitmaps are on the same frame
		// Non-proxy info holds the desired frame, whether in proxy mode or not
		int currentFrame = m_nonProxyBitmapInfo.CurrentFrame();
		if( !IsCurrentFrame( m_nonProxyBitmap, currentFrame ) ) {
			if( m_nonProxyBitmap!=NULL )
				BitmapNonProxy_GoTo( m_nonProxyBitmapInfo, m_nonProxyBitmap );
		}
	}

	if(deleteProxyBitmap) {
		FreeProxyBitmap();
	}
}

bool BMTex::BitmapHolder::LoadBitmap(BMMRES& status, bool silenceBitmapManager) {

	// Set silent mode if necessary
	BMMSilentModeGuard silentModeGuard(silenceBitmapManager);

	if(m_isProxyMode) {
		// The bitmap should be NULL, otherwise we'll get a memory leak
		DbgAssert(m_proxyBitmap == NULL);
		m_proxyBitmap = BitmapProxy_Load(m_nonProxyBitmapInfo, m_proxyBitmapInfo, &status);
		return (m_proxyBitmap != NULL);
	}
	else {
		// The bitmap should be NULL, otherwise we'll get a memory leak
		DbgAssert(m_nonProxyBitmap == NULL);
		m_nonProxyBitmap = BitmapNonProxy_Load(m_nonProxyBitmapInfo, &status);
		return (m_nonProxyBitmap != NULL);
	}
}

void BMTex::BitmapHolder::LoadInto(bool forceReload) {

	// Reload both the proxy and non-proxy bitmaps, to ensure that none of them is invalid.
	if(m_nonProxyBitmap != NULL) {
		BitmapNonProxy_LoadInto(m_nonProxyBitmapInfo, &m_nonProxyBitmap, forceReload);
	}

	if(m_proxyBitmap != NULL) {
		BitmapProxy_LoadInto(m_nonProxyBitmapInfo, m_proxyBitmapInfo, &m_proxyBitmap, forceReload);
	}
}

void BMTex::BitmapHolder::FreeBitmap() {

	// Free both the proxy and non-proxy bitmaps.
	FreeNonProxyBitmap();
	FreeProxyBitmap();
}

void BMTex::BitmapHolder::FreeProxyBitmap() {

	if(m_proxyBitmap != NULL) {
		m_proxyBitmap->DeleteThis();
		m_proxyBitmap = NULL;
	}
}

void BMTex::BitmapHolder::FreeNonProxyBitmap() {

	if(m_nonProxyBitmap != NULL) {
		m_nonProxyBitmap->DeleteThis();
		m_nonProxyBitmap = NULL;
	}
}

BMMRES BMTex::BitmapHolder::GotoFrame(bool silenceBitmapManager) {

	// Forward the call to the active bitmap.
	// The "inactive" bitmap doesn't get the call, which I think should be fine.
	// If there's ever a problem, though, it might be necessary to call GoTo()
	// on both bitmaps.

	BMMRES result = BMMRES_SUCCESS;

	// Set silent mode if necessary
	BMMSilentModeGuard silentModeGuard(silenceBitmapManager);

	if(m_isProxyMode) {
		if(m_proxyBitmap != NULL) {
			result = BitmapProxy_GoTo(m_nonProxyBitmapInfo, m_proxyBitmapInfo, m_proxyBitmap);
		}
		else {
			DbgAssert(false);
		}
	}
	else {
		if(m_nonProxyBitmap != NULL) {
			result = BitmapNonProxy_GoTo(m_nonProxyBitmapInfo, m_nonProxyBitmap);
		}
		else {
			DbgAssert(false);
		}
	}

	return result;
}

BitmapInfo& BMTex::BitmapHolder::GetBitmapInfo() {

	return m_nonProxyBitmapInfo;
}

const BitmapInfo& BMTex::BitmapHolder::GetBitmapInfo() const {

	return m_nonProxyBitmapInfo;
}

Bitmap* BMTex::BitmapHolder::GetBitmap() const {

	if(m_isProxyMode) {
		return m_proxyBitmap;
	}
	else {
		return m_nonProxyBitmap;
	}
}

void BMTex::BitmapHolder::SetBitmap(Bitmap* bitmap) {

	// The bitmap should first have been freed in order to avoid memroy leaks.
	DbgAssert((m_nonProxyBitmap == NULL) && (m_proxyBitmap == NULL));

	// Set the proper bitmap, depending on whether it's a proxy or not.
	if(bitmap == NULL) {
		m_proxyBitmap = NULL;
		m_nonProxyBitmap = NULL;
	}
	else if((bitmap->Storage() != NULL) && (bitmap->Storage()->bi.Flags() & MAP_PROXY)) {
		m_proxyBitmap = bitmap;
	}
	else {
		m_nonProxyBitmap = bitmap;
	}
}

int BMTex::BitmapHolder::SetFilter(UINT filterType) {

	int result = 0;
	if(m_proxyBitmap != NULL) {
		result |= m_proxyBitmap->SetFilter(filterType);
	}
	if(m_nonProxyBitmap != NULL) {
		result |= m_nonProxyBitmap->SetFilter(filterType);
	}

	return result;
}

void BMTex::BitmapHolder::SetCroppingValues(float u, float v, float w, float h, BOOL placeImage) {

	if(m_proxyBitmap != NULL) {
		m_proxyBitmap->SetCroppingValues(u, v, w, h, placeImage);
	}
	if(m_nonProxyBitmap != NULL) {
		m_nonProxyBitmap->SetCroppingValues(u, v, w, h, placeImage);
	}
}

void BMTex::BitmapHolder::SetNotify(BitmapNotify *bmnot) {

	// Register the notification with only one bitmap; registering it
	// with both bitmaps results in a crash when the notification is received
	// and the bitmaps are reloaded (the crash occurs when the notification system
	// attempts to notify the second, now defunct bitmap).

	// Register the notification with the proxy bitmap.
	// The notification is responsible for updating the viewport,
	// so it must be registered against the proxy bitmap which is used for viewport display.
	// The notification also triggers a full reload if the subject changes,
	// which would be equivalent if registered against either bitmap.

	if( (m_proxyBitmap != NULL) && (m_proxyBitmap->GetNotify() != bmnot) ) {
		m_proxyBitmap->SetNotify(bmnot);
	}
}

//==============================================================================
// class BMTex::ProxyModeDisableGuard
//==============================================================================

BMTex::ProxyModeDisableGuard::ProxyModeDisableGuard(BMTex& bitmapTex, bool doDisable) 
: m_bitmapTex(bitmapTex),
  m_doDisable(doDisable)
{
	if(m_doDisable) {
		m_bitmapTex.DisableProxyMode(false);
	}
}

BMTex::ProxyModeDisableGuard::~ProxyModeDisableGuard() {

	if(m_doDisable) {
		bool flushNonProxyAfterRender = !KeepNonProxyInMemoryAfterRendering();
		m_bitmapTex.EnableProxyMode(flushNonProxyAfterRender);
	}
}
