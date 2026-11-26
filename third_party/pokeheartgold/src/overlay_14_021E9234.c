#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "pokemon_storage_system.h"
#include "bg_window.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_14_move_owner.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E65C4(void *);
s32 ov14_021E6CF8(void *, unsigned int, s32);
s32 ov14_021E7148(void *, void *);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021E9234(UnkStruct_ov14_boxapp *app);
s32 ov14_021E8514(UnkStruct_0201956C *);
s32 ov14_021E92AC(UnkStruct_ov14_boxapp *app);
s32 ov14_021E9370(UnkStruct_ov14_boxapp *app);

s32 sub_02019978(UnkStruct_0201956C *, s32);
s32 ov14_021E9234(UnkStruct_ov14_boxapp *arg0) {
    s32 slotno_p;
    s32 boxno_p;
    u16 v0;
    UnkStruct_ov14_boxsys *v1;
    MoveSet *v2;

    v1 = arg0->sys;
    v0 = v1->unk_10;
    switch (v0) {
    case 0:
        boxno_p = (s32) arg0->unk1F;
        slotno_p = 0;
        PCStorage_FindFirstEmptySlot(arg0->storage, &boxno_p, &slotno_p);
        ov14_021E6CF8(arg0, arg0->unk21, slotno_p);
        ov14_021F40E8(arg0, 0);
        arg0->unk21 = 0xFFU;
        v1->unk_10 = (u16)(v1->unk_10 + 1);
    default:
        break;
    case 1:
        if (ov14_021E65C4(arg0) == 0) {
            v2 = v1->unk_C;
            ov14_021F34C8(arg0->sys, v2->from, 1);
            ov14_021E7148(arg0, v2);
            v1->unk_10 = 0U;
            return 0;
        }
        break;
    }
    return 1;
}
s32 ov14_021E92AC(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 v3;
    UnkStruct_ov14_boxsys *v1;
    UnkStruct_ov14_boxsys *v2;

    v1 = arg0->sys;
    if (v1->unk_12 == 0x17) {
        if (GridInputHandler_IsButtonInputMode(v1->unk_2C) == 1) {
            GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        }
        arg0->sys->unk_12 = 0U;
        *((s16 *)&arg0->sys->unk_10) = 0;
        return 0;
    }
    ScheduleSetBgPosText(v1->unk_14, 3U, BG_POS_OP_SUB_ROT, 8);
    ov14_021F3210(arg0, 8);
    sub_02019978(arg0->sys->unk_2F0, 0xE);
    sub_02019978(arg0->sys->unk_2F0, 0xA);
    sub_02019978(arg0->sys->unk_2F0, 8);
    sub_02019978(arg0->sys->unk_2F0, 9);
    v3 = sub_020199E4(arg0->sys->unk_2F0, 0xB);
    ov14_021E8514(arg0->sys->unk_2F0);
    v0 = sub_020199E4(arg0->sys->unk_2F0, 0xB);
    if ((v3 == 1) && (v0 == 0)) {
        ov14_021E8328(arg0->sys->unk_2F0);
    }
    v2 = arg0->sys;
    v2->unk_12 = (u16)(v2->unk_12 + 1);
    return 1;
}
s32 ov14_021E9370(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    s32 v3;
    UnkStruct_ov14_boxsys *v1;
    UnkStruct_ov14_boxsys *v2;

    v1 = arg0->sys;
    if (v1->unk_12 == 0x17) {
        if (GridInputHandler_IsButtonInputMode(v1->unk_2C) == 1) {
            GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        }
        arg0->sys->unk_12 = 0U;
        *((s16 *)&arg0->sys->unk_10) = 0;
        return 0;
    }
    ScheduleSetBgPosText(v1->unk_14, 3U, BG_POS_OP_ADD_ROT, 8);
    ov14_021F3210(arg0, -8);
    sub_02019978(arg0->sys->unk_2F0, 0xE);
    sub_02019978(arg0->sys->unk_2F0, 0xA);
    sub_02019978(arg0->sys->unk_2F0, 8);
    sub_02019978(arg0->sys->unk_2F0, 9);
    v3 = sub_020199E4(arg0->sys->unk_2F0, 0xB);
    ov14_021E8514(arg0->sys->unk_2F0);
    v0 = sub_020199E4(arg0->sys->unk_2F0, 0xB);
    if ((v3 == 1) && (v0 == 0)) {
        ov14_021E8328(arg0->sys->unk_2F0);
    }
    v2 = arg0->sys;
    v2->unk_12 = (u16)(v2->unk_12 + 1);
    return 1;
}
