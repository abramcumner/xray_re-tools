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
// FILE:        maxtypes.h
// DESCRIPTION: Typedefs for general jaguar types.
// AUTHOR:      Rolf Berteig
// HISTORY:     created 19 November 1994
//**************************************************************************/

#ifndef __JAGTYPES__
#define __JAGTYPES__

#include "maxheap.h"
#include <wtypes.h>
#include <basetsd.h>

typedef unsigned long ulong;
typedef unsigned char uchar;
typedef uchar UBYTE;
typedef unsigned short USHORT;
typedef unsigned short UWORD;
typedef __int64        INT64;
														 
struct Color24: public MaxHeapOperators { 
	Color24(uchar vr = 0, uchar vg = 0, uchar vb = 0): r(vr), g(vg), b(vb) {}
	uchar r,g,b;
	};

struct Color48: public MaxHeapOperators { 
	Color48(UWORD vr = 0, UWORD vg = 0, UWORD vb = 0): r(vr), g(vg), b(vb) {}
	UWORD r,g,b;
	};

struct Color64: public MaxHeapOperators { 
	Color64(UWORD vr = 0, UWORD vg = 0, UWORD vb = 0, UWORD va = 0): r(vr), g(vg), b(vb), a(va) {}
	UWORD r,g,b,a;
	};

//-- Pixel storage classes used by BitmapManager ----------------------------------------------------

struct BMM_Color_24: public MaxHeapOperators {
   BMM_Color_24(BYTE vr = 0, BYTE vg = 0, BYTE vb = 0): r(vr), g(vg), b(vb) {}
   BYTE r,g,b;
};

struct BMM_Color_32: public MaxHeapOperators {
   BMM_Color_32(BYTE vr = 0, BYTE vg = 0, BYTE vb = 0, BYTE va = 0): r(vr), g(vg), b(vb), a(va) {}
   BYTE r,g,b,a;
};

struct BMM_Color_48: public MaxHeapOperators {
   BMM_Color_48(WORD vr = 0, WORD vg = 0, WORD vb = 0): r(vr), g(vg), b(vb) {}
   WORD r,g,b;
};

struct BMM_Color_64: public MaxHeapOperators {
   BMM_Color_64(WORD vr = 0, WORD vg = 0, WORD vb = 0, WORD va = 0): r(vr), g(vg), b(vb), a(va) {}
   WORD r,g,b,a;
};

struct BMM_Color_fl: public MaxHeapOperators {
   BMM_Color_fl(float vr = 0.0f, float vg = 0.0f, float vb = 0.0f, float va = 0.0f): r(vr), g(vg), b(vb), a(va) {}
     
   float r,g,b,a;

   operator float*() { return &r; }
   operator const float*() const { return &r; }

   static WORD clipColor(float c)
      { return c <= 0.0f ? 0 : c >= 1.0f ? 65535 : int(c * 65535.0); }
};

/* Time:
 */
typedef int TimeValue;

#define TIME_TICKSPERSEC	4800

#define TicksToSec( ticks ) ((float)(ticks)/(float)TIME_TICKSPERSEC)
#define SecToTicks( secs ) ((TimeValue)(secs*TIME_TICKSPERSEC))
#define TicksSecToTime( ticks, secs ) ( (TimeValue)(ticks)+SecToTicks(secs) )
#define TimeToTicksSec( time, ticks, secs ) { (ticks) = (time)%TIME_TICKSPERSEC; (secs) = (time)/TIME_TICKSPERSEC ; } 

#define TIME_PosInfinity	TimeValue(0x7fffffff)
#define TIME_NegInfinity 	TimeValue(0x80000000)


//-----------------------------------------------------
// Class_ID
//-----------------------------------------------------
class Class_ID: public MaxHeapOperators {
	ULONG a,b;
	public:
		Class_ID() { a = b = 0xffffffff; }
		Class_ID(const Class_ID& cid) { a = cid.a; b = cid.b;	}
		Class_ID(ulong aa, ulong bb) { a = aa; b = bb; }
		ULONG PartA() const { return a; }
		ULONG PartB() const { return b; }
		void SetPartA( ulong aa ) { a = aa; } //-- Added 11/21/96 GG
		void SetPartB( ulong bb ) { b = bb; }
		int operator==(const Class_ID& cid) const { return (a==cid.a&&b==cid.b); }
		int operator!=(const Class_ID& cid) const { return (a!=cid.a||b!=cid.b); }
		Class_ID& operator=(const Class_ID& cid)  { a=cid.a; b = cid.b; return (*this); }
		// less operator - allows for ordering Class_IDs (used by stl maps for example) 
		bool operator<(const Class_ID& rhs) const
		{
			if ( a < rhs.a || ( a == rhs.a && b < rhs.b ) )
				return true;

			return false;
		}
	};

// SuperClass ID
typedef ULONG SClass_ID;  	// SR NOTE64: was ULONG_PTR, which was WRONG.

// new ID for interfaces  R4 JBW 2.16.00
class Interface_ID: public MaxHeapOperators {
	ULONG a,b;
	public:
		Interface_ID() { a = b = 0xffffffff; }
		Interface_ID(const Interface_ID& iid) { a = iid.a; b = iid.b;	}
		Interface_ID(ulong aa, ulong bb) { a = aa; b = bb; }
		ULONG PartA() { return a; }
		ULONG PartB() { return b; }
		void SetPartA( ulong aa ) { a = aa; }
		void SetPartB( ulong bb ) { b = bb; }
		int operator==(const Interface_ID& iid) const { return (a==iid.a&&b==iid.b); }
		int operator!=(const Interface_ID& iid) const { return (a!=iid.a||b!=iid.b); }
		Interface_ID& operator=(const Interface_ID& iid)  { a=iid.a; b = iid.b; return (*this); }
		// less operator - allows for ordering Class_IDs (used by stl maps for example) 
		bool operator<(const Interface_ID& rhs) const
		{
			if ( a < rhs.a || ( a == rhs.a && b < rhs.b ) )
				return true;

			return false;
		}
	};


// Types used by ISave, ILoad, AppSave, AppLoad 
typedef enum {IO_OK=0, IO_END=1, IO_ERROR=2} IOResult; 
typedef enum {NEW_CHUNK=0, CONTAINER_CHUNK=1, DATA_CHUNK=2} ChunkType;
typedef enum {IOTYPE_MAX=0, IOTYPE_MATLIB=1, IOTYPE_RENDER_PRESETS=2} FileIOType; 


// Lock / licensing types  000817  --prs.

enum ProductVersionType { PRODUCT_VERSION_DEVEL, PRODUCT_VERSION_TRIAL,
						  PRODUCT_VERSION_ORDINARY, PRODUCT_VERSION_EDU,
						  PRODUCT_VERSION_NFR };

enum LockBehaviorType { LICENSE_BEHAVIOR_PERMANENT, LICENSE_BEHAVIOR_EXTENDABLE,
                        LICENSE_BEHAVIOR_NONEXTENDABLE, LICENSE_BEHAVIOR_BORROWED };



#endif // __JAGTYPES__

