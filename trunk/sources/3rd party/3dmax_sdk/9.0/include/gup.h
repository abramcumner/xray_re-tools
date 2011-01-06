//-----------------------------------------------------------------------------
// ----------------
// File ....: gup.h
// ----------------
// Author...: Gus Grubba
// Date ....: September 1998
//
// History .: Sep, 30 1998 - Started
//
//-----------------------------------------------------------------------------
		
#ifndef	GUP_H_DEFINED
#define	GUP_H_DEFINED

#include "maxheap.h"
#include <gupapi.h>

//-- Forward References

class GUP;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-- Plug-Ins Handler
//
	
class GUPHandler : public InterfaceServer {

		//-- DLL Handler ----------------------------------
		
		ClassDesc*	cd;
		Class_ID	id;
		GUP*		instance;
		
	public:

								GUPHandler		( );

		GUPExport	void		SetCD			( ClassDesc *dll )	{ cd = dll;	}
		GUPExport	ClassDesc*	GetCD			( )					{ return cd;}
		GUPExport	void		SetID			( Class_ID cid )	{ id = cid;	}
		GUPExport	Class_ID	GetID			( )					{ return id;}
		GUPExport	void		SetInstance		( GUP *ins )		{ instance = ins;  }
		GUPExport	GUP*		GetInstance		( )					{ return instance; }

		GUPExport	GUP*		Gup				( )					{ return instance; }

};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-- List of Loaded Plug-Ins
//
	
class GUPList: public Tab<GUPHandler> {

	public:

		GUPExport			GUPList				( )	{ ; }
		GUPExport	int		FindGup				( const Class_ID id );
		GUPExport	GUP*	CreateInstance		( const Class_ID id );

};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-- Global Utility Plug-In Class
//

class GUP : public InterfaceServer {
	
	public:
	
		GUPExport	GUP								( );
		GUPExport virtual	~GUP							( );

		//-----------------------------------------------------------
		//-- Plug-In Service Methods
		
		GUPExport virtual	HINSTANCE		MaxInst			( );
		GUPExport virtual	HWND			MaxWnd			( );
		GUPExport virtual	DllDir*			MaxDllDir		( );
		GUPExport virtual	Interface*		Max				( );
		GUPExport virtual	BitmapManager*	Bmi				( );
		GUPExport virtual	int				EnumTree		( ITreeEnumProc *proc );

		//-----------------------------------------------------------
		//-- Plug-In Service Methods (Scripting)
		
		GUPExport virtual	bool			ExecuteStringScript	( TCHAR *string );
		GUPExport virtual	bool			ExecuteFileScript	( TCHAR *file );

		//-----------------------------------------------------------
		//-----------------------------------------------------------
		//-- Plug-In Implementation Methods
		
		//-----------------------------------------------------------
		// Start() is called at boot time. If the plug-in
		// desires to remain loaded, it returns GUPRESULT_KEEP. If
		// it returns GUPRESULT_NOKEEP, it will be discarded. If it
		// returns GUPRESULT_ABORT MAX will be shut down.

		#define GUPRESULT_KEEP		0x00
		#define GUPRESULT_NOKEEP	0x01
		#define GUPRESULT_ABORT		0x03

		GUPExport virtual	DWORD			Start		( ) = 0;
		
		//-------------------------------------------------
		// Stop is called prior to discarding the plug-in
		// for persistent plug-ins (i.e. only those that 
		// returned GUPRESULT_KEEP for Start() above).
		
		GUPExport virtual	void			Stop		( ) = 0;

		//-------------------------------------------------
		// Control is an entry point for external access to
		// GUP plug-ins. For instance, Utility plugins can
		// invoke their GUP plugin counterpart and have 
		// direct access to them.
		//
      // SR NOTE64: This can return pointer-sized data, so 
      //            it has to be a DWORD_PTR.
		
		GUPExport virtual	DWORD_PTR		Control		( DWORD parameter ) { return 0;}

		//-------------------------------------------------
		// Optional Methods for saving the plug-in state
		// within a scene file.

		GUPExport virtual	IOResult		Save		( ISave *isave );
		GUPExport virtual	IOResult		Load		( ILoad *iload );
		
		// RK: 06/28/00, added it to support static instances
		GUPExport virtual	void			DeleteThis	( ) { }
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-- Main GUP Manager Class
//
//

class GUPManager: public MaxHeapOperators    {
	
		GUPInterface*	iface;

		int				InitList		( );
		bool			listed;
		
	public:
	
		GUPExport					GUPManager		( GUPInterface *i );
		GUPExport					~GUPManager		( );
		
		GUPList		gupList;
		
		GUPExport	bool			Ready			( );
		GUPExport	bool			Init			( );
		GUPExport	GUPInterface*	Interface		( ) { return iface; }
		GUPExport	IOResult		Save			( ISave *isave );
		GUPExport	IOResult		Load			( ILoad *iload );

};

//-----------------------------------------------------------------------------
//-- Exported
//

extern GUPExport	void			OpenGUP			(  GUPInterface *i );
extern GUPExport	void			CloseGUP		(  );
extern GUPExport	GUPManager*		gupManager; 
extern GUPExport	GUP*			OpenGupPlugIn	( const Class_ID id); 

//-----------------------------------------------------------------------------
//-- Cleanup

#endif	GUP_H_DEFINED

//-- EOF: gup.h ---------------------------------------------------------------
