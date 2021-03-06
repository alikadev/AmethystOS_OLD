#include "peripherals/gpio.h"
#include "utils.h"
#include "peripherals/aux.h"
#include "uart.h"

#define TXD 14
#define RXD 15

void uart_init(const u32 baudrate) {
    gpio_pin_set_func(TXD, GFAlt5);
    gpio_pin_set_func(RXD, GFAlt5);

    gpio_pin_enable(TXD);
    gpio_pin_enable(RXD);

    REGS_AUX->enables = 1;
    REGS_AUX->mu_control = 0;
    REGS_AUX->mu_ier = 0;
    REGS_AUX->mu_lcr = 3;
    REGS_AUX->mu_mcr = 0;

#if RPI_VERSION == 3
    REGS_AUX->mu_baud_rate = (u32) (250000000 / (8 * baudrate) - 1);
#endif

#if RPI_VERSION == 4
    REGS_AUX->mu_baud_rate = (u32) (500000000 / (8 * baudrate) - 1);
#endif

    REGS_AUX->mu_control = 3;

    uart_putc('\r');
    uart_putc('\n');
    uart_putc('\n');
}

char uart_getc() {
    while(!(REGS_AUX->mu_lsr & 1));

    return REGS_AUX->mu_io & 0xFF;
}

void uart_putc(char c) {
    while(!(REGS_AUX->mu_lsr & 0x20));

    REGS_AUX->mu_io = c;
}

void uart_puts(char *str) {
    while(*str) {
        if (*str == '\n') {
            uart_putc('\r');
        }

        uart_putc(*str);
        str++;
    }
}
