#pragma once

#include "common.h"

u0  delay(u64 ticks);
u0  put32(u64 addr, u32 value);
u32 get32(u64 addr);