#include "global.h"
struct UnkStruct_ov14_boxapp;
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F5EB4(void *, s32);
s32 ov14_021F6AC0(void *, s32, s32);
u32 ov14_021E7930(void *, u32);
s32 ov14_021E81B4(void *);
s32 ov14_021E87BC(void *);
s32 ov14_021F4428(void *);
s32 ov14_021F4530(void *);
s32 ov14_021F459C(void *);
s32 ov14_021F58B8(void *);
s32 ov14_021E8560(void *);
s32 ov14_021F6094(void *arg0);
void ov14_021ED7B8(void *arg0);

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

s32 ov14_021E95B4(UnkStruct_ov14_boxapp *app);

void ov14_021ED6D0(void *arg0) {
    s32 v0;

    ov14_021F29E4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 8);
    ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0);
    ov14_021F5EB4(arg0, 0);
    ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    v0 = *(s32 *)((u8 *)(*(void **)arg0) + 8);
    if (v0 == 1) {
        ov14_021F6AC0(arg0, 2, 0x1E);
        ov14_021F0234(arg0, ov14_021E95B4, 0x52);
        return;
    }
    if (v0 == 3) {
        ov14_021F6AC0(arg0, 6, 0x1E);
    } else {
        ov14_021F6AC0(arg0, 3, 0x1E);
    }
    ov14_021E8248(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021E82A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E94BC, 0x4D);
}

s32 ov14_021E9660(UnkStruct_ov14_boxapp *app);

void ov14_021ED760(UnkStruct_ov14_boxapp *arg0) {
    *((s8 *)&arg0->sys->unk_44D) = ov14_021E7930(arg0, arg0->unk25);
    ov14_021F4428(arg0);
    ov14_021F4530(arg0);
    ov14_021F459C(arg0);
    ov14_021F58B8(arg0);
    ov14_021E87BC(((u8 *)arg0->sys->unk_2F0));
    ov14_021E81B4(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9660, 0x40);
}

s32 ov14_021E95C8(UnkStruct_ov14_boxapp *app);

void ov14_021ED7B8(void *arg0) {
    ov14_021F6094(arg0);
    ov14_021E8560(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E95C8, 0x41);
}
