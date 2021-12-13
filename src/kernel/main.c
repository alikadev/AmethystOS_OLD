#include "uart.h"

void kernel_start(){
    write_string("\n\n --- Welcome in AmethystOS! ---\n");
    while(1){
        u8 c = read_char();
        if(c=='\r'){
            write_char('\n');
        }
        write_char(c);
    }
}