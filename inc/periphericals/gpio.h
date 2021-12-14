#pragma once

#include "base.h"
#include "common.h"

struct GpioPinData {
    r32 reserved;
    r32 data[2];
};

struct GpioRegs {
    r32 func_select[6];
    struct GpioPinData outputSet;
    struct GpioPinData outputClear;
    struct GpioPinData level;
    struct GpioPinData evDetectStatus;
    struct GpioPinData reDetectEnable;
    struct GpioPinData feDetectEnable;
    struct GpioPinData hiDetectEnable;
    struct GpioPinData loDetectEnable;
    struct GpioPinData asyncReDetect;
    struct GpioPinData asyncFeDetect;
    r32 reserved;
    r32 pupdEnable;
    r32 pupdEnableClock[2];
};

typedef enum GpioFunc {
    GFInput  = 0,
    GFOutput = 1,
    GFAlt0   = 4,
    GFAlt1   = 5,
    GFAlt2   = 6,
    GFAlt3   = 7,
    GFAlt4   = 3,
    GFAlt5   = 2
} GpioFunc;

void gpioPinSetFunc(u8 pinNumber, GpioFunc func);
void gpioPinEnable(u8 pinNumber);

#define REGS_GPIO ((struct GpioRegs *)(PBASE + 0x200000))
