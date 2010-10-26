/**********************************************************************
*<
FILE: DxStdMtl2.h

DESCRIPTION:	DirectX Interface for the standard material

CREATED BY:		Neil Hazzard

HISTORY:		Created:  04/19/04
				The EffectElements class has been taken from the DxMaterial.  In the standard material case we are not
				hosting a UI, so it is much simpler.  We take care of converting textures to D3D versions, but all multi
				pass handling is done by the parser.


*>	Copyright (c) 2004, All Rights Reserved.
************************************************************************/

#ifndef	__DXSTDMTL2_H__
#define __DXSTDMTL2_H__


//#include "Max.h"

#include "maxscrpt\MAXScrpt.h"
#include "maxscrpt\MAXObj.h"
#include "maxscrpt\3DMath.h"
#include "maxscrpt\msplugin.h"

#include "ID3D9GraphicsWindow.h"
#include "IStdDualVS.h"
#ifndef IDX9_PIXELSHADER9_H
	#include "idx9pixelshader.h"
#endif

#ifndef IDX9_VERTEXSHADER9_H
	#include "idx9vertexshader.h"
#endif

#include "iparamb2.h"

#ifndef NO_ASHLI // MSW 5/14/2004

#include "RTMax.h"
#include "stdmat.h"
#include "notify.h"

class EffectElements;
class TextureElement;
class DxStdMtl2;

static TCHAR * supportedParams[]={
	_T("diffuse"),
	_T("ambient"),
	_T("specular"),
	_T("specularLevel"),
	_T("opacity"),
	_T("glossiness"),
	_T("selfIllumAmount"),
	_T("selfIllumColor"),
	_T("Falloff"),
	_T("Hotspot"),
	_T("Color"),
	_T("Multiplier"),
};

#define supportedParam_count (sizeof(supportedParams)/sizeof(supportedParams[1]))

class ElementContainer
{
	Tab<EffectElements *> elementList;

public:
	void AddElement(EffectElements * ele); 
	inline int NumberOfElements(){return elementList.Count();}
	int NumberofElementsByType(int Type);
	EffectElements * GetElement(int index);
	EffectElements * GetElementByType(int index, int Type);
	void DeleteAllElements();

};

class IDX8VertexShaderCache : public VertexShaderCache
{
public:

};

class LightCache : public ReferenceMaker
{
	INode * node;
	IParamBlock * lightParamBlock;
	DxStdMtl2 * shader;

public:

	LightCache(DxStdMtl2 * dxMtl, INode * lightNode);
	~LightCache();
		
	int NumRefs();
	RefTargetHandle GetReference(int i);
	void SetReference(int i, RefTargetHandle rtarg);

	INode * GetLightNode(){return node;}

	RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, 
		PartID& partID, RefMessage message );
};


class DxStdMtl2 : public IDX9VertexShader, public IStdDualVSCallback
    ,public IEffectManager
    ,public ILightManager
	,public IEffectFile
	,public IParameterManager
{

	LPDIRECT3DDEVICE9 pd3dDevice;
	ID3D9GraphicsWindow * myGWindow;
	DWORD	dwVertexShader;
	bool bRefreshElements;
	LPD3DXEFFECT pEffect;
	LPD3DXEFFECT m_pErrorEffect;

	IStdDualVS *stdDualVS;
	StdMat2 *map;
	IRenderMeshCache * mpMeshCache;
	ElementContainer elementContainer;
	Tab<LightCache * > sceneLights;

	IEffectParser * pEffectParser;
	bool bTransparency;
	D3DXHANDLE hTechnique;
	TSTR techniqueName;
	bool bReEntrant;
	int ReEntrantCount;
//	IHLSLCodeGenerator * codeGen;

	int m_CurCache;
	UINT m_DirectXVersion;

public:
	bool bPreDeleteLight;
	DxStdMtl2(StdMat2 * m);
	~DxStdMtl2();

	StdMat2* GetStdMat(){return map;}
	
	HRESULT Initialize(Mesh *mesh, INode *node);
	HRESULT Initialize(MNMesh *mnmesh, INode *node);
	// From FPInterface
//	virtual LifetimeType	LifetimeControl() { return noRelease; }

	// From IVertexShader
	HRESULT ConfirmDevice(ID3D9GraphicsWindow *d3dgw);
	HRESULT ConfirmPixelShader(IDX9PixelShader *pps);
	bool CanTryStrips();
	int GetNumMultiPass();

	LPDIRECT3DVERTEXSHADER9 GetVertexShaderHandle(int numPass) { return 0; }
	HRESULT SetVertexShader(ID3D9GraphicsWindow *d3dgw, int numPass);

	// Draw 3D mesh as TriStrips
	bool	DrawMeshStrips(ID3D9GraphicsWindow *d3dgw, MeshData *data);

	// Draw 3D mesh as wireframe
	bool	DrawWireMesh(ID3D9GraphicsWindow *d3dgw, WireMeshData *data);

	// Draw 3D lines
	void	StartLines(ID3D9GraphicsWindow *d3dgw, WireMeshData *data);
	void	AddLine(ID3D9GraphicsWindow *d3dgw, DWORD *vert, int vis);
	bool	DrawLines(ID3D9GraphicsWindow *d3dgw);
	void	EndLines(ID3D9GraphicsWindow *d3dgw, GFX_ESCAPE_FN fn);

	// Draw 3D triangles
	void	StartTriangles(ID3D9GraphicsWindow *d3dgw, MeshFaceData *data);
	void	AddTriangle(ID3D9GraphicsWindow *d3dgw, DWORD index, int *edgeVis);
	bool	DrawTriangles(ID3D9GraphicsWindow *d3dgw);
	void	EndTriangles(ID3D9GraphicsWindow *d3dgw, GFX_ESCAPE_FN fn);

	// from IStdDualVSCallback
	ReferenceTarget *GetRefTarg();
	VertexShaderCache *CreateVertexShaderCache();
	HRESULT  InitValid(Mesh* mesh, INode *node);
	HRESULT  InitValid(MNMesh* mnmesh, INode *node);
	void	 DeleteRenderMeshCache(INode * node);

	void Draw();
	void DrawError();
	void SetRenderStates();
	void ResetDXStates(bool deviceReset = false);

	IHLSLCodeGenerator::CodeVersion GetPixelShaderSupport(LPDIRECT3DDEVICE9 pd3dDevice, DWORD & instSlots);

	int GetMatIndex(Mtl * m_Mtl);

	void PatchInLightNodes();
	
    RefResult NotifyMaterialChanged(Interval changeInt, RefTargetHandle hTarget,	PartID& partID, RefMessage message );
    void SetEffectData();
	void FindLights(INode * parentNode);
	void UpdateSceneLights(bool forceUpdate=false);
	HRESULT  CreateEffectParser();
	static void NotificationCallback(void* param, NotifyInfo* info);
	bool CreateAndLoadEffectData();
	void LoadTextureData(IHLSLCodeGenerator * codeGen);
	TextureElement * FindTextureElement(TCHAR * name);

	IRenderMeshCache * GetMeshCache(){return mpMeshCache;}
	LPD3DXEFFECT GetEffect(){return NULL;}

	// From IEffectManager
	void SetFloatParam(D3DXHANDLE handle, TCHAR * name,TCHAR * uiName, float val,float min, float max, float step,MaxSemantics semantic );
	void SetIntParam(D3DXHANDLE handle, TCHAR * name, TCHAR * uiName,int val, int min, int max, int step,MaxSemantics semantic ){};
	void SetColorParam(D3DXHANDLE handle,TCHAR * name, TCHAR * uiName,D3DXVECTOR4 color, MaxSemantics semantic);
	void SetTextureParam(D3DXHANDLE handle, TCHAR * name,TCHAR * uiName, TCHAR * filename, MaxSemantics semantic, bool mappingEnabled, int mappingChannel);
	void SetLightParam(D3DXHANDLE handle,TCHAR * name,TCHAR * uiName, bool target,MaxSemantics semantic);
	void SetTransformParam(D3DXHANDLE handle, MaxSemantics semantic);
	void SetPoint4Param(D3DXHANDLE handle, TCHAR * paramName, TCHAR * uiName,D3DXVECTOR4 val,float min, float max, float step,MaxSemantics semantic){};
	void SetGeometryParam(D3DXHANDLE handle,MaxSemantics semantic){};
	void SetEnvironmentParam(D3DXHANDLE handle, MaxSemantics semantic){};
	void SetBooleanParam(D3DXHANDLE handle, TCHAR *paramName, TCHAR *uiName, BOOL val, MaxSemantics semantic){}
	void SetTechnique(D3DXHANDLE handle, TCHAR * techniqueName, bool bDefault);

	ILightingData * GetLightingData(D3DXHANDLE handle);

	// From IEffectFile
	bool SaveEffectFile(TCHAR * fileName);

	UINT GetDirectXVersion(){return IEffectManager::kDirectX9;}
	int GetNumberOfParams();
	virtual MaxSemantics GetParamSemantics(int index);
	const TCHAR * GetParamName(int index);
	bool GetParamData(LPVOID data, int index);
	bool GetParamData(LPVOID data, const TCHAR * paramName);
	int GetParamType(int index);
	int GetParamType(const TCHAR* paramName);
};

class EffectElements
{
	TSTR UIName;				//Named used for the Dialog resource
	TSTR ParameterName;			//The actual parameter name in the effect file
	D3DXHANDLE hParameter;		//A handle to the paramter
	int elementStyle;			//The type of element present
	bool valid;					//Is the element upto date
	MaxSemantics semantic;		//The elements semantic
	INode * renderNode;			//Used for Geometry parameters


public:

	enum{
		kEleSwatch,
		kEleSpin,
		kEleTex,
		kEleLight,
		kEleLightProp,	//this is a light element but it represent actual light properties not the node
		kEleTM,
		kEleGeom,
		kEleEnviron,
		kEleBool,
		kEleFloat,
		kElePoint4,
		kEleInt,
	};

	EffectElements();
	virtual ~EffectElements();

	void SetGenericData(D3DXHANDLE handle, int uiID, int DlgInt, TCHAR * UIName, TCHAR * paramName, LPDIRECT3DDEVICE9 dev);

	inline void SetUIName(TSTR &Name){UIName = Name;}
	inline TSTR& GetUIName(){return UIName;}
	inline void SetParameterName(TSTR &Name) {ParameterName = Name;}
	inline TSTR& GetParameterName(){return ParameterName;}
	inline void SetParamHandle(D3DXHANDLE Handle){hParameter = Handle;}
	inline D3DXHANDLE GetParamHandle(){return hParameter;}
	inline void SetStyle(int style){elementStyle = style;}
	virtual int GetStyle(){return elementStyle;}
	inline bool GetParamValid(){return valid;}
	//	virtual void SetParamValid(bool v){valid = v;}
	inline void SetSemantic(MaxSemantics s){semantic = s;}
	inline MaxSemantics GetSemantic(){return semantic;}
	inline void SetRenderNode(INode * node){renderNode = node;}
	inline INode* GetRenderNode(){return renderNode;}

	
	virtual bool SetValue(LPD3DXEFFECT pEffect, IParamBlock2 * pblock){return false;}
	virtual bool SetValue(LPD3DXEFFECT pEffect, StdMat2 * map){return false;}
	virtual bool GetValue(void * data, StdMat * map)=0;

};


class FloatElement : public EffectElements
{
	float defaultVal;
public:
	FloatElement();
	bool GetValue(void * data, StdMat * map);
	void GetParamValidity(IParamBlock2 * pblock, TimeValue t, Interval & valid){};

};


class LightElement : public EffectElements ,public ILightingData
{
	D3DXVECTOR4 defaultVec;
	INode * lightNode;
	int lightType;
	bool target;
	LightElement * parentElement;
	bool useLPRT;
	float lightIntens;
	LPDIRECT3DDEVICE9 pDevice;

public:

	enum{
		kUnassigned,
		kPoint,
		kTarget,
	};

	LightElement(bool target);

	inline void AddLight(INode *light){lightNode = light;}
	inline INode * GetLight() {return lightNode;}
	void DeleteLight(int index);
	void DeleteLight(ULONG handle);
	bool GetValue(void * data, StdMat * map){return false;}
	int GetLightType();
	inline INode * GetActiveLight(){return lightNode;}
	void SetDevice(LPDIRECT3DDEVICE9 dev){pDevice = dev;}


	//from LightingData
	D3DXVECTOR4 GetPosition();
	D3DXVECTOR4 GetDirection();
	D3DXVECTOR4 GetColor();
	float GetHotSpot();
	float GetFallOff();
	D3DXVECTOR4 GetAttenuation();


};


class TransformElement : public EffectElements
{
private:
	ID3D9GraphicsWindow * m_gw;
public:
	TransformElement(){};
	void SetGraphicsWindow(ID3D9GraphicsWindow*pGW){m_gw = pGW;}
	bool GetValue(void * data, StdMat * map);
	int GetStyle();

};

class TextureElement : public EffectElements
{
	TSTR mapName;
	TSTR fileName;
//	LPDIRECT3DTEXTURE9 pTex;
//	LPDIRECT3DTEXTURE9 pBumpTex;
	D3DXHANDLE texHandle;

public:

	LPDIRECT3DTEXTURE9 pTex;
	LPDIRECT3DTEXTURE9 pBumpTex;


public:
	enum{

		kTexture2D,
		kTextureCube,
		kTextureVolume,
		kTextureRenderTarget,
	};
	TextureElement(D3DXHANDLE handle,TCHAR * mapName, TCHAR * filename);
	~TextureElement();
	bool GetValue(LPVOID data, StdMat * map){return false;}
	inline TSTR GetMapName(){return mapName;}
	inline TSTR GetFileName(){return fileName;}
	inline LPDIRECT3DTEXTURE9 GetD3DTexture(){return pTex;}
	inline LPDIRECT3DTEXTURE9 GetD3DBumpTexture(){return pBumpTex;}

};

class ColorElement : public EffectElements
{
	AColor defaultVal;

public:
	ColorElement();
	bool GetValue(void * data, StdMat * map);
	void GetParamValidity(IParamBlock2 * pblock, TimeValue t, Interval & valid){};

};



inline bool DxStdMtl2::CanTryStrips()
{
	return true;
}

inline int DxStdMtl2::GetNumMultiPass()
{
	return 1;
}

inline HRESULT DxStdMtl2::SetVertexShader(ID3D9GraphicsWindow *d3dgw, int numPass)
{

	return S_OK;
}


inline bool DxStdMtl2::DrawWireMesh(ID3D9GraphicsWindow *d3dgw, WireMeshData *data)
{
	return false;
}

inline void DxStdMtl2::StartLines(ID3D9GraphicsWindow *d3dgw, WireMeshData *data)
{
}

inline void DxStdMtl2::AddLine(ID3D9GraphicsWindow *d3dgw, DWORD *vert, int vis)
{
}

inline bool DxStdMtl2::DrawLines(ID3D9GraphicsWindow *d3dgw)
{
	return false;
}

inline void DxStdMtl2::EndLines(ID3D9GraphicsWindow *d3dgw, GFX_ESCAPE_FN fn)
{
}

inline void DxStdMtl2::StartTriangles(ID3D9GraphicsWindow *d3dgw, MeshFaceData *data)
{
}

inline void DxStdMtl2::AddTriangle(ID3D9GraphicsWindow *d3dgw, DWORD index, int *edgeVis)
{
}

inline bool DxStdMtl2::DrawTriangles(ID3D9GraphicsWindow *d3dgw)
{
	return false;
}

inline void DxStdMtl2::EndTriangles(ID3D9GraphicsWindow *d3dgw, GFX_ESCAPE_FN fn)
{
}

#endif //~NO_ASHLI MSW 5/14/2004

#endif
