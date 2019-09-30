#ifndef _adskDebugCRTP_impl_h
#define _adskDebugCRTP_impl_h

namespace adsk {
	namespace Debug {
		// Forced to declare a virtual destructor by some compilers
		template< typename Derived, typename RequestType >
		CRTP_Debug<Derived,RequestType>::~CRTP_Debug() 
		{}

		template< typename Derived, typename RequestType >
		bool CRTP_Debug<Derived,RequestType>::debug(RequestType& request) const
		{ 
			return static_cast<const Derived*>(this)->Debug( static_cast<const Derived*>(this), request ); 
		}
	}
}

//=========================================================
//-
// Copyright  2012  Autodesk,  Inc.  All  rights  reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // _adskDebugCRTP_impl_h
