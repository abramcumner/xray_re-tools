#ifndef __CRC32_H__
#define __CRC32_H__

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

unsigned int crc32(const void *buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif
