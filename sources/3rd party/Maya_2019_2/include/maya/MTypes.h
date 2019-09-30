#ifndef _MTypes
#define _MTypes
//-
// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
// ****************************************************************************
//
// FILE:    MTypes.h
//
// ****************************************************************************
//
// DESCRIPTION (MTypes)
//
//		This file contains the definitions for NULL, numeric array types, type 
//		bool, and constants true and false
//
// ****************************************************************************

#if defined __cplusplus

#include <maya/MApiNamespace.h>

// ****************************************************************************


// ****************************************************************************

// DECLARATIONS

// ****************************************************************************

// Define a CPP macro that reflects a numeric representation of the
// base application version. For example
//	Maya 1.0.1	would be 10
//	Maya	1.5	would be 15
//	Maya 2.0	would be 20
//	...
//	Maya 2017	would be 2017
//	Maya 2018	would be 2018
//
#define MAYA_APP_VERSION 2019

// Define a CPP macro that reflects the current Maya API version.
// This macro is designed to be an integer so that one can do
// CPP arithmetic and comparisons on it.  The digits in the number
// are derived by taking the Maya version number and deleting the '.'
// characters. Dot releases do not update this value so a release with
// version #.#.# would havethe same number as #.#. Thus, for example
//   Maya 1.0.1  would be 100
//   Maya 1.5    would be 150
//   Maya 2.0    would be 200
//   Maya 2.5    would be 250
//   Maya 3.0    would be 300
//   Maya 4.0    would be 400
//   Maya 4.0.1  would be 400
//   Maya 4.0.2  would be 400
//   Maya 4.0.3  would be 400
//   Maya 4.5    would be 450
//   Maya 5.0    would be 500
//   Maya 6.0    would be 600
//   Maya 6.5    would be 650
//   Maya 7.0    would be 700
//   Maya 8.0    would be 800
//   ...
//   Maya 2017   would be 201700
//   Maya 2018   would be 20180000 --> Namespacing the API
//   Maya 2019   would be 201900
//
// since this variable did not exist in the Maya 1.0 API, it will
// default to zero in that release.  Thus a construct such as
//
//    #if MAYA_API_VERSION > 100
//
// will be true in all post 1.0 versions of the API.
//
// API version for to match previous gold API version
//
#define MAYA_API_VERSION 20190200

// Define a CPP macro that reflects the current Maya custom version.
// This macro is designed to be an integer so that one can do CPP arithmetic
// and comparisons on it. The digits in the number are derived by the following
// formula:
//
// MAYA_APP_VERSION * 10000 + MAJOR_VERSION * 100 + MINOR_VERSION
//
// This macro is only relevant for custom builds of Maya.
//
#define MAYA_CUSTOM_VERSION_MAJOR 0
#define MAYA_CUSTOM_VERSION_MINOR 0
#define MAYA_CUSTOM_VERSION_CLIENT ""
#define MAYA_CUSTOM_VERSION (MAYA_APP_VERSION * 10000 + MAYA_CUSTOM_VERSION_MAJOR * 100 + MAYA_CUSTOM_VERSION_MINOR)

#ifdef _WIN32

#ifndef PLUGIN_EXPORT
#define PLUGIN_EXPORT _declspec( dllexport ) 
#endif // PLUGIN_EXPORT

#ifndef FND_EXPORT
#define FND_EXPORT _declspec( dllimport ) 
#endif // FND_EXPORT

#ifndef IMAGE_EXPORT
#define IMAGE_EXPORT _declspec( dllimport )
#endif // IMAGE_EXPORT

#ifndef OPENMAYA_EXPORT
#define OPENMAYA_EXPORT _declspec( dllimport )
#endif // OPENMAYA_EXPORT

#ifndef OPENMAYAUI_EXPORT
#define OPENMAYAUI_EXPORT _declspec( dllimport )
#endif // OPENMAYAUI_EXPORT

#ifndef OPENMAYAANIM_EXPORT
#define OPENMAYAANIM_EXPORT _declspec( dllimport )
#endif // OPENMAYAANIM_EXPORT

#ifndef OPENMAYAFX_EXPORT
#define OPENMAYAFX_EXPORT _declspec( dllimport )
#endif // OPENMAYAFX_EXPORT

#ifndef OPENMAYARENDER_EXPORT
#define OPENMAYARENDER_EXPORT _declspec( dllimport )
#endif // OPENMAYARENDER_EXPORT

// Typedefs and functions definitions to avoid including windows.h
typedef unsigned short u_short;
#ifndef	STRICT
	#define STRICT 1
#endif

typedef void*			HANDLE;
typedef unsigned long	DWORD;
typedef unsigned int	UINT;
typedef const char*		LPCSTR;

#define OPENMAYA_DECLARE_HANDLE(name) struct name##__; typedef struct name##__ *name

OPENMAYA_DECLARE_HANDLE(HINSTANCE);
OPENMAYA_DECLARE_HANDLE(HWND);
OPENMAYA_DECLARE_HANDLE(HDC);
OPENMAYA_DECLARE_HANDLE(HGLRC);

#if defined(NT_PLUGIN) || defined(NT_APP)
extern "C" {
	__declspec(dllimport) void	__stdcall Sleep( DWORD dwMilliseconds );
	__declspec(dllimport) DWORD	__stdcall GetTickCount( void );

	#if !defined(OutputDebugString)
		__declspec(dllimport) void	__stdcall OutputDebugStringA( LPCSTR lpOutputString );
		inline void	OutputDebugString( LPCSTR lpOutputString )
		{
			OutputDebugStringA(lpOutputString);
		}
	#endif
}

// In the case where we really need to include windows.h, let's make sure we are not including too much.
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOSOUND
#define NOCOMM
#define NOCRYPT
#ifndef NOMINMAX
	#define NOMINMAX
#endif

#ifndef _BOOL
	#define _BOOL
#endif

#ifndef NT_APP
	extern HINSTANCE MhInstPlugin;
#endif

#define uint UINT

#ifndef strcasecmp
	#define	strcasecmp _strcmpi
#endif

#ifndef M_PI
#define	M_PI		3.14159265358979323846	/* pi */
#endif

#if _MSC_VER < 1900
    #define		isnan		_isnan
    #define		isnanf		_isnanf
    inline int round(const float x)
    {
        return (x > 0.0f) ? int(x + 0.5f) : -int(-x + 0.5f);
    }
#endif
#if !defined(fsqrt)
#define		fsqrt(x)	sqrtf(x)
#endif
#if !defined(fsin)
#define		fsin(x)		sinf(x)
#endif
#if !defined(fcos)
#define		fcos(x)		cosf(x)
#endif
#if !defined(ftan)
#define		ftan(x)		tanf(x)
#endif
#if !defined(ffloor)
#define		ffloor(x)	floorf(x)
#endif
#if !defined(fatan2)
#define		fatan2(y,x)	atan2f(y,x)
#endif
#if !defined(fatan)
#define		fatan(x)	atanf(x)
#endif
#if !defined(flog)
#define		flog(x)		logf(x)
#endif
#if !defined(fexp)
#define		fexp(x)		expf(x)
#endif
#if !defined(fexpm1)
#define		fexpm1(x)	expm1f(x)
#endif
#if !defined(ftrunc)
#define		ftrunc(d)	truncf(d)
#endif
#if !defined(facos)
#define		facos(d)	acosf(d)
#endif

#endif // NT_PLUGIN

#else // !_WIN32

#if defined (OSMac_) && defined (__MWERKS__) && !defined (M_PI)
#define	M_PI		3.14159265358979323846	/* pi */
#endif

#ifndef DLL_PUBLIC
# define DLL_PUBLIC	__attribute__ ((visibility("default")))
# define DLL_PRIVATE __attribute__ ((visibility("hidden")))
#endif

#ifndef PLUGIN_EXPORT
#define PLUGIN_EXPORT DLL_PUBLIC
#endif
#ifndef FND_EXPORT 
#define FND_EXPORT  DLL_PUBLIC
#endif
#ifndef IMAGE_EXPORT
#define IMAGE_EXPORT DLL_PUBLIC
#endif
#ifndef OPENMAYA_EXPORT
#define OPENMAYA_EXPORT  DLL_PUBLIC
#endif
#ifndef OPENMAYAUI_EXPORT
#define OPENMAYAUI_EXPORT DLL_PUBLIC
#endif
#ifndef OPENMAYAANIM_EXPORT
#define OPENMAYAANIM_EXPORT DLL_PUBLIC
#endif
#ifndef OPENMAYAFX_EXPORT
#define OPENMAYAFX_EXPORT DLL_PUBLIC
#endif
#ifndef OPENMAYARENDER_EXPORT
#define OPENMAYARENDER_EXPORT DLL_PUBLIC
#endif

#if !defined (MAC_PLUGIN)
#include <sys/types.h>
#endif // MAC_PLUGIN
#include <sys/stat.h>

#endif // _WIN32

#if (defined (NT_PLUGIN) || defined(NT_APP) || (defined(MAC_PLUGIN) && !defined(__MACH__)))
#ifdef __cplusplus
extern "C" {
#endif

//from /usr/include/math.h
FND_EXPORT extern double	drand48(void);
FND_EXPORT extern double	erand48(unsigned short [3]);
FND_EXPORT extern long		lrand48(void);
FND_EXPORT extern long		nrand48(unsigned short [3]);
FND_EXPORT extern long		mrand48(void);
FND_EXPORT extern long		jrand48(unsigned short [3]);
FND_EXPORT extern void	srand48(long);
FND_EXPORT extern unsigned short * seed48(unsigned short int [3]);
FND_EXPORT extern void	lcong48(unsigned short int [7]);
FND_EXPORT extern long	 random(void);
FND_EXPORT extern int	srandom( unsigned x );

#ifdef __cplusplus
}
#endif
#endif // NT_PLUGIN || MAC_PLUGIN

#ifndef NULL
#	define NULL	0L
#endif 

typedef signed short short2[2];
typedef signed short short3[3];
typedef signed int   long2[2];
typedef signed int   long3[3];
typedef signed int   int2[2];
typedef signed int   int3[3];
typedef float        float2[2];
typedef float        float3[3];
typedef double       double2[2];
typedef double       double3[3];
typedef double       double4[4];

#define MBits64_

#if defined(_WIN32)
	typedef unsigned long long MUint64;
#elif defined(linux) || defined(__linux__)
	typedef unsigned long MUint64;
#elif defined(OSMac_)
	typedef unsigned long long MUint64;
#else
	Unknown OS: need to provide an implementation here
#endif

#if defined(_WIN32)
	typedef __int64 MInt64;
#elif defined(linux) || defined(__linux__)
	typedef long MInt64;
#elif defined(OSMac_)
	typedef long long MInt64;
#else
	Unknown OS: need to provide an implementation here
#endif

#if defined(_WIN32)
#	include <basetsd.h>
	typedef INT_PTR		MIntPtrSz;
	typedef UINT_PTR	MUintPtrSz;
#elif defined(__unix) || defined(__MACH__)
#	include <inttypes.h>
// On Linux stdarg.h is not included in inttypes.h, so do it here instead
#if defined(linux)  || defined(__linux__)
#include <stdarg.h>
#endif
	typedef intptr_t		MIntPtrSz;
	typedef uintptr_t		MUintPtrSz;
#else
#	error	"__FILE__:__LINE__ No pointer-as-int types defined for this O/S"
#endif

//! \brief Pointer to a creator function.
/*!
 \return
 	An opaque pointer to the created object. The type of the object
	depends upon where the function is being used.
*/
typedef void *   (*MCreatorFunction)();

//! \brief Pointer to a syntax creation function.
/*!
 \return The syntax object created.
*/
typedef MSyntax	 (*MCreateSyntaxFunction)(); 

//! \brief Pointer to an initialization function.
/*!
 \return Status of the function call.
*/
typedef MStatus  (*MInitializeFunction)();


//! \brief Pointer to a function which creates a custom transformation matrix.
/*!
 \return Pointer to the created transformation matrix.
*/
typedef MPxTransformationMatrix*   (*MCreateXformMatrixFunction)();

//! \brief Pointer to a function which creates a cache configuration rule filter.
/*!
\param[in] param    the initialization parameter for the created filter.
\return The filter created.
*/
typedef MPxCacheConfigRuleFilter* (*MCreateCacheConfigRuleFilterFunction)( const MString &param ); 

#define BEGIN_NO_SCRIPT_SUPPORT public
#define END_NO_SCRIPT_SUPPORT public

OPENMAYA_MAJOR_NAMESPACE_OPEN

//! Space transformation identifiers
/*!
  MSpace encompasses all of the types of transformation possible. The
  MSpace identifiers are used to determine the space in which the user
  is applying or querying transformation or component (i.e. vertex
  positions) data of a Maya object.

  Note that not all the MSpace types can be passed into all methods
  which take an MSpace as a parameter.  The MSpace enumerated type can
  be split into two parts, types which can be passed into MFnTransform
  and MTransformationMatrix classes (kTransform, kPreTransform and
  kPostTransform) and types which can be passed into shape classes
  such as MFnMesh, MFnNurbsSurface and MFnCamera (kWorld and kObject).

  The following is a description of each of the MSpace types.

  <dl>
  <dt><b>kInvalid</b></dt>

  <dd>There are currently no methods in the API that return a result
  of type MSpace. This may be used for user defined methods that
  return a result of type MSpace to signify an invalid result.</dd>

  <dt><b>kTransform</b></dt>

  <dd>The data applied or queried using the kTransform type represents a
  transform's local (or relative) coordinates system. This type is
  valid for methods of MFnTransform and MTransformationMatrix classes.</dd>

  <dt><b>kPreTransform</b></dt>

  <dd>The data applied or queried using the kPreTransform type represents
  pre-transformed matrix data. Given a matrix which has had other
  matrix operations applied to it, this type is used to obtain a
  matrix that does not taken into consideration any other matrix
  operations. This type is valid for methods of the
  MTransformationMatrix classes.</dd>

  <dt><b>kPostTransform</b></dt>

  <dd>The data applied or queried using the kPostTransform type
  represents post-transformed matrix data. Given a matrix which has
  had other matrix operations applied to it, this type is used to
  obtain a matrix that has taken into consideration all other matrix
  operations. This type is valid for methods of the
  MTransformationMatrix classes.</dd>

  <dt><b>kWorld</b></dt>

  <dd>The data applied or queried using the kWorld type represents the
  data for the objects world coordinates system. The results of this
  type will take into consideration all the transforms in the objects
  hierarchy. This type can be used with shape classes such as MFnMesh,
  MFnNurbsSurface and MFnCamera and the MFnTransform class. Note that
  to get the proper world space transformation data with
  MSpace::kWorld you <b>MUST</b> use the class constructor which
  initializes the function set with an MDagPath object.</dd>

  <dt><b>kObject</b></dt>

  <dd>The data applied or queried using the kObject type represents
  the data for the objects local coordinates system. The results of
  this type will be in the local space of the object (object
  space). This enum can only be used with shape classes such as
  MFnMesh, MFnNurbsSurface and MFnCamera.</dd>

  <dt><b>kLast</b></dt>

  <dd>This simply signifies the end of the MSpace enumerated
  types. All values that precede this value represent a coordinate
  system (with the exception of kInvalid which represents an invalid
  result.)</dd>

  </dl>

  \par Comparative Description:

  There are four spaces in which to apply a transformation.

  <ul>
  <li> World Space - This is the space that an object is in after it
  has had all of its transformations applied to it. A world space
  transformation is the the very last transformation that would be
  applied to the object.
  <li> Object Space - This is the space in which an object is
  defined. An object space transformation would be the first
  transformation that would be applied to an object. This is the same
  as pre-transform space.
  <li> Post Transform Space - This is the space in which the object
  lives after having the transformations of its transform node
  applied. For example, when a primitive object is created in Maya,
  there is a shape node which defined the geometry and a transform
  node which positions/orients the geometry. The space that the
  geometry is in is object space. After applying the transformations
  of the transform node to the geometry, the transformed geometry is
  in post transform space.
  <li> Transform Space - This space depends on the type of
  transformation being applied. Transform nodes define a fixed
  sequence of affine transformations. Basically, there is scale
  followed by rotation and finally translation. There are four spaces
  here:

		1) The space before applying the scale.\n
		2) The space after the scale but before the rotation.\n
		3) The space between the rotation and translation.\n
		4) The space after the translation.\n
		
		Space 1) is pre-transform space.\n
		Space 4) is post-transform space.\n
		Spaces 2) and 3) define transform spaces.\n 
		  
	If applying a transform space scale, this transform will be
	applied after the current scale but before the rotation (space
	2). If applying a transform space rotation, this transform will be
	applied after the current rotation but before the
	translation. Applying a translation in transform space, the
	translation will be applied after the current translation.
  </ul>
*/
class MSpace {
public:

    //! Transformation spaces.
	enum Space {
		kInvalid = 0,				//!< Invalid value.
	 	kTransform = 1,				//!< Transform matrix (relative) space.
 		kPreTransform = 2,			//!< Pre-transform matrix (geometry).
 		kPostTransform = 3,			//!< Post-transform matrix (world) space.
 		kWorld = 4,					//!< transform in world space.
 		kObject = kPreTransform,	//!< Same as pre-transform space.
		kLast = 5					//!< Last value, used for counting.
	};

};

OPENMAYA_NAMESPACE_CLOSE

// stat does not work reliably on NT for directories. Trailing '\'s cause an 
// error on NT4.0 and trailing '/'s and '\'s cause an error on Windows 2000.
// Use the following STAT macro to get consistent behavior on NT and UNIX.
//
#ifndef STAT
#ifdef _WIN32
#   define STAT statNT
	extern FND_EXPORT int statNT (const char *path, struct stat *buffer );
#elif defined(OSMac_MachO_)
	#ifdef __cplusplus
		#define STAT statMachO
	
		__inline int	statMachO (const char *path, struct stat *buffer)
		{
			return (stat(path, buffer));
		}
	#else	/* The inline definition cause multiple definition for C files. */
		#define STAT(path,buffer)		stat((path),(buffer))
	#endif
#else
#   define STAT statUNIX

__inline int statUNIX (const char *path, struct stat *buffer )
{
	return stat(path, buffer);
}
#endif
#endif
// ****************************************************************************
#endif /* __cplusplus */
#endif /* _MTypes */
