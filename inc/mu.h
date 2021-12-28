#ifndef __MMU_H__
#define __MMU_H__

#define MM_TYPE_PAGE_TABLE      0x3
#define MM_TYPE_PAGE            0x3
#define MM_TYPE_BLOCK           0x1
#define MM_ACCESS               (0x1 << 10)
#define MM_ACCESS_PERMISSION    (0x01 << 6);

#define MATTR_DEVICE_nGnRnE        0x0
#define MATTR_NORMAL_NC            0x44
#define MATTR_DEVICE_nGnRnE_INDEX  0
#define MATTR_NORMAL_NC_INDEX      1
#define MAIR_EL1_VAL               ((MATTR_NORMAL_NC << (8 * MATTR_NORMAL_NC_INDEX)) | MATTR_DEVICE_nGnRnE << (8 * MATTR_DEVICE_nGnRnE_INDEX))

#define ATTRINDX_NORMAL             0
#define ATTRINDX_DEVICE             1
#define ATTRINDX_COHERENTS          2

#define MAIR_VALUENEW (0xFF << ATTRINDX_NORMAL*8) \
                    | 0x04 << ATTRINDX_DEVICE*8 \
                    | 0x00 << ATTRINDX_COHERENTS*8)

#endif