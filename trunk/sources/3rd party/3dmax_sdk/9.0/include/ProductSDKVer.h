/*****************************************************************************
* FILE:					ProductSDKVer.h
* DESCRIPTION:	Defines symbols for changing the SDK ver per product
* CREATED BY:		Michael Russo
* HISTORY:	
* - 2003.oct.14 - AS - Pulled them out in their own file
******************************************************************************
* Copyright (c) 2003 Autodesk, All Rights Reserved.
******************************************************************************/

#ifndef _PRODUCT_SDK_VER_H_
#define _PRODUCT_SDK_VER_H_

#include "buildver.h"

#ifdef USE_PRODUCT_SDK_VER

  // Do not change SDK_ORDER_VER or SDK_RESERVED_VER without notifying the build team.
  #if defined(RENDER_VER) // Viz Render

    #define SDK_ORDER_VER     3
    #define SDK_RESERVED_VER  1

  #elif defined(GAME_VER) // GMax Dev

    #define SDK_ORDER_VER     1
    #define SDK_RESERVED_VER  1

  #elif defined(WEBVERSION) // Plasma

    #define SDK_ORDER_VER     1
    #define SDK_RESERVED_VER  3

  #else // Max, Viz
		
    #error "USE_PRODUCT_SDK_VER should be off for this product!"

  #endif // Product

	
  #define SDK_BASE_METHOD(a,b) virtual void sdkReservedMethod##a##b (void){return;}
  #if (SDK_RESERVED_VER > 2)

    #define SDK_RESERVED_METHOD(a) \
      SDK_BASE_METHOD(a,1); \
      SDK_BASE_METHOD(a,2); \
      SDK_BASE_METHOD(a,3);

  #elif (SDK_RESERVED_VER > 1)

    #define SDK_RESERVED_METHOD(a) \
      SDK_BASE_METHOD(a,1); \
      SDK_BASE_METHOD(a,2);

  #elif (SDK_RESERVED_VER > 0)

    #define SDK_RESERVED_METHOD(a) \
      SDK_BASE_METHOD(a,1);

  #else

    #define SDK_RESERVED_METHOD(a)

  #endif

#else // USE_PRODUCT_SDK_VER

  #define SDK_RESERVED_METHOD(a)
  #define SDK_ORDER_VER	0

#endif // USE_PRODUCT_SDK_VER

#endif // _PRODUCT_SDK_VER_H_

