#include "irq.h"
#include "printf.h"
#include "entry.h"
#include "peripherals/aux.h"
#include "uart.h"
#include "video.h"
#include "timer.h"
#include "os/baio.h"

const char entryErrorMessage[16][32] = {
    "SYNC_INVALID_EL1t",
    "IRQ_INVALID_EL1t",
    "FIQ_INVALID_EL1t",
    "ERROR_INVALID_EL1t",

    "SYNC_INVALID_EL1h",
    "IRQ_INVALID_EL1h",
    "FIQ_INVALID_EL1h",
    "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64",
    "IRQ_INVALID_EL0_64",
    "FIQ_INVALID_EL0_64",
    "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32",
    "IRQ_INVALID_EL0_32",
    "FIQ_INVALID_EL0_32",
    "ERROR_INVALID_EL0_32"
};

void showInvalidEntryMessage(u32 type, u64 esr, u64 addr){
    printf("ERROR CAUGHT: %s - %d, ESR: %X, Address: %X\n",
    entryErrorMessage[type],
    type,
    esr,
    addr);
}

void enableInterruptController() {
    #if RPI_VERSION == 4
    REGS_IRQ->irq0_enable_0 = AUX_IRQ | SYS_TIMER_IRQ_1 | SYS_TIMER_IRQ_3;
    #else
    REGS_IRQ->irq0_enable_1 = AUX_IRQ | SYS_TIMER_IRQ_1 | SYS_TIMER_IRQ_3;
    #endif
}

void handleIrq() {
    u32 irq;
    #if RPI_VERSION == 4
    irq = REGS_IRQ->irq0_pending_0;
    #else
    irq = REGS_IRQ->irq0_pending_1;
    #endif

    while(irq) {
        if(irq & AUX_IRQ) {
            //irq &= ~AUX_IRQ;
            //while((REGS_AUX->mu_iir & 4) == 4){
            //    baio_putc(uart_getc());
            //}
        }

        if(irq & SYS_TIMER_IRQ_1){
            irq &= ~SYS_TIMER_IRQ_1;

            handleTimer_1();
        }

        if(irq & SYS_TIMER_IRQ_3){
            irq &= ~SYS_TIMER_IRQ_3;

            handleTimer_3();
        }
    }
}
