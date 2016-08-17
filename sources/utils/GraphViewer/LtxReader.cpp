//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "GraphViewer.h"
#include "LtxReader.h"

//-------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
CWayObject::CWayObject()
{
    m_pName = NULL;
    m_iPoints = 0;
    m_pPoints = NULL;
    m_iLinks = 0;
    m_pLinks = NULL;
}

//-------------------------------------------------------------------------------------------------
CWayObject::~CWayObject()
{
    Clean();
}

//-------------------------------------------------------------------------------------------------
void CWayObject::Clean()
{
    if (m_pName)
    {
        delete m_pName;
        m_pName = NULL;
    }

    if (m_pPoints)
    {
        while (m_iPoints > 0)
        {
            m_iPoints--;
            if (m_pPoints[m_iPoints].pName)
                delete m_pPoints[m_iPoints].pName;
        }

        delete m_pPoints;
        m_pPoints = NULL;
    }
    else
    {
        m_iPoints = 0;
    }

    m_iLinks = 0;
    if (m_pLinks)
    {
        delete m_pLinks;
        m_pLinks = NULL;
    }
}

//-------------------------------------------------------------------------------------------------
bool CWayObject::Read(CLtxReader* pReader, LPCSTR pName)
{
    Clean();

    char chKey[32];
    char chTemp[512];
    if (!pReader->ReadString(pName, "points", chTemp, 512))
        return false;

    m_pName = new char[strlen(pName)+1];
    if (!m_pName)
    {
        Clean();
        return false;
    }
    strcpy(m_pName, pName);

    char* ptr = strtok(chTemp, ",");
    while (ptr)
    {
        m_iPoints++;
        ptr = strtok(NULL, ",");
    }

    if (m_iPoints <= 0)
    {
        Clean();
        return false;
    }

    m_pPoints = new SWayPoint[m_iPoints];
    if (!m_pPoints)
    {
        Clean();
        return false;
    }
    memset(m_pPoints, 0, m_iPoints*sizeof(SWayPoint));

    for (int i=0; i<m_iPoints; i++)
    {
        // имя
        sprintf(chKey, "p%d:name", i);
        if (!pReader->ReadString(pName, chKey, chTemp, 512) || (chTemp[0] == 0))
        {
            Clean();
            return false;
        }
        m_pPoints[i].pName = new char[strlen(chTemp)+1];
        if (!m_pPoints[i].pName)
        {
            Clean();
            return false;
        }
        strcpy(m_pPoints[i].pName, chTemp);

        // флаги
        sprintf(chKey, "p%d:flags", i);
        if (pReader->ReadString(pName, chKey, chTemp, 512) && (chTemp[0] != 0))
        {
            m_pPoints[i].flags = strtoul(chTemp, NULL, 16);
        }

        // позиция
        sprintf(chKey, "p%d:position", i);
        if (!pReader->ReadString(pName, chKey, chTemp, 512) || (chTemp[0] == 0))
        {
            Clean();
            return false;
        }
        ptr = strtok(chTemp, ",");
        if (!ptr)
        {
            Clean();
            return false;
        }
        m_pPoints[i].x = atof(ptr);
        ptr = strtok(NULL, ",");
        if (!ptr)
        {
            Clean();
            return false;
        }
        m_pPoints[i].y = atof(ptr);
        ptr = strtok(NULL, ",");
        if (!ptr)
        {
            Clean();
            return false;
        }
        m_pPoints[i].z = atof(ptr);

        // gvid
        sprintf(chKey, "p%d:game_vertex_id", i);
        if (!pReader->ReadString(pName, chKey, chTemp, 512) || (chTemp[0] == 0))
        {
            Clean();
            return false;
        }
        m_pPoints[i].gvid = (unsigned) atoi(chTemp);

        // lvid
        sprintf(chKey, "p%d:level_vertex_id", i);
        if (!pReader->ReadString(pName, chKey, chTemp, 512) || (chTemp[0] == 0))
        {
            Clean();
            return false;
        }
        m_pPoints[i].lvid = (unsigned) atoi(chTemp);

        // подсчитываем связи
        sprintf(chKey, "p%d:links", i);
        if (pReader->ReadString(pName, chKey, chTemp, 512) && (chTemp[0] != 0))
        {
            ptr = strtok(chTemp, ",");
            while (ptr)
            {
                m_iLinks++;
                ptr = strtok(NULL, ",");
            }
        }
    }

    if (m_iLinks > 0)
    {
        m_pLinks = new SWayLink[m_iLinks];
        if (!m_pLinks)
        {
            Clean();
            return false;
        }
        memset(m_pLinks, 0, m_iLinks*sizeof(SWayLink));

        int curr = 0;
        // читаем связи
        for (int i=0; i<m_iPoints; i++)
        {
            sprintf(chKey, "p%d:links", i);
            if (pReader->ReadString(pName, chKey, chTemp, 512) && (chTemp[0] != 0))
            {
                ptr = strtok(chTemp, ",");
                while (ptr)
                {
                    m_pLinks[curr].from = (unsigned) i;
                    m_pLinks[curr].to = atoi(ptr+1);
                    if (ptr[2] == '(')
                    {
                        m_pLinks[curr].prob = atoi(ptr+3);
                    }
                    else if (ptr[3] == '(')
                    {
                        m_pLinks[curr].prob = atoi(ptr+4);
                    }
                    else
                    {
                        Clean();
                        return false;
                    }
                    curr++;
                    ptr = strtok(NULL, ",");
                }
            }
        }
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
CAnyObject::CAnyObject()
{
    m_pName = NULL;
    m_pSection = NULL;

    m_iShapesCount = 0;
    m_pShapes = NULL;
    m_uRestrType = 0;
}

//-------------------------------------------------------------------------------------------------
CAnyObject::~CAnyObject()
{
    Clean();
}

//-------------------------------------------------------------------------------------------------
void CAnyObject::Clean()
{
    if (m_pName)
    {
        delete[] m_pName;
        m_pName = NULL;
    }

    if (m_pSection)
    {
        delete[] m_pSection;
        m_pSection = NULL;
    }

    if (m_pShapes)
    {
        delete[] m_pShapes;
        m_pShapes = NULL;
    }

    m_iShapesCount = 0;
    m_uRestrType = 0;
}

//-------------------------------------------------------------------------------------------------
bool CAnyObject::Read(CLtxReader* pReader, LPCSTR pName)
{
    Clean();

    strncpy(m_chKey, pName, 64);
    m_chKey[63] = 0;

    char chKey[32];
    char chTemp[512];

    // имя
    if (!pReader->ReadString(pName, "name", chTemp, 512))
    {
        Clean();
        return false;
    }
    m_pName = new char[strlen(chTemp)+1];
    if (!m_pName)
    {
        Clean();
        return false;
    }
    strcpy(m_pName, chTemp);

    // секция
    if (!pReader->ReadString(pName, "section_name", chTemp, 512))
    {
        Clean();
        return false;
    }
    m_pSection = new char[strlen(chTemp)+1];
    if (!m_pSection)
    {
        Clean();
        return false;
    }
    strcpy(m_pSection, chTemp);

    // позиция
    if (!pReader->ReadVector(pName, "position", &x, &y, &z))
    {
        Clean();
        return false;
    }

    // gvid
    if (!pReader->ReadString(pName, "game_vertex_id", chTemp, 512) || (chTemp[0] == 0))
    {
        Clean();
        return false;
    }
    gvid = (unsigned) atoi(chTemp);

    // lvid
    if (!pReader->ReadString(pName, "level_vertex_id", chTemp, 512) || (chTemp[0] == 0))
    {
        Clean();
        return false;
    }
    lvid = (unsigned) atoi(chTemp);

    // читаем форму, если есть
    if (pReader->ReadString(pName, "shapes", chTemp, 512) && (chTemp[0] != 0))
    {
        // подсчитываем количество шейпов
        char* ptr = strtok(chTemp, ",");
        while (ptr)
        {
            m_iShapesCount++;
            ptr = strtok(NULL, ",");
        }

        if (m_iShapesCount <= 0)
        {
            Clean();
            return false;
        }

        m_pShapes = new SShape[m_iShapesCount];
        if (!m_pShapes)
        {
            Clean();
            return false;
        }

        // читаем шейпы
        for (int i=0; i<m_iShapesCount; i++)
        {
            sprintf(chKey, "shape%d:type", i);
            if (!pReader->ReadString(pName, chKey, chTemp, 512) || (chTemp[0] == 0))
            {
                Clean();
                return false;
            }

            if (strcmp(chTemp, "sphere") == 0)
            {
                m_pShapes[i].type = 0;

                sprintf(chKey, "shape%d:offset", i);
                if (!pReader->ReadVector(pName, chKey, &m_pShapes[i].x, &m_pShapes[i].y, &m_pShapes[i].z))
                {
                    Clean();
                    return false;
                }

                sprintf(chKey, "shape%d:radius", i);
                if (!pReader->ReadString(pName, chKey, chTemp, 512) || (chTemp[0] == 0))
                {
                    Clean();
                    return false;
                }
                m_pShapes[i].r = atof(chTemp);

            }
            else if (strcmp(chTemp, "box") == 0)
            {
                m_pShapes[i].type = 1;

                sprintf(chKey, "shape%d:axis_x", i);
                if (!pReader->ReadVector(pName, chKey, &m_pShapes[i].matrix[0][0], &m_pShapes[i].matrix[0][1], &m_pShapes[i].matrix[0][2]))
                {
                    Clean();
                    return false;
                }

                sprintf(chKey, "shape%d:axis_y", i);
                if (!pReader->ReadVector(pName, chKey, &m_pShapes[i].matrix[1][0], &m_pShapes[i].matrix[1][1], &m_pShapes[i].matrix[1][2]))
                {
                    Clean();
                    return false;
                }

                sprintf(chKey, "shape%d:axis_z", i);
                if (!pReader->ReadVector(pName, chKey, &m_pShapes[i].matrix[2][0], &m_pShapes[i].matrix[2][1], &m_pShapes[i].matrix[2][2]))
                {
                    Clean();
                    return false;
                }

                sprintf(chKey, "shape%d:offset", i);
                if (!pReader->ReadVector(pName, chKey, &m_pShapes[i].x, &m_pShapes[i].y, &m_pShapes[i].z))
                {
                    Clean();
                    return false;
                }
            }
            else
            {
                Clean();
                return false;
            }
        }

        if (!pReader->ReadString(pName, "restrictor_type", chTemp, 512) || (chTemp[0] == 0))
            m_uRestrType = -1;
        else
            m_uRestrType = (unsigned) atoi(chTemp);
    }

    return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
CLtxReader::CLtxReader()
{
    m_iBuffer = 32*1024; // максимальный размер буфера под имена секций
    m_pBuffer = NULL;

    m_iSectionsMaxCount = 4*1024; // максимальное количество секций
    m_iSections = 0;
    m_pSections = NULL;
}

//-------------------------------------------------------------------------------------------------
CLtxReader::~CLtxReader()
{
    Close();

    if (m_pBuffer)
    {
        delete[] m_pBuffer;
        m_pBuffer = NULL;
    }

    if (m_pSections)
    {
        delete[] m_pSections;
        m_pSections = NULL;
    }
}

//-------------------------------------------------------------------------------------------------
void CLtxReader::Close()
{
    m_iSections = 0;
}

//-------------------------------------------------------------------------------------------------
bool CLtxReader::Open(LPCSTR pFilename)
{
    Close();

    if (!m_pBuffer)
    {
        m_pBuffer = new char[m_iBuffer];
        if (!m_pBuffer)
        {
            return false;
        }
    }

    if (!m_pSections)
    {
        m_pSections = new int[m_iSectionsMaxCount];
        if (!m_pSections)
        {
            return false;
        }
    }

    DWORD dwSize = GetPrivateProfileSectionNames(m_pBuffer, m_iBuffer, pFilename);
    if ((dwSize == -2) || (dwSize == -1) || (dwSize == 0))
    {
        return false;
    }

    m_iSections = 0;
    m_pSections[m_iSections] = 0;
    DWORD dwOffset = 0;
    while (dwOffset < dwSize)
    {
        m_pSections[m_iSections++] = (int) dwOffset;
        dwOffset += strlen(&m_pBuffer[dwOffset]) + 1;

    }

    strcpy(m_chFilename, pFilename);
    return true;
}
/*
//-------------------------------------------------------------------------------------------------
int CLtxReader::GetStringSize(int iOffset)
{
    char* ptr = m_pFileBuff + iOffset;

    char* pMultiStringBegin = "<<END";
    char* pMultiStringEnd = "END";

    int iMultiStringBegin = 0;
    int iMultiStringEnd = 0;

    bool bMultiString = false;
    bool bEndOfString = false;

    int iSize = 0;
    while (iOffset < m_iFileSize)
    {
        if (bMultiString)
        {
            if (*ptr == pMultiStringEnd[iMultiStringEnd])
            {
                iMultiStringEnd++;
                if (iMultiStringEnd >= 3)
                {
                    bMultiString = false;
                    iMultiStringBegin = 0;
                }
            }
            else
            {
                iMultiStringEnd = 0;
            }
        }
        else
        {
            if ((*ptr == '\r') || (*ptr == '\n))
            {
                bEndOfString = true;
            }
            else if (bEndOfString)
            {
                break;
            }

            if (*ptr == pMultiStringBegin[iMultiStringBegin])
            {
                iMultiStringBegin++;
                if (iMultiStringBegin >= 5)
                {
                    bMultiString = true;
                    iMultiStringEnd = 0;
                }
            }
            else
            {
                iMultiStringBegin = 0;
            }
        }

        ptr++;
        iSize++;
        iOffset++;
    }

    return iSize;
}

//-------------------------------------------------------------------------------------------------
bool CLtxReader::IsSection(int iOffset, int iSize)
{
    char* ptr = m_pFileBuff + iOffset;

    if (*ptr == '[')
    {
        for (int i=1; i<iSize; i++)
        {
            ptr++;
            if (*ptr == ']')
            {
                return true;
            }
            else if ((*ptr == '\r') || (*ptr == '\n'))
            {
                break;
            }
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
int CLtxReader::ItemsCount(char* pString)
{
    int iCount = 0;
    char* ptr = strtok(pString, ",");
    while (ptr)
    {
        ptr = strtok(NULL, ",");
        iCount++;
    }
    return iCount;
}

//-------------------------------------------------------------------------------------------------
int CLtxReader::FindSection(LPCSTR pSection)
{
    for (int i=0; i<m_iSections; i++)
    {
        if (strncmp(pSection, &m_pFileBuff[m_pSections[i]+1], strlen(pSection)) == 0)
        {
            return i;
        }
    }

    return -1;
}

//-------------------------------------------------------------------------------------------------
int CLtxReader::GetSectionCount()
{
    return m_iSections;
}

//-------------------------------------------------------------------------------------------------
bool CLtxReader::GetSectionName(int index, char* pName, int iMaxSize)
{
    if ((index < 0) || (index >= m_iSections))
        return false;

    char* ptr = m_pFileBuff + m_pSections[index] + 1;
    for (int i=0; i<iMaxSize; i++)
    {
        if (*ptr == ']')
        {
            *pName = 0;
            break;
        }

        *pName = *ptr;
        ptr++;
        pName++;
    }

    return (*pName == 0) ? true : false;
}

//-------------------------------------------------------------------------------------------------
bool CLtxReader::ReadSection(LPCSTR pSection, char* pData, int iMaxSize)
{
    int index = FindSection(pSection);
    if (index < 0)
        return false;

    int iOffset = m_pSections[index];
    int iSize = m_pSections[index+1] - iOffset;
    if (iSize >= iMaxSize)
        return false;

    strncpy(pData, &m_pFileBuff[iOffset], iSize);
    pData[iSize] = 0;
    return true;
}

//-------------------------------------------------------------------------------------------------
bool CLtxReader::ReadString(LPCSTR pSection, LPCSTR pKey, char* chValue, int iMaxSize)
{
    int index = FindSection(pSection);
    if (index < 0)
        return false;

    size_t iKey = strlen(pKey);
    int iOffset = m_pSections[index];
    int iEnd = m_pSections[index+1];
    int iSize = GetStringSize(iOffset);
    iOffset += iSize;
    while (iOffset < iEnd)
    {
        iSize = GetStringSize(iOffset);
        if (strncmp(pKey, &m_pFileBuff[iOffset], iKey) == 0)
        {
            iOffset += iKey;
            char* ptr
            return true;
        }

        iOffset += iSize;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
bool CLtxReader::ReadVector(LPCSTR pSection, LPCSTR pKey, float* px, float* py, float* pz)
{
}

//-------------------------------------------------------------------------------------------------
int CLtxReader::GetSectionCount()
{
    return m_iSections;
}
*/
//-------------------------------------------------------------------------------------------------
char* CLtxReader::GetSectionName(int index)
{
    if ((index < 0) || (index >= m_iSections))
        return NULL;

    return &m_pBuffer[m_pSections[index]];
}

//-------------------------------------------------------------------------------------------------
bool CLtxReader::ReadString(LPCSTR pSection, LPCSTR pKey, char* chValue, DWORD dwValueMaxSize)
{
    DWORD dwSize = GetPrivateProfileString(pSection, pKey, 0, chValue, dwValueMaxSize, m_chFilename);

    return (dwSize > 0);
}

//-------------------------------------------------------------------------------------------------
bool CLtxReader::ReadVector(LPCSTR pSection, LPCSTR pKey, float* px, float* py, float* pz)
{
    char chTemp[256];
    if (!ReadString(pSection, pKey, chTemp, 256) || (chTemp[0] == 0))
        return false;

    char* ptr = strtok(chTemp, ",");
    if (!ptr)
        return false;
    *px = atof(ptr);

    ptr = strtok(NULL, ",");
    if (!ptr)
        return false;
    *py = atof(ptr);

    ptr = strtok(NULL, ",");
    if (!ptr)
        return false;
    *pz = atof(ptr);

    return true;
}

//-------------------------------------------------------------------------------------------------
