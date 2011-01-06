/*****************************************************************************
* FILE:          ProductFileSupport.h
* DESCRIPTION:  Defines symbols used to support files saved by different 
*                vertical apps. 
* CREATED BY:    Michael Russo
* HISTORY:  
* - 2003.oct.14 - AS - Pulled them out in their own file
******************************************************************************
* Copyright (c) 2003 Autodesk, All Rights Reserved.
******************************************************************************/

#ifndef _PRODUCT_FILE_SUPPORT_H_
#define _PRODUCT_FILE_SUPPORT_H_

#include "buildver.h"

#ifdef MULTI_FILE_SUPPORT
  // File format support tokens
  // Turned on support for DRF files in R6 - aszabo - may.09.03
  // Added explicit support for File Replace - aszabo - july.07.03
  #define FILE_SUPPORT_NONE       0x000
  #define FILE_SUPPORT_OPEN       0x001
  #define FILE_SUPPORT_SAVE       0x002
  #define FILE_SUPPORT_DEFAULT    0x004
  #define FILE_SUPPORT_EXPORT     0x008
  #define FILE_SUPPORT_IMPORT     0x010
  #define FILE_SUPPORT_MERGE      0x020
  #define FILE_SUPPORT_REPLACE    0x040
  #define FILE_SUPPORT_XREF       0x080
  #define FILE_SUPPORT_RPS_OPEN   0x100	// russom - 12/08/03 Render Presets support
  #define FILE_SUPPORT_RPS_SAVE   0x200	// russom - 12/08/03 Render Presets support

  #define FILE_SUPPORT_NATIVE     (FILE_SUPPORT_SAVE | FILE_SUPPORT_OPEN | \
                                  FILE_SUPPORT_MERGE | FILE_SUPPORT_REPLACE | \
                                  FILE_SUPPORT_XREF | FILE_SUPPORT_DEFAULT)

  // File formats
  #ifdef RENDER_VER // Viz Render =============================================

    #define FILE_FORMAT_VIZR	FILE_SUPPORT_NATIVE
    #define FILE_FORMAT_RPS		(FILE_SUPPORT_RPS_OPEN|FILE_SUPPORT_RPS_SAVE)

    #if defined(NDEBUG) && !defined(RENDER_DEV_VER) // Release ver of Viz Render
      #define FILE_FORMAT_MAX     FILE_SUPPORT_MERGE
    #else // Debug or Viz Render Developer
      #define FILE_FORMAT_MAX     (FILE_SUPPORT_SAVE | FILE_SUPPORT_OPEN | \
                                  FILE_SUPPORT_MERGE)
	#endif

  #elif defined(DESIGN_VER) // Viz ============================================

    #define FILE_FORMAT_MAX       FILE_SUPPORT_NATIVE
    #define FILE_FORMAT_VIZR      FILE_SUPPORT_OPEN
    #define FILE_FORMAT_RPS		  (FILE_SUPPORT_RPS_OPEN|FILE_SUPPORT_RPS_SAVE)
	#define FILE_FORMAT_CHR       (FILE_SUPPORT_SAVE|FILE_SUPPORT_OPEN|FILE_SUPPORT_MERGE|FILE_SUPPORT_REPLACE|FILE_SUPPORT_XREF)

  #elif defined(GAME_VER) // GMax Dev =========================================

    #if !defined(GAME_FREE_VER) 
      #define FILE_FORMAT_MAX     FILE_SUPPORT_READ
    #endif
      #define FILE_FORMAT_GMAX    FILE_SUPPORT_NATIVE
      #define FILE_FORMAT_PLASMA  FILE_SUPPORT_EXPORT

  #elif defined(WEBVERSION) // Plasma =========================================

    #define FILE_FORMAT_MAX       FILE_SUPPORT_READ
    #define FILE_FORMAT_PLASMA    FILE_SUPPORT_NATIVE

  #else // MAX ================================================================

    #define FILE_FORMAT_MAX       FILE_SUPPORT_NATIVE
    #define FILE_FORMAT_VIZR      FILE_SUPPORT_OPEN
    #define FILE_FORMAT_RPS		  (FILE_SUPPORT_RPS_OPEN|FILE_SUPPORT_RPS_SAVE)
	#define FILE_FORMAT_CHR       (FILE_SUPPORT_SAVE|FILE_SUPPORT_OPEN|FILE_SUPPORT_MERGE|FILE_SUPPORT_REPLACE|FILE_SUPPORT_XREF)

  #endif // Products

#endif // MULTI_FILE_SUPPORT

#endif // _PRODUCT_FILE_SUPPORT_H_
