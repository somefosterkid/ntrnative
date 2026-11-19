#include "global.h"

#include "heap.h"
#include "system.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

typedef struct UnkStruct_ov71_0224A0F0_Heap_Alloc {
    void *unk_0;
    s32 unk_4;
    u8 unk_8[0x4];
    s32 unk_C;
    u8 unk_10[0x14];
    s32 unk_24;
    s32 unk_28;
    u8 unk_2C[0x8];
    u8 unk_34;
    u8 unk_35[0x7B];
} UnkStruct_ov71_0224A0F0_Heap_Alloc;

struct BgConfig;

void ov71_0224A0B8(BgConfig *bgConfig, int bg1Scroll, int bg2Scroll);

s32 ov71_0224A3D8(void *);
s32 ov71_0224A500(void *);
s32 ov71_0224A590(void *);
s32 ov71_0224A5B0(void *);
void ov71_0224A12C(void *arg0);

void ov71_0224A0B8(BgConfig *bgConfig, int bg1Scroll, int bg2Scroll) {
    bg2Scroll &= 0x1ff;
    bg1Scroll &= 0x1ff;

    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, bg2Scroll);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, bg1Scroll);
    BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, bg1Scroll + 192);
}

s32 ov71_02247384(void *);

s32 ov71_0224A5A8(void *);

void ov71_0224A8B0(void *);

void *ov71_0224A0F0(void *arg0);

void *ov71_0224A0F0(void *arg0) {
    UnkStruct_ov71_0224A0F0_Heap_Alloc *arg;

    arg = Heap_Alloc(HEAP_ID_57, sizeof(UnkStruct_ov71_0224A0F0_Heap_Alloc));
    if (arg != NULL) {
        arg->unk_0 = arg0;
        arg->unk_4 = 0;
        arg->unk_24 = ov71_02247384(arg0);
        arg->unk_28 = 0;
        arg->unk_C = 0;
        ov71_0224A5A8((void *)&arg->unk_34);
        Main_SetVBlankIntrCB(ov71_0224A8B0, arg);
    }
    return arg;
}

void ov71_0224A12C(void *ptr) {
    if (ptr != NULL) {
        Main_SetVBlankIntrCB(NULL, NULL);
        ov71_0224A3D8(ptr);
        ov71_0224A500(ptr);
        ov71_0224A590(ptr);
        ov71_0224A5B0(ptr + 0x34);
        Heap_Free(ptr);
    }
}
