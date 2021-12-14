#include "uart.h"
#include "common.h"

u0 kernel_entry(){
    uart_init();

    uart_puts((u8*)"\n\n --- Welcome in AmethystOS! ---\n");
    while(1){
        u8 c = uart_getc();
        if(c == '\r'){
            uart_putc('\n');
        }
        uart_putc(c);
    }
}
