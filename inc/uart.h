#ifndef __UART_H__
#define __UART_H__

#include "common.h"

u0 uart_init(const u32 baudrate);
char uart_getc();
u0 uart_putc(char c);
u0 uart_puts(char *str);
u0 uart_cmd(char c);

#endif