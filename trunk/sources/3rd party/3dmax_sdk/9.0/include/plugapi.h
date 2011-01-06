/*******************************************************************
 *
 *    DESCRIPTION: DLL Plugin API
 *
 *    AUTHOR: Dan Silva
 *
 *    HISTORY: 11/30/94 Started coding
 *
 *******************************************************************/

#include "maxheap.h"
#include "maxtypes.h"
#include "buildver.h"	// russom - 03/08/01
#include "maxversion.h"

#ifndef PLUGAPI_H_DEFINED
#define PLUGAPI_H_DEFINED

// 3DSMAX Version number to be compared against that returned by DLL's
// LibVersion function to detect obsolete DLLs.
//#define VERSION_3DSMAX 100      // changed  4/ 9/96 -- DB
//#define VERSION_3DSMAX 110      // changed  7/17/96 -- DS
//#define VERSION_3DSMAX 111      // changed  9/24/96 -- DS
//#define VERSION_3DSMAX 120      // changed  9/30/96 -- DB
//#define VERSION_3DSMAX 200      // changed  10/30/96 -- DS


// MAX release number X 1000
//#define MAX_RELEASE				2000      // DDS 9/30/97  
//#define MAX_RELEASE				2500      // JMK 2/25/98  
//#define MAX_RELEASE				3000      // DDS 9/3/98  
//#define MAX_RELEASE				3100      // CCJ 10/21/99  
//#define MAX_RELEASE				3900      // CCJ 6/26/00
//#define MAX_RELEASE				4000      // CCJ 11/29/00
//#define MAX_RELEASE				4200      // 010514  --prs.
//#define MAX_RELEASE				4900      // for R5 alpha, 011115  --prs.
//#define MAX_RELEASE				5000      // for R5, 020604  --prs.
//#define MAX_RELEASE				5100      // for R5.1, 020801  --prs.
//#define MAX_RELEASE				5900      // for R6 alpha, 030403  --prs.
//#define MAX_RELEASE				6000      // for R6, 030806  --prs.
//#define MAX_RELEASE_R7_ALPHA		6900 // for R7 alpha
#define MAX_RELEASE_R7				7000 // for R7 
#define MAX_RELEASE_R8_ALPHA		7800 // for R8 alpha
#define MAX_RELEASE_R8				8000 // for R8 
#define MAX_RELEASE_R9_ALPHA		8900 // for R9 alpha 
#define MAX_RELEASE_R9_ALPHA2		8901 // for R9 alpha - file format change
#define MAX_RELEASE_R9				9000 // for R9 release 
#define MAX_RELEASE					MAX_RELEASE_R9  // The current Max Release number

// MAX_RELEASE_EXTERNAL is an alternative for MAX_RELEASE
// that can be used for UI when the internal version of the product
// is different from the branded version number.
// MAX_RELEASE_EXTERNAL is defined in maxversion.h
#ifndef MAX_RELEASE_EXTERNAL
 #define MAX_RELEASE_EXTERNAL MAX_RELEASE
#endif


//! \brief Max SDK version numbers
/*! The SDK version number is updated every time a build of Max containing SDK 
changes is distributed to 3rd party plugin developers. Although we make every 
effort to keep Max backward compatible with plugins built against previous 
versions of the Max SDK, occasionally plugins are required to be re-built against
the latest version of the Max SDK in order for Max to load them. Plugins built 
against a certain version of the Max SDK will not load in previous versions of Max.
Plugins do not need to directly refer to the Max SDK version number, since it is 
encoded in VERSION_3DSMAX. 

SDK backward compatibility history:
- No SDK break between Max R4.x and R5.0, thus plugins built against all R4.x 
versions will load into Max R5.0
- No SDK break between Max R6.x and R8.0, thus plugins built against all R6.x 
versions will load into Max R8.0
- Max R9.0 SDK breaks compatibility with previous versions of Max, thus plugins 
must be re-built in order to load in Max R9.0

\see VERSION_3DSMAX
\see LibVersion
*/
// Blocks of values are reserved for specific products that are meant
// to have non-compatible APIs. Currently assigned:
//    000-???
//    255:     temporary use to prevent NOVA plugins from loading in MAX
#if defined(GAME_VER)			// russom 03/08/01

	// API numbers used by plugins to GMax. 
	#define GMAX_API_NUM_MAX_R50			128	// russom 01/17/01
	// Plugins to a version of GMax that is based on Max 9 would need to recompile 
	#define GMAX_API_NUM_MAX_R90			129	
	#define MAX_API_NUM						GMAX_API_NUM_MAX_R9	

#elif defined(WEBVERSION)

	// API numbers used by plugins to Plasma
	#define PLASMA_API_NUM_MAX_R50			144	// russom 02/07/01
	// Plugins to a version of Plasma that is based on Max 9 would need to recompile 
	#define PLASMA_API_NUM_MAX_R90			145
	#define MAX_API_NUM						PLASMA_API_NUM_MAX_R9

#elif defined(RENDER_VER)

	// API numbers used by plugins to Viz Render
	#define VIZRENDER_API_NUM_MAX_R50		160	// cleve ard 11/6/02
	#define VIZRENDER_API_NUM_MAX_R90		161	
	#define MAX_API_NUM						VIZRENDER_API_NUM_MAX_R9

#else

	// API numbers used by plugins to Max. The API number needs to be incremented for
	// every Max build (alpha, beta, final version) that is distributed to 3rd party 
	// developers and contains SDK changes.

	#define MAX_API_NUM_R20					4	   // DDS 9/30/97	
	#define MAX_API_NUM_R25					5	   // DDS 10/06/97
	#define MAX_API_NUM_R30					6	   // DDS 9/3/98
	#define MAX_API_NUM_R31					7	   // CCJ 5/14/99
	#define MAX_API_NUM_R40					8	   // CCJ 11/29/00 - R4

#ifndef MAXIMUS_BUILD

	// API numbers used by plugins to Max
	#define MAX_API_NUM_R42					9	 // R5 011031  --prs.
	#define MAX_API_NUM_R50					11	 // R5 FCS 020624  --prs.
	#define MAX_API_NUM_R60_PRE_REL			12	 // R6 prerelease 030409  --prs.
	#define MAX_API_NUM_R60					13	 // R6 FCS 030806  --prs.
	#define MAX_API_NUM_R70_PRE_REL			14	 // R7 prerelease 05/18/04 - russom
	#define MAX_API_NUM_R70					15	 // R7 FCS -- 2004-09-02 -- raffenn
	#define MAX_API_NUM_R80_PRE_REL			16	 // R8 pre-release Vesper -- 2005-01-18 -- raffenn
	#define MAX_API_NUM_R80					17	 // R8 FCS max 8 -- 2005-09-01 -- raffenn
	#define MAX_API_NUM_R90_ALPHA			18	 // R9 alpha -- 2005-09-21 -- aszabo
	#define MAX_API_NUM_R90_BETA1			19	 // R9 SDK Freeze for Beta 1 
	#define MAX_API_NUM_R90_BETA3			20	 // R9 SDK Freeze for Beta 3 (skipped beta 2)
	#define MAX_API_NUM_R90					21	 // R9 release 
	#define MAX_API_NUM						MAX_API_NUM_R90

#else	// if MAXIMUS_BUILD:
	
	#define MAXIMUS_API_NUM_MAX_R50			10	 // Maximus 020315  --prs.
	#define MAX_API_NUM						MAXIMUS_API_NUM_MAX_R50

#endif // Maximus

#endif // products

// This denotes the revision of the SDK for a given API. Increment this
// when the SDK functionality changes in some significant way (for instance
// a new GetProperty() query  response is added), but the headers have 
// not been changed.
#define MAX_SDK_REV     0	  // DDS 9/20/97

// This value should be returned by the LibVersion() method for DLL's:
#define VERSION_3DSMAX ((MAX_RELEASE<<16)+(MAX_API_NUM<<8)+MAX_SDK_REV)
#ifdef DESIGN_VER
#define VERSION_DMAX ((MAX_RELEASE<<16)+(MAX_API_NUM<<8)+MAX_SDK_REV)
#endif

// update jagapi.cpp product_app_id if new enums are added
typedef enum {kAPP_NONE, kAPP_MAX, kAPP_VIZ, kAPP_GMAX, kAPP_PLASMA, kAPP_VIZR} APPLICATION_ID;

//This method returns the ApplicationID, either VIZ or MAX. If a plugin
//is designed to work only in one product, then you could use this method
//in your IsPublic() call to switch between exposing the plug-in or not.
CoreExport APPLICATION_ID GetAppID();

// Macros for extracting parts of VERSION_3DSMAX
#define GET_MAX_RELEASE(x)  (((x)>>16)&0xffff)
#define GET_MAX_API_NUM(x)  (((x)>>8)&0xff)
#define GET_MAX_SDK_REV(x)  ((x)&0xff)
#define GET_MAX_SDK_NUMREV(x)  ((x)&0xffff) 

// Internal super classes, that plug-in developers need not know about
#define GEN_MODAPP_CLASS_ID 		0x00000b
#define MODAPP_CLASS_ID 			0x00000c
#define OBREF_MODAPP_CLASS_ID 		0x00000d
#define BASENODE_CLASS_ID			0x000001
#define GEN_DERIVOB_CLASS_ID 		0x000002
#define DERIVOB_CLASS_ID 			0x000003
#define WSM_DERIVOB_CLASS_ID 		0x000004
#define PARAMETER_BLOCK_CLASS_ID 	0x000008	
#define PARAMETER_BLOCK2_CLASS_ID 	0x000082	// <JBW> ParamBlocks Ed. 2
#define EASE_LIST_CLASS_ID			0x000009
#define AXIS_DISPLAY_CLASS_ID		0x00000e
#define MULT_LIST_CLASS_ID			0x00000f
#define NOTETRACK_CLASS_ID			0x0000ff
#define TREE_VIEW_CLASS_ID			0xffffff00
#define SCENE_CLASS_ID				0xfffffd00
#define THE_GRIDREF_CLASS_ID		0xfffffe00
#define VIEWREF_CLASS_ID			0xffffff01
#define BITMAPDAD_CLASS_ID			0xffffff02 // for drag/drop of bitmaps
#define PARTICLE_SYS_CLASS_ID		0xffffff03 // NOTE: this is for internal use only. Particle systems return GEOMOBJECT_CLASS_ID -- use IsParticleObject() to determine if an object is a particle system.
#define AGGMAN_CLASS_ID				0xffffff05 // Object aggregation, VIZ
#define MAXSCRIPT_WRAPPER_CLASS_ID	0xffffff06 // JBW, 5/14/99, MAX object wrappers within MAXScript
#define TRACKBAR_CLASS_ID			0xffffff07	// nleonard 22-DEC-05 the trackbar
#define MAKEREF_REST_CLASS_ID		0x9876544	// the superclassid for MakeRefRestore
#define DELREF_REST_CLASS_ID		0x9876543	// the superclassid for DeleteRefRestore
#define ASSIGNREF_REST_CLASS_ID		0x121242	// the superclassid for AssignMtlRestore

//-------------------------------------------
// Super-classes that are plugable.
#define GEOMOBJECT_CLASS_ID			0x000010
#define CAMERA_CLASS_ID				0x000020
#define LIGHT_CLASS_ID				0x000030
#define SHAPE_CLASS_ID				0x000040
#define HELPER_CLASS_ID				0x000050
#define SYSTEM_CLASS_ID	 			0x000060 
#define REF_MAKER_CLASS_ID			0x000100 	
#define REF_TARGET_CLASS_ID	 		0x000200
#define OSM_CLASS_ID				0x000810
#define WSM_CLASS_ID				0x000820
#define WSM_OBJECT_CLASS_ID			0x000830
#define SCENE_IMPORT_CLASS_ID		0x000A10
#define SCENE_EXPORT_CLASS_ID		0x000A20
#define BMM_STORAGE_CLASS_ID		0x000B10
#define BMM_FILTER_CLASS_ID			0x000B20
#define BMM_IO_CLASS_ID				0x000B30
#define BMM_DITHER_CLASS_ID			0x000B40
#define BMM_COLORCUT_CLASS_ID		0x000B50
#define USERDATATYPE_CLASS_ID		0x000B60
#define MATERIAL_CLASS_ID			0x000C00    // Materials
#define TEXMAP_CLASS_ID				0x000C10    // Texture maps
#define UVGEN_CLASS_ID				0x0000C20   // UV Generator
#define XYZGEN_CLASS_ID				0x0000C30   // XYZ Generator
#define TEXOUTPUT_CLASS_ID			0x0000C40   // Texture output filter 
#define SOUNDOBJ_CLASS_ID			0x000D00
#define FLT_CLASS_ID				0x000E00
#define RENDERER_CLASS_ID			0x000F00
#define BEZFONT_LOADER_CLASS_ID		0x001000
#define ATMOSPHERIC_CLASS_ID		0x001010
#define UTILITY_CLASS_ID			0x001020	// Utility plug-ins
#define TRACKVIEW_UTILITY_CLASS_ID	0x001030
#define FRONTEND_CONTROL_CLASS_ID	0x001040
#define MOT_CAP_DEV_CLASS_ID		0x001060
#define MOT_CAP_DEVBINDING_CLASS_ID	0x001050
#define OSNAP_CLASS_ID				0x001070
#define TEXMAP_CONTAINER_CLASS_ID	0x001080    // Texture map container
#define RENDER_EFFECT_CLASS_ID      0x001090    // Render post-effects
#define FILTER_KERNEL_CLASS_ID      0x0010a0    // AA Filter kernel
#define SHADER_CLASS_ID				0x0010b0    // plugable shading into stdmtl
#define COLPICK_CLASS_ID		  	0x0010c0    // color picker
#define SHADOW_TYPE_CLASS_ID		0x0010d0    // shadow generators
#define GUP_CLASS_ID		  		0x0010e0    // Global Utility Plug-In
#define LAYER_CLASS_ID				0x0010f0
#define SCHEMATICVIEW_UTILITY_CLASS_ID	0x001100
#define SAMPLER_CLASS_ID			0x001110
#define ASSOC_CLASS_ID				0x001120
#define GLOBAL_ASSOC_CLASS_ID		0x001130
#define IK_SOLVER_CLASS_ID			0x001140
#define RENDER_ELEMENT_CLASS_ID		0x001150	// render output elements
#define BAKE_ELEMENT_CLASS_ID		0x001151	// texture bake output elements
#define CUST_ATTRIB_CLASS_ID		0x001160    // Custom attributes
#define RADIOSITY_CLASS_ID			0x001170	// global illumination plugins
#define TONE_OPERATOR_CLASS_ID		0x001180	// tone operator plugins
#define MPASS_CAM_EFFECT_CLASS_ID	0x001190	// multi-pass camera effect plugins
#define MR_SHADER_CLASS_ID_DEFUNCT	0x0011a0	// mental ray shaders (defunct, kept for file compatibility)

// Super-class ID's of various controls
#define	CTRL_SHORT_CLASS_ID 	   	0x9001
#define	CTRL_INTEGER_CLASS_ID		0x9002
#define	CTRL_FLOAT_CLASS_ID			0x9003
#define	CTRL_POINT2_CLASS_ID	   	0x9004
#define	CTRL_POINT3_CLASS_ID	   	0x9005
#define	CTRL_POS_CLASS_ID		   	0x9006
#define	CTRL_QUAT_CLASS_ID			0x9007
#define	CTRL_MATRIX3_CLASS_ID		0x9008
#define	CTRL_COLOR_CLASS_ID     	0x9009	// float color (RGB)
#define	CTRL_COLOR24_CLASS_ID   	0x900A   // 24 bit color
#define	CTRL_POSITION_CLASS_ID		0x900B
#define	CTRL_ROTATION_CLASS_ID		0x900C
#define	CTRL_SCALE_CLASS_ID			0x900D
#define CTRL_MORPH_CLASS_ID			0x900E
#define CTRL_USERTYPE_CLASS_ID		0x900F  // User defined type
#define CTRL_MASTERPOINT_CLASS_ID	0x9010	
#define MASTERBLOCK_SUPER_CLASS_ID	0x9011
#define	CTRL_POINT4_CLASS_ID	   	0x9012
#define	CTRL_FRGBA_CLASS_ID     	0x9013	// float color (RGBA)

//------------------------------------------------------------------------------
// Pseudo-super-class ids
#define DEFORM_OBJ_CLASS_ID 		0x000005 // Deformable object psuedo-super-class
#define MAPPABLE_OBJ_CLASS_ID 		0x000006 // Mappable object psuedo-super-class
#define GENERIC_SHAPE_CLASS_ID		0x0000ab // Shape psuedo-super-class

//------------------------------------------------------------------------------
// Class ID's of built-in classes. The value is the first ULONG of the 
// 8 byte Class ID: the second ULONG is 0 for all built-in classes.
// NOTE: Only built-in classes should have the second ULONG == 0.
#define PATH_CONTROL_CLASS_ID				0x2011
#define EULER_CONTROL_CLASS_ID				0x2012
#define EXPR_POS_CONTROL_CLASS_ID			0x2013
#define EXPR_P3_CONTROL_CLASS_ID			0x2014
#define EXPR_FLOAT_CONTROL_CLASS_ID			0x2015
#define EXPR_SCALE_CONTROL_CLASS_ID			0x2016
#define EXPR_ROT_CONTROL_CLASS_ID			0x2017
#define LOCAL_EULER_CONTROL_CLASS_ID		0x2018
#define POSITION_CONSTRAINT_CLASS_ID		0x2019  // AG added for Position Constraint 4/21/2000
#define ORIENTATION_CONSTRAINT_CLASS_ID		0x2020  // AG added for Position Constraint 5/04/2000
#define LOOKAT_CONSTRAINT_CLASS_ID			0x2021  // AG added for new LookAt Constraint 5/24/2000
#define ADDITIVE_EULER_CONTROL_CLASS_ID		0x2022	// 000830  --prs.
#define BOOLCNTRL_CLASS_ID					Class_ID(0x5ed56fc5, 0x6af0e521) // AG added for the new Boolean Controller 11/08/2001


#define FLOATNOISE_CONTROL_CLASS_ID		0x87a6df24
#define POSITIONNOISE_CONTROL_CLASS_ID	0x87a6df25
#define POINT3NOISE_CONTROL_CLASS_ID	0x87a6df26
#define ROTATIONNOISE_CONTROL_CLASS_ID	0x87a6df27
#define SCALENOISE_CONTROL_CLASS_ID		0x87a6df28
#define SURF_CONTROL_CLASSID			Class_ID(0xe8334011,0xaeb330c8)
#define LINKCTRL_CLASSID				Class_ID(0x873fe764,0xaabe8601)
#define ATTACH_CONTROL_CLASS_ID			Class_ID(0xbb27e611,0xa72f43e7)

#define FLOATLIMITCTRL_CLASS_ID			0x17127e3e

#define DUMMYCHANNEL_CLASS_ID				0xefffffff // used by PB and PB2 - superclass is CTRL_USERTYPE_CLASS_ID

#define RINGARRAY_CLASS_ID			0x9120 // use it as Class_ID(RINGARRAY_CLASS_ID,0);
#define SUNLIGHT_CLASS_ID			Class_ID(0x5897670e, 0x61b5008d)
#define DAYLIGHT_CLASS_ID			Class_ID(0x18147db5, 0x20f17194)

#define CUSTATTRIB_CONTAINER_CLASS_ID Class_ID(0x5ddb3626, 0x23b708db) // CustAttribContainer class id


// subclasses of GEOMOBJECT_CLASS_ID:
// Built into CORE
#define TRIOBJ_CLASS_ID 	 	0x0009	  
#define EDITTRIOBJ_CLASS_ID	0xe44f10b3	// base triangle mesh
#define POLYOBJ_CLASS_ID		0x5d21369a	// polygon mesh
#define PATCHOBJ_CLASS_ID  		0x1030
#define NURBSOBJ_CLASS_ID		0x4135

// editable poly
#define EPOLYOBJ_CLASS_ID Class_ID(0x1bf8338d,0x192f6098)

// Primitives
// xavier robitaille | 03.02.15 
// Prevents the user from creating box/cylinder/sphere objects without
// removing them completely (we need them for the material editor).
#ifndef NO_OBJECT_STANDARD_PRIMITIVES
#define BOXOBJ_CLASS_ID 		0x0010
#define SPHERE_CLASS_ID 		0x0011 
#define CYLINDER_CLASS_ID 		0x0012
#define PLANE_CLASS_ID          Class_ID(0x81f1dfc, 0x77566f65)
#define PYRAMID_CLASS_ID        Class_ID(0x76bf318a, 0x4bf37b10)
#define GSPHERE_CLASS_ID        Class_ID(0, 32670)

#else
#define BOXOBJ_CLASS_ID 		0x108f1589
#define SPHERE_CLASS_ID 		0x72ce5ad6 
#define CYLINDER_CLASS_ID 		0x759e40e1
#endif
#define CONE_CLASS_ID			0xa86c23dd
#define TORUS_CLASS_ID			0x0020
#define TUBE_CLASS_ID			0x7B21
#define HEDRA_CLASS_ID			0xf21c5e23
#define BOOLOBJ_CLASS_ID		0x387BB2BB
// Above used in obselete Boolean -- henceforth use following:
#define NEWBOOL_CLASS_ID Class_ID(0x51db4f2f,0x1c596b1a)

// Class ID for XRef object
#define XREFOBJ_CLASS_ID		Class_ID(0x92aab38c, 0)
// Class ID for Xref atmospherics
#define XREFATMOS_CLASS_ID	Class_ID(0x4869d60f, 0x21af2ae4)
// Class ID for Xref Materials
#define XREFMATERIAL_CLASS_ID Class_ID(0x272c0d4b, 0x432a414b)
// Class ID for Xref Controllers
#define XREFCTRL_CLASS_ID Class_ID(0x32fb4637, 0x65fd482b)

//Subclasses of object snaps.
#define GRID_OSNAP_CLASS_ID Class_ID(0x62f565d6, 0x110a1f97)


// The teapot is unique in that it uses both DWORDs in its class IDs
// Note that this is what 3rd party plug-ins SHOULD do.
#define TEAPOT_CLASS_ID1		0xACAD13D3
#define TEAPOT_CLASS_ID2		0xACAD26D9

#define PATCHGRID_CLASS_ID  	0x1070

// New procedural bone object for Magma (subclass of GeomObject)
#define BONE_OBJ_CLASSID		Class_ID(0x28bf6e8d, 0x2ecca840)


// Particles
#define RAIN_CLASS_ID			0x9bd61aa0
#define SNOW_CLASS_ID			0x9bd61aa1

// Space Warp Objects
#define WAVEOBJ_CLASS_ID 		0x0013

// The basic lofter class
#define LOFTOBJ_CLASS_ID		0x1035
#define LOFT_DEFCURVE_CLASS_ID	0x1036

// Our implementation of the lofter
#define LOFT_GENERIC_CLASS_ID	0x10B0

#define TARGET_CLASS_ID  		0x1020  // should this be a helper?
#define MORPHOBJ_CLASS_ID		0x1021

// Subclasses of SHAPE_CLASS_ID
#define SPLINESHAPE_CLASS_ID 	0x00000a
#define LINEARSHAPE_CLASS_ID 	0x0000aa
#define SPLINE3D_CLASS_ID  		0x1040
#define NGON_CLASS_ID  			0x1050
#define DONUT_CLASS_ID  		0x1060
#define STAR_CLASS_ID			0x1995
#define RECTANGLE_CLASS_ID		0x1065
#define HELIX_CLASS_ID			0x1994
#define ELLIPSE_CLASS_ID		0x1097
#define CIRCLE_CLASS_ID			0x1999
#define TEXT_CLASS_ID			0x1993
#define ARC_CLASS_ID			0x1996
#define HALF_ROUND_CLASS_ID		0x1997
#define QUARTER_ROUND_CLASS_ID	0x1998
#define PIPE_CLASS_ID			0x199A
#define WALLED_RECTANGLE_CLASS_ID 0xd8bff66c
#define CHANNEL_CLASS_ID		0x7dbc0e96
#define ANGLE_CLASS_ID			0x93257030
#define TEE_CLASS_ID			0xfaead70c
#define WIDE_FLANGE_CLASS_ID	0xa2b72ef6


// subclasses of CAMERA_CLASS_ID:
#define SIMPLE_CAM_CLASS_ID  	0x1001
#define LOOKAT_CAM_CLASS_ID  	0x1002

// subclasses of LIGHT_CLASS_ID:
#define OMNI_LIGHT_CLASS_ID  	0x1011
#define SPOT_LIGHT_CLASS_ID  	0x1012
#define DIR_LIGHT_CLASS_ID  	0x1013
#define FSPOT_LIGHT_CLASS_ID  	0x1014
#define TDIR_LIGHT_CLASS_ID  	0x1015

// subclasses of HELPER_CLASS_ID
#define DUMMY_CLASS_ID 			0x876234
#define BONE_CLASS_ID 			0x8a63c0
#define TAPEHELP_CLASS_ID 		0x02011
#define GRIDHELP_CLASS_ID		0x02010
#define POINTHELP_CLASS_ID		0x02013
#define PROTHELP_CLASS_ID		0x02014

//subclasses of UVGEN_CLASS_ID
#define STDUV_CLASS_ID 			0x0000100

//subclasses of XYZGEN_CLASS_ID
#define STDXYZ_CLASS_ID 		0x0000100

//subclasses of TEXOUT_CLASS_ID
#define STDTEXOUT_CLASS_ID 		0x0000100

// subclasses of MATERIAL_CLASS_ID	
#define DMTL_CLASS_ID  			0x00000002	// Origninal Stdmtl
#define DMTL2_CLASS_ID  		0x00000003	// R2.5 stdmtl

#ifndef NO_MTL_TOPBOTTOM // orb 01-07-2001
#define CMTL_CLASS_ID 			0x0000100  // top-bottom material 
#endif // NO_MTL_TOPBOTTOM

#define MULTI_CLASS_ID 			0x0000200  // multi material
#define DOUBLESIDED_CLASS_ID 	0x0000210  // double-sided mtl
#define MIXMAT_CLASS_ID 		0x0000250  // blend mtl
#define BAKE_SHELL_CLASS_ID 	0x0000255  // 2 material shell for baking

#ifndef NO_MTL_MATTESHADOW // orb 01-07-2001
#define MATTE_CLASS_ID 			0x0000260  // Matte mtl
#endif // NO_MTL_MATTESHADOW

// subclasses of TEXMAP_CLASS_ID	
#define CHECKER_CLASS_ID 		0x0000200
#define MARBLE_CLASS_ID 		0x0000210
#define MASK_CLASS_ID 			0x0000220  // mask texture
#define MIX_CLASS_ID 			0x0000230
#define NOISE_CLASS_ID 			0x0000234

#ifndef NO_MAPTYPE_GRADIENT // orb 01-07-2001
#define GRADIENT_CLASS_ID 		0x0000270
#endif // NO_MAPTYPE_GRADIENT

#ifndef NO_MAPTYPE_RGBTINT // orb 01-07-2001
#define TINT_CLASS_ID 			0x0000224  // Tint texture
#endif // NO_MAPTYPE_RGBTINT

#define BMTEX_CLASS_ID 			0x0000240  // Bitmap texture

#ifndef NO_MAPTYPE_REFLECTREFRACT // orb 01-07-2001
#define ACUBIC_CLASS_ID 		0x0000250  // Reflect/refract
#endif // NO_MAPTYPE_REFLECTREFRACT

#ifndef NO_MAPTYPE_FLATMIRROR // orb 01-07-2001
#define MIRROR_CLASS_ID 		0x0000260  // Flat mirror
#endif // NO_MAPTYPE_FLATMIRROR

#define COMPOSITE_CLASS_ID 		0x0000280   // Composite texture

#ifndef NO_MAPTYPE_RGBMULT // orb 01-07-2001
#define RGBMULT_CLASS_ID 		0x0000290   // RGB Multiply texture
#endif //NO_MAPTYPE_RGBMULT

#define FALLOFF_CLASS_ID 		0x00002A0   // Falloff texture

#ifndef NO_MAPTYPE_OUTPUT // orb 01-07-2001
#define OUTPUT_CLASS_ID 		0x00002B0   // Output texture
#endif // NO_MAPTYPE_OUTPUT

#define PLATET_CLASS_ID 		0x00002C0   // Plate glass texture

#ifndef NO_MAPTYPE_VERTCOLOR // orb 01-07-2001
#define VCOL_CLASS_ID 			0x0934851	// Vertex color map
#endif // NO_MAPTYPE_VERTCOLOR

// Subclasses of SHADER_CLASS_ID
#define STDSHADERS_CLASS_ID		0x00000035	// to 39 

// Subclasses of SHADOW_TYPE_CLASS_ID	
#define STD_SHADOW_MAP_CLASS_ID       0x0000100
#define STD_RAYTRACE_SHADOW_CLASS_ID  0x0000200
		
// subclasses of RENDERER_CLASS_ID		  
#define SREND_CLASS_ID 			0x000001 // default scan-line renderer

// subclasses of BAKE_ELEMENT_CLASS_ID
#define COMPLETE_BAKE_ELEMENT_CLASS_ID			0x00010001
#define SPECULAR_BAKE_ELEMENT_CLASS_ID			0x00010002
#define DIFFUSE_BAKE_ELEMENT_CLASS_ID			0x00010003
#define REFLECT_REFRACT_BAKE_ELEMENT_CLASS_ID	0x00010004
#define SHADOW_BAKE_ELEMENT_CLASS_ID			0x00010005
#define LIGHT_BAKE_ELEMENT_CLASS_ID				0x00010006
#define NORMALS_ELEMENT_CLASS_ID				0x00010007
#define BLEND_BAKE_ELEMENT_CLASS_ID				0x00010008
#define ALPHA_BAKE_ELEMENT_CLASS_ID				0x00010009
#define HEIGHT_BAKE_ELEMENT_CLASS_ID			0x0001000A
#define AMBIENTOCCLUSION_BAKE_ELEMENT_CLASS_ID	0x0001000B

// subclasses of REF_MAKER_CLASS_ID			
#define MTL_LIB_CLASS_ID 		0x001111
#define MTLBASE_LIB_CLASS_ID 	0x003333
#define THE_SCENE_CLASS_ID   	0x002222
#define MEDIT_CLASS_ID 	 		0x000C80

// subclass of all classes
#define STANDIN_CLASS_ID   		0xffffffff  // subclass of all super classes



// Default sound object
#define DEF_SOUNDOBJ_CLASS_ID	0x0000001

// Default atmosphere
#define FOG_CLASS_ID 0x10000001

//------------------ Modifier sub classes --------
#define SKEWOSM_CLASS_ID			0x6f3cc2aa
#define BENDOSM_CLASS_ID 			0x00010
#define TAPEROSM_CLASS_ID 			0x00020
#define TWISTOSM_CLASS_ID 			0x00090
#define SPLINEIKCONTROL_CLASS_ID	Class_ID(0x5f43ba4, 0x55fe9305) //AG added : 01/08/02

#define UVWMAPOSM_CLASS_ID			0xf72b1
#define SELECTOSM_CLASS_ID			0xf8611
#define MATERIALOSM_CLASS_ID		0xf8612
#define SMOOTHOSM_CLASS_ID			0xf8613
#define NORMALOSM_CLASS_ID			0xf8614
#define OPTIMIZEOSM_CLASS_ID		0xc4d31
#define AFFECTREGION_CLASS_ID		0xc4e32
#define SUB_EXTRUDE_CLASS_ID		0xc3a32
#define TESSELLATE_CLASS_ID			0xa3b26ff2
#define DELETE_CLASS_ID				0xf826ee01
#define MESHSELECT_CLASS_ID			0x73d8ff93
#define UVW_XFORM_CLASS_ID			0x5f32de12
#define UVW_XFORM2_CLASS_ID			0x5f32de13 //SS 11/20/2002: added

#define EXTRUDEOSM_CLASS_ID 		0x000A0
#define SURFREVOSM_CLASS_ID 		0x000B0

#define DISPLACEOSM_CLASS_ID		0xc4d32
#define DISPLACE_OBJECT_CLASS_ID	0xe5240
#define DISPLACE_WSM_CLASS_ID		0xe5241

#define SINEWAVE_OBJECT_CLASS_ID 	0x00030
#define SINEWAVE_CLASS_ID 			0x00040
#define SINEWAVE_OMOD_CLASS_ID 		0x00045
#define LINWAVE_OBJECT_CLASS_ID 	0x00035
#define LINWAVE_CLASS_ID 			0x00042
#define LINWAVE_OMOD_CLASS_ID 		0x00047

#define GRAVITYOBJECT_CLASS_ID		0xe523c
#define GRAVITYMOD_CLASS_ID			0xe523d
#define WINDOBJECT_CLASS_ID			0xe523e
#define WINDMOD_CLASS_ID			0xe523f

#define DEFLECTOBJECT_CLASS_ID		0xe5242
#define DEFLECTMOD_CLASS_ID			0xe5243

#define BOMB_OBJECT_CLASS_ID 		0xf2e32
#define BOMB_CLASS_ID 				0xf2e42

// These are the FFD Modifier's class ID's
#define FFDNMOSSQUARE_CLASS_ID		Class_ID(0x8ab36cc5,0x82d7fe74)
#define FFDNMWSSQUARE_CLASS_ID		Class_ID(0x67ea40b3,0xfe7a30c4)
#define FFDNMWSSQUARE_MOD_CLASS_ID	Class_ID(0xd6636ea2,0x9aa42bf3)

#define FFDNMOSCYL_CLASS_ID			Class_ID(0x98f37a63,0x3ffe9bca)
#define FFDNMWSCYL_CLASS_ID			Class_ID(0xfa4700be,0xbbe85051)
#define FFDNMWSCYL_MOD_CLASS_ID		Class_ID(0xf1c630a3,0xaa8ff601)

#define FFD44_CLASS_ID				Class_ID(0x21325596, 0x2cd10bd8)
#define FFD33_CLASS_ID				Class_ID(0x21325596, 0x2cd10bd9)
#define FFD22_CLASS_ID				Class_ID(0x21325596, 0x2cd10bd0)

//JH Association context modifiers
//GEOM TO GEOM
#define ACMOD_GEOM_GEOM_BOOLADD_CID	0x4e0f483a
#define ACMOD_GEOM_GEOM_BOOLSUB_CID	0x61661a5c
#define ACMOD_GEOM_GEOM_BOOLINT_CID	0x2a4f3945
#define ACMOD_GEOM_GEOM_SIMPAGG_CID	0x40cb05ab

//SHAPE To GEOM
#define ACMOD_SHAPE_GEOM_HOLE_CID	0x366307b0
#define ACMOD_SHAPE_GEOM_INT_CID	0x782d8d50
#define ACMOD_SHAPE_GEOM_EMBOSS_CID	0x7a13397c
#define ACMOD_SHAPE_GEOM_REVEAL_CID	0x55ed658c

//JH Solids
#define GENERIC_AMSOLID_CLASS_ID	Class_ID(0x5bb661e8, 0xa2c27f02)


//------------------ Controller sub classes --------
#define LININTERP_FLOAT_CLASS_ID 			0x2001
#define LININTERP_POSITION_CLASS_ID 		0x2002
#define LININTERP_ROTATION_CLASS_ID 		0x2003
#define LININTERP_SCALE_CLASS_ID			0x2004
#define PRS_CONTROL_CLASS_ID				0x2005
#define LOOKAT_CONTROL_CLASS_ID				0x2006				

#define HYBRIDINTERP_FLOAT_CLASS_ID 		0x2007
#define HYBRIDINTERP_POSITION_CLASS_ID 		0x2008
#define HYBRIDINTERP_ROTATION_CLASS_ID 		0x2009
#define HYBRIDINTERP_POINT3_CLASS_ID		0x200A
#define HYBRIDINTERP_SCALE_CLASS_ID			0x2010
#define HYBRIDINTERP_COLOR_CLASS_ID			0x2011
#define HYBRIDINTERP_POINT4_CLASS_ID		0x2012
#define HYBRIDINTERP_FRGBA_CLASS_ID			0x2013

#define TCBINTERP_FLOAT_CLASS_ID 			0x442311
#define TCBINTERP_POSITION_CLASS_ID 		0x442312
#define TCBINTERP_ROTATION_CLASS_ID 		0x442313
#define TCBINTERP_POINT3_CLASS_ID			0x442314
#define TCBINTERP_SCALE_CLASS_ID			0x442315
#define TCBINTERP_POINT4_CLASS_ID			0x442316

#define MASTERPOINTCONT_CLASS_ID			0xd9c20ff


//------------------ PF sub classes ---------------
#define PF_SUBCLASS_ID_PARTB		0x1eb34000
//-------------------sub class part A
#define PF_OPERATOR_SUBCLASS_ID 	0x74f93a02
#define PF_OPERATOR3D_SUBCLASS_ID 	0x74f93a03
#define PF_TEST_SUBCLASS_ID 		0x74f93a04
#define PF_TEST3D_SUBCLASS_ID 		0x74f93a05
#define PF_ITEM_SUBCLASS_ID 		0x74f93a06
#define PF_OBJECT_SUBCLASS_ID 		0x74f93a07

//--------------------------------------------------

class ISave;
class ILoad;
class Interface;
class ActionTable;
typedef short BlockID;
class ParamBlockDesc2;
class IParamBlock2;
class IObjParam;
class Animatable;
class ParamMap2UserDlgProc;
class IParamMap2;
class FPInterface;
class Manipulator;
class ReferenceTarget;


// System keeps a list of the DLL's found on startup.
// This is the interface to a single class
class ClassDesc: public MaxHeapOperators {
	private:
		Tab<FPInterface*>		interfaces;		// the FnPub interfaces published by this plugin
	public:
		virtual					~ClassDesc() {}
		virtual int				IsPublic()=0;  // Show this in create branch?
		virtual void *			Create(BOOL loading=FALSE)=0;   // return a pointer to an instance of the class.
		virtual	int 			BeginCreate(Interface *i) {return 0;}
		virtual int 			EndCreate(Interface *i) {return 0;};
		virtual const TCHAR* 	ClassName()=0;
		virtual SClass_ID		SuperClassID()=0;
		virtual Class_ID		ClassID()=0;
		virtual const TCHAR* 	Category()=0;   // primitive/spline/loft/ etc
		virtual BOOL			OkToCreate(Interface *i) { return TRUE; }	// return FALSE to disable create button		
		virtual BOOL			HasClassParams() {return FALSE;}
		virtual void			EditClassParams(HWND hParent) {}
		virtual void			ResetClassParams(BOOL fileReset=FALSE) {}

        // These functions return keyboard action tables that plug-ins can use
        virtual int             NumActionTables() { return 0; }
        virtual ActionTable*  GetActionTable(int i) { return NULL; }

        // Manipulator Interface

        // Returns true of the class implements a manipulator object.
        virtual BOOL IsManipulator() { return FALSE; }

        // Returns true if the class is a manipulator and it
        // manipulates the given reference targer which can be a base
        // object, modifier or controller.  When starting "Manipulate"
        // mode, this is called on selected nodes for the base object,
        // all modifiers, the TM controller and the position, rotation
        // and scale controllers, if the TM controller is a
        // PRSController.
        virtual BOOL CanManipulate(ReferenceTarget* hTarget) { return FALSE; }
        // Returns true if the manipulator applies to the given node.
        // This is general case if CanManipulateClassID isn't sufficient.
        // This can be used to indicate that the manipulator works on a part
        // of the node not covered by the cases mentioned above, such as the
        // visibility controller.
        virtual BOOL CanManipulateNode(INode* pNode) { return FALSE; }

        // When a manipulator returns TRUE to CanManipulateClassID(),
        // the system calls this version of CreateManipulator to
        // create an instance.  The hTarget passed is the object,
        // modifier or controller that the manipulator said it could
        // manipulate.
        virtual Manipulator* CreateManipulator(ReferenceTarget* hTarget,
                                               INode* pNode) {return NULL;}
        // When a manipulator returns TRUE to CanManipulateNode(INode*),
        // the system calls this version of CreateManipulator to
        // create an instance.  
        virtual Manipulator* CreateManipulator(INode* pNode) {return NULL;}

		// Class IO
		virtual BOOL			NeedsToSave() { return FALSE; }
		virtual IOResult 		Save(ISave *isave) { return IO_OK; }
		virtual IOResult 		Load(ILoad *iload) { return IO_OK; }

		// bits of dword set indicate corrresponding rollup page is closed.
		// the value 0x7fffffff is returned by the default implementation so the
		// command panel can detect this method is not being overridden, and just leave 
		// the rollups as is.
		virtual DWORD			InitialRollupPageState() { return 0x7fffffff; }

		// ParamBlock2-related metadata interface, supplied & implemented in ClassDesc2 (see maxsdk\include\iparamb2.h)
		virtual const TCHAR*	InternalName() { return NULL; }
		virtual HINSTANCE		HInstance() { return NULL; }
		// access parameter block descriptors for this class
		virtual int				NumParamBlockDescs() { return 0; }
		virtual ParamBlockDesc2* GetParamBlockDesc(int i) { return NULL; }
		virtual ParamBlockDesc2* GetParamBlockDescByID(BlockID id) { return NULL; }
		virtual void			AddParamBlockDesc(ParamBlockDesc2* pbd) { }
		// automatic UI management
		virtual void			BeginEditParams(IObjParam *ip, ReferenceMaker* obj, ULONG flags, Animatable *prev) { }
		virtual void			EndEditParams(IObjParam *ip, ReferenceMaker* obj, ULONG flags, Animatable *prev) { }
		virtual void			InvalidateUI(ParamBlockDesc2* pbd) { }
		CoreExport virtual TCHAR* GetRsrcString(INT_PTR id);
		// automatic ParamBlock construction
		virtual void			MakeAutoParamBlocks(ReferenceMaker* owner) { }
		// access automatically-maintained ParamMaps, by simple index or by associated ParamBlockDesc
		virtual int				NumParamMaps() { return 0; }
		virtual IParamMap2*		GetParamMap(int i) { return NULL; }
		virtual IParamMap2*		GetParamMap(ParamBlockDesc2* pbd) { return NULL; }
		// maintain user dialog procs on automatically-maintained ParamMaps
		virtual void			SetUserDlgProc(ParamBlockDesc2* pbd, ParamMap2UserDlgProc* proc=NULL) { }
		virtual ParamMap2UserDlgProc* GetUserDlgProc(ParamBlockDesc2* pbd) { return NULL; }

		// Class can draw an image to represent itself graphically...
		virtual bool DrawRepresentation(COLORREF bkColor, HDC hDC, Rect &rect) { return FALSE; }

		// Function Publishing interfaces - jbw 3.13.00
		virtual int				NumInterfaces() { return interfaces.Count(); }
		virtual FPInterface*	GetInterfaceAt(int i) { return interfaces[i]; }
		CoreExport virtual FPInterface*	GetInterface(Interface_ID id);
		CoreExport virtual FPInterface*	GetInterface(TCHAR* name);
		CoreExport virtual void	AddInterface(FPInterface* fpi);
		virtual void			ClearInterfaces() { interfaces.ZeroCount(); }
		
		// This method can be used for further categorizing plugin's. If a plugin has 
		// sub-plugins(like light > shadows, particles > operators), this method can be 
		// used to differentiate them. sub-plugins can be derived off reference target
		// but return a particular class ID published by the parent plugins SDK headers. 
		// Then parent plugin can get a list of all reference targets whose SubClassID 
		// matches the published SubClassID
		virtual Class_ID		SubClassID() { return Class_ID(); }

		// Generic expansion function
		virtual INT_PTR Execute(int cmd, ULONG_PTR arg1=0, ULONG_PTR arg2=0, ULONG_PTR arg3=0) { return 0; } 
	};


// ClassDesc::Execute cmd values
#define I_EXEC_CTRL_BYPASS_TREE_VIEW 1001 // return 1 to hide your controller in track view

// arg1 is expected to be bool* The bool gets set to true or false 
// depending whether sub-obj anim is or not enabled for that object type.
// The Execute method returns TRUE if the arg1 is set successfully, otherwise FALSE
#define I_EXEC_GET_SOA_STATE 1002 

// arg1 is expected to be a TimeValue* The TimeValue gets set appropriately,
// depending on whether sub-obj animation is allowed of not for that object type.
// The Execute method returns TRUE if the arg1 is set successfully, otherwise FALSE
#define I_EXEC_EVAL_SOA_TIME 1003


// Create instance of the specified class
CoreExport void *CreateInstance(SClass_ID superID, Class_ID classID);

//! \brief CallParam data for NOTIFY_CLASSDESC_REPLACED broadcasts.
/*! A pointer to an instance of this structure is passed to BroadcastNotification after a DataClassDesc is 
replaced by a ClassDesc in a ClassDirectory. This occurs when the dll containing a deferred loaded plugin 
is loaded.
\see NOTIFY_CLASSDESC_REPLACED
*/
struct ClassDescReplaced: public MaxHeapOperators {  
	//! the old class descriptor 
	const ClassDesc *oldClassDesc;
	//! the new class descriptor
	const ClassDesc *newClassDesc;
}; 


#endif