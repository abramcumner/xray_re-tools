#pragma once

#include <limits>

namespace xrFSL
{
	template<typename T> class NumericLimits: public std::numeric_limits<T> 
	{
	public:
		static T real_min();
	};

	template<> inline float NumericLimits<float>::real_min() { return -FLT_MAX; }
	template<> inline double NumericLimits<double>::real_min() { return -DBL_MAX; }

	/**
	 *	
	 */
	class Math
	{
	public:
		/**
		*  Round (to nearest) a floating point number to an integer.
		*/
		static inline int Round( float f );

		/**
		* Converts to integer equal to or less than.	
		*/
		static inline int Floor( float f );
	};

	//====================================================
	// Inline impl
	//====================================================
	inline int Math::Round( float f )
	{
		__asm cvtss2si eax,[f]
		// return value in eax.
	}

	inline int Math::Floor( float f )
	{
		const DWORD mxcsr_floor = 0x00003f80;
		const DWORD mxcsr_default = 0x00001f80;

		__asm ldmxcsr [mxcsr_floor]		// Round toward -infinity.
		__asm cvtss2si eax,[f]
		__asm ldmxcsr [mxcsr_default]	// Round to nearest
		// return value in eax.
	}
}