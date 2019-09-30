#ifndef adskCheckpointed_h
#define adskCheckpointed_h

#include <maya/adskCommon.h>

//! Static versions of counters which change when objects are created or destroyed.
//! Use this macro inside a class definition to enable this static tracking.
//! A bit ugly to use macros for this thanks to the nature of C++.
#define DeclareCheckpointed()														\
	public:																			\
		virtual Checkpoint classCheckpoint			  ();							\
		virtual bool	   classChangedSinceCheckpoint( const Checkpoint& check );	\
		virtual Checkpoint classChanged				  ();							\
		static Checkpoint  sClassCheckpoint			  ();							\
		static bool		   sClassChangedSinceCheckpoint( const Checkpoint& check );	\
		static Checkpoint  sClassChanged				  ();						\
	private:																		\
		static Checkpoint sfCheckpoint

#define ImplementCheckpointed( CLASS )													\
		Checkpoint	CLASS :: sfCheckpoint = globalCheckpoint();							\
		bool		CLASS :: sClassChangedSinceCheckpoint ( const Checkpoint& check )	\
					  { return CLASS :: sfCheckpoint > check; }							\
		Checkpoint	CLASS :: sClassChanged		 ()										\
					  { return (CLASS :: sfCheckpoint = globalChange()); }				\
		Checkpoint	CLASS :: sClassCheckpoint	 () { return CLASS :: sfCheckpoint; }	\
		bool		CLASS :: classChangedSinceCheckpoint ( const Checkpoint& check )	\
					  { return CLASS :: sClassChangedSinceCheckpoint( check ); }		\
		Checkpoint	CLASS :: classChanged		 ()										\
					  { return CLASS :: sClassChanged(); }								\
		Checkpoint	CLASS :: classCheckpoint	 () { return CLASS :: sClassCheckpoint(); }

#define CheckpointCreate( CLASS ) fCheckpoint = CLASS :: classChanged()
#define CheckpointDestroy( CLASS ) fCheckpoint = CLASS :: classChanged()


namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};

	typedef uint64_t Checkpoint; //!< Make it big enough to handle tons and tons of edits

// ****************************************************************************
/*!
	\brief Class implementing ability to keep track of when objects are changed

	Often the question arises "has this object changed since I did X?". Using
	this class gives you the ability to create a unique monotonic ID to identify
	how recently something has changed, usually accompanied by the queryer
	remembering a checkpoint value to compare against.

	This class must be a mixin to the class being tracked. The important
	thing is to insert this call at every point that object is changed:

	\code
		objectChanged();
	\endcode

	Since the virtual function table isn't correct until the top level class
	constructor is called unfortunately you also have to insert checkpoint
	updates into the constructors and destructor of your class. The macros

	\code
		CheckpointCreate( MyClass );
		CheckpointDestroy( MyClass );
	\endcode

	will do that for you.

	Object changes will also flag the class as changed but if any other
	changes happen which apply to the class as a whole they can be tagged with:

	\code
		classChanged();
	\endcode

	Example 1: An external object handler wishes to know when any object of
	class type MyObject was changed.

	\code
		class MyObject : public adsk::Checkpointed {
		...
		};
		MyObject::MyObject()
			{ CheckpointCreate(MyObject); ... }
		MyObject::~MyObject()
			{ CheckpointDestroy(MyObject); ... }
		void MyObject::anyMethodChangingAnObject()
			{ objectChanged(); doTheChanges(); }
	
		... in the handler ...
		adsk::Checkpoint checkpoint = MyObject::sClassCheckpoint();
		doHandlerStuff();
		if( MyObject::sClassChangedSinceCheckpoint( checkpoint ) )
			doHandlerChangedStuff();
	\endcode

	Note that since construction and destruction of objects count as changes
	to "the set of" MyObject they also get the edit flag updated.

	Example 2: An external object handler wishes to know when a specific object
	has been changed.

	\code
		class MyObject : public adsk::Checkpointed {
		...
		};
		MyObject::MyObject()
			{ CheckpointCreate(MyObject); ... }
		MyObject::~MyObject()
			{ CheckpointDestroy(MyObject); ... }
		void MyObject::anyMethodChangingAnObject()
			{ objectChanged(); doTheChanges(); }
	
		... in the handler ...
		MyObject mObj;
		adsk::Checkpoint checkpoint = myObj.checkpoint();
		doObjectStuff( mObj );
		if( mObj.changedSinceCheckpoint( checkpoint ) )
			doObjectChangedStuff();
	\endcode
*/
	class METADATA_EXPORT Checkpointed
	{
			DeclareCheckpointed();

		protected:
			//============================================================
			// Call this within your object any time its contents change
			Checkpoint	objectChanged			();

		public:
			//============================================================
			// Call these from outside to check when an object was changed
			// and compare against a known checkpoint location.
			Checkpoint	checkpoint				()								const;
			bool		changedSinceCheckpoint	( const Checkpoint& check )		const;

			//============================================================
			// Usual construction/destruction.
			Checkpointed			();
			virtual ~Checkpointed	();
			Checkpointed			(const Checkpointed& rhs);
			//! Assignment operator and copy constructor are just to allow
			//! objects with counters embedded or derived to copy properly.
			//! \param[in] rhs Counter to copy
			Checkpointed& operator=	(const Checkpointed& rhs);

			//============================================================
			// Global counter support. This is used to maintain a global
			// counter across all types so that edit checkpoints can be
			// compared between different types.
			static Checkpoint	globalCheckpoint();
			static Checkpoint	globalChange	();

			//============================================================
			// Debugging support
			static	bool Debug	( const Checkpointed* me, Debug::Print& request );
			static	bool Debug	( const Checkpointed* me, Debug::Footprint& request );

		protected:
			//============================================================
			static Checkpoint	sfGlobalCheckpoint; //!< Global monotonic checkpoint
			Checkpoint			fCheckpoint;		//!< ID updating when the object changes
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
#endif // adskCheckpointed_h
