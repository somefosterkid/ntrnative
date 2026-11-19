#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021F0A80(UnkStruct_ov14_boxapp *arg0);

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021E8234(void *);
s32 ov14_021E8294(void *);
s32 ov14_021E8314(void *);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F43F4(struct UnkStruct_ov14_boxsys *, s32);
void ov14_021F0AD8(struct UnkStruct_ov14_boxapp *arg0);
struct UnkStruct_ov14_boxapp;

s32 ov14_021E9434(UnkStruct_ov14_boxapp *app);

void ov14_021F0A80(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(arg0->sys, 9, 0);
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E9434, 0x13);
}

s32 ov14_021E9450(UnkStruct_ov14_boxapp *app);

void ov14_021F0AAC(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(arg0->sys, 9, 0);
    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E9450, 0x19);
}

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

void ov14_021F0AD8(UnkStruct_ov14_boxapp *arg0) {
    *((s8 *)&arg0->unk23[0]) = 1;
    ov14_021F43F4(*((void * *)&arg0->sys), 0);
    ov14_021F3488(arg0, 1, 1);
    ov14_021F2A18(*((void * *)&arg0->sys), 9, 0);
    ov14_021E8234(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8294(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8314(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E94BC, 0x22);
}
