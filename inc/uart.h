#pragma once

void uart_init(const u32 baudrate);
char uart_getc();
void uart_putc(char c);
void uart_puts(char *str);
