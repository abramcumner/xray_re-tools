enum eobj_chunk_id {
	EOBJ_CHUNK_MAIN		= 0x7777,

	EOBJ_CHUNK_VERSION	= 0x0900,
	EOBJ_CHUNK_FLAGS	= 0x0903,
	EOBJ_CHUNK_SURFACES_0	= 0x0905,	// old format
	EOBJ_CHUNK_SURFACES_1	= 0x0906,	// old format
	EOBJ_CHUNK_SURFACES_2	= 0x0907,
	EOBJ_CHUNK_MESHES	= 0x0910,
	EOBJ_CHUNK_0911		= 0x0911,	// ignored by AE
	EOBJ_CHUNK_USERDATA	= 0x0912,
	EOBJ_CHUNK_BONES_0	= 0x0913,	// old format
	EOBJ_CHUNK_MOTIONS	= 0x0916,
	EOBJ_CHUNK_SHADERS_0	= 0x0918,	// old format
	EOBJ_CHUNK_PARTITIONS_0	= 0x0919,	// old format
	EOBJ_CHUNK_TRANSFORM	= 0x0920,
	EOBJ_CHUNK_BONES_1	= 0x0921,
	EOBJ_CHUNK_REVISION	= 0x0922,	// file revision
	EOBJ_CHUNK_PARTITIONS_1	= 0x0923,
	EOBJ_CHUNK_MOTION_REFS	= 0x0924,
	EOBJ_CHUNK_LOD_REF	= 0x0925,	// LOD\Reference
};

enum eobj_clip_chunk_id {
	EOBJ_CLIP_VERSION_CHUNK	= 0x9000,
	EOBJ_CLIP_DATA_CHUNK	= 0x9001,
};

const uint16_t EMESH_VERSION = 0x11;

enum emesh_chunk_id {
	EMESH_CHUNK_VERSION	= 0x1000,
	EMESH_CHUNK_MESHNAME	= 0x1001,
	EMESH_CHUNK_FLAGS	= 0x1002,
	EMESH_CHUNK_BBOX	= 0x1004,
	EMESH_CHUNK_VERTS	= 0x1005,
	EMESH_CHUNK_FACES	= 0x1006,
	EMESH_CHUNK_VMAPS_0	= 0x1007,
	EMESH_CHUNK_VMREFS	= 0x1008,
	EMESH_CHUNK_SFACE	= 0x1009,
	EMESH_CHUNK_OPTIONS	= 0x1010,
	EMESH_CHUNK_VMAPS_1	= 0x1011,
	EMESH_CHUNK_VMAPS_2	= 0x1012,
	EMESH_CHUNK_SG		= 0x1013,
};

struct OBJFace
{
	int v0, vref0, v1, vref1, v2, vref2; 
};

struct VMRefs
{
	unsigned int tmap[3]; 
	unsigned int ind[3]; 
	unsigned int tx[3]; 
	unsigned int ty[3]; 
};

struct Faces
{
	unsigned int	count;
	OBJFace	*fcs;
};

struct uvmap // 0x09 (x09), 0x1008 EMESH_CHUNK_VMREFS
{
 unsigned char unk; // count
 unsigned int tabind; // vmap_index, link to map
 unsigned int txtind; // index, vmap index to uv/w
};

struct uvmap0 // 0x11 (17), 0x1008 EMESH_CHUNK_VMREFS, another one
{
 unsigned char unk; // count
 unsigned int unk1;
 unsigned int unk2;
 unsigned int tabind; // vmap_index, link to map
 unsigned int txtind; // index, vmap index to uv/w
};

struct uvmap1 // 0x19 (25), 0x1008 EMESH_CHUNK_VMREFS, another one
{
 unsigned char unk; // count
 unsigned int tabind; // vmap_index, link to map
 unsigned int txtind; // index, vmap index to uv/w
 unsigned int unk1; 
 unsigned int unk2;
 unsigned int unk3;
 unsigned int unk4;
};

struct uvcoord // 0x1012 EMESH_CHUNK_VMAPS_2
{
 float u,v;
};

struct txmap // 0x1012 EMESH_CHUNK_VMAPS_2
{
 char name[8]; // = "Texture\0"
 unsigned char unk; // dim
 unsigned short tabnum; // char polymap, char type (vmtUV	= 0; vmtWeight	= 1;)
 unsigned int txcount; // size
...
 uvcoord vm[txcount]; // u,v
 int vindices[txcount];
 if polymap { int pindices[txcount]; }
...

};

struct sface2 // 0x0907 EOBJ_CHUNK_SURFACES_2
{
 unsigned int matcount; // material count
 ...
 char mat_name[]; // Name
 char shader_name[]; // ShaderName
 char shader_xrlc_name[]; // ShaderXRLCName
 char game_mtl_name[]; // GameMtlName
 char texture_name[]; // TextureName
 char vmap_name[]; // = "Texture\0"
 unsigned int m_flags;
 unsigned int _fvf;
 unsigned int unk; // = 1
 ...
}