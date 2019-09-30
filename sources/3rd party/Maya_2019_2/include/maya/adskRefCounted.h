#ifndef adskRefCounted_h
#define adskRefCounted_h

#include <maya/adskCommon.h>

// ****************************************************************************
/*!
	\class adsk::RefCounted
 	\brief Class implementing standard reference counting

	Simple abstract reference counting class, derive from it and implement
	the destructor, copy constructor, and assignment operator. Maintains a
	reference count and indicates deletion is safe when the count drops to
	zero.
*/

namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};

	class METADATA_EXPORT RefCounted
	{
		public:
			void	ref			() const;
			void	unref		() const;
			int		refCount	() const;
			bool	isShared	() const;

			//-----------------------------------------------------------
			// Debugging support
			static	bool Debug	( const RefCounted* me, Debug::Print& request );
			static	bool Debug	( const RefCounted* me, Debug::Footprint& request );

		protected:
			RefCounted	();
			RefCounted	(const RefCounted&);
			RefCounted&	operator= (const RefCounted&);

			virtual ~RefCounted();

		private:
			class Impl;
			Impl * fPimpl;
	};

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
#endif // adskRefCounted_h
