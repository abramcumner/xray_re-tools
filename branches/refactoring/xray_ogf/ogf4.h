//
// OGF v4 - builds 2215, 2945, 2939, 2947+
// OGF v4+ - builds 3120, 3456+
//
// OGF chunk ids.
enum ogf_chunk_id {
	OGF_HEADER		= 0x1,

	OGF4_TEXTURE		= 0x2,
	OGF4_VERTICES		= 0x3,
	OGF4_INDICES		= 0x4,
	OGF4_P_MAP		= 0x5,
	OGF4_SWIDATA		= 0x6,
	OGF4_VCONTAINER		= 0x7,
	OGF4_ICONTAINER		= 0x8,
	OGF4_CHILDREN		= 0x9,
	OGF4_CHILDREN_L		= 0xa,
	OGF4_LODDEF2		= 0xb,
	OGF4_TREEDEF2		= 0xc,
	OGF4_S_BONE_NAMES	= 0xd,
	OGF4_S_MOTIONS		= 0xe,
	OGF4_S_SMPARAMS		= 0xf,
	OGF4_S_IKDATA		= 0x10,
	OGF4_S_USERDATA		= 0x11,
	OGF4_S_DESC		= 0x12,
	OGF4_S_MOTION_REFS_0	= 0x13,	// pre-CS format
	OGF4_SWICONTAINER	= 0x14,
	OGF4_GCONTAINER		= 0x15,
	OGF4_FASTPATH		= 0x16,
	OGF4_S_LODS		= 0x17,
	OGF4_S_MOTION_REFS_1	= 0x18,	// introduced in clear sky
};

// OGF header definitions.
enum ogf_version {
	OGF4_VERSION	= 4,
};

enum ogf_model_type {
	MT4_NORMAL		= 0,	// Fvisual
	MT4_HIERRARHY		= 0x1,	// FHierrarhyVisual
	MT4_PROGRESSIVE		= 0x2,	// FProgressive
	MT4_SKELETON_ANIM	= 0x3,	// CKinematicsAnimated
	MT4_SKELETON_GEOMDEF_PM	= 0x4,	// CSkeletonX_PM
	MT4_SKELETON_GEOMDEF_ST	= 0x5,	// CSkeletonX_ST
	MT4_LOD			= 0x6,	// FLOD
	MT4_TREE_ST		= 0x7,	// FTreeVisual_ST
	MT4_PARTICLE_EFFECT	= 0x8,	// PS::CParticleEffect
	MT4_PARTICLE_GROUP	= 0x9,	// PS::CParticleGroup
	MT4_SKELETON_RIGID	= 0xa,	// CKinematics
	MT4_TREE_PM		= 0xb,	// FTreeVisual_PM

	MT4_OMF			= 0x40,	// fake model type to distinguish .omf
};

// Vertex formats.
enum ogf_vertex_format {
	OGF_VERTEXFORMAT_FVF		= 0x112,// D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
	OGF4_VERTEXFORMAT_FVF_1L	= 0x12071980,
	OGF4_VERTEXFORMAT_FVF_2L	= 0x240e3300,
	OGF4_VERTEXFORMAT_FVF_NL	= 0x36154c80,

	OGF4_VERTEXFORMAT_FVF_1L_CS	= 0x1,
	OGF4_VERTEXFORMAT_FVF_2L_CS	= 0x2,
	OGF4_VERTEXFORMAT_FVF_3L_CS	= 0x3,
	OGF4_VERTEXFORMAT_FVF_4L_CS	= 0x4,
};

struct ChunkIFF
{
	unsigned int type, size;
};

struct vector2f {
	float	x,y;
};

struct vector3f {
	float	x,y,z;
};

struct ogf_bbox {
	struct vector3f		min;
	struct vector3f		max;
};

struct ogf_bsphere {
	struct vector3f		c;
	float			r;
};

struct OGFHeader_v4 // 0x2C (44)
{
 unsigned char format_version;
 unsigned char type;
 unsigned short shader_id;
 struct ogf_bbox		bb;
 struct ogf_bsphere	bs;
};

struct OGFVertex_v0 // 32 (0x20), OGF_VERTEXFORMAT_FVF (x-ray actor_editor export)
{
	struct vector3f p; // point
	struct vector3f n; // normal
	float u,v;
};

struct OGFVertex_v1 // 60 (0x3C), OGF4_VERTEXFORMAT_FVF_1L, OGF4_VERTEXFORMAT_FVF_1L_CS
{
	struct vector3f p; // point
	struct vector3f n; // normal
	struct vector3f t; // tangent (for bump mapping)
	struct vector3f b; // binoral (for bump mapping)
	float u,v; // texture coordinates
	unsigned int bone;
};

struct OGFVertex_v2 // 64 (0x40), OGF4_VERTEXFORMAT_FVF_2L, OGF4_VERTEXFORMAT_FVF_2L_CS
{
	unsigned short bone[2];
	struct vector3f p; // point
	struct vector3f n; // normal
	struct vector3f t; // tangent (for bump mapping)
	struct vector3f b; // binoral (for bump mapping)
	float w; // weight
	float u,v; // texture coordinates
};

struct OGFVertex_v3 // 70 (0x46), OGF4_VERTEXFORMAT_FVF_3L, OGF4_VERTEXFORMAT_FVF_3L_CS
{
  unsigned short bone[3]; 
	struct vector3f p; // point
	struct vector3f n; // normal
	struct vector3f t; // tangent (for bump mapping)
	struct vector3f b; // binoral (for bump mapping)
	float w[2];
	float u,v; // texture coordinates
};

struct OGFVertex_v4 // 76 (0x4C), 	OGF4_VERTEXFORMAT_FVF_4L_CS
{
  unsigned short matrix[4]; 
	struct vector3f p; // point
	struct vector3f n; // normal
	struct vector3f t; // tangent (for bump mapping)
	struct vector3f b; // binoral (for bump mapping)
	float w[3];
	float u,v; // texture coordinates
};

// ***	OGF4_VERTICES	***

struct vertices_v0
{
	unsigned int type = OGF_VERTEXFORMAT_FVF;
  unsigned int numvertices;
  struct OGFVertex_v0[];
}
struct vertices_v1
{
	unsigned int type = OGF4_VERTEXFORMAT_FVF_1L = OGF4_VERTEXFORMAT_FVF_1L_CS;
  unsigned int numvertices;
  struct OGFVertex_v2[];
}
struct vertices_v2
{
	unsigned int type = OGF4_VERTEXFORMAT_FVF_2L = OGF4_VERTEXFORMAT_FVF_2L_CS;
  unsigned int numvertices;
  struct OGFVertex_v2[];
}
struct vertices_v3
{
	unsigned int type = OGF4_VERTEXFORMAT_FVF_3L = OGF4_VERTEXFORMAT_FVF_3L_CS;
  unsigned int numvertices;
  struct OGFVertex_v3[];
}
struct vertices_v4
{
	unsigned int type = OGF4_VERTEXFORMAT_FVF_4L_CS;
  unsigned int numvertices;
  struct OGFVertex_v4[];
}

// ***	OGF4_INDICES	***

struct indices
{
  unsigned int numindices;
  unsigned short ind[];
}

struct swidata {
  unsigned int m_reserved[4];
  unsigned int swicout;

  unsigned int offset;  // repeat swicount times
  unsigned short num_tris;
  unsigned short num_verts;
};

struct slide_window {
	unsigned int		offset;
	unsigned short		num_tris;
	unsigned short		num_verts;
};

struct matrix33f {
	float	_11, _12, _13;
	float	_21, _22, _23;
	float	_31, _32, _33;
};

struct obb {
	struct matrix33f	rotate;
	struct vector3f		translate;
	struct vector3f		halfsize;
};

struct sphere {
	struct vector3f		p;
	float			r;
};

struct cylinder {
	struct vector3f		center;
	struct vector3f		direction;
	float			height;
	float			radius;
};

struct s_joint_limit {
	struct vector2f		limit;
	float			spring_factor;
	float			dumping_factor;
};

struct s_joint_ik_data {
	unsigned int		type;
	struct s_joint_limit	limits[3];
	float			spring_factor;
	float			damping_factor;
	unsigned int		ik_flags;
	float			break_force;
	float			break_torque;
	float			friction;
};

struct s_bone_shape {
	unsigned short		type;
	unsigned short		flags;
	struct obb		box;
	struct sphere		sphere;
	struct cylinder		cylinder;
};

struct motion_def {
	unsigned short		bone_or_part;
	unsigned short		motion;
	float			speed;
	float			power;
	float			accrue;
	float			falloff;
};

struct ogf_bone_data {
	const char		*name;
	const char		*parent_name;
	struct obb		bbox;
	int		partition;	// really?
	const char		*game_mtl_name;
	struct s_bone_shape	shape;
	struct s_joint_ik_data	joint_ik_data;
	struct vector3f		bind_rotation;
	struct vector3f		bind_position;
	float			mass;
	struct vector3f		center_of_mass;
};

struct bone
{
	// OGF_S_BONE_NAMES and OGF_S_IKDATA chunks
	unsigned		m_bone_count;
	struct ogf_bone_data	*m_bones;
};
