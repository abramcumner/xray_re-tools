/**********************************************************************
 *<
	FILE: ObjectPlugin.h

	DESCRIPTION:	Includes for Plugins

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 2003, All Rights Reserved.
 **********************************************************************/

#ifndef __xrFSLObjectImport__H
#define __xrFSLObjectImport__H

// max stuff
#include "Max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "stdmat.h"
//SIMPLE TYPE

#include <direct.h>
#include <commdlg.h>

// common
#include <map>

// xrFSL
#include "Types.h"
#include "mesh.h"
#include "Vector2.h"

extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define xrFSLObjectImport_CLASS_ID	Class_ID(0x65baca38, 0xc38cdfef)

// Forward decl
namespace xrFSL
{
	class Object;
	class Mesh;
}

// typedefs
typedef std::map<xrFSL::String, StdMat*> MaterialsMap_t;
typedef std::map<xrFSL::String, int> MaterialsMapIDs_t;
typedef std::map<xrFSL::uint32, xrFSL::fVector2> VertsUVMap_t;			// Vert idx -> UV
//typedef std::map<xrFSL::uint32, VertsUVMap_t> FaceVertsUVsMap_t;		// Face idx -> TexCoordMap_t

/**
 *	
 */
class xrFSLObjectImport : public SceneImport 
{
public:
	//Constructor/Destructor
	xrFSLObjectImport();
	~xrFSLObjectImport();

	int				ExtCount();					// Number of extensions supported
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	int				DoImport(const TCHAR *name,ImpInterface *i,Interface *gi, BOOL suppressPrompts=FALSE);	// Import file

	static HWND hParams;
private:
	/**
	 *	
	 */
	bool ImportObject(const xrFSL::Object* pObject);
	bool ImportMesh(const xrFSL::Mesh* pMesh);
	StdMat* CreateMaterial(const xrFSL::String& sTextureName, bool* bStatus = 0);

	Interface* m_pInterface;				///< Pointer to 3d max interface
	ImpInterface* m_pImport;				///< Pointer to 3d max import interface

	/** Карта имён поверхностей и ID-шников материалов
	для данной поверхности в мультиматериале*/
	MaterialsMapIDs_t m_mapMaterialsIDs;
	MultiMtl* m_pMultiMaterial;				///< Мультиматериал содержащий материалы

	//TexCoordMap_t m_mapTexCoord;			///< Карта вершин и их текстурных координат
	//FaceVertsUVsMap_t m_mapTexCoord;		///< Карта фейсов и текстурных координат для вершин этих фейсов
	VertsUVMap_t m_mapUVs;

	// !!! HACK for max 7/8 please fix me!!!
	char* m_pszTexName;		
};

/**
 *	
 */
class xrFSLObjectImportClassDesc : public ClassDesc2 {
public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new xrFSLObjectImport(); }
	const TCHAR *	ClassName() { return _T("xrFSLObjectImport"); }
	SClass_ID		SuperClassID() { return SCENE_IMPORT_CLASS_ID; }
	Class_ID		ClassID() { return xrFSLObjectImport_CLASS_ID; }
	const TCHAR* 	Category() { return _T(""); }

	const TCHAR*	InternalName() { return _T("xrFSLObjectImport"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }					// returns owning module handle
};

#endif
