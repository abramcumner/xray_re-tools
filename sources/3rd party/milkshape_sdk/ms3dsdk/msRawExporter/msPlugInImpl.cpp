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
    return cMsPlugIn::eTypeExport;
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
        ::MessageBox (NULL, "The model is empty!  Nothing exported!", "RAW Export", MB_OK | MB_ICONWARNING);
        return 0;
    }

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
    ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = "Export RAW";

    if (!::GetSaveFileName (&ofn))
        return 0;

    //
    // export
    //
    FILE *file = fopen (szFile, "wt");
    if (!file)
        return -1;

    int i, j;
    for (i = 0; i < msModel_GetMeshCount (pModel); i++)
    {
        msMesh *pMesh = msModel_GetMeshAt (pModel, i);
        char szName[64];
        msMesh_GetName (pMesh, szName, 64);

        fprintf (file, "%s\n", szName);

        for (j = 0; j < msMesh_GetTriangleCount (pMesh); j++)
        {
            msTriangle *pTriangle = msMesh_GetTriangleAt (pMesh, j);
            word nIndices[3];
            msTriangle_GetVertexIndices (pTriangle, nIndices);

            msVertex *pVertex;
            pVertex = msMesh_GetVertexAt (pMesh, nIndices[0]);
            fprintf (file, "%f %f %f   ", pVertex->Vertex[0], pVertex->Vertex[1], pVertex->Vertex[2]);
            pVertex = msMesh_GetVertexAt (pMesh, nIndices[1]);
            fprintf (file, "%f %f %f   ", pVertex->Vertex[0], pVertex->Vertex[1], pVertex->Vertex[2]);
            pVertex = msMesh_GetVertexAt (pMesh, nIndices[2]);
            fprintf (file, "%f %f %f\n", pVertex->Vertex[0], pVertex->Vertex[1], pVertex->Vertex[2]);
        }
    }

    fclose (file);

    // dont' forget to destroy the model
    msModel_Destroy (pModel);

    return 0;
}
