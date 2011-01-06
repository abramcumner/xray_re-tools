/**********************************************************************
 *<
	FILE: IViewportManager.h

	DESCRIPTION:	Viewport Manager for loading up DXEffects

	CREATED BY:		Neil Hazzard

	HISTORY:		Created:  02/15/02
					Updated:  05/05/04	New interface added for 3ds max7
					

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/

#ifndef	__IVIEWPORTMANAGER_H__
#define __IVIEWPORTMANAGER_H__

#include "iFnPub.h"
#include "custattrib.h"
#include "IHardwareMaterial.h"

#define VIEWPORT_SHADER_MANAGER_INTERFACE Interface_ID(0x5dbe33d6, 0x2e1b422b)
#define VIEWPORT_SHADER_MANAGER_INTERFACE2 Interface_ID(0x5a4d6672, 0x7d8a2857)

#define VIEWPORT_SHADER_CLIENT_INTERFACE Interface_ID(0x40c926b8, 0x7c3a66b7)
#define VIEWPORT_SHADER9_CLIENT_INTERFACE Interface_ID(0x40c926b7, 0x7c3a6347)

#define IDX_SHADER_MANAGER Interface_ID(0x6dce7429, 0x200169ac)

//! An Interface class to access the Viewport Manager
/*! This Interface allows access to the various components of the Viewport Manager.  THis includes finding out
how many %effects are registered with the system
*/
class IViewportShaderManager : public FPMixinInterface {
	public:

		enum {	get_num_effects, get_active_effect,	is_effect_active, is_manager_active,
		get_effect_name,set_effect,activate_effect};

		FPInterfaceDesc* GetDesc();    

		/*! Get the number of effects registered with the system
		\return The total number of effects
		*/
		virtual int GetNumEffects()=0;

		/*! Get the current active effect.  This effect will implement the IDXDataBridge classes
		\return A referenceTarget implementing the effect
		*/
		virtual ReferenceTarget* GetActiveEffect()=0;

		/*! Access the UI name for the effect
		\param i An index of the effect whose name is required
		\return A TCHAR containing the name
		*/
		virtual TCHAR * GetEffectName(int i)=0;

		/*! Set the actual viewport effect to be used.
		\param i An index of the effect to set.
		\return A pointer to the effect
		*/
		virtual ReferenceTarget * SetViewportEffect(int i)=0;

		/*! Turns on the effect based on the material and state supplied.  THis sets the internal flag on the material
		that tells the system that a DX shader is present
		\param *mtl THe material to activate
		\param State A boolean value that defines the state of the flag
		*/
		virtual void ActivateEffect(MtlBase * mtl, BOOL State)=0;


	};
//! A new interface for 3dsmax 7
/*! This extends the current interface, and provides support for the new UI options on the Viewport Manager
*/
class IViewportShaderManager2 : public IViewportShaderManager
{
public:
	//! Save the current effect based on the Standard Material as an effect file
	/*!
	\param *fileName The name of the file to save
	\return  The success of the operation
	*/
	virtual bool SaveFXFile(TCHAR * fileName)=0;

	/*! Is the DX Display of the Standard Material enabled
	\return TRUE/FALSE
	*/
	virtual bool IsDxStdMtlEnabled()=0;

	/*! Access to set the state of the DX Display of the Standard Material flag
	\param state The state of the flag - TRUE to set
	*/
	virtual void SetDxStdMtlEnabled(bool state)=0;

	/*! The name of the current effect as displayed in the UI
	\return The name of the effect
	*/
	virtual TCHAR * GetActiveEffectName() = 0;
};

//! Viewport DXEffect interface
/*! A Direct X 8.1 shader should implement this class.  It is used by the system to get information from the shader
If a material implements this class, then all it needs to do is pass back a pointer in the GetInterface implementation.  
All the other methods can simply be a no op.
*/
class IDXDataBridge : public BaseInterface
{
public:
	virtual Interface_ID	GetID() { return VIEWPORT_SHADER_CLIENT_INTERFACE; }

	// Interface Lifetime
	virtual LifetimeType	LifetimeControl() { return noRelease; }
	/*! The Viewport Manager will call this to display the dialog (if needed)
	\param hWnd The parent window handle
	\param *imp A pointer to IMtlParam - see the main max SDK for info on this class
	\return A new ParamDlg pointer which the Viewport Manager will uses as a child of its window
	*/
	virtual ParamDlg * CreateEffectDlg(HWND hWnd, IMtlParams * imp)= 0;

	//! This is no longer used
	virtual void DisableUI()=0;

	/*! Get the UI name of the effect - this is only used for Viewport MAnager effects - not materials
	\return A string containing the name
	*/
	virtual TCHAR * GetName()=0;

	/*! For effects that will be using the fixed function pipeline.  This provided access to the IHArdwareMaterial
	allowing access to DirectX FFP.
	\param *pHWMtl A pointer to the internal hardware material.
	\param *pMtl A pointer to the parent Material
	*/
	virtual void SetDXData(IHardwareMaterial * pHWMtl, Mtl * pMtl)=0;
};


// If you are creating a DX9 based DXEffects then you need to implement this interface 
class IDX9DataBridge : public IDXDataBridge
{
public:
	// Interface Lifetime
	virtual LifetimeType	LifetimeControl() { return noRelease; }
	virtual Interface_ID	GetID() { return VIEWPORT_SHADER9_CLIENT_INTERFACE; }

	//! Versioning
	/*! The system uses this to determine whether the effect is compatible to the current GFX loaded on MAX
	\return 8.1 or 9.0 etc...
	*/
	virtual float GetDXVersion() = 0;
	
};

/*! An Interface that provides access to the actual manager.  The manager is responsible for displaying the Viewport Manager which is 
implemented as a Cust Attribute on the material
*/
class IDXShaderManagerInterface : public FPStaticInterface
{
	public:
		virtual CustAttrib* FindViewportShaderManager (MtlBase* mtl)=0;
		virtual CustAttrib* AddViewportShaderManager(MtlBase * mtl)=0;
		
		virtual void	SetVisible(BOOL show=TRUE)=0;
		virtual BOOL	IsVisible()=0;
	
};

inline IDXShaderManagerInterface* GetDXShaderManager() { return (IDXShaderManagerInterface*)GetCOREInterface(IDX_SHADER_MANAGER); }


#endif