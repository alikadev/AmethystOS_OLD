/*#include "common.h"
#include "gpio.h"
#include "mailbox.h"
#include "timer.h"
#include "mem.h"
#include "printf.h"
#include "peripherals/emmc.h"
#include "utils.h"
#include "timer.h"

bool emmcSetupClock();
bool switchClockRate(u32 baseClock, u32 targetRate);

static emmc_Device device = {0};
static const emmcCmd INVALIDE_CMD = RES_CMD;

bool waitRegMack(reg32 *reg, u32 mask, bool set, u32 timeout){
    for (u32 cycles = 0; cycles <= timeout; cycles++)
    {
        if((*reg & mask) ? set : !set) {
            return true;
        }
        timerSleep(1);
    }
    
}

static const emmcCmd commands[] = {
    {0, 0, 0, 0, 0, 0, 0,        0, 0, 0, 0, 0, 0, 0},
    RES_CMD,
    {0, 0, 0, 0, 0, 0, RT136,    0, 1, 0, 0, 0, 2, 0},
    {0, 0, 0, 0, 0, 0, RT48,     0, 1, 0, 0, 0, 3, 0},
    {0, 0, 0, 0, 0, 0, 0,        0, 0, 0, 0, 0, 4, 0},
    {0, 0, 0, 0, 0, 0, RT136,    0, 0, 0, 0, 0, 5, 0},
    {0, 0, 0, 0, 0, 0, RT48,     0, 1, 0, 0, 0, 6, 0},
    {0, 0, 0, 0, 0, 0, RT48Busy, 0, 1, 0, 0, 0, 7, 0},
    {0, 0, 0, 0, 0, 0, RT136,    0, 1, 0, 0, 0, 8, 0},
    {0, 0, 0, 0, 0, 0, RT48,     0, 1, 0, 0, 0, 9, 0},
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    {0, 0, 0, 0, 0, 0, RT48,    0, 1, 0, 0, 0, 16 ,0},
    {0, 0, 0, 1, 0, 0, RT48,    0, 1, 0, 1, 0, 17 ,0},
    {0, 1, 1, 1, 1, 0, RT48,    0, 1, 0, 1, 0, 18 ,0},
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    {0, 0, 0, 0, 0, 0, RT48,    0, 0, 0, 0, 0, 41, 0},
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    RES_CMD,
    {0, 0, 0, 1, 0, 0, RT48,    0, 1, 0, 1, 0, 51, 0},
    RES_CMD,
    RES_CMD,
    RES_CMD,
    {0, 0, 0, 0, 0, 0, RT48,    0, 1, 0, 0, 0, 55, 0},
}

static u32 sdErrorMask(sdError err){
    return 1 << (16 + (u32)err);
}*/