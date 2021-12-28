#ifndef __DMA_H__
#define __DMA_H__

#include "common.h"
#include "peripherals/base.h"

typedef struct {
    u32 transferInfo;
    u32 srcAddr;
    u32 destAddr;
    u32 transferLength;
    u32 mode2dStide;
    u32 nextBlockAddr;
    u32 res[2];
} dma_ControlBlock;

typedef struct {
    u32 control;
    u32 controlBlockAddr;
    dma_ControlBlock block;
} dma_ChannelRegs;

#define REGS_DMA(channel) ((dma_ChannelRegs *)(PBASE + 0x7000 + (channel * 0x100)))

#define REGS_DMA_INT_STATUS *((volatile u32 *)(PBASE + 0x7FE0))
#define REGS_DMA_ENABLE *((volatile u32 *)(PBASE + 0x07FF0))

// def for bits of controle and transfer information
#define CS_RESET			            (1 << 31)
#define CS_ABORT			            (1 << 30)
#define CS_WAIT_FOR_OUTSTANDING_WRITES	(1 << 28)
#define CS_PANIC_PRIORITY_SHIFT		    20
#define DEFAULT_PANIC_PRIORITY		    15
#define CS_PRIORITY_SHIFT		        16
#define DEFAULT_PRIORITY		        1
#define CS_ERROR            			(1 << 8)
#define CS_INT				            (1 << 2)
#define CS_END          				(1 << 1)
#define CS_ACTIVE		            	(1 << 0)

#define TI_PERMAP_SHIFT		        	16
#define TI_BURST_LENGTH_SHIFT		    12
#define DEFAULT_BURST_LENGTH	    	0
#define TI_SRC_IGNORE       			(1 << 11)
#define TI_SRC_DREQ	    		        (1 << 10)
#define TI_SRC_WIDTH        			(1 << 9)
#define TI_SRC_INC		            	(1 << 8)
#define TI_DEST_DREQ			        (1 << 6)
#define TI_DEST_WIDTH       			(1 << 5)
#define TI_DEST_INC			            (1 << 4)
#define TI_WAIT_RESP          			(1 << 3)
#define TI_TDMODE       		    	(1 << 1)
#define TI_INTEN		        	    (1 << 0)

typedef struct {
    u32 channel;
    dma_ControlBlock *block;
    bool status;
} dma_Channel;

typedef enum {
    CT_NONE = -1,
    CT_NORMAL = 0x81,
} dma_ChannelType;

dma_Channel *dma_openChannel(u32 channel);
u0 dma_closeChannel(dma_Channel *channel);
u0 dma_setupMemCpy(dma_Channel *channel, u0 *dest, u0 *src, u32 len, u32 burstLen);
u0 dma_start(dma_Channel *channel);
bool dma_wait(dma_Channel *channel);



#endif