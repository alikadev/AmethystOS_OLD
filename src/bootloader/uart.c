#include "utils.h"
#include "periphericals/aux.h"
#include "periphericals/gpio.h"
#include "uart.h"

#define TXD 14
#define RXD 15

u0 uart_init(){
    gpioPinSetFunc(TXD, GFAlt5);
    gpioPinSetFunc(RXD, GFAlt5);

    gpioPinEnable(TXD);
    gpioPinEnable(RXD);

    REGS_AUX->enables = 1;
    REGS_AUX->mu_cntl = 0;
    REGS_AUX->mu_ier  = 0;
    REGS_AUX->mu_lcr  = 0b11;
    REGS_AUX->mu_mcr  = 0;

#if RPI_VERSION == 3
    REGS_AUX->mu_baud = 270; // = 115200 @ 250 Mhz
#elif RPI_VERSION == 4
    REGS_AUX->mu_baud = 541; // = 115200 @ 500 Mhz
#endif
    REGS_AUX->mu_cntl = 3;

    uart_putc('\r');
    uart_putc('\n');
    uart_putc('\n');
}


u8 uart_getc(){
    while(!(REGS_AUX->mu_lsr & 1));
    return REGS_AUX->mu_io & 0xFF;
}


u0 uart_putc(u8 c){
    while(!(REGS_AUX->mu_lsr & 0x20));
    REGS_AUX->mu_io = c;
}


u0 uart_puts(u8 *str){
    while(*str){
        if(*str == '\n')
            uart_putc('\r');
        uart_putc(*str);
        str++;
    }
}
