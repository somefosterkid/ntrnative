#include "global.h"
#include "overlay_14.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021E76B8(void *);
s32 ov14_021F0CD8(void *);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F6688(void *, s32);
s32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021F1058(void *, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F63B8(void *);
s32 ov14_021E7588(void *, s32);
s32 ov14_021F63A8(void *);
s32 ov14_021F6AC0(void *, s32, s32);
s32 ov14_021E7ED0(void *);
s32 ov14_021E7EE0(void *);
s32 ov14_021E81A8(void *, s32);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021F3044(void *);
s32 ov14_021F63F0(UnkStruct_ov14_boxsys *);
s32 ov14_021F1090(UnkStruct_ov14_boxapp *, s32);

s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);

s32 ov14_021ED1AC(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxsys *v0;
    ov14_021F6688(arg0->sys, 0x25);
    if ((arg0->unk2A) != 0) {
        v0 = arg0->sys;
        ov14_021F29E4(v0, 9, 8);
        ov14_021E76B8(arg0);
        return ov14_021F0CD8(arg0);
    }
    GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    return 0x29;
}

void ov14_021ED1E8(void *arg0) {
    ov14_021F63B8(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    if (((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 == 3) {
        ov14_021F3488(arg0, 0x81, 1);
    } else {
        ov14_021F3488(arg0, 1, 0);
    }
    if (ov14_021E6070(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk2B, 0xAC, 0) != 0) {
        ov14_021F6408(arg0, 0);
        ov14_021E8620(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    }
    *((s8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk23[1]) = 1;
    *((s8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk26[3]) = 1;
    ((UnkStruct_ov14_boxapp *)arg0)->unk25 = (u8) ((UnkStruct_ov14_boxapp *)arg0)->unk1F;
    ov14_021F1058(arg0, 0x28);
}

s32 ov14_021ED258(void *arg0) {
    ov14_021F63A8(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    if (((UnkStruct_ov14_boxapp *)arg0)->unk_0->unk8 == 3) {
        ov14_021F6AC0(arg0, 7, 0);
        ov14_021E7588(arg0, GridInputHandler_GetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C) + 0x1E);
        return 0x8B;
    }
    if (((UnkStruct_ov14_boxapp *)arg0)->unk2A != 0) {
        ov14_021F6AC0(arg0, 5, ((UnkStruct_ov14_boxapp *)arg0)->unk2B - 0x1E);
    } else {
        ov14_021F6AC0(arg0, 5, 0);
    }
    ov14_021E7588(arg0, GridInputHandler_GetNextInput(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2C) + 0x1E);
    return 0x24;
}

void ov14_021ED2C8(UnkStruct_ov14_boxapp *arg0) {
    arg0->unk25 = (u8) arg0->unk1F;
    ov14_021F1058(arg0, 0x30);
}

void ov14_021ED2DC(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E81A8(*(void **)&arg0->sys->unk_2F0, 1);
    ov14_021E7ED0(*(void **)&arg0->sys->unk_2F0);
    ov14_021E7EE0(*(void **)&arg0->sys->unk_2F0);
    ov14_021F63F0(((void *)arg0->sys));
    ov14_021F63B8(((void *)arg0->sys));
    ov14_021F3044(arg0);
    ov14_021F6408(arg0, 0);
    if (ov14_021E6070(arg0, arg0->unk2B, 0xAC, 0) != 0) {
        ov14_021E8620(arg0->sys->unk_2F0);
    }
    ov14_021F0234(arg0, ov14_021E9518, 0x28);
}

void ov14_021ED350(void *arg0) {
    ov14_021F1090(arg0, 0x32);
}
