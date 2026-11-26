#include "global.h"
#include "overlay_14.h"
struct UnkStruct_ov14_boxapp;
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8234(void *);
s32 ov14_021E8294(void *);
s32 ov14_021E8314(void *);
s32 ov14_021E834C(UnkStruct_0201956C *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F10DC(void *, s32);
s32 ov14_021F2A18(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F6654(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021E87BC(void *);
void ov14_021ED6A4(void *arg0);
s32 ov14_021F1090(UnkStruct_ov14_boxapp *, s32);
s32 ov14_021F1058(void *, s32);
s32 ov14_021E875C(void *);
s32 ov14_021F6070(void *arg0);
void ov14_021ED62C(void *arg0);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F6844(void *, s32, s32);
s32 ov14_021F6AC0(void *, s32, u32);

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

void ov14_021ED5B0(void *arg0) {
    void *v0;

    v0 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
    if (((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 == 1) {
        if (ov14_021E834C(((UnkStruct_ov14_boxsys *)v0)->unk_2F0) == 0) {
            ov14_021E8314(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
        }
    } else {
        ov14_021E8234(((void *)((UnkStruct_ov14_boxsys *)v0)->unk_2F0));
        ov14_021E8294(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
        ov14_021E8314(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    }
    ov14_021F0234(arg0, ov14_021E94BC, 0x35);
}

void ov14_021ED60C(UnkStruct_ov14_boxapp *arg0) {
    arg0->unk25 = (u8) arg0->unk1F;
    ov14_021F1058(arg0, 0x36);
}

void ov14_021ED620(void *arg0) {
    ov14_021F10B4(arg0, 0x37);
}

s32 ov14_021E9618(UnkStruct_ov14_boxapp *app);

void ov14_021ED62C(void *arg0) {
    ov14_021F6070(arg0);
    ov14_021E875C(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9618, 0x38);
}

s32 ov14_021ED650(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    int new_var;
    ov14_021F6AC0(arg0, 9, *((u32 *)&arg0->unk2C));
    ;
    if (((u32)(*((u32 *)&arg0->unk2C))) <= 5U) {
        new_var = 9;
        ov14_021F29E4(arg0->sys, new_var, 0xE);
    } else {
        ov14_021F29E4(arg0->sys, 9, 8);
    }
    v0 = 0x3D;
    ov14_021F6844(arg0, 0, 0x27);
    return v0;
}

void ov14_021ED684(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F2A18(arg0->sys, 9, 0);
    ov14_021F6654(arg0->sys, 0x27);
    ov14_021F10DC(arg0, 0x3A);
}

void ov14_021ED6A4(void *arg0) {
    ov14_021E87BC(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9618, 0x3B);
}

void ov14_021ED6C4(void *arg0) {
    ov14_021F1090(arg0, 0x3C);
}
