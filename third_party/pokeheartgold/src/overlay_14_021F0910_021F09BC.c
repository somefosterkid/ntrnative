#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F09BC(void *arg0);

s32 ov14_021E7E64(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F57B8(void *);
s32 ov14_021F6654(void *, s32);
s32 ov14_021F6AC0(void *, s32, s32);

s32 ov14_021E94A8(UnkStruct_ov14_boxapp *app);

void ov14_021F09BC(void *arg0) {
    s32 temp_r1;
    temp_r1 = *((s32 *)(((u8 *)(*((void **)arg0))) + 8));
    switch (temp_r1) {
    case 0:
        ov14_021F6AC0(arg0, 0, 9);
        break;

    case 1:
        ov14_021F6AC0(arg0, 2, 0x24);
        break;

    case 2:
    default:
        if (((u32)(((UnkStruct_ov14_boxapp *)arg0)->unk21)) < 0x1EU) {
            ov14_021F6AC0(arg0, 3, 0x27);
        } else {
            ov14_021F6AC0(arg0, 5, 0xB);
        }
        break;
    }

    ov14_021F2A18(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 9, 0);
    ov14_021F6654(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys), 0x26);
    if ((*((u8 *)&((UnkStruct_ov14_boxapp *)arg0)->unk23[1])) != 0) {
        ov14_021F57B8(arg0);
    }
    if (((u32)(((UnkStruct_ov14_boxapp *)arg0)->unk21)) < 0x1EU) {
        ov14_021F3488(arg0, 1, 0);
    }
    ov14_021F3488(arg0, 2, 0);
    ov14_021E7E64(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    if (((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) == 2) && (((u32)(((UnkStruct_ov14_boxapp *)arg0)->unk21)) < 0x1EU)) {
        ov14_021F0234(arg0, ov14_021E94A8, 0x17);
        return;
    }
    ov14_021F0234(arg0, ov14_021E94A8, 0xE);
}
