#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_02200858_work.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"
#include "player_avatar.h"
#include "sys_task.h"
#include "unk_0205FD20.h"

s32 ov01_02200AB0(u8);

void ov01_0220093C(OverworldAnimManager *param0, void *param1);

void ov01_0220093C(OverworldAnimManager *param0, void *param1_) {
    UnkStruct_021F6264 *param1 = param1_;
    VecFx32 sp18;
    VecFx32 spC;
    u32 sp8;
    PlayerAvatar *sp4;
    OverworldAnimManager *sp0;
    LocalMapObject *temp_r0_2;
    PlayerAvatar *temp_r0;
    s32 temp_r0_3;
    s32 temp_r4;
    s32 temp_r7;
    s32 temp_r7_2;
    u32 temp_r4_2;

    sp0 = param0;
    temp_r0 = param1->unk_24.playerAvatar;
    sp4 = temp_r0;
    temp_r0_2 = PlayerAvatar_GetMapObject(temp_r0);
    if (sub_0205F0A8(temp_r0_2, param1->unk_14, param1->unk_18) == 0) {
        ov01_021F1640(sp0);
        return;
    }
    temp_r7 = param1->unk_04;
    param1->unk_08 = (u8)sub_0205F504(temp_r0_2);
    param1->unk_04 = ov01_02200AB0(param1->unk_08);
    param1->unk_00 = 0;
    temp_r4 = param1->unk_04;
    if ((temp_r4 == -1) || (temp_r4 != PlayerAvatar_GetFacingDirection(sp4))) {
        param1->unk_1C = 0;
        param1->unk_20 = 0;
        param1->unk_00 = 1;
        return;
    }
    if (temp_r7 != temp_r4) {
        param1->unk_1C = 0;
        param1->unk_20 = 0;
    }
    temp_r4_2 = MapObject_GetXCoord(temp_r0_2);
    temp_r7_2 = GetDeltaXByFacingDirection(param1->unk_04);
    sp8 = MapObject_GetZCoord(temp_r0_2);
    sub_020611C8(temp_r4_2 + temp_r7_2, sp8 + GetDeltaYByFacingDirection(param1->unk_04), &spC);
    MapObject_CopyPositionVector(temp_r0_2, &sp18);
    spC.y = sp18.y;
    sub_02068DA8(sp0, &spC);
    temp_r0_3 = param1->unk_1C + 1;
    param1->unk_1C = temp_r0_3;
    if (temp_r0_3 > 0xF) {
        param1->unk_1C = 0;
        param1->unk_20 = (s32)((param1->unk_20 + 1) % 2);
    }
}
