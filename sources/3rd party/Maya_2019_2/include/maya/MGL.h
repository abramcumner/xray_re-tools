#ifndef __MGL_h_
#define __MGL_h_
//-
// Copyright 2015 Autodesk, Inc. All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//+
//
// FILE:    MGL.h
//
// ****************************************************************************
//
// DESCRIPTION (MGL)
//
//	This file include the proper OpenGL and OpenGL utility (glu)
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>

#if defined(OSMac_)
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	//class NSOpenGLContext;
	//! \brief GL context type (OS X 64-bit)
	typedef	void*	MGLContext;

#elif defined (LINUX)
	#include <GL/glx.h>
	#include <GL/glu.h>
  	//! \brief GL context type (Linux)
  	typedef	GLXContext	MGLContext;

#elif defined (_WIN32)
	#ifndef APIENTRY
		#define MAYA_APIENTRY_DEFINED
		#define APIENTRY __stdcall
	#endif
	#ifndef CALLBACK
		#define MAYA_CALLBACK_DEFINED
		#define CALLBACK __stdcall
	#endif
	#ifndef WINGDIAPI
		#define MAYA_WINGDIAPI_DEFINED
		#define WINGDIAPI __declspec(dllimport)
	#endif

	#include <gl/Gl.h>
	#include <gl/Glu.h>

	#ifdef MAYA_APIENTRY_DEFINED
		#undef MAYA_APIENTRY_DEFINED
		#undef APIENTRY
	#endif
	#ifdef MAYA_CALLBACK_DEFINED
		#undef MAYA_CALLBACK_DEFINED
		#undef CALLBACK
	#endif
	#ifdef MAYA_WINGDIAPI_DEFINED
		#undef MAYA_WINGDIAPI_DEFINED
		#undef WINGDIAPI
	#endif

    //! \brief GL context type (Microsoft Windows)
    typedef	HGLRC MGLContext;
#else
	#error Unknown OS
#endif //OSMac_


#endif /* __cplusplus */
#endif /* __MGL_h_ */
