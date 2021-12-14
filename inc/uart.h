#pragma once
#include "common.h"

u0 uart_init();
u8 uart_getc();
u0 uart_putc(u8 c);
u0 uart_puts(u8 *str);
