/*#include "peripherals/emmc.h"
#include "mailbox.h"
#include "printf.h"
#include "timer.h"

bool waitRegMask(reg32 *reg, u32 mask, bool set, u32 timeout);

u32 getClockDivider(u32 baseClock, u32 targetRate) {
    u32 targetDiv = 1;
    if(targetRate <= baseClock) {
        targetDiv = baseClock / targetRate;

        if(baseClock % targetRate)
            targetDiv = 0;
    }

    int div = -1;
    for(int fb = 31; fb >= 0; fb--) {
        u32 bt = (1 << fb);

        if(targetDiv & bt) {
            div = fb;
            targetDiv &= ~(bt);

            if(targetDiv) {
                div++;
            }

            break;
        }
    }

    if(div == -1)
        div = 31;

    if(div >= 32)
        div = 31;

    if (div != 0)
        div = (1 << (div - 1));

    if(div >= 0x400)
        div = 0x3FF;
    
    u32 freqSel = div & 0xFF;
    u32 upper = (div >> 8) & 0x3;
    u32 ret = (freqSel << 8) | (upper << 6) | (0 << 5);

    return ret;
}

bool switchClockRate(u32 baseClock, u32 targetRate) {
    u32 divider = getClockDivider(baseClock, targetRate);

    while(EMMC->status & (EMMC_STATUS_CMD_INHIBIT | EMMC_STATUS_DAT_INHIBIT)){
        timerSleep(1);
    }

    u32 c1 = EMMC->control[1] & ~EMMC_CTRL1_CLK_ENABLE;
    EMMC->control[1] = c1;
    
    timerSleep(3);
    EMMC->control[1] = (c1 | divider) & ~0xFFE0;

    timerSleep(3);
    EMMC->control[1] = c1 | EMMC_CTRL1_CLK_ENABLE;

    return true;
}

bool emmcSetupClock() {
    EMMC
}*/