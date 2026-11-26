#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022031C0.h"

Billboard *ov01_021F771C(MapObjectManager *param_1);


s32 ov01_022039E0(u32);

s32 sub_02023E78(Billboard *, void *);

extern s32 ov01_022094B0;

void ov01_02203890(OverworldAnimManager *arg0, UnkOv01_022031C0_Work *arg1);

void ov01_02203890(OverworldAnimManager *arg0, UnkOv01_022031C0_Work *arg1) {
    s32 sp0[3];
    LocalMapObject *temp_r0_2;
    LocalMapObject *temp_r6;
    LocalMapObject *temp_r7;
    s32 temp_r0;
    s32 temp_r0_3;
    s32 temp_r1;
    u8 temp_r4;
    temp_r7 = arg1->data.mapObject;
    if (sub_0205F0F8(temp_r7, arg1->spriteId, arg1->objectId, arg1->mapId) == 0) {
        ov01_021F1640(arg0);
        return;
    }
    temp_r0 = arg1->state;
    switch (temp_r0) {
    case 0:
        temp_r6 = FollowMon_GetMapObject(arg1->data.fieldSystem);
        ov01_021F771C(arg1->data.fieldSystem->mapObjectManager);
        if (ov01_022055DC(temp_r6) != 0) {
            temp_r4 = (u8)MapObject_GetFacingDirection(temp_r6);
            MapObject_CopyPositionVector(temp_r6, &arg1->followPos);
            ov01_022039BC(temp_r4, &arg1->followPos, &arg1->coordPtr);
            arg1->coordStart = *arg1->coordPtr;
            arg1->delta = ov01_022039E0(temp_r4);
        } else {
            arg1->coordPtr = 0L;
        }
        arg1->state = (s32)(arg1->state + 1);

    case 1:
        *((VecFx32 *)((u8 *)sp0)) = *((VecFx32 *)((u8 *)(&ov01_022094B0)));
        temp_r1 = arg1->counter + 1;
        arg1->counter = temp_r1;
        sp0[0] = 0x1000 / temp_r1;
        sp0[1] = 0x1000 / arg1->counter;
        sub_02023E78(ov01_021F771C(arg1->data.fieldSystem->mapObjectManager), sp0);
        if (arg1->coordPtr != 0L) {
            temp_r0_2 = FollowMon_GetMapObject(arg1->data.fieldSystem);
            *arg1->coordPtr = arg1->coordStart + (arg1->counter * (arg1->delta << 0xC)) / 4;
            MapObject_SetPositionVector(temp_r0_2, &arg1->followPos);
        }
        if (((s32)arg1->counter) >= 4) {
            arg1->state = (s32)(arg1->state + 1);
            Field3dObject_SetActiveFlag(&arg1->data.manager->obj0, 1);
            sub_02069DC8(temp_r7, 1);
            arg1->counter = 0;
            return;
        }
        return;

    case 2:
        temp_r0_3 = arg1->counter + 1;
        arg1->counter = temp_r0_3;
        if (temp_r0_3 >= 0x10) {
            ov01_021F1640(arg0);
        }
        break;
    }
}
