#ifndef _adskDebugCRTP_h
#define _adskDebugCRTP_h

#include <maya/adskCommon.h>
#include <maya/adskDebugCount.h>

// ****************************************************************************
/*! \file
	Collection of debugging templates for automatic instantiation of
	debug methods.
*/

namespace adsk {
	namespace Debug {

//======================================================================
//
/*!
	This template defines virtual methods for calling the statically
	defined Debug methods within a hierarchy. A template was used since
	the patterns for the Debug calls are all the same.

	To add a handler for a Debug message you would first derive the class
	to debug off of this template, once per message type that will be
	handled. e.g. for messages "MyRequest" and "YourRequest" the class
	definition would look like this:

		class MyClass : public CRTP_Debug<MyClass, MyRequest>
					  , public CRTP_Debug<MyClass, YourRequest>

	Static debug methods are then defined and in the source file they
	will be implemented using this syntax:

		bool MyClass::Debug( const MyClass*, MyRequest& ) { ... }
		bool MyClass::Debug( const MyClass*, YourRequest& ) { ... }
		
	After, you will need to include adskDebugCRTPImpl.h in the compilation unit
	when you are defining your class (eg MyClass.cpp), so that the templated
	code gets instantiated once.
*/
template <typename Derived, typename RequestType>
class CRTP_Debug
{
public:
	// Forced to declare a virtual destructor by some compilers
	virtual ~CRTP_Debug<Derived,RequestType>();

	/*! \fn bool debug(RequestType& request) const
		\brief Gather this object's counting information
		\param[in] request Debug object accepting the count request
		\return true if the debugging succeeded
	*/
	virtual bool	debug	(RequestType& request)			const;
};

	} // namespace Debug
} // namespace adsk

//=========================================================
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // _adskDebugCRTP_h
