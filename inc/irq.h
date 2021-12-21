/*
File: irq.h
Copyright (C) 2020 Alikadev
This file contains some irq/interrupt structure and functions. 
vm_irqs is an enum to define each system timer and irq auxillary 
bit. the struct arm_irq_regs contains the structure for the arm 
irq regs. The adresse to the irq regs is REGS_IRQ. This file
contains some functions too:
u0 irq_intiVectors();
u0 irq_enable();
u0 irq_disable();
u0 enableInterruptController();
*/

#ifndef __IRQ_H__
#define __IRQ_H__

#include "peripherals/base.h"
#include "common.h"

enum vc_irqs {
    SYS_TIMER_IRQ_0 = 1,
    SYS_TIMER_IRQ_1 = 2,
    SYS_TIMER_IRQ_2 = 4,
    SYS_TIMER_IRQ_3 = 8,
    AUX_IRQ = (1 << 29)
};

#if RPI_VERSION == 3
typedef struct arm_irq_regs_2837 {
    reg32 irq0_pending_0;
    reg32 irq0_pending_1;
    reg32 irq0_pending_2;
    reg32 fiq_control;
    reg32 irq0_enable_1;
    reg32 irq0_enable_2;
    reg32 irq0_enable_0;
    reg32 res;
    reg32 irq0_disable_1;
    reg32 irq0_disable_2;
    reg32 irq0_disable_0;
} arm_irq_regs;
#else
typedef struct arm_irq_regs_2711 {
    reg32 irq0_pending_0;
    reg32 irq0_pending_1;
    reg32 irq0_pending_2;
    reg32 res0;
    reg32 irq0_enable_0;
    reg32 irq0_enable_1;
    reg32 irq0_enable_2;
    reg32 res1;
    reg32 irq0_disable_0;
    reg32 irq0_disable_1;
    reg32 irq0_disable_2;
}arm_irq_regs;
#endif

#define REGS_IRQ ((arm_irq_regs *)(PBASE + 0xB200))

u0 irq_intiVectors();
u0 irq_enable();
u0 irq_disable();

u0 enableInterruptController();

#endif