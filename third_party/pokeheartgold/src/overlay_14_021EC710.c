#include "global.h"
#include "field_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7F4C(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021EC710(UnkStruct_ov14_boxapp *arg0);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F43F4(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F5C84(void *, s32);
s32 ov14_021F5E94(void *, s32);
s32 ov14_021F5EB4(void *, s32);
s32 ov14_021F6AC0(void *, s32, s32);

s32 ov14_021E9518(UnkStruct_ov14_boxapp *app);

void ov14_021EC710(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E7F4C(arg0->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E9518, 0x26);
}

s32 ov14_021E94BC(UnkStruct_ov14_boxapp *app);

void ov14_021EC730(void *arg0) {
    ov14_021E7E98(arg0);
    ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 1);
    ov14_021F5C84(arg0, 0);
    ov14_021F5E94(arg0, 0);
    if (*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk23[1]) == 0) {
        ov14_021F5EB4(arg0, 0);
    }
    ov14_021E8248(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021E82A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    if (*(s32 *)((u8 *)(*(void **)arg0) + 8) == 3) {
        ov14_021F3488(arg0, 0x81, 1);
        ov14_021F6AC0(arg0, 6, 0x21);
        ov14_021F0234(arg0, ov14_021E94BC, 0x75);
        return;
    }
    ov14_021F3488(arg0, 1, 0);
    ov14_021F6AC0(arg0, 3, 0x21);
    ov14_021F0234(arg0, ov14_021E94BC, 0xC);
}
