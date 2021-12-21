#ifndef __TIMER_H__
#define __TIMER_H__

#include "peripherals/base.h"
#include "common.h"

#define CLOCKHZ 1000000

//10.2
struct timerRegs {
    reg32 control_status;
    reg32 counter_lo;
    reg32 counter_hi;
    reg32 compare[4];
};

#define REGS_TIMER ((struct timerRegs *)(PBASE + 0x3000))


u0 timerInit();
u0 handleTimer_1();
u0 handleTimer_3();
u64 timerGetTicks();
u0 timerSleep(u32 ms);

#endif