#include "global.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E7DF8(void *);
s32 ov14_021E7E40(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F638C(void *);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F67A4(void *, s32);
s32 ov14_021F6AC0(void *, s32, s32);
struct UnkStruct_ov14_boxapp;

s32 ov14_021E94A8(UnkStruct_ov14_boxapp *app);

void ov14_021EBDE8(UnkStruct_ov14_boxapp *arg0) {
    arg0->sys->unk_40C0 = ov14_021E6070(arg0, arg0->unk21, 0xB, 0);
    ov14_021E7DF8(arg0);
    ov14_021F638C(*((void * *)&arg0->sys));
    ov14_021E7E40(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E94A8, 0x15);
}

s32 ov14_021EBE2C(void *arg0) {
    ov14_021F6AC0(arg0, 8, 0);
    ov14_021F3488(arg0, 1, 1);
    ov14_021F3488(arg0, 2, 1);
    ov14_021F34C8(((UnkStruct_ov14_boxapp *)arg0)->sys, *(u8 *)((u8 *)arg0 + 0x21), 0);
    ov14_021F67A4(arg0, 0x26);
    return 0x16;
}
