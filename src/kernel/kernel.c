#include "common.h"
#include "string.h"
#include "../inc/printf.h"
#include "uart.h"
#include "irq.h"
#include "timer.h"
#include "mailbox.h"

#define BAUDRATE 115200

void putc(void *p, char c){
    if(c=='\n')
        uart_putc('\r');
    else if(c=='\t'){
        uart_puts("    ");
        return;
    }
    uart_putc(c);
}

u32 get_el();

void kernel_main() {
    // init uart
    uart_init(BAUDRATE);
    init_printf(0, putc);
    printf("\r\r\r\r\r\r\r\r\r\r\r\r\r\n");
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

    for(u8 i=0; i<3; i++){
        bool on = mailbox_powerCheck(i);

        printf("POWER DOMAIN STATUS FOR %d = %d\n", i, on);
    }

    u32 maxTemp = 0;
    mailbox_genericCommand(RPI_FIRMWARE_GET_MAX_TEMPERATURE, 0, &maxTemp);


    while(1) {
        u32 curtemp = 0;
        mailbox_genericCommand(RPI_FIRMWARE_GET_TEMPERATURE, 0, &curtemp);
        printf("curTemp: %d, max: %d\n", 
            curtemp, 
            maxTemp);
        timerSleep(5000);
    }
}
