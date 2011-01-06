/**********************************************************************
*<
FILE: RTMax.h

DESCRIPTION:	Main Include for DirectX and MS Effect related code
				
CREATED BY:		Neil Hazzard

HISTORY:		February 2004

*>	Copyright (c) 2004, All Rights Reserved.
**********************************************************************/

#ifndef __RTMAX__H
#define __RTMAX__H

#include "maxheap.h"

#ifndef __D3DX9_H__
	#include <d3dx9.h>
#endif

#ifndef __JAGAPI__
	#include "max.h"
#endif

#ifndef __STDMAT__H
	#include "stdmat.h"
#endif

//!Some useful memory management macros
#define SAFE_DELETE(p)			{ if (p) { delete (p);		(p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p);	(p)=NULL; } }
#define SAFE_RELEASE(p)			{ if (p) { (p)->Release();	(p)=NULL; } }

//! Switch to compile between Dx9.0b and Dx9.0c
#define SUMMER_SDK



class ILightManager;
class IRenderMesh;
class IEffectParser;
class IParameterManager;

typedef enum MaxSemantics{
	kSemanticUnknown,
	kSemanticDirection,
	kSemanticPosition,
	kSemanticWorld,
	kSemanticView,
	kSemanticPerspective,
	kSemanticWorldView,
	kSemanticProjection,
	kSemanticProjectionI,
	kSemanticProjectionIT,
	kSemanticViewI,
	kSemanticWorldI,
	kSemanticWorldViewI,
	kSemanticWorldViewProj,
	kSemanticWorldViewIT,
	kSemanticViewIT,
	kSemanticWorldIT,
	kSemanticWorldViewT,
	kSemanticWorldT,
	kSemanticViewT,
	kSemanticProjectionT,
	kSemanticWorldCamPos,
	kSemanticLightColor,
	kSemanticNormalMap,
	kSemanticBumpMap,
	kSemanticDiffuseMap,
	kSemanticSpecularMap,
	kSemanticOpacityMap,
	kSemanticLightMap,
	kSemanticDisplacementMap,
	kSemanticGlobalTime,
	kSemanticGlobalBkgrd,
	kSemanticLocalBBMin,
	kSemanticLocalBBMax,
	kSemanticLocalBBSize,
	kSemanticRecipLocalBBSize,
	kSemanticBitmapOverride,
	kSemanticColorOverride,
	kSemanticViewportDimensions,
	kSemanticATan,
	kSemanticDiffuse,
	kSemanticAmbient,
	kSemanticSpecular,
	kSemanticSpecularLevel,
	kSemanticGlossLevel,
	kSemanticOpacity,
	kSemanticEmissive,
	kSemanticReflectionMap,
	kSemanticLightFallOff,
	kSemanticLightHotSpot,
	kSemanticDirectionLocal,
	kSemanticPositionLocal,
	kSemanticLightAttenuation,	/*!<See ILightingData::GetAttenuation for details*/

}MaxSemantics;

//! The %Effect Manager is responsible for displaying %effect parameters.  In max this is implemented by the DirectX 9 %Material.
/*! \brief All parameters except textures are handled by the %Effect Manager.  This includes setting the data on the render pass.  Textures
are controlled by the %Effect Parser, as the the parser understands the usage, the manager simply provided the resource name from the UI
It is the managers responsibility to make sure the data is set for each pass.  This may or may not mean providing a UI, but the methods
provide enough hints to be able to implement this.
*/
class IEffectManager: public MaxHeapOperators{

public:
	enum{
		kDirectX9,
		kDirectX10,
	};
	/*! Display a floating point value in the Dialog box
	\param handle The handle to the parameter in the %effect file
	\param paramName The name of the actual parameter.  This will be used in the paramblock
	\param uiName The name to be displayed in the Dialog box
	\param val The default value
	\param min The minimum value for the UI control
	\param max The maximum value for the UI control
	\param step The increment value for the UI control
	\param semantic The semantic of the parameter.  
	*/
	virtual void SetFloatParam(D3DXHANDLE handle, TCHAR * paramName, TCHAR * uiName, float val,float min, float max, float step,MaxSemantics semantic )=0;

	/*! Display an integer value in the Dialog box
	\param handle The handle to the parameter in the %effect file
	\param paramName The name of the actual parameter.  This will be used in the paramblock
	\param uiName The name to be displayed in the Dialog box
	\param val The default value
	\param min The minimum value for the UI control
	\param max The maximum value for the UI control
	\param step The increment value for the UI control
	\param semantic The semantic of the parameter.  
	*/
	virtual void SetIntParam(D3DXHANDLE handle, TCHAR * paramName, TCHAR * uiName,int val,int min, int max, int step,MaxSemantics semantic )=0;

	/*! Display a color value in the Dialog box.  This will be a standard max color swatch
	\param handle The handle to the parameter in the %effect file
	\param paramName The name of the actual parameter.  This will be used in the paramblock
	\param uiName The name to be displayed in the Dialog box
	\param color The default color to display
	\param semantic The semantic of the parameter.  
	*/
	virtual void SetColorParam(D3DXHANDLE handle, TCHAR * paramName, TCHAR * uiName,D3DXVECTOR4 color,MaxSemantics semantic)=0;

	/*! Display a D3DVECTOR4 value in the Dialog box.  This will be 4 max spinner UI elements
	\param handle The handle to the parameter in the %effect file
	\param paramName The name of the actual parameter.  This will be used in the paramblock
	\param uiName The name to be displayed in the Dialog box
	\param val The default value
	\param min The minimum value for the UI control
	\param max The maximum value for the UI control
	\param step The increment value for the UI control
	\param semantic The semantic of the parameter.  
	*/	
	virtual void SetPoint4Param(D3DXHANDLE handle, TCHAR * paramName, TCHAR * uiName,D3DXVECTOR4 val,float min, float max, float step,MaxSemantics semantic) = 0;

	/*! Display a boolean value in the Dialog box.  This will be a standard max check box
	\param handle The handle to the parameter in the %effect file
	\param paramName The name of the actual parameter.  This will be used in the paramblock
	\param uiName The name to be displayed in the Dialog box
	\param val The default color to display
	\param semantic The semantic of the parameter.  
	*/
	virtual void SetBooleanParam(D3DXHANDLE handle, TCHAR * paramName, TCHAR * uiName,BOOL val,MaxSemantics semantic) = 0;
	
	/*! Display a bitmap selection button
	\param handle The handle to the parameter in the %effect file
	\param paramName The name of the actual parameter.  This will be used in the paramblock
	\param uiName The name to be displayed in the Dialog box
	\param filename The default filename to display
	\param semantic The semantic of the parameter.  This is defined in the %effect file and is used for render to texture.
	\param mappingEnabled Specifies whether the mapping channel UI should be shown for the texture
	\param mappingChannel This provides the default mapping channel to use.
	*/
	virtual void SetTextureParam(D3DXHANDLE handle, TCHAR * paramName, TCHAR * uiName,TCHAR * filename, MaxSemantics semantic, bool mappingEnabled = false, int mappingChannel=1)=0;

	/*! Display a Light selection box, or provide access to lights based on the parameter
	\param handle The handle to the parameter in the %effect file
	\param paramName The name of the actual parameter.  This will be used in the paramblock
	\param uiName The name to be displayed in the Dialog box
	\param target Specifies if the light is a directional light.  This is used to filter the UI light list.
	\param semantic The semantic of the parameter.
	*/
	virtual void SetLightParam(D3DXHANDLE handle, TCHAR * paramName, TCHAR * uiName,bool target,MaxSemantics semantic) = 0;

	/*! Non UI Element used to set various transforms defined in the %effect file
	\param handle The handle to the parameter in the %effect file
	\param semantic The semantic of the transform to set.
	*/
	virtual void SetTransformParam(D3DXHANDLE handle,MaxSemantics semantic)=0;

	/*! Non UI Element used to set various geometry based data defined in the %effect file
	\param handle The handle to the parameter in the %effect file
	\param semantic The semantic of the transform to set.
	*/
	virtual void SetGeometryParam(D3DXHANDLE handle,MaxSemantics semantic)=0;

	/*! Non UI Element used to set various environmental based data defined in the %effect file
	\param handle The handle to the parameter in the %effect file
	\param semantic The semantic of the transform to set.
	*/

	virtual void SetEnvironmentParam(D3DXHANDLE handle,MaxSemantics semantic)=0;

	/*! Set the techniques used in the %effect file.  The default is used as the primary technique especially for multi pass effects
	\param handle The handle to the technique
	\param techniqueName The name to display in the UI
	\param bDefault The default technique to used
	*/
	virtual void SetTechnique(D3DXHANDLE handle, TCHAR * techniqueName, bool bDefault) =0;

	//! Get the current DirectX version
	/*!
	Currently this will ONLY return kDirectX9
	\return This will return kDirectX9.  In the future this could also be kDirectX10
	*/
	virtual UINT GetDirectXVersion()=0;

};
//!Implemented by %Effect Parser.
/*! %Effect parsers will implement this interface in order to actually render the %effect
*/
class IEffectParser: public MaxHeapOperators
{
public:
	/*! Parse the actual %effect file.  All data that is needed to actually render the %effect is extracted here.  If a UI is 
	needed, then the IEffectManager interface can be used for this purpose
	\param pDevice The main D3D device
	\param *filename The name of the actual file the represents the %effect file
	\param *em A pointer to the %Effect Manager
	\return true/false depending on whether the parsing was successful
	*/
	virtual bool ParseEffectFile(LPVOID pDevice, TCHAR * filename, IEffectManager * em)=0;

	/*! The UI code will use this to make sure the %effect has all the textures loaded and applied to the %effect when they change
	from the UI
	\param pDevice The main D3D device
	\param paramName The handle of the texture parameter from the %effect file
	\param *filename The name of the texture to load
	\param forceReload If true don't pull the texture from a cache, you need to load from disk
	\return true/false depending on whether loading was successful
	*/
	virtual bool LoadTexture(LPVOID pDevice, TCHAR * paramName, TCHAR * filename, bool forceReload)=0;

	/*! Remove the parser*/
	virtual void DestroyParser()=0;

	/*! Any initialisation or one off setup, before the render passes happens here.  If new rendertargets are needed or back buffers, this is the 
	place to do it
	\param pDevice The main D3D device
	\param *rmesh  The render %mesh that needs to be drawn
	\param *lm A pointer to the light manager
	\param bkgColor The color of the background to clear to
	\param width The width of the viewport
	\param height The height of the viewport
	\return True/False
	*/
	virtual bool PreRender(LPVOID pDevice, IRenderMesh * rmesh, ILightManager * lm, IParameterManager * pm, D3DCOLOR bkgColor, int width, int height ) = 0;

	/*! Render the actual object, based on the data supplied in the %effect file
	\param pDevice The main D3D device
		\param *rmesh  The render %mesh that needs to be drawn
	\param primaryTechnique The actual technique that should be used for the final result.
	*/
	virtual bool Render(LPVOID pDevice, IRenderMesh * rmesh, D3DXHANDLE primaryTechnique)=0;

	/*! Alows the user to change the mapping channel used by the texture in the UI
	\param handle The handle of the parameter to set
	\param Channel The value of the mapping channel
	*/
	virtual void SetMappingChannel(D3DXHANDLE handle, int Channel)=0;

	//! Used internally
	virtual void SetUseLPRT(bool state){};

	//! Load a shader based on the filename supplied.  
	/*!The parser must keep a pointer to this effect, and manage any resources associated with it
	\param pDevice The DirectX device
	\param em A pointer to the IEffectManager instance
	\param effect A string containing the effect - this can either be a filename or a buffer contaning the effect
	\param fileType Defines whether the string contains a file name or a buffer
	\param forceReload If true don't pull the effect from a cache, you need to reload from disk
	\return True if successful
	*/
	virtual bool LoadEffect(LPVOID pDevice, IEffectManager * em, const TCHAR * effect, bool fileType, bool forceReload) =0;

	//! retrieves the last error message maintained by the parser
	virtual const TCHAR * GetLastParserError() =0;

	//!virtual destructor
	virtual ~IEffectParser() {}

	
//! Allow the EffectManager access to lighting data, so it can expose it through the public interfaces
/*! The EffectManager used to maintain all the lights, now it simply host the UI.  This means the parser needs to keep tabs
on the lights, or at least what parameters of the lights are needed.  This is simple for the parser, as it will need to keep a local
list of the light data so it can request the actual max data from the LightManager.
*/
	//!The number of light based parameters the %effect contains
	virtual int GetNumberOfLightParameters() = 0;

	//! The light parameter usage
	virtual MaxSemantics GetLightSemantic(int index)=0;

	//! Get a handle to the light based on the index supplied
	virtual const TCHAR * GetLightHandle(int index)=0;

	//! Called when a Lost Device is detected - free any resources here
	virtual void OnLostDevice() = 0;

	//! Called when a ResetDevice is detected - free and rebuild any resources here.
	virtual void OnResetDevice() = 0;

	//! Provides the ability to load a texture created out side the parser.
	/*!
	\param pTexture A pointer to DirectX texture resource
	\paramName The name of the parameter who should receive the texture
	\return True/False depending on whether the texture was uploaded correctly
	*/
	virtual bool LoadTexture(LPVOID pTexture, const TCHAR* paramName) =0;

};

//! Access Lighting data from 3ds max lights
/*! This is the main access to the lights.  The user would request the light data from the LightManager based on the D3DHANDLE
*/
class ILightingData: public MaxHeapOperators{
public:
	/*! Get the Position of the light
	\return A directX vector containing the position
	*/
	virtual D3DXVECTOR4 GetPosition()=0;

	/*! Get the current direction of the light
	\return A directX vector containing the direction
	*/
	virtual D3DXVECTOR4 GetDirection()=0;

	/*! Get the current color
	\return A DirectX color value.
	*/
	virtual D3DXVECTOR4 GetColor()=0;

	/*! Get the current falloff value
	\return A float representing the value.
	*/
	virtual float GetFallOff()=0;

	/*! Get the current Hotspot value
	\return A float representing the value.
	*/
	virtual float GetHotSpot()=0;

	//! Get Attenuation data from active light
	/*! The attenuation from a max light is returned using this method.  The D3DXVECTOR4 is used in the following was
	x = near Attenuation start
	y = near Attenuation snd
	z = far Attenuation start
	w = far Attenuation end
	This can be used in conjunction with GetFallOff and GetHotSpot to create very realistic lighting results
	\return The attenuation data for the light queried.
	*/
	virtual D3DXVECTOR4 GetAttenuation() =0;

};

//! A basic light container
/*! The user would use this to access the light data for a specific light.  The system maintains the light list so the user
does not need to worry about the maintainence.
*/
class ILightManager: public MaxHeapOperators{

public:
	//! Access the Light Data
	/*! Access the light data based on the handle supplied.  The handle is based on the parameter found in the Effect File
	\param handle The handle of the light to access
	\return A pointer to the light data
	*/
	virtual ILightingData * GetLightingData(const TCHAR * paramName)=0;

};
//! A data management class for accessing data required for displaying real time effects.
/*! An instance of this class is maintained by the DirectX 9 Shader material 
and is passed into the IEffectParser::PreRender() method.  This allows the Effect Parsers to format the data accordingly based
on the runtime active for the shader
*/
class IParameterManager: public MaxHeapOperators{
public:
	enum{
		kPType_Unknown,
		kPType_Float,
		kPType_Int,
		kPType_Bool,
		kPType_Point4,
		kPType_Matrix,
		kPType_Color,
	};

	//! The total number of parameters maintained by the manager
	/*!
	\return The number of parameters
	*/
	virtual int GetNumberOfParams()=0;

	//! The semantic for the parameter
	/*! This will be the semantic usage found in the shader file when it is parsed by the system
	\param index The index of the parameter to query
	\return An item from MaxSemantics representing the semantic found in the file
	*/
	virtual MaxSemantics GetParamSemantics(int index)=0;

	//! Retireve the name of the parameter
	/*! This will be the name of the parameter found in the shader file
	\param index The index of the parameter to query
	\return A string containing the name
	*/
	virtual const TCHAR * GetParamName(int index)=0;

	//! Access to the actual data stored by the manager
	/*! This is the main access point for data retrieval.  It is the calling code's responsibility to make sure there is enough
	space allocated to recieve the data.  This can be determined by calling GetParamType, which will return the data type.  If it is 
	a kPtype_Matrix, then internally this is stored as an 4x4 float array
	\param data A pointer to a buffer to receive the data
	\index THe index of the parameter to query
	\return True/False depending if the parameter was found, and the data copied correctly
	*/
	virtual bool GetParamData(LPVOID data, int index)=0;
	
	//! Access to the actual data stored by the manager
	/*! This is an access point for data retrieval.  It is the calling code's responsibility to make sure there is enough
	space allocated to recieve the data.  This can be determined by calling GetParamType, which will return the data type.  If it is 
	a kPtype_Matrix, then internally this is stored as an 4x4 float array
	\param data A pointer to a buffer to receive the data
	\paramName The name of the parameter to query
	\return True/False depending if the parameter was found, and the data copied correctly
	*/
	virtual bool GetParamData(LPVOID data, const TCHAR * paramName)=0;

	//! Access the data type of the parameter
	/*! This method returns the type of parameter.  This will be one of the enumerated types define in 
	IParameterManager, kPType_Matrix etc..
	\param index The index of the parameter to query
	\return The parameter type
	*/
	virtual int GetParamType(int index) = 0;

	//! Access the data type of the parameter
	/*! This method returns the type of parameter.  This will be one of the enumerated types define in 
	IParameterManager, kPType_Matrix etc..
	\param paramName The name of the parameter to query
	\return The parameter type
	*/
	virtual int GetParamType(const TCHAR * parmName) =0;
};



#include "iFnPub.h"

#define PARSER_LOADER_INTERFACE Interface_ID(0x7fbe42d6, 0x3e7f412b)

//! A mini Max class descriptor.  This is purely to instantiate multiple objects, and query the Parser ID for the DLL
class EffectDescriptor: public MaxHeapOperators
{
public:
	//! Called by the system to create an instance of the parser
	/*!
	\return A valid pointer to the parser
	*/
	virtual IEffectParser * CreateParser() = 0;

	//! The unique ID that is used in the %effect file.  
	/*! The system will look at the %effect file and then will get the ID.  it will then use this ID to query for a parser
	\return The ID of this parser
	*/
	virtual ULONG GetParserID()=0;

	//! The string to go along with the extension supported to be diplayed in the Open Dilaog box
	/*! This should be in the form "Microsoft Direct3D Effects"
	*/
	virtual const TCHAR * GetParserFileFilterName()=0;

	//! Get the support file extension for the parser.
	/*! The system will filter "like" extensions.  This is only used for the open dilaog box.  The ParamID is still
	used to define the actual parser to use.  THe string should be in the form "*.fx"
	*/
	virtual const TCHAR * GetParserFileExtension()=0;

};

//! A core interface used to store parser DLLs found in the system
class IParserLoader : public FPStaticInterface
{
public:

	/*!
	The total number of parsers found at startup
	\return The number of parsers
	*/
	virtual int	GetNumberOfParsers()=0;

	//! Get a parser based on parser ID
	/*!
	\param parserID The ID of the prser you are requesting
	\return An instance of the parser
	*/
	virtual IEffectParser * GetParser(ULONG parserID) =0;

	//! used by the system to load the parser DLLs from disk
	virtual void LoadDLLs()=0;

	//! Used by the system to unload the DLLs
	virtual void UnloadDLLs()=0;

	//! Get the filter name.  THis will be used in the open dilaog boxes.
	/*!
	\param index The index to the parser.  Use GetNumberOfParsers to find the total number in the system
	\return A string with the extension.  It will be in the form "Microsoft Direct3D Effects"
	*/
	virtual const TCHAR * GetParserFilter(int index)=0;

	//! Get the file extension for the parser. THis will be used in the open dilaog boxes.
	/*!
	\param index The index to the parser.  Use GetNumberOfParsers to find the total number in the system
	\return A string with the extension.  It will be in the form *.fx
	*/
	virtual const TCHAR * GetParserExtension(int index) =0;
};

inline IParserLoader* GetParserLoader() { return (IParserLoader*)GetCOREInterface(PARSER_LOADER_INTERFACE); }

//! Maintains a Dx version of the current mesh in the cache.  It will render either the max mesh or a screen aligned quad
class IRenderMesh: public MaxHeapOperators
{

public:

	enum MeshType{
		kMesh,
		kScreenQuad,
	};

	//! Exported function to access the IRenderMesh interface
	/*
	\returns A pointer to the IRenderMesh interface
	*/
	DllExport static IRenderMesh * GetRenderMesh();

	//! Defines what the mesh represents
	/*!
	\param type specify whether it is a mesh or a full screen quad
	*/
	virtual void SetMeshType(MeshType type)=0;

	//! Used to update the Quad version of the mesh
	/*!
	\param *Device A Pointer to the main DX interface
	\param width The width of the render target
	\param height The height of the render target
	\param texWidth The width of the texture being used
	\param texHeight The height of the texture being used
	\return TRUE if successful
	*/
	virtual bool Evaluate(LPVOID Device, int width, int height, int texWidth, int texHeight)=0;

	//! Setting the node parameter to non NULL, will make sure the Normals are transformed correctly
	/*!
	\param *Device A Pointer to the main DX interface
	\param *aMesh The mesh whose data is to be extracted
	\param MatIndex The material index for the faces to be rendered.  This provides support for multi subobject materials
	\param NegScale Used to determine if negative scale is being used on the object.
	\return TRUE if successful
	*/
	virtual bool Evaluate(LPVOID Device, Mesh *aMesh, int MatIndex, bool NegScale)=0;

	//! Store relevant mapping data
	/*! Allows the app to provide upto 8 mapping channels, this is used to extract the texcoord data from the mesh
	\param  map A tab of active mapping channels
	*/
	virtual void SetMappingData(Tab <int> &map)=0;

	//! Renders the current object
	/*!
	\param *Device A Pointer to the main DX interface
	\return TRUE if successful
	*/
	virtual bool Render(LPVOID Device)=0;

	/*! force an invalidation of the internal data structures
	*/
	virtual void Invalidate()=0;

	//! Get the current Vertex format for the mesh
	/*! This retrieves the current active vertex format for the mesh. The return value will be false if DirectX9 is running
	or true and an itemCount >0 if  DirectX 10 is running. 
	\param **pFormat A pointer to vertex descriptor - this will be different for Dx9 and Dx10
	\param * itemCount The number of items in the vertex descriptor
	\return TRUE if successful.  In Dx9 and 3ds max 9 this will be false.
	*/
	virtual bool GetVertexFormat(void ** pFormat, int * itemCount)=0;
};


//! This class maintains a list of the meshes and RenderMeshes used per instance.  It acts as a factory class
/*! \brief This is useful for Materials where a single material could be applied to multiple nodes, so a cache is needed so the material
can pull the correct mesh data
*/
class IRenderMeshCache: public MaxHeapOperators{

public:
	//! Exported function to access the IRenderMeshCache interface
	/*
	\returns A pointer to the IRenderMeshCache interface
	*/
	DllExport static  IRenderMeshCache *  GetRenderMeshCache();

	//! Set the type of the mesh cache to maintain;
	/*!
	\param type Set the type of object to hold.  This can be either a mesh or a quad.
	*/
	virtual void SetMeshType(IRenderMesh::MeshType type)=0;

	//! Fill up the cache with data from max.
	/*!
	This method needs to be called when the node has actually changed.  In max 8 and before you could call this method
	at any time and the method would attempt to cache and maintain the data.  However this would not work correctly with
	a MNMesh type object.  Now the method expects to only update itself when the object changes.  This is easily achieved 
	by only calling this method when IStdDualVSCallback::InitValid() is called.  To simply get the cache position for the node
	you can call this method with a NULL mnMesh pointer.

	\param *mnMesh The MNMesh to store in the cache.  If this is NULL the system will return the cache entry only
	\param *node The node to store in the cache
	\param time The time to use in any evaluations
	\param &meshChanged If the mesh is different to that in the cache, this specifies the case.
	\return The position in the cache of the mesh/node combo
	*/
	virtual int SetCachedMNMesh (MNMesh *mnMesh, INode *node, TimeValue time, bool & meshChanged)=0;

	//! Fill up the cache with data from max.
	/*!
	This method needs to be called when the node has actually changed.  In max 8 and before you could call this method
	at any time and the method would attempt to cache and maintain the data.  However this would not work correctly with
	a MNMesh type object.  Now the method expects to only update itself when the object changes.  This is easily achieved 
	by only calling this method when IStdDualVSCallback::InitValid() is called.  To simply get the cache position for the node
	you can call this method with a NULL mesh pointer.

	\param *mesh The mesh to store in the cache. If this is NULL the system will return the cache entry only
	\param *node The node to store in the cache
	\param time The time to use in any evaluations
	\param &meshChanged If the mesh is different to that in the cache, this specifies the case.
	\return The position in the cache of the mesh/node combo
	*/
	virtual int SetCachedMesh (Mesh *mesh, INode *node, TimeValue time, bool & meshChanged)=0;

	//! Get the number of cached mesh
	/*! 
	\return The number of cached mesh
	*/
	virtual int GetCacheSize()=0;

	//! Get the active render mesh
	/*!
	\param index The index of the mesh to retrieve
	\return The IRenderMesh based on the index provided
	*/
	virtual IRenderMesh * GetActiveRenderMesh(int index)=0;

	/*!
	\param index Get the node for the current cached object
	\return The INode for the current cache
	*/
	virtual INode * GetActiveNode(int index)=0;

	/*!
	\param index Get the mesh for the current cached object
	\return The INode for the current cache
	*/
	virtual Mesh * GetActiveMesh(int index)=0;

	virtual void SetDirectXVersion(UINT version)=0;

	//! default destructor
	virtual ~IRenderMeshCache() {}

	//! Used to release any data stored by the cache
	virtual void Release()=0;

	//! Used to delete the cache for a specific node
	virtual void DeleteRenderMeshCache(INode * node)=0;

};

//! A factory class, used to create HLSL code based on the material passed in.
class IHLSLCodeGenerator: public MaxHeapOperators{

public:
	enum CodeVersion {
		PS_1_X,
		PS_2_0,
		PS_3_0,
		PS_2_X,
	};

	//! Access to the factory class
	DllExport static IHLSLCodeGenerator * GetHLSLCodeGenerator();

	//!Generate HLSL Code based on the material
	/*! This will create HLSL code based on the material and lights in the scene.  It will generate code for Blinn and ONB style
	specular component.  It will support Ambient,Diffuse, specular, Self Illumination, opacity, bump, relfection maps.
	\param *mtl The material source
	\param lights A table of lights to used in the HLSL generation
	\param &transp True if there is a transparent component
	\returns A string containing the code.
	*/
	virtual TCHAR * GenerateFragmentCode(StdMat2 * mtl, Tab <INode *> lights, bool & transp) = 0;

	//! Generates a basic Vertex shader to support the pixel shader
	/*\
	\return A string containing the code
	*/
	virtual TCHAR * GenerateVertexCode()=0;

	//! Generates an Effect File based on the material supplied.
	/*! This is similar to GenerateFragmentCode, but will optimize the HLSL code to assembler, based on a pass compiler.
	The effect file generated will only represent what is currently seen in the viewport
	\param *mtl The material that wil be used to generate the %effect
	\param lights An array of lights to be used by the shader
	\param code The Shader Model to compile to
	\param &transp  Tells the code if transparency is used.  This can be used to setup specific RenderStates
	\param instSize The maximum number of instructions
	\param useLPRT This is not needed by the developer.  It is used in conjunction with Ashli
	\return A string containing the %effect file.  This can be loaded in the DirectX 9 Shader material
	*/
	virtual TCHAR * GenerateEffectFile(StdMat2 * mtl,Tab <INode *> lights,CodeVersion code, bool & transp, int instSize, bool useLPRT = false) =0;

	//! Get the texture map used by the specified mapName
	/*! When the material tree is processed, a texture map can provide its only HLSL code and texture samplers.  This function
	will return the texmap of these maps and the default diffuse, bump etc..
	\param *mat The standard material whose textures are to be queried
	\param *mapName This is the name parsed out in the %effect file
	\param &bump Allows special handling of the bump map
	\returns A pointer to the specified texture map
	*/
	virtual Texmap* GetShaderDefinedTexmap(StdMat2 * mat, TCHAR * mapName, bool & bump)=0;

	virtual ~IHLSLCodeGenerator() {}
	virtual void Release() = 0;
};

class IHLSLCodeGenerator2 : public IHLSLCodeGenerator
{
public:
	//! Generates an Effect File based on the material supplied.
	/*! This is similar to GenerateFragmentCode, but will optimize the HLSL code to assembler, based on a pass compiler.
	This version of the function will generate code for all maps whether they are visible in the viewport or not.
	\param *mtl The material that wil be used to generate the %effect
	\param lights An array of lights to be used by the shader
	\param code The Shader Model to compile to
	\param &transp  Tells the code if transparency is used.  This can be used to setup specific RenderStates
	\param instSize The maximum number of instructions
	\param useLPRT This is not needed by the developer.  It is used in conjunction with Ashli
	\return A string containing the %effect file.  This can be loaded in the DirectX 9 Shader material
	*/
	virtual TCHAR * GenerateEffectFile2(StdMat2 * mtl,Tab <INode *> lights,CodeVersion code, bool & transp, int instSize, bool useLPRT = false) =0;

	//!Generate HLSL Code based on the material
	/*! This will create HLSL code based on the material and lights in the scene.  It will generate code for Blinn and ONB style
	specular component.  It will support Ambient,Diffuse, specular, Self Illumination, opacity, bump, relfection maps.
	This version of the function will generate code for all maps whether they are visible in the viewport or not.
	\param *mtl The material source
	\param lights A table of lights to used in the HLSL generation
	\param &transp True if there is a transparent component
	\returns A string containing the code.
	*/
	virtual TCHAR * GenerateFragmentCode2(StdMat2 * mtl, Tab <INode *> lights, bool & transp) = 0;
	virtual ~IHLSLCodeGenerator2() {}

	//! Access to the factory class
	DllExport static IHLSLCodeGenerator2 * GetHLSLCodeGenerator2();

};


#define HLSL_TEXTURE_INTERFACE Interface_ID(0xc2b2185, 0x635556bf)

class TextureData: public MaxHeapOperators{
public:
	//! UI Name to appear in the Effect file
	TCHAR UIName[25];

	//! Sampler name is used as the filename for the texture, in the %effect file.  It is also used as a lookup for the texmap
	TCHAR SamplerName[25];

	//! The sub map number the texture refers to - the calling code will use texmap->GetSubTexmap(SubMapNum) to access it.
	int SubMapNum;
};

typedef Tab < TextureData> TexDataList;

class IHLSLTexmap : public BaseInterface
{
public:
	// Interface Lifetime
	virtual LifetimeType	LifetimeControl() { return noRelease; }
	virtual Interface_ID	GetID() { return HLSL_TEXTURE_INTERFACE; }

	//! Specifies that the Texmap will provide HLSL code for the Dx Standard Material
	/*! 
	\return True if the Texmap supports HLSL code generation.
	*/
	virtual bool DoesSupportHLSL(){return false;}
	
	//! Allow texture to provide an hlsl representation of the texture.
	/*! 
	\param *code A pointer to a buffer to store the HLSL code
	\param *entryPoint A pointer to a buffer to store the name of the function
	*/
	virtual void GetTexmapHLSLFunction(TCHAR * code, TCHAR * entryPoint){};

	//! Provide access to any textures used in the HLSL code
	/*!
	This is used to store data to allow the HLSL compiler to find the sub maps used by the texture map.
	This also included a UI name for the %effect file generation
	\param *list A pointer to a TexDataList that is used to store texture info
	*/
	virtual void GetTextureData(TexDataList * list ){};

};

#define EFFECT_FILE_INTERFACE Interface_ID(0xc3d2425, 0x637366bc)

class IEffectFile : public BaseInterface
{
public:
	// Interface Lifetime
	virtual Interface_ID	GetID() { return EFFECT_FILE_INTERFACE; }

	//! Save the current material as a D3D compliant %Effect file
	/*! This will save the %effect file, along with all textures to the directory specified.  The texture
	will be saved as DDS files.
	\param *fileName The name of the file to save
	\return TRUE if successful
	*/
	virtual bool SaveEffectFile(TCHAR * fileName)=0;

};

/*! A simple class that a developer must implement if they want to use Scene traversal
*/
class IValidateNodeCallback: public MaxHeapOperators
{
public:
	/*! A simple test to define whether the node is accepted or rejected.
	\param *node The node to test
	\param *vpt The active view
	\return Whether the node is valid or not.
	*/
	virtual bool NodeValid(INode * node, ViewExp * vpt)=0;

	/*! This allows a sort of the nodes accepted.  This needs to be quick as it is called during the render process.
	\ param & nodeList - the list to sort.
	*/
	virtual void NodeSort(Tab <INode *> & nodeList){};
};

/*!\brief An interface to allow access to scene transforms.  This is important as depending on the pass
different transforms may be required.  Using this system makes sure the correct transform is used.
*/
class IDxSceneTransformManager: public MaxHeapOperators
{
public:
	enum TransformType{
		kWorldXform,
		kViewXform,
		kProjectionXform
	};
	//! Get the current matrix
	/*!
	Get the Scene based Transforms such as %View and Projection, this shouldn't be used for world matrix info
	\param matType The matrix to recieve
	\return  A D3D Matrix
	*/
	virtual D3DXMATRIX GetD3DMatrix(TransformType matType)=0;

	//! Set the current Matrix
	/*!
	Set the Projection and %View Matrix for the parameter supplied.  This currently only supports lights
	\param paramName A pointer to a string containing the name as defined in the %effect file.
	*/
	virtual void SetD3DMatrix(TCHAR * paramName)=0;

	//!Set the currentMatrix
	/*!
	Allows a developer to store Projection and %View matrix data.  This will be typically used to store %effect specific
	matrices used in environmental rendering.  An example would be cubemap generation.  The developer needs to make sure the
	data is up to date for when the shaders require this.  Ideally this would set in a PreRender method.
	\param mat The matrix to store
	\param matType The type of matrix to store
	*/
	virtual void SetD3DMatrix(D3DXMATRIX mat, TransformType matType)=0;

	//! Get the current matric
	/*! Returns the matrix for the specified parameter
	\param paramName A pointer to a string containing the name as defined in the %effect file
	\param matType The transform to obtain
	\return A D3D Matrix
	*/
	virtual D3DXMATRIX GetD3DMatrix(TCHAR * paramName, TransformType matType) = 0;


};

//! A system to manage the current state of the Dx Scene and render passes.
/*! This will provide support for RTT scene based %effects, as well as handling matrix conversions.
*/
class IDxSceneManager: public MaxHeapOperators
{
public:
	/*! Describes the %effect in use*/
	enum DXEffectType{
		kPreEffect, /*!<Pre %Effect - drawn behind the geometry*/
		kPostEffect,/*!<Post %Effect - drawn on top as a screen quad*/
		kEnvEffect /*!<Environment %effect, used to create shadow buffers etc..*/
	};

	/*! Describes what the current pass is doing, standard rendering to the viewport, or for example creating 
	textures for use later in the render process
	*/
	enum DXPassType{
		kStandard, /*!<General scene rendering*/
		kEnvironment, /*!<Texture generation*/
		kEnvironmentLast,	/*!<Final environment render call in current environment pass*/
	};

	/*! Add a scene effect to the database
	\param *dxMtl A pointer to a DirectX %Shader %Material
	\param effectType Describes what type of effect is being stored
	\param numberOfPasses How many passes are required.  This is only really useful for environment %effects
	*/
	virtual void StoreSceneEffect(Mtl * dxMtl, DXEffectType effectType = kPostEffect, int numberOfPasses=0)=0;

	/*!Add a scene %effect to the database
	\param &dxMtl A table of effects.  This would be used to copy a complete block over.  This is
	useful when setting multi effects, or the ordering has changed.
	*/
	virtual void StoreSceneEffect(Tab<Mtl*> &dxMtl)=0;

	/*!Number of Scene effects active
	\param effectType The type of %effect to query
	\return The  number of scene effects
	*/
	virtual int GetNumberOfSceneEffects(DXEffectType effectType = kPostEffect) = 0;

	/*! Get the scene %effect 
	\param index The index of the material to access
	\param effectType The type of %effect to query
	\return The material at the index
	*/
	virtual Mtl * GetSceneEffect(int index, DXEffectType effectType = kPostEffect) = 0;

	//! flush out the effects, and reset all Render Targets
	virtual void ResetSceneEffects()=0;

	//! Remove specified %Effect
	/*!
	\param index The index of the %effect to remove
	\param effectType The type of %effect
	*/
	virtual void RemoveSceneEffect(int index, DXEffectType effectType = kPostEffect) =0;
	
	/*!Called at the beginning of the Scene Traversal - All effects PreRenders are called here.
	\param *gw  The current viewport being rendered
	*/
	virtual void StartScene(GraphicsWindow *gw )=0;

	
	/*!Called at the end of traversal.  The main effects Render method is called here.
	\param *gw  The current viewport being rendered
	*/
	virtual void EndScene(GraphicsWindow *gw ) = 0;

	//! Get access to the scene Render Target.  This will change depending on how many effects are loaded
	/*!
	This is used to access any rendertargets that are either the result of a full render, or can be stored 
	by the parsers.  Only Environmental effects can store render targets, and they can then be looked up by name
	\param env Defines whether the requested Render Target is the result of an environmental %effect
	\param *paramName The name of the Render Target to access
	\return The render target
	*/
	virtual LPDIRECT3DBASETEXTURE9 GetSceneRenderTarget(bool env = false, TCHAR * paramName = NULL)=0;
	
	//! Set the environment texture for use by an object based effect.  The paramName needs to be consistant.
	/*!
	\param texture The Render Target to store
	\param *paramName The name to be used to store the render target.  This will be used in GetSceneRenderTarget
	*/
	virtual void SetSceneRenderTarget(LPDIRECT3DBASETEXTURE9 texture, TCHAR * paramName)=0;

	//! Special Debugging mode - textures will be saved for each pass/combiner - USE WITH CAUTION !!
	/*! The various passes will be saved as DDS file to the specified location - they will be titled XXX_Pre_pass
	and XXX_post_pass - the final rendered pass is not saved, as this is available in the viewport.  The XX is the name prefix supplied
	\param onOff Turn debug on/off
	\param *outputPath A pointer to a string containing the path, ending with "\"
	\param *namePrefix A pointer to a string containing a name prefix
	*/
	virtual void EnableDebugMode(bool onOff, TCHAR * outputPath, TCHAR * namePrefix) = 0;

	/*!Turn on the traversal system.  When activated the developer can control what is rendered to the viewport
	\sa IValidateNodeCallback
	\param state a boolean used to turn the travwersal on/off
	\param *proc A callback used to validate and sort nodes 
	*/
	virtual void EnableSceneTraversal(bool state, IValidateNodeCallback * proc)=0;
	
	//! access to the Transform manager
	/*! This provides scene level access to the various DX transforms.  When rendering scene based effects these transforms should be used
	It is safe when drawing object level effect to still use this, but only for PROJECTION and VIEW.  World should be obtained from the regular methods.
	\sa IDsSceneTransformManager
	\returns A pointer to IDxSceneTransformManager
	*/
	virtual IDxSceneTransformManager * GetTransformManager() = 0;

	/*! A one time per scene initialisation that must be called first before the scene is rendered
	\param *gw The viewport being rendered
	*/
	virtual void SceneInitialize(GraphicsWindow * gw) =0;

	/*! The number of passes required to render the scene based on the effects loaded
	\return the total number of passes
	*/
	virtual int GetNumberOfPasses()=0;

	/*! The value of the current pass
	\return The current pass number
	*/
	virtual int GetCurrentPassNumber()=0;

	/*! Provides access to the type of pass being rendered.  This allows developers to pre process or switch techniques based on whether
	environmental effects are being calculated or actual scene drawing is taking place
	\return The active pass
	*/
	virtual IDxSceneManager::DXPassType GetActivePassType()=0;

	//! Specifies whether the current scene effects are valid or not.
	/*! The system will check the validity channel of the post effects to determine whether a full scene update is required.  This is mainly
	for effects that require a TIME semantic, which denotes that it constantly runs and updates the scene.
	\return True if the effects are valid and require no scene update
	*/
	virtual bool IsSceneEffectValid() = 0;



	//! Access to the singleton
	DllExport static IDxSceneManager * GetIDxSceneManager();
};

struct DxGeneralFunctions{
	DllExport  static TCHAR * GetDXResourceFilename(TCHAR * fileName);
};


#endif
