//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// FILE:        dummy.h
// DESCRIPTION: 
// AUTHOR:      Dan Silva
// HISTORY:     
//**************************************************************************/

#ifndef __DUMMY__H
#define __DUMMY__H


extern CoreExport Class_ID dummyClassID;

class DummyObject: public HelperObject {			   
	friend class LuminaireObject;
	friend class LuminaireObjectCreateCallBack;
	friend class DummyObjectCreateCallBack;
	friend INT_PTR CALLBACK DummyParamDialogProc( HWND hDlg, UINT message, 
		WPARAM wParam, LPARAM lParam );

	friend class CharacterObject;
	
		// Mesh cache
		Mesh mesh;	
		long dumFlags;	
		Point3 color;
		Box3 box;
		Interval valid;
		void BuildMesh();
		void UpdateMesh();

	//  inherited virtual methods for Reference-management
		RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, 
		   PartID& partID, RefMessage message );

	public:
		CoreExport DummyObject();

		CoreExport Box3 GetBox() const;
		CoreExport void SetBox(Box3& b);
		CoreExport void SetColor(Point3 color);
		CoreExport void EnableDisplay();
		CoreExport void DisableDisplay();
		CoreExport void SetValidity(Interval v);

		//  inherited virtual methods:
		// From BaseObject
		CoreExport int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt);
		CoreExport void Snap(TimeValue t, INode* inode, SnapInfo *snap, IPoint2 *p, ViewExp *vpt);
		CoreExport int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags);
		CoreExport CreateMouseCallBack* GetCreateMouseCallBack();
		CoreExport RefTargetHandle Clone(RemapDir& remap = DefaultRemapDir());
		Interval ObjectValidity(TimeValue t) { return valid; }

		// From Object
		CoreExport ObjectState Eval(TimeValue time);
		void InitNodeName(TSTR& s);
		int DoOwnSelectHilite() {return 1; }		
		int IsRenderable(){ return 0; }
		TCHAR *GetObjectName();

		//  From Object
		CoreExport void GetWorldBoundBox(TimeValue t, INode *mat, ViewExp *vpt, Box3& box );
		CoreExport void GetLocalBoundBox(TimeValue t, INode *mat, ViewExp *vpt, Box3& box );
		CoreExport void GetDeformBBox(TimeValue t, Box3& box, Matrix3 *tm, BOOL useSel=FALSE );


		// IO
		CoreExport IOResult Save(ISave *isave);
		CoreExport IOResult Load(ILoad *iload);

		// From ReferenceMaker
		CoreExport void RescaleWorldUnits(float f);

		// Animatable methods
		CoreExport void DeleteThis();
		Class_ID ClassID() { return dummyClassID; }  
		void GetClassName(TSTR& s);
		int IsKeyable(){ return 1;}
		LRESULT CALLBACK TrackViewWinProc( HWND hwnd,  UINT message, 
	            WPARAM wParam,   LPARAM lParam ){return(0);}
	};

CoreExport ClassDesc* GetDummyObjDescriptor();
#endif // __DUMMY__H
