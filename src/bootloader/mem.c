#include "mem.h"

void *memcpy(u0 *dest, const u0 *src, u32 n){
    u8 *byteDest = (u8 *)dest;
    u8 *byteSrc = (u8 *)src;

    for (u32 i = 0; i < n; i++) {
        byteDest[i] = byteSrc[i];
    }

    return dest;
    
}

