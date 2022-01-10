#include "mem.h"
#include "peripherals/base.h"
#include "mm.h"
#include "mmu.h"

static u16 memMap[PAGING_PAGES] = {0};

u0 *allocateMemory(int bytes){
    int pages = bytes / PAGE_SIZE;
    
    if(bytes % PAGE_SIZE) pages++;

    return getFreePages(pages);
}

u0 freeMemory(void *base){
    u64 pageNum = (((u64)base - LOW_MEMORY) / PAGE_SIZE);
    u32 pages = memMap[pageNum];

    for(u32 i=0; i<pages; i++){
        memMap[pageNum+i]=0;
    }
}
PACKED

u0 *getFreePages(int numPages){
    u32 startIndex = 0;
    u32 count = 0;

    for(u32 i=0; i<PAGING_PAGES; i++) {
        if(memMap[i]== 0){
            if(!count){
                startIndex = i;
            }
            count++;
            if(count == numPages){
                memMap[startIndex] = count;
                for(int c=1; c<count; c++){
                    memMap[c + startIndex] = 1;
                }
                u0 *p = (u0 *)(LOW_MEMORY + (startIndex * PAGE_SIZE));
                return p;
            }
        } else {
            count = 0;
        }
    }
}

void *memcpy(u0 *dest, const u0 *src, u32 n){
    u8 *byteDest = (u8 *)dest;
    u8 *byteSrc = (u8 *)src;

    for (u32 i = 0; i < n; i++) {
        byteDest[i] = byteSrc[i];
    }

    return dest;
}

#define BLOCK_SIZE 0x40000000

u0 createTableEntry(u64 tbl, u64 nextTbl, u64 va, u64 shift, u64 flags){
    u64 tableIndex = va >> shift;
    tableIndex &= (ENTRIES_PER_TABLE -1);
    u64 descriptor = nextTbl | flags;
    *((u64 *)(tbl + (tableIndex << 3))) = descriptor;   
}

u0 createBlockMap(u64 pmd, u64 vstart, u64 vend, u64 pa){
    vstart >>= SECTION_SHIFT;
    vstart &= (ENTRIES_PER_TABLE - 1);

    vend >>= SECTION_SHIFT;
    vend--;
    vend &= (ENTRIES_PER_TABLE -1 );

    pa >>= SECTION_SHIFT;
    pa <<= SECTION_SHIFT;

    do{
        u64 _pa = pa;

        if(pa >= DEVICE_START) {
            _pa |= TD_DEVICE_BLOCK_FLAGS;
        } else {
            _pa |= TD_KERNEL_BLOCK_FLAGS;
        }

        *((u64 *)(pmd + (vstart << 3))) = _pa;
        pa += SECTION_SIZE;
        vstart++;
    }while(vstart <= vend);
}

u64 id_pgd_addr();

u0 initMmu() {
    u64 id_pgd = id_pgd_addr();

    memzero(id_pgd, ID_MAP_TABLE_SIZE);
    
    u64 mapBase = 0;
    u64 tbl = id_pgd;
    u64 nextTbl = tbl + PAGE_SIZE;

    createTableEntry(tbl, nextTbl, mapBase, PGD_SHIFT, TD_KERNEL_TABLE_FLAGS);

    tbl += PAGE_SIZE;
    nextTbl += PAGE_SIZE;

    u64 blockTbl = tbl;

    for(u64 i=0; i<4; i++) {
        createTableEntry(tbl, nextTbl, mapBase, PUD_SHIFT, TD_KERNEL_TABLE_FLAGS);

        nextTbl += PAGE_SIZE;
        mapBase += PUD_ENTRY_MAP_SIZE;
        blockTbl += PAGE_SIZE;

        u64 offset = BLOCK_SIZE * i;
        createBlockMap(blockTbl, offset, offset + BLOCK_SIZE, offset);
    }
}