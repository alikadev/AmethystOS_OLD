/*
File: base.h
Copyright (C) 2020 Alikadev
This file contains the start memory adresse PBASE for RPI3 and 
RPI4. If the RPI_VERSION is not defined, it'll give an error
*/

#ifndef __PERIPHERICALS_BASE_H__
#define __PERIPHERICALS_BASE_H__

#if RPI_VERSION == 3
#define PBASE 0x3F000000

#elif RPI_VERSION == 4
#define PBASE 0xFE000000

#else
#define PBASE 0
#error RPI_VERSION NOT DEFINED

#endif

#endif