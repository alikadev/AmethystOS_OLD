#ifndef __UART_H__
#define __UART_H__

void uart_init(const u32 baudrate);
char uart_getc();
void uart_putc(char c);
void uart_puts(char *str);

#endif