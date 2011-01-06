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
// FILE:        osnaphit.h
// DESCRIPTION: Defines the classes which are passed from the osnaps 
//              to the manager.
// AUTHOR:      John Hutchinson
// HISTORY:     December 9, 1996
//**************************************************************************/
#ifndef _OSNAPHIT_H_
#define _OSNAPHIT_H_

#include "maxheap.h"
#include "baseinterface.h"

class Osnap;
class HitMesh;
class OsnapMarker;

class OsnapHit : public BaseInterfaceServer
{
	friend class OsnapManager;
	friend class Osnap;
	friend class TypedHit;
public:
	CoreExport OsnapHit(Point3 p3, Osnap* s, int sub, HitMesh *m);
	CoreExport OsnapHit(Point3 pt);
	CoreExport OsnapHit(const OsnapHit& h);
	CoreExport virtual OsnapHit& operator=(const OsnapHit& h);
	CoreExport virtual ~OsnapHit();
	CoreExport virtual OsnapHit* clone();

	void setscreendata(IPoint3 screen3, int len);

	//////////////////////////////////////////////////////////////////////
	// Display Methods
	//////////////////////////////////////////////////////////////////////
	CoreExport virtual boolean display(ViewExp *vpt, TimeValue t, Point3 color,  \
		int markersize, boolean markers = TRUE, boolean hilite = TRUE);
	CoreExport void erase(ViewExp *vpt, TimeValue t) const; // the hit can erase itself
	CoreExport void GetViewportRect(TimeValue t,ViewExp *vpt,Rect *rect, int marksize)const;

	//////////////////////////////////////////////////////////////////////
	// Accessor Methods
	//////////////////////////////////////////////////////////////////////
	CoreExport Point3 GetHitpoint(){return hitpoint;};
	CoreExport Point3 GetWorldHitpoint(){return worldpoint;};
	CoreExport IPoint3 GetHitscreen(){return m_hitscreen;};
	CoreExport int GetSubsnap(){return subsnap;}
	CoreExport POINT GetCursor(){return m_cursor;}
	INode *GetNode(){return node;}
	void Dump()const;

	//////////////////////////////////////////////////////////////////////
	// Operator Methods
	//////////////////////////////////////////////////////////////////////
	//define comparators so we can sort a list of these
	CoreExport BOOL operator<(OsnapHit& hit);
	CoreExport BOOL operator>(OsnapHit& hit);

public:
	void Update(TimeValue t);
	CoreExport Point3 ReEvaluate(TimeValue t);
	virtual bool IsWorldSpaceHit(){return false;}


private: //data
	Point3 hitpoint; // the hit location in object space
	Point3 worldpoint; // the hit location in world space
	IPoint3 m_hitscreen; // the hit location in screen space
	POINT m_cursor;//the position of the cursor when this guy was recorded
	int m_len; //The distace from the cursor
	int m_z_depth; //The depth in z space
	BOOL m_complete; //indicates whether the screendata has been set
	Osnap*  snap; //the snap which made this hit
	int subsnap; //the subsnap index that made this hit
	OsnapMarker *m_pmarker; //a pointer to this snaps marker

	INode* node;// the node which got hit
	HitMesh *hitmesh;//a mesh used to hilite the topolgy we hit

	ViewExp *m_vpt;//the viewport which was active

	BOOL m_invisible;//this guy won't display itself
};

//a class to hold a list of object space points for highlighting the geometry
class HitMesh: public MaxHeapOperators 
{
private:
	int m_numverts;
	Point3 *m_pverts;
	static long m_cref;
public:
	CoreExport HitMesh();
	CoreExport ~HitMesh();
	CoreExport HitMesh(const HitMesh& h);
	CoreExport HitMesh(int n);
	Point3 operator[](int i){return m_pverts[i];}
	int getNumVerts(){return m_numverts;}
 	CoreExport void setNumVerts(int n);     
	void setVert(int i, const Point3 &xyz){ m_pverts[i] = xyz; }	
	Point3& getVert(int i){return m_pverts[i];}
	Point3* getVertPtr(){return m_pverts;}
};


class TypedHit: public OsnapHit
{
public:
	TypedHit(Point3 pt);
	virtual boolean display(ViewExp *vpt, TimeValue t, Point3 color,  \
		int markersize, boolean markers = TRUE, boolean hilite = TRUE);
	virtual bool IsWorldSpaceHit(){return true;}
};
#endif //_OSNAPHIT_H