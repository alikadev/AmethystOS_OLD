#ifndef __MEMORY_MANAGMENT_H__
#define __MEMORY_MANAGMENT_H__

#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define SECTION_SHIFT           (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE               (1 << PAGE_SHIFT)
#define SECTION_SIZE            (1 << SECTION_SHIFT)

#define LOW_MEMORY              (2 * SECTION_SIZE)
#define HIGH_MEMORY             0x40000000

#define PAGING_MEMORY           (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES            (PAGING_MEMORY / PAGE_SIZE)
#define ID_MAP_PAGES            6
#define ID_MAP_TABLE_SIZE       (ID_MAP_PAGES * PAGE_SIZE)
#define ENTRIES_PER_TABLE       512
#define PGD_SHIFT               (PAGE_SHIFT + 3 * TABLE_SHIFT)
#define PUD_SHIFT               (PAGE_SHIFT + 2 * TABLE_SHIFT)
#define PMD_SHIFT               (PAGE_SHIFT + TABLE_SHIFT)
#define PUD_ENTRY_MAP_SIZE      (1 << PUD_SHIFT)

#ifndef __ASSEMBLER__

void memzero(unsigned long src, unsigned int n);

#endif

#endif