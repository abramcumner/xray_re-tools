const u8 vmtUV = 0;

struct st_VMapPt{
	int vmap_index; // ссылка на мапу
	int index; // индекс в V-мапе на uv/w
};
// uv's
class st_VMap{
	FloatVec vm; // u,v - координаты или weight
public:
	str_shared name; // vertex uv map name
	struct{
		u8 type :2;
		u8 polymap :1;
		u8 dim :2;
		u8 reserved:3;
	};
	IntVec vindices;
	IntVec pindices;
public:
	st_VMap (LPCSTR nm=0, u8 t=vmtUV, bool pm=false)
	{
		type = t;
		polymap = pm;
		name = nm;
		if (t==vmtUV) dim=2; else dim=1;
	}
	Fvector2& getUV (int idx) {VERIFY(type==vmtUV); return (Fvector2&)vm[idx*dim];}
	float& getW (int idx) {VERIFY(type==vmtWeight); return vm[idx];}
	FloatVec& getVM () {return vm;}
	float* getVMdata () {return &*vm.begin();}
	float* getVMdata (int start) {return &*(vm.begin()+start*dim);}
	int VMdatasize () {return vm.size()*sizeof(float);}
	int* getVIdata () {return &*vindices.begin();}
	int VIdatasize () {return vindices.size()*sizeof(int);}
	int* getPIdata () {return &*pindices.begin();}
	int PIdatasize () {return pindices.size()*sizeof(int);}
	int size () {return vm.size()/dim;}
	void resize (int cnt) {vm.resize(cnt*dim);vindices.resize(cnt);if (polymap) pindices.resize(cnt); }
	void appendUV (float u, float v) {vm.push_back(u);vm.push_back(v);}
	void appendUV (Fvector2& uv) {appendUV(uv.x,uv.y);}
	void appendW (float w) {vm.push_back(w);}
	void appendVI (int vi) {vindices.push_back(vi);}
	void appendPI (int pi) {VERIFY(polymap); pindices.push_back(pi);}
	void copyfrom (float* src, int cnt) {resize(cnt); CopyMemory(&*vm.begin(),src,cnt*dim*4);}
};

struct st_VMapPtLst{
	u8 count;
	st_VMapPt* pts;
};
DEFINE_VECTOR (st_VMapPtLst,VMRefsVec,VMRefsIt);
// faces
struct st_FaceVert{
	int pindex; // point index in PointList
	int vmref; // vm-ref index
};
struct st_Face{
	st_FaceVert pv[3]; // face vertices (P->P...)
};



F.open_chunk (EMESH_CHUNK_VERSION);
F.w_u16 (EMESH_CURRENT_VERSION);
F.close_chunk ();

F.open_chunk (EMESH_CHUNK_MESHNAME);
F.w_stringZ (m_Name);
F.close_chunk ();

F.w_chunk (EMESH_CHUNK_BBOX,&m_Box, sizeof(m_Box));
F.w_chunk (EMESH_CHUNK_FLAGS,&m_Flags,1);
F.w_chunk (EMESH_CHUNK_BOP,&m_Ops, sizeof(m_Ops));

F.open_chunk (EMESH_CHUNK_VERTS);
F.w_u32 (m_VertCount);
F.w (m_Verts, m_VertCount*sizeof(Fvector));
/*
for (AdjIt a_it=m_Adjs.begin(); a_it!=m_Adjs.end(); a_it++){
int sz = a_it->size(); VERIFY(sz<=255);
F.w_u8 ((u8)sz);
F.w (&*a_it->begin(), sizeof(int)*sz);
}
*/
F.close_chunk ();

F.open_chunk (EMESH_CHUNK_FACES);
F.w_u32 (m_FaceCount); /* polygon count */
F.w (m_Faces, m_FaceCount*sizeof(st_Face));
F.close_chunk ();

if (m_SGs){
	F.open_chunk (EMESH_CHUNK_SG);
	F.w (m_SGs, m_FaceCount*sizeof(u32));
	F.close_chunk ();
}

F.open_chunk (EMESH_CHUNK_VMREFS);
F.w_u32 (m_VMRefs.size());
for (VMRefsIt r_it=m_VMRefs.begin(); r_it!=m_VMRefs.end(); r_it++){
	int sz = r_it->count; VERIFY(sz<=255);
	F.w_u8 ((u8)sz);
	F.w (r_it->pts, sizeof(st_VMapPt)*sz);
}
F.close_chunk ();

F.open_chunk (EMESH_CHUNK_SFACE);
F.w_u16 ((u16)m_SurfFaces.size()); /* surface polygon count*/
for (SurfFacesPairIt plp_it=m_SurfFaces.begin(); plp_it!=m_SurfFaces.end(); plp_it++){
	F.w_stringZ (plp_it->first->_Name()); /* surface name*/
	IntVec& pol_lst = plp_it->second;
	F.w_u32 (pol_lst.size()); /* surface-polygon indices*/
	F.w (&*pol_lst.begin(), sizeof(int)*pol_lst.size());
}
F.close_chunk ();

F.open_chunk (EMESH_CHUNK_VMAPS_2);
F.w_u32 (m_VMaps.size());
for (VMapIt vm_it=m_VMaps.begin(); vm_it!=m_VMaps.end(); vm_it++){
	F.w_stringZ ((*vm_it)->name);
	F.w_u8 ((*vm_it)->dim);
	F.w_u8 ((u8)(*vm_it)->polymap);
	F.w_u8 ((*vm_it)->type);
	F.w_u32 ((*vm_it)->size());
	F.w ((*vm_it)->getVMdata(), (*vm_it)->VMdatasize());
	F.w ((*vm_it)->getVIdata(), (*vm_it)->VIdatasize());
	if ((*vm_it)->polymap)
		F.w ((*vm_it)->getPIdata(), (*vm_it)->PIdatasize());
}
F.close_chunk ();