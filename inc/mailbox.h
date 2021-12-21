/*
File: mailbox.h
Copyright (C) 2020 Alikadev
This file contains some mailbox struct, firmware enum, 
power domain define and functions:
u32 mailbox_clockRate(ClockType ct);
bool mailbox_genericCommand(u32 tagId, u32 id, u32 *value);
bool mailbox_powerCheck(u32 type);
bool mailbox_process(mailbox_Tag *tag, u32 tagSize);
*/

#ifndef __MAILBOX_H__
#define __MAILBOX_H__

#include "common.h"

typedef struct {
    u32 id;
    u32 bufferSize;
    u32 valueLength;
} mailbox_Tag;

typedef struct {
    u32 size;
    u0 *tag;
} mailbox_Command;

typedef struct {
    mailbox_Tag tag;
    u32 id;
    u32 value;
} mailbox_Generic;

typedef struct {
    mailbox_Tag tag;
    u32 id;
    u32 state;
} mailbox_Power;

typedef struct {
    mailbox_Tag tag;
    u32 id;
    u32 rate;
} mailbox_Clock;

typedef enum {
    EMMC = 1,
    UART = 2,
    ARM  = 3,
    CORE = 4
} ClockType;

#define RPI_POWER_DOMAIN_I2C0   		0
#define RPI_POWER_DOMAIN_I2C1		    1
#define RPI_POWER_DOMAIN_I2C2		    2
#define RPI_POWER_DOMAIN_VIDEO_SCALER	3
#define RPI_POWER_DOMAIN_VPU1   		4
#define RPI_POWER_DOMAIN_HDMI	    	5
#define RPI_POWER_DOMAIN_USB		    6
#define RPI_POWER_DOMAIN_VEC    		7
#define RPI_POWER_DOMAIN_JPEG	    	8
#define RPI_POWER_DOMAIN_H264		    9
#define RPI_POWER_DOMAIN_V3D	    	10
#define RPI_POWER_DOMAIN_ISP    		11
#define RPI_POWER_DOMAIN_UNICAM0	    12
#define RPI_POWER_DOMAIN_UNICAM1    	13
#define RPI_POWER_DOMAIN_CCP2RX		    14
#define RPI_POWER_DOMAIN_CSI2   		15
#define RPI_POWER_DOMAIN_CPI	    	16
#define RPI_POWER_DOMAIN_DSI0		    17
#define RPI_POWER_DOMAIN_DSI1   		18
#define RPI_POWER_DOMAIN_TRANSPOSER 	19
#define RPI_POWER_DOMAIN_CCP2TX		    20
#define RPI_POWER_DOMAIN_CDP	    	21
#define RPI_POWER_DOMAIN_ARM    		22

#define RPI_POWER_DOMAIN_COUNT		    23


enum rpi_firmware_property_status {
	RPI_FIRMWARE_STATUS_REQUEST = 0,
	RPI_FIRMWARE_STATUS_SUCCESS = 0x80000000,
	RPI_FIRMWARE_STATUS_ERROR =   0x80000001,
};

enum rpi_firmware_property_tag {
	RPI_FIRMWARE_PROPERTY_END =                           0,
	RPI_FIRMWARE_GET_FIRMWARE_REVISION =                  0x00000001,

	RPI_FIRMWARE_SET_CURSOR_INFO =                        0x00008010,
	RPI_FIRMWARE_SET_CURSOR_STATE =                       0x00008011,

	RPI_FIRMWARE_GET_BOARD_MODEL =                        0x00010001,
	RPI_FIRMWARE_GET_BOARD_REVISION =                     0x00010002,
	RPI_FIRMWARE_GET_BOARD_MAC_ADDRESS =                  0x00010003,
	RPI_FIRMWARE_GET_BOARD_SERIAL =                       0x00010004,
	RPI_FIRMWARE_GET_ARM_MEMORY =                         0x00010005,
	RPI_FIRMWARE_GET_VC_MEMORY =                          0x00010006,
	RPI_FIRMWARE_GET_CLOCKS =                             0x00010007,
	RPI_FIRMWARE_GET_POWER_STATE =                        0x00020001,
	RPI_FIRMWARE_GET_TIMING =                             0x00020002,
	RPI_FIRMWARE_SET_POWER_STATE =                        0x00028001,
	RPI_FIRMWARE_GET_CLOCK_STATE =                        0x00030001,
	RPI_FIRMWARE_GET_CLOCK_RATE =                         0x00030002,
	RPI_FIRMWARE_GET_VOLTAGE =                            0x00030003,
	RPI_FIRMWARE_GET_MAX_CLOCK_RATE =                     0x00030004,
	RPI_FIRMWARE_GET_MAX_VOLTAGE =                        0x00030005,
	RPI_FIRMWARE_GET_TEMPERATURE =                        0x00030006,
	RPI_FIRMWARE_GET_MIN_CLOCK_RATE =                     0x00030007,
	RPI_FIRMWARE_GET_MIN_VOLTAGE =                        0x00030008,
	RPI_FIRMWARE_GET_TURBO =                              0x00030009,
	RPI_FIRMWARE_GET_MAX_TEMPERATURE =                    0x0003000a,
	RPI_FIRMWARE_GET_STC =                                0x0003000b,
	RPI_FIRMWARE_ALLOCATE_MEMORY =                        0x0003000c,
	RPI_FIRMWARE_LOCK_MEMORY =                            0x0003000d,
	RPI_FIRMWARE_UNLOCK_MEMORY =                          0x0003000e,
	RPI_FIRMWARE_RELEASE_MEMORY =                         0x0003000f,
	RPI_FIRMWARE_EXECUTE_CODE =                           0x00030010,
	RPI_FIRMWARE_EXECUTE_QPU =                            0x00030011,
	RPI_FIRMWARE_SET_ENABLE_QPU =                         0x00030012,
	RPI_FIRMWARE_GET_DISPMANX_RESOURCE_MEM_HANDLE =       0x00030014,
	RPI_FIRMWARE_GET_EDID_BLOCK =                         0x00030020,
	RPI_FIRMWARE_GET_CUSTOMER_OTP =                       0x00030021,
	RPI_FIRMWARE_GET_DOMAIN_STATE =                       0x00030030,
	RPI_FIRMWARE_SET_CLOCK_STATE =                        0x00038001,
	RPI_FIRMWARE_SET_CLOCK_RATE =                         0x00038002,
	RPI_FIRMWARE_SET_VOLTAGE =                            0x00038003,
	RPI_FIRMWARE_SET_TURBO =                              0x00038009,
	RPI_FIRMWARE_SET_CUSTOMER_OTP =                       0x00038021,
	RPI_FIRMWARE_SET_DOMAIN_STATE =                       0x00038030,
	RPI_FIRMWARE_GET_GPIO_STATE =                         0x00030041,
	RPI_FIRMWARE_SET_GPIO_STATE =                         0x00038041,
	RPI_FIRMWARE_SET_SDHOST_CLOCK =                       0x00038042,
	RPI_FIRMWARE_GET_GPIO_CONFIG =                        0x00030043,
	RPI_FIRMWARE_SET_GPIO_CONFIG =                        0x00038043,
	RPI_FIRMWARE_GET_PERIPH_REG =                         0x00030045,
	RPI_FIRMWARE_SET_PERIPH_REG =                         0x00038045,


	/* Dispmanx TAGS */
	RPI_FIRMWARE_FRAMEBUFFER_ALLOCATE =                   0x00040001,
	RPI_FIRMWARE_FRAMEBUFFER_BLANK =                      0x00040002,
	RPI_FIRMWARE_FRAMEBUFFER_GET_PHYSICAL_WIDTH_HEIGHT =  0x00040003,
	RPI_FIRMWARE_FRAMEBUFFER_GET_VIRTUAL_WIDTH_HEIGHT =   0x00040004,
	RPI_FIRMWARE_FRAMEBUFFER_GET_DEPTH =                  0x00040005,
	RPI_FIRMWARE_FRAMEBUFFER_GET_PIXEL_ORDER =            0x00040006,
	RPI_FIRMWARE_FRAMEBUFFER_GET_ALPHA_MODE =             0x00040007,
	RPI_FIRMWARE_FRAMEBUFFER_GET_PITCH =                  0x00040008,
	RPI_FIRMWARE_FRAMEBUFFER_GET_VIRTUAL_OFFSET =         0x00040009,
	RPI_FIRMWARE_FRAMEBUFFER_GET_OVERSCAN =               0x0004000a,
	RPI_FIRMWARE_FRAMEBUFFER_GET_PALETTE =                0x0004000b,
	RPI_FIRMWARE_FRAMEBUFFER_GET_TOUCHBUF =               0x0004000f,
	RPI_FIRMWARE_FRAMEBUFFER_GET_GPIOVIRTBUF =            0x00040010,
	RPI_FIRMWARE_FRAMEBUFFER_RELEASE =                    0x00048001,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_PHYSICAL_WIDTH_HEIGHT = 0x00044003,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_VIRTUAL_WIDTH_HEIGHT =  0x00044004,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_DEPTH =                 0x00044005,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_PIXEL_ORDER =           0x00044006,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_ALPHA_MODE =            0x00044007,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_VIRTUAL_OFFSET =        0x00044009,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_OVERSCAN =              0x0004400a,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_PALETTE =               0x0004400b,
	RPI_FIRMWARE_FRAMEBUFFER_TEST_VSYNC =                 0x0004400e,
	RPI_FIRMWARE_FRAMEBUFFER_SET_PHYSICAL_WIDTH_HEIGHT =  0x00048003,
	RPI_FIRMWARE_FRAMEBUFFER_SET_VIRTUAL_WIDTH_HEIGHT =   0x00048004,
	RPI_FIRMWARE_FRAMEBUFFER_SET_DEPTH =                  0x00048005,
	RPI_FIRMWARE_FRAMEBUFFER_SET_PIXEL_ORDER =            0x00048006,
	RPI_FIRMWARE_FRAMEBUFFER_SET_ALPHA_MODE =             0x00048007,
	RPI_FIRMWARE_FRAMEBUFFER_SET_VIRTUAL_OFFSET =         0x00048009,
	RPI_FIRMWARE_FRAMEBUFFER_SET_OVERSCAN =               0x0004800a,
	RPI_FIRMWARE_FRAMEBUFFER_SET_PALETTE =                0x0004800b,
	RPI_FIRMWARE_FRAMEBUFFER_SET_TOUCHBUF =               0x0004801f,
	RPI_FIRMWARE_FRAMEBUFFER_SET_GPIOVIRTBUF =            0x00048020,
	RPI_FIRMWARE_FRAMEBUFFER_SET_VSYNC =                  0x0004800e,
	RPI_FIRMWARE_FRAMEBUFFER_SET_BACKLIGHT =              0x0004800f,

	RPI_FIRMWARE_VCHIQ_INIT =                             0x00048010,

	RPI_FIRMWARE_GET_COMMAND_LINE =                       0x00050001,
	RPI_FIRMWARE_GET_DMA_CHANNELS =                       0x00060001,
};

u32 mailbox_clockRate(ClockType ct);
bool mailbox_genericCommand(u32 tagId, u32 id, u32 *value);
bool mailbox_powerCheck(u32 type);
bool mailbox_process(mailbox_Tag *tag, u32 tagSize);

#endif