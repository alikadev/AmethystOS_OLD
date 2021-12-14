#pragma once

#include "common.h"
#include "base.h"

struct AuxRegs
{
    r32 irq;
    r32 enables;
    r32 reserved[14];
    r32 mu_io;
    r32 mu_ier;
    r32 mu_iir;
    r32 mu_lcr;
    r32 mu_mcr;
    r32 mu_lsr;
    r32 mu_msr;
    r32 mu_scratch;
    r32 mu_cntl;
    r32 mu_stat;
    r32 mu_baud;
};

#define REGS_AUX ((struct AuxRegs *)(PBASE + 0x215000))
