#pragma once

// max stuff
#include <max.h>
#include <impexp.h>
#include <iparamb2.h>
// IGame
#include <IGame/IGame.h>

// xrFSL
#include "Vector3.h"
#include "AABB.h"
#include "Mesh.h"
#include "Object.h"

// common
#include <map>

// Forward decl
namespace xrFSL
{
	class Object;
}

// Typedefs
typedef Tab<IGameNode*> TabPNodes_t;
typedef std::map<xrFSL::String, xrFSL::Surface*> MapSurface_t; 

struct SP2Proxy
{
	explicit SP2Proxy(Point2 _point2);

	template<typename T> struct less;
	template<typename T1, typename T2> struct less2;

	Point2 point2;
};


extern HINSTANCE hInstance;

#define xrFSLObjectExport_CLASS_ID	Class_ID(0x8536ade1, 0x2cdf1aa1)

class xrFSLObjectExport : public SceneExport 
{
public:

	static HWND hParams;

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

	BOOL SupportsOptions(int ext, DWORD options);
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

	//Constructor/Destructor
	xrFSLObjectExport();
	~xrFSLObjectExport();	
private:
	Interface* m_pInterface;					///< Pointer to max core interface
	ExpInterface* m_pExportInterface;			///< Pointer to max export interface
	IGameScene* m_pIGameScene;					///< Pointer to IGame interface
	bool m_bSkeletal;							///< Флаг экспорта анимации
	TabPNodes_t m_tabNodes;						///< Массив нодов объектов, заполняется из методов ProcessNodes... etc.
	/** Карта материалов(сурфейсов) объекта. В неё заносятся указатели на сурфейсы при
	извлечении сурфейсов из мешей. */
	MapSurface_t m_mapSharedSurfaces;		

	xrFSL::Object* CreateObject(void);

	void ProcessNodes(void);
	void ProcessNode(IGameNode *pNode);
	void ProcessObject(IGameNode *pNode);	

	bool ExtractPoints(IGameNode* pNode, std::vector<xrFSL::fVector3>& vecPoints, xrFSL::fBox& aabb);
	bool ExtractFaces(IGameNode* pNode, xrFSL::LWFace_vec& vecFaces);
	bool ExtractUVs(IGameNode* pNode, xrFSL::LWFace_vec& vecFaces, xrFSL::lw_vmref_vec& vecVMRefs, xrFSL::VMap_vec& vecVMaps);
	bool ExtractSurfaces(IGameNode* pNode, xrFSL::SurfaceMap_vec& vecSurfMaps);
	bool ExtractSMGroups(IGameNode* pNode, std::vector<xrFSL::uint32>& vecSMGroups);

	xrFSL::Surface* CreateSurface(IGameMaterial* pMaterial);
	void CommitSurfaces(xrFSL::Surface_vec& vecSurfaces);
};



class xrFSLObjectExportClassDesc : public ClassDesc2 
{
public:
	int 			IsPublic() { return TRUE; }
	void *			Create(BOOL loading = FALSE) { return new xrFSLObjectExport(); }
	const TCHAR *	ClassName() { return _T("xrFSLObjectExport"); }
	SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID		ClassID() { return xrFSLObjectExport_CLASS_ID; }
	const TCHAR* 	Category() { return _T(""); }

	const TCHAR*	InternalName() { return _T("xrFSLObjectExport"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance() { return hInstance; }					// returns owning module handle


};