/**********************************************************************
 *<
	FILE: IProjectionMod.h

	DESCRIPTION: Interface for Projection Modifier

	CREATED BY:  Michael Russo

	HISTORY:     02-23-2004

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/

#ifndef __IPROJECTIONMOD__H
#define __IPROJECTIONMOD__H

#include "maxheap.h"
#include "ObjectWrapper.h"

#define PROJECTIONMOD_CLASS_ID			Class_ID(0x166c301c, 0x19f90c49)
#define IPROJECTIONMOD_INTERFACE_ID		Interface_ID(0x6b231b96, 0xb26a72)
#define IPROJECTIONMODDATA_INTERFACE_ID	Interface_ID(0x341e47bc, 0x1f923a2f)

class IProjectionModData;

//=============================================================================
//
//	Class IProjectionMod
//
//=============================================================================
class IProjectionMod : public FPMixinInterface
{
public:
	// IProjectionMod

	// Geometry Selection Methods
	enum { PM_SL_OBJECT, PM_SL_VERT, PM_SL_FACE, PM_SL_ELEM };

	// Access to Geom Selections by Node/Object Index
	virtual int					NumObjects() = 0;
	virtual INode*				GetObjectNode(int iIndex) = 0;
	virtual TCHAR*				GetObjectName(int iIndex) = 0;
	virtual int					GetObjectSelLevel(int iIndex) = 0;
	virtual void				DeleteObjectNode(int iIndex) = 0;

	// Access to Geom Selections by Geom Selections
	virtual int					NumGeomSels() = 0;
	virtual TCHAR*				GetGeomSelName( int iSelIndex ) = 0;
	virtual int					GetGeomSelSelLevel( int iSelIndex ) = 0;
	virtual int					NumGeomSelNodes( int iSelIndex ) = 0;
	virtual INode*				GetGeomSelNode( int iSelIndex, int iNodeIndex ) = 0;
	virtual void				SetGeomSelMapProportion( int iSelIndex, float fProportion ) = 0;
	virtual float				GetGeomSelMapProportion( int iSelIndex ) = 0;
	virtual void				DeleteGeomSel( int iSelIndex ) = 0;
	virtual void				DeleteGeomSelNode( int iSelIndex, int iNodeIndex ) = 0;

	virtual IProjectionModData*	GetProjectionModData(INode *pNode) = 0;

	// Modifier Access
	virtual bool				IsValidObject( INode *pNode ) = 0;
	virtual bool				AddObjectNode( INode *pNode ) = 0;
	virtual void				DeleteAll() = 0;
	virtual void				SetGeomSelNodesVisibility( bool bVisible ) = 0;
	virtual bool				GetGeomSelNodesVisibility() = 0;

	virtual void				UpdateProjectionTypeList() = 0;
	virtual void				AutoWrapCage() = 0;
	virtual void				ResetCage() = 0;
	virtual void				PushCage(float fAmount) = 0;

	virtual int					fpGetObjectSelMtlIDs(INode *pNode, int iIndex, Tab<int> &mtlIDs) = 0;
	virtual int					fpGetGeomSelMtlIDs(INode *pNode, int iSelIndex, Tab<int> &mtlIDs) = 0;
	virtual float				fpGetGeomSelFaceArea(INode *pNode, int iSelIndex) = 0;
	virtual	int					fpCheckDuplicateMatIDs( INode *pNode, BitArray &bitFaceDups ) = 0;
	virtual	int					fpCheckDuplicateSels( INode *pNode, BitArray &bitFaceDups ) = 0;

	virtual void				fpSelectByMtlID( int iMtlID ) = 0;
	virtual void				fpSelectBySG( int iSG ) = 0;

	virtual int					fpGetNumProjectionTypes() = 0;
	virtual ReferenceTarget*	fpGetProjectionType(int iIndex) = 0;
	virtual void				fpRemoveProjectionType(int iIndex) = 0;
	virtual void				fpProject(int iIndex) = 0;
	virtual void				fpProjectAll() = 0;
	virtual int					fpGetNumRegisteredProjectionTypes() = 0;
	virtual void				fpAddRegisteredProjectionType(int iIndex) = 0;
	virtual void				fpGetRegisteredProjectionTypeClassID(int iIndex, Tab<int> &classid) = 0;
	virtual void				fpShowAlignment() = 0;
	virtual void				fpClearAlignment() = 0;
};

//=============================================================================
//
//	Class IProjectionModData
//
//=============================================================================
class IProjectionModData : public BaseInterface
{
public:

	// GenericInterface
	virtual Interface_ID	GetID() { return IPROJECTIONMODDATA_INTERFACE_ID; }

	// IProjectionModData
	virtual ObjectWrapper &GetBaseObject() = 0;	// Object with Projection Modifier
	virtual ObjectWrapper &GetCage() = 0;	    // Cage mesh

	// Access to Geom Selections by Node/Object Index
	virtual bool		GetObjectSel(int iIndex, BitArray& sel) = 0;
	virtual int			GetObjectSelMtlIDs(int iIndex, Tab<int> &mtlIDs) = 0;

	// Access to Geom Selections by Geom Selections
	virtual bool		GetGeomSel(int iSelIndex, BitArray& sel) = 0;
	virtual int			GetGeomSelMtlIDs(int iSelIndex, Tab<int> &mtlIDs) = 0;
	virtual float		GetGeomSelFaceArea(int iSelIndex) = 0;

	// Returns number of material ids that conflict within SO Geometry selections.
	// bitFaceDups is a bitarray of the faces containing the conflict.
	virtual	int			CheckDuplicateMatIDs( BitArray &bitFaceDups ) = 0;
	// Returns number of face selections conflict within SO Geometry selections.
	// bitFaceDups is a bitarray of the faces containing the conflict.
	virtual	int			CheckDuplicateSels( BitArray &bitFaceDups ) = 0;

};


//=============================================================================
//
//	Class IProjectionModType
//
//	Class Category: ProjectionModType
//
//=============================================================================
class IProjectionModType : public ReferenceTarget
{
public:

	virtual void SetIProjectionMod( IProjectionMod *pPMod ) {}

	virtual bool CanProject(Tab<INode*> &tabSourceNodes, int iSelIndex, int iNodeIndex) { return false; }
	virtual void Project(Tab<INode*> &tabSourceNodes, int iSelIndex, int iNodeIndex) {}

	virtual void SetInitialName(int iIndex) {}
	virtual TCHAR *GetName(){return NULL;}
	virtual void SetName(TCHAR *name) {}

	virtual void Enable(bool enable) {}
	virtual bool IsEditing() { return false; }
	virtual void EndEditing() {}
	virtual void EnableEditing(bool enable) {}

	virtual RefResult NotifyRefChanged( Interval changeInt,RefTargetHandle hTarget, PartID& partID, RefMessage message )	
		{ return REF_SUCCEED; }

    virtual IOResult Load(ILoad *iload) {return IO_OK;}
    virtual IOResult Save(ISave *isave) {return IO_OK;}

	virtual void ModifyObject(TimeValue t, ModContext &mc, ObjectState * os, INode *inode, IProjectionModData *pPModData) {}

    virtual void BeginEditParams(IObjParam  *ip, ULONG flags,Animatable *prev) {}
    virtual void EndEditParams(IObjParam *ip,ULONG flags,Animatable *next) {}         

	virtual void NotifyInputChanged(Interval changeInt, PartID partID, RefMessage message, ModContext *mc, IProjectionModData *pPModData) {}

	virtual void GetWorldBoundBox(TimeValue t,INode* inode, ViewExp *vpt, Box3& box, ModContext *mc){}               
	virtual int Display(TimeValue t, GraphicsWindow *gw, Matrix3 tm ) { return 1;}

    virtual int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt, ModContext* mc, Matrix3 tm) {return 0;}
    virtual void SelectSubComponent(HitRecord *hitRec, BOOL selected, BOOL all, BOOL invert=FALSE) {}
    virtual void Move( TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, Matrix3 tm, BOOL localOrigin=FALSE ) {}
    virtual void GetSubObjectCenters(SubObjAxisCallback *cb,TimeValue t,INode *node, Matrix3 tm) {}
    virtual void GetSubObjectTMs(SubObjAxisCallback *cb,TimeValue t,INode *node, Matrix3 tm) {}
    virtual void ClearSelection(int selLevel) {}
	virtual void SelectAll(int selLevel) {}
    virtual void InvertSelection(int selLevel) {}
	
};

//=============================================================================
//
//	Class IProjectionIntersector
//
//	Ray-intersection for projection mapping
//
//=============================================================================
class IProjectionIntersector: public MaxHeapOperators {
	public:
		IProjectionIntersector() {}
		virtual			~IProjectionIntersector() {}
		virtual	void	DeleteThis() {}

		// The root is normally the object on which the projection modifier is applied.
		// The cage is optional, it may be NULL or empty;
		// if supplied, the cage must have the same topology as the root.
		virtual	BOOL	InitRoot( MNMesh* root,			MNMesh* cage,			Matrix3& objectToWorldTM ) = 0;
		virtual	BOOL	InitRoot( ObjectWrapper& root,	ObjectWrapper& cage,	Matrix3& objectToWorldTM ) = 0;
		virtual BOOL	RootValid() = 0; //true if initialized, false when free'd

		// A leaf is normally one of the objects targeted by the projection modifier.
		virtual BOOL	InitLeaf( MNMesh* leaf,			Matrix3& objectToWorldTM ) = 0;
		virtual BOOL	InitLeaf( ObjectWrapper& leaf,	Matrix3& objectToWorldTM ) = 0;
		virtual BOOL	LeafValid() = 0; //true if initialized, false when free'd

		// releases memory allocated by the Init functions
		virtual void	Free() = 0;

		// --- Root functions: Require the root be initialized first
		//Given a point in root object space, and a normal for the point (optional - may be zero),
		//find the closest face, and barycentric coordinates of the point as projected onto the face
		enum {FINDCLOSEST_CAGED=1, FINDCLOSEST_IGNOREBACKFACING=2};
		virtual BOOL	RootClosestFace( Ray& pointAndNormal, int flags, float& dist, DWORD& rootFaceIndex, Tab<float>& rootFaceBary ) = 0;

		// --- Leaf functions: Require a leaf be initialized first
		//Find an intersection on a leaf, given a ray
		virtual BOOL	LeafIntersectRay( Ray& ray, float& dist, DWORD& leafFaceIndex, Tab<float>& leafFaceBary ) = 0;

		// --- General functions:  Require that BOTH the root and a leaf are initialized
		//Find an intersection on a leaf, given a point on the root mesh
		virtual BOOL	LeafIntersectRay( DWORD rootFaceIndex, Point3& rootFaceBary, float& dist, DWORD& leafFaceIndex, Tab<float>& leafFaceBary ) = 0;
};

// Manager class, creates instances of IProjectionIntersector
class IProjectionIntersectorMgr : public FPStaticInterface {
	public:
		virtual IProjectionIntersector* CreateProjectionIntersector() = 0;
};

#define IPROJECTIONINTERSECTORMGR_INTERFACE_ID Interface_ID(0x457d402d, 0x36e81f43)
#define GetIProjectionIntersectorMgr() \
	((IProjectionIntersectorMgr*)GetCOREInterface(IPROJECTIONINTERSECTORMGR_INTERFACE_ID))


#endif	// __IPROJECTIONMOD__H

