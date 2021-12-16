#include "common.h"
#include "string.h"
#include "io.h"

void kernel_main() {
    uart_puts("Rasperry PI AmethystOS Initializing...\n");

#if RPI_VERSION == 3
    uart_puts("\tRPI VERSION: Raspberry PI 3\n");
#endif

#if RPI_VERSION == 4
    uart_puts("\tRPI VERSION: Raspberry PI 4\n");
#endif
    while(1) {
        u16 c = listenKeyboard();
        if(c<NotDef){
            uart_putc(c);
            continue;
        }
        switch (c)
        {   
        case Left:
            uart_puts("[L]");
            continue;
        case Right:
            uart_puts("[R]");
            continue;
        case Up:
            uart_puts("[U]");
            continue;
        case Down:
            uart_puts("[D]");
            continue;
        
        default:
            uart_puts("[?]");
        }
    }
}
