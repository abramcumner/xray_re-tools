/**********************************************************************
 *<
   FILE: normalrender.cpp

   DESCRIPTION:Gnormal(colorbumpmap)

   CREATED BY:  Ben Lipman

   HISTORY:  version 0.17 (6/01/04)

 *>   Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/

#include "normalrender.h"
#include "VNormal.h"

static AColor grey(0.5f,0.5f,0.5f,1.0f);


//===========================================================================
//
// Class Gnormal - Class Descriptor
//
//===========================================================================

class GnormalClassDesc:public ClassDesc2
#ifdef CATALYST
   ,public IMtlRender_Compatibility_MtlBase  
#endif
{
   public:
   GnormalClassDesc();

   int                  IsPublic() { 
#ifdef NORMAL_MAPPING_RTT_BAKE_ELEMENT
      return TRUE;
#else 
      return FALSE;
#endif 
   }

   void*             Create(BOOL loading = FALSE) { return new Gnormal(); }
   const TCHAR*         ClassName() { return GetString(IDS_CLASS_NAME); }
   SClass_ID            SuperClassID() { return TEXMAP_CLASS_ID; }
   Class_ID          ClassID() { return GNORMAL_CLASS_ID; }
   const TCHAR*         Category() { return GetString(IDS_CATEGORY); }

   const TCHAR*         InternalName() { return _T("Gnormal"); }  // returns fixed parsable name (scripter-visible name)
   HINSTANCE            HInstance() { return hInstance; }            // returns owning module handle
#ifdef CATALYST
   virtual bool         IsCompatibleWithRenderer(ClassDesc& rendererClassDesc);
#endif
};

GnormalClassDesc::GnormalClassDesc() {
#ifdef CATALYST
   IMtlRender_Compatibility_MtlBase::Init(*this);
#endif
}

static GnormalClassDesc GnormalDesc;
ClassDesc2* GetGnormalDesc() { return &GnormalDesc; }


//===========================================================================
//
// Class Gnormal - Parameter Block
//
//===========================================================================

static ParamBlockDesc2 gnormal_param_blk (
   Gnormal::gnormal_params, _T("params"),  0, &GnormalDesc, P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
   //rollout
   IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
   // params
   Gnormal::gn_mult_spin,     _T("mult_spin"),     TYPE_FLOAT,    P_ANIMATABLE,  IDS_GN_MULT_SPIN, 
      p_default,     1.0f, 
      p_range,       -100.0f,1000.0f, 
      p_ui,          TYPE_SPINNER,     EDITTYPE_FLOAT, IDC_GN_MULT_EDIT,   IDC_GN_MULT_SPIN, 0.1f, 
      end,
   Gnormal::gn_bmult_spin,       _T("bump_spin"),     TYPE_FLOAT,    P_ANIMATABLE,  IDS_GN_BMULT_SPIN, 
      p_default,     1.0f, 
      p_range,       -100.0f,1000.0f, 
      p_ui,          TYPE_SPINNER,     EDITTYPE_FLOAT, IDC_GN_BMULT_EDIT,  IDC_GN_BMULT_SPIN, 0.1f, 
      end,
   Gnormal::gn_map_normal,    _T("normal_map"),    TYPE_TEXMAP,  P_OWNERS_REF,  IDS_GN_MAP_NORMAL,
      p_refno,    1,
      p_subtexno,    0,
      p_ui,            TYPE_TEXMAPBUTTON,   IDC_GN_NORMAL_MAP,
      end,
   Gnormal::gn_map_bump,      _T("bump_map"),      TYPE_TEXMAP,  P_OWNERS_REF,  IDS_GN_MAP_BUMP,
      p_refno,    2,
      p_subtexno,    1,
      p_ui,            TYPE_TEXMAPBUTTON,   IDC_GN_BUMP_MAP,
      end,
   Gnormal::gn_map1on,     _T("map1on"),     TYPE_BOOL,  0, IDS_GN_MAP1ON,
      p_default,        TRUE,
      p_ui,          TYPE_SINGLECHEKBOX,     IDC_GN_MAP1ON,
      end,
   Gnormal::gn_map2on,     _T("map2on"),     TYPE_BOOL,  0, IDS_GN_MAP2ON,
      p_default,        TRUE,
      p_ui,          TYPE_SINGLECHEKBOX,     IDC_GN_MAP2ON,
      end,
   Gnormal::gn_method,     _T("method"),     TYPE_INT,   0,    IDS_GN_METHOD_RADIO,
      p_default,         0,
      p_ui,          TYPE_RADIO, 4, IDC_GN_UV_RAD, IDC_GN_XYZ_RAD, IDC_GN_SCREEN_RAD, IDC_GN_WORLD_RAD,
      p_vals,        0,1,2,3,
      end,
   Gnormal::gn_flip_red,   _T("flipred"),    TYPE_BOOL,  0, IDS_GN_FLIP_RED,
      p_default,        false,
      p_ui,          TYPE_SINGLECHEKBOX,     IDC_GN_FLIP_RED,
      end,
   Gnormal::gn_flip_green, _T("flipgreen"),  TYPE_BOOL,  0, IDS_GN_FLIP_GREEN,
      p_default,        false,
      p_ui,          TYPE_SINGLECHEKBOX,     IDC_GN_FLIP_GREEN,
      end,
   Gnormal::gn_swap_rg,    _T("swap_rg"),    TYPE_BOOL,  0, IDS_GN_SWAP_RG,
      p_default,        false,
      p_ui,          TYPE_SINGLECHEKBOX,     IDC_GN_SWAP_RG,
      end,
   end
   );



//===========================================================================
//
// Class Gnormal - Implementation
//
//===========================================================================

Gnormal::Gnormal()
{
   for (int i=0; i<NSUBTEX; i++) subTex[i] = NULL;

   pblock = NULL;
   GnormalDesc.MakeAutoParamBlocks(this);
   Init();
}

Gnormal::~Gnormal()
{
}

//From MtlBase
void Gnormal::Reset() 
{
for (int i=0; i<NSUBTEX; i++)
      DeleteReference(i+1);
      Init();
   ivalid.SetEmpty();

}

void Gnormal::Init()
{
   ivalid.SetEmpty();
   gnMult = 1.0f;
   gnBMult = 1.0f;
   swap_rg = flip_red, flip_green = false;
   method = 0;
   map1on = map2on = true;
}

void Gnormal::InvalidateUI()
{
   gnormal_param_blk.InvalidateUI(pblock->LastNotifyParamID());
}

inline void Gnormal::Reorder(AColor &s) { 
   if (flip_red)  s.r = 1.0f - s.r;
   if (flip_green) s.g = 1.0f - s.g;
   if (swap_rg) {
      float t = s.r;
      s.r = s.g;
      s.g = t;
   }
}

inline void Gnormal::Reorder(Point3& p) { 
   if (flip_red)  p.x = -p.x;
   if (flip_green) p.y = -p.y;
   if (swap_rg) {
      float temp = p.x;
      p.x = p.y;
      p.y = temp;
   }
}



//-----------------------------------------------------------------------------
//-- From Animatable


    
Animatable* Gnormal::SubAnim(int i) 
{
   switch (i) {
   case 0:     return pblock;
   default: return subTex[i-1];
   }
}

TSTR Gnormal::SubAnimName(int i) 
{
   switch (i) {
   case 0:     return GetString(IDS_PARAMS);
   default: return GetSubTexmapTVName(i-1);
   }
}

int Gnormal::RenderBegin(TimeValue t, ULONG flags) {
   //free all vnormal data; it will be initialized inside Eval()
   GetVNormalMgr()->Free();
   return 1;
}

int Gnormal::RenderEnd(TimeValue t)
{
   GetVNormalMgr()->Free(); //free all vnormal data
   return 1;
}

//-----------------------------------------------------------------------------
//-- From ReferenceMaker

#define MTL_HDR_CHUNK 0x4000

IOResult Gnormal::Save(ISave *isave) 
{
   IOResult res;
   isave->BeginChunk(MTL_HDR_CHUNK);
   res = MtlBase::Save(isave);
   if (res!=IO_OK) return res;
   isave->EndChunk();
   return IO_OK;
}

IOResult Gnormal::Load(ILoad *iload) 
{ 
   IOResult res;
   int id;
   while (IO_OK==(res=iload->OpenChunk())) {
      switch(id=iload->CurChunkID())  {
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

RefTargetHandle Gnormal::GetReference(int i) 
{
   switch (i) {
   case 0:     return pblock;
   default: return subTex[i-1];
   }
}

void Gnormal::SetReference(int i, RefTargetHandle rtarg) 
{
   switch(i) {
   case 0:     pblock = (IParamBlock2 *)rtarg;  break;
   default: subTex[i-1] = (Texmap *)rtarg;   break;
   }
}

RefResult Gnormal::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
   PartID& partID, RefMessage message ) 
{
   switch (message) {
   case REFMSG_CHANGE:
      ivalid.SetEmpty();
      if (hTarget==pblock)
      {
         ParamID changing_param = pblock->LastNotifyParamID();
         gnormal_param_blk.InvalidateUI(changing_param);
      }
      break;
   }  
   return(REF_SUCCEED);
}

//-----------------------------------------------------------------------------
//-- From ReferenceTarget

RefTargetHandle Gnormal::Clone(RemapDir &remap) 
{
   Gnormal *mnew = new Gnormal();
   *((MtlBase*)mnew) = *((MtlBase*)this); // copy superclass stuff
    mnew->ReplaceReference(0,remap.CloneRef(pblock));
   mnew->gnMult = gnMult;
   mnew->gnBMult = gnBMult;
   mnew->method = method;
   mnew->ivalid.SetEmpty();
   mnew->subTex[0] = NULL;
   mnew->subTex[1] = NULL;
   if (subTex[0])
      mnew->ReplaceReference(1,remap.CloneRef(subTex[0]));
   if (subTex[1])
      mnew->ReplaceReference(2,remap.CloneRef(subTex[1]));
   mnew->map1on = map1on;
   mnew->map2on = map2on;
   BaseClone(this, mnew, remap);
   return (RefTargetHandle)mnew;
}

//-----------------------------------------------------------------------------
//-- From MtlBase

ULONG Gnormal::LocalRequirements(int subMtlNum) {
   ULONG retval = 0;
   for(int i=0; i<NSUBTEX; i++)
      if (subTex[i]) retval |= subTex[i]->LocalRequirements(subMtlNum);
   return retval;
}

void Gnormal::LocalMappingsRequired(int subMtlNum, BitArray & mapreq, BitArray &bumpreq) {
   for(int i=0; i<NSUBTEX; i++)
      if (subTex[i]) subTex[i]->LocalMappingsRequired(subMtlNum,mapreq,bumpreq);
}

ParamDlg* Gnormal::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) 
{
   IAutoMParamDlg* masterDlg = GnormalDesc.CreateParamDlgs(hwMtlEdit, imp, this);
   return masterDlg; 
}

void Gnormal::Update(TimeValue t, Interval& valid) 
{
   BOOL doUpdate = TRUE;
   if( pblock->GetInt( gn_method, t ) == TANGENTUV_SPACE )
      ivalid.SetInstant(t);
   else if (!ivalid.InInterval(t))
      ivalid.SetInfinite();
   else doUpdate = FALSE;

   if( doUpdate ) {
      pblock->GetValue( gn_mult_spin, t, gnMult, ivalid);
      pblock->GetValue( gn_bmult_spin, t, gnBMult, ivalid);
      pblock->GetValue( gn_method, t, method, ivalid);
      pblock->GetValue( gn_flip_red, t, flip_red, ivalid);
      pblock->GetValue( gn_flip_green, t, flip_green, ivalid);
      pblock->GetValue( gn_swap_rg, t, swap_rg, ivalid);
      pblock->GetValue( gn_map1on, t, map1on, ivalid );
      pblock->GetValue( gn_map2on, t, map2on, ivalid );

      for(int i=0; i<NSUBTEX; i++)
         if (subTex[i]) subTex[i]->Update(t,ivalid);
   }
   valid &= ivalid;
   GetVNormalMgr()->Free(); //free all vnormal data
}

void Gnormal::SetSubTexmap(int i, Texmap *m) 
{
   ReplaceReference(i+1,m);
   if(i==0)
   {
      gnormal_param_blk.InvalidateUI(gn_map_normal);
      ivalid.SetEmpty();
   }
   else if(i==1)
   {
      gnormal_param_blk.InvalidateUI(gn_map_bump);
      ivalid.SetEmpty();
   }
}

TSTR Gnormal::GetSubTexmapSlotName(int i) 
{  
   switch (i) 
   {
   case 0: TSTR(GetString(IDS_GN_MAP_NORMAL));
   case 1: TSTR(GetString(IDS_GN_MAP_BUMP));
   default: return TSTR(_T(""));
   }
}

DWORD_PTR Gnormal::GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker)
{
   //TODO: Return the texture handle to this texture map
   return NULL;
}

void Gnormal::ActivateTexDisplay(BOOL onoff)
{
   //TODO: Implement this only if SupportTexDisplay() returns TRUE
}

//-----------------------------------------------------------------------------
//-- From Texmap

AColor Gnormal::EvalColor( ShadeContext& sc ) {
   if( (!map1on) || (subTex[0]==NULL) )
		return AColor(0.5f,0.5f,1.0f,1.0f);

   AColor retval = subTex[0]->EvalColor(sc);
   Reorder( retval );
   return retval;
}

float Gnormal::EvalMono(ShadeContext& sc)
{
   return Intens(EvalColor(sc));
}

Point3 Gnormal::EvalNormalPerturb(ShadeContext& sc)
{
   if( (!map1on) || (!subTex[0]) ) {
      if(  map2on && subTex[1] ) {
         Point3 additionalPerturb = subTex[1]->EvalNormalPerturb(sc);
         if( gnBMult!=1.0f ) additionalPerturb *= gnBMult;
		 return additionalPerturb;
      }
	  else return Point3(0,0,0);
   }

   AColor s  =  subTex[0]->EvalColor(sc);

   Reorder(s);
   Point3 p  =  Point3 ( 2*s.r - 1, 2*s.g - 1, 2*s.b - 1 );

   if (gbufID) sc.SetGBufferID(gbufID);

   // Calculate the front surface normal.  For the backface, normal is reversed by caller 
   Point3 frontNormal = sc.OrigNormal();
   if( sc.backFace ) frontNormal = -frontNormal;

   switch (method){
   case TANGENTUV_SPACE: {
         int mapChannel = subTex[0]->GetMapChannel();

         Point3 frontOrigNormal = sc.OrigNormal(); // Front-facing normal
         if( sc.backFace ) frontOrigNormal = -frontOrigNormal;

         //get the basis vectors at the current location
         TangentBasis tangentBasis;
         GetVNormalMgr()->GetTangentBasis( sc, mapChannel, tangentBasis );

         p.y = -p.y; //default input is X-Right, Y-Down;  Must negate the Y value
         p = ((p.x*tangentBasis.uBasis) + (p.y*tangentBasis.vBasis) + (p.z*frontOrigNormal)).Normalize();
      }
      break;

   case SCREEN_SPACE:
      // p is treated as if in camera space (no conversion needed)
      break;

   case WORLDXYZ_SPACE:
      // p is treated as if in world space, convert to camera space
      p = sc.VectorFromNoScale(p,REF_WORLD);
      break;

   case LOCALXYZ_SPACE:
      // p is treated as if in object space, convert to camera space
      p = sc.VectorFromNoScale(p,REF_OBJECT);
      break;
   }


   //-- Case 1: Most common case, multiplier is 1.0 and no additional bump is supplied

   Point3 perturb = p - frontNormal;

   if( (gnMult==1.0f) && ((!map2on) || (subTex[1]==NULL)) ) {
      return perturb;
   }


   //-- Case 2: The bump multiplier is not 1.0, or an additional bump is supplied

   if( gnMult!=1.0f ) perturb *= gnMult;

   if( map2on && subTex[1] )
   {
	  Point3 additionalPerturb = subTex[1]->EvalNormalPerturb(sc);
	  if( gnBMult!=1.0f ) additionalPerturb *= gnBMult;
      perturb += additionalPerturb;
   }

   return perturb;
}



//===========================================================================
//
// Mental Ray support
//
//===========================================================================


#ifdef CATALYST
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

bool GnormalClassDesc::IsCompatibleWithRenderer(ClassDesc& rendererClassDesc) {

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

imrShader* Gnormal::GetMRShader(imrShaderCreation& shaderCreation) {

   // Create the shader and return it
   imrShader* shader = shaderCreation.CreateShader(_T("max_GNormal"), GetFullName());
   return shader;
}

void Gnormal::ReleaseMRShader() {

   /* Do nothing */
}

bool Gnormal::NeedsAutomaticParamBlock2Translation() {

   return false;
}

void Gnormal::TranslateParameters(imrTranslation& translationInterface, imrShader* shader, TimeValue t, Interval& valid) {

   // Get the parameter block in which we need to store the parameter values
   IParamBlock2* paramsPBlock = shader->GetParametersParamBlock();
   if(paramsPBlock != NULL) {

      Interval localValid = FOREVER;

      // Update at the current time 
      Update(t, localValid);

      TranslateMRShaderParameter(paramsPBlock, _T("multiplier"), gnMult);
      TranslateMRShaderParameter(paramsPBlock, _T("bumpMultiplier"), gnBMult);
      TranslateMRShaderParameter(paramsPBlock, _T("flip_red"), flip_red);
      TranslateMRShaderParameter(paramsPBlock, _T("flip_green"), flip_green);
      TranslateMRShaderParameter(paramsPBlock, _T("swap_rg"), swap_rg);
      TranslateMRShaderParameter(paramsPBlock, _T("normalMapOn"), map1on);
      TranslateMRShaderParameter(paramsPBlock, _T("normalMapShader"), subTex[0]);
      TranslateMRShaderParameter(paramsPBlock, _T("bumpMapOn"), map2on);
      TranslateMRShaderParameter(paramsPBlock, _T("bumpMapShader"), subTex[1]);
      TranslateMRShaderParameter(paramsPBlock, _T("method"), method);
      TranslateMRShaderParameter(paramsPBlock, _T("grey"), grey);
      TranslateMRShaderParameter(paramsPBlock, _T("normalMap_MapChannel"), (subTex[0] != NULL) ? subTex[0]->GetMapChannel() : 0);
      TranslateMRShaderParameter(paramsPBlock, _T("gbufID"), static_cast<int>(gbufID));

      valid &= localValid;
   }
   else {
      // This shouldn't happen
      DbgAssert(false);
   }
}

void Gnormal::GetAdditionalReferenceDependencies(AdditionalDependencyTable& refTargets) {
   // None
}

bool Gnormal::HasRequirement(Requirement requirement, TimeValue t, Interval& valid, void* arg) {

   switch(requirement) {
      case kReq_TangentBasisVectors:
         if(pblock != NULL) {
            Update(t, valid);
            // HACK FIXME: There's a difference between the #define's of the mental ray and MAX shaders
            // I don't know why... but that means I have to consider both tangent and tangent local.
            if((method == TANGENTUV_SPACE) || (method == TANGENTLOCAL_SPACE)) {
               int* mapChannel = static_cast<int*>(arg);
               *mapChannel = subTex[0]?subTex[0]->GetMapChannel():1;
               return true;
            }
         }
         else {
            DbgAssert(false);
         }
         break;
      default:
         break;
   }

   return false;
}

BaseInterface* Gnormal::GetInterface(Interface_ID id) {

   if(id == IMRSHADERTRANSLATION_INTERFACE_ID) {
      return static_cast<imrShaderTranslation*>(this);
   }
   else {
      return Texmap::GetInterface(id);
   }
}

extern "C"
{
   LONG  __cdecl _InterlockedIncrement(LONG volatile *Addend);
   LONG  __cdecl _InterlockedDecrement(LONG volatile *Addend);
   LONG  __cdecl _InterlockedCompareExchange(LONG volatile *Dest,
                                             LONG Exchange, LONG Comp);
   LONG  __cdecl _InterlockedExchangeAdd(LONG volatile *Addend,
                                         LONG Value);
   LONG  __cdecl _InterlockedAnd(LONG volatile *Val, LONG mask);
}

#pragma intrinsic (_InterlockedIncrement)
#pragma intrinsic (_InterlockedDecrement)
#pragma intrinsic (_InterlockedCompareExchange)
#pragma intrinsic (_InterlockedExchangeAdd)
#pragma intrinsic (_InterlockedAnd)

// Atomic operation:
// Sets (*p)+=a, and returns the previous value of (*p)
DWORD LockXAdd( DWORD* p, DWORD a ) {
   return _InterlockedExchangeAdd(reinterpret_cast<LONG *>(p), a);
}

// Atomic operation:
// If (*p)==a, then ... sets (*p)=b and returns true
// Else           ... sets (*q)=(*p) and returns false
DWORD LockCmpXChg( DWORD* p, DWORD Exchange, DWORD Comperand ) {
   return _InterlockedCompareExchange(reinterpret_cast<LONG *>(p), Exchange, Comperand);
}

// Atomic operation: (*p)++
void LockInc( DWORD* p ) {
   _InterlockedIncrement(reinterpret_cast<LONG*>(p));
}

// Atomic operation: (*p)--
void LockDec( DWORD* p ) {
   _InterlockedDecrement(reinterpret_cast<LONG*>(p));
}

// Atomic operation: (*p) &= a
void LockAnd( DWORD* p, DWORD a ) {
   _InterlockedAnd(reinterpret_cast<LONG*>(p), a);
}

//===========================================================================
//
// Class ReaderWriterLock
//
//===========================================================================

#define READER_BITS 0x7FFFFFFF
#define WRITER_BITS 0x80000000

void ReaderWriterLock::EnterRead() {
   // Increments the count, and get the previous count (atomic operation)
   int checkBits = LockXAdd( &bits, 1 );
   while( checkBits & WRITER_BITS ) {
      Sleep(0); //SwitchToThread();
      checkBits = bits;
   }
}

void ReaderWriterLock::ExitRead() {
   DbgAssert( (bits & READER_BITS)!=0 );
   LockDec( &bits ); //bits-- (atomic operation)
}

void ReaderWriterLock::EnterWrite() {
   DWORD prevBits;
   while( TRUE ) {
      // If bits==0, sets bits=WRITER_BITS. Returns original value of bits (atomic operation)
      prevBits = LockCmpXChg( &bits, WRITER_BITS, 0 );
      if( prevBits==0  ) break; // success
      else
         Sleep(0); //SwitchToThread();
   }
}

void ReaderWriterLock::ExitWrite() {
   DbgAssert( (bits & WRITER_BITS)!=0 );
   LockAnd( &bits, (~WRITER_BITS) ); //bits &= ~WRITER_BITS (atomic operation)
}

#endif