#ifndef _MRANDOM
#define _MRANDOM
//-
// Copyright 2016 Autodesk, Inc.  All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//+
//
// CLASS:    MRandom
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MTypes.h>

// ****************************************************************************
// CLASS DECLARATION (MRandom)

//! \ingroup OpenMaya
//! \brief Methods for generating random numbers.
/*!
  
    The MRandom class implements several methods to generate sequences of 
	random numbers.

	For a given fixed seed, input i produces the i'th random number of a
	2^64-long pseudo-random sequence.  Different seeds will give different 
	sequences, but nearby seeds may on rare occasion exhibit statistical 
	correlations; it is therefore best to use hashed seeds.

	All methods are thread-safe, values can be requested in any order, and
	sequences are repeatable.
*/

namespace MRandom
{

	OPENMAYA_EXPORT unsigned long long Rand( unsigned long long i, unsigned long long seed);

	OPENMAYA_EXPORT double Rand_d( unsigned long long i, unsigned long long seed);

	OPENMAYA_EXPORT float Rand_f( unsigned long long i, unsigned long long seed);

	OPENMAYA_EXPORT float Rand_f( unsigned long long i, unsigned long long seed, float rangeMin, float rangeMax);

	OPENMAYA_EXPORT void Rand_2f( float output[2], unsigned long long i, unsigned long long seed);

	OPENMAYA_EXPORT void Rand_3f( float output[3], unsigned long long i, unsigned long long seed);

}


#endif /* __cplusplus */
#endif /* _MRANDOM */
