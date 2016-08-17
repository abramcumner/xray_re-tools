#include <conio.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

struct ChunkIFF
{
	unsigned long type;
	unsigned long size;
};

struct vector2f {
	float	x,y;
};

struct vector3f {
	float	x,y,z;
};

struct ogf_bbox { // 0x18 (24)
	struct vector3f		min;
	struct vector3f		max;
};

struct Vertex
{
	float x,y,z; 
};

struct Vertices
{
	unsigned long	count;
	unsigned short	*vtx;
};

struct OBJFace
{
	long v0, vref0, v1, vref1, v2, vref2; 
};

struct Faces
{
	long	count;
	unsigned short	*fcs;
};

struct submap
{
 unsigned long vmap_index; // link to map
 unsigned long index; // vmap index to uv/w
};

struct uvmap // 0x19 (25), 0x1008 EMESH_CHUNK_VMREFS, another one
{
 unsigned char count; 
 submap m[3];
};

#define MAX_T_LEN 32

struct txmap // BLOCK 0x1012
{
 char name[MAX_T_LEN]; // "Texture\0", or less, more
 unsigned char dim;
 unsigned char polymap; // when = 1, third block exists
 unsigned char type; // when = 1, second block absent (vmtUV	= 0; vmtWeight	= 1;)
 unsigned long txcount;
};

struct uvcoord // BLOCK 0x1012
{
 float u,v;
};

struct matpars // 0x0C (12) - BLOCK 0x0907
{
 unsigned long m_flags;
 unsigned long fvf;
 unsigned long unk; // = 1
};

char destFlag = 0;

long DecodeChunk(ChunkIFF chunk,FILE *inputFile)
{
	unsigned long cnt;
  Vertices v;
	Faces f;
	char c_data; // 1
	short s_data; // 2
	long l_data; // 4

	//printf("DecodeChunk(0x%04X, 0x%08X) starts.\n",chunk.type,chunk.size);
	//getch();

	switch (chunk.type)
	{
		// 0x0900 EOBJ_CHUNK_VERSION
		// 0x0911 EOBJ_CHUNK_0911
		// 0x0912 EOBJ_CHUNK_USERDATA
		// 0x0925 EOBJ_CHUNK_LOD_REF
		// 0x0903 EOBJ_CHUNK_FLAGS
		// 0x0910 EOBJ_CHUNK_MESHES -->
		// 0x0907 EOBJ_CHUNK_SURFACES_2 
		// 0x0922 EOBJ_CHUNK_REVISION
		
	case 0x0900:{
		fread(&s_data,chunk.size,1,inputFile);
		if (!destFlag) printf(" * 0x0900 EOBJ_CHUNK_VERSION = %d\n",s_data); 
		return 1;}
	case 0x0911:{
		char *tmpdata = new char[chunk.size];
		fread(tmpdata,chunk.size,1,inputFile);
		if (!destFlag) printf(" * 0x0911 EOBJ_CHUNK_0911\n"); 
		delete [] tmpdata;
		return 1;}
	case 0x0912:{
		char *tmpdata = new char[chunk.size];
		fread(tmpdata,chunk.size,1,inputFile);
		if (!destFlag) printf(" * 0x0912 EOBJ_CHUNK_USERDATA\n"); 
    printf("   %s\n",tmpdata); 
		delete [] tmpdata;
		return 1;}
	case 0x0925:{
		fread(&c_data,chunk.size,1,inputFile);
		if (!destFlag) printf(" * 0x0925 EOBJ_CHUNK_LOD_REF = %d\n",c_data); 
		return 1;}
	case 0x0903:{
		fread(&l_data,chunk.size,1,inputFile);
		if (!destFlag) printf(" * 0x0903 EOBJ_CHUNK_FLAGS = %d\n",l_data); 
		return 1;}
/////////////////////////////////////////////////////////////////////////////

// EMESH_CHUNK_VERSION = 0x1000
// EMESH_CHUNK_MESHNAME = 0x1001
// EMESH_CHUNK_BBOX = 0x1004 
// EMESH_CHUNK_FLAGS = 0x1002
// EMESH_CHUNK_OPTIONS = 0x1010
// EMESH_CHUNK_VERTS = 0x1005
// EMESH_CHUNK_FACES = 0x1006 
// EMESH_CHUNK_SG = 0x1013 ?
// EMESH_CHUNK_VMREFS = 0x1008 ?
// EMESH_CHUNK_SFACE = 0x1009 
// EMESH_CHUNK_VMAPS_2 = 0x1012 

	case 0x1000:{
		fread(&s_data,chunk.size,1,inputFile);
		if (!destFlag) printf(" + 0x1000 EMESH_CHUNK_VERSION = %d\n",s_data); 
		return 1;}

	case 0x1001:{
		char *tmpdata = new char[chunk.size];
		fread(tmpdata,chunk.size,1,inputFile);
		if (!destFlag) printf(" + 0x1001 EMESH_CHUNK_MESHNAME = %s\n",tmpdata); 
		delete [] tmpdata;
		return 1;}

	case 0x1004:{
		ogf_bbox *tmpdata = new ogf_bbox;
		fread(tmpdata,chunk.size,1,inputFile);
		if (!destFlag) printf(" + 0x1004 EMESH_CHUNK_BBOX = (%f,%f,%f),(%f,%f,%f)\n",
     tmpdata->min.x,tmpdata->min.y,tmpdata->min.z,
     tmpdata->max.x,tmpdata->max.y,tmpdata->max.z); 
		delete tmpdata;
		return 1;}

	case 0x1002:{
		fread(&c_data,chunk.size,1,inputFile);
		if (!destFlag) printf(" + 0x1002 EMESH_CHUNK_FLAGS = %d\n",c_data); 
		return 1;}

	case 0x1010:{
		char *tmpdata = new char[chunk.size];
		fread(tmpdata,chunk.size,1,inputFile);
		if (!destFlag) printf(" + 0x1010 EMESH_CHUNK_OPTIONS\n"); 
		delete [] tmpdata;
		return 1;}

	case 0x1005:{

      fread(&v,4,1,inputFile);

			if (destFlag) printf("v=%d, ", v.count); else
				printf(" + 0x1005 EMESH_CHUNK_VERTS: verts=%d\n", v.count); 

			Vertex *vtx = new Vertex[v.count];
			fread(vtx,v.count*sizeof(Vertex),1,inputFile);

				if (destFlag) {
					for (cnt = 0; cnt < v.count; cnt++) ; // fprintf(outputFile, "v %f %f %f\n",vtx[cnt].x,vtx[cnt].y,vtx[cnt].z); 
				} // else 
				// for (cnt = 0; cnt < v.count; cnt++) 
				  // printf("%05d v %f %f %f\n",cnt,vtx[cnt].x,vtx[cnt].y,vtx[cnt].z);
				delete [] vtx;

			if ((chunk.size-4) > (v.count * 12)) 
			{
    		char *tmpdata = new char[chunk.size-4-(v.count*12)];
        fread(tmpdata,chunk.size-4-(v.count*12),1,inputFile); // fake read
     		delete [] tmpdata;
     		printf("chunk is reduced !!!!!!!!\n");
     	}

		return 1;}

	case 0x1006:{
			fread(&f,4,1,inputFile);
			if (destFlag) printf("f=%d, ", f.count); else
				printf(" + 0x1006 EMESH_CHUNK_FACES: faces=%d\n", f.count); 

				OBJFace *fcs = new OBJFace[f.count];
				fread(fcs,f.count*sizeof(OBJFace),1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < f.count; cnt++) ; // fprintf(outputFile, "%05d f %d/%d %d/%d %d/%d\n",cnt,
         //fcs[cnt].v0,fcs[cnt].v1,fcs[cnt].v2,
         //fcs[cnt].vref0,fcs[cnt].vref1,fcs[cnt].vref2); 
				}  else 
				 for (cnt = 0; cnt < f.count; cnt++) 
				 printf("%05d f %d/%d/%d (%d/%d/%d)\n",cnt,
         fcs[cnt].v0,fcs[cnt].v1,fcs[cnt].v2,
         fcs[cnt].vref0,fcs[cnt].vref1,fcs[cnt].vref2); 
				delete [] fcs;
		return 1;}

	case 0x1013:{
			if (destFlag) printf("e=%d, ", chunk.size >> 2); else
				printf(" + 0x1013 EMESH_CHUNK_SG: elements=%d\n", chunk.size >> 2); 

				long *ints = new long[chunk.size >> 2];
				fread(ints,chunk.size,1,inputFile);
				if (destFlag) {
					for (cnt = 0; cnt < (chunk.size >> 2); cnt++) ; // fprintf(outputFile, "%05d sg 0x%08x\n",cnt,ints[cnt]);
				}//  else 
				 //for (cnt = 0; cnt < (chunk.size >> 2); cnt++) 
				  //printf("%05d sg 0x%08x\n",cnt,ints[cnt]);
				delete [] ints;
		return 1;}

	case 0x1008:{
      fread(&l_data,4,1,inputFile);
			if (destFlag) printf("uvmaps=%d (%d * 3), ", l_data, l_data/3); else
				printf(" + 0x1008 EMESH_CHUNK_VMREFS: uvmaps=%d\n", l_data); 

        l_data = chunk.size - 4; // header
        uvmap vm;
        long k = 0;
				while (l_data > 0)
				{
        fread(&vm.count,1,1,inputFile);
        fread(&vm.m,vm.count*sizeof(submap),1,inputFile);

        for (cnt = 0; cnt < vm.count; cnt++)
				 printf("%05d tmap (%d of %d) vind %d ind %d\n",k,cnt,vm.count,vm.m[cnt].vmap_index,vm.m[cnt].index);
				 k++;

        l_data--; l_data -= vm.count*sizeof(submap); 
				}
		return 1;}

	case 0x1009:{
      fread(&s_data,2,1,inputFile);
			if (destFlag) printf("materials=%d, ", s_data); else
				printf(" + 0x1009 EMESH_CHUNK_SFACE: materials=%d\n", s_data); 
		char *tmpdata = new char[chunk.size-2];
		fread(tmpdata,chunk.size-2,1,inputFile);
		long p = 0;
		for (long j=0;j<s_data;j++)
		{
		printf("material - %d of %d\n",j,s_data);
		printf("name: ");p+=printf("%s\n",&tmpdata[p]);
		long facets, ind;
		memcpy(&facets,&tmpdata[p],sizeof(long)); p+=4;
		printf("facets: %d\n",facets);  
		for (long i=0;i<facets;i++)
		{
		memcpy(&ind,&tmpdata[p],sizeof(long)); p+=4;
		printf("ind %d face %d\n",i,ind);
		}
		}
		delete [] tmpdata;
		return 1;}

	case 0x1012:{
	fread(&l_data,4,1,inputFile);
			if (destFlag) printf("txmaps=%d, ", l_data); else
				printf(" + 0x1012 EMESH_CHUNK_VMAPS_2: tables=%d\n", l_data); 
	txmap *tm = new txmap[l_data];
	for (long j=0;j<l_data;j++)
	{
		long pos = ftell(inputFile);
		fread(&tm[j].name,MAX_T_LEN,1,inputFile);
		fseek(inputFile, pos+strlen(tm[j].name)+1, SEEK_SET); 
		
		fread(&tm[j].dim,sizeof(txmap)-MAX_T_LEN,1,inputFile);
		long tcnt = (tm[j].txcount);

    printf(" tmap '%s' %d of %d, dim %d, tcnt = %d, polymap = %d, type = %d\n",
            tm[j].name,j,l_data,tm[j].dim,tm[j].txcount,tm[j].polymap,tm[j].type);
				
				if (!tm[j].type)
				{
					uvcoord *vm = new uvcoord[tcnt];
					fread(vm,sizeof(uvcoord)*tcnt,1,inputFile);

				if (destFlag) {
					for (cnt = 0; cnt < tcnt; cnt++) ; // fprintf(outputFile, "%05d uv %f %f %d %f\n",cnt,
            //uv[cnt].u, uv[cnt].v,unk0[cnt],unk1[cnt]);
				} else 
				 for (cnt = 0; cnt < tcnt; cnt++) ;
				 //printf("%05d uv %f %f\n",cnt, vm[cnt].u, vm[cnt].v);

					delete [] vm;
				}
				pos = ftell(inputFile);
				
				if (tm[j].polymap) pos += (sizeof(float)*tcnt); // skip pindices, sizeof(float)*tcnt
        if (!tm[j].type) pos += (sizeof(float)*tcnt); // skip vindices, sizeof(float)*tcnt
				else 
				pos += (sizeof(uvcoord)*tcnt); // skip vm, sizeof(uvcoord)*tcnt

				fseek(inputFile, pos, SEEK_SET);
				
	}
	delete [] tm;
		return 1;}
	case 0x0907:{
		fread(&l_data,4,1,inputFile);
    printf(" + 0x0907 EOBJ_CHUNK_SURFACES_2 - materials: %d\n",l_data);
		char *tmpdata = new char[chunk.size-4];
		fread(tmpdata,chunk.size-4,1,inputFile);
		long p = 0;
		for (long j=0;j<l_data;j++)
		{
		printf("material - %d of %d\n",j,l_data);
		printf("mat_name: ");p+=printf("%s\n",&tmpdata[p]);
		printf("shader_name: ");p+=printf("%s\n",&tmpdata[p]);
		printf("shader_xrlc_name: ");p+=printf("%s\n",&tmpdata[p]);
		printf("game_mtl_name: ");p+=printf("%s\n",&tmpdata[p]);
		printf("texture_name: ");p+=printf("%s\n",&tmpdata[p]);
		printf("vmap_name: ");p+=printf("%s\n",&tmpdata[p]);
		matpars m;
		memcpy(&m,&tmpdata[p],sizeof(matpars));
		printf("m_flags: 0x%08X\n",m.m_flags);
		printf("fvf: 0x%08X\n",m.fvf);
		printf("unk: 0x%08X\n",m.unk);
    p+=12; 
		}
		delete [] tmpdata;
		return 1;}
	case 0x0922:{
    printf(" + 0x0922 EOBJ_CHUNK_REVISION\n");
		char *tmpdata = new char[chunk.size];
		fread(tmpdata,chunk.size,1,inputFile);
		long p = 0;
		printf("creator: ");p+=printf("%s\n",&tmpdata[p]);
		long time;
		memcpy(&time,&tmpdata[p],sizeof(long));
		printf("time: 0x%08X\n",time); p+=8; 
		printf("last_creator: ");p+=printf("%s\n",&tmpdata[p]);
		memcpy(&time,&tmpdata[p],sizeof(long));
		printf("time: 0x%08X\n",time);
		delete [] tmpdata;
		return 1;}
 	default:{
    printf(" + 0x%04X EOBJ_CHUNK_??? = %d\n", chunk.type,chunk.size);
	return 0;}
	 }
//return 0;
}

int main(int argc, char* argv[])
{
  ChunkIFF chunk;
  unsigned long value, i, subid=0,
  subfullsize=0, subsubfullsize=0, fullsize=0;
	char fileName[256];
  FILE *inputFile;

 	printf("X-Ray OBJECT parser [201012xx]\n");
	if (argc < 2) 
  {
		printf("usage: parse_obj < srcfile.object >\n");
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

	fseek(inputFile,0,SEEK_SET);

	for (;;)
	{
label:
	if (fread( &chunk, 4, 1, inputFile ) == 0) break;

	if (chunk.type ==0x7777) { fseek(inputFile,8,SEEK_SET); fullsize+=8; goto label; }

	if ((chunk.type !=0x0910))  {
	fread( &chunk.size, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
	if (DecodeChunk(chunk,inputFile) == 0)
	for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
  fullsize+= 8;fullsize+= chunk.size;
  }
  else {
	fread( &chunk.size, 4, 1, inputFile );
	printf("Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
  fullsize+= 8;fullsize+= chunk.size;
	// sub#1
  subfullsize = chunk.size;
	for (;;)
	{
  fread( &chunk, 8, 1, inputFile );
	printf("--> SubChunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
  subfullsize-= 8;subfullsize-= chunk.size;
	if (chunk.type == subid) {
	// sub#2
  subsubfullsize = chunk.size;
	for (;;)
	{
  fread( &chunk, 8, 1, inputFile );
	printf("----> Chunk #:0x%04X, size is 0x%08X (%u)\n",chunk.type,chunk.size,chunk.size);
	if (DecodeChunk(chunk,inputFile) == 0)
	for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
  subsubfullsize-= 8;subsubfullsize-= chunk.size;
  if (!subsubfullsize) { subid++; break; }
	}
	} else {
	for (i = 0;i < chunk.size;i++) fread( &value, 1, 1, inputFile );
	}
	// sub#2
  if (!subfullsize) break;
	}
	// sub#1
  }
  }
	printf("Full size is 0x%08X (%u)\n",fullsize,fullsize);

  fclose(inputFile);
	return 0;
	}