//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
class CLtxReader;

//-------------------------------------------------------------------------------------------------
struct SWayPoint
{
    char*     pName;
    float     x,y,z;
    unsigned  flags;
    unsigned  lvid;
    unsigned  gvid;
    POINT     pt;
};

//-------------------------------------------------------------------------------------------------
struct SWayLink
{
    unsigned  from;
    unsigned  to;
    unsigned  prob;
};

//-------------------------------------------------------------------------------------------------
class CWayObject
{
public:
    char*       m_pName;
    int         m_iPoints;
    SWayPoint*  m_pPoints;
    int         m_iLinks;
    SWayLink*   m_pLinks;

public:
    CWayObject();
    ~CWayObject();

    void Clean();
    bool Read(CLtxReader* pReader, LPCSTR pName);
};

//-------------------------------------------------------------------------------------------------
struct SShape
{
    char      type;  // 0 - ספונא, 1 - box
    float     x,y,z;
    union{
        // ספונא
        struct{
            float  r;
        };
        // box
        struct{
            float  matrix[3][3];
        };
    };
};

//-------------------------------------------------------------------------------------------------
class CAnyObject
{
public:
    char        m_chKey[64];
    char*       m_pName;
    char*       m_pSection;

    float       x,y,z;
    unsigned    lvid;
    unsigned    gvid;
    POINT       pt;

    int         m_iShapesCount;
    SShape*     m_pShapes;
    int         m_uRestrType;

public:
    CAnyObject();
    ~CAnyObject();

    void Clean();
    bool Read(CLtxReader* pReader, LPCSTR pName);
};

//-------------------------------------------------------------------------------------------------
class CLtxReader
{
public:
    char   m_chFilename[MAX_PATH];
    //int    m_iFileSize;
    //char*  m_pFileBuff;
    int    m_iBuffer;
    char*  m_pBuffer;

    int    m_iSectionsMaxCount;
    int    m_iSections;
    int*   m_pSections;

public:
	CLtxReader();
	virtual ~CLtxReader();

    void Close();
    bool Open(LPCSTR pFilename);

    //int  GetStringSize(int iOffset);
    //bool IsSection(int iOffset, int iSize);
    //int  ItemsCount(char* pString);
    //int  FindSectionOffset(LPCSTR pSection);

    //int  GetSectionCount();
    //bool GetSectionName(int index, char* pName, int iMaxSize);

    //bool ReadSection(LPCSTR pSection, char* pData, int iMaxSize);
    //bool ReadString(LPCSTR pSection, LPCSTR pKey, char* chValue, int iMaxSize);
    //bool ReadVector(LPCSTR pSection, LPCSTR pKey, float* px, float* py, float* pz);

    int   GetSectionCount();
    char* GetSectionName(int index);

    bool ReadString(LPCSTR pSection, LPCSTR pKey, char* chValue, DWORD dwValueMaxSize);
    bool ReadVector(LPCSTR pSection, LPCSTR pKey, float* px, float* py, float* pz);
};

//-------------------------------------------------------------------------------------------------
