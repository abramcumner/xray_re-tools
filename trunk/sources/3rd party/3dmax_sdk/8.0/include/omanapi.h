/**********************************************************************
 	FILE: omanapi.h

	DESCRIPTION:  Defines an interface to the osnapmanager class

	CREATED BY: John Hutchinson
	HISTORY: May 14, 1997
	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/
#ifndef _IOMAN_H
#define _IOMAN_H
// This class provides an interface to the OsnapManager. People who implement osnaps
// need to record hits with the manager. People implementing command modes are responsible 
// for getting the snap preview done and may be responsible for initting and closing point
// sequences.
class Osnap;
class OsnapHit;

// Flag masks for user settings
#define SHOW_PREVIEW  			(1<<0)
#define HIGHLIGHT_GEOM  		(1<<1)
#define HIGHLIGHT_POINT  		(1<<2)
#define SHOW_CURSOR					(1<<3)
#define XYZ_CONSTRAINT			(1<<5)
#define SNAP_FROZEN					(1<<15)
#define DISPLAY_RUBBER_BAND	(1<<20)

// Flag masks for internal stuff
#define BUFFER_HOLDING  		(1<<6)

  
#define IID_IOsnapManager Interface_ID(0x5ba68f3, 0x490c28a2)
class IOsnapManager :  public BaseInterface
{
public:
	virtual BOOL getactive() const =0;
	virtual BOOL getAxisConstraint()=0;
	virtual void RecordHit(OsnapHit* somehit)=0;
	virtual BOOL OKForRelativeSnap()=0;
	virtual BOOL RefPointWasSnapped()=0;
	virtual Point3 GetRefPoint(BOOL top = TRUE)=0;
	virtual BOOL IsHolding()=0;
	virtual OsnapHit &GetHit()=0;
	virtual ViewExp* GetVpt()=0;
	virtual INode* GetNode()=0;
	virtual int GetSnapStrength()=0;
	virtual Matrix3 GetObjectTM()=0;
	virtual TimeValue GetTime()=0;
	virtual void wTranspoint(Point3 *inpt, IPoint3 *outpt)=0;
	virtual void Reset() =0;
	virtual BOOL TestAFlag(int mask)=0;
	virtual Point3 GetCurrentPoint()=0;
};

#define IID_IOsnapManager7 Interface_ID(0x795e7eae, 0x48e02d13)
class IOsnapManager7 :  public IOsnapManager
{
public:
	// --- Snap Preview Radius management
	// This is the radius of the circular area around the mouse cursor in which 
	// the snap point has to be in order to register a hit with the Snap Manager.
	// From all the registered hits, the Snap Manager determines the closest to
	// the mouse cursor and asks it to highlight ("preview" itself)
	virtual int GetSnapPreviewRadius() const = 0; 
	virtual void SetSnapPreviewRadius(int nVal) = 0; 

	// --- Snap Radius management
	// This is the radius of the circular area around the mouse cursor in which 
	// the highlighted snap point has to fall into for the transformed entity to 
	// be automatically snapped to that snap point. Note that the entity will 
	// always snap to the highlighted snap point when the user starts or finishes 
	// transforming the entity
	virtual int GetSnapRadius() const = 0; 
	virtual void SetSnapRadius(int nVal) = 0; 

	// --- Rubber-Band Display management methods
	// These methods allow the toggling of the rubber-band display when the user
	// transforms an entity. Note that the rubber-band is not displayed during
	// object creation
	virtual bool GetDisplaySnapRubberBand() const = 0;
	virtual void SetDisplaySnapRubberBand(bool bDisplayRubberBand) = 0;

	// Use Axis Constraint
	virtual void SetAxisConstraint(BOOL bUseAxisConstraint) = 0;

	// Read-Only access to the SnapInfo data. 
	// Returns false if it could not satisfy the request
	virtual bool GetSnapInfo(SnapInfo& si) const = 0;

	// Returns a pointer to the Osnap object that is current override snap object
	// The active snap type of the returned snap object is the exact snap override type.
	// NULL is returned when the snap system is not in override mode.
	virtual const Osnap* GetSnapOverride() const = 0;

	// for MXS exposure
	virtual void SetAFlag(int mask) = 0;
	virtual void ClearAFlag(int mask) = 0;
	virtual void setactive(boolean state) = 0;
	virtual void sethilite(Point3 col) = 0;
	virtual Point3 gethilite() = 0;
	virtual void setmarksize(int val) = 0;
	virtual int getmarksize() = 0;
	virtual BOOL SnapToFrozen() = 0;
	virtual void SetSnapToFrozen(BOOL state = TRUE) = 0;
	virtual Osnap* Enumerate(int FLAGS) const = 0;
	virtual DWORD GetSnapFlags() = 0;

	CoreExport static IOsnapManager7* GetInstance();
};

#define IID_IOsnapManager8 Interface_ID(0x77fa49fa, 0x30c73aa7)
class IOsnapManager8 :  public IOsnapManager7
{
public:
	// --- Snap Start point management
	virtual bool IsTMAxisCenterTheDefaultStartSnapPoint() const = 0; 
	virtual void SetTMAxisCenterAsDefaultStartSnapPoint(bool bEnable) = 0; 

	CoreExport static IOsnapManager8* GetInstance();
};


#endif// _IOMAN_H