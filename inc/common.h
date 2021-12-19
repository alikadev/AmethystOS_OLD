#pragma once

#include <stdint.h>

#define u0  void
#define i8  signed char
#define u8  unsigned char
#define i16 signed short int
#define u16 unsigned short int
#define i32 signed int
#define u32 unsigned int
#define i64 signed long long int
#define u64 unsigned long long int

typedef volatile u32 reg32;

typedef char  bool;
#define true  1
#define false 0

#ifndef RPI_VERSION
#define RPI_VERSION -1
#endif
