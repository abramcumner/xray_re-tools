#ifndef _adskFootprint_h
#define _adskFootprint_h

#include <maya/adskCommon.h>

// ****************************************************************************
/*!
	\class adsk::Debug::Footprint
 	\brief Class implementing the gathering of data footprint information

	It's always useful to know how much space your data occupies and how
	spread out it is. This class provides a simple method to gather that
	information for reporting and analysis. It's set up to accumulate
	data of interest recursively from a set of root objects.

	This base implementation just collects total sizes. More complex
	implementations could gather size bucketing information, avoid
	collecting duplicates, track memory fragmentation, etc.
	
	By putting it into a hierarchy you can choose how efficient the
	collection will be by the footprint object type you instantiate.
*/

namespace adsk
{
	namespace Debug
	{
		class Print;

		class METADATA_EXPORT Footprint
		{
		public:
			Footprint						();
			virtual ~Footprint				();

			size_t			totalSize		()							const;
			size_t			totalFragments	()							const;
			virtual	void	clear			();
			//! Use to add members of an object (e.g. strlen of a char*)
			virtual void	addMember		( const void* where, size_t howBig );
			//! Use to add real objects (e.g. "this" in a class)
			virtual	void	addObject		( const void* where, size_t howBig );

			static	bool	Debug			( const Footprint* me, Print& request );
			static	bool	Debug			( const Footprint* me, Footprint& request );

		private:
			size_t	fTotalBytes;		//! Count of bytes occupied by objects
			size_t	fTotalFragments;	//! Count of objects in distinct locations
		public:
			// Do not access directly, use the ParentFootprint macro below
			bool	fSkipObject;
					/*! True if the object itself should be skipped.
						Used for taking footprint of a class hierarchy - the
						derived classes get the true object size, the base
						classes should not add their size as well since they
						are a (possibly smaller) subset of the same object.
					*/
};

	}; // namespace Debug

}; // namespace adsk

//----------------------------------------------------------------------
/*! Helper macro to allow a class hierarchy to easily account for the
	fact that derived classes occupy the same space as their base class.
	Any objects in a class hierarchy can add up the entire hierarchy cost
	using this simple sequence:
		void DerivedClass::Debug(DerivedClass* me, Footprint& foot)
		{
			ParentFootprint( BaseClass, me, foot );
			foot.addOwner( me, sizeof(*me) );
		}
	
	\param[in] BASECLASS Name of the base class
	\param[in] ME Pointer to the object on which to collect footprint data
	\param[in] FOOTPRINT Footprint object collecting the data
*/
#define ParentFootprint( BASECLASS, ME, FOOTPRINT )	\
	{ bool _oldSkip = FOOTPRINT .fSkipObject;		\
	  FOOTPRINT .fSkipObject = true;				\
	  BASECLASS :: Debug( ME, FOOTPRINT );			\
	  FOOTPRINT .fSkipObject = _oldSkip;			\
	}

//=========================================================
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // _adskFootprint_h
