#pragma once

#include <string>

namespace xrFSL
{
	//-------------------------------------------
	// Type defs
	//-------------------------------------------
	typedef std::string String;
	typedef unsigned int uint32;
	typedef int int32;
	typedef unsigned short uint16;
	typedef short int16;
	typedef unsigned char uint8;
	typedef char int8;

	const int16 INT16_MIN = -32768;

	const int16 INT16_MAX = 0x7fff;
	const int32 INT32_MAX = 0x7fffffff;

	const uint8 UINT8_MAX = 0xff;
	const uint16 UINT16_MAX = 0xffff;
	const uint32 UINT32_MAX = 0xffffffff;
//	const uint64 UINT64_MAX = 0xffffffffffffffffull;
}