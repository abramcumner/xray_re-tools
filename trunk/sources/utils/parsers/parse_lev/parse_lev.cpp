#include "windows.h"
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

struct face
{
	unsigned short f0,f1,f2; 
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

struct tris_blk
{
 unsigned int tris[3];
 unsigned short matid; 
 unsigned short sec_num;
};

struct vformat_blk
{
	unsigned short v_offset; 
	unsigned short v_type; // 1(shortsingle*2),4(single*3),6(???),7(single*2)

// 1	DWord(4) + DWord(4) , float?
// 4	Byte(1) + Byte(1) + Byte(1) + Byte(1)
// 6	Word(2)/32768 + Word(2)/32768
// 7	(Byte(1)-128)/128 + (Byte(1)-128)/128 + (Byte(1)-128)/128 + (Byte(1)-128)/128 + pad(4)

	unsigned short v_class; 

// 0.12 - vertices (4*float) - X/Z/Y
// 3.4 - normales (4*(char-128)/128)
// 6.4 - binormales (4*(char-128)/128)
// 7.4 - tangent normales (4*(char-128)/128)
// 10.4 - ???? (4*char)
// 5.1 texcoords : (2*float)
// 5.6 texcoords : (2*short/1024)
// 5.7 texcoords : (2*short/2048) + dummy (4)
// 261.6 - lightmaps (2*short/32768)

};

struct blk_01
{
 unsigned short code;
 unsigned short texture;
 struct ogf_bbox		bb;
 struct ogf_bsphere	bs;
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
 unsigned short code;
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
 float xform[16];
 struct ogf_bbox		bb;
 struct ogf_bsphere	bs;
};

obj_blk * o;
unsigned short ocount = 0;
unsigned short orange = 0;
char * tmp_m_dds[MAX_TXT_COUNT];
char * tmp_m_shd[MAX_TXT_COUNT];

///////////////////////////////


#define VEC_Cross(a, b, c)((c).x = (a).y * (b).z - (a).z * (b).y, (c).y = (a).z * (b).x - (a).x * (b).z, (c).z = (a).x * (b).y - (a).y * (b).x)
#define VEC_Subtract(a, b, c)((c).x = (a).x - (b).x, (c).y = (a).y - (b).y, (c).z = (a).z - (b).z)
#define VEC_Magnitude(v)(sqrt((v).x * (v).x + (v).y * (v).y + (v).z * (v).z))
#define VEC_Normalizef(v)        { float __mag = 1.0f / (float)VEC_Magnitude(v); \
(v).x *= __mag; (v).y *= __mag; (v).z *= __mag; }
void Make_Normal(vector3f a, vector3f b, vector3f c, vector3f * n)
{
	vector3f v1 = {0,0,0},v2 = {0,0,0};
	VEC_Subtract(b, a, v1);
	VEC_Subtract(c, a, v2);
	VEC_Cross(v1, v2, *n);
	VEC_Normalizef(*n);
}

int DecodeChunk(lev_blk h,FILE *inputFile)
{
	printf(" DecodeChunk(0x%04X, 0x%08X)\n",h.type,h.size);

	switch (h.type)
	{
	case 0x01: // level; level.geom, LEV_HEADER
  {
	int version;
	fread(&version,h.size,1,inputFile);
	printf("  level version = 0x%08X\n",version);
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
  if (!destFlag) printf("   [%d]: s,t,lmap0,lmap1 '%s'\n",i,&tmpdata[p]);

  plen = 0;
  while(tmpdata[p+plen] != 0x2F) plen++;
  memcpy(tmp_m_shd[i],&tmpdata[p],plen); tmp_m_shd[i][plen] = 0;
  p += plen; p++; 

  plen = 0;
  while((tmpdata[p+plen]) && (tmpdata[p+plen] != 0x2C)) plen++; //  != 0x00
  memcpy(tmp_m_dds[i],&tmpdata[p],plen); tmp_m_dds[i][plen] = 0;
  p += plen;

  //printf("DDS:%s\nSHD:%s\n",tmp_m_dds[i],tmp_m_shd[i]);

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
  if (!destFlag) printf("   block = %d, size = %u\n",j,szBlk);

  if (j > 255) printf("!!! incorrect block, it is an object start\n"), _getch();
  if (ocount > MAX_OBJ_COUNT) printf("!!! too many objects, maximum is %u\n",MAX_OBJ_COUNT), _getch();

  switch(j)
  {

/*
  code = 
   4 - 21,22,1 or 21,1 or 21,22,21,1 // brush
 104 - 1,10 // brush+entity
 204 - 21,6,22,1 or 21,6,1 or 21,6,22,21,6,1 // entity
 604 - 1,10,11 // greens joined
 704 - 21,12,1 // greens
 B04 - 21,1,12,6 or 21,12,1,20 // greens scaled

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
  blk_01 ft;
	fread(&ft,sizeof(blk_01),1,inputFile);
  if (!destFlag) printf("    blk_#%02d: type 0x%03X texture %d bs.c(%f,%f,%f), size = %u\n",
    j,ft.code,ft.texture,ft.bs.c.x,ft.bs.c.y,ft.bs.c.z,szBlk);
   if (destFlag) 
   if (orange) // we don't need 'no i/v objects!'
   {
   o[ocount].code = ft.code;
   o[ocount].texture = ft.texture;
   o[ocount].bb = ft.bb;
   o[ocount].bs = ft.bs;
   }
   if (ft.code == 4) bdone = 1;
   else
   if (ft.code == 0x104) bdone = 10;
   else
   if (ft.code == 0x204) bdone = 1;
   else
   if (ft.code == 0x604) bdone = 11;
   else
   if (ft.code == 0x704) bdone = 1;
   else
   if (ft.code == 0xB04) bdone = 20;
   else
   printf("!!!!!!!!!!!! unknown object !!!!!!!!!!!!!!!!!!!!!\n"), _getch();

  i += szBlk;
	break;
  }

  case 20: 
  {
  unsigned int swi;
	fread(&swi,4,1,inputFile);
  if (!destFlag) printf("    blk_#%02d, swi = %u, size = %u\n",j,swi,szBlk);
  i += szBlk;
	break;
  }

  case 21: 
  {
  blk_21 iv;
	fread(&iv,sizeof(blk_21),1,inputFile);
   if (!destFlag)  printf("    blk_#%02d: (v:#%d,%d,%d) (i:#%d,%d,%d), size = %u\n",
    j,iv.vBlk,iv.vFirst,iv.vCount,iv.iBlk,iv.iFirst,iv.iCount,szBlk);
  else
  {
  o[ocount].range[orange].vBlk = iv.vBlk;
  o[ocount].range[orange].vFirst = iv.vFirst;
  o[ocount].range[orange].vCount = iv.vCount;
  o[ocount].range[orange].iBlk = iv.iBlk;
  o[ocount].range[orange].iFirst = iv.iFirst;
  o[ocount].range[orange].iCount = iv.iCount;
  orange++;
  }
  i += szBlk;
	break;
  }

  case 22: 
  {
  if (!destFlag) printf("    blk_cform_header: %u\n",szBlk);
  i += szBlk;
	break;
  }

  case 6: 
  {
  if (!destFlag) printf("    blk_unknown#%02d, len: %d\n",j,szBlk);
  for (unsigned int m = 0; m < szBlk; m++) fread(&k,1,1,inputFile); // dummy read szBlk times
  if ((i + szBlk) > szObj) szBlk = 8; // , printf("!!! blk#6: overflow fixed\n");
  i += szBlk;
	break;
  }

  case 10: 
  {
  if (!destFlag) printf("    blk_unknown#%02d, len: %d\n",j,szBlk);
  for (unsigned int m = 0; m < szBlk; m++) fread(&k,1,1,inputFile); // dummy read szBlk times
  i += (szBlk + 16);
  // printf("!!! blk#10: advance 16 bytes\n");
	break;
  }

  case 12: 
  {
  blk_12 iv;
	fread(&iv,sizeof(blk_12),1,inputFile);
  if (!destFlag)  printf("    blk_#%02d: xform[16] matrix, size = %u\n", j, szBlk); 
  else memcpy(&o[ocount].xform,&iv,16*sizeof(float));
  i += szBlk;
	break;
  }

  case 11: 
  case 24: 
  default:
  {
  if (!destFlag)  printf("    blk_unknown#%02d, len: %d\n",j,szBlk);
  for (unsigned int m = 0; m < szBlk; m++) fread(&k,1,1,inputFile); // dummy read szBlk times
  i += szBlk;
	break;
  }

  } // end of switch

  if (i == szObj)
  {
  if (!destFlag) printf("    object completed\n");
  break;
  }

  if ((bdone == j) && (i < szObj) && ((szObj - i) <= 32)) // read all the rest and break
  {
  if (!destFlag) printf("    object completed (forced EOO)\n");
  break;
  }
	
  if (i > szObj) { printf("obj ended beyond limit (i %d szobj %d).\n",i,szObj); break;  }

	}

  if (destFlag) 
  {
  o[ocount].rangecount = orange;
  if ((orange) && ((o[ocount].code == 4) || (o[ocount].code == 0x704) || (o[ocount].code == 0xB04) || (o[ocount].code == 0x204))) //  || (o[ocount].code == 0x704)
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
  vformat_blk v_b;

	fread(&numvblocks,4,1,inputFile);
	printf("  numvblocks = %d\n",numvblocks);


	for (unsigned int cnt=0;cnt<numvblocks;cnt++) // base header
	{

	fread(&dummy32,4,1,inputFile); // = 0
	fread(&dummy32,4,1,inputFile); // = ?
  if (dummy32 == 2) { printf("    vBlock[%u]\n",cnt); } // start

  unsigned short coordtype[10], coordclass[10], ccnt = 0;
  unsigned int szData = 12; // x,y,z at 0 = (4 * 3 = 12)
	for (;;) // header of vblock
	{
	fread(&dummy16,2,1,inputFile); //	padding #1
	if (dummy16 == 255) break; // end
	fread(&v_b,sizeof(vformat_blk),1,inputFile);
	/// printf("     offset:%d v_type:%d v_class:%d\n",v_b.v_offset,v_b.v_type,v_b.v_class);
  coordclass[ccnt] = v_b.v_class;
  coordtype[ccnt++] = v_b.v_type;

	if (v_b.v_type == 1) szData += 8; // f32,f32
	else if (v_b.v_type == 4) szData += 4; // u8,u8,u8,u8
	else if (v_b.v_type == 6) szData += 4; // u16,u16
	else if (v_b.v_type == 7) szData += 8; // u8,u8,u8,u8 + pad(4)
	}
	fread(&dummy16,2,1,inputFile); //	padding #2
	fread(&dummy32,4,1,inputFile);
	/// if (dummy32 == 17) printf("      [+] vert_blk_start\n"); 
	fread(&dummy32,4,1,inputFile); // printf("       vert_blks: %d [ ... ]\n", dummy32 ); 
  tot_vblks += dummy32;

   OGFVertex_v0 * v_tmp = new OGFVertex_v0[dummy32];

   for (int  i = 0;i < dummy32;i++)
   {
   vector3f v0;
   vector2f v5;
   unsigned char u8;
   unsigned short u16;
   short d16;
   float nx1,ny1,nz1,nn1;
 	 fread(&v0,12,1,inputFile);
 	 // if (!destFlag) printf("       class 0, type 0 = vertex[%d](X,Z,Y) = %f,%f,%f\n",i,v0.x,v0.y,v0.z);

 	 v_tmp[i].p = v0;

   for (int dcnt = 0;dcnt < ccnt;dcnt++)
   {
   // if (!destFlag)  printf("       class %u, ",coordclass[dcnt]);
   switch(coordclass[dcnt])
   {
   case 3: 
   {
 	 fread(&u8,1,1,inputFile); nx1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); ny1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); nz1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); nn1 = ((float(u8)-128) / 128);
	 // if (!destFlag) printf("type %u = normal[%d](X,Z,Y) = %f,%f,%f\n",coordtype[dcnt],i,nx1,ny1,nz1,nn1);
   v_tmp[i].n.x = nx1;
   v_tmp[i].n.y = ny1;
   v_tmp[i].n.z = nz1;
   break;
   }
   case 6: 
   {
 	 fread(&u8,1,1,inputFile); nx1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); ny1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); nz1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); nn1 = ((float(u8)-128) / 128);
	 // if (!destFlag) printf("type %u = binormal[%d](X,Z,Y) = %f,%f,%f\n",coordtype[dcnt],i,nx1,ny1,nz1,nn1);
   break;
   }
   case 7: 
   {
 	 fread(&u8,1,1,inputFile); nx1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); ny1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); nz1 = ((float(u8)-128) / 128);
 	 fread(&u8,1,1,inputFile); nn1 = ((float(u8)-128) / 128);
	 // if (!destFlag) printf("type %u = tangentnormal[%d](X,Z,Y) = %f,%f,%f\n",coordtype[dcnt],i,nx1,ny1,nz1,nn1);
   break;
   }
   case 10: 
   {
 	 fread(&u8,1,1,inputFile); 
 	 fread(&u8,1,1,inputFile); 
 	 fread(&u8,1,1,inputFile); 
 	 fread(&u8,1,1,inputFile); 
	 // if (!destFlag) printf("type %u = ???\n",coordtype[dcnt]);
   break;
   }
   case 5: 
   {
   if (coordtype[dcnt] == 1)
   {
 	 fread(&v5,8,1,inputFile);
 	 // if (!destFlag) printf("type %d = uvcoord[%d](U,V) = %f,%f\n",coordtype[dcnt],i,v5.tx,v5.ty);
   v_tmp[i].u = v5.tx;
   v_tmp[i].v = v5.ty;
   }
   else
   if (coordtype[dcnt] == 6)
   {
 	 fread(&d16,2,1,inputFile); v5.tx = (float(d16) / 1024);
 	 fread(&d16,2,1,inputFile); v5.ty = (float(d16) / 1024);
 	 // if (!destFlag) printf("type %d = uvcoord[%d](U,V) = %f,%f\n",coordtype[dcnt],i,v5.tx,v5.ty);
   v_tmp[i].u = v5.tx;
   v_tmp[i].v = v5.ty;
   }
   else
   if (coordtype[dcnt] == 7)
   {
 	 fread(&d16,2,1,inputFile); v5.tx = (float(d16) / 2048);
 	 fread(&d16,2,1,inputFile); v5.ty = (float(d16) / 2048);
   fread(&d16,2,1,inputFile); 
   fread(&d16,2,1,inputFile); 
 	 // if (!destFlag) printf("type %d = uvcoord[%d](U,V) = %f,%f\n",coordtype[dcnt],i,v5.tx,v5.ty);
   v_tmp[i].u = v5.tx;
   v_tmp[i].v = v5.ty;
   }
   break;
   }
   case 261: 
   {
 	 fread(&u16,2,1,inputFile); v5.tx = (float(u16) / 32768);
 	 fread(&u16,2,1,inputFile); v5.ty = (float(u16) / 32768);
 	 // if (!destFlag) printf("type %d = lmapcoord[%d](U,V) = %f,%f\n",coordtype[dcnt],i,v5.tx,v5.ty);
   break;
   }

   default: printf("should not get here\n"); _getch(); break;
   }
   }
   //printf("press a key to break\n"); _getch();
   }
/////////////////////////
   if (destFlag) for (unsigned int obj = 0; obj < ocount; obj++)
   if (o[obj].range[0].vBlk == cnt) 
   {
   unsigned int bsz = o[obj].range[0].vCount;
   unsigned int bst = o[obj].range[0].vFirst;
   o[obj].geom_vBlk = new OGFVertex_v0[bsz];
   memcpy(o[obj].geom_vBlk,&v_tmp[bst],sizeof(OGFVertex_v0)*bsz);
  if ((o[obj].code == 0xB04) || (o[obj].code == 0x704))
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

	case 0x05: // level
	case 0x08: // level, LEV_SECTORS
	case 0x0B: // level.geom
	case 0x0E: // ??????????
  case 0x04: // level, LEV_PORTALS
  {
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

 	printf("X-Ray level (2xxx+) parser-converter [201008xx]\n");

	if (argc < 2) 
  {
  	printf("usage: parse_lev < mode > [ destfile.ogf ]\n\twhere mode is: 0 - dump only, 1 - convert to ogf, 2 - convert to ogf (optimized)\n\tdestfile.ogf - destination file (modes 1, 2 only)\n\n");
		return 1;
	}

  parseFlag = (char)atoi(argv[1]);

	if (parseFlag) 
	{
  if (lstrcpy((char*)fileName,argv[2]) == 0) 
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
  printf("Type 0x%04X, size %d\n",hdr.type,hdr.size);
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
  printf("Type 0x%04X, size %d\n",hdr.type,hdr.size);
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
   lstrcpy(o[obj].m_dds,tmp_m_dds[i]),lstrcpy(o[obj].m_shd,tmp_m_shd[i]);
   }
   }
   for (i = 0; i < numtextures; i++)
    delete tmp_m_dds[i], delete tmp_m_shd[i];
/////////////////////////
//////////////////////////////////////////////////////////////////

  printf("parse complete\n");

  unsigned short numblocks = 0;
  unsigned int tot_i = 0, tot_v = 0;

  //   for (obj = 0; obj < ocount; obj++) -- nattefrost.. wtf :)

   for (unsigned int obj = 0; obj < ocount; obj++)
  {
  if (o[obj].texture)
  printf("o%05u - m,t,s, 0x%03X, '%s', '%s'\n",
  obj,o[obj].code, o[obj].m_dds,o[obj].m_shd), numblocks++;
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
  ih->size = 8 + (8+sizeof(OGFHeader_v4))+(0x14+8)+(8+((o[0].range[0].vCount*sizeof(OGFVertex_v0))+8))+(8+((o[0].range[0].iCount*2)+4));
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

  printf("making %u blocks\n",numblocks);

	for (unsigned int j = 0;j < numblocks; j++)
	{
	printf("child %u\n",j);
	// child start

  ih->type = j; // children #j
  ih->size = (8+sizeof(OGFHeader_v4))+((lstrlen(o[j].m_dds) + lstrlen(o[j].m_shd) + 0x02)+8)+(8+((o[j].range[0].vCount*sizeof(OGFVertex_v0))+8))+(8+((o[j].range[0].iCount*2)+4));
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

	printf(" blks: '%s'",o[j].m_dds);
  ih->type = 0x02; // texture
  ih->size = lstrlen(o[j].m_dds) + lstrlen(o[j].m_shd) + 0x02; 
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0;
  fputs(o[j].m_dds,outputFile); fwrite(&i,1,1,outputFile); 
  fputs(o[j].m_shd,outputFile); fwrite(&i,1,1,outputFile); 

	printf(" v %u",o[j].range[0].vCount); tot_v += o[j].range[0].vCount;
  ih->type = 0x03; // verts
  ih->size = (o[j].range[0].vCount*sizeof(OGFVertex_v0))+8;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0x00000112; // D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
  fwrite(&i,4,1,outputFile);
	i = o[j].range[0].vCount; 
  fwrite(&i,4,1,outputFile);
	fwrite(o[j].geom_vBlk,sizeof(OGFVertex_v0)*i,1,outputFile);
	
	printf(" i %u\n",o[j].range[0].iCount); tot_i += o[j].range[0].iCount;
  ih->type = 0x04; // idx
  ih->size = (o[j].range[0].iCount*2)+4;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = o[j].range[0].iCount; 
  fwrite(&i,4,1,outputFile);
	fwrite(o[j].geom_iBlk,sizeof(unsigned short)*i,1,outputFile);

	// child end 
	}

  delete [] o;

  delete hdr4;
  delete ih;

  fclose(outputFile);
  printf("writing '%s' [%u,%u] OK\n",fileName,tot_v,tot_i);
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
   o[obj].m_dds = new char[_MAX_PATH], o[obj].m_shd = new char[_MAX_PATH],
   lstrcpy(o[obj].m_dds,tmp_m_dds[i]),lstrcpy(o[obj].m_shd,tmp_m_shd[i]);
   }
   }

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
   
struct merged_blk
{
 OGFVertex_v0 * geom_vBlk;
 unsigned short * geom_iBlk;
 unsigned int iCount;
 unsigned int vCount;
 unsigned short start_obj_num;
 char * m_dds;
 char * m_shd;
 unsigned short texture;
};

#define LINE_COMPACT
//////////////////////////////////////////
#ifdef LINE_COMPACT

   unsigned short mix_numt=0;
 
	 merged_blk mix[MAX_TXT_COUNT];

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
   lstrcpy(mix[mix_numt].m_dds,tmp_m_dds[t]),lstrcpy(mix[mix_numt].m_shd,tmp_m_shd[t]);
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
   lstrcpy(mix[mix_numt].m_dds,tmp_m_dds[t]),lstrcpy(mix[mix_numt].m_shd,tmp_m_shd[t]);
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

   merged_blk mix[MAX_TXT_COUNT*2];

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
   lstrcpy(mix[mix_numt].m_dds,tmp_m_dds[o[m_first].texture]);
   lstrcpy(mix[mix_numt].m_shd,tmp_m_shd[o[m_first].texture]);
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
   lstrcpy(mix[mix_numt].m_dds,tmp_m_dds[o[m_first].texture]);
   lstrcpy(mix[mix_numt].m_shd,tmp_m_shd[o[m_first].texture]);
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
    delete tmp_m_dds[i], delete tmp_m_shd[i];
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
	printf("child %u\n",j);
	// child start

  if (!mix[j].vCount) { printf(" blk: vCount is zero sized!!!\n"); continue; }
  if (!mix[j].iCount) { printf(" blk: iCount is zero sized!!!\n"); continue; }

  ih->type = j; // children #j
  ih->size = (8+sizeof(OGFHeader_v4))+((lstrlen(mix[j].m_dds) + lstrlen(mix[j].m_shd) + 0x02)+8)+(8+((mix[j].vCount*sizeof(OGFVertex_v0))+8))+(8+((mix[j].iCount*2)+4));
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

	printf(" blk: '%s'",mix[j].m_dds);
  ih->type = 0x02; // texture
  ih->size = lstrlen(mix[j].m_dds) + lstrlen(mix[j].m_shd) + 0x02; 
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0;
  fputs(mix[j].m_dds,outputFile); fwrite(&i,1,1,outputFile); 
  fputs(mix[j].m_shd,outputFile); fwrite(&i,1,1,outputFile); 

	printf(" v %u",mix[j].vCount); tot_v+= mix[j].vCount;
  ih->type = 0x03; // verts
  ih->size = (mix[j].vCount*sizeof(OGFVertex_v0))+8;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0x00000112; // D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
  fwrite(&i,4,1,outputFile);
	i = mix[j].vCount; 
  fwrite(&i,4,1,outputFile);
	fwrite(mix[j].geom_vBlk,sizeof(OGFVertex_v0)*i,1,outputFile);
	
	printf(" i %u\n",mix[j].iCount); tot_i+= mix[j].iCount;
  ih->type = 0x04; // idx
  ih->size = (mix[j].iCount*2)+4;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = mix[j].iCount; 
  fwrite(&i,4,1,outputFile);
	fwrite(mix[j].geom_iBlk,sizeof(unsigned short)*i,1,outputFile);

	// child end 
	}

  delete hdr4;
  delete ih;

  fclose(outputFile);
  printf("writing '%s' [%u,%u] OK\n",fileName,tot_v,tot_i);
///////////////////////////////////////

} // parseFlag == 2

	return 0;
	}