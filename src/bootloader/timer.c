#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "peripherals/aux.h"

const u32 interval_1 = CLOCKHZ;
u32 curVal_1 = 0;

const u32 interval_3 = CLOCKHZ / 4;
u32 curVal_3 = 0;

u0 timerInit() {
    curVal_1 = REGS_TIMER->counter_lo;
    curVal_1 += interval_1;
    REGS_TIMER->compare[1] = curVal_1;

    curVal_3 = REGS_TIMER->counter_lo;
    curVal_3 += interval_3;
    REGS_TIMER->compare[3] = curVal_3;
}

u0 handleTimer_1() {
    curVal_1 += interval_1;
    REGS_TIMER->compare[1] = curVal_1;
    REGS_TIMER->control_status |= SYS_TIMER_IRQ_1;
}

u0 handleTimer_3() {
    curVal_3 += interval_3;
    REGS_TIMER->compare[3] = curVal_3;
    REGS_TIMER->control_status |= SYS_TIMER_IRQ_3;
}

u64 timerGetTicks() {
    u32 hi = REGS_TIMER->counter_hi;
    u32 lo = REGS_TIMER->counter_lo;

    //recheck
    if(hi != REGS_TIMER->counter_hi){
        hi = REGS_TIMER->counter_hi;
        lo = REGS_TIMER->counter_lo;
    }

    return ((u64)hi << 32 | lo);
}

void timerSleep(u32 ms) {
    u64 start = timerGetTicks();
    while(timerGetTicks() <= start + ms*1000);
}