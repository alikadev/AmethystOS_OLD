#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

void *memcpy(u0 *dest, const u0 *src, u32 n);

#define GPU_CACHED_BASE		0x40000000
#define GPU_UNCACHED_BASE	0xC0000000
#define GPU_MEM_BASE	    GPU_UNCACHED_BASE

#define BUS_ADDRESS(addr)	(((addr) & ~0xC0000000) | GPU_MEM_BASE)

#endif