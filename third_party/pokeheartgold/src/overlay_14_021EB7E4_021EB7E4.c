#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8434(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F43F4(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F6688(void *, s32);
s32 ov14_021F685C(void *, u32, s32, s32);

s32 ov14_021E9434(UnkStruct_ov14_boxapp *app);

extern s32 ov14_021F7D1C;

extern s32 ov14_021F7D2C;

extern s32 ov14_021F7D3C;

void ov14_021EB7E4(void *arg0) {
    s32 temp_r0;
    s32 var_r4;
    void *temp_r0_2;
    ov14_021F6688(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0x25);
    temp_r0 = *((s32 *)(((u8 *)(*((void **)arg0))) + 8));
    switch (temp_r0) {
    case 1:
        ov14_021F5EE4(arg0, &ov14_021F7D2C, 4);
        ov14_021F685C(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21, 1, 0x27);
        var_r4 = 0x5A;
        break;

    case 0:
        ov14_021F5EE4(arg0, &ov14_021F7D1C, 4);
        ov14_021F685C(arg0, ((UnkStruct_ov14_boxapp *)arg0)->unk21, 1, 0x27);
        var_r4 = 0x72;
        break;

    case 2:
    default:
        ov14_021F5EE4(arg0, &ov14_021F7D3C, 5);
        if (((u32)(((UnkStruct_ov14_boxapp *)arg0)->unk21)) < 0x1EU) {
            var_r4 = 0x4D;
        } else {
            var_r4 = 0x4E;
        }
        break;
    }

    temp_r0_2 = ((void *)((UnkStruct_ov14_boxapp *)arg0)->sys);
    if (((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_44E_7 == 1) {
        ((UnkStruct_ov14_boxsys *)temp_r0_2)->unk_44E_7 = 0;
        if (((u32)(((UnkStruct_ov14_boxapp *)arg0)->unk21)) >= 0x1EU) {
            ov14_021F3488(arg0, 2, 0);
        } else {
            ov14_021F43F4(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 1);
            ov14_021F3488(arg0, 1, 0);
        }
    }
    ov14_021E8434(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9434, var_r4);
}
