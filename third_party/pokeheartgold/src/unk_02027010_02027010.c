#include "global.h"

#include "unk_02027010.h"

extern const char _020F6458[];

void sub_02027010(void) {
    FSArchive *arc;
    u8 *header;

    if (!FS_IsAvailable()) {
        OS_Terminate();
        return;
    }

    if (*(u32 *)(HW_MAIN_MEM + 0x007ff00c) == 0) {
        CARD_Init();
        MI_CpuCopy8((const void *)HW_ROM_HEADER_BUF, (void *)(HW_MAIN_MEM + 0x007ff000), HW_CARD_ROM_HEADER_SIZE);
        MI_CpuCopy8((const void *)HW_ROM_HEADER_BUF, (void *)HW_CARD_ROM_HEADER, HW_CARD_ROM_HEADER_SIZE);
        *(u32 *)(HW_MAIN_MEM + 0x007ff00c) = 0x4A414441;
    }

    arc = FS_FindArchive(_020F6458, 3);
    header = (u8 *)(HW_MAIN_MEM + 0x007ff000);
    arc->fat = *(u32 *)(header + 0x48);
    arc->fat_size = *(u32 *)(header + 0x4C);
    arc->fnt = *(u32 *)(header + 0x40);
    arc->fnt_size = *(u32 *)(header + 0x44);
    if (*(u32 *)(header + 0xC) != 0x4A414441 || *(u16 *)(header + 0x10) != 0x3130) {
        OS_Terminate();
    }
}
