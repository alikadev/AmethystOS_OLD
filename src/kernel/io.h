#pragma once

#include "common.h"
#include "uart.h"

typedef enum SpecialKeys {
    NotDef  = 256, // Key = 256 + kc ; 256 only is mod unset
    Left    = 512,
    Right   = 513,
    Up      = 514,
    Down    = 515,
    Esc     = 0x1B, // return 1<<15 + new char
    Del     = 0x7F
} SpecialKeys;

u16 listenKeyboard();
