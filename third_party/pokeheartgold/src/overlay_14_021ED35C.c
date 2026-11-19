#include "global.h"
struct UnkStruct_ov14_boxapp;
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7F4C(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0C58(void *);
s32 ov14_021E7588(void *, s32, s32);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021F5C84(void *, s32);
s32 ov14_021F5E94(void *, s32);
s32 ov14_021F5EB4(void *, s32);
s32 ov14_021F6AC0(void *, s32, u32);

s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

void ov14_021ED35C(UnkStruct_ov14_boxapp *arg0) {
    if ((*((u8 *)&arg0->unk26[3])) == 1) {
        ov14_021F0C58(arg0);
        return;
    }
    ov14_021E7F4C(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9518, 0x33);
}

void ov14_021ED38C(void *arg0, s32 arg1, s32 arg2) {
    s32 v2;
    u8 v1;
    s32 v0;
    if ((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) == 3) {
        v2 = 6;
        v1 = 0x22;
        v0 = 0x75;
    } else {
        v2 = 3;
        v1 = 0x22;
        v0 = 0xC;
        if ((((UnkStruct_ov14_boxapp *)arg0)->unk2A) != 0) {
            v1 = ((UnkStruct_ov14_boxapp *)arg0)->unk2B;
            ov14_021E7588(arg0, v1, arg2);
        }
    }
    ov14_021F6AC0(arg0, v2, v1);
    ov14_021F5C84(arg0, 0);
    ov14_021F5E94(arg0, 0);
    ov14_021F5EB4(arg0, 0);
    ov14_021E8248(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021E82A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E94BC, v0);
}
