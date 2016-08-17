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
    return cMsPlugIn::eTypeExport|cMsPlugIn::eNormalsAndTexCoordsPerTriangleVertex;
}



const char*
cPlugIn::GetTitle ()
{
    return szTitle;
}


int
cPlugIn::Execute (msModel *pModel)
{
    if (!pModel)
        return -1;

    //
    // check, if we have something to export
    //
    if (msModel_GetMeshCount (pModel) == 0)
    {
        ::MessageBox (NULL, "The model is empty!  Nothing exported!", "Text Export", MB_OK | MB_ICONWARNING);
        return 0;
    }

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
    ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Export Text";

    if (!::GetSaveFileName (&ofn))
        return 0;

    //
    // export
    //
    FILE *file = fopen (szFile, "wt");
    if (!file)
        return -1;

	int nVersion = 1;
	fprintf(file, "SimpleMeshFormat %d\n", nVersion);

    int i, j, v;
	int nNumMeshes = msModel_GetMeshCount(pModel);
	fprintf(file, "%d\n", nNumMeshes);
    for (i = 0; i < nNumMeshes; i++)
    {
        msMesh *pMesh = msModel_GetMeshAt(pModel, i);
        char szName[64];
        msMesh_GetName(pMesh, szName, 64);

        fprintf(file, "%s\n", szName);

		// vertices
		int nNumVertices = msMesh_GetVertexCount(pMesh);
		fprintf(file, "%d\n", nNumVertices);
		for (j = 0; j < nNumVertices; j++)
		{
			msVertex *pVertex = msMesh_GetVertexAt(pMesh, j);
			msVec3 Vertex;
			msVertex_GetVertex(pVertex, Vertex);
			fprintf(file, "%g %g %g\n", Vertex[0], Vertex[1], Vertex[2]);
		}

		// triangles
		int nNumTriangles = msMesh_GetTriangleCount(pMesh);
		fprintf(file, "%d\n", nNumTriangles);
        for (j = 0; j < nNumTriangles; j++)
        {
            msTriangle *pTriangle = msMesh_GetTriangleAt(pMesh, j);
			msTriangleEx *pTriangleEx = msMesh_GetTriangleExAt(pMesh, j);

            word nIndices[3];
            msTriangle_GetVertexIndices (pTriangle, nIndices);

			msVec3 Normals[3];
			msVec2 TexCoords[3];
			for (v = 0; v < 3; v++)
			{
				msTriangleEx_GetNormal(pTriangleEx, v, Normals[v]);
				msTriangleEx_GetTexCoord(pTriangleEx, v, TexCoords[v]);
			}

			int nSmoothingGroup = msTriangle_GetSmoothingGroup(pTriangle);

			fprintf(file, "%d %d %d %d %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
				nSmoothingGroup,
				nIndices[0], nIndices[1], nIndices[2],
				Normals[0][0], Normals[0][1], Normals[0][2],
				Normals[1][0], Normals[1][1], Normals[1][2],
				Normals[2][0], Normals[2][1], Normals[2][2],
				TexCoords[0][0], TexCoords[0][1],
				TexCoords[1][0], TexCoords[1][1],
				TexCoords[2][0], TexCoords[2][1]
				);
        }
    }

    fclose (file);

    // dont' forget to destroy the model
    msModel_Destroy (pModel);

    return 0;
}
