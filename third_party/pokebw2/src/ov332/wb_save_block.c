#include "types.h"
#include "app/unova_link.h"

// Finds the blocks of a Black or White save. The ROM doesn't name this file. Its table is laid out before
// wb_save_convert.c's smaller ones, which a single file's size-sorted .rodata can't do, so it is a file of its own

#define WB_SAVE_BLOCK_COUNT 71
// The blocks before this one take a multiple of 256 bytes
#define WB_SAVE_ALIGNED_BLOCKS 69

static int WBSaveBlock_GetOffset(int block);
static u32 WBSaveBlock_GetAlignedSize(u32 block);
static u32 WBSaveBlock_GetSize(u32 block);

// clang-format off
static const u32 sBlockSizes[WB_SAVE_BLOCK_COUNT] = {
#include "asset/wb_save_block_sBlockSizes.inc"
};

// clang-format on

static int WBSaveBlock_GetOffset(int block) {
    int offset = 0;
    u8 i;

    for (i = 0; i < block; i++) {
        if (i >= WB_SAVE_ALIGNED_BLOCKS) {
            offset += WBSaveBlock_GetSize(i);
        } else {
            offset += WBSaveBlock_GetAlignedSize(i);
        }
    }
    return offset;
}

static u32 WBSaveBlock_GetAlignedSize(u32 block) {
    u32 size = sBlockSizes[block];
    u8 rest = size % 0x100;

    return size + (rest == 0 ? 0 : 0x100 - rest);
}

static u32 WBSaveBlock_GetSize(u32 block) {
    return sBlockSizes[block];
}

void *WBSaveBlock_Get(void *save, int block) {
    return (u8 *)save + WBSaveBlock_GetOffset(block);
}
