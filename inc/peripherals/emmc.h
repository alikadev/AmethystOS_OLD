#ifndef __EMMC_H__
#define __EMMC_H__

#include "base.h"
#include "common.h"
#include "io.h"

#define EMMC_DEBUG 0
#define BSWAP32(x) (((x << 24) & 0xff000000) | \
                    ((x << 8 ) & 0x00ff0000) | \
                    ((x >> 8 ) & 0x0000ff00) | \
                    ((x >> 24) & 0x000000ff))

typedef struct {
    u8 respA : 1;
    u8 blockCount : 1;
    u8 autoCommand : 2;
    u8 direction : 1;
    u8 multiBlock : 1;
    u16 respB : 10;
    u8 responseType : 2;
    u8 res0 : 1;
    u8 crcEnable : 1;
    u8 idxEnable : 1;
    u8 isData : 1;
    u8 type : 2;
    u8 index : 6;
    u8 res1 : 2;
} emmcCmd;

#define RES_CMD {1, 1, 3, 1, 1, 0xF, 3, 1, 1, 1, 1, 3, 0xF, 3}

typedef enum {
    RTNone,
    RT136,
    RT48,
    RT48Busy
} cmdRespType;

typedef enum {
    CTGoIdle = 0,
    CTSendCide = 2,
    CTSendRelativeAddr = 3,
    CTIOSetOpCond = 5,
    CTSelectCard = 7,
    CTSendIfCond = 8,
    CTSetBlockLen = 16,
    CTReadBlock = 17,
    CTReadMultiple = 18,
    CTWriteBlock = 24,
    CTWriteMultiple = 25,
    CTOcrCheck = 41,
    CTSendSCR = 51,
    CTApp = 55
} cmdType;

typedef struct {
    u32 scr[2];
    u32 bisWidths;
    u32 version;
} scrReg;

typedef enum {
    SDECommandTimeout,
    SDECommandCrc,
    SDECommandEndBit,
    SDECommandIndex,
    SDEDataTimeout,
    SDEDataCrc,
    SDEDataEndBit,
    SDECurrentLimit,
    SDEAutoCmd12,
    SDEADma,
    SDETuning,
    SDERsvd
} sdError;

typedef struct {
    bool lastSuccess;
    u32 transferBlocks;
    emmcCmd lastCommand;
    reg32 lastCommandValue;
    u32 blockSize;
    u32 lastResponse[4];
    bool sdhc;
    u16 ocr;
    u32 rca;
    u64 offset;
    u0 *buffer;
    u32 baseClock;
    u32 listError;
    u32 lastInterrupt;
    scrReg src;
} emmc_Device;

typedef struct {
    reg32 arg2;
    reg32 blockSizeCount;
    reg32 args1;
    reg32 cmdXferMode;
    reg32 response[4];
    reg32 data;
    reg32 status;
    reg32 status;
    reg32 intFlag;
    reg32 intMask;
    reg32 intEnable;
    reg32 control[2];
    reg32 cap1;
    reg32 cap2;
    reg32 res0[2];
    reg32 forceInt;
    reg32 res1[7];
    reg32 bootTimeout;
    reg32 debugConfig;
    reg32 res2[2];
    reg32 extFifoConfig;
    reg32 extFifoEnable;
    reg32 tuneStep;
    reg32 tuneSDR;
    reg32 tuneDDR;
    reg32 res3[23];
    reg32 spiIntSupport;
    reg32 res4[2];
    reg32 slotIntBalue;
} emmc_Regs;

#define TO_REG(p) *((reg32 *)p)

// SD clock Freq (Hz)
#define SD_CLOCK_ID         400000
#define SD_CLOCK_NORMAL     25000000
#define SD_CLOCK_HIGH       50000000
#define SD_CLOCK_100        100000000
#define SD_CLOCK_208        208000000
#define SD_CMD_COMPLETE     1
#define SD_TRANS_COMPLETE   (1 << 1)
#define SD_BLOCK_GAP_EVENT  (1 << 2)
#define SD_DMA_INTERRUPT    (1 << 3)
#define SD_BUFF_WRT_READY   (1 << 4)
#define SD_BUFF_READ_READY  (1 << 5)
#define SD_CARD_INVERTION   (1 << 6)
#define SD_CARD_REMOVAL     (1 << 7)
#define SD_CARD_INTERRUPT   (1 << 8)

#if RPI_VERSION == 3
#define EMMC_BASE (PBASE + 0x300000)
#else
#define EMMC_BASE (PBASE + 0x340000)
#endif

#define EMMC ((emmc_Regs *)EMMC_BASE)

bool emmc_init();
bool emmc_read(u8 *buff, u32 size);
bool emmc_seek(u64 off);

#define EMMC_CTRL1_RESET_DATA   (1 << 26)
#define EMMC_CTRL1_RESET_CMD    (1 << 25)
#define EMMC_CTRL1_RESET_HOST   (1 << 24)
#define EMMC_CTRL1_RESET_ALL    (EMMC_CTRL1_RESET_DATA | EMMC_CTRL1_RESET_CMD | EMMC_CTRL1_RESET_HOST)

#define EMMC_CTRL1_CLK_GENSEL   (1 << 5)
#define EMMC_CTRL1_CLK_ENABLE   (1 << 2)
#define EMMC_CTRL1_CLK_STABLE   (1 << 1)
#define EMMC_CTRL1_CLK_INT_EN   1

#define EMMC_CTRL0_ALT_BOOT_EN  (1 << 22)
#define EMMC_CTRL0_BOOT_EN      (1 << 21)
#define EMMC_CTRL0_SPI_MODE     (1 << 20)

#define EMMC_STATUS_DAT_INHIBIT (1 << 1)
#define EMMC_STATUS_CMD_INHIBIT 1

#endif