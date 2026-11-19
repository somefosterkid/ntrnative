#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "heap.h"
#include "item.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2A44(UnkStruct_ov14_boxsys *, s32);

s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021E88F8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F391C(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F6654(void *, s32);
s32 ov14_021F0254(void *, s16);
s32 ov14_021F68C0(void *, s32, s32);

s32 ov14_021F40DC(void *);

s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);

void ov14_021EEBFC(UnkStruct_ov14_boxapp *arg0) {
    if (ov14_021F2A44(*((void * *)&arg0->sys), 0xB) == 1) {
        ov14_021F40DC(arg0);
        ov14_021F391C(*((void * *)&arg0->sys), 1);
        ov14_021F29E4(*((void * *)&arg0->sys), 0xB, 2);
        ov14_021E88F8(((u8 *)arg0->sys->unk_2F0));
    }
    ov14_021F6654(*((void * *)&arg0->sys), 0x25);
    ov14_021E8328(arg0->sys->unk_2F0);
    if ((u32) arg0->unk21 < 0x1EU) {
        ov14_021E8248(((u8 *)arg0->sys->unk_2F0));
        ov14_021E82A8(((u8 *)arg0->sys->unk_2F0));
        ov14_021F3488(arg0, 0x81, 1);
    } else {
        ov14_021F3488(arg0, 0x82, 1);
    }
    ov14_021F0234(arg0, ov14_021E9450, 0x7B);
}

s32 ov14_021EEC8C(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x75;
}

s32 ov14_021EEC9C(void *arg0) {
    u8 py[2];
    u16 v0;

    if (((u32) * (u8 *)((u8 *)arg0 + 0x21) >= 0x1EU) && (ItemIdIsMail(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_88C8) == 1)) {
        v0 = *(u8 *)((u8 *)arg0 + 0x21) - 0x1E;
        GridInputHandler_SetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)(u8)v0);
        DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C, (s32)v0), &py[1], &py[0]);
        ManagedSprite_SetPositionXY(((UnkStruct_ov14_boxapp *)arg0)->sys->sprites[9], (s16) py[1], (s16) py[0]);
        PlaySE(0x5F3U);
        ov14_021F68C0(arg0, 4, 0x25);
        *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30) = 0x77;
        return 6;
    }
    ov14_021F68C0(arg0, 2, 0x25);
    return ov14_021F0254(arg0, 2);
}
