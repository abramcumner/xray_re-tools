#ifndef _adskDebugCount_h
#define _adskDebugCount_h

#include <string>
#include <iostream>
#include <maya/adskCommon.h>

// ****************************************************************************
/*!
	\class adsk::Debug::Count
 	\brief Class implementing debug object counting operation

	This class converts a simple counter into a Debug request.
*/
namespace adsk
{
	namespace Debug
	{
		class METADATA_EXPORT Count
		{
		public:
			Count	();
			~Count	();
			Count&	operator=( size_t rhsCounter );
			size_t	staticObjectCount;
		};
	}; // namespace Debug
}; // namespace adsk

//**********************************************************************
/*!
	Helper macros for instantiating static object counters. Originally
	these were implemented as templates using the Curiously Recurring
	Template Pattern (CRTP) but the platform support for the required
	template functionality was inconsistent and insufficient for proper
	implementation so it was shelved until C++11 comes along.

	Usage of the per-object-type counter is to add this macro into the
	(probably public section of the) class to be counted:
   
   		DeclareObjectCounter( TheClassName );
		
	and then add this macro to the source file implementing the class:
	
		ImplementObjectCounter( TheClassName );

	then in each of the object's constructors add this line:

		ObjectCreated( TheClassName );
		
	and to the object's destructor add this line:
	
		ObjectDestroyed( TheClassName );

	Then the current number of object's of this type ever created is
	available as:
	
		TheClassName::objectsCreated()

	and the current number of the objects created and not yet destroyed
	is available as:

		TheClassName::objectsAlive()
*/
#define DeclareObjectCounter( CLASS )								\
	public:															\
		static size_t objectsCreated() { return _objectsCreated; }	\
		static size_t objectsAlive  () { return _objectsAlive; }	\
		static bool   Debug(const CLASS* , Debug::Count& q)			\
					  { q = _objectsAlive; return true; }			\
	private:														\
		static size_t _objectsCreated;								\
		static size_t _objectsAlive;

#define ImplementObjectCounter( CLASS )		\
		size_t CLASS::_objectsCreated = 0;	\
        size_t CLASS::_objectsAlive = 0;

#define ObjectCreated( CLASS )   CLASS::_objectsCreated++; CLASS::_objectsAlive++;
#define ObjectDestroyed( CLASS ) CLASS::_objectsAlive--;

//=========================================================
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // _adskDebugCount_h
