#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022031C0.h"
#include "unk_0205FD20.h"
#include "unk_020689C8.h"
#include "vram_transfer_manager.h"

Billboard *ov01_021F771C(MapObjectManager *);

s32 ov01_022039E0(u32);

s32 sub_02023E78(Billboard *, VecFx32 *);

s32 sub_02023FB0(const Billboard *);

extern s32 ov01_022094BC;

extern s32 ov01_02209B18;

void ov01_02203654(OverworldAnimManager *arg0, UnkOv01_022031C0_Work *arg1);

void ov01_02203654(OverworldAnimManager *arg0, UnkOv01_022031C0_Work *arg1) {
    VecFx32 sp1C;
    VecFx32 sp10;
    VecFx32 sp4;
    LocalMapObject *sp0;
    LocalMapObject *temp_r0_5;
    LocalMapObject *temp_r6;
    s32 temp_r0_2;
    s32 temp_r0_4;
    s32 temp_r1;
    s32 temp_r4;
    s32 temp_r4_3;
    Billboard *temp_r7;
    u8 temp_r4_2;

    sp0 = arg1->data.mapObject;
    if (sub_0205F0F8(sp0, arg1->spriteId, arg1->objectId, arg1->mapId) == 0) {
        ov01_021F1640(arg0);
        return;
    }
    if (arg1->unk14 == 0) {
        sub_02068DB8((void *)arg0, &sp1C);
        sp10 = arg1->data.pos;
        temp_r0_2 = sub_0206121C(arg1->data.fieldSystem, &sp10);
        arg1->unk14 = temp_r0_2;
        if (temp_r0_2 == 1) {
            sp1C.y = sp10.y;
            sub_02068DA8(arg0, &sp1C);
        }
    }
    switch (arg1->state) {
    case 0:
        temp_r6 = FollowMon_GetMapObject(arg1->data.fieldSystem);
        temp_r7 = ov01_021F771C(arg1->data.fieldSystem->mapObjectManager);
        temp_r4 = sub_02023FB0(temp_r7);
        GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (u32)(sub_02023FB0(temp_r7) << 0x10) >> 0xD, &ov01_02209B18, ((0xFFFF0000 & temp_r4) >> 0x10) * 8);
        if (ov01_022055DC(temp_r6) != 0) {
            temp_r4_2 = MapObject_GetFacingDirection(temp_r6);
            MapObject_CopyPositionVector(temp_r6, &arg1->followPos);
            ov01_022039BC(temp_r4_2, &arg1->followPos, &arg1->coordPtr);
            arg1->coordStart = *arg1->coordPtr;
            arg1->delta = ov01_022039E0(temp_r4_2);
        } else {
            arg1->coordPtr = NULL;
        }
        arg1->state = arg1->state + 1;
        /* fall through */
    case 1:
        sp4 = *(VecFx32 *)&ov01_022094BC;
        temp_r1 = arg1->counter + 1;
        arg1->counter = temp_r1;
        sp4.x = 0x1000 / temp_r1;
        sp4.y = 0x1000 / arg1->counter;
        sub_02023E78(ov01_021F771C(arg1->data.fieldSystem->mapObjectManager), &sp4);
        if (arg1->coordPtr != NULL) {
            temp_r0_5 = FollowMon_GetMapObject(arg1->data.fieldSystem);
            temp_r4_3 = arg1->counter * (arg1->delta << 0xC);
            *arg1->coordPtr = arg1->coordStart + (temp_r4_3 / 4);
            MapObject_SetPositionVector(temp_r0_5, &arg1->followPos);
        }
        if (arg1->counter >= 4) {
            arg1->state = arg1->state + 1;
            Field3dObject_SetActiveFlag(&arg1->data.manager->obj0, 1);
            MapObject_SetVisible(sp0, 1);
            arg1->counter = 0;
            return;
        }
        break;
    case 2:
        temp_r0_4 = arg1->counter + 1;
        arg1->counter = temp_r0_4;
        if (temp_r0_4 >= 4) {
            ov01_021F1640(arg0);
        }
        break;
    }
}
