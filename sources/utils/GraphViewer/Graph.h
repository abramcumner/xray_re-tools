//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
#pragma pack(push, 1)

struct POINT_3D
{
    float            x;
    float            y;
    float            z;
};

struct BOX_3D
{
    POINT_3D         p1;
    POINT_3D         p2;
};

struct GG_HEADER
{
    unsigned __int8  version;
    unsigned __int16 vertex_count;
    unsigned __int32 edge_count;
    unsigned __int32 level_point_count;
    unsigned __int8  guid[16];
    unsigned __int8  level_count;
};

struct GG_LEVEL
{
    char             level_name[256];
    POINT_3D         offset;
    unsigned __int8  level_id;
    char             section_name[256];
    unsigned __int8  guid[16];
};

struct GG_VERTEX
{
    POINT_3D         level_point;
    POINT_3D         game_point;
    unsigned __int8  level_id;
    unsigned __int32 level_vertex_id : 24;
    unsigned __int32 vertex_type_1   : 8;
    unsigned __int8  vertex_type_2   : 8;
    unsigned __int8  vertex_type_3   : 8;
    unsigned __int8  vertex_type_4   : 8;
    unsigned __int32 edge_offset;
    unsigned __int32 level_point_offset;
    unsigned __int8  edge_count;
    unsigned __int8  level_point_count;
};

struct GG_EDGE
{
    unsigned __int16 game_vertex_id;
    float            distance;
};

struct GG_LEVEL_POINT
{
    POINT_3D         point;
    unsigned __int32 level_vertex_id;
    float            distance;
};



struct AI_HEADER
{
	unsigned __int32 version;
	unsigned __int32 vertex_count;
	float            cell_size;
	float            factor_y;
	BOX_3D           box;
    unsigned __int8  guid[16];
};

struct AI_POINT
{
    union{
        unsigned __int32 neighbours1     : 23;
        struct{
            unsigned __int8  unused1[2];
            unsigned __int32 unused2     : 7;
            unsigned __int32 neighbours2 : 23;
        };
        struct{
            unsigned __int8  unused1[5];
            unsigned __int32 unused2     : 6;
            unsigned __int32 neighbours3 : 23;
        };
        struct{
            unsigned __int8  unused1[8];
            unsigned __int32 unused2     : 5;
            unsigned __int32 neighbours4 : 23;
            unsigned __int32 light       : 4;
        };
    };
    unsigned __int16 cover1 : 4;
    unsigned __int16 cover2 : 4;
    unsigned __int16 cover3 : 4;
    unsigned __int16 cover4 : 4;
	unsigned __int16 plane;
    union{
        unsigned __int32 packed_xz       : 24;
        struct{
            unsigned __int8  unused1[3];
	        unsigned __int16 packed_y;
        };
    };
};

#pragma pack(pop, 1)

//-------------------------------------------------------------------------------------------------
struct DRAW_DATA
{
    CDC*   pDC;
    int    x;
    int    y;
    int    cx;
    int    cy;
    int    iZoom;
    float  fMinY;
    float  fMaxY;
    int    iCoverMode;
};

//-------------------------------------------------------------------------------------------------
class CGraph  
{
public:
    char             m_chFilename[MAX_PATH];
    GG_HEADER        m_sHeader;
    GG_LEVEL*        m_pLevels;
    GG_VERTEX*       m_pVertexs;
    GG_EDGE*         m_pEdges;
    GG_LEVEL_POINT*  m_pLevelPoints;

    BOX_3D           m_sGridBox;
    unsigned         m_uGridCX;
    unsigned         m_uGridCY;
    unsigned         m_uGridCZ;
    unsigned*        m_pGrid;

public:
	CGraph();
	virtual ~CGraph();

    void Clean();
    bool Load(LPCSTR pFilename);
    bool LoadLevel(HANDLE hFile, GG_LEVEL* pLevel);

    void GridClean();
    bool GridInit(unsigned uLevel);

    void DrawVertexs(DRAW_DATA* pDD);
    void DrawEdges(DRAW_DATA* pDD);
    void DrawLevelPoints(DRAW_DATA* pDD);

    void DrawSelVertexs(DRAW_DATA* pDD, unsigned uVertex);
    void DrawSelEdges(DRAW_DATA* pDD, unsigned uVertex);
    void DrawSelLevelPoints(DRAW_DATA* pDD, unsigned uVertex);
};

//-------------------------------------------------------------------------------------------------
class CLevel  
{
public:
    char             m_chFilename[MAX_PATH];
    AI_HEADER        m_sHeader;
    AI_POINT*        m_pPoints;

    unsigned         m_uGridCX;
    unsigned         m_uGridCY;
    unsigned         m_uGridCZ;
    unsigned*        m_pGrid;

public:
	CLevel();
	virtual ~CLevel();

    void Clean();
    bool Load(LPCSTR pFilename);

    void GridClean();
    bool GridInit();

    void DrawVertexs(DRAW_DATA* pDD);

    void DrawSelVertexs(DRAW_DATA* pDD, unsigned uVertex);
};

//-------------------------------------------------------------------------------------------------
