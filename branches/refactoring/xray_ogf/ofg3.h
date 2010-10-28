//
// OGF v3 - builds 1098, 1114, 1154, 1xxx
//
// OGF chunk ids.
enum ogf_chunk_id {
	OGF_HEADER		= 0x1,

	OGF3_TEXTURE		= 0x2,
	OGF3_TEXTURE_L		= 0x3,
	OGF3_CHILD_REFS		= 0x5,
	OGF3_BBOX		= 0x6,
	OGF3_VERTICES		= 0x7,
	OGF3_INDICES		= 0x8,
	OGF3_LODDATA		= 0x9,	// not sure
	OGF3_VCONTAINER		= 0xa,
	OGF3_BSPHERE		= 0xb,
	OGF3_CHILDREN_L		= 0xc,
	OGF3_S_BONE_NAMES	= 0xd,
	OGF3_S_MOTIONS		= 0xe,
	OGF3_DPATCH		= 0xf,	// guessed name
	OGF3_LODS		= 0x10,	// guessed name
	OGF3_CHILDREN		= 0x11,
	OGF3_S_SMPARAMS		= 0x12,
};

// OGF header definitions.
enum ogf_version {
	OGF3_VERSION	= 3,
};

enum ogf_model_type {
	MT3_NORMAL		= 0,	// Fvisual
	MT3_HIERRARHY		= 0x1,	// FHierrarhyVisual
	MT3_PROGRESSIVE		= 0x2,	// FProgressiveFixedVisual
	MT3_SKELETON_GEOMDEF_PM	= 0x3,	// CSkeletonX_PM
	MT3_SKELETON_ANIM	= 0x4,	// CKinematics
	MT3_DETAIL_PATCH	= 0x6,	// FDetailPatch
	MT3_SKELETON_GEOMDEF_ST	= 0x7,	// CSkeletonX_ST
	MT3_CACHED		= 0x8,	// FCached
	MT3_PARTICLE		= 0x9,	// CPSVisual
	MT3_PROGRESSIVE2	= 0xa,	// FProgressive
};

// Vertex formats.
enum ogf_vertex_format {
	OGF_VERTEXFORMAT_FVF		= 0x112,
	OGF3_VERTEXFORMAT_FVF_1L	= 0x12071980,
};

struct ChunkIFF
{
	unsigned int type, size;
};

struct ogf_bbox {
	struct vector3f		min;
	struct vector3f		max;
};

struct ogf_bsphere {
	struct vector3f		c;
	float			r;
};

struct OGFHeader_v3
{
 unsigned char format_version;
 unsigned char type;
 unsigned short unknown;
};

struct vector2f {
	float	x,y;
};

struct vector3f {
	float	x,y,z;
};

struct OGFVertex_v0a // 36 (0x24), OGF_VERTEXFORMAT_FVF
{
	struct vector3f p; // point
	struct vector3f n; // normal
	float u,v; // texture coordinates
  float unknown; // ???
};

struct OGFVertex_v1a // 64 (0x3C), OGF3_VERTEXFORMAT_FVF_1L
{
	struct vector3f p; // point
	struct vector3f n; // normal
	struct vector3f t; // tangent (for bump mapping)
	struct vector3f b; // binoral (for bump mapping)
	float w;
	float u,v; // texture coordinates
	short matrix0;
	short matrix1;
};

// ***	OGF3_VERTICES	***

struct vertices_v0a
{
	unsigned int type = OGF_VERTEXFORMAT_FVF;
  unsigned int numvertices;
  struct OGFVertex_v0a[];
}
struct vertices_v1a
{
	unsigned int type = OGF4_VERTEXFORMAT_FVF_1L;
  unsigned int numvertices;
  struct OGFVertex_v1a[];
}

// ***	OGF3_INDICES	***

struct indices
{
  unsigned int numindices;
  unsigned short ind[];
}
