#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

unsigned int numtextures,numiblocks,numiblocks2,numvblocks, numobjblocks;
char destFlag=0, parseFlag=0;

struct vector3f
{
	float x,y,z; 
};

struct vector2f
{
	float tx,ty; 
};

struct lev_blk
{
	unsigned short type; // 1..8
	unsigned short fcomp; // compressed if = 32768
  unsigned int size;
};

struct lev_light  // 108 bytes
{
  int type0;
  int type1;
  float fRed;
  float fGreen;
  float fBlue;
  int unk[9];
  vector3f pos; // X,Z,Y
  vector3f rot; // X,Z,Y
  int unk1[2];
  float unk2[3];
  int unk3;
  float unk4;
};

struct lev_crown  // 18 bytes
{
  vector3f pos; // X,Z,Y
  float unk;
  short unk1;
};

struct ogf_bbox {
	struct vector3f		min;
	struct vector3f		max;
};

struct ogf_bsphere {
	struct vector3f		c;
	float			r;
};

struct cform_blk
{
  unsigned int version;
  unsigned int num_verts;
  unsigned int num_tris;
  ogf_bbox bb;
};

/*
struct tris_blk
{
 unsigned int tris[3];
 unsigned short matid; 
 unsigned short sec_num;
};
*/

struct VERTEXELEMENT9 // D3DVERTEXELEMENT9
{
	unsigned short Stream; 
	unsigned short Offset; 
	unsigned char Type;
/*
  D3DDECLTYPE_FLOAT1      = 0,
  D3DDECLTYPE_FLOAT2      = 1,
  D3DDECLTYPE_FLOAT3      = 2,
  D3DDECLTYPE_FLOAT4      = 3,
  D3DDECLTYPE_D3DCOLOR    = 4,
  D3DDECLTYPE_UBYTE4      = 5,
  D3DDECLTYPE_SHORT2      = 6,
  D3DDECLTYPE_SHORT4      = 7,
  D3DDECLTYPE_UBYTE4N     = 8,
  D3DDECLTYPE_SHORT2N     = 9,
  D3DDECLTYPE_SHORT4N     = 10,
  D3DDECLTYPE_USHORT2N    = 11,
  D3DDECLTYPE_USHORT4N    = 12,
  D3DDECLTYPE_UDEC3       = 13,
  D3DDECLTYPE_DEC3N       = 14,
  D3DDECLTYPE_FLOAT16_2   = 15,
  D3DDECLTYPE_FLOAT16_4   = 16,
  D3DDECLTYPE_UNUSED      = 17 
*/
	unsigned char Method;
	unsigned char Usage; 

// 0.12 - vertices (4*float) - X/Z/Y
// 3.4 - normales (4*(char-128)/128)
// 6.4 - binormales (4*(char-128)/128)
// 7.4 - tangent normales (4*(char-128)/128)
// 10.4 - ???? (4*char)
// 5.1 texcoords : (2*float)
// 5.6 texcoords : (2*short/1024)
// 5.7 texcoords : (2*short/2048) + dummy (4)
// 5.1.6 - lightmaps (2*short/32768)
	unsigned char UsageIndex; 
};

struct blk_12
{
 float xform[16];
 float c_scale_rgb[3];
 float c_scale_hemi;
 float c_scale_sun;
 float c_bias_rgb[3];
 float c_bias_hemi;
 float c_bias_sun;
};

struct blk_21
{
 unsigned int vBlk;
 unsigned int vFirst;
 unsigned int vCount;
 unsigned int iBlk;
 unsigned int iFirst;
 unsigned int iCount;
};

struct ChunkIFF
{
	unsigned int type, size;
};

struct OGFVertex_v0 // x-ray actor_editor (0x00000112)
{
	struct vector3f p;
	struct vector3f n;
	float u,v;
	float lu,lv;
};

struct OGFHeader_v4 // 0x2C (44)
{
 unsigned char format_version;
 unsigned char type;
 unsigned short shader_id;
 struct ogf_bbox		bb;
 struct ogf_bsphere	bs;
};

///////////////////////////////

#define MAX_OBJ_COUNT 30000
#define MAX_TXT_COUNT 1000

struct obj_blk // for ogf export
{
 unsigned short rangecount;
 unsigned char code;
 unsigned short texture;
 blk_21 range[2];
 OGFVertex_v0 * geom_vBlk;
 unsigned short * geom_iBlk;
/*
 OGFVertex_v0 * cform_vBlk;
 unsigned short * cform_iBlk;
*/
 char * m_dds;
 char * m_shd;
 char * m_lm_dds;
 float xform[16];
 struct ogf_bbox		bb;
 struct ogf_bsphere	bs;
 unsigned char dds_type;
 unsigned short swi;
};

obj_blk * o;
unsigned short ocount = 0;
unsigned short orange = 0;
char * tmp_m_dds[MAX_TXT_COUNT];
char * tmp_m_shd[MAX_TXT_COUNT];
char * tmp_m_lm_dds[MAX_TXT_COUNT];

struct merged_blk
{
 OGFVertex_v0 * geom_vBlk;
 unsigned short * geom_iBlk;
 unsigned int iCount;
 unsigned int vCount;
 char * m_dds;
 char * m_shd;
 unsigned short texture;
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

slide_window * swiblk;

#ifdef LINE_COMPACT
	 merged_blk mix[MAX_TXT_COUNT];
#else
	 merged_blk mix[MAX_TXT_COUNT*2];
#endif
///////////////////////////////

int DecodeChunk(lev_blk h,FILE *inputFile)
{
	printf(" DecodeChunk(0x%04X, 0x%08X)\n",h.type,h.size);

	if (h.fcomp == 0x8000) h.type = 0xFF; // force_end

	switch (h.type)
	{
	case 0x01: // level; level.geom, LEV_HEADER
  {
	int version;
	fread(&version,h.size,1,inputFile);
	printf("  level version = 0x%08X\n",version);
	// only XRLC_0x0D, XRLC_0x0E (level+.cform+.geom)
	if (version < 0x0D) printf("unsupported version :(\n"), _getch();
	// 0x08 - bld 11xx, 0x0A - bld 15xx, 0x0C - bld 18xx, 2218
	return 1;
	}

	case 0x06: // level, LEV_LIGHTS
  {
	int numlights;
  numlights = h.size/108;
	// printf("  lights = %d\n",numlights);
	lev_light *ll = new lev_light[numlights];
	fread(ll,h.size,1,inputFile);
	if (!destFlag) for (int i = 0;i< numlights; i++)
	{
	printf("  light[%d]:%d:%d pos(%f,%f,%f) rot(%f,%f,%f)\n",i,
   ll[i].type0,ll[i].type1,
   ll[i].pos.x,ll[i].pos.y,ll[i].pos.z,
   ll[i].rot.x,ll[i].rot.y,ll[i].rot.z);
	}
	delete [] ll;
	return 1;
	}

	case 0x07: // level, LEV_CORONAS
  {
	int numcrowns;
  numcrowns = h.size/18;
	if (!destFlag) printf("  crowns = %d\n",numcrowns);
	lev_crown *lc = new lev_crown[numcrowns];
	fread(lc,h.size,1,inputFile);
  if (!destFlag) for (int i = 0;i< numcrowns; i++)
	{
	printf("  crown[%d]: pos(%f,%f,%f) %f %d\n",i,
   lc[i].pos.x,lc[i].pos.y,lc[i].pos.z,
   lc[i].unk,lc[i].unk1);
	} 
	delete [] lc;
	return 1;
	}

	case 0x02: // level, LEV_SHADERS
  {
	fread(&numtextures,4,1,inputFile);
	printf("  textures = %d\n",numtextures);
  if (numtextures > MAX_TXT_COUNT) printf("!!! too many textures, maximum is %u\n",MAX_TXT_COUNT), _getch();

	char *tmpdata = new char[h.size-4];
	fread(tmpdata,h.size-4,1,inputFile);
	unsigned int p=0, plen; p++; // skip #0
	for (unsigned int i = 1;i< numtextures; i++) // start from 1!!!,0 = zero texture!!!
	{
  tmp_m_dds[i] = new char[_MAX_PATH];
  tmp_m_shd[i] = new char[_MAX_PATH];
  tmp_m_lm_dds[i] = new char[_MAX_PATH];
  if (!destFlag) printf("   [%d]: s,t,lmap0,lmap1 '%s'\n",i,&tmpdata[p]);

  plen = 0;
  while(tmpdata[p+plen] != 0x2F) plen++;
  memcpy(tmp_m_shd[i],&tmpdata[p],plen); tmp_m_shd[i][plen] = 0;
  p += plen; p++; 

  plen = 0;
  while((tmpdata[p+plen]) && (tmpdata[p+plen] != 0x2C)) plen++; //  != 0x00
  memcpy(tmp_m_dds[i],&tmpdata[p],plen); tmp_m_dds[i][plen] = 0;
  p += plen; 

  if (tmpdata[p] != 0x2C) goto no_lmap;
  p++; 

  plen = 0;
  while((tmpdata[p+plen]) && (tmpdata[p+plen] != 0x2C)) plen++; //  != 0x00
  memcpy(tmp_m_lm_dds[i],&tmpdata[p],plen); tmp_m_lm_dds[i][plen] = 0;
  p += plen;

no_lmap:
  //printf("DDS:%s\nSHD:%s\nLM_DDS:%s\n",tmp_m_dds[i],tmp_m_shd[i],tmp_m_lm_dds[i]);

  while(tmpdata[p]) p++; //  != 0x00
  p++;

	}
	delete [] tmpdata;
	return 1;
	}

	case 0x03: // level, LEV_OBJECTS
  {
  unsigned int j, k, szBlk, szAll, szObj, idObj;
  szAll = h.size;
  while (szAll > 0)
  {
  ///
	fread(&idObj,4,1,inputFile);
	fread(&szObj,4,1,inputFile);
	if (!destFlag) printf("  [ OBJECT#%04d ] size %d\n",idObj,szObj);
  unsigned int i = 0, bdone = 0; 
  orange = 0;

	for (;;)
	{
	fread(&j,4,1,inputFile);
	fread(&szBlk,4,1,inputFile);  // size
  if (!destFlag) printf("   chunk = 0x%02X, size = %u\n",j,szBlk);

  if (j > 255) printf("!!! incorrect block (%u), it is an object start\n",j), _getch();
  if (ocount > MAX_OBJ_COUNT) printf("!!! too many objects, maximum is %u\n",MAX_OBJ_COUNT), _getch();

  switch(j)
  {

/*
  code = // OGF4 based
   0 - 21,22,1 or 21,1 or 21,22,21,1 // MT4_NORMAL, brush
   1 - 1,10 // MT4_HIERRARHY, brush+entity
   2 - 21,6,22,1 or 21,6,1 or 21,6,22,21,6,1 // MT4_PROGRESSIVE, entity
   6 - 1,10,11 // MT4_LOD, greens joined
   7 - 21,12,1 // MT4_TREE_ST, greens
   B - 21,1,12,6 or 21,12,1,20 // MT4_TREE_PM, greens scaled

 subcode

 21, 22 - geom - ib,vb
  1 - base, pos(x,y,z) 
 10 - merged data ???
 11 - green  
 12 - green placement ???
 20 - swidata
*/

  case 1: 
  {
  OGFHeader_v4 hdr;
	fread(&hdr,sizeof(OGFHeader_v4),1,inputFile);
  if (!destFlag) printf("    MT4_HEAD: type 0x%02X sh_id %d bs.c(%f,%f,%f), size = %u\n",
    hdr.type,hdr.shader_id,hdr.bs.c.x,hdr.bs.c.y,hdr.bs.c.z,szBlk);
   if (destFlag) 
   if (orange) // we don't need 'no i/v objects!'
   {
   o[ocount].code = hdr.type;
   o[ocount].texture = hdr.shader_id;
   o[ocount].bb = hdr.bb;
   o[ocount].bs = hdr.bs;
   }
   if (hdr.type == 0x00) bdone = 1;
   else
   if (hdr.type == 0x01) bdone = 10;
   else
   if (hdr.type == 0x02) bdone = 1;
   else
   if (hdr.type == 0x06) bdone = 11;
   else
   if (hdr.type == 0x07) bdone = 1;
   else
   if (hdr.type == 0x0B) bdone = 20;
   else
   printf("!!! unknown object (0x%02X) !!!\n",hdr.type), _getch();

  i += szBlk;
	break;
  }

  case 20: 
  {
  unsigned int swi;
	fread(&swi,4,1,inputFile);
  if (!destFlag) printf("    MT4_SWICNTR = %u, size = %u\n",swi,szBlk);
  else o[ocount].swi = swi;
  i += szBlk;
	break;
  }

  case 21: 
  {
  blk_21 iv;
	fread(&iv,sizeof(blk_21),1,inputFile);
   if (!destFlag)  printf("    OGF4_GCONTAINER: (v:#%d,%d,%d) (i:#%d,%d,%d), size = %u\n",
    iv.vBlk,iv.vFirst,iv.vCount,iv.iBlk,iv.iFirst,iv.iCount,szBlk);
  else
  {
  o[ocount].range[orange].vBlk = iv.vBlk;
  o[ocount].range[orange].vFirst = iv.vFirst;
  o[ocount].range[orange].vCount = iv.vCount;
  o[ocount].range[orange].iBlk = iv.iBlk;
  o[ocount].range[orange].iFirst = iv.iFirst;
  o[ocount].range[orange].iCount = iv.iCount;
  o[ocount].swi = 65535; // hack
  orange++;
  }
  i += szBlk;
	break;
  }

  case 22: 
  {
  if (!destFlag) printf("    OGF4_FASTPATH: %u\n",szBlk); // cform_header
  i += szBlk;
	break;
  }

  case 6: 
  {
  if (!destFlag) printf("    OGF4_SWIDATA, len: %d\n",szBlk);
  swidata s;
	fread(&s,sizeof(swidata),1,inputFile);
	s.num_tris *= 3; // tris to idx
  if (!destFlag) printf("    swi[%u] = sz %u / off %u, swicnt = %u\n",0,s.num_tris,s.offset,s.swicout);
  else
  {
  printf("    swi[0] = sz %u (was %u) / off %u\n",s.num_tris,o[ocount].range[orange-1].iCount,s.offset);
	o[ocount].range[orange-1].iFirst += s.offset;
  o[ocount].range[orange-1].iCount = s.num_tris;
  }
  for (unsigned int m = 0; m < (szBlk-sizeof(swidata)); m++) fread(&k,1,1,inputFile); // dummy read szBlk times
  if ((i + szBlk) > szObj) szBlk = 8; // , printf("!!! blk#6: overflow fixed\n");
  i += szBlk;
	break;
  }

  case 10: 
  {
  unsigned int cnt, ob;
	fread(&cnt,4,1,inputFile);
  if (!destFlag) printf("    OGF4_CHILDREN_L, size = %d, objcnt = %u\n",szBlk,cnt);
  for (unsigned int m = 0; m < cnt; m++) { fread(&ob,4,1,inputFile);  } // printf(" %u",ob);
  i += (szBlk + 16);
  //printf("!!! blk#10: advance 16 bytes, rest = %u\n",szBlk-4-(cnt*4));
	break;
  }

  case 12: 
  {
  blk_12 iv;
	fread(&iv,sizeof(blk_12),1,inputFile);
  if (!destFlag)  printf("    OGF4_TREEDEF2: xform[16] matrix, size = %u\n", szBlk); 
  else memcpy(&o[ocount].xform,&iv,16*sizeof(float));
  i += szBlk;
	break;
  }

  case 11: 
  case 24: 
  default:
  {
  if (!destFlag)  printf("    undef_chunk[0x#%02X], len: %d\n",j,szBlk);
  for (unsigned int m = 0; m < szBlk; m++) fread(&k,1,1,inputFile); // dummy read szBlk times
  i += szBlk;
	break;
  }

  } // end of switch

  // printf("    size check (i=%u ? szObj=%u)\n", i, szObj);

  if (i == szObj)
  {
  if (!destFlag) printf("    object completed\n");
  break;
  }

  if ((bdone == j) && (i < szObj) && ((szObj - i) <= 32)) // read all the rest and break
  {
  if (!destFlag) printf("    object completed (forced EOO [%u - %u = %u])\n",szObj,i,szObj - i);
  break;
  }

  if (j == 0x14) // read all the rest and break after chunk(0x14)
  {
  if (!destFlag) printf("    null_object completed (forced EOO [%u - %u = %u])\n",szObj,i,szObj - i);
  break;
  }

  if (i > szObj) { printf("obj ended beyond limit (i %d szobj %d).\n",i,szObj); break;  }

	}

  if (destFlag) 
  {
  o[ocount].rangecount = orange;
  if ((orange) && ((o[ocount].code == 0x00) || (o[ocount].code == 0x07) || (o[ocount].code == 0x0B) || (o[ocount].code == 0x02)))
  {
  ocount++; 
  }
  orange = 
  o[ocount].rangecount =
  o[ocount].code = 
  o[ocount].texture = 0;
  }

  szAll -= szObj;
  szAll -= 8;

	} ///

	return 1;
	}

	case 0x09: // level.geom, LEV_V_BLOCKS
  {
  short dummy16;
  int dummy32, tot_vblks = 0;
  VERTEXELEMENT9 v_e[10];

	fread(&numvblocks,4,1,inputFile);
	printf("  numvblocks = %d\n",numvblocks);


	for (unsigned int cnt=0;cnt<numvblocks;cnt++) // base header
	{
  printf("    vBlock[%u]\n",cnt); // start
  unsigned short ccnt = 0;
	for (;;) // header of vblock
	{
  if (ccnt >= 10) { printf("!!!! max sz = 10 reached\n"); _getch(); }
	fread(&v_e[ccnt],2,1,inputFile); //	stream #
	if (v_e[ccnt].Stream == 255) break; // end
	fread(&v_e[ccnt].Offset,sizeof(VERTEXELEMENT9)-2,1,inputFile);
	/// printf("     Offset:%d Type:%d Usage:%d\n",v_e[ccnt].v_offset,v_e[ccnt].Type,v_e[ccnt].Usage);
	ccnt++;
	}

	fread(&dummy16,2,1,inputFile); //	padding #2
	fread(&dummy32,4,1,inputFile);
	/// if (dummy32 == 17) printf("      [+] vert_blk_start\n");
	fread(&dummy32,4,1,inputFile); // printf("       vert_blks: %d [ ... ]\n", dummy32 );
  tot_vblks += dummy32;

   OGFVertex_v0 * v_tmp = new OGFVertex_v0[dummy32];
   char * v_tmp_type = new char[dummy32];

   for (int  i = 0;i < dummy32;i++)
   {
   vector2f v5;
   short s16;
   unsigned char u8;
   float f4[4];

 	 v_tmp_type[i] = 0; // i.e. texture.dds

   for (int dcnt = 0;dcnt < ccnt;dcnt++)
   {
/*
   if (!destFlag)
		if (v_e[dcnt].Usage != 0) // p
		if (v_e[dcnt].Usage != 3) // n
		if (v_e[dcnt].Usage != 6) // t
		if (v_e[dcnt].Usage != 7) // b
    if (v_e[dcnt].UsageIndex != 1) // lm
	  printf("* t/m/u/i %u:%u:%u:%u = ",
		v_e[dcnt].Type,v_e[dcnt].Method,v_e[dcnt].Usage,
	  v_e[dcnt].UsageIndex);
*/
   switch(v_e[dcnt].Type) // D3DDECLTYPE
   {
   case 2: // D3DDECLTYPE_FLOAT3
   {
 	 fread(&f4,sizeof(float)*3,1,inputFile); 
 	 f4[3] = 1.0f;
 	 break;
 	 }
   case 4: // D3DDECLTYPE_D3DCOLOR
   {
 	 fread(&u8,1,1,inputFile); f4[0] = (float)u8;
 	 fread(&u8,1,1,inputFile); f4[1] = (float)u8;
 	 fread(&u8,1,1,inputFile); f4[2] = (float)u8;
 	 fread(&u8,1,1,inputFile); f4[3] = (float)u8;
 	 break;
 	 }
   case 6: // D3DDECLTYPE_SHORT2
   {
 	 fread(&s16,2,1,inputFile); f4[0] = (float)s16;
 	 fread(&s16,2,1,inputFile); f4[1] = (float)s16; 
 	 break;
 	 }
   case 7: // D3DDECLTYPE_SHORT4
   {
 	 fread(&s16,2,1,inputFile); f4[0] = (float)s16;
 	 fread(&s16,2,1,inputFile); f4[1] = (float)s16;
 	 fread(&s16,2,1,inputFile); f4[2] = (float)s16;
 	 fread(&s16,2,1,inputFile); f4[3] = (float)s16;
 	 break;
 	 }
   default: printf("should not get here, D3D9VE.Type: %u\n",v_e[dcnt].Type); _getch(); break;
 	 }

   switch(v_e[dcnt].Method) // D3DDECLMETHOD
   {
   case 0: break; // D3DDECLMETHOD_DEFAULT
   default: printf("should not get here, D3D9VE.Method: %u\n",v_e[dcnt].Method); _getch(); break;
 	 }

   switch(v_e[dcnt].Usage) // D3DDECLTYPE
   {
   case 0: // D3DDECLUSAGE_POSITION
   {
 	 //if (!destFlag) printf("position[%d](X,Y,Z) = %f,%f,%f\n",i,f4[0],f4[1],f4[2]);
   v_tmp[i].p.x = f4[0]; v_tmp[i].p.y = f4[1]; v_tmp[i].p.z = f4[2];
   break;
   }
   case 3: // D3DDECLUSAGE_NORMAL
   {
   v_tmp[i].n.x = (f4[0] - 128.0f) / 128.0f; 
	 v_tmp[i].n.y = (f4[1] - 128.0f) / 128.0f; 
   v_tmp[i].n.z = (f4[2] - 128.0f) / 128.0f;
	 //if (!destFlag) printf("normal[%d](X,Y,Z,W) = %f,%f,%f,%f\n",i,f4[0],f4[1],f4[2],f4[3]);
   break;
   }
   case 6: // D3DDECLUSAGE_TANGENT
   {
	 //if (!destFlag) printf("tangent[%d](X,Y,Z,W) = %f,%f,%f,%f\n",i,f4[0],f4[1],f4[2],f4[3]);
   break;
   }
   case 7: // D3DDECLUSAGE_BINORMAL
   {
	 //if (!destFlag) printf("binormal[%d](X,Y,Z,W) = %f,%f,%f,%f\n",i,f4[0],f4[1],f4[2],f4[3]);
   break;
   }
   case 10: // D3DDECLUSAGE_COLOR
   {
	 //if (!destFlag) printf("color[%d](X,Y,Z,W) = %f,%f,%f,%f\n",i,f4[0],f4[1],f4[2],f4[3]);
   break;
   }
   case 5: // D3DDECLUSAGE_TEXCOORD
   {

	 if (v_e[dcnt].UsageIndex == 0)
	 {
   if (v_e[dcnt].Type == 1)
   {
 	 //if (!destFlag) printf("texcoord[%d](U,V) = %f,%f\n",i,f4[0],f4[1]);
   v_tmp[i].u = f4[0];
   v_tmp[i].v = f4[1];
   }
   else if (v_e[dcnt].Type == 6)
	// #6 used for statics, i.e. non 0x07, 0x0B objects
   {
 	 v5.tx = f4[0] / 1024.0f; // ????
 	 v5.ty = f4[1] / 1024.0f; // ????
	 //if (!destFlag) printf("texcoord[%d](U,V) = %f,%f\n",i,v5.tx,v5.ty);
   v_tmp[i].u = v5.tx;
   v_tmp[i].v = v5.ty;
   }
   else if (v_e[dcnt].Type == 7) 
	// #7 used for "placeable" statics, i.e. 0x07, 0x0B objects
   {
 	 v5.tx = f4[0] / 2048.0f; // ????
 	 v5.ty = f4[1] / 2048.0f; // ????
	 //if (!destFlag) printf("texcoord[%d](U,V) = %f,%f\n",i,v5.tx,v5.ty);
   v_tmp[i].u = v5.tx;
   v_tmp[i].v = v5.ty;
   }
   }
   else if (v_e[dcnt].UsageIndex == 1) // _lmap_
	 {
   if (v_e[dcnt].Type == 1)
   {
 	 //if (!destFlag) printf("lmapcoord[%d](U,V) = %f,%f\n",i,f4[0],f4[1]);
   v_tmp[i].lu = f4[0];
   v_tmp[i].lv = f4[1];
// 	 v_tmp_type[i] = 1; // i.e. texture_lm.dds, uncomment to enable lmaps :)
   }
   else
   if (v_e[dcnt].Type == 6) // used for statics, i.e. non 0x07, 0x0B objects
   {
 	 v5.tx = f4[0] / 32768.0f;
 	 v5.ty = f4[1] / 32768.0f;
	 //if (!destFlag) printf("lmapcoord[%d](U,V) = %f,%f\n",i,v5.tx,v5.ty);
   v_tmp[i].lu = v5.tx;
   v_tmp[i].lv = v5.ty;
// 	 v_tmp_type[i] = 1; // i.e. texture_lm.dds, uncomment to enable lmaps :)
   }
   else
   if (v_e[dcnt].Type == 7) // used for "placeable" statics, i.e. 0x07, 0x0B objects
   {
 	 v5.tx = f4[0] / 32768.0f;
 	 v5.ty = f4[1] / 32768.0f;
	 //if (!destFlag) printf("lmapcoord[%d](U,V) = %f,%f\n",i,v5.tx,v5.ty);
   v_tmp[i].lu = v5.tx;
   v_tmp[i].lv = v5.ty;
// 	 v_tmp_type[i] = 1; // i.e. texture_lm.dds, uncomment to enable lmaps :)
   }
   } // lmap end

   break;
   }

   default: printf("should not get here, D3D9VE.Usage: %u\n",v_e[dcnt].Usage); _getch(); break;
   }
   }
   // printf("press a key to break\n"); _getch();
   }
/////////////////////////
   if (destFlag) for (unsigned int obj = 0; obj < ocount; obj++)
   if (o[obj].range[0].vBlk == cnt) 
   {
   unsigned int bsz = o[obj].range[0].vCount;
   unsigned int bst = o[obj].range[0].vFirst;
   o[obj].dds_type = v_tmp_type[bst]; // differ lmap - dds
   o[obj].geom_vBlk = new OGFVertex_v0[bsz];
   memcpy(o[obj].geom_vBlk,&v_tmp[bst],sizeof(OGFVertex_v0)*bsz);
  if ((o[obj].code == 0x0B) || (o[obj].code == 0x07))
   for (unsigned int q = 0; q < bsz; q++)
   {
   float x,y,z;
   x = o[obj].geom_vBlk[q].p.x;
   y = o[obj].geom_vBlk[q].p.y;
   z = o[obj].geom_vBlk[q].p.z;
	o[obj].geom_vBlk[q].p.x = o[obj].xform[0]*x + o[obj].xform[4]*y + o[obj].xform[8]*z + o[obj].xform[12]*1;
	o[obj].geom_vBlk[q].p.y = o[obj].xform[1]*x + o[obj].xform[5]*y + o[obj].xform[9]*z + o[obj].xform[13]*1;
	o[obj].geom_vBlk[q].p.z = o[obj].xform[2]*x + o[obj].xform[6]*y + o[obj].xform[10]*z + o[obj].xform[14]*1;
	o[obj].geom_vBlk[q].p.x += o[obj].xform[3];
	o[obj].geom_vBlk[q].p.y += o[obj].xform[7];
	o[obj].geom_vBlk[q].p.z += o[obj].xform[11];
   }
   }
/////////////////////////

  delete [] v_tmp;
  delete [] v_tmp_type;
	//printf("press a key to break\n"); _getch();
	}
  printf("    tot_vert_blks: %d\n", tot_vblks );
	return 1;
	}

	case 0x0A: // level.geom, LEV_I_BLOCKS
  {
  unsigned short ii; unsigned int tot_f = 0, tot_i = 0;
	fread(&numiblocks,4,1,inputFile);
	printf("  numiblocks = %d\n",numiblocks);
	for (unsigned int i = 0;i< numiblocks; i++)
	{
	fread(&numiblocks2,4,1,inputFile);
	unsigned short * i_tmp = new unsigned short[numiblocks2];
	printf("  iBlock[%d]: '%d faces, %d indexes'\n",i,numiblocks2/3,numiblocks2);
  tot_f += (numiblocks2/3); tot_i += numiblocks2;
	fread(i_tmp,sizeof(unsigned short)*numiblocks2,1,inputFile);

/////////////////////////
   if (destFlag) 
   for (ii = 0; ii < ocount; ii++)
   if (o[ii].range[0].iBlk == i) 
   {
   unsigned int bsz = o[ii].range[0].iCount;
   unsigned int bst = o[ii].range[0].iFirst;
   o[ii].geom_iBlk = new unsigned short[bsz];
   memcpy(o[ii].geom_iBlk,&i_tmp[bst],sizeof(unsigned short)*bsz);
   }
/////////////////////////

  delete [] i_tmp;
	}
	printf("  totals: '%d faces, %d indexes'\n",tot_f,tot_i);
	return 1;
	}

	case 0x0B: // level.geom, SWIDATA
  {
    unsigned short ii; 
	  unsigned int swcnt;
    swidata d;
		fread(&swcnt,4,1,inputFile);
		printf("  numswiblocks = %d\n",swcnt);
    swiblk = new slide_window[swcnt];
		for (unsigned int i = 0;i < swcnt; i++)
		{
		fread(&d,sizeof(swidata),1,inputFile);
    swiblk[i].num_tris = d.num_tris*3;
    swiblk[i].offset = d.offset;
    printf("    swi[%u] = sz %u / off %u, swicnt = %u\n",i,swiblk[i].num_tris,swiblk[i].offset,d.swicout);
		for (unsigned int j = 0;j < (d.swicout-1); j++) 
			fread(&d.offset,sizeof(slide_window),1,inputFile); // read rest
		}

   if (destFlag) 
   for (ii = 0; ii < ocount; ii++) 
   {
   if (o[ii].swi != 65535) // .swi filled
   printf("    *obj[0x%02X].iCnt: reducing[%u] from %u to %u\n",o[ii].code, ii,o[ii].range[0].iCount,swiblk[o[ii].swi].num_tris),
   o[ii].range[0].iCount = swiblk[o[ii].swi].num_tris,
	 memcpy(o[ii].geom_iBlk,
   &o[ii].geom_iBlk[swiblk[o[ii].swi].offset],swiblk[o[ii].swi].num_tris*sizeof(unsigned short)); // hack
   }

   delete [] swiblk;
		return 1;}

	case 0x05: // level
	case 0x08: // level, LEV_SECTORS
	case 0x0E: // ??????????
  case 0x04: // level, LEV_PORTALS
  case 0xFF: // forced_end
  {
		//printf("  UNKNOWN LEV_BLK = %d\n",h.type); 
    //_getch();
		char *tmpdata = new char[h.size];
		fread(tmpdata,h.size,1,inputFile);
		delete [] tmpdata;
		return 0;}
	default: { printf("unknown lev_blk = %d\n",h.type); return 0;}
	}
}

int main(int argc, char* argv[])
{
  lev_blk hdr;
  cform_blk hdr2;
  unsigned int i, fullsize=0;
	char fileName[256];
  FILE *inputFile, *outputFile;

 	printf("X-Ray level (2xxx+) parser-converter [201012xx]\n");

	if (argc < 2) 
  {
  	printf("usage: parse_lev < mode > [ destfile.ogf ]\n\tmode: 0 - dump only, 1 - convert to ogf, 2 - convert to ogf (optimized)\n\tdestfile.ogf: destination file (modes 1, 2 only)\n\n");
		return 1;
	}

  parseFlag = (char)atoi(argv[1]);

	if (parseFlag) 
	{
  if (strcpy((char*)fileName,argv[2]) == 0) 
  {
		printf("X-Ray PARSE_LEV: no arguments given in mode(s) 1, 2.\n"); return 1;
  }
  else destFlag = 1;
  }

  if (destFlag) o = new obj_blk[MAX_OBJ_COUNT];

  inputFile = fopen("level","rb");
	if(!inputFile)
  {
 	printf("ERR: cannot open 'level'.\n");
  return 1;
  }
  printf("------------------------------\nLoading spatial-DB (level)...\n");
	fseek(inputFile,0,SEEK_END);
  fullsize = ftell(inputFile);
	fseek(inputFile,0,SEEK_SET);
  while(fullsize)
  {
  fread( &hdr, sizeof(lev_blk), 1, inputFile );
  printf("Type 0x%04X, compress 0x%04X, size %d\n",hdr.type,hdr.fcomp,hdr.size);
  DecodeChunk(hdr,inputFile);
  fullsize -= hdr.size; fullsize -= 8;
	}
  fclose(inputFile);
  printf("loading OK\n");

  inputFile = fopen("level.geom","rb");
	if(!inputFile)
  {
 	printf("ERR: cannot open 'level.GEOM'.\n");
  return 1;
  }
  printf("------------------------------\nLoading geometry (level.GEOM)...\n");
	fseek(inputFile,0,SEEK_END);
  fullsize = ftell(inputFile);
	fseek(inputFile,0,SEEK_SET);
  while(fullsize)
  {
  fread( &hdr, sizeof(lev_blk), 1, inputFile );
  printf("Type 0x%04X, compress 0x%04X, size %d\n",hdr.type,hdr.fcomp,hdr.size);
  DecodeChunk(hdr,inputFile);
  fullsize -= hdr.size; fullsize -= 8;
	}
  fclose(inputFile);
  printf("loading OK\n");


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

  inputFile = fopen("level.cform","rb");
	if(!inputFile)
  {
 	printf("WARN: cannot open 'level.CFORM'.\n");
  goto no_cform;
  }
  printf("------------------------------\nLoading CFORM (level.CFORM)...\n");

  fread( &hdr2, sizeof(cform_blk), 1, inputFile );
  printf(" version 0x%08X, verts %d faces %d,\n bb(%f,%f,%f)-(%f,%f,%f)\n",
  hdr2.version,hdr2.num_verts,hdr2.num_tris,
  hdr2.bb.min.x,hdr2.bb.min.y,hdr2.bb.min.z,hdr2.bb.max.x,hdr2.bb.max.y,hdr2.bb.max.z);
/*
	vector3f *v = new vector3f[hdr2.num_verts];
	fread(v,sizeof(vector3f)*hdr2.num_verts,1,inputFile);
	tris_blk *idx = new tris_blk[hdr2.num_tris];
	fread(idx,sizeof(tris_blk)*hdr2.num_tris,1,inputFile);

///////////////////////////
   for (unsigned int obj = 0; obj < ocount; obj++)
   if (o[obj].rangecount == 2) 
   {
   printf("cform match with obj=%u\n",obj);

   unsigned int vsz = o[obj].range[1].vCount;
   unsigned int vst = o[obj].range[1].vFirst;
   unsigned int isz = o[obj].range[1].iCount / 3;
   unsigned int ist = o[obj].range[1].iFirst / 3;

   o[obj].cform_vBlk = new OGFVertex_v0[vsz];
   for (unsigned int b = 0; b < vsz; b++) o[obj].cform_vBlk[b].p = v[vst+b];

   o[obj].cform_iBlk = new unsigned short[isz*3];
   unsigned int b0 = 0; for (b = 0; b < isz; b++) 
   {
   //o[obj].cform_iBlk[b0] = idx[ist+b].tris[0]; // x,y,z
   //o[obj].cform_iBlk[b0+1] = idx[ist+b].tris[1];
   //o[obj].cform_iBlk[b0+2] = idx[ist+b].tris[2];
   b0 += 3;
   }
   }
///////////////////////////
  delete [] v;
  delete [] idx;
*/
  fclose(inputFile);
  printf("loading OK\n");
no_cform:

  if (!destFlag) return 0;

if (parseFlag == 1) 

{
//////////////////////////////////////////////////////////////////
/////////////////////////
   for (unsigned int obj = 0; obj < ocount; obj++)
   {
   for (i = 1; i < numtextures; i++) // from 1 !!!
   {
   if (o[obj].texture == i) 
   o[obj].m_dds = new char[_MAX_PATH], o[obj].m_shd = new char[_MAX_PATH],
   strcpy(o[obj].m_dds,tmp_m_dds[i]),strcpy(o[obj].m_shd,tmp_m_shd[i]);
   if (o[obj].dds_type) o[obj].m_lm_dds = new char[_MAX_PATH], strcpy(o[obj].m_lm_dds,tmp_m_lm_dds[i]); 
   }
   }
   for (i = 0; i < numtextures; i++)
    delete tmp_m_dds[i], delete tmp_m_lm_dds[i], delete tmp_m_shd[i];
/////////////////////////
//////////////////////////////////////////////////////////////////

  printf("parse complete\n");

  unsigned short numblocks = 0;
  unsigned int tot_i = 0, tot_v = 0;

 //for (obj = 0; obj < ocount; obj++)  -- nattefrost.. wtf :)

   for (unsigned int obj = 0; obj < ocount; obj++)  
  {
  if (o[obj].texture)
  printf("o%05u - c,t,s 0x%02X, '%s', '%s', %u, %u\n",
  obj,o[obj].code, o[obj].m_dds,o[obj].m_shd,o[obj].range[0].vCount,o[obj].range[0].iCount), numblocks++;
  else
  printf("o%05u - no texture\n",obj);
  }

  printf("final complete\n");

///////////////////////////////////////
  printf("creating '%s'\n",fileName);
  outputFile = fopen(fileName,"wb");

  ChunkIFF * ih = new ChunkIFF;
  ih->type = 0x01; // header
  ih->size = sizeof(OGFHeader_v4);
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

  OGFHeader_v4 * hdr4 = new OGFHeader_v4;
  hdr4->format_version = 4;
  hdr4->type = 1;
  hdr4->shader_id = 0;
  hdr4->bb = hdr2.bb;
  // hdr4.bs ???
	fwrite(hdr4,sizeof(OGFHeader_v4),1,outputFile);

  ih->type = 0x12; // text info
  ih->size = 0x6E; // guessed :)
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
  i = 0; 
  fputs("test_level\\test_level.object",outputFile);
  fwrite(&i,1,1,outputFile); 
  fputs("\\\\MSHOME\\Administrator",outputFile); fwrite(&i,1,1,outputFile); 
  fwrite(&i,4,1,outputFile); // fake time
  fputs("\\\\MSHOME\\Administrator",outputFile); fwrite(&i,1,1,outputFile); 
  fwrite(&i,4,1,outputFile); // fake time
  fputs("\\\\MSHOME\\Administrator",outputFile); fwrite(&i,1,1,outputFile); 
  fwrite(&i,4,1,outputFile); // fake time


  ih->type = 0x09; // childrens
  ih->size = 8 + (8+sizeof(OGFHeader_v4))+(0x14+8)+(8+((o[0].range[0].vCount*(sizeof(OGFVertex_v0)-8))+8))+(8+((o[0].range[0].iCount*2)+4));
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

  printf("making %u blocks\n",numblocks);

  unsigned int k = 0;
	for (unsigned int j = 0;j < numblocks; j++)
	{
	// child start

  ih->type = k; // children #j
  ih->size = (8+sizeof(OGFHeader_v4))+((strlen(o[j].m_dds) + strlen(o[j].m_shd) + 0x02)+8)+(8+((o[j].range[0].vCount*(sizeof(OGFVertex_v0)-8))+8))+(8+((o[j].range[0].iCount*2)+4));
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

	// .... 1.2.3.4

  ih->type = 0x01; // header
  ih->size = sizeof(OGFHeader_v4);
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
  hdr4->format_version = 4;
  hdr4->type = 0;
  hdr4->shader_id = 0;
  hdr4->bb = o[j].bb;
  hdr4->bs = o[j].bs;
	fwrite(hdr4,sizeof(OGFHeader_v4),1,outputFile);

  ih->type = 0x02; // texture
  ih->size = strlen(o[j].m_dds) + strlen(o[j].m_shd) + 0x02; 
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0;
  fputs(o[j].m_dds,outputFile); fwrite(&i,1,1,outputFile); 
  fputs(o[j].m_shd,outputFile); fwrite(&i,1,1,outputFile); 

	tot_v += o[j].range[0].vCount;
  ih->type = 0x03; // verts
  ih->size = (o[j].range[0].vCount*(sizeof(OGFVertex_v0)-8))+8;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0x00000112; // D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
  fwrite(&i,4,1,outputFile);
	i = o[j].range[0].vCount; 
  fwrite(&i,4,1,outputFile);
  for (unsigned int s = 0; s < i ; s++)
	fwrite(&o[j].geom_vBlk[s],sizeof(OGFVertex_v0)-8,1,outputFile);
	
	tot_i += o[j].range[0].iCount;
  ih->type = 0x04; // idx
  ih->size = (o[j].range[0].iCount*2)+4;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = o[j].range[0].iCount; 
  fwrite(&i,4,1,outputFile);
	fwrite(o[j].geom_iBlk,sizeof(unsigned short)*i,1,outputFile);

	printf("."); if ((k % 50)==0) printf("\n");
	k++;
	// child end 

	if (o[j].dds_type) {
	// child start

  ih->type = k; // children #j
  ih->size = (8+sizeof(OGFHeader_v4))+((strlen(o[j].m_lm_dds) + strlen(o[j].m_shd) + 0x02)+8)+(8+((o[j].range[0].vCount*(sizeof(OGFVertex_v0)-8))+8))+(8+((o[j].range[0].iCount*2)+4));
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

	// .... 1.2.3.4

  ih->type = 0x01; // header
  ih->size = sizeof(OGFHeader_v4);
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
  hdr4->format_version = 4;
  hdr4->type = 0;
  hdr4->shader_id = 0;
  hdr4->bb = o[j].bb;
  hdr4->bs = o[j].bs;
	fwrite(hdr4,sizeof(OGFHeader_v4),1,outputFile);

  ih->type = 0x02; // texture
  ih->size = strlen(o[j].m_lm_dds) + strlen(o[j].m_shd) + 0x02; 
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0;
  fputs(o[j].m_lm_dds,outputFile); fwrite(&i,1,1,outputFile);
  fputs(o[j].m_shd,outputFile); fwrite(&i,1,1,outputFile);

	tot_v += o[j].range[0].vCount;
  ih->type = 0x03; // verts
  ih->size = (o[j].range[0].vCount*(sizeof(OGFVertex_v0)-8))+8;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0x00000112; // D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
  fwrite(&i,4,1,outputFile);
	i = o[j].range[0].vCount;
  fwrite(&i,4,1,outputFile);
  for (unsigned int s = 0; s < i ; s++)
  //o[j].geom_vBlk[s].u = o[j].geom_vBlk[s].lu, o[j].geom_vBlk[s].v = o[j].geom_vBlk[s].lv,
  o[j].geom_vBlk[s].p.y+=0.1,
	fwrite(&o[j].geom_vBlk[s],sizeof(OGFVertex_v0)-8,1,outputFile);
	
	tot_i += o[j].range[0].iCount;
  ih->type = 0x04; // idx
  ih->size = (o[j].range[0].iCount*2)+4;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = o[j].range[0].iCount;
  fwrite(&i,4,1,outputFile);
	fwrite(o[j].geom_iBlk,sizeof(unsigned short)*i,1,outputFile);

	printf("*"); if ((j % 50)==0) printf("\n");
	k++;
	// child end (lmap)
	}
	}

  delete [] o;

  delete hdr4;
  delete ih;

  fclose(outputFile);
  printf("\nwriting '%s' [v %u, i %u] OK\n",fileName,tot_v,tot_i);
///////////////////////////////////////
} // parseFlag == 1
else if (parseFlag == 2) 

{

//////////////////////////////////////////////////////////////////
/////////////////////////
   for (unsigned int obj = 0; obj < ocount; obj++)
   {
   for (i = 1; i < numtextures; i++) // from 1 !!!
   {
   if (o[obj].texture == i) 
   {
   o[obj].m_dds = new char[_MAX_PATH], o[obj].m_shd = new char[_MAX_PATH],
   strcpy(o[obj].m_shd,tmp_m_shd[i]);
	 strcpy(o[obj].m_dds,tmp_m_dds[i]);
   if (o[obj].dds_type) o[obj].m_lm_dds = new char[_MAX_PATH], strcpy(o[obj].m_lm_dds,tmp_m_lm_dds[i]); 
   }
   }
   }

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
   
#define LINE_COMPACT
//////////////////////////////////////////
#ifdef LINE_COMPACT

   unsigned short mix_numt=0;
 
   printf(" compacting (full): first pass\n");
   for (unsigned short t = 1; t < numtextures; t++) // from 1 !!!
   {
   unsigned int max_v=0, max_i=0;
   for (unsigned int obj = 0; obj < ocount; obj++)
   if ((o[obj].texture == t) && (o[obj].range[0].vCount)) 
   {
   //printf(" -> obj = %u, v = %u, i = %u\n",obj,o[obj].range[0].vCount,o[obj].range[0].iCount);
   unsigned int vtmp = max_v + o[obj].range[0].vCount;
   if (vtmp <= 65535)
   max_v += o[obj].range[0].vCount, max_i += o[obj].range[0].iCount;
   //printf("obj %u chained\n",obj);
   else
   {
   //printf("obj %u eof\n",obj);
   printf("%05i: *texture='%s', max_v=%u, max_i=%u\n",t,tmp_m_dds[t],max_v,max_i);
   mix[mix_numt].texture = t;
   mix[mix_numt].m_dds = new char[strlen(tmp_m_dds[t])], mix[mix_numt].m_shd = new char[strlen(tmp_m_shd[t])],
   strcpy(mix[mix_numt].m_dds,tmp_m_dds[t]),strcpy(mix[mix_numt].m_shd,tmp_m_shd[t]);
   mix[mix_numt].vCount = max_v;
   mix[mix_numt].iCount = max_i;
   //printf(" [%u]:create of vblk[%u],iblk[%u]\n",mix_numt,max_v,max_i);
   //printf(" pre-v\n");
   mix[mix_numt].geom_vBlk = new OGFVertex_v0[max_v];
   //printf(" pre-i\n");
   mix[mix_numt].geom_iBlk = new unsigned short[max_i];
   //printf(" done make\n");
   mix_numt++;
   max_v = o[obj].range[0].vCount, max_i = o[obj].range[0].iCount;
   }
   }
   if (max_v)
   {
   printf("%05i: +texture='%s', max_v=%u, max_i=%u\n",t,tmp_m_dds[t],max_v,max_i);
   mix[mix_numt].texture = t;
   mix[mix_numt].m_dds = new char[strlen(tmp_m_dds[t])], mix[mix_numt].m_shd = new char[strlen(tmp_m_shd[t])],
   strcpy(mix[mix_numt].m_dds,tmp_m_dds[t]),strcpy(mix[mix_numt].m_shd,tmp_m_shd[t]);
   mix[mix_numt].vCount = max_v;
   mix[mix_numt].iCount = max_i;
   //printf(" [%u]:create of vblk[%u],iblk[%u]\n",mix_numt,max_v,max_i);
   //printf(" pre-v\n");
   mix[mix_numt].geom_vBlk = new OGFVertex_v0[max_v];
   //printf(" pre-i\n");
   mix[mix_numt].geom_iBlk = new unsigned short[max_i];
   //printf(" done make\n");
   mix_numt++;
   }
   }

   printf(" compacting (full): second pass\n");
   for (i = 0; i < mix_numt; i++)
   {

   unsigned int v_pos = 0, i_pos = 0;
   for (unsigned int obj = 0; obj < ocount; obj++)
   if (mix[i].texture == o[obj].texture)
   {
   unsigned int v_tmp = v_pos + o[obj].range[0].vCount;
   if (v_tmp <= mix[i].vCount)
   {
   memcpy(&mix[i].geom_vBlk[v_pos],o[obj].geom_vBlk,sizeof(OGFVertex_v0)*o[obj].range[0].vCount);
   memcpy(&mix[i].geom_iBlk[i_pos],o[obj].geom_iBlk,sizeof(unsigned short)*o[obj].range[0].iCount);
   if (i_pos) for (unsigned int ii = 0; ii < o[obj].range[0].iCount; ii++)
   mix[i].geom_iBlk[i_pos+ii] += (unsigned short)v_pos;
   v_pos += o[obj].range[0].vCount;
   i_pos += o[obj].range[0].iCount;
   }
   else
   {
   printf("*mix_num %u: max_v=%u, max_i=%u\n",i,v_pos,i_pos);
   i++;
   v_pos = 0; i_pos = 0;
   memcpy(&mix[i].geom_vBlk[v_pos],o[obj].geom_vBlk,sizeof(OGFVertex_v0)*o[obj].range[0].vCount);
   memcpy(&mix[i].geom_iBlk[i_pos],o[obj].geom_iBlk,sizeof(unsigned short)*o[obj].range[0].iCount);
   if (i_pos) for (unsigned int ii = 0; ii < o[obj].range[0].iCount; ii++)
   mix[i].geom_iBlk[i_pos+ii] += (unsigned short)v_pos;
   v_pos += o[obj].range[0].vCount;
   i_pos += o[obj].range[0].iCount;
   }
   }
   printf("+mix_num %u: max_v=%u, max_i=%u\n",i,v_pos,i_pos);

   }
   
/////////////////////////
#else // partial compact
/////////////////////////

   unsigned short mix_numt=0, m_first = 0, m_last = 0;
   unsigned int max_v = 0, max_i = 0;

   printf(" compacting (partial,line): first pass\n");

   for (obj = 0; obj < ocount; obj++) // from 1
   {
   printf("  blk [%u]: ",obj);

   unsigned int vtmp = max_v + o[obj].range[0].vCount;
   if (vtmp > 65535) printf("[v overflow '%s'] ",tmp_m_dds[o[obj].texture]);
   if ((o[obj].texture == o[m_first].texture) && (vtmp <= 65535))
   {
   printf("merge into [%u]\n",m_first);
   max_v += o[obj].range[0].vCount;
   max_i += o[obj].range[0].iCount;
   m_last = obj;
   }
   else
   {
   printf("delayed\n");
   printf(" *blk %u completed: ",mix_numt);
   mix[mix_numt].texture = o[m_first].texture;
   mix[mix_numt].m_dds = new char[strlen(tmp_m_dds[o[m_first].texture])];
   mix[mix_numt].m_shd = new char[strlen(tmp_m_shd[o[m_first].texture])];
   strcpy(mix[mix_numt].m_dds,tmp_m_dds[o[m_first].texture]);
   strcpy(mix[mix_numt].m_shd,tmp_m_shd[o[m_first].texture]);
   mix[mix_numt].vCount = max_v;
   mix[mix_numt].iCount = max_i;
   mix[mix_numt].geom_vBlk = new OGFVertex_v0[max_v];
   mix[mix_numt].geom_iBlk = new unsigned short[max_i];
   int i_pos=0, v_pos=0;
   for (unsigned int m = m_first; m <= m_last; m++)
   {
   memcpy(&mix[mix_numt].geom_vBlk[v_pos],o[m].geom_vBlk,sizeof(OGFVertex_v0)*o[m].range[0].vCount);
   memcpy(&mix[mix_numt].geom_iBlk[i_pos],o[m].geom_iBlk,sizeof(unsigned short)*o[m].range[0].iCount);

   if (i_pos) for (unsigned int ii = 0; ii < o[m].range[0].iCount; ii++)
   mix[mix_numt].geom_iBlk[i_pos+ii] += v_pos;

   v_pos += o[m].range[0].vCount;
   i_pos += o[m].range[0].iCount;

   }
   printf("max_v=%u, max_i=%u, v_pos=%u, i_pos=%u\n",max_v,max_i,v_pos,i_pos);
   mix_numt++;
   max_v = max_i = v_pos = i_pos = 0;
   obj = m_last;
   m_first = m_last + 1; m_last = m_first;
   }
   }

/////////////////////////

   // finish
   printf(" +blk %u completed: ",mix_numt);
   mix[mix_numt].texture = o[m_first].texture;
   mix[mix_numt].m_dds = new char[strlen(tmp_m_dds[o[m_first].texture])];
   mix[mix_numt].m_shd = new char[strlen(tmp_m_shd[o[m_first].texture])];
   strcpy(mix[mix_numt].m_dds,tmp_m_dds[o[m_first].texture]);
   strcpy(mix[mix_numt].m_shd,tmp_m_shd[o[m_first].texture]);
   mix[mix_numt].vCount = max_v;
   mix[mix_numt].iCount = max_i;
   mix[mix_numt].geom_vBlk = new OGFVertex_v0[max_v];
   mix[mix_numt].geom_iBlk = new unsigned short[max_i];
   unsigned int i_pos=0, v_pos=0;
   for (unsigned int m = m_first; m <= m_last; m++)
   {
   memcpy(&mix[mix_numt].geom_vBlk[v_pos],o[m].geom_vBlk,sizeof(OGFVertex_v0)*o[m].range[0].vCount);
   memcpy(&mix[mix_numt].geom_iBlk[i_pos],o[m].geom_iBlk,sizeof(unsigned short)*o[m].range[0].iCount);

   if (i_pos) for (unsigned int ii = 0; ii < o[m].range[0].iCount; ii++)
   mix[mix_numt].geom_iBlk[i_pos+ii] += v_pos;
   }
   printf("max_v=%u, max_i=%u\n",mix_numt,v_pos,i_pos);
   mix_numt++;
#endif
//////////////////////////////////////////////////////////////////

  printf("parse complete\n");

  unsigned int tot_i = 0, tot_v = 0;

   for (i = 1; i < numtextures; i++)
    delete tmp_m_dds[i], delete tmp_m_lm_dds[i], delete tmp_m_shd[i];
   delete [] o;

///////////////////////////////////////
  printf("creating '%s'\n",fileName);
  outputFile = fopen(fileName,"wb");

  ChunkIFF * ih = new ChunkIFF;
  ih->type = 0x01; // header
  ih->size = sizeof(OGFHeader_v4);
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

  OGFHeader_v4 * hdr4 = new OGFHeader_v4;
  hdr4->format_version = 4;
  hdr4->type = 1;
  hdr4->shader_id = 0;
  hdr4->bb = hdr2.bb;
  // hdr4.bs ???
	fwrite(hdr4,sizeof(OGFHeader_v4),1,outputFile);

  ih->type = 0x12; // text info
  ih->size = 0x6E; // guessed :)
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
  i = 0; 
  fputs("test_level\\test_level.object",outputFile);
  fwrite(&i,1,1,outputFile); 
  fputs("\\\\MSHOME\\Administrator",outputFile); fwrite(&i,1,1,outputFile); 
  fwrite(&i,4,1,outputFile); // fake time
  fputs("\\\\MSHOME\\Administrator",outputFile); fwrite(&i,1,1,outputFile); 
  fwrite(&i,4,1,outputFile); // fake time
  fputs("\\\\MSHOME\\Administrator",outputFile); fwrite(&i,1,1,outputFile); 
  fwrite(&i,4,1,outputFile); // fake time


  ih->type = 0x09; // childrens
  ih->size = 8 + (8+sizeof(OGFHeader_v4))+(0x14+8)+(8+((mix[0].vCount*sizeof(OGFVertex_v0))+8))+(8+((mix[0].iCount*2)+4));
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

  printf("making %u blocks\n",mix_numt);

	for (unsigned int j = 0;j < mix_numt; j++)
	{
	// child start

  if (!mix[j].vCount) { printf(" blk: vCount is zero sized!!!\n"); continue; }
  if (!mix[j].iCount) { printf(" blk: iCount is zero sized!!!\n"); continue; }

  ih->type = j; // children #j
  ih->size = (8+sizeof(OGFHeader_v4))+((strlen(mix[j].m_dds) + strlen(mix[j].m_shd) + 0x02)+8)+(8+((mix[j].vCount*sizeof(OGFVertex_v0))+8))+(8+((mix[j].iCount*2)+4));
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

	// .... 1.2.3.4

  ih->type = 0x01; // header
  ih->size = sizeof(OGFHeader_v4);
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
  hdr4->format_version = 4;
  hdr4->type = 0;
  hdr4->shader_id = 0;
  hdr4->bb.min = hdr2.bb.min;
  hdr4->bb.max = hdr2.bb.max;
  // hdr4.bs ???
	fwrite(hdr4,sizeof(OGFHeader_v4),1,outputFile);

  ih->type = 0x02; // texture
  ih->size = strlen(mix[j].m_dds) + strlen(mix[j].m_shd) + 0x02; 
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0;
  fputs(mix[j].m_dds,outputFile); fwrite(&i,1,1,outputFile); 
  fputs(mix[j].m_shd,outputFile); fwrite(&i,1,1,outputFile); 

	tot_v+= mix[j].vCount;
  ih->type = 0x03; // verts
  ih->size = (mix[j].vCount*sizeof(OGFVertex_v0))+8;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0x00000112; // D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
  fwrite(&i,4,1,outputFile);
	i = mix[j].vCount; 
  fwrite(&i,4,1,outputFile);
	fwrite(mix[j].geom_vBlk,sizeof(OGFVertex_v0)*i,1,outputFile);
	
	tot_i+= mix[j].iCount;
  ih->type = 0x04; // idx
  ih->size = (mix[j].iCount*2)+4;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = mix[j].iCount; 
  fwrite(&i,4,1,outputFile);
	fwrite(mix[j].geom_iBlk,sizeof(unsigned short)*i,1,outputFile);

	printf("."); if ((j % 50)==0) printf("\n");
	// child end 
	}

  delete hdr4;
  delete ih;

  fclose(outputFile);
  printf("\nwriting '%s' [v %u, i %u] OK\n",fileName,tot_v,tot_i);
///////////////////////////////////////

} // parseFlag == 2

	return 0;
	}
