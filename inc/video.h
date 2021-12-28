#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "common.h"

u0 video_init();
u0 video_setDma(bool b);
u0 video_dma();
u0 video_setResolution(u32 resX, u32 resY, u32 bpp);
u32 video_getResolutionX();
u32 video_getResolutionH();
u0 video_clean();
u0 video_drawPixel(u32 x, u32 y, u32 color);
u0 video_drawChar(char c, u32 posX, u32 posY);
u0 video_drawString(char *s, u32 posX, u32 posY);

// fontData.c
u32 fontGetHeight();
u32 fontGetWidth();
bool fontGetPixel(char ch, u32 x, u32 y);


#endif