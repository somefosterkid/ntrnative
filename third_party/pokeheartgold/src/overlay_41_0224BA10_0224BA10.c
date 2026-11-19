#include "global.h"

#include "fashion_case.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "overlay_41_unk_struct_ov22_0225_b85_c.h"

s32 ov41_02246130(void);

s32 ov41_02246670(void *, s32);

void *ov41_0224B530(s32 *, SaveFashionDataSub *);

void ov41_0224BBF0(void *);

s32 ov41_0224BC04(void *);

s32 ov41_0224BCA4(void *);

s32 ov41_0224BCF0(void *);

s32 ov41_0224BDCC(void *);

s32 ov41_0224BE5C(void *);

s32 AccessoryPortrait_Init(OverlayManager *man, s32 *state);

s32 AccessoryPortrait_Init(OverlayManager *man, s32 *state) {
    s32 v0[4];
    void *arg;
    void *v1;

    Heap_Create(HEAP_ID_3, HEAP_ID_13, 0x20000U);
    Heap_Create(HEAP_ID_3, HEAP_ID_14, 0x40000U);
    arg = OverlayManager_CreateAndGetData(man, sizeof(UnkStruct_ov22_0225B85C), HEAP_ID_13);
    memset(arg, 0, sizeof(UnkStruct_ov22_0225B85C));
    Main_SetVBlankIntrCB(ov41_0224BBF0, arg);
    HBlankInterruptDisable();
    v1 = OverlayManager_GetArgs(man);
    *(SaveFashionDataSub **)arg = sub_0202B9B8(*(SaveFashionData **)v1, *(s32 *)((u8 *)v1 + 4));
    *((s32 *)&((UnkStruct_ov22_0225B85C *)arg)->unk_08) = (s32) * (s32 *)((u8 *)v1 + 4);
    *((s32 *)&((UnkStruct_ov22_0225B85C *)arg)->unk_0C) = (s32) * (s32 *)((u8 *)v1 + 8);
    ov41_02246130();
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    ov41_02246670(arg + 0x14, 0xE);
    v0[0] = *((s32 *)&((UnkStruct_ov22_0225B85C *)arg)->unk_14.unk_40);
    v0[1] = 0x48;
    v0[2] = 0x10;
    v0[3] = 0xE;
    *((void **)&((UnkStruct_ov22_0225B85C *)arg)->unk_10) = ov41_0224B530(&v0[0], *(SaveFashionDataSub **)arg);
    ov41_0224BC04(arg);
    ov41_0224BCA4(arg);
    ov41_0224BCF0(arg);
    ov41_0224BDCC(arg);
    ov41_0224BE5C(arg);
    return 1;
}
