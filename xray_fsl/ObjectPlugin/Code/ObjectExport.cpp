#include "ObjectExport.h"

// max
#include <meshadj.h>

// IGame
#include <IGame/IConversionManager.h>

// xrFSL
#include "Object.h"
#include "Log.h"

static xrFSLObjectExportClassDesc xrFSLObjectExportDesc;
extern ClassDesc2* GetxrFSLObjectExportDesc() { return &xrFSLObjectExportDesc; }


INT_PTR CALLBACK xrFSLObjectExportOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static xrFSLObjectExport *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (xrFSLObjectExport *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- maxProject1 -------------------------------------------------------
xrFSLObjectExport::xrFSLObjectExport()
{

}

xrFSLObjectExport::~xrFSLObjectExport() 
{

}

int xrFSLObjectExport::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *xrFSLObjectExport::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("object");
}

const TCHAR *xrFSLObjectExport::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("xray *.object mesh file");
}

const TCHAR *xrFSLObjectExport::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("xray mesh");
}

const TCHAR *xrFSLObjectExport::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("Neo][");
}

const TCHAR *xrFSLObjectExport::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("Copyright 2009(c) Neo][ Special for gameru.net");
}

const TCHAR *xrFSLObjectExport::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *xrFSLObjectExport::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int xrFSLObjectExport::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void xrFSLObjectExport::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL xrFSLObjectExport::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

	return TRUE;
}

// Dummy function for progress bar
DWORD WINAPI dummyProgressFN(LPVOID arg)
{
	return(0);
}

int	xrFSLObjectExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	//TODO: Implement the actual file Export here and 
	//		return TRUE If the file is exported properly

// 	if(!suppressPrompts)
// 		DialogBoxParam(hInstance, 
// 		MAKEINTRESOURCE(IDD_PANEL), 
// 		GetActiveWindow(), 
// 		maxProject1OptionsDlgProc, (LPARAM)this);


	// получим указатель на интерфейс IGameScene
	m_pIGameScene = GetIGameInterface();
	// зададим систему координат
	GetConversionManager()->SetCoordSystem(IGameConversionManager::IGAME_D3D);

	m_pIGameScene->InitialiseIGame();	// TODO: В зависимости от опций выставлять параметр
	m_pIGameScene->SetStaticFrame(0);

	if (m_pIGameScene->GetTopLevelNodeCount() == 0)
	{
		LOGW("Nothing to export");
		return FALSE;
	}

	// Пройдёмся по нодам и сохраним указатели на меши в m_tabNodes
	ProcessNodes();

	bool status = false;
	if (xrFSL::Object* pObject = CreateObject())
	{
		if (pObject->Save(name))
			status = true;

		delete pObject;
	}

	return status;
}

void xrFSLObjectExport::ProcessNodes( void )
{
	// do top level nodes
	for(int i = 0; i < m_pIGameScene->GetTopLevelNodeCount(); i++)
		ProcessNode(m_pIGameScene->GetTopLevelNode(i));
}

void xrFSLObjectExport::ProcessNode( IGameNode *pNode )
{
	if (pNode->IsGroupOwner())
	{
	}
	else if (pNode->IsTarget())
	{
	}
	else
		ProcessObject(pNode);

	// process childs nodes
	for (int n = 0; n < pNode->GetChildCount(); n++)
		ProcessNode(pNode->GetNodeChild(n));
}

void xrFSLObjectExport::ProcessObject( IGameNode *pNode )
{
	IGameObject* pObject = pNode->GetIGameObject();
	IGameObject::ObjectTypes type = pObject->GetIGameType();

	switch(type)
	{
	case IGameObject::IGAME_MESH:
		// Указатель на нод загоняем в массив
		m_tabNodes.Append(1, (IGameNode**)&pNode);
		break;
	case IGameObject::IGAME_BONE:
		//doBone(pNode);
		break;
	case IGameObject::IGAME_SPLINE:
	case IGameObject::IGAME_HELPER:
	case IGameObject::IGAME_IKCHAIN:
	case IGameObject::IGAME_UNKNOWN:
		break;
	}
}

//************************************
// Method:    CreateObject
// FullName:  xrFSLObjectExport::CreateObject
// Access:    private 
// Returns:   xrFSL::Object*
// Qualifier:
// Parameter: TabPMeshes_t & tabMeshes
//************************************
xrFSL::Object* xrFSLObjectExport::CreateObject( void )
{
	bool status;

	xrFSL::Object* pObject = new xrFSL::Object();
	pObject->flags() = xrFSL::EOF_STATIC;
	pObject->meshes().reserve(m_tabNodes.Count());

	for (unsigned i = m_tabNodes.Count(); i != 0;)
	{
		IGameNode* pNode = m_tabNodes[--i];

		xrFSL::Mesh* pxrMesh = new xrFSL::Mesh();
		pObject->meshes().push_back(pxrMesh);
		pxrMesh->name() = pNode->GetName();

		if (!(status = ExtractPoints(pNode, pxrMesh->points(), pxrMesh->bbox())))
		{
			goto fail;
		}

		if (!(status = ExtractFaces(pNode, pxrMesh->faces())))
		{
			goto fail;
		}
		
		if (!(status = ExtractUVs(pNode, pxrMesh->faces(), pxrMesh->vmrefs(), pxrMesh->vmaps())))
		{
			goto fail;
		}

		if (!(status = ExtractSurfaces(pNode, pxrMesh->surfmaps())))
		{
			goto fail;
		}

		if (!(status = ExtractSMGroups(pNode, pxrMesh->sgroups())))
		{
			goto fail;
		}
	}

	CommitSurfaces(pObject->surfaces());

	return pObject;

fail:
	delete pObject;
	return NULL;
}

//************************************
// Method:    ExtractPoints
// FullName:  xrFSLObjectExport::ExtractPoints
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: IGameMesh * pGameMesh
// Parameter: std::vector<xrFSL::fVector3> & vecPoints
// Parameter: xrFSL::fBox & aabb
//************************************
bool xrFSLObjectExport::ExtractPoints( IGameNode* pGameNode, std::vector<xrFSL::fVector3>& vecPoints, xrFSL::fBox& aabb )
{
	// Получим указатель на враппер меша
	IGameMesh* pGameMesh = (IGameMesh*)pGameNode->GetIGameObject();


	int NumPoints = pGameMesh->GetNumberOfVerts();
	if (NumPoints < 4) 
	{
		LOGSE << "Can't export mesh" <<  pGameMesh->GetMaxObject()->GetObjectName() << " with less than four vertices";
		return false;
	}

	vecPoints.reserve(size_t(NumPoints & INT_MAX));
	aabb.Invalidate();

	Point3 p0;
	xrFSL::fVector3 p;

	for (int i = 0; i != NumPoints; i++)
	{
		pGameMesh->GetVertex(i, p0);

		// Тут стоит делать скэйл и т.п. вещи
		aabb.Extend(p.Set(p0.x,	p0.y, p0.z));

		vecPoints.push_back(p);
	}

	return true;
}

//************************************
// Method:    ExtractFaces
// FullName:  xrFSLObjectExport::ExtractFaces
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: IGameMesh * pGameMesh
// Parameter: xrFSL::LWFace_vec & vecFaces
//************************************
bool xrFSLObjectExport::ExtractFaces( IGameNode* pGameNode, xrFSL::LWFace_vec& vecFaces )
{
	// Получим указатель на враппер меша
	IGameMesh* pGameMesh = (IGameMesh*)pGameNode->GetIGameObject();

	// Extract the 3ds Max data into IGame data.  
	if (!pGameMesh->InitializeData())
	{
		LOGSE << "Can't extract the 3ds Max data into IGame data for node: " << pGameNode->GetName();
		return false;
	}

	// TODO: По идее данные инициализированы, можем уйти от работы с максовым мешем.

	int NumFaces = pGameMesh->GetMaxMesh()->getNumFaces();
	if (NumFaces < 2)
	{
		LOGSE << "Can't export mesh " << pGameMesh->GetMaxObject()->GetObjectName() << " with less than two faces";
		return false;
	}

	vecFaces.reserve(size_t(NumFaces & INT_MAX));

	for (uint_fast32_t i = 0; i != NumFaces; ++i)
	{
		// получим указатель на фейс
		Face& objFace = pGameMesh->GetMaxMesh()->faces[i];

		xrFSL::LWFace face(objFace.v[2], objFace.v[1], objFace.v[0]);
		vecFaces.push_back(face);
	}

	return true;
}


//************************************
// Функция простой сортировки Tab-a
// Method:    CompTable
// FullName:  CompTable
// Access:    public static 
// Returns:   int
// Qualifier:
// Parameter: const void * item1
// Parameter: const void * item2
//************************************
static int NumComp(const void* item1, const void* item2) 
{
	if (*(int*)item1 == *(int*)item2)
		return 0;

	return *(int*)item1 > *(int*)item2 ? 1 : -1;
}


//************************************
// Method:    ExtractUVs
// FullName:  xrFSLObjectExport::ExtractUVs
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: IGameMesh * pGameMesh
// Parameter: xrFSL::LWFace_vec & vecFaces
// Parameter: xrFSL::lw_vmref_vec & vecVMRefs
// Parameter: xrFSL::VMap_vec & vecVMaps
//************************************
#if 1
bool xrFSLObjectExport::ExtractUVs( IGameNode* pGameNode, xrFSL::LWFace_vec& vecFaces, xrFSL::lw_vmref_vec& vecVMRefs, xrFSL::VMap_vec& vecVMaps )
{
	// Получим указатель на враппер меша
	IGameMesh* pGameMesh = (IGameMesh*)pGameNode->GetIGameObject();


	xrFSL::UVVMap* pUVMap = 0;
	xrFSL::FaceUVVmap* pFaceUVMap = 0;
	std::map<xrFSL::fVector2, int> mapUVsRefs;			// Карта соотв. текстурных координат и индексов в карте смещений(UV -> VMRefs)
	xrFSL::uint32 uiVertIdx = -1;						// Индекс вершины

	// Создаём карту UV
	pUVMap = new xrFSL::UVVMap("Texture");
	pUVMap->Reserve(size_t(pGameMesh->GetNumberOfVerts() & INT_MAX));
	vecVMaps.push_back(pUVMap);

	// Заполним UV карту
	int NumVertices = pGameMesh->GetNumberOfVerts();
	for (uint_fast32_t i = 0; i < NumVertices; ++i)
	{
		// UV value
		Point2 p2UV;
		xrFSL::fVector2 uv0;			

		// получим UV
		if (!pGameMesh->GetTexVertex(i, p2UV))
		{
			LOGSE << "Can't extract UVs for vert: " << i;
			return false;
		}
		uv0.Set(p2UV.x, 1.f - p2UV.y);

		xrFSL::lw_vmref vmref0;
		vmref0.push_back(xrFSL::lw_vmref_entry(0, pUVMap->AddUV(uv0, i)));

		xrFSL::uint32 vmref0_idx = xrFSL::uint32(vecVMRefs.size() & xrFSL::UINT32_MAX);
		vecVMRefs.push_back(vmref0);

		// запомним индекс координаты
		mapUVsRefs[uv0] = vmref0_idx;
	}

	//-------------------------------------------
	// Проход по всем фейсам(создание полимапов)
	// Т.к. каждый фейс на своей вершине может
	// иметь другие координаты.
	//--------------------------------------------
	int NumFaces = pGameMesh->GetNumberOfFaces();
	for (uint_fast32_t face_idx = 0; face_idx != NumFaces; ++face_idx)
	{
		// цикл прохода по вершинам фейса
		for (uint_fast32_t j = 3; j != 0;)
		{
			// Получим ссылку на фейс
			xrFSL::LWFace& face = vecFaces[face_idx];

			// получим указатель на текстурный фейс
			//TVFace& pTFace = pGameMesh->GetMaxMesh()->tvFace[face_idx];		

			// Получим индекс вершины
			uiVertIdx = face.v[--j];

			// UV value
			Point2 p2UV;
			//Point3 p3UV;
			xrFSL::fVector2 uv0;			

			// получим UV
			if (!pGameMesh->GetTexVertex(pGameMesh->GetFace(face_idx)->texCoord[j], p2UV))
			{
				return false;
			}
			//p3UV = pGameMesh->GetMaxMesh()->tVerts[pTFace.getTVert(j)];			
			//uv0.Set(p3UV.x, 1.f - p3UV.y);

			//---------------------------------
			// Построение полимапов
			//---------------------------------
			xrFSL::uint32 vmref_idx;

			// проверим, есть ли вершины с такой же текстурной координатой
			//if (mapVerticesUVs[p2UV].size() > 1)
			if (FindInMap(mapUVsRefs, uv0, xrFSL::UINT32_MAX) != xrFSL::UINT32_MAX) 
			{
				// Если есть, тогда сохраним индекс в карте соотв.
				//vmref_idx = vmref0_idx;
				vmref_idx = mapUVsRefs[uv0];
			}
			else
			{
				if (pFaceUVMap == 0)
				{
					pFaceUVMap = new xrFSL::FaceUVVmap("Texture");
					vecVMaps.push_back(pFaceUVMap);
				}

				xrFSL::lw_vmref vmref;
				vmref.push_back(xrFSL::lw_vmref_entry(1, pFaceUVMap->AddUV(uv0, uiVertIdx, face_idx)));
				vmref_idx = xrFSL::uint32(vecVMRefs.size() & xrFSL::UINT32_MAX);
				vecVMRefs.push_back(vmref);
			}

			for (uint_fast32_t i = 3; i != 0;)
			{
				if (face.v[--i] == uiVertIdx)
				{
					face.ref[i] = vmref_idx;
					vmref_idx = xrFSL::UINT32_MAX;
					break;
				}
			}

			ASSERT_F(vmref_idx == xrFSL::UINT32_MAX, (""));
		}
	}
	return true;
}
#else

bool xrFSLObjectExport::ExtractUVs( IGameNode* pGameNode, xrFSL::LWFace_vec& vecFaces, xrFSL::lw_vmref_vec& vecVMRefs, xrFSL::VMap_vec& vecVMaps )
{
	// Получим указатель на враппер меша
	IGameMesh* pGameMesh = (IGameMesh*)pGameNode->GetIGameObject();


	xrFSL::UVVMap* pUVMap = 0;
	xrFSL::FaceUVVmap* pFaceUVMap = 0;

	// получим количество вершин в меше
	int NumVertices = pGameMesh->GetNumberOfVerts();

	// проход по всем вершинам меша
	for (uint_fast32_t vert_idx = 0; vert_idx < NumVertices; ++vert_idx)
	{
		// UV value
		Point2 p2UV;
		xrFSL::fVector2 uv0;			

		// получим UV
		if (!pGameMesh->GetTexVertex(vert_idx, p2UV))
		{
			LOGSE << "Can't extract UVs for vert: " << vert_idx;
			return false;
		}
		uv0.Set(p2UV.x, 1.f - p2UV.y);

		// Если это первая итерация цикла, тогда создадим UVMap
		if (pUVMap == 0)
		{
			pUVMap = new xrFSL::UVVMap("Texture");
			pUVMap->Reserve(size_t(pGameMesh->GetNumberOfVerts() & INT_MAX));
			vecVMaps.push_back(pUVMap);
		}

		xrFSL::lw_vmref vmref0;
		vmref0.push_back(xrFSL::lw_vmref_entry(0, pUVMap->AddUV(uv0, uiVertIdx)));

		xrFSL::uint32 vmref0_idx = xrFSL::uint32(vecVMRefs.size() & xrFSL::UINT32_MAX);
		vecVMRefs.push_back(vmref0);


		//--------------
		// Face adj
		//--------------
		AdjEdgeList* pAdjEdges = new AdjEdgeList(pGameMesh->GetMaxMesh());
		if (!pAdjEdges)
			return false; 

		AdjFaceList* pAdjFaces = new AdjFaceList(pGameMesh->GetMaxMesh(), *pAdjEdges);
		if (!pAdjFaces)
			return false;
		//----------------------------------------------------------

		// получим все фейсы, которые приконекчены к данной вершине
		DWORDTab tabAdjFaces;
		pAdjEdges->GetFaceList(vert_idx, tabAdjFaces);

		for (uint_fast32_t i = tabAdjFaces.Count(); i != 0;)
		{
			xrFSL::uint32 face_idx = tabAdjFaces[--i];
		}
	}

	/*

		MIntArray adjacents;
		it.getConnectedFaces(adjacents);
		for (unsigned i = adjacents.length(); i != 0;) {
			uint32_t face_idx = uint32_t(adjacents[--i] & INT_MAX), vmref_idx;

			fvector2 uv;
			if (!it.getUV(face_idx, uv.xy)) {
				msg("can't extract UVs for vert %"PRIu32" face %"PRIu32, vert_idx, face_idx);
				uv = uv0;
			}
			uv.v = 1.f - uv.v;

			lw_face& face = faces[face_idx];
			if (uv == uv0) {
				vmref_idx = vmref0_idx;
			} else {
				if (face_uv_vmap == 0) {
					face_uv_vmap = new xr_face_uv_vmap("Texture");
					vmaps.push_back(face_uv_vmap);
				}
				lw_vmref vmref;
				vmref.push_back(lw_vmref_entry(1, face_uv_vmap->add_uv(uv, vert_idx, face_idx)));
				vmref_idx = uint32_t(vmrefs.size() & UINT32_MAX);
				vmrefs.push_back(vmref);
			}
			for (uint_fast32_t j = 3; j != 0;) {
				if (face.v[--j] == vert_idx) {
					face.ref[j] = vmref_idx;
					vmref_idx = UINT32_MAX;
					break;
				}
			}
			xr_assert(vmref_idx == UINT32_MAX);
		}
	}
	return status;
	*/
}
#endif

//************************************
// Method:    CreateSurface
// FullName:  xrFSLObjectExport::CreateSurface
// Access:    private 
// Returns:   xrFSL::Surface*
// Qualifier:
// Parameter: IGameMaterial * pMaterial
//************************************
xrFSL::Surface* xrFSLObjectExport::CreateSurface( IGameMaterial* pMaterial )
{
	xrFSL::Surface* pSurface = new xrFSL::Surface(m_bSkeletal);
	pSurface->name() = pMaterial->GetMaterialName();

//	xrFSL::String sFilePath = pMaterial->GetIGameTextureMap()->GetBitmapFileName();

/*
		MFnDependencyNode shader_fn(shader_obj);
		shader_fn.findPlug("c").connectedTo(connected_plugs, true, false);
		for (unsigned i = connected_plugs.length(); i != 0;) {
			MFnDependencyNode dep_fn(connected_plugs[--i].node());
			if (dep_fn.typeName() == "file") {
				MString file_path = dep_fn.findPlug("ftn").asString();
				if (file_path.numChars()) {
					int i, j;
					if ((i = file_path.rindexW('/')) < 0)
						i = file_path.rindexW('\\');
					if ((j = file_path.rindexW('.')) < 0)
						j = file_path.numChars();
					MString name(file_path.substringW(i + 1, j - 1));
					if ((i = name.indexW('_')) > 0) {
						surface->texture() = (name.substringW(0, i - 1) + "\\" + name).asChar();
					} else {
						surface->texture() = name.asChar();
					}
				}
				break;
			}
		}*/
	

	return pSurface;
}


//************************************
// Method:    ExtractSurfaces
// FullName:  xrFSLObjectExport::ExtractSurfaces
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: IGameMesh * pGameMesh
// Parameter: xrFSL::SurfaceMap_vec & vecSurfMaps
//************************************
bool xrFSLObjectExport::ExtractSurfaces( IGameNode* pGameNode, xrFSL::SurfaceMap_vec& vecSurfMaps )
{
	// Получим указатель на враппер меша
	IGameMesh* pGameMesh = (IGameMesh*)pGameNode->GetIGameObject();

	// Получим все ID-шники материалов меша
	Tab<int> tabMtlIDs = pGameMesh->GetActiveMatIDs();

	if (tabMtlIDs.Count() == 0)
	{
		LOGSE << "Materials IDs count is zero";
		return false;
	}

	// резервируем место под материалы
	vecSurfMaps.resize(tabMtlIDs.Count());

	for (int i = 0; i < tabMtlIDs.Count(); i++)
	{
		// Получим все фейсы соотв. материалу
		Tab<FaceEx*> tabFacesByMatID = pGameMesh->GetFacesFromMatID(tabMtlIDs[i]);

		for (unsigned j = 0; j < tabFacesByMatID.Count(); ++j)
		{
			xrFSL::SurfaceMap* pSurfaceMap = vecSurfMaps[i];
			if (pSurfaceMap == 0)
			{
				// Получим материал асигнованный к обрабатываемому фейсу
				IGameMaterial* pMaterial = pGameMesh->GetMaterialFromFace(tabFacesByMatID[j]);

				// Возможно такой тип материала мы уже имеем, работали - знаем :)
				// Смотрим в карте материалов наличие такого материала
				xrFSL::Surface*& pSurface = m_mapSharedSurfaces[pMaterial->GetMaterialName()];
				if (pSurface == 0)
				{	
					// Если нет, тогда создаём такой тип сурфейса
					pSurfaceMap = new xrFSL::SurfaceMap(CreateSurface(pMaterial));
					pSurface = pSurfaceMap->pSurface;
				} 
				else
				{
					// Если уже есть, тогда просто добавляем к карте сурфейсов
					pSurfaceMap = new xrFSL::SurfaceMap(pSurface);
				}

				// Заносим surface map в меш
				vecSurfMaps[i] = pSurfaceMap;
			}

			// Заносим индекс фейса
			pSurfaceMap->vecFaces.push_back(tabFacesByMatID[j]->meshFaceIndex);
		}
	}

	return true;
}

//************************************
// Method:    ExtractSMGroups
// FullName:  xrFSLObjectExport::ExtractSMGroups
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: IGameMesh * pGameMesh
// Parameter: std::vector<xrFSL::uint32> & vecSMGroups
//************************************
bool xrFSLObjectExport::ExtractSMGroups( IGameNode* pGameNode, std::vector<xrFSL::uint32>& vecSMGroups )
{
	return true;
}

//************************************
// Method:    CommitSurfaces
// FullName:  xrFSLObjectExport::CommitSurfaces
// Access:    private 
// Returns:   void
// Qualifier:
// Parameter: xrFSL::Surface_vec & vecSurfaces
//************************************
void xrFSLObjectExport::CommitSurfaces( xrFSL::Surface_vec& vecSurfaces )
{
	vecSurfaces.reserve(m_mapSharedSurfaces.size());

	for (MapSurface_t::iterator it = m_mapSharedSurfaces.begin(),
		end = m_mapSharedSurfaces.end(); it != end; ++it)
	{
		vecSurfaces.push_back(it->second);
	}
}