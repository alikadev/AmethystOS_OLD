#include "common.h"
#include "string.h"
#include "../inc/printf.h"
#include "uart.h"
#include "irq.h"
#include "timer.h"
#include "mailbox.h"
#include "video.h"
#include "mem.h"
#include "os/baio.h"

#define BAUDRATE 115200

#define CHAR_SIZE_W (fontGetWidth() + 2)
#define CHAR_SIZE_H (fontGetHeight())

u32 screenPosX = 0;
u32 screenPosY = 0;

void putc(void *p, char c)
{
    if (c == '\n'){
        uart_putc('\r');
    }
    else if (c == '\t')
    {
        uart_puts("    ");
        return;
    }
    uart_putc(c);
}

u0 video_putc(void *p, char c){
    if (c == '\n'){
        uart_putc('\r');
        screenPosX = 0;
        screenPosY+=CHAR_SIZE_H;
        if(video_getResolutionH() < screenPosY+CHAR_SIZE_H){
            screenPosY = 0;
            video_clean();
        }
    }
    else if (c == '\t')
    {
        uart_puts("    ");
        screenPosY += 4*CHAR_SIZE_W;
        return;
    }
    uart_putc(c);
    video_drawChar(c, screenPosX, screenPosY);
    screenPosX += CHAR_SIZE_W;
}

u0 uart_cmd(char c)
{
    printf("\n\nRecv uart cmd: %c\n", c);

    if (c == 'T')
    {
        u32 maxTemp = 0;
        mailbox_genericCommand(RPI_FIRMWARE_GET_MAX_TEMPERATURE, 0, &maxTemp);
        u32 curtemp = 0;
        mailbox_genericCommand(RPI_FIRMWARE_GET_TEMPERATURE, 0, &curtemp);
        printf("    curTemp: %d\n    max: %d\n\n",
               curtemp,
               maxTemp);
    }
    else if (c == '?')
    {
        printf("\nusable uart commands:\n");
        printf("    ?  Display this\n");
        printf("    V  Get system version\n");
        printf("    T  Get system temerature\n");
    }
    else if (c == 'V')
    {
        printf("\nAmethystOS Version %d.%d.%d\n", VER_MAJOR, VER_MINOR, VER_PATCH);
    }
    else
    {
        printf("\nUnrecognized uart commands '%c'...\nType '?' to display the uart commands\n", c);
    }
}

u32 get_el();

void kernel_main()
{
    // init uart
    uart_init(BAUDRATE);
    init_printf(0, putc);
    printf("\r\n\r\n");
    printf("Rasperry PI AmethystOS Initializing...\n");

    // init
    irq_intiVectors();
    enableInterruptController();
    irq_enable();
    timerInit();

    printf("\tRPI VERSION: Raspberry PI %d\n", RPI_VERSION);

    printf("\nException Level: %d\n", get_el());

    printf("\nMAILBOX:\n");
    printf("CORE CLOCK: %d\n", mailbox_clockRate(CORE));
    printf("EMMC CLOCK: %d\n", mailbox_clockRate(EMMC));
    printf("UART CLOCK: %d\n", mailbox_clockRate(UART));
    printf("ARM  CLOCK: %d\n", mailbox_clockRate(ARM));

    printf("\nI2C POWER STATE:\n");

    for (u8 i = 0; i < 3; i++)
    {
        bool on = mailbox_powerCheck(i);

        printf("POWER DOMAIN STATUS FOR %d = %d\n", i, on);
    }

#if INIT_MMU == 1
    printf("\nMMU - DMA initialized!");
#endif
    printf("\nTry to allocate/free memory...\n");
    u0 *p1 = getFreePages(10);
    u0 *p2 = getFreePages(4);
    freeMemory(p1);
    freeMemory(p2);
    u0 *p3 = allocateMemory(sizeof(arm_irq_regs));
    freeMemory(p3);
    printf("End allocate/free memory...\n");


    // init video
    printf("\nVideo - DMA initializing...\n");
    video_init();
    video_setDma(true);
    video_setResolution(1440, 960, 32);
    printf("Video - DMA initialized!\n\n");

    // play
    baio_play(BAIO_VIDEO | BAIO_DEBUG);
}
