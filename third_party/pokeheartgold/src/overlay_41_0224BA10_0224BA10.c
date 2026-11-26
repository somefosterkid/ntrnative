#include "global.h"

#include "fashion_case.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "overlay_41_unk_struct_ov22_0225_b85_c.h"
#include "overlay_41_unk_struct_ov22_0225af8c.h"

s32 ov41_02246130(void);

s32 ov41_02246670(void *, s32);

void *ov41_0224B530(const UnkStruct_ov22_0225AF8C *, SaveFashionDataSub *);

void ov41_0224BBF0(void *);

s32 ov41_0224BC04(void *);

s32 ov41_0224BCA4(void *);

s32 ov41_0224BCF0(void *);

s32 ov41_0224BDCC(void *);

s32 ov41_0224BE5C(void *);

s32 AccessoryPortrait_Init(OverlayManager *man, s32 *state);

s32 AccessoryPortrait_Init(OverlayManager *man, s32 *state) {
    UnkStruct_ov22_0225AF8C v0;
    UnkStruct_ov22_0225B85C *arg;
    void *v1;

    Heap_Create(HEAP_ID_3, HEAP_ID_13, 0x20000U);
    Heap_Create(HEAP_ID_3, HEAP_ID_14, 0x40000U);
    arg = OverlayManager_CreateAndGetData(man, sizeof(UnkStruct_ov22_0225B85C), HEAP_ID_13);
    memset(arg, 0, sizeof(UnkStruct_ov22_0225B85C));
    Main_SetVBlankIntrCB(ov41_0224BBF0, arg);
    HBlankInterruptDisable();
    v1 = OverlayManager_GetArgs(man);
    arg->photo = sub_0202B9B8(*(SaveFashionData **)v1, *(s32 *)((u8 *)v1 + 4));
    arg->unk_08 = *(s32 *)((u8 *)v1 + 4);
    arg->unk_0C = *(s32 *)((u8 *)v1 + 8);
    ov41_02246130();
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    ov41_02246670(&arg->unk_14, 0xE);
    v0.unk_00 = arg->unk_14.unk_40;
    v0.unk_04 = 0x48;
    v0.unk_08 = 0x10;
    v0.heapID = HEAP_ID_14;
    arg->unk_10 = ov41_0224B530(&v0, arg->photo);
    ov41_0224BC04(arg);
    ov41_0224BCA4(arg);
    ov41_0224BCF0(arg);
    ov41_0224BDCC(arg);
    ov41_0224BE5C(arg);
    return 1;
}
