#include "dma.h"
#include "mem.h"
#include "mm.h"
#include "timer.h"
#include "printf.h"

dma_Channel channels[15];

static u16 channelMap = 0x1F35;

static u16 allocateChannel(u32 channel){
    if(!(channel & ~0xF0)) {
        if(channelMap & (1 << channel)){
            channelMap &= ~(1 << channel);
            return channel;
        }
        return -1;
    }

    u16 i=channel=CT_NORMAL ? 6 : 12;

    for(; i>=0; i--){
        if(channelMap & (1 << i)){
            channelMap &= ~(1 << i);
            return i;
        }
    }

    return CT_NONE;
}

dma_Channel *dma_openChannel(u32 channel) {
    u32 channelId = allocateChannel(channel);

    if(channelId == CT_NONE){
        printf("INVALIDE CHANNEL! %d\n", channel);
        return 0;
    }

    dma_Channel *dma = (dma_Channel *)&channels[channelId];
    dma->channel = channelId;
    
    dma->block = (dma_ControlBlock *)((LOW_MEMORY + 31) & ~31);
    dma->block->res[0] = 0;
    dma->block->res[1] = 0;

    REGS_DMA_ENABLE |= (1 << dma->channel);
    timerSleep(3);
    REGS_DMA(dma->channel)->control |= CS_RESET;

    while(REGS_DMA(dma->channel)->control & CS_RESET);
    return dma;
}
u0 dma_closeChannel(dma_Channel *channel) {
    channelMap |= (1 << channel->channel);
}
u0 dma_setupMemCpy(dma_Channel *channel, u0 *dest, u0 *src, u32 len, u32 burstLen) {
    channel->block->transferInfo = (burstLen << TI_BURST_LENGTH_SHIFT)
						    | TI_SRC_WIDTH
						    | TI_SRC_INC
						    | TI_DEST_WIDTH
						    | TI_DEST_INC;

    channel->block->srcAddr = (u32)src;
    channel->block->destAddr = (u32)dest;
    channel->block->transferLength = len;
    channel->block->mode2dStide = 0;
    channel->block->nextBlockAddr = 0;
}
u0 dma_start(dma_Channel *channel) {
    REGS_DMA(channel->channel)->controlBlockAddr = BUS_ADDRESS((u32)channel->block);

    REGS_DMA(channel->channel)->control = CS_WAIT_FOR_OUTSTANDING_WRITES
					      | (DEFAULT_PANIC_PRIORITY << CS_PANIC_PRIORITY_SHIFT)
					      | (DEFAULT_PRIORITY << CS_PRIORITY_SHIFT)
					      | CS_ACTIVE;
}
bool dma_wait(dma_Channel *channel) {
    while(REGS_DMA(channel->channel)->control & CS_ACTIVE);
    channel->status = REGS_DMA(channel->channel)->control & CS_ERROR ? false : true;
    return channel->status;
}