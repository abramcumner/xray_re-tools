#ifndef __OGFDEFS_H__
#define __OGFDEFS_H__

#include <stdint.h>

// chunk ids
#define OGF_HEADER		0x1
#define OGF_TEXTURE		0x2
#define OGF_VERTICES		0x3
#define OGF_INDICES		0x4
#define OGF_P_MAP		0x5
#define OGF_SWIDATA		0x6
#define OGF_VCONTAINER		0x7
#define OGF_ICONTAINER		0x8
#define OGF_CHILDREN		0x9
#define OGF_CHILDREN_L		0xa
#define OGF_LODDEF2		0xb
#define OGF_TREEDEF2		0xc
#define OGF_S_BONE_NAMES	0xd
#define OGF_S_MOTIONS		0xe
#define OGF_S_SMPARAMS		0xf
#define OGF_S_IKDATA		0x10
#define OGF_S_USERDATA		0x11
#define OGF_S_DESC		0x12
#define OGF_S_MOTION_REFS	0x13
#define OGF_SWICONTAINER	0x14
#define OGF_GCONTAINER		0x15
#define OGF_FASTPATH		0x16
#define OGF_S_LODS		0x17

// model types
#define MT_NORMAL		0x0
#define MT_HIERRARHY		0x1
#define MT_PROGRESSIVE		0x2
#define MT_SKELETON_ANIM	0x3
#define MT_SKELETON_GEOMDEF_PM	0x4
#define MT_SKELETON_GEOMDEF_ST	0x5
#define MT_LOD			0x6
#define MT_TREE_ST		0x7
#define MT_PARTICLE_EFFECT	0x8
#define MT_PARTICLE_GROUP	0x9
#define MT_SKELETON_RIGID	0xa
#define MT_TREE_PM		0xb

// vertex formats
#define OGF_VERTEXFORMAT_FVF_1L	0x12071980
#define OGF_VERTEXFORMAT_FVF_2L	0x240e3300
#define OGF_VERTEXFORMAT_FVF_NL	0x36154c80

// s_joint_ik_data type
#define JT_RIGID		0x0
#define JT_CLOTH		0x1
#define JT_JOINT		0x2
#define JT_WHEEL		0x3
#define JT_NONE			0x4
#define JT_SLIDER		0x5

// s_joint_ik_data ik_flags
#define JF_BREAKABLE		0x1

// shape type
#define ST_NONE			0x0
#define ST_BOX			0x1
#define ST_SPHERE		0x2
#define ST_CYLINDER		0x3

// shape flags
#define SF_NO_PICKABLE		0x1
#define SF_REMOVE_AFTER_BREAK	0x2
#define SF_NO_PHYSICS		0x4

struct vector2f {
	float	x, y;
};

struct vector3f {
	float	x, y, z;
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

struct ogf_bbox {
	struct vector3f		min;
	struct vector3f		max;
};

struct ogf_bsphere {
	struct vector3f		c;
	float			r;
};

struct ogf_header {
	uint8_t			format_version;
	uint8_t			type;
	uint16_t		shader_id;
	struct ogf_bbox		bb;
	struct ogf_bsphere	bs;
};

struct vert_boned_1w {
	struct vector3f		p;
	struct vector3f		n;
	struct vector3f		t;
	struct vector3f		b;
	float			u;
	float			v;
	uint32_t		matrix;
};

struct vert_boned_2w {
	uint16_t		matrix0;
	uint16_t		matrix1;
	struct vector3f		p;
	struct vector3f		n;
	struct vector3f		t;
	struct vector3f		b;
	float			w;
	float			u;
	float			v;
};

struct slide_window {
	uint32_t		offset;
	uint16_t		num_tris;
	uint16_t		num_verts;
};

struct s_joint_limit {
	struct vector2f		limit;
	float			spring_factor;
	float			dumping_factor;
};

struct s_joint_ik_data {
	uint32_t		type;
	struct s_joint_limit	limits[3];
	float			spring_factor;
	float			damping_factor;
	uint32_t		ik_flags;
	float			break_force;
	float			break_torque;
	float			friction;
};

struct s_bone_shape {
	uint16_t		type;
	uint16_t		flags;
	struct obb		box;
	struct sphere		sphere;
	struct cylinder		cylinder;
};

struct motion_def {
	uint16_t		bone_or_part;
	uint16_t		motion;
	float			speed;
	float			power;
	float			accrue;
	float			falloff;
};

#endif
