#ifndef _BAIO_H
#define _BAIO_H

#include <common.h>
#include <mm.h>

#define BAIO_DEBUG 0b1
#define BAIO_VIDEO 0x10

u0 baio_play(char mode);
#endif