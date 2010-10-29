/*********************************************************************
 *<
	FILE: ObjectPlugin.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2003, All Rights Reserved.
 **********************************************************************/

#include "ObjectImport.h"

// max stuff
#include "maxapi.h"
#include <mesh.h>
#include <triobj.h>
#include <meshadj.h>
#include <imtledit.h>
#include <quat.h>

// xrFSL
#include "Types.h"
#include "FileSystem.h"
#include "StringUtils.h"
#include "Log.h"
#include "Assert.h"
#include "Object.h"


static xrFSLObjectImportClassDesc xrFSLObjectImportDesc;
ClassDesc2* GetxrFSLObjectImportDesc() { return &xrFSLObjectImportDesc; }


INT_PTR CALLBACK xrFSLObjectImportOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static xrFSLObjectImport *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (xrFSLObjectImport *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- xrFSLObjectImport -------------------------------------------------------
xrFSLObjectImport::xrFSLObjectImport()
{
	
}

xrFSLObjectImport::~xrFSLObjectImport() 
{

}

int xrFSLObjectImport::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR *xrFSLObjectImport::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
	return _T("object");
}

const TCHAR *xrFSLObjectImport::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("xray *.object mesh file");
}
	
const TCHAR *xrFSLObjectImport::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("xray mesh");
}

const TCHAR *xrFSLObjectImport::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("Neo][");
}

const TCHAR *xrFSLObjectImport::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("Copyright 2009(c) Neo][ Special for gameru.net");
}

const TCHAR *xrFSLObjectImport::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *xrFSLObjectImport::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int xrFSLObjectImport::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void xrFSLObjectImport::ShowAbout(HWND hWnd)
{			
	// Optional
}

int xrFSLObjectImport::DoImport(const TCHAR *filename,ImpInterface *i,
						Interface *gi, BOOL suppressPrompts)
{
	// save pointers to interfaces
	m_pInterface = gi;
	m_pImport = i;

	LOGS << "Start importing file: \"" << filename << "\".";

	// Загружаем объект
	xrFSL::Object object;
	object.Load(filename);

	// Импортируем объект
	LOG("Loading complete. Start importing...");
	bool status = ImportObject(&object);

	LOGS << "[" << status << "]Complete import(" << filename << ")";

	m_pInterface->RedrawViews(0);

	return status;	

// 	if(!suppressPrompts)
// 		DialogBoxParam(hInstance, 
// 				MAKEINTRESOURCE(IDD_PANEL), 
// 				GetActiveWindow(), 
// 				xrFSLObjectImportOptionsDlgProc, (LPARAM)this);
}

//************************************
// Method:    ImportObject
// FullName:  xrFSLObjectImport::ImportObject
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: const xrFSL::Object * pObject
//************************************
bool xrFSLObjectImport::ImportObject( const xrFSL::Object* pObject )
{
	bool bStatus = false;

	//------------------------
	// Surfaces
	//------------------------
	MaterialsMap_t mapTextures;

	// Карта ID-шников и текстур(Имя текстуры -> ID in multimaterial)
	std::map<xrFSL::String, int> mapTexIDs;

	LOGS << "Creating materials";

	// Создаём мультиматериал, который будет содержать все наши материалы
	m_pMultiMaterial = NewDefaultMultiMtl();
	// Загоняем материал в 1-й слот редактора материалов
	GetMtlEditInterface()->PutMtlToMtlEditor(m_pMultiMaterial, 0);
	for (xrFSL::Surface_vec_cit it = pObject->surfaces().begin(),
		end = pObject->surfaces().end(); it != end; ++it) 
	{
		xrFSL::Surface* surface = *it;

		MaterialsMap_t::const_iterator tex_it = mapTextures.find(surface->texture()); 
		if (tex_it == mapTextures.end())
		{
			mapTextures[surface->texture()] = CreateMaterial(surface->texture(), &bStatus);
			mapTexIDs[surface->texture()] = mapTextures.size();	// Запомним id текструы в мультиматериале
			if (!bStatus)
				break;
		}	

		if (!bStatus)
			return bStatus;


		// disable warning
		// warning C4267: 'argument' : conversion from 'size_t' to 'int', possible loss of data
#		pragma warning(disable: 4267)
		// Загоним материалы в наш один мультиматериал
		m_pMultiMaterial->SetNumSubMtls(mapTextures.size() + 1);
#		pragma warning(default: 4267)

		int id = mapTexIDs[surface->texture()];

		m_mapMaterialsIDs[surface->name()] = id;
		m_pMultiMaterial->SetSubMtl(id, mapTextures[surface->texture()]);	
	}


	//-------------------------
	// Meshes
	//-------------------------
	LOGS << "Importing meshes(Meshes count: " << pObject->meshes().size() << ").";
	for (xrFSL::Mesh_vec_cit it = pObject->meshes().begin(),
		end = pObject->meshes().end(); it != end; ++it) 
	{
		if (!(bStatus = ImportMesh(*it)))
			break;
	}

//	if (!bStatus)
	return bStatus;
}

TriObject *GetTriObjectFromNode(INode *node, BOOL &deleteIt)  
{ 
	deleteIt   = FALSE; 

	//Get current time from UI 
	//TimeValue t = GetCOREInterface9()->GetTime(); 
	TimeValue t = GetCOREInterface7()->GetTime();

	//Evaluate object at current time 
	//Object *obj = node->EvalWorldState(t).obj; 
	Object* obj = node->GetObjectRef();

	//Can we convert it in a TriObject ? 
	if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) 
	{  
		//Let’s convert the object in a TriObject 
		TriObject *tri = static_cast<TriObject *>  
			( obj->ConvertToType(t,Class_ID(TRIOBJ_CLASS_ID, 0))); 

		// Note that the TriObject should only be deleted 
		// if the pointer to it is not equal to the object 
		// pointer that called ConvertToType() 
		if (obj != tri)  
			deleteIt = TRUE; 
		return tri; 
	} 
	else  
		return NULL; 
}

void GetMtlIDFromNode(INode* _node) 
{ 
	if (!_node)return; 

	//Now get the mesh from the node 
	BOOL  deleteit; 
	TriObject*  Tri = GetTriObjectFromNode(_node,deleteit); 
	if (!Tri)return; 

	//Get mesh from triobject 
	Mesh& mesh = Tri->GetMesh(); 

	//Get the material from node 
	Mtl* nodemtl = _node->GetMtl(); 
	if (!nodemtl)return; //No material applied 

	if (! nodemtl->IsMultiMtl() )  return; //It's not a multimaterial 

	MultiMtl* multimtl = static_cast<MultiMtl*>(nodemtl); 

	const int NumSubMaterials = multimtl->NumSubs(); 

	const int NumFaces = mesh.getNumFaces(); 
	//Scan all faces of the mesh 
	for (int i=0;i<NumFaces;i++) 
	{ 
		//Get the ID in this face (apply the modulo) 
		const int ID = mesh.getFaceMtlIndex(i) % NumSubMaterials; 

		//Get the material applied on this face     
		Mtl* MtlAppliedOnThisFace = multimtl->GetSubMtl(ID); 
		if (! MtlAppliedOnThisFace) continue; 
	}   

	if (deleteit) 
		Tri->DeleteMe(); 
}

void GetQuat(Quat* pQuat, INode* pNode)
{
	Control* pTransf = pNode->GetTMController();
	if(pTransf)
	{
		Control * pCtrl = pTransf->GetRotationController();
		if(pCtrl)
			pCtrl->GetValue(0, pQuat, NEVER, CTRL_ABSOLUTE);
	}
}

//************************************
// Method:    ImportMesh
// FullName:  xrFSLObjectImport::ImportMesh
// Access:    private 
// Returns:   bool
// Qualifier:
// Parameter: const xrFSL::Mesh * pMesh
//************************************
bool xrFSLObjectImport::ImportMesh( const xrFSL::Mesh* pMesh )
{
	TriObject* pObject = CreateNewTriObject();
	ASSERT_W(pObject, ("can't create a TriObject"));
	if (!pObject)
		return FALSE;

	Mesh& maxMesh = pObject->GetMesh();

	ImpNode* pImpNode = m_pImport->CreateNode();
	ASSERT_W(pImpNode, ("can't create a ImportNode"));
	if (!pImpNode)
	{
		delete pObject;
		return FALSE;
	}

	Matrix3 m3TM;
	m3TM.IdentityMatrix();	
	//m3TM.PreRotateX(DegToRad(90));
	//m3TM.PreRotateY(DegToRad(180));
	//m3TM.SetRotateZ(DegToRad(180));
	//m3TM.SetRotateX(-(DegToRad(90)));		// change object orientation from xray
	//m3TM.SetRotateY(DegToRad(180));
	//m3TM.SetRotate(DegToRad(-90), DegToRad(180), DegToRad(180));

	pImpNode->Reference(pObject);
	pImpNode->SetTransform(0, m3TM);
	m_pImport->AddNodeToScene(pImpNode);
	pImpNode->SetName(pMesh->name().c_str());

	// change coord system
	Quat qMesh, qMax;
	GetQuat(&qMesh, pImpNode->GetINode());
	qMax.Set(-qMesh.x, -qMesh.z, -qMesh.y, qMesh.w);
	m3TM.SetRotate(qMax);
	pImpNode->SetTransform(0, m3TM);

	// Асигнуем наш мультиматериал, содержащий все материалы к ноду
	pImpNode->GetINode()->SetMtl(m_pMultiMaterial);

#	pragma warning(disable: 4267)
	maxMesh.setNumVerts(pMesh->points().size());			// number of mesh vertices
	maxMesh.setNumTVerts(pMesh->faces().size() * 3);		// number of texture vertices
	maxMesh.setNumFaces(pMesh->faces().size());				// number of mesh faces
	maxMesh.setNumTVFaces(pMesh->faces().size());		
#	pragma warning(default: 4267)

	//-------------------------------
	//
	//-------------------------------
	const xrFSL::VMap_vec& vecVMaps = pMesh->vmaps();
	const std::vector<xrFSL::fVector3>& vecPoints = pMesh->points();
	const xrFSL::LWFace_vec& vecFaces = pMesh->faces();
	const xrFSL::lw_vmref_vec& vmrefs = pMesh->vmrefs();
	const xrFSL::UVVMap* pVMap = 0;

	xrFSL::uint32 uiVertIdx = 0;		// Счётчик вершин
	xrFSL::uint32 uiFaceIdx = 0;		// Счётчик фэйсов

	//------------------------------
	// Vertices
	//------------------------------
	LOGS << "[ImportMesh]Adding vertices(Verts count: " << vecPoints.size() << ")";	
	
	uiVertIdx = 0;
	
	for (std::vector<xrFSL::fVector3>::const_iterator it = vecPoints.begin(), 
		end = vecPoints.end(); it != end; ++it)
	{
		maxMesh.setVert(uiVertIdx++, it->x, it->y, it->z);
	}

	//------------------------------
	// Faces
	//------------------------------
	LOGS << "[ImportMesh]Adding faces(Faces count: " << pMesh->faces().size() << ")";
	
	uiVertIdx = 0;
	uiFaceIdx = 0;

	// !!! TODO: Optimize loop
	for (xrFSL::LWFace_vec_cit it = vecFaces.begin(), end = vecFaces.end(); it != end; ++it)
	{
		maxMesh.faces[uiFaceIdx].setVerts(it->v0, it->v1, it->v2);		// set the vertices of face
		maxMesh.faces[uiFaceIdx].setEdgeVisFlags(1, 1, 1);	

		for (uint_fast32_t i = 3; i != 0;) 
		{
			const xrFSL::lw_vmref& vmref = vmrefs[it->ref[--i]];
			for (xrFSL::lw_vmref::const_iterator it1 = vmref.begin(), end1 = vmref.end(); it1 != end1; ++it1) 
			{
				if (vecVMaps[it1->vmap]->type() == xrFSL::VMap::VMT_UV)
				{
					// Заполнение карты текстурных координат
					pVMap = static_cast<const xrFSL::UVVMap*>(vecVMaps[it1->vmap]);
					m_mapUVs[uiVertIdx] = pVMap->UVs()[it1->offset];										
				}
			}
			++uiVertIdx;
		}

		++uiFaceIdx;
	}
	
	// !!! Assigning tex coord	
	uiFaceIdx = 0;
	uiVertIdx = 0;

	
	for (xrFSL::LWFace_vec_cit it = vecFaces.begin(), end = vecFaces.end(); it != end; ++it)
	{
		for (uint_fast32_t i = 3; i != 0;)
		{
			maxMesh.setTVert(uiVertIdx, m_mapUVs[uiVertIdx].u, 1.f - m_mapUVs[uiVertIdx].v, 0.0f);
			maxMesh.tvFace[uiFaceIdx].t[--i] = uiVertIdx;

			++uiVertIdx;			
		}
		++uiFaceIdx;
	}

	//----------------------------------
	// set smooth groups
	//----------------------------------
	LOG("[ImportMesh]Set smooth groups.");
	if (!pMesh->sgroups().empty())
	{
		/*
		class MEdge
		Data Members:
		DWORD f[2];	The indices into the meshes face table of the two faces that share this edge.

		class AdjEdgeList
		Data Members:
		DWORDTab *list;	This is an array of DWORDTabs, one per vertex. The Tab is a list of indices into 
		the edge list, one for each edge adjacent to the vertex.

		Tab<MEdge> edges; The table of edges.
		int nverts;	The size of list. 
		*/
		// building an adjacency table between edges
		AdjEdgeList* pAdjEdges = new AdjEdgeList(maxMesh); 
		ASSERT_W(pAdjEdges, ("can't build an asjacency table"));

		const std::vector<xrFSL::uint32>& sgroups = pMesh->sgroups();
		if (pMesh->flags() & xrFSL::EMF_3DSMAX)
		{
			LOGI("Mesh has max sgroups...");
			//for (int i = 0; i < pAdjEdges->nverts; i++)
			for (int i = 0; i < maxMesh.getNumFaces(); i++)
			{
				maxMesh.faces[i].setSmGroup(sgroups[i]);
			}
		}
		else
		{
#if 1
			LOGE("Other than max sgroups isn't implemented in current version");
#else
			for (int i = 0; i < pAdjEdges->nverts; i++)
			{
				//sm = (sgroups[pAdjEdges->edges[i].f[0]] & sgroups[pAdjEdges->edges[i].f[1]]) != 0;
				maxMesh.faces[i].setSmGroup(sgroups[i]);
			}
			/*
			for (MItMeshEdge it(mesh_obj); !it.isDone(); it.next()) {
				it.getConnectedFaces(connected, &status);
				it.setSmoothing(status && connected.length() == 2 &&
					sgroups[connected[0]] != EMESH_NO_SG &&
					sgroups[connected[0]] == sgroups[connected[1]]);
			}
			*/		
#endif
		}
		
	}

	//--------------------------------------
	// Set faces mat id
	//--------------------------------------
	for (xrFSL::SurfaceMap_vec_cit it = pMesh->surfmaps().begin(), 
		end = pMesh->surfmaps().end(); it != end; ++it) 
	{
		const xrFSL::SurfaceMap* smap = *it;
		
		MaterialsMapIDs_t::const_iterator mat_it = m_mapMaterialsIDs.find(smap->pSurface->name()); 
		if (mat_it == m_mapMaterialsIDs.end())
		{
			LOGE("null shading group object");
			return FALSE;
		}

		std::vector<xrFSL::uint32>::const_reverse_iterator it1 = smap->vecFaces.rbegin();
		for (unsigned i = unsigned(smap->vecFaces.size() & UINT_MAX); i != 0; ++it1)
		{
			maxMesh.faces[*it1].setMatID(m_mapMaterialsIDs[smap->pSurface->name()]);
			--i;
		}
	}

	return TRUE;
}

//************************************
// Method:    CreateMaterial
// FullName:  xrFSLObjectImport::CreateMaterial
// Access:    private 
// Returns:   StdMat
// Qualifier:
// Parameter: const xrFSL::String & sTextureName
// Parameter: bool * bStatus
//************************************
StdMat* xrFSLObjectImport::CreateMaterial( const xrFSL::String& sTextureName, bool* bStatus /*= 0*/ )
{
	xrFSL::String path(sTextureName);
	xrFSL::FileSystem& fs = xrFSL::FileSystem::Instance();

	// Если файл существует, тогда получим полный путь до файла
	if (fs.FileExist(xrFSL::PA_GAME_TEXTURES, path.append(".dds"))) 
	{
		xrFSL::String full_path;
		fs.ResolvePath(xrFSL::PA_GAME_TEXTURES, path, full_path);
		path = full_path;
	}
	else
	{
		LOGSW << "File doesn't exists: " << sTextureName;
	}

	// Заменим в пути к файлу все обратные слэши прямыми
	for (xrFSL::String::size_type i = 0; (i = path.find('\\', i)) != xrFSL::String::npos; ++i)
		path[i] = '/';

	// Получим чисто имя файла текстуры, чтобы обозвать материал
	xrFSL::String::size_type i = sTextureName.rfind('\\');
	xrFSL::String max_name(i != xrFSL::String::npos ? sTextureName.c_str() + i + 1 : sTextureName.c_str());

	// Создаём стандартный материал
	StdMat* pMat = NewDefaultStdMat();
	ASSERT_W(pMat, ("Can't create the material: %s", sTextureName));
	if (!pMat)
	{
		*bStatus = false;
		return NULL;
	}

	pMat->SetName(max_name.c_str());
#if 1
	pMat->SetMtlFlag(MTL_TEX_DISPLAY_ENABLED);		// Отображать материал в окне перспективы
#endif


	// Асигнуем к материалу текстуру
	BitmapTex* pTex = NewDefaultBitmapTex();
	ASSERT_W(pTex, ("Can't create the bitmap texture: %s", sTextureName));
	if (!pTex)
	{
		*bStatus = false;
		return NULL;
	}
	pTex->SetMapName((char*)path.c_str());

	pMat->SetSubTexmap(ID_DI, pTex);
	pMat->EnableMap(ID_DI, true);

	*bStatus = true;
	return pMat;
}
	
