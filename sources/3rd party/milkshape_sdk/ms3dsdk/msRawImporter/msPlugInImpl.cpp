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



cPlugIn::cPlugIn ()
{
    strcpy (szTitle, "RAW...");
}



cPlugIn::~cPlugIn ()
{
}



int
cPlugIn::GetType ()
{
    return cMsPlugIn::eTypeImport;
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
    char szDefExt[32] = "raw";
    char szFilter[128] = "RAW Files (*.raw)\0*.raw\0All Files (*.*)\0*.*\0\0";
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
    ofn.lpstrTitle = "Import RAW";

    if (!::GetOpenFileName (&ofn))
        return 0;

    //
    // import
    //
    FILE *file = fopen (szFile, "rt");
    if (!file)
        return -1;

	char line[256];
    msMesh *pMesh = 0;
	while (fgets (line, 256, file))
	{
        msVec3 v1, v2, v3;
		char szName[MS_MAX_NAME];
		if (sscanf (line, "%f %f %f %f %f %f %f %f %f",
			&v1[0], &v1[1], &v1[2], 
			&v2[0], &v2[1], &v2[2], 
			&v3[0], &v3[1], &v3[2]) == 9)
		{
			if (pMesh == 0)
            {
                int nMesh = msModel_AddMesh (pModel);
                pMesh = msModel_GetMeshAt (pModel, nMesh);
                msMesh_SetName (pMesh, "default");
            }

            int nTriangle = msMesh_AddTriangle (pMesh);
            msTriangle *pTriangle = msMesh_GetTriangleAt (pMesh, nTriangle);
            word nIndices[3];

            int nVertex;
            msVertex Vertex, *pVertex;
            memset (&Vertex, 0, sizeof (msVertex));

            Vertex.Vertex[0] = v1[0];
            Vertex.Vertex[1] = v1[1];
            Vertex.Vertex[2] = v1[2];
            nVertex = msMesh_FindVertex (pMesh, &Vertex);
            if (nVertex == -1)
                nVertex = msMesh_AddVertex (pMesh);
            pVertex = msMesh_GetVertexAt (pMesh, nVertex);
            msVertex_SetVertex (pVertex, Vertex.Vertex);
            nIndices[0] = nVertex;

            Vertex.Vertex[0] = v2[0];
            Vertex.Vertex[1] = v2[1];
            Vertex.Vertex[2] = v2[2];
            nVertex = msMesh_FindVertex (pMesh, &Vertex);
            if (nVertex == -1)
                nVertex = msMesh_AddVertex (pMesh);
            pVertex = msMesh_GetVertexAt (pMesh, nVertex);
            msVertex_SetVertex (pVertex, Vertex.Vertex);
            nIndices[1] = nVertex;

            Vertex.Vertex[0] = v3[0];
            Vertex.Vertex[1] = v3[1];
            Vertex.Vertex[2] = v3[2];
            nVertex = msMesh_FindVertex (pMesh, &Vertex);
            if (nVertex == -1)
                nVertex = msMesh_AddVertex (pMesh);
            pVertex = msMesh_GetVertexAt (pMesh, nVertex);
            msVertex_SetVertex (pVertex, Vertex.Vertex);
            nIndices[2] = nVertex;

            msTriangle_SetVertexIndices (pTriangle, nIndices);
		}

		else if (sscanf (line, "%s", szName) == 1)
		{
            int nMesh = msModel_AddMesh (pModel);
            pMesh = msModel_GetMeshAt (pModel, nMesh);
            msMesh_SetName (pMesh, szName);
		}
	}

    fclose (file);

    return 0;
}
