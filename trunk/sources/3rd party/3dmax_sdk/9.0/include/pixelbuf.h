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
// FILE:        pixelbuf.h
// DESCRIPTION: Pixel Buffer Classes
//              These templated classes let you set up a buffer for pixels
//              that will automatically clean itself up when it goes out of 
//              scope.
// AUTHOR:      Tom Hudson
// HISTORY:     Dec. 09 1995 - Started file
//**************************************************************************/

#ifndef __PIXBUF_H__

#define __PIXBUF_H__

#include "maxheap.h"
#include "maxheapdirect.h"

// Handy-dandy pixel buffer classes:

template <class T> class PixelBufT: public MaxHeapOperators {
private:
     T *buf;
     int width;
public:
     inline               PixelBufT(int width) { buf = (T *)MAX_calloc(width,sizeof(T)); this->width=width; };
     inline               ~PixelBufT() { if(buf) MAX_free(buf); };
     inline   T*          Ptr() { return buf; };
	 inline   T&          operator[](int i) { return buf[i]; }
           int            Fill(int start, int count, T color) {
                          int ix,jx=start+count;
                          if(jx > width) // MAB - 07/15/03 - changed from >=
                             return 0;
                          for(ix=start; ix<jx; buf[ix++]=color);
                          return 1;
                          };
     };

typedef PixelBufT<UBYTE> PixelBuf8;
typedef PixelBufT<USHORT> PixelBuf16;
typedef PixelBufT<BMM_Color_24> PixelBuf24;
typedef PixelBufT<BMM_Color_32> PixelBuf32;
typedef PixelBufT<BMM_Color_48> PixelBuf48;
typedef PixelBufT<BMM_Color_64> PixelBuf64;
typedef PixelBufT<BMM_Color_fl> PixelBufFloat;

typedef PixelBufT<BMM_Color_64> PixelBuf;

#endif // __PIXBUF_H__
