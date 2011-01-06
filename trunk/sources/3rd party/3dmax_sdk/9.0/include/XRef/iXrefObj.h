//**************************************************************************/
// Copyright (c) 1998-2005 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Interface to xref objects
// AUTHOR: Attila Szabo - created Feb.09.2005
//***************************************************************************/

#ifndef _IXREF_OBJ_H_
#define _IXREF_OBJ_H_

#include <object.h>

// Forward declarations
class IObjXRefRecord;
class Animatable;

//! \brief Legacy interface for xref objects
/* Although this interface is still supported, it is recommended that IXRefObject8 
and IXRefItem is used instead.
*/
class IXRefObject : public Object 
{
	public:
		Class_ID ClassID() {return XREFOBJ_CLASS_ID;}
		SClass_ID SuperClassID() {return SYSTEM_CLASS_ID;}

		// Initialize a new XRef object
		virtual void Init(TSTR &fname, TSTR &oname, Object *ob, BOOL asProxy=FALSE)=0;

		virtual void SetFileName(TCHAR *name, BOOL proxy=FALSE, BOOL update=TRUE)=0;
		virtual void SetObjName(TCHAR *name, BOOL proxy=FALSE)=0;
		virtual void SetUseProxy(BOOL onOff,BOOL redraw=TRUE)=0;
		virtual void SetRenderProxy(BOOL onOff)=0;
		virtual void SetUpdateMats(BOOL onOff)=0;
		virtual void SetIgnoreAnim(BOOL onOff,BOOL redraw=TRUE)=0;
		
		virtual TSTR GetFileName(BOOL proxy=FALSE)=0;
		virtual TSTR GetObjName(BOOL proxy=FALSE)=0;
		virtual TSTR &GetCurFileName()=0;
		virtual TSTR &GetCurObjName()=0;
		virtual BOOL GetUseProxy()=0;
		virtual BOOL GetRenderProxy()=0;
		virtual BOOL GetUpdateMats()=0;
		virtual BOOL GetIgnoreAnim()=0;		
		
		// Causes browse dialogs to appear
		virtual void BrowseObject(BOOL proxy)=0;
		virtual void BrowseFile(BOOL proxy)=0;

		// Try to reload ref
		virtual void ReloadXRef()=0;
};


//! Id for IXRefObject8 interface
#define IID_XREF_OBJECT8 Interface_ID(0x470145ad, 0x3daa64e1)

//! \brief Class representing Xref objects
/*! This interface groups functionality specific to xref objects. In order to
access functionality common to all xref scene entities, use the IXRefItem interface.
An xref object acts as a wrapper around the xrefed object (also referred to as
the "source object"). For example, requests for evaluating the xref object (Eval) and 
finding its base object (FindBaseObject) are passed to the source object, as are
requests for interfaces. Client or plugin code working with objects should not be 
required to write xref object specific code, unless the desired functionality is xref specific.
*/
// Needs to derive from the IXrefObject in order to preserve sdk backward compatibility
class IXRefObject8 : public IXRefObject, public FPMixinInterface
{
public:
	//! \brief Retrieves the active source file name 
	/*! The file currently used by the xref object could be the one where 
	the source object comes from or the one where the proxy object comes from.
	Which file is currently used depends on the proxy settings of the xref object
	\return - Pointer to the path and name of the file currently used by the 
	xref object or NULL if the current file name is not specified.
	\see IXRefProxy, IXRefItem
	*/
	virtual const TCHAR* GetActiveFileName() const = 0;
	
	//! \brief Retrieves the active source item name
	/*! The object name currently used by the xref object could be the source or
	the proxy object name. Which object name currently used depends on the proxy 
	settings of the xref object.
	\return - Pointer to the name of the object currently xrefed or NULL if	the name is not specified.
	\see IXRefProxy, IXRefItem
	*/
	virtual const TCHAR* GetActiveObjName() const = 0;

	//! \brief Retrieves the active xref record this xref object belongs to.
	/*! When the proxy object assigned to this xref object is active, this method
	returns the xref record that owns the xref proxy. Otherwise it returns the xref
	record that owns the original xref item.
	\return The active xref record this xref object belongs to. A return value of NULL, 
	indicates that the xref object is not part of an xref record yet.
	*/
	virtual IObjXRefRecord* GetActiveRecord() const = 0;

	//! \brief Retrieves the actual source object behind the XRef.
	/*! \param [in] resolveNested - When set to true, the method will recursively resolve
	any nested xref objects, guaranteeing that the one being returned is NOT an xref object.
	When set to false, the source object is returned without attempting to resolve nesting.
	\param [in, out] modifiers - a pointer to a Tab<Modifier*> or NULL. If not NULL, 
	the modifiers in the source object will be placed in the tab.
	\return The source object, or NULL if the xref is unresolved. 
	*/
	virtual Object* GetSourceObject(bool resolveNested = false, Tab<Modifier*> *modifiers = NULL) const = 0;

	//! \brief Allows to determine whethe a scene entity implements this interface.
	/*! \param [in] animatable - Reference to scene entity
	\return - true if the scene entity implements the IXRefObject8 interface,	false otherwise
	*/
	static bool Is_IXRefObject8(Animatable& animatable);

	using FPMixinInterface::GetInterface;
	//! \brief Retrieves the IXRefObject8 interface from a scene entity.
	/*! \param [in] is - Reference to scene entity
	\return - Pointer to the IXRefObject8 interface of the scene entity or NULL	if it's not an xref object
	*/
	static IXRefObject8* GetInterface(InterfaceServer& is) {
		return static_cast<IXRefObject8*>(is.GetInterface(IID_XREF_OBJECT8));
	};

	// Function publishing - todo
	virtual Interface_ID GetID() { return IID_XREF_OBJECT8; }

	enum {
		kfpGetActiveFileName,
		kfpGetActiveObjName,
		kfpGetActiveRecord,
		kfpGetSourceObject,
	};

	BEGIN_FUNCTION_MAP
		RO_PROP_FN(kfpGetActiveFileName, GetActiveFileName, TYPE_FILENAME);
		RO_PROP_FN(kfpGetActiveObjName, GetActiveObjName, TYPE_STRING);
		RO_PROP_FN(kfpGetActiveRecord, GetActiveRecord, TYPE_INTERFACE);
		FN_2(kfpGetSourceObject, TYPE_REFTARG, FPGetSourceObject, TYPE_bool, TYPE_REFTARG_TAB_BR);
	END_FUNCTION_MAP

	// From FPMixinInterface
	virtual FPInterfaceDesc* GetDesc() { return &mFPInterfaceDesc; }
	static FPInterfaceDesc mFPInterfaceDesc;

	private:
		Object* FPGetSourceObject(bool resolveNestedXRef, Tab<ReferenceTarget*> &modifiers);

	protected:
		// Deprecated methods
		virtual void Init(TSTR &fname, TSTR &oname, Object *ob, BOOL asProxy=FALSE)=0;
		// Exposed by class IXrefItem
		virtual void SetFileName(TCHAR *name, BOOL proxy=FALSE, BOOL update=TRUE)=0;
		virtual void SetObjName(TCHAR *name, BOOL proxy=FALSE)=0;
		virtual TSTR GetFileName(BOOL proxy=FALSE)=0;
		virtual TSTR GetObjName(BOOL proxy=FALSE)=0;
		// Exposed by class IXrefProxy
		virtual void SetUseProxy(BOOL onOff,BOOL redraw=TRUE)=0;
		virtual void SetRenderProxy(BOOL onOff)=0;
		virtual BOOL GetUseProxy()=0;
		virtual BOOL GetRenderProxy()=0;
		// Replaced by IXRefObject8::Get\SetActiveFileName
		virtual TSTR &GetCurFileName()=0;
		virtual TSTR &GetCurObjName()=0;
};

inline bool IXRefObject8::Is_IXRefObject8(Animatable& animatable) {
	return ((animatable.SuperClassID() == SYSTEM_CLASS_ID) 
					&& (animatable.ClassID() == XREFOBJ_CLASS_ID));
}

#endif // _IXREF_OBJ_H_
