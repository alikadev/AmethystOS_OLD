#include "io.h"

u16 listenKeyboard(){

    char c=uart_getc();
    if(c>=' ' && c<=Del)
        return c;
    if(c=='\r')
        return '\n';
        
    // just not def
    if(c!=Esc){
        return NotDef+c;
    }

    //is not mode
    c=uart_getc();
    if(c!=0x5B){
        return (1 << 15) + c;
    }

    c=uart_getc();
    switch (c){
    case 'A':
        return Up;

    case 'B':
        return Down;

    case 'C':
        return Right;

    case 'D':
        return Left;
        
    default:
        return NotDef;
    }
}