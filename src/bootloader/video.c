#include "mailbox.h"
#include "printf.h"
#include "timer.h"
#include "video.h"
#include "dma.h"
#include "mm.h"

typedef struct
{
    mailbox_Tag tag;
    u32 resX;
    u32 resY;
} mailbox_frameBufferSize;

typedef struct
{
    mailbox_Tag tag;
    u32 bpp;
} mailbox_frameBufferDepth;

typedef struct
{
    mailbox_Tag tag;
    u32 pitch;
} mailbox_frameBufferPitch;

typedef struct
{
    mailbox_Tag tag;
    u32 base;
    u32 screenSize;
} mailbox_frameBufferBuffer;

typedef struct
{
    mailbox_frameBufferSize res;
    mailbox_frameBufferSize vres;
    mailbox_frameBufferDepth depth;
    mailbox_frameBufferBuffer buff;
    mailbox_frameBufferPitch pitch;
} mailbox_frameBufferRequest;

static mailbox_frameBufferRequest fbReq;

static dma_Channel *dma;
static u8 *videoBuffer;
static u32 *bg32Buffer;
static u32 *bg8Buffer;

#define TEXT_COLOR 0xFFFFFFFF
#define BACK_COLOR 0xFF2D2D2D

#define MB (1024 * 1024)

#define BG32_MEM_LOCATION (LOW_MEMORY + (10 * MB))
#define BG8_MEM_LOCATION (BG32_MEM_LOCATION + (10 * MB))
#define VB_MEM_LOCATION (BG8_MEM_LOCATION + (4 * MB))

u0 video_init()
{
    dma = dma_openChannel(CT_NORMAL);
    videoBuffer = (u8 *)VB_MEM_LOCATION;

    printf("DMA Channel:  %d\n", dma->channel);
    printf("VIDEO Buffer: 0x%X\n", videoBuffer);

    bg32Buffer = (u32 *)BG32_MEM_LOCATION;
    bg8Buffer = (u32 *)BG8_MEM_LOCATION;

    for (u32 i = 0; i < (10 * MB); i++)
    {
        bg32Buffer[i] = BACK_COLOR;
    }
    for (u32 i = 0; i < (4 * MB); i++)
    {
        bg8Buffer[i] = 0x01010101;
    }
}

static bool useDma = false;

#define BUS_ADDR(x) (((u64)x | 0x40000000) & ~0xC0000000)

#define FRAMEBUFFER ((volatile u8 *)BUS_ADDR(fbReq.buff.base))
#define DMABUFFER ((volatile u8 *)videoBuffer)
#define DRAWBUFFER (useDma ? DMABUFFER : FRAMEBUFFER)

u0 video_setDma(bool b)
{
    useDma = b;
}

u0 doDma(u0 *dest, u0 *src, u32 total)
{
    u32 msStart = timerGetTicks() / 1000;

    u32 start = 0;
    while (total > 0)
    {
        int numBytes = total;
        if (numBytes > 0xFFFFFF)
            numBytes = 0xFFFFFF;

        dma_setupMemCpy(dma, dest + start, src + start, numBytes, 2);
        dma_start(dma);
        dma_wait(dma);

        start += numBytes;
        total -= numBytes;
    }

    u32 msEnd = timerGetTicks() / 1000;
}

u0 video_dma()
{
    doDma(FRAMEBUFFER, DMABUFFER, fbReq.buff.screenSize);
}
typedef struct
{
    mailbox_Tag tag;
    u32 offset;
    u32 numEntries;
    u32 entries[8];
} mailbox_SetPalette;

u0 video_setResolution(u32 resX, u32 resY, u32 bpp)
{

    fbReq.res.tag.id = RPI_FIRMWARE_FRAMEBUFFER_SET_PHYSICAL_WIDTH_HEIGHT;
    fbReq.res.tag.bufferSize = 8;
    fbReq.res.tag.valueLength = 8;
    fbReq.res.resX = resX;
    fbReq.res.resY = resY;

    fbReq.vres.tag.id = RPI_FIRMWARE_FRAMEBUFFER_SET_VIRTUAL_WIDTH_HEIGHT;
    fbReq.vres.tag.bufferSize = 8;
    fbReq.vres.tag.valueLength = 8;
    fbReq.vres.resX = resX;
    fbReq.vres.resY = resY;

    fbReq.depth.tag.id = RPI_FIRMWARE_FRAMEBUFFER_SET_DEPTH;
    fbReq.depth.tag.bufferSize = 4;
    fbReq.depth.tag.valueLength = 4;
    fbReq.depth.bpp = bpp;

    fbReq.buff.tag.id = RPI_FIRMWARE_FRAMEBUFFER_ALLOCATE;
    fbReq.buff.tag.bufferSize = 8;
    fbReq.buff.tag.valueLength = 4;
    fbReq.buff.base = 16;
    fbReq.buff.screenSize = 0;

    fbReq.pitch.tag.id = RPI_FIRMWARE_FRAMEBUFFER_GET_PITCH;
    fbReq.pitch.tag.bufferSize = 4;
    fbReq.pitch.tag.valueLength = 4;
    fbReq.pitch.pitch = 0;

    mailbox_SetPalette palette;
    palette.tag.id = RPI_FIRMWARE_FRAMEBUFFER_SET_PALETTE;
    palette.tag.bufferSize = 40;
    palette.tag.valueLength = 0;
    palette.offset = 0;
    palette.numEntries = 8;
    palette.entries[0] = 0;
    palette.entries[1] = BACK_COLOR;
    palette.entries[2] = TEXT_COLOR;
    palette.entries[3] = 0xFFFF0000;
    palette.entries[4] = 0xFF00FF00;
    palette.entries[5] = 0xFF0000FF;
    palette.entries[6] = 0x55555555;
    palette.entries[7] = 0xCCCCCCCC;

    // set resolution
    mailbox_process((mailbox_Tag *)&fbReq, sizeof(fbReq));

    printf("Allocated Buffer: 0x%X - %d - %d\n", fbReq.buff.base, fbReq.buff.screenSize, fbReq.depth.bpp);

    if (bpp == 8)
    {
        mailbox_process((mailbox_Tag *)&palette, sizeof(palette));
    }

    // draw some text showing resolution is...

    char res[64];

    u64 msStart = timerGetTicks() / 1000;

    video_clean();

    u64 msEnd = timerGetTicks() / 1000;
    u32 msBuff = msEnd - msStart;
    u64 msBeforeDraw = msEnd;
    if (useDma)
    {
        video_dma();
    }
    msEnd = timerGetTicks() / 1000;

    sprintf(res, "Resolution: %d x %d x %d\n", resX, resY, bpp);
    video_drawString(res, 20, 20);

    sprintf(res, "BG write took: %d ms\n", msBuff);
    video_drawString(res, 20, 40);

    sprintf(res, "DMA BG draw took: %d ms\n", msEnd - msBeforeDraw);
    video_drawString(res, 20, 60);

    sprintf(res, "Video drawing done!");
    video_drawString(res, 20, 80);

    if (useDma)
        video_dma();

    msEnd = timerGetTicks() / 1000;

    sprintf(res, "FRAME DRAW TIME: %d ms\n", msEnd - msStart);
    video_drawString(res, 20, 100);

    if (useDma)
        video_dma();
}
u0 video_drawPixel(u32 x, u32 y, u32 color)
{
    u32 pixelOffset = (x * (fbReq.depth.bpp >> 3)) + (y * fbReq.pitch.pitch);

    if (fbReq.depth.bpp == 32)
    {
        u32 *buff = (u32 *)DRAWBUFFER;
        buff[pixelOffset / 4] = color;
    }
    else if (fbReq.depth.bpp == 16)
    {
        u16 *buff = (u16 *)DRAWBUFFER;
        buff[pixelOffset / 2] = color & 0xFF;
    }
    else
    {
        DRAWBUFFER[pixelOffset++] = (color & 0xFF);
    }
}

u0 video_drawChar(char c, u32 posX, u32 posY)
{
    u32 text_color = TEXT_COLOR;
    u32 back_color = BACK_COLOR;

    if (fbReq.depth.bpp == 8)
    {
        text_color = 2;
        back_color = 1;
    }

    for (int y = 0; y < fontGetHeight(); y++)
    {
        for (int x = 0; x < fontGetWidth(); x++)
        {
            bool yes = fontGetPixel(c, x, y); //gets whether there is a pixel for the font at this pos...
            video_drawPixel(posX + x, posY + y, yes ? text_color : back_color);
        }
    }
}
u0 video_drawString(char *s, u32 posX, u32 posY)
{
    for (int i = 0; s[i] != 0; posX += (fontGetWidth() + 2), i++)
    {
        video_drawChar(s[i], posX, posY);
    }
}

u32 video_getResolutionX(){
    return fbReq.res.resX;
}
u32 video_getResolutionH(){
    return fbReq.res.resY;
}

u0 video_clean(){
    if (fbReq.depth.bpp == 32)
    {
        if (!useDma)
        {
            u32 *buff = (u32 *)FRAMEBUFFER;
            for (u32 j = 0; j < fbReq.buff.screenSize / 4; j++)
            {
                buff[j] = bg32Buffer[j];
            }
        }
        else
        {
            doDma(BUS_ADDR(videoBuffer), bg32Buffer, fbReq.buff.screenSize);
        }
    }
    else if (fbReq.depth.bpp == 8)
    {
        if (!useDma)
        {
            u32 *buff = (u32 *)FRAMEBUFFER;
            for (u32 j = 0; j < fbReq.buff.screenSize / 4; j++)
            {
                buff[j] = bg8Buffer[j];
            }
        }
        else
        {
            doDma(BUS_ADDR(videoBuffer), bg8Buffer, fbReq.buff.screenSize);
        }
    }
}