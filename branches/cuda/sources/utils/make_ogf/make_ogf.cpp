#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

unsigned int numblocks, tot_v = 0, tot_i = 0;

struct vector3f
{
	float x,y,z; 
};

struct vector2f
{
	float tx,ty; 
};

struct ogf_bbox {
	struct vector3f		min;
	struct vector3f		max;
};

struct ogf_bsphere {
	struct vector3f		c;
	float			r;
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

#define MAX_OBJ_COUNT 1
#define MAX_TXT_COUNT 1

const char m_dds[] = "trees\\tree";
const char m_shd[] = "default";

OGFVertex_v0 * vtx;
unsigned short * idx;

unsigned int vcount;
unsigned int icount;

int main(int argc, char* argv[])
{
  unsigned int i, j;
	char fileName[256];
  FILE *outputFile;

 	printf("X-Ray ogf tris_pattern maker [201012xx]\n");

	if (argc < 4) 
  {
  	printf("usage: make_ogf < vtx >  < idx > < filename >\n\tv: vtx\n\ti: idx\n\tfile: output ogf\n\n");
		return 1;
	}

  if (strcpy((char*)fileName,argv[3]) == 0) 
  {
		printf("X-Ray PARSE_LEV: no arguments given\n"); return 1;
  }

  vcount = (unsigned int)atoi(argv[1]);
  icount = (unsigned int)atoi(argv[2]);
	numblocks = 1;
	srand((unsigned)time(NULL));

  while((vcount % 3) != 0) { vcount--; }
  while((icount % 3) != 0) { icount--; }

  printf("creating object ( %u / %u )... ",vcount,icount);

  vtx = new OGFVertex_v0[vcount];
  idx = new unsigned short[icount];

  for (i = 0; i < vcount; i++)
  {
  vtx[i].p.x = ((float)(rand()-16383)) / 8192;
  vtx[i].p.y = ((float)(rand()-16383)) / 8192;
  vtx[i].p.z = ((float)(rand()-16383)) / 8192;
  vtx[i].n.x = 0.0f;
  vtx[i].n.y = 0.0f;
  vtx[i].n.z = 0.0f;
  vtx[i].u = 0.0f;
  vtx[i].v = 0.0f;
	}


	unsigned int k = 0;
  for (j = 0; j < icount; j++)
  {
  idx[j] = (unsigned short)k; k++;
  if ((k>65535) || (k>=vcount)) k = 0;
	}

  printf("DONE\n");
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
  hdr4->bb.min.x = -2.0f;
  hdr4->bb.min.y = -2.0f;
  hdr4->bb.min.z = -2.0f;
  hdr4->bb.max.x = 2.0f;
  hdr4->bb.max.y = 2.0f;
  hdr4->bb.max.z = 2.0f;
  hdr4->bs.c.x = 0.0f;
  hdr4->bs.c.y = 0.0f;
  hdr4->bs.c.z = 0.0f;
  hdr4->bs.r = 3.0f;
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
  ih->size = 8 + (8+sizeof(OGFHeader_v4))+(0x14+8)+(8+((vcount*sizeof(OGFVertex_v0))+8))+(8+((icount*2)+4));
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

  printf("making %u blocks\n",numblocks);

  k = 0;
	for (j = 0;j < numblocks; j++)
	{
	printf("child %u\n",k);
	// child start

  ih->type = k; // children #j
  ih->size = (8+sizeof(OGFHeader_v4))+((strlen(m_dds) + strlen(m_shd) + 0x02)+8)+(8+((vcount*sizeof(OGFVertex_v0))+8))+(8+((icount*2)+4));
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);

	// .... 1.2.3.4

  ih->type = 0x01; // header
  ih->size = sizeof(OGFHeader_v4);
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
  hdr4->format_version = 4;
  hdr4->type = 0;
  hdr4->shader_id = 0;
	fwrite(hdr4,sizeof(OGFHeader_v4),1,outputFile);

	printf(" blks: '%s'",m_dds);
  ih->type = 0x02; // texture
  ih->size = strlen(m_dds) + strlen(m_shd) + 0x02; 
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0;
  fputs(m_dds,outputFile); fwrite(&i,1,1,outputFile); 
  fputs(m_shd,outputFile); fwrite(&i,1,1,outputFile); 

	printf(" v %u",vcount); tot_v += vcount;
  ih->type = 0x03; // verts
  ih->size = (vcount*sizeof(OGFVertex_v0))+8;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = 0x00000112; // D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
  fwrite(&i,4,1,outputFile);
	i = vcount; 
  fwrite(&i,4,1,outputFile);
	fwrite(vtx,sizeof(OGFVertex_v0)*i,1,outputFile);
	
	printf(" i %u\n",icount); tot_i += icount;
  ih->type = 0x04; // idx
  ih->size = (icount*2)+4;
	fwrite(ih,sizeof(ChunkIFF),1,outputFile);
	i = icount; 
  fwrite(&i,4,1,outputFile);
	fwrite(idx,sizeof(unsigned short)*i,1,outputFile);

	k++;
	// child end 
	}

  delete [] vtx;
  delete [] idx;

  fclose(outputFile);
  printf("writing '%s' [%u,%u] OK\n",fileName,tot_v,tot_i);
///////////////////////////////////////

	return 0;
	}
