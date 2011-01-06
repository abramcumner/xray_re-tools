#include "stdafx.h"
#include "msPlugInImpl.h"
#include "msLib.h"



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



cMsPlugIn*
CreatePlugIn ()
{
    return new cPlugIn ();
}

void DestroyPlugIn(cMsPlugIn *plugin)
{
	delete plugin;
}

void
DestroyPlugIn (cMsPlugIn* pPlugin)
{
    delete pPlugin;
}


cPlugIn::cPlugIn ()
{
    strcpy (szTitle, "Text...");
}



cPlugIn::~cPlugIn ()
{
}



int
cPlugIn::GetType ()
{
    return cMsPlugIn::eTypeImport|cMsPlugIn::eNormalsAndTexCoordsPerTriangleVertex;
}



const char*
cPlugIn::GetTitle ()
{
    return szTitle;
}


int
msMesh_FindVertex (msMesh *pMesh, msVertex *pVertex)
{
    for (int i = 0; i < msMesh_GetVertexCount (pMesh); i++)
    {
        msVertex *pV = msMesh_GetVertexAt (pMesh, i);
        if (pVertex->Vertex[0] == pV->Vertex[0] &&
            pVertex->Vertex[1] == pV->Vertex[1] &&
            pVertex->Vertex[2] == pV->Vertex[2])
        {
            return i;
        }
    }
    return -1;
}

int
cPlugIn::Execute (msModel *pModel)
{
    if (!pModel)
        return -1;

    //
    // choose filename
    //
    OPENFILENAME ofn;
    memset (&ofn, 0, sizeof (OPENFILENAME));
    
    char szFile[MS_MAX_PATH];
    char szFileTitle[MS_MAX_PATH];
    char szDefExt[32] = "txt";
    char szFilter[128] = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
    szFile[0] = '\0';
    szFileTitle[0] = '\0';

    ofn.lStructSize = sizeof (OPENFILENAME);
    ofn.lpstrDefExt = szDefExt;
    ofn.lpstrFilter = szFilter;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MS_MAX_PATH;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = MS_MAX_PATH;
    ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Import Text";

    if (!::GetOpenFileName (&ofn))
        return 0;

    //
    // import
    //
    FILE *file = fopen (szFile, "rt");
    if (!file)
        return -1;

	int r, i, j;
	int nVersion = 0;
	r = fscanf(file, "SimpleMeshFormat %d\n", &nVersion);

	int nNumMeshes = 0;
	r = fscanf(file, "%d\n", &nNumMeshes);
	for (i = 0; i < nNumMeshes; i++)
	{
		int nMesh = msModel_AddMesh(pModel);
		msMesh *pMesh = msModel_GetMeshAt(pModel, nMesh);

		char szName[64];
		r = fscanf(file, "%s\n", szName);
		msMesh_SetName(pMesh, szName);

		int nNumVertices = 0;
		r = fscanf(file, "%d\n", &nNumVertices);
		for (j = 0; j < nNumVertices; j++)
		{
			msVec3 Vertex;
			r = fscanf(file, "%g %g %g\n", &Vertex[0], &Vertex[1], &Vertex[2]);
			int nVertex = msMesh_AddVertex(pMesh);
			msVertex *pVertex = msMesh_GetVertexAt(pMesh, nVertex);
			msVertex_SetVertex(pVertex, Vertex);
		}

		int nNumTriangles = 0;
		r = fscanf(file, "%d\n", &nNumTriangles);
		for (j = 0; j < nNumTriangles; j++)
		{
			int nTriangle = msMesh_AddTriangle(pMesh);
			msTriangle *pTriangle = msMesh_GetTriangleAt(pMesh, nTriangle);
			msTriangleEx *pTriangleEx = msMesh_GetTriangleExAt(pMesh, nTriangle);
			
			int nSmoothingGroup = 0;
			word nIndices[3];
			msVec3 Normals[3];
			msVec2 TexCoords[3];
			r = fscanf(file, "%d %d %d %d %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
				&nSmoothingGroup,
				&nIndices[0], &nIndices[1], &nIndices[2],
				&Normals[0][0], &Normals[0][1], &Normals[0][2],
				&Normals[1][0], &Normals[1][1], &Normals[1][2],
				&Normals[2][0], &Normals[2][1], &Normals[2][2],
				&TexCoords[0][0], &TexCoords[0][1],
				&TexCoords[1][0], &TexCoords[1][1],
				&TexCoords[2][0], &TexCoords[2][1]
				);
			msTriangle_SetSmoothingGroup(pTriangle, nSmoothingGroup);
			msTriangle_SetVertexIndices(pTriangle, nIndices);
			for (int v = 0; v < 3; v++)
			{
				msTriangleEx_SetNormal(pTriangleEx, v, Normals[v]);
				msTriangleEx_SetTexCoord(pTriangleEx, v, TexCoords[v]);
			}
		}
	}

    fclose (file);

    return 0;
}
