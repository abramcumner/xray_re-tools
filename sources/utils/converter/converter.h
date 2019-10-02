#ifndef __GNUC__
#pragma once
#endif
#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#ifdef _CONSOLE
#include "xr_cl_parser.h"
#endif

const char BUILD_DATE[] = __DATE__;
const char DEFAULT_FS_SPEC[] = "fsconverter.ltx";

#endif
