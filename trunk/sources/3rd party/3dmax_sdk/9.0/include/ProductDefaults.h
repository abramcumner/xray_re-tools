/*****************************************************************************
* FILE:					ProductDefaults.h
* DESCRIPTION:	File defining per product built in defaults 
* CREATED BY:		Attila Szabo
* HISTORY:	
* - 2003.Oct.14 - created 
* - 2003.Oct.21 - AS - Added MAX_RELEASE_EXTERNAL 
******************************************************************************
* Copyright (c) 2003 Autodesk, All Rights Reserved.
******************************************************************************/

#ifndef _PRODUCT_DEFAULTS_H_
#define _PRODUCT_DEFAULTS_H_

#include "buildver.h"

#ifdef METRIC_UNITS_FIXED_VALUES
  #define DFLT_XFORM_MOVE_SPIN_INCREMENT        0.01f
  #define DFLT_EDIT_MESH_NORMAL_SCALE           0.5f
  #define DFLT_EDIT_MESH_MOD_NORMAL_SCALE       0.5f
  #define DFLT_EDIT_MESH_WELD_TRESHOLD          0.005f
  #define DFLT_EDIT_MESH_MOD_WELD_TRESHOLD      0.005f
  #define DFLT_MAPSCALER_OSM_MOD_DFLT_SCALE     1.0f
  #define DFLT_MAPSCALER_WSM_MOD_DFLT_SCALE     1.0f
  #define DFLT_EDIT_SPLINE_WELD_TRESHOLD        0.005f
  #define DFLT_EDIT_SPLINE_MOD_WELD_TRESHOLD    0.005f
  #define DFLT_EDIT_SPLINE_THICKNESS            0.05f
#endif // METRIC_UNITS_FIXED_VALUES

#ifdef RENDER_VER // VIZ Render ==============================================
	// Possible values:	"Flow Ray Traced" "Flow Ray Traced" "Adv. Ray Traced" "Ray Traced Shadow" "Shadow Map"
  #define SHADOW_GENERATOR_STR              _T("Flow Ray Traced") // TODO: do it through resource override
  #define PRIVATE_INCOMPATIBLE_OMNI_LIGHT   Class_ID(0x396431cc, 0x62c20b0) 

  // Possible values:	
  //  Class_ID(0x39ff19ec, 0x772d5bff)	- LightTracer
  //  Class_ID(0x795c4168, 0x669a1835)	- Radiosity
  #define DEFAULT_RADIOSITY_CLASS_ID        Class_ID(0x795c4168, 0x669a1835)	

  // Define this to be the class id for the default tone operator class.
  // If it isn't defined, no tone operator is assigned by default.
  // Possible values:	
  //  Class_ID(0x58b4684b, 0x507f76e9)	- Logarithmic
  //  Class_ID(0x786c6aaa, 0x449349db)	- Adaptive
  //  Class_ID(0x55897318, 0x34db21ac)	- Linear
  //  Class_ID(0x575e3dff, 0x60e13d9a)	- Pseudo color
  #define DEFAULT_TONE_OPERATOR_CLASS_ID    Class_ID(0x58b4684b, 0x507f76e9)

#endif

#ifdef DESIGN_VER
#endif

#ifdef GAME_VER
  // MAX_RELEASE_EXTERNAL is an alternative for MAX_RELEASE (plugapi.h)
  // that can be used by UI when the internal version of the product
  // is different from the branded version number
  #define MAX_RELEASE_EXTERNAL	1200
#endif

#ifdef WEBVERION
  // MAX_RELEASE_EXTERNAL is an alternative for MAX_RELEASE (plugapi.h)
  // that can be used by UI when the internal version of the product
  // is different from the branded version number
  #define MAX_RELEASE_EXTERNAL	1000
#endif
#endif // _PRODUCT_DEFAULTS_H_

