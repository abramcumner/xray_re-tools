#include <conio.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

struct OGFHeader
{
 unsigned char format_version;
 unsigned char type;
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

struct vector4f {
	float	x,y,z,w;
};

struct ogf_bbox {
	struct vector3f		min;
	struct vector3f		max;
};

struct ogf_bsphere {
	struct vector3f		c;
	float			r;
};

struct OGFHeader_v3 // old
{
	char format_version;
	char type;
	short unknown;
};

struct OGFHeader_v4 // 0x2C (44)
{
 unsigned char format_version;
 unsigned char type;
 unsigned short shader_id;
 struct ogf_bbox		bb;
 struct ogf_bsphere	bs;
};

struct TextData // 0x02
{
	char data[1024];
};

struct Vertices // 0x03
{
	int	ident;
	int	count;
	unsigned short	*vtx;
};

struct Indices
{
	int	count;
	unsigned short	*idx;
};

struct OGFVertex_v0a // 36 (0x24), OGF_VERTEXFORMAT_FVF
{
	struct vector3f p; // point
	struct vector3f n; // normal
	float u,v; // texture coordinates
	unsigned int bone;
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

struct vector4s {
	short	x,y,z,w;
};


FILE *outputFile,*outputFile2;
char destFlag=0, j=0, subid=0,modeltype=0;
short k=1;

char txtdata[1024];

int DecodeChunk_v4(ChunkIFF chunk,FILE *inputFile)
{
  vector4s n0;
  vector4f n1;
	Vertices v;
	Indices i;
	int cnt;
	int bones;
	char c;
	switch (chunk.type)
	{
		// 0x01 OGF_HEADER
		// 0x09 data is subchunk
		// 0x12 OGF_S_DESC, data is modelname ( char )
		
		// sub: 0x01 OGF_HEADER
		// sub: 0x02 OGF_TEXTURE
		// sub: 0x03 OGF_VERTICES
		// sub: 0x04 OGF_INDICES
		// sub: 0x05 OGF4_P_MAP
		// sub: 0x06 OGF_SWIDATA
		
	case 0x01:
		{
				OGFHeader_v4 header;
				fread(&header,chunk.size,1,inputFile);
				if (destFlag) printf("h: 0x%02X%02X, ",header.format_version,header.type); else {
					printf(" * 0x01 OGF_HEADER: version is 0x%02X,",header.format_version);
					printf(" type: 0x%02X,",header.type);
					printf(" shader_id: 0x%04X\n",header.shader_id);
					printf(" bbox (%f,%f,%f) - (%f,%f,%f)\n",header.bb.min.x,header.bb.min.y,header.bb.min.z,
          header.bb.max.x,header.bb.max.y,header.bb.max.z);
					printf(" bsphere (%f %f %f), r = %f\n",header.bs.c.x,header.bs.c.y,header.bs.c.z,header.bs.r);
          modeltype = 1; // 2xxx+
          }
			return 1;
		}
	case 0x2:
		{
			fread(&txtdata,chunk.size,1,inputFile);
			if (destFlag){ 

		if(!outputFile2) outputFile2 = fopen("ogfmodel.mtl","wb");
		if(!outputFile2)
		{
			printf("MTL: cannot open for write.\n");
			return 0;
		} else
		fprintf(outputFile2,"newmtl material%d\n  Ka  0.5000  0.5000  0.5000\n  Kd  1.0000  1.0000  1.0000\n  illum 1\n  Map_Ka %s.dds\n  Map_Kd %s.dds\n",j,txtdata,txtdata);

printf("b: %s.dds, ",txtdata); 
	  	fprintf(outputFile,"o objMesh%d\n",j);
}
			else
				printf(" * 0x02 OGF_TEXTURE: %s.dds\n",txtdata);
			return 1;
		}
	case 0x03:
		{
			fread(&v,8,1,inputFile);
			if (destFlag) printf("t=0x%08X, v=%d, ", v.ident,v.count); else
				printf(" * 0x03 OGF_VERTICES: id=0x%08X, verts=%d\n", v.ident,v.count); 
  	  if (destFlag) fprintf(outputFile,"# vertices : %d\n",v.count);
			if ((v.ident == 0x1) || (v.ident == 0x12071980))
			{
				OGFVertex_v1 *vtx = new OGFVertex_v1[v.count];
				fread(vtx,v.count*sizeof(OGFVertex_v1),1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "v %f %f %f\n",vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vn %f %f %f\n",vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vt %f %f\n",vtx[cnt].u,vtx[cnt].v); 
				} else 
				for (cnt = 0; cnt < v.count; cnt++) 
				  //printf("%05d v %f %f %f vn %f %f %f vt %f %f b %d\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				   //vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].u,vtx[cnt].v,vtx[cnt].bone);
				  printf("%05d v %f %f %f %f %f %f b %d\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				   vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].bone);
				delete [] vtx;
			} else if ((v.ident == 0x2) || (v.ident == 0x240e3300))
			{
				OGFVertex_v2 *vtx = new OGFVertex_v2[v.count];
				fread(vtx,v.count*sizeof(OGFVertex_v2),1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "v %f %f %f\n",vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vn %f %f %f\n",vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vt %f %f\n",vtx[cnt].u,vtx[cnt].v); 
				} else 
				for (cnt = 0; cnt < v.count; cnt++) 
				  printf("%05d v %f %f %f vn %f %f %f vt %f %f b %u %u\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				   vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].u,vtx[cnt].v,vtx[cnt].bone[0],vtx[cnt].bone[1]);
				delete [] vtx;
			} else if (v.ident == 0x3)
			{
				OGFVertex_v3 *vtx = new OGFVertex_v3[v.count];
				fread(vtx,v.count*sizeof(OGFVertex_v3),1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "v %f %f %f\n",vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vn %f %f %f\n",vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vt %f %f\n",vtx[cnt].u,vtx[cnt].v); 
				} else 
				for (cnt = 0; cnt < v.count; cnt++) 
				  printf("%05d v %f %f %f vn %f %f %f vt %f %f b %u %u %u\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				   vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].u,vtx[cnt].v,vtx[cnt].bone[0],vtx[cnt].bone[1],vtx[cnt].bone[2]);
				delete [] vtx;
			} else if (v.ident == 0x4)
			{
				OGFVertex_v4 *vtx = new OGFVertex_v4[v.count];
				fread(vtx,v.count*sizeof(OGFVertex_v4),1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "v %f %f %f\n",vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vn %f %f %f\n",vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vt %f %f\n",vtx[cnt].u,vtx[cnt].v); 
				} else 
				for (cnt = 0; cnt < v.count; cnt++) 
				  printf("%05d v %f %f %f vn %f %f %f vt %f %f\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				   vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].u,vtx[cnt].v);
				delete [] vtx;
			} else if (v.ident == 0x00000112)
			{
				OGFVertex_v0 *vtx = new OGFVertex_v0[v.count];
				fread(vtx,v.count*sizeof(OGFVertex_v0),1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "v %f %f %f\n",vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vn %f %f %f\n",vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vt %f %f\n",vtx[cnt].u,vtx[cnt].v); 
				} else 
				for (cnt = 0; cnt < v.count; cnt++) 
				  printf("%05d v %f %f %f vn %f %f %f vt %f %f\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				   vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].u,vtx[cnt].v);
				delete [] vtx;
			}
			return 1;
		}
	case 0x04:
		{
			fread(&i.count,4,1,inputFile);
			if (destFlag) {
				printf("i=%d", i.count);
				fprintf(outputFile,"g objGroup%d\nusemtl material%d\n",j,j);
			} else printf(" * 0x04 OGF_INDICES: indices=%d\n", i.count);
			if (destFlag) {
    	fprintf(outputFile,"# faces : %d\n",i.count/3);
    	fprintf(outputFile,"# indices : %d\n",i.count);
    	}
			i.idx = new unsigned short[i.count];
			fread(i.idx,sizeof(unsigned short)*i.count,1,inputFile);
			if (destFlag)	for (cnt = 0; cnt < i.count; cnt+=3)
				fprintf(outputFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
				i.idx[cnt]+k,i.idx[cnt]+k,i.idx[cnt]+k,
				i.idx[cnt+1]+k,i.idx[cnt+1]+k,i.idx[cnt+1]+k,
				i.idx[cnt+2]+k,i.idx[cnt+2]+k,i.idx[cnt+2]+k);
			//else for (cnt = 0; cnt < i.count; cnt++) printf("%05d %d\n",cnt,i.idx[cnt]);
			delete i.idx;
      k += v.count;	j++;
			return 1;
		}
	case 0x05:{
		if (!destFlag) printf(" * 0x05 OGF4_P_MAP\n"); 
		return 0;}
	case 0x06:{
		if (!destFlag) printf(" * 0x06 OGF_SWIDATA\n"); 
			swidata swi;
			fread(&swi,sizeof(swidata),1,inputFile);
      printf(" +++ swicount=%d, vtx count=%d, idx count=%d, idx offset=%d\n", 
				swi.swicout, swi.num_verts, swi.num_tris, swi.offset);
			swi.swicout--; while (swi.swicout)
				fread(&swi.offset,8,1,inputFile), swi.swicout--; // read rest of all
		return 1;}
	case 0x07: { // old_ogf
		if (!destFlag) printf(" * 0x07 OGF_VCONTAINER\n"); 
		return 0;}
	case 0x08: { // old_ogf
		if (!destFlag) printf(" * 0x08 OGF_ICONTAINER\n"); 
		return 0;}
	case 0x0D:{
		if (!destFlag) printf(" * 0x0D OGF_S_BONE_NAMES\n"); 
			fread(&bones,4,1,inputFile);
      printf(" +++ bone count=%d\n", bones);
			char *tmpdata = new char[chunk.size-4];
			fread(tmpdata,chunk.size-4,1,inputFile);
			int p = 0;
			for (int w = 0; w < bones; w++)
			{
       printf("[%d] - name: '%s'",w,&tmpdata[p]);
       while(tmpdata[p] != 0x00) p++;
       p++;
       if (w)
       {
       printf(", parent_name: '%s'\n",&tmpdata[p]);
       while(tmpdata[p] != 0x00) p++;
       }
       else printf("\n");
       p++;
       p+= 60;
      }
     delete [] tmpdata; 
		return 1;}
	case 0x0E:{
		int p=0,q,frames,m;
    if (!destFlag) printf(" * 0x0E OGF_S_MOTIONS\n");
 			 char *tmpdata = new char[chunk.size];
 			 fread(tmpdata,chunk.size,1,inputFile);
			 p+=8;
			 memcpy(&m,&tmpdata[p],4);
			 p+=4;
       printf(" +++ motions = %u +++\n",m);
			 for (int w = 0;w < m; w++)
			 {
			 memcpy(&q,&tmpdata[p],4);
       p+=4;
			 memcpy(&cnt,&tmpdata[p],4);
       p+=4;
       printf("[%u] - name: '%s', size = %u,",q,&tmpdata[p],cnt);
       while(tmpdata[p] != 0x00) {p++; cnt--;}
       p++;cnt--;
			 memcpy(&frames,&tmpdata[p],4);
       printf(" frames = %u,",frames);
       p++;cnt--;
			 memcpy(&c,&tmpdata[p],1);
       printf(" type = %u\n",c);
       //printf("   [%u] bones\n", bones);
       for (int f = 0;f < bones;f++)
       {
       //printf("    [%u] frames\n", frames);
       for (int e = 0;e < frames;e++)
       {
       p+=4;cnt-=4; // int32
			 memcpy(&n0,&tmpdata[p],sizeof(vector4s));
			 n1.x = (float)n0.x*1/32767;
			 n1.y = (float)n0.y*1/32767;
			 n1.z = (float)n0.z*1/32767;
			 n1.w = (float)n0.w*1/32767;
       // printf("      [%u] n(%f,%f,%f,%f)\n", e, n1.x, n1.y, n1.z, n1.w);
       p+=8;cnt-=8; // vector4s
       //getch();
       }
       //getch();
       }

       p+=cnt;
       //printf("      +++ %u\n", cnt);

       //getch();
       if (p == chunk.size) break;
       }

      delete [] tmpdata; 
		return 1;}
	case 0x10:{
		if (!destFlag) printf(" * 0x10 OGF_S_IKDATA\n"); 
       vector3f f3;
       float f;

			char *tmpdata = new char[chunk.size];
			fread(tmpdata,chunk.size,1,inputFile);
			int p = 0;
			for (int w = 0; w < cnt; w++)
			{
	  	 p+=4;
       printf("[%d] - name: '%s'\n",w,&tmpdata[p]);
       while(tmpdata[p] != 0x00) p++;
       p++;

       p+= 228-40;
       memcpy(&f3,&tmpdata[p],sizeof(f3)); p+=sizeof(f3);
       printf("     rot(%f,%f,%f)\n",f3.x,f3.y,f3.z); 
       memcpy(&f3,&tmpdata[p],sizeof(f3)); p+=sizeof(f3);
       printf("     pos(%f,%f,%f)\n",f3.x,f3.y,f3.z); 
       memcpy(&f,&tmpdata[p],sizeof(f)); p+=sizeof(f);
       memcpy(&f3,&tmpdata[p],sizeof(f3)); p+=sizeof(f3);
      }
		return 1;}
	case 0x11:{
  	if (!destFlag) printf(" * 0x11 OGF_S_USERDATA\n");
			return 0;}
	case 0x12:
		{
    if (destFlag) printf("o: ?, "); else
    printf(" * 0x12 OGF_S_DESC\n");
		return 0;
		}
	default:{
		if (!destFlag) printf(" * 0x%02X OGF_unknown_chunk\n",chunk.type); 
		return 0;}
}
}

/////////////////////////////////////////////////////////

int DecodeChunk_v3(ChunkIFF chunk,FILE *inputFile)
{
	int cnt;
  Vertices v;
	Indices i;
	switch (chunk.type)
	{
		// 0x01 OGF_HEADER
		// 0x09 data is subchunk
		// 0x12 OGF_S_DESC, data is modelname ( char )
		
		// sub: 0x01 OGF_HEADER
		// sub: 0x02 OGF_TEXTURE
		// sub: 0x03 OGF_VERTICES
		// sub: 0x04 OGF_INDICES
		// sub: 0x06 OGF_SWIDATA
		
	case 0x01:
		{
      if (chunk.size != sizeof(OGFHeader_v3)) { printf("!!! incorrect header block\n"); return 1; }
				OGFHeader_v3 header;
  			fread(&header,chunk.size,1,inputFile);
				if (destFlag) printf("h: 0x%02X%02X, ",header.format_version,header.type); else {
					printf(" * 0x01 OGF_HEADER: version is 0x%02X,",header.format_version);
					printf(" type: 0x%02X\n",header.type);
					}
          modeltype = 0; // older one
			return 1;
		}
	case 0x2:
		{
			fread(&txtdata,chunk.size,1,inputFile);
			if (destFlag){ 

		if(!outputFile2) outputFile2 = fopen("ogfmodel.mtl","wb");
		if(!outputFile2)
		{
			printf("MTL: cannot open for write.\n");
			return 0;
		} else
		fprintf(outputFile2,"newmtl material%d\n  Ka  0.5000  0.5000  0.5000\n  Kd  1.0000  1.0000  1.0000\n  illum 1\n  Map_Ka %s.dds\n  Map_Kd %s.dds\n",j,txtdata,txtdata);

printf("b: %s.dds, ",txtdata); 
	  	fprintf(outputFile,"o objMesh%d\n",j);
}
			else
				printf(" * 0x02 OGF_TEXTURE: %s.dds\n",txtdata);
			return 1;
		}
	case 0x03:{
		if (!destFlag) printf(" * 0x03 OGF3_TEXTURE_L\n"); 
		return 0;}
	case 0x05:{
		if (!destFlag) printf(" * 0x05 OGF3_CHILD_REFS\n"); 
		return 0;}
	case 0x06:{
		if (!destFlag) printf(" * 0x06 OGF3_BBOX "); 
		ogf_bbox bb;
		fread(&bb,sizeof(ogf_bbox),1,inputFile);
    if (!destFlag) 
		printf("bbox (%f,%f,%f) - (%f,%f,%f)\n",bb.min.x,bb.min.y,bb.min.z,
     bb.max.x,bb.max.y,bb.max.z);
		return 1;}
	case 0x07: // old_ogf
		{
			fread(&v,8,1,inputFile);
			if (destFlag) printf("t=0x%08X, v=%d, ", v.ident,v.count); else
				printf(" * 0x07 OGF_VCONTAINER: id=0x%08X, verts=%d\n", v.ident,v.count); 
  	  if (destFlag) fprintf(outputFile,"# vertices : %d\n",v.count);
			if ((v.ident == 0x1) || (v.ident == 0x12071980))
			{
				OGFVertex_v0a *vtx = new OGFVertex_v0a[v.count];
				fread(vtx,v.count*sizeof(OGFVertex_v0a),1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "v %f %f %f\n",vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vn %f %f %f\n",vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vt %f %f\n",vtx[cnt].u,vtx[cnt].v); 
				} else 
				for (cnt = 0; cnt < v.count; cnt++) 
				 //  printf("%05d v %f %f %f vn %f %f %f vt %f %f b %d\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				 //  vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].u,vtx[cnt].v,vtx[cnt].bone);
				  printf("%05d v %f %f %f %f %f %f b %d\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				   vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].bone);
				delete [] vtx;
				return 1;
			} else if ((v.ident == 0x2) || (v.ident == 0x240e3300))
			{
				OGFVertex_v2 *vtx = new OGFVertex_v2[v.count];
				fread(vtx,v.count*sizeof(OGFVertex_v2),1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "v %f %f %f\n",vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vn %f %f %f\n",vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z); 
					for (cnt = 0; cnt < v.count; cnt++) fprintf(outputFile, "vt %f %f\n",vtx[cnt].u,vtx[cnt].v); 
				} else 
				for (cnt = 0; cnt < v.count; cnt++) 
				  printf("%05d v %f %f %f vn %f %f %f vt %f %f\n",cnt,vtx[cnt].p.x,vtx[cnt].p.y,vtx[cnt].p.z,
				   vtx[cnt].n.x,vtx[cnt].n.y,vtx[cnt].n.z,vtx[cnt].u,vtx[cnt].v);
				delete [] vtx;
				return 1;
			}
		}
	case 0x08: // old_ogf
		{
			fread(&i.count,4,1,inputFile);
			if (destFlag) {
				printf("i=%d", i.count);
				fprintf(outputFile,"g objGroup%d\nusemtl material%d\n",j,j);
			} else printf(" * 0x08 OGF_ICONTAINER: indices=%d\n", i.count);
			if (destFlag) {
    	fprintf(outputFile,"# faces : %d\n",i.count/3);
    	fprintf(outputFile,"# indices : %d\n",i.count);
    	}
			i.idx = new unsigned short[i.count];
			fread(i.idx,sizeof(unsigned short)*i.count,1,inputFile);
			if (destFlag)	for (cnt = 0; cnt < i.count; cnt+=3)
				fprintf(outputFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
				i.idx[cnt]+k,i.idx[cnt]+k,i.idx[cnt]+k,
				i.idx[cnt+1]+k,i.idx[cnt+1]+k,i.idx[cnt+1]+k,
				i.idx[cnt+2]+k,i.idx[cnt+2]+k,i.idx[cnt+2]+k);
			//else for (cnt = 0; cnt < i.count; cnt++) printf("%05d %d\n",cnt,i.idx[cnt]);
			delete i.idx;
      k += v.count;	j++;
			return 1;
		}
	case 0x09:{
		if (!destFlag) printf(" * 0x09 OGF3_LODDATA???\n"); 
		return 0;}
	case 0x0D:{
		if (!destFlag) printf(" * 0x0D OGF3_S_BONE_NAMES\n"); 
		return 0;}
	case 0x0E:{
    if (!destFlag) printf(" * 0x0E OGF3_S_MOTIONS\n");
		return 0;}
	case 0x10:{
		if (!destFlag) printf(" * 0x10 OGF3_LODS\n"); 
		return 0;}
	default:{
		if (!destFlag) printf(" * 0x%02X OGF_unknown_chunk\n",chunk.type); 
		return 0;}
}
}

/////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
  OGFHeader oh;
  ChunkIFF chunk;
  int value, i, 
		subfullsize=0, subsubfullsize=0, fullsize=0;
	char fileName[256];
  FILE *inputFile;
	
	if (argc < 2) 
  {
		printf("X-Ray OGF to OBJECT (OGF2OBJ) converter [201012xx]\n\n");
		printf("usage: ogf2obj srcfile.ogf [ destfile.object ]\nif destfile.object is omitted - only dump will be provided\n");
		return 1;
	}
	if (strcpy((char*)fileName,argv[1]) == 0) 
  {
		printf("X-Ray OGF2OBJ: no arguments given.\n");
		return 1;
  }
  inputFile = fopen(fileName,"rb");
	if(!inputFile)
  {
		printf("X-Ray OGF2OBJ: cannot open file for read.\n");
		return 1;
  }
	if (strcpy((char*)fileName,argv[2]) == 0) 
	{
		printf("X-Ray OGF2OBJ: output to screen.\n");
	} 
else 
  {
	destFlag = 1;
		outputFile = fopen(fileName,"wb");
		if(!outputFile)
		{
			printf("OGF: cannot open file for write.\n");
			return 0;
		}
	}
  if (destFlag) fputs("mtllib ogfmodel.mtl\n",outputFile);
	
	fseek(inputFile,8,SEEK_SET);
  fread( &oh, 4, 1, inputFile );
  printf("X-Ray OGF v%d (type 0x%02X)\n",oh.format_version,oh.type);

	fseek(inputFile,0,SEEK_SET);

	if (oh.format_version == 4) 
	{ // v4
	
	for (;;)
	{
		if (fread( &chunk.type, 4, 1, inputFile ) == 0) break;
		if ((chunk.type !=9)) { //  || (modeltype == 1)
			fread( &chunk.size, 4, 1, inputFile );
			if (!destFlag) printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
			if (DecodeChunk_v4(chunk,inputFile) == 0)
				for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
				fullsize+= 8;fullsize+= chunk.size;
		}
		else {
			fread( &chunk.size, 4, 1, inputFile );
			if (!destFlag) printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
			fullsize+= 8;fullsize+= chunk.size;
			// sub#1
			subfullsize = chunk.size;
			for (;;)
			{
				fread( &chunk, 8, 1, inputFile );
				if (!destFlag) printf("--> SubChunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
				// fread( &chunksz, 4, 1, inputFile );
				subfullsize-= 8;subfullsize-= chunk.size;
				if (chunk.type == subid) {
					// sub#2
					subsubfullsize = chunk.size; printf("\n");
					for (;;)
					{
						fread( &chunk, 8, 1, inputFile );
						if (!destFlag) printf("----> Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
						if (DecodeChunk_v4(chunk,inputFile) == 0)
							for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
							subsubfullsize-= 8;subsubfullsize-= chunk.size;
							if (!subsubfullsize) { subid++; break; }
					}
				} else {
					if (DecodeChunk_v4(chunk,inputFile) == 0)
						for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
				}
				// sub#2
				if (!subfullsize) break;
			}
			// sub#1
		}
  }
	if (!destFlag) printf("Full size is 0x%08X (%u)\n",fullsize,fullsize);
	}
	else if (oh.format_version == 3) 
	{ // v3

	for (;;)
	{
		if (fread( &chunk.type, 4, 1, inputFile ) == 0) break;
		if ((chunk.type !=0x11)) { //   || (modeltype == 0)
			fread( &chunk.size, 4, 1, inputFile );
			if (!destFlag) printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
			if (DecodeChunk_v3(chunk,inputFile) == 0)
				for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
				fullsize+= 8;fullsize+= chunk.size;
		}
		else {
			fread( &chunk.size, 4, 1, inputFile );
			if (!destFlag) printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
			fullsize+= 8;fullsize+= chunk.size;
			// sub#1
			subfullsize = chunk.size;
			for (;;)
			{
				fread( &chunk, 8, 1, inputFile );
				if (!destFlag) printf("--> SubChunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
				// fread( &chunksz, 4, 1, inputFile );
				subfullsize-= 8;subfullsize-= chunk.size;
				if (chunk.type == subid) {
					// sub#2
					subsubfullsize = chunk.size; printf("\n");
					for (;;)
					{
						fread( &chunk, 8, 1, inputFile );
						if (!destFlag) printf("----> Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
						if (DecodeChunk_v3(chunk,inputFile) == 0)
							for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
							subsubfullsize-= 8;subsubfullsize-= chunk.size;
							if (!subsubfullsize) { subid++; break; }
					}
				} else {
					if (DecodeChunk_v3(chunk,inputFile) == 0)
						for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
				}
				// sub#2
				if (!subfullsize) break;
			}
			// sub#1
		}
  }
	if (!destFlag) printf("Full size is 0x%08X (%u)\n",fullsize,fullsize);
	}
  else printf("!!! unknown type\n");

  if (inputFile) fclose(inputFile);
	if (outputFile) fclose(outputFile);
	if (outputFile2) fclose(outputFile2);

	return 0;
}