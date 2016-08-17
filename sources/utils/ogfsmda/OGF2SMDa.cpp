#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <d3d8.h>
#include <d3dx8math.h>
#include <math.h>
#include <windows.h>


typedef float vec_t;

typedef vec_t vec3_t[3];// x,y,z
typedef vec_t vec4_t[4];// x,y,z,w

typedef float vec_t;

typedef vec_t vec3_t[3];	// x,y,z
typedef vec_t vec4_t[4];
void QuaternionMatrix( const vec4_t quaternion, float (*matrix)[4] )
{

	matrix[0][0] = 1.0 - 2.0 * quaternion[1] * quaternion[1] - 2.0 * quaternion[2] * quaternion[2];
	matrix[1][0] = 2.0 * quaternion[0] * quaternion[1] + 2.0 * quaternion[3] * quaternion[2];
	matrix[2][0] = 2.0 * quaternion[0] * quaternion[2] - 2.0 * quaternion[3] * quaternion[1];

	matrix[0][1] = 2.0 * quaternion[0] * quaternion[1] - 2.0 * quaternion[3] * quaternion[2];
	matrix[1][1] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[2] * quaternion[2];
	matrix[2][1] = 2.0 * quaternion[1] * quaternion[2] + 2.0 * quaternion[3] * quaternion[0];

	matrix[0][2] = 2.0 * quaternion[0] * quaternion[2] + 2.0 * quaternion[3] * quaternion[1];
	matrix[1][2] = 2.0 * quaternion[1] * quaternion[2] - 2.0 * quaternion[3] * quaternion[0];
	matrix[2][2] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[1] * quaternion[1];
	matrix[0][3]=0;
	matrix[1][3]=0;
	matrix[2][3]=0;
}

void GUtil_GetEulerAngles2(const float m[3][4], float angles[3])
{
	float cy;
	
	// order of angles x,y,z
	
	cy = (float) sqrt(m[0][0] * m[0][0] + m[0][1] * m[0][1]); // AX^2 + BX^2
	
	if (cy > 16 * 0.001)
	{
		angles[0] = (float) atan2(m[1][2], m[2][2]); // CY, CZ
		angles[1] = (float) atan2(-m[0][2], cy); // -CX, cy
		angles[2] = (float) atan2(m[0][1], m[0][0]); // BX, AX
	}
	else
	{
		angles[0] = (float) atan2(-m[2][1], m[1][1]); //-BZ, BY
		angles[1] = (float) atan2(-m[0][2], cy);// -CX, cy
		angles[2] = 0;
	}
}

void GUtil_GetEulerAngles2a(const float m[3][4], float angles[3])
{
	float cy;
	
	// order of angles x,y,z
	
	cy = (float) sqrt(m[0][0] * m[0][0] + m[0][2] * m[0][2]); // AX^2 + BX^2
	
	if (cy > 16 * 0.001)
	{
		angles[0] = (float) atan2(m[2][1], m[1][1]); // CY, CZ
		angles[1] = (float) atan2(-m[0][1], cy); // -CX, cy
		angles[2] = (float) atan2(m[0][2], m[0][0]); // BX, AX
	}
	else
	{
		angles[0] = (float) atan2(-m[1][2], m[2][2]); //-BZ, BY
		angles[1] = (float) atan2(-m[0][1], cy);// -CX, cy
		angles[2] = 0;
	}
}

class __declspec(dllimport) xrCore
{
public:
	void _initialize(char const *, void (__stdcall *)(char const *), int);
	void _destroy(void);
};
__declspec(dllimport) extern xrCore Core;
class xrDebug
{
public:
	void fail(char const *what, char const *_where, int line);
	void fail(char const *, char const *, char const *, int);
};
extern __declspec(dllimport) xrDebug Debug;

class __declspec(dllimport) xrMemory
{
public:
	xrMemory(void);
	void mem_free(void *buffer);
};
__declspec(dllimport) extern xrMemory Memory;
struct str_value
{
	int ref;
	int len;
	int crc;
};
class __declspec(dllimport) str_container
{
public:
	str_value *dock(const char*);
	void dump(void);
};
__declspec(dllimport) extern str_container * g_pStringContainer;
template<class T>
struct _vector3
{
	vec3_t vert;
};
class shared_str
{
public:
	
};
class __declspec(dllimport) IReader 
{
public:
	IReader(class IReader const &);
	IReader(void *, int, int);
	IReader(void);
	~IReader(void);
	void advance(int);
	unsigned int advance_term_string(void);
	void close(void);
	unsigned int correction(unsigned int);
	int elapsed(void)const;
	IReader *open_chunk(unsigned int);
	IReader *open_chunk_iterator(unsigned int &, class IReader *);
	void r(void *, int);
	float r_angle16(void);
	float r_angle8(void);
	//void r_dir(struct _vector3<float> &);does not work
	//void __thiscall IReader::r_fcolor(struct _color<float> &)//does not work either
	int r_chunk_safe(unsigned int, void *, unsigned int);
	int r_chunk(unsigned int, void *);
	void *pointer(void)const;
	int length(void)const;
	unsigned int find_chunk(unsigned int, int *);
	unsigned short r_u16(void);
	unsigned int r_u32(void);
	unsigned char r_u8(void);
	void r_fvector3(_vector3<float> &);
	short r_s16(void);
	int tell(void)const;
	float r_float(void);
	float r_float_q16(float, float);
	float r_float_q8(float, float);
	void r_stringZ(char *, unsigned int);
	void r_stringZ(shared_str &);
	void seek(int);
	int eof(void)const;
};
class __declspec(dllimport) IWriter
{
public:
	void w_u32(unsigned int size);
	void w_stringZ(char const *string);
	void w_chunk(unsigned int version, void *buffer, unsigned int length);
	void close_chunk(void);
	void open_chunk(unsigned int);
};

template<class T>
class __declspec(dllimport) xr_vector
{
public:
	
};

//template<class T>
class __declspec(dllimport) CLocatorAPI
{
public:
	CLocatorAPI(void);
	~CLocatorAPI(void);
	void _initialize(unsigned int, char const *);
	IReader *r_open(char const *filename);
	IReader *r_open(char const *, char const *);
	void w_close(class IWriter * &);
	void r_close(IReader * &filename);
	IWriter * w_open(char const *filename);
	char const *update_path(char *, char const *, char const *);
	xr_vector <char*>* file_list_open(char const *, unsigned int);
//	void file_list_close(template<> class xr_vector<char *> * &); -- nattefrost.. wtf? :)
	void file_list_close(class xr_vector<char *> * &);
};
__declspec(dllimport) extern CLocatorAPI * xr_FS;

class CPU 
{
public:
	unsigned __int64 cycles();
};
unsigned int __stdcall crc32(void const *, unsigned int);

struct __declspec(dllimport) smem_value
{
	int a;
	int b;
	int c;
};
class __declspec(dllimport) smem_container
{
public:
	smem_value *dock(unsigned int, unsigned int, void *);
	
};
__declspec(dllimport) extern smem_container * g_pSharedMemoryContainer;

struct OGFVertex_v0 // x-ray actor_editor (0x00000112)
{
	vec3_t p;
	vec3_t n;
	float u,v;
};

struct OGFVertex_v1 // 60 = 15 * 4
{
	vec3_t p;
	vec3_t n;
	vec3_t t;
	vec3_t b;
	float u,v;
	unsigned int bone;
};

struct OGFVertex_v2 // 64 = 16 * 4
{
	unsigned short bone[2];
	vec3_t p;
	vec3_t n;
	vec3_t t;
	vec3_t b;
	float w;
	float u,v;
};

struct OGFVertex_v3 // 70
{
	unsigned short bone[3];
	vec3_t p;
	vec3_t n;
	vec3_t t;
	vec3_t b;
	float w[2];
	float u,v;
};

struct OGFVertex_v4 // 76
{
	unsigned short bone[4];
	vec3_t p;
	vec3_t n;
	vec3_t t;
	vec3_t b;
	float w[3];
	float u,v;
};

struct Test_t
{
	short x,y,z,w;
	
};
Test_t *test;
struct Test_t2
{
	char x,y,z;
	
};
Test_t2 *test2;
struct Indices
{
	unsigned short tris[3];
};

struct Bones_t
{
	char name[32];
	char parentname[32];
	D3DXMATRIX Parent;
	int num;
	int parent;
	vec3_t pos;
	vec3_t rot;
	
};
struct Animation_t
{
	
	float x,y,z,w;
	
};
struct Translate_t
{
	vec3_t translate;
};
struct Animbones
{
	vec3_t pos;
	vec3_t pos2;
	
	Animation_t *quaternion;
	Translate_t *trans;
};


typedef struct
{
	char label[32];// sequence label
	int numframes;
	Animbones *animbones;
	
} mstudioseqdesc_t;

class Model
{
public:
	int numseq;
	int numbones;
	Bones_t pbones[128];
	mstudioseqdesc_t *pseqdescs;
	void MeshToSMD(IReader *file);
	void AnimToSMD(IReader *file);
	void SaveAnimSMD();
	
};
Model *model;
int GetNum(char *name)
{
	int i;
	for(i=0;i<model->numbones;i++)
	{
		if(strcmp(name,model->pbones[i].name)==0)
		{
			return i;
		}
	}
	return 0;
}



void VectorInverse (vec3_t v)
{
	v[0] = -v[0];
	v[1] = -v[1];
	v[2] = -v[2];
}
void CalkParent(IReader *file)
{
	int i;
	for(i=0;i<model->numbones;i++)
	{
		if(i==0)
		{
			model->pbones[i].parent=-1;
		}
		else
		{
			model->pbones[i].parent=GetNum(model->pbones[i].parentname);
		}
	}
	
	
}
void SearchForParent(IReader *file)
{
	int i;
	for(i=0;i<model->numbones;i++)
	{
		if(i==0)
		{
			file->r_stringZ(model->pbones[i].name,32);
			memset(model->pbones[i].parentname,0,32);
			file->advance(61);
		}
		else
		{
			file->r_stringZ(model->pbones[i].name,32);
			file->r_stringZ(model->pbones[i].parentname,32);
			file->advance(60);
		}
	}
	/*
	int i,num;
	char name[256];
	file->open_chunk(0x0F);
	file->r_u16();
	file->r_u16();
	file->r_stringZ(name,256);
	file->r_u16();
	num=0;
	for(i=0;i<model->numbones;i++)
	{
	file->r_stringZ(name,32);
	num=file->r_u32();
	memcpy(model->pbones[num].name,name,32);
	model->pbones[num].num=num;////sorting bones...
	}
	file->find_chunk(0xD,0);
	file->r_u32();
	file->r_stringZ(name,32);
	model->pbones[0].parent=-1;//The first bone - always -1;
	for(i=0;i<model->numbones-1;i++)
	{
	file->advance(62);
	file->r_stringZ(name,32);
	
		file->r_stringZ(model->pbones[i].parentname,32);
		model->pbones[i+1].parent=GetNum(model->pbones[i].parentname);
		}
		
	*/
}

class SWIs_t
{
public:
  unsigned int m_reserved[4];
	unsigned int count;
	unsigned int offset;
	unsigned short num_tris;
	unsigned short num_verts;
	
	
};
void Model::MeshToSMD(IReader *file)
{
	int swi;
	
	_vector3<float> vector; 
	char name[256];
	int i;
	char fileName[_MAX_PATH];
	
	if ((!file->find_chunk(0x02,0)) && (!file->find_chunk(0x09,0)))
   { printf("MeshToSMD() ... NO_DATA\n"); return; }

	GetCurrentDirectory(_MAX_PATH, fileName);
	mkdir("smd");
	strcat(fileName,"\\smd\\reference.smd");
	FILE *smd=fopen(fileName,"wb");
	
	fprintf(smd,"version 1\n");
	
  if (model->numbones)
  { // bones start
		fprintf(smd,"nodes\n");
		for(i=0;i<model->numbones;i++)
		{
			fprintf(smd,"%3i \"%s\" %i\n",i,pbones[i].name,pbones[i].parent);///oh... work normally...
		}
		
		file->open_chunk(0x10);
		for(i=0;i<model->numbones;i++)
		{
			file->r_u32();
			file->r_stringZ(name,256);
			file->advance(112);
			file->r_u32();
			file->advance(48);
			file->r_float();
			file->r_float();
			file->r_u32();
			file->r_float();
			file->r_float();
			file->r_float();
			file->r_fvector3(vector);
			pbones[i].rot[0]=vector.vert[0];
			pbones[i].rot[1]=vector.vert[1]; 
			pbones[i].rot[2]=vector.vert[2]; 
			file->r_fvector3(vector);
			pbones[i].pos[0]=vector.vert[0];
			pbones[i].pos[1]=vector.vert[1]; 
			pbones[i].pos[2]=vector.vert[2]; 
			file->r_float();
			file->r_fvector3(vector);
		}
    //fprintf(smd,"%3i \"SMDImport\" %d\n",i,-1);
		fprintf(smd,"end\nskeleton\ntime 0\n");
		D3DXMATRIX Local,Parent,inversed;
		D3DXQUATERNION Out;
		float matrix[3][4];
		float angles[3];
		vec4_t q;
		for(i=0;i<model->numbones;i++)
		{
			
			
		/*
		Local._11=0;
		Local._12=0;
		Local._13=0;
		Local._14=0;
		Local._21=0;
		Local._22=0;
		Local._23=0;
		Local._24=0;
		Local._31=0;
		Local._32=0;
		Local._33=0;
			Local._34=0;*/
			
			//D3DXMatrixTransformation ???
			//D3DXMatrixRotationYawPitchRoll(&Local,pbones[i].rot[1],pbones[i].rot[0],pbones[i].rot[2]);
			D3DXMatrixRotationYawPitchRoll(&Local,pbones[i].rot[1],pbones[i].rot[0],pbones[i].rot[2]);
			//D3DXMatrixIdentity(&Local);
			Local._41=pbones[i].pos[0];
			Local._42=pbones[i].pos[1];
			Local._43=pbones[i].pos[2];
			
			D3DXMatrixInverse(&inversed,NULL,&Local);
			D3DXQuaternionRotationMatrix(&Out,&inversed);

			q[0]=Out.x;
			q[1]=Out.y;
			q[2]=Out.z;
			q[3]=Out.w;
			QuaternionMatrix(q,matrix);
			GUtil_GetEulerAngles2a(matrix,angles);

			fprintf(smd,"%i %f %f %f %f %f %f\n",i,
				Local._41*100,Local._43*100,Local._42*100, 
				angles[0],angles[1],angles[2]); /*
			fprintf(smd,"%i %f %f %f %f %f %f\n",i,
				pbones[i].pos[0],pbones[i].pos[1],pbones[i].pos[2], 
				pbones[i].rot[0],pbones[i].rot[1],pbones[i].rot[2]);
			*/
   		//fprintf(smd,"%i 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000\n",i);
   		

   		/* /// bred!!!!!!!!!!!
      D3DXMATRIX mOut;
      D3DXVECTOR3 vTrans;
      D3DXQUATERNION qRot;
      D3DXQuaternionRotationYawPitchRoll(&qRot, pbones[i].rot[1],
            pbones[i].rot[0],pbones[i].rot[2]);
      vTrans.x = pbones[i].pos[0];
      vTrans.y = pbones[i].pos[1];
      vTrans.z = pbones[i].pos[2];
      D3DXMatrixTransformation(&mOut,NULL,NULL,NULL,NULL,&qRot,&vTrans);

			//D3DXMatrixRotationYawPitchRoll(mrot.d3dmatrix,yaw,pitch,roll)
			//D3DXMatrixTranslation(mtrans.d3dmatrix,x,y,z)
			//D3DXMatrixMultiply(mtrans,mtrans,mrot)
			
			fprintf(smd,"%i %f %f %f %f %f %f\n",i,
				Local._41,Local._43,Local._42, // rev
				angles[0],angles[2],angles[1]); // rev
			*/
		}
		
		fprintf(smd,"end\n");

		} else { // no bones

		fprintf(smd,"nodes\n");
		fprintf(smd,"  0 \"bone01\" -1\nend\nskeleton\ntime 0\n");
		fprintf(smd,"0 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000\nend\n");

		}

		fprintf(smd,"triangles\n");
		int nummesh=0;
		IReader *mod;

		if (file->find_chunk(0x09,0)) 
		{

		do{
			mod=file->open_chunk(0x09);
			mod=mod->open_chunk(nummesh);
			nummesh++;
		}while(mod);
		nummesh--;

    printf("OGF4_CHILDREN chunk found\n");
		
		SWIs_t *swis;
		int numverts,size,ident;
		IReader *mod;

		for(int a=0;a<nummesh;a++)
		{
			char name[256];
			memset(name,0,256);
			
  		mod=file->open_chunk(0x09);
			mod=mod->open_chunk(a);
			swi=mod->find_chunk(0x06,0);
			swis=new SWIs_t;
			swis->offset=0;
			if(swi>0) mod->r(swis,sizeof(SWIs_t));

/*
      printf("blk%4d swi[   0] offset %5d(%5d) tris %5d(%5d) verts %5d\n",
        a,swis->offset, 
        39804-swis->offset,
        swis->num_tris,
        swis->num_tris*3,
        swis->num_verts);

		swis->count--; while (swis->count)
		 {
     mod->r((void*)&swis->offset,sizeof(SWIs_t)-20); swis->count--; // read rest of all
     printf("blk%4d swi[%4u] offset %5d(%5d) tris %5d(%5d) verts %5d\n",
        a,swis->count,
        swis->offset, 
        39804-swis->offset,
        swis->num_tris,
        swis->num_tris*3,
        swis->num_verts);
     }
*/
			mod->find_chunk(0x02,0);
			mod->r_stringZ(name,256);
			strcat(name,".dds");
			size=mod->find_chunk(0x03,0);
			ident=mod->r_u32();
			numverts=mod->r_u32();
			OGFVertex_v0 *v0;
			OGFVertex_v1 *v1;
			OGFVertex_v2 *v2;
			OGFVertex_v3 *v3;
			OGFVertex_v4 *v4;
/*
      printf("mtllib ogfmodel.mtl\no OGFObject0\n");
*/			
			if (ident == 0x00000112)
			{
				printf("OGF type: #0 (0x00000112), offset: %d\n",swis->offset); 
        v0 = new OGFVertex_v0[numverts], mod->r(v0,sizeof(OGFVertex_v0)*numverts);
			}
			else if ((ident == 0x1) || (ident == 0x12071980))
			{
        printf("OGF type: #1 (0x12071980), offset: %d\n",swis->offset);  
				v1 = new OGFVertex_v1[numverts], mod->r(v1,sizeof(OGFVertex_v1)*numverts);
			}
			else if ((ident == 0x2) || (ident == 0x240E3300))
			{
        printf("OGF type: #2 (0x240E3300), offset: %d\n",swis->offset); 
				v2 = new OGFVertex_v2[numverts], mod->r(v2,sizeof(OGFVertex_v2)*numverts);

/*
			for (int kk=0;kk < numverts;kk++)
			printf("v(%f,%f,%f), uv(%f,%f), m(%d,%d), w(0x%08X)\n",  // n(%f,%f,%f), 
      v2[kk].p[0], v2[kk].p[1], v2[kk].p[2],
		  //v2[kk].n[0], v2[kk].n[1], v2[kk].n[2], 
      v2[kk].u, v2[kk].v,v2[kk].bone[0],v2[kk].bone[1], v2[kk].w);

			for (int kk=0;kk < numverts;kk++)
			printf("v %f %f %f\n", v2[kk].p[0], v2[kk].p[1], v2[kk].p[2]);
			for (kk=0;kk < numverts;kk++)
		  printf("vt %f %f\n", v2[kk].u, -v2[kk].v);
			for (kk=0;kk < numverts;kk++)
			printf("vn %f %f %f\n", v2[kk].n[0], v2[kk].n[1], v2[kk].n[2]);

      printf("g OGFGroup0\nusemtl material0\n");
*/
			}
			else if (ident == 0x00000003)
			{
        printf("OGF type: #3 (0x00000003), offset: %d\n",swis->offset);
				v3 = new OGFVertex_v3[numverts], mod->r(v3,sizeof(OGFVertex_v3)*numverts);
			}
			else if (ident == 0x00000004)
			{
        printf("OGF type: #4 (0x00000004), offset: %d\n",swis->offset);
				v4 = new OGFVertex_v4[numverts], mod->r(v4,sizeof(OGFVertex_v4)*numverts);
			}
			else printf("OGF type: unknown (0x%08X), offset: %d\n",ident,swis->offset);
			
			int datasize,count;
			Indices *indices;
			datasize=mod->find_chunk(0x04,0);
			count=mod->r_u32();
			
			indices = new Indices[count];
			mod->r(indices,sizeof(unsigned short)*count);
			
			char* dir = name;
			char* p = strchr(dir, '\\');
			while(p)
			{
				*p++ = 0;
				
				dir = p;
				p = strchr(dir, '\\');
			}
			char* FileName = dir;
			unsigned short ii;
			for(int i=swis->offset/3; i<count/3;i++)
			{
				fprintf(smd,"%s\n", FileName);

        if (ident == 0x00000112)
        { 
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	0, // no bone!
						v0[ii].p[0]*100, v0[ii].p[2]*100, v0[ii].p[1]*100,
						v0[ii].n[0], v0[ii].n[2], v0[ii].n[1],
						v0[ii].u, 1-v0[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	0, // no bone!
						v0[ii].p[0]*100, v0[ii].p[2]*100, v0[ii].p[1]*100,
						v0[ii].n[0], v0[ii].n[2], v0[ii].n[1],
						v0[ii].u, 1-v0[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	0, // no bone!
						v0[ii].p[0]*100, v0[ii].p[2]*100, v0[ii].p[1]*100,
						v0[ii].n[0], v0[ii].n[2], v0[ii].n[1],
						v0[ii].u, 1-v0[ii].v);
        }
				else if ((ident == 0x1) || (ident == 0x12071980))
        {
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v1[ii].bone,
						v1[ii].p[0]*100, v1[ii].p[2]*100, v1[ii].p[1]*100,
						v1[ii].n[0], v1[ii].n[2], v1[ii].n[1],
						v1[ii].u, 1-v1[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v1[ii].bone,
						v1[ii].p[0]*100, v1[ii].p[2]*100, v1[ii].p[1]*100,
						v1[ii].n[0], v1[ii].n[2], v1[ii].n[1],
						v1[ii].u, 1-v1[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v1[ii].bone,
						v1[ii].p[0]*100, v1[ii].p[2]*100, v1[ii].p[1]*100,
						v1[ii].n[0], v1[ii].n[2], v1[ii].n[1],
						v1[ii].u, 1-v1[ii].v);
        }
				else if ((ident == 0x2) || (ident == 0x240e3300))
        {
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v2[ii].bone[0],
						v2[ii].p[0]*100, v2[ii].p[2]*100, v2[ii].p[1]*100, 
						v2[ii].n[0], v2[ii].n[2], v2[ii].n[1],
						v2[ii].u, 1-v2[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n", v2[ii].bone[0],
						v2[ii].p[0]*100, v2[ii].p[2]*100, v2[ii].p[1]*100, 
						v2[ii].n[0], v2[ii].n[2], v2[ii].n[1],
						v2[ii].u, 1-v2[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n", v2[ii].bone[0],
						v2[ii].p[0]*100, v2[ii].p[2]*100, v2[ii].p[1]*100, 
						v2[ii].n[0], v2[ii].n[2], v2[ii].n[1],
						v2[ii].u, 1-v2[ii].v);
        }
				else if (ident == 0x00000003)
        {
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n", v3[ii].bone[0],
						v3[ii].p[0]*100, v3[ii].p[2]*100, v3[ii].p[1]*100,
						v3[ii].n[0], v3[ii].n[2], v3[ii].n[1],
						v3[ii].u, 1-v3[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n", v3[ii].bone[0],
						v3[ii].p[0]*100, v3[ii].p[2]*100, v3[ii].p[1]*100,
						v3[ii].n[0], v3[ii].n[2], v3[ii].n[1],
						v3[ii].u, 1-v3[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v3[ii].bone[0],
						v3[ii].p[0]*100, v3[ii].p[2]*100, v3[ii].p[1]*100,
						v3[ii].n[0], v3[ii].n[2], v3[ii].n[1],
						v3[ii].u, 1-v3[ii].v);
        }
				else if (ident == 0x00000004)
        {
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v4[ii].bone[0],
						v4[ii].p[0]*100, v4[ii].p[2]*100, v4[ii].p[1]*100,
						v4[ii].n[0], v4[ii].n[2], v4[ii].n[1],
						v4[ii].u, 1-v4[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v4[ii].bone[0],
						v4[ii].p[0]*100, v4[ii].p[2]*100, v4[ii].p[1]*100,
						v4[ii].n[0], v4[ii].n[2], v4[ii].n[1],
						v4[ii].u, 1-v4[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v4[ii].bone[0],
						v4[ii].p[0]*100, v4[ii].p[2]*100, v4[ii].p[1]*100,
						v4[ii].n[0], v4[ii].n[2], v4[ii].n[1],
						v4[ii].u, 1-v4[ii].v);
        }
        else printf("unknown OGF type!\n");
		}
		delete swis;
/*
			for(i=0; i<count/3;i++)
				printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
        indices[i].tris[0]+1,indices[i].tris[0]+1,indices[i].tris[0]+1,
        indices[i].tris[1]+1,indices[i].tris[1]+1,indices[i].tris[1]+1,
        indices[i].tris[2]+1,indices[i].tris[2]+1,indices[i].tris[2]+1);
*/

			if (ident == 0x00000112) delete v0;
			else if ((ident == 0x1) || (ident == 0x12071980)) delete v1;
			else if ((ident == 0x2) || (ident == 0x240E3300)) delete v2;
			else if (ident == 0x00000003) delete v3;
			else if (ident == 0x00000004) delete v4;
	}
 	} else { // no subblocks
		/////////////////////////////////////////////////////////////////////

    printf("OGF4_CHILDREN chunk NOT found\n");
		
		int numverts,size,ident;
		
		char name[256];
		memset(name,0,256);
		
		file->find_chunk(0x02,0);
		file->r_stringZ(name,256);
		strcat(name,".dds");
		size=file->find_chunk(0x03,0);
		ident=file->r_u32();
		numverts=file->r_u32();
		OGFVertex_v0 *v0;
		OGFVertex_v1 *v1;
		OGFVertex_v2 *v2;
		OGFVertex_v3 *v3;
		OGFVertex_v4 *v4;
		
		if (ident == 0x00000112)
		{
			printf("OGF type: #0 (0x00000112)\n"); 
			v0 = new OGFVertex_v0[numverts], file->r(v0,sizeof(OGFVertex_v0)*numverts);
		}
				else if ((ident == 0x1) || (ident == 0x12071980))
        {
					printf("OGF type: #1 (0x12071980)\n"); 
					v1 = new OGFVertex_v1[numverts], file->r(v1,sizeof(OGFVertex_v1)*numverts);
        }
				else if ((ident == 0x2) || (ident == 0x240E3300))
        {
					printf("OGF type: #2 (0x240E3300)\n"); 
					v2 = new OGFVertex_v2[numverts], file->r(v2,sizeof(OGFVertex_v2)*numverts);
        }
				else if (ident == 0x00000003)
        {
					printf("OGF type: #3 (0x00000003)\n"); 
					v3 = new OGFVertex_v3[numverts], file->r(v3,sizeof(OGFVertex_v3)*numverts);
        }
				else if (ident == 0x00000004)
        {
					printf("OGF type: #4 (0x00000004)\n"); 
					v4 = new OGFVertex_v4[numverts], file->r(v4,sizeof(OGFVertex_v4)*numverts);
        }
        else printf("OGF type: unknown (0x%08X)\n");
				
				int datasize,count;
				Indices *indices;
				datasize=file->find_chunk(0x04,0);
				count=file->r_u32();
				
				indices = new Indices[count];
				file->r(indices,sizeof(unsigned short)*count);
				
				char* dir = name;
				char* p = strchr(dir, '\\');
				while(p)
				{
					*p++ = 0;
					
					dir = p;
					p = strchr(dir, '\\');
				}
				char* FileName = dir;
				unsigned short ii;
				for(int i=0; i<count/3;i++)
				{
				fprintf(smd,"%s\n", FileName);

        if (ident == 0x00000112)
        { 
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	0, // no bone!
						v0[ii].p[0]*100, v0[ii].p[2]*100, v0[ii].p[1]*100,
						v0[ii].n[0], v0[ii].n[2], v0[ii].n[1],
						v0[ii].u, 1-v0[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	0, // no bone!
						v0[ii].p[0]*100, v0[ii].p[2]*100, v0[ii].p[1]*100,
						v0[ii].n[0], v0[ii].n[2], v0[ii].n[1],
						v0[ii].u, 1-v0[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	0, // no bone!
						v0[ii].p[0]*100, v0[ii].p[2]*100, v0[ii].p[1]*100,
						v0[ii].n[0], v0[ii].n[2], v0[ii].n[1],
						v0[ii].u, 1-v0[ii].v);
        }
				else if ((ident == 0x1) || (ident == 0x12071980))
        {
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v1[ii].bone,
						v1[ii].p[0]*100, v1[ii].p[2]*100, v1[ii].p[1]*100,
						v1[ii].n[0], v1[ii].n[2], v1[ii].n[1],
						v1[ii].u, 1-v1[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v1[ii].bone,
						v1[ii].p[0]*100, v1[ii].p[2]*100, v1[ii].p[1]*100,
						v1[ii].n[0], v1[ii].n[2], v1[ii].n[1],
						v1[ii].u, 1-v1[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v1[ii].bone,
						v1[ii].p[0]*100, v1[ii].p[2]*100, v1[ii].p[1]*100,
						v1[ii].n[0], v1[ii].n[2], v1[ii].n[1],
						v1[ii].u, 1-v1[ii].v);
        }
				else if ((ident == 0x2) || (ident == 0x240e3300))
        {
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v2[ii].bone[0],
						v2[ii].p[0]*100, v2[ii].p[2]*100, v2[ii].p[1]*100, 
						v2[ii].n[0], v2[ii].n[2], v2[ii].n[1],
						v2[ii].u, 1-v2[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n", v2[ii].bone[0],
						v2[ii].p[0]*100, v2[ii].p[2]*100, v2[ii].p[1]*100, 
						v2[ii].n[0], v2[ii].n[2], v2[ii].n[1],
						v2[ii].u, 1-v2[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n", v2[ii].bone[0],
						v2[ii].p[0]*100, v2[ii].p[2]*100, v2[ii].p[1]*100, 
						v2[ii].n[0], v2[ii].n[2], v2[ii].n[1],
						v2[ii].u, 1-v2[ii].v);
        }
				else if (ident == 0x00000003)
        {
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n", v3[ii].bone[0],
						v3[ii].p[0]*100, v3[ii].p[2]*100, v3[ii].p[1]*100,
						v3[ii].n[0], v3[ii].n[2], v3[ii].n[1],
						v3[ii].u, 1-v3[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n", v3[ii].bone[0],
						v3[ii].p[0]*100, v3[ii].p[2]*100, v3[ii].p[1]*100,
						v3[ii].n[0], v3[ii].n[2], v3[ii].n[1],
						v3[ii].u, 1-v3[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v3[ii].bone[0],
						v3[ii].p[0]*100, v3[ii].p[2]*100, v3[ii].p[1]*100,
						v3[ii].n[0], v3[ii].n[2], v3[ii].n[1],
						v3[ii].u, 1-v3[ii].v);
        }
				else if (ident == 0x00000004)
        {
					ii = indices[i].tris[2];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v4[ii].bone[0],
						v4[ii].p[0]*100, v4[ii].p[2]*100, v4[ii].p[1]*100,
						v4[ii].n[0], v4[ii].n[2], v4[ii].n[1],
						v4[ii].u, 1-v4[ii].v);
					ii = indices[i].tris[1];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v4[ii].bone[0],
						v4[ii].p[0]*100, v4[ii].p[2]*100, v4[ii].p[1]*100,
						v4[ii].n[0], v4[ii].n[2], v4[ii].n[1],
						v4[ii].u, 1-v4[ii].v);
					ii = indices[i].tris[0];
					fprintf(smd,"%i %f %f %f %f %f %f %f %f\n",	v4[ii].bone[0],
						v4[ii].p[0]*100, v4[ii].p[2]*100, v4[ii].p[1]*100,
						v4[ii].n[0], v4[ii].n[2], v4[ii].n[1],
						v4[ii].u, 1-v4[ii].v);
        }
        else printf("unknown OGF type!\n");
		}
			if (ident == 0x00000112) delete v0;
			else if ((ident == 0x1) || (ident == 0x12071980)) delete v1;
			else if ((ident == 0x2) || (ident == 0x240E3300)) delete v2;
			else if (ident == 0x00000003) delete v3;
			else if (ident == 0x00000004) delete v4;
		/////////////////////////////////////////////////////////////////////
  }
	fprintf(smd,"end\n");
	fclose(smd);
};
void Model::AnimToSMD(IReader *file)
{
	
	int i,pos,a;
	void *buffer,*buf;
	_vector3<float> vector,vector2; 
	IReader *chunk;
	numseq=0;
	do{
		chunk=file->open_chunk(0x0E);
		i=chunk->find_chunk(numseq+1,0);
		numseq++;
	}while(i!=0);
	numseq=numseq-1;//determine quantity of animations in model sequences;
	pseqdescs=new mstudioseqdesc_t[numseq+1];
	for(int j=0;j<numseq;j++){
		
		chunk=file->open_chunk(0x0E);
		i=chunk->find_chunk(j+1,0);
		chunk->r_stringZ(pseqdescs[j].label,32);
		pseqdescs[j].animbones=new Animbones[numbones];
		pseqdescs[j].numframes=chunk->r_u32();
		for(i=0;i<numbones;i++)
		{
			pseqdescs[j].animbones[i].quaternion=new Animation_t[pseqdescs[j].numframes];
			pseqdescs[j].animbones[i].trans=new Translate_t[pseqdescs[j].numframes];
			pos=chunk->r_u8();
			int unk1;
			if(pos==2)
			{
				buffer=malloc(8);
				chunk->r(buffer,8);
				test=(Test_t*)buffer;
				for(a=0;a<pseqdescs[j].numframes;a++)
				{
					pseqdescs[j].animbones[i].quaternion[a].x=(float)test->x*1/32767;
					pseqdescs[j].animbones[i].quaternion[a].y=(float)test->y*1/32767; 
					pseqdescs[j].animbones[i].quaternion[a].z=(float)test->z*1/32767;
					pseqdescs[j].animbones[i].quaternion[a].w=(float)test->w*1/32767;
					pseqdescs[j].animbones[i].trans[a].translate[0]=1;
					pseqdescs[j].animbones[i].trans[a].translate[1]=1;
					pseqdescs[j].animbones[i].trans[a].translate[2]=1;
				}
				chunk->r_fvector3(vector); 
				pseqdescs[j].animbones[i].pos[0]=vector.vert[0];
				pseqdescs[j].animbones[i].pos[1]=vector.vert[1]; 
				pseqdescs[j].animbones[i].pos[2]=vector.vert[2]; 
				pseqdescs[j].animbones[i].pos2[0]=0;
				pseqdescs[j].animbones[i].pos2[1]=0;
				pseqdescs[j].animbones[i].pos2[2]=0;
				free(buffer);
			}
			else if(pos==0)
			{
				buffer=malloc(8*pseqdescs[j].numframes);
				unk1=chunk->r_u32();
				chunk->r(buffer,8*pseqdescs[j].numframes);
				test=(Test_t*)buffer;
				for(a=0;a<pseqdescs[j].numframes;a++)
				{
					pseqdescs[j].animbones[i].quaternion[a].x=(float)test[a].x*1/32767;
					pseqdescs[j].animbones[i].quaternion[a].y=(float)test[a].y*1/32767; 
					pseqdescs[j].animbones[i].quaternion[a].z=(float)test[a].z*1/32767; 
					pseqdescs[j].animbones[i].quaternion[a].w=(float)test[a].w*1/32767;
					pseqdescs[j].animbones[i].trans[a].translate[0]=1;
					pseqdescs[j].animbones[i].trans[a].translate[1]=1;
					pseqdescs[j].animbones[i].trans[a].translate[2]=1;
				}
				chunk->r_fvector3(vector);
				pseqdescs[j].animbones[i].pos[0]=vector.vert[0];
				pseqdescs[j].animbones[i].pos[1]=vector.vert[1];
				pseqdescs[j].animbones[i].pos[2]=vector.vert[2];
				
				pseqdescs[j].animbones[i].pos2[0]=0;
				pseqdescs[j].animbones[i].pos2[1]=0;
				pseqdescs[j].animbones[i].pos2[2]=0;
				free(buffer);
			}
			else if(pos==1)
			{
				
				buffer=malloc(8*pseqdescs[j].numframes);
				unk1=chunk->r_u32();
				chunk->r(buffer,8*pseqdescs[j].numframes);
				test=(Test_t*)buffer;
				for(a=0;a<pseqdescs[j].numframes;a++)
				{
					pseqdescs[j].animbones[i].quaternion[a].x=(float)test[a].x*1/32767;
					pseqdescs[j].animbones[i].quaternion[a].y=(float)test[a].y*1/32767; 
					pseqdescs[j].animbones[i].quaternion[a].z=(float)test[a].z*1/32767; 
					pseqdescs[j].animbones[i].quaternion[a].w=(float)test[a].w*1/32767;
				}
				unk1=chunk->r_u32();
				buf=malloc(3*pseqdescs[j].numframes);
				chunk->r(buf,3*pseqdescs[j].numframes);
				test2=(Test_t2*)buf;
				for(a=0;a<pseqdescs[j].numframes;a++)
				{
					pseqdescs[j].animbones[i].trans[a].translate[0]=test2[a].x;
					pseqdescs[j].animbones[i].trans[a].translate[1]=test2[a].y; 
					pseqdescs[j].animbones[i].trans[a].translate[2]=test2[a].z;
				}
				chunk->r_fvector3(vector2);
				pseqdescs[j].animbones[i].pos2[0]=vector2.vert[0];
				pseqdescs[j].animbones[i].pos2[1]=vector2.vert[1]; 
				pseqdescs[j].animbones[i].pos2[2]=vector2.vert[2]; 

				chunk->r_fvector3(vector);
				pseqdescs[j].animbones[i].pos[0]=vector.vert[0];
				pseqdescs[j].animbones[i].pos[1]=vector.vert[1];
				pseqdescs[j].animbones[i].pos[2]=vector.vert[2]; 
				free(buffer);
				free(buf);
			}
			else if(pos==3)
			{
				
				
				buffer=malloc(8);
				chunk->r(buffer,8);
				test=(Test_t*)buffer;
				for(a=0;a<pseqdescs[j].numframes;a++)
				{
					pseqdescs[j].animbones[i].quaternion[a].x=(float)test->x*1/32767;
					pseqdescs[j].animbones[i].quaternion[a].y=(float)test->y*1/32767; 
					pseqdescs[j].animbones[i].quaternion[a].z=(float)test->z*1/32767; 
					pseqdescs[j].animbones[i].quaternion[a].w=(float)test->w*1/32767;
				}
				unk1=chunk->r_u32();
				buf=malloc(99999);
				chunk->r(buf,3*pseqdescs[j].numframes);
				test2=(Test_t2*)buf;
				
				for(a=0;a<pseqdescs[j].numframes;a++)
				{
					pseqdescs[j].animbones[i].trans[a].translate[0]=test2[a].x;
					pseqdescs[j].animbones[i].trans[a].translate[1]=test2[a].y; 
					pseqdescs[j].animbones[i].trans[a].translate[2]=test2[a].z; 
				}
				
				chunk->r_fvector3(vector2);
				pseqdescs[j].animbones[i].pos2[0]=vector2.vert[0];
				pseqdescs[j].animbones[i].pos2[1]=vector2.vert[1]; 
				pseqdescs[j].animbones[i].pos2[2]=vector2.vert[2]; 
				chunk->r_fvector3(vector);
				pseqdescs[j].animbones[i].pos[0]=vector.vert[0];
				pseqdescs[j].animbones[i].pos[1]=vector.vert[1]; 
				pseqdescs[j].animbones[i].pos[2]=vector.vert[2];
				free(buf);
				free(buffer);
			}
      else printf("!!! unsupported animation type found (0x04%X)\n",pos);
}
}
}

void Model::SaveAnimSMD()
{
	float matrix[3][4];
	float angles[3];
	vec3_t pos;
	vec4_t q;
	
	int j;
	
	char szCurrentDirectory[_MAX_PATH];
	
	for(j=0;j<numseq;j++)
	{
		GetCurrentDirectory(_MAX_PATH, szCurrentDirectory);
		strcat(szCurrentDirectory,"\\smd\\");
		strcat(szCurrentDirectory,pseqdescs[j].label);
		strcat(szCurrentDirectory,".smd");
		FILE *smd=fopen(szCurrentDirectory,"wb");
		
		fprintf(smd,"version 1\n");
		fprintf(smd,"nodes\n");
		for(int i=0;i<model->numbones;i++)
		{
			fprintf(smd,"%3i \"%s\" %i\n",i,pbones[i].name,pbones[i].parent);
		}
		fprintf(smd,"end\n");
		fprintf(smd,"skeleton\n");
		
		for(int a=0;a<pseqdescs[j].numframes;a++)
		{
			fprintf(smd,"time %i\n",a);
			for(int i=0;i<numbones;i++)
			{
				q[0]=pseqdescs[j].animbones[i].quaternion[a].x;
				q[1]=pseqdescs[j].animbones[i].quaternion[a].y; 
				q[2]=pseqdescs[j].animbones[i].quaternion[a].z; 
				q[3]=pseqdescs[j].animbones[i].quaternion[a].w;
				QuaternionMatrix(q,matrix);
				GUtil_GetEulerAngles2a(matrix,angles);
				pos[0]=pseqdescs[j].animbones[i].pos[0]+
					(pseqdescs[j].animbones[i].pos2[0]*pseqdescs[j].animbones[i].trans[a].translate[0]);
				pos[1]=pseqdescs[j].animbones[i].pos[1]+
					(pseqdescs[j].animbones[i].pos2[1]*pseqdescs[j].animbones[i].trans[a].translate[1]);
				pos[2]=pseqdescs[j].animbones[i].pos[2]+
					(pseqdescs[j].animbones[i].pos2[2]*pseqdescs[j].animbones[i].trans[a].translate[2]);
				//fprintf(smd,"%i %f %f %f %f %f %f\n",i,0.0f,
				//0.0f,0.0f,0.0f,0.0f,0.0f);
				fprintf(smd,"%i %f %f %f %f %f %f\n",i,pos[0]*100,pos[2]*100,pos[1]*100,
					angles[0],angles[1],angles[2]);
			}
		}
		fprintf(smd,"end\n");
		fclose(smd);
	}
}

int main(int argc, char *argv[])
{
	int version;
	char fileName[_MAX_PATH];
	
	if (lstrcpy((char*)fileName,argv[1]) == 0) 
	{
		printf("OGF2SMD v3.0a [20100517]: no arguments given.\n");
		printf("usage: ogf2smd meshfile.ext\n");
		printf("meshfile.ext can be: xray ogf v4\n");
		printf("output writed to 'smd\\' directory\n");
		return 1;
	} else
		printf("OGF2SMD v3.0a [20100517]: %s\n",fileName);
	
	GetCurrentDirectory(_MAX_PATH, fileName);
	model =new Model;//Allocate memory;
	IReader *file;
	Core._initialize("OGF2SMD",0,0);//XrCore.dll - initialising, Sorry GSC... :'( ; 
  xr_FS->_initialize(32,fileName);//File System initialising... (xrcore.dll);
	strcat(fileName,"\\");
	strcat(fileName,argv[1]);
	file=xr_FS->r_open(fileName); // Loading model;
	if(!file) 
	{
		Core._destroy(); return 0;
	}

	file->seek(8);
	char ogfver=file->r_u8();
	if(ogfver != 4) 
	{
		printf("!!! Only OGF v4 models supported. :(\n");
		Core._destroy(); return 0;
	}

	if (file->find_chunk(0xD,0))
	{
		printf("S_BONE_NAMES chunk found\n");
		model->numbones=file->r_u32();//Bones Amount
		//model->pbones=new Bones_t[model->numbones];
		SearchForParent(file);
		CalkParent(file);
	}
	else
	{
		printf("S_BONE_NAMES chunk NOT found\n");
		model->numbones = 0;
	}
	model->MeshToSMD(file);//writing *.smd
	printf("MeshToSMD() ... OK\n");
	version=file->find_chunk(0xE,0);
	if(version)
	{
		model->AnimToSMD(file);
  	printf("AnimToSMD() ... OK\n");
    model->SaveAnimSMD();
	} else printf("AnimToSMD() ... NO_DATA\n");
	delete model;
	Core._destroy();
	return 1;
}
