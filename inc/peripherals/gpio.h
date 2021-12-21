/*
File: gpio.h
Copyright (C) 2020 Alikadev
This file contains the structure of the gpio regs GpioRegs, an
enum of gpio functions _GpioFunc and two function:
void gpio_pin_set_func(u8 pinNumber, GpioFunc func);
void gpio_pin_enable(u8 pinNumber);
*/

#ifndef __PERIPHERICALS_GPIO_H__
#define __PERIPHERICALS_GPIO_H__

#include "common.h"
#include "peripherals/base.h"

struct GpioPinData {
    reg32 reserved;
    reg32 data[2];
};

struct GpioRegs {
    reg32 func_select[6];
    struct GpioPinData output_set;
    struct GpioPinData output_clear;
    struct GpioPinData level;
    struct GpioPinData ev_detect_status;
    struct GpioPinData re_detect_enable;
    struct GpioPinData fe_detect_enable;
    struct GpioPinData hi_detect_enable;
    struct GpioPinData lo_detect_enable;
    struct GpioPinData async_re_detect;
    struct GpioPinData async_fe_detect;
    reg32 reserved;
    reg32 pupd_enable;
    reg32 pupd_enable_clocks[2];
};

#define REGS_GPIO ((struct GpioRegs *)(PBASE + 0x200000))

typedef enum _GpioFunc {
    GFInput = 0,
    GFOutput = 1,
    GFAlt0 = 4,
    GFAlt1 = 5,
    GFAlt2 = 6,
    GFAlt3 = 7,
    GFAlt4 = 3,
    GFAlt5 = 2
} GpioFunc;

void gpio_pin_set_func(u8 pinNumber, GpioFunc func);

void gpio_pin_enable(u8 pinNumber);

#endif