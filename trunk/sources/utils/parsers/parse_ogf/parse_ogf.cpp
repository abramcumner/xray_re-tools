#include "windows.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

struct OGFHeader
{
 unsigned char format_version;
 unsigned char type;
};

int main(int argc, char* argv[])
{
  OGFHeader oh;
  unsigned long chunkid, chunksz, value, i, subid=0,
  subfullsize=0, subsubfullsize=0, fullsize=0;
	char fileName[256];
  FILE *inputFile;

	printf("X-Ray OGF parser [20100520]\n");

	if (lstrcpy((char*)fileName,argv[1]) == 0) 
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
  fread( &oh, 4, 1, inputFile );
  printf("X-Ray OGF v%d (type 0x%02X)\n",oh.format_version,oh.type);

	fseek(inputFile,0,SEEK_SET);

	if (oh.format_version == 4) 
	{ // v4
	while (TRUE)
	{
	if (fread( &chunkid, 4, 1, inputFile ) == 0) break;
	if (chunkid !=9) {
	fread( &chunksz, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
	for (i = 0;i < chunksz;i++) fread( &value, 1, 1, inputFile );
  fullsize+= 8;fullsize+= chunksz;
  }
  else {
	fread( &chunksz, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
  fullsize+= 8;fullsize+= chunksz;
	// sub#1
  subfullsize = chunksz;
	while (TRUE)
	{
  fread( &chunkid, 4, 1, inputFile );
	fread( &chunksz, 4, 1, inputFile );
	printf("--> SubChunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
  subfullsize-= 8;subfullsize-= chunksz;
	if (chunkid == subid) {
	// sub#2
  subsubfullsize = chunksz;
	while (TRUE)
	{
  fread( &chunkid, 4, 1, inputFile );
	fread( &chunksz, 4, 1, inputFile );
	printf("----> Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
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
	else if (oh.format_version == 3) 
	{ // v3
	while (TRUE)
	{
	if (fread( &chunkid, 4, 1, inputFile ) == 0) break;
	if (chunkid !=0x11) {
	fread( &chunksz, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
	for (i = 0;i < chunksz;i++) fread( &value, 1, 1, inputFile );
  fullsize+= 8;fullsize+= chunksz;
  }
  else {
	fread( &chunksz, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
  fullsize+= 8;fullsize+= chunksz;
	// sub#1
  subfullsize = chunksz;
	while (TRUE)
	{
  fread( &chunkid, 4, 1, inputFile );
	fread( &chunksz, 4, 1, inputFile );
	printf("--> SubChunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
  subfullsize-= 8;subfullsize-= chunksz;
	if (chunkid == subid) {
	// sub#2
  subsubfullsize = chunksz;
	while (TRUE)
	{
  fread( &chunkid, 4, 1, inputFile );
	fread( &chunksz, 4, 1, inputFile );
	printf("----> Chunk #:0x%04X, size is 0x%08X (%u)\n",chunkid,chunksz,chunksz);
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