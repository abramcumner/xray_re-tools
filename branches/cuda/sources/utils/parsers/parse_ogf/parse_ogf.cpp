#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc, char* argv[])
{
  OGFHeader_v3 oh3;
  OGFHeader_v4 oh4;
  unsigned long chunkid, chunksz, value, i, subid=0,
  subfullsize=0, subsubfullsize=0, fullsize=0;
	char fileName[256];
  FILE *inputFile;

	printf("X-Ray OGF parser [201012xx]\n");
	if (argc < 2) 
  {
		printf("usage: parse_ogf < srcfile.ogf >\n");
		return 1;
	}

	if (strcpy((char*)fileName,argv[1]) == 0) 
  {
 	printf("ERR: no argument given.\n");
  return 1;
  }
  inputFile = fopen(fileName,"rb");
	if(!inputFile)
  {
 	printf("ERR: cannot open.\n");
  return 1;
  }

	fseek(inputFile,8,SEEK_SET);
  fread( &oh3, sizeof(OGFHeader_v3), 1, inputFile );

	fseek(inputFile,0,SEEK_SET);

	if (oh3.format_version == 4) 
	{ // v4
	for (;;)
	{
	if (fread( &chunkid, 4, 1, inputFile ) == 0) break;
	if (chunkid !=9) {
	fread( &chunksz, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
	if (chunkid == 1)
  fread( &oh4, sizeof(OGFHeader_v4), 1, inputFile ),
  printf(" X-Ray OGF v%d (type 0x%02X)\n",oh4.format_version,oh4.type);
  else
	for (i = 0;i < chunksz;i++) fread( &value, 1, 1, inputFile );
  fullsize+= 8;fullsize+= chunksz;
  }
  else {
	fread( &chunksz, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
  fullsize+= 8;fullsize+= chunksz;
	// sub#1
  subfullsize = chunksz;
	for (;;)
	{
  fread( &chunkid, 4, 1, inputFile );
	fread( &chunksz, 4, 1, inputFile );
	printf("--> SubChunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
  subfullsize-= 8;subfullsize-= chunksz;
	if (chunkid == subid) {
	// sub#2
  subsubfullsize = chunksz;
	for (;;)
	{
  fread( &chunkid, 4, 1, inputFile );
	fread( &chunksz, 4, 1, inputFile );
	printf("----> Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
	if (chunkid == 1)
  fread( &oh4, sizeof(OGFHeader_v4), 1, inputFile ),
  printf("       X-Ray OGF v%d (type 0x%02X)\n",oh4.format_version,oh4.type);
  else
	for (i = 0;i < chunksz;i++) fread( &value, 1, 1, inputFile );
  subsubfullsize-= 8;subsubfullsize-= chunksz;
  if (!subsubfullsize) { subid++; break; }
	}
	} else {
	for (i = 0;i < chunksz;i++) fread( &value, 1, 1, inputFile );
	}
	// sub#2
  if (!subfullsize) break;
	}
	// sub#1
  }
  }
	printf("Full size is 0x%08X (%u)\n",fullsize,fullsize);
	}
	else if (oh3.format_version == 3) 
	{ // v3
	for (;;)
	{
	if (fread( &chunkid, 4, 1, inputFile ) == 0) break;
	if (chunkid !=0x11) {
	fread( &chunksz, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
	if (chunkid == 1)
  fread( &oh3, sizeof(OGFHeader_v3), 1, inputFile ),
  printf(" X-Ray OGF v%d (type 0x%02X)\n",oh3.format_version,oh3.type);
  else
	for (i = 0;i < chunksz;i++) fread( &value, 1, 1, inputFile );
  fullsize+= 8;fullsize+= chunksz;
  }
  else {
	fread( &chunksz, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
  fullsize+= 8;fullsize+= chunksz;
	// sub#1
  subfullsize = chunksz;
	for (;;)
	{
  fread( &chunkid, 4, 1, inputFile );
	fread( &chunksz, 4, 1, inputFile );
	printf("--> SubChunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
  subfullsize-= 8;subfullsize-= chunksz;
	if (chunkid == subid) {
	// sub#2
  subsubfullsize = chunksz;
	for (;;)
	{
  fread( &chunkid, 4, 1, inputFile );
	fread( &chunksz, 4, 1, inputFile );
	printf("----> Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
	if (chunkid == 1)
  fread( &oh3, sizeof(OGFHeader_v3), 1, inputFile ),
  printf("       X-Ray OGF v%d (type 0x%02X)\n",oh3.format_version,oh3.type);
  else
	for (i = 0;i < chunksz;i++) fread( &value, 1, 1, inputFile );
  subsubfullsize-= 8;subsubfullsize-= chunksz;
  if (!subsubfullsize) { subid++; break; }
	}
	} else {
	for (i = 0;i < chunksz;i++) fread( &value, 1, 1, inputFile );
	}
	// sub#2
  if (!subfullsize) break;
	}
	// sub#1
  }
  }
	printf("Full size is 0x%08X (%u)\n",fullsize,fullsize);
	}
  else printf("!!! unknown type\n");

  fclose(inputFile);
  
	return 0;
	}