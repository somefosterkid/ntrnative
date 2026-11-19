#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022031C0.h"
#include "unk_0205FD20.h"
#include "unk_020689C8.h"

void ov01_022033E4(OverworldAnimManager *arg0, UnkOv01_022031C0_Work *arg1);

void ov01_022033E4(OverworldAnimManager *arg0, UnkOv01_022031C0_Work *arg1) {
    VecFx32 spC;
    VecFx32 sp0;
    LocalMapObject *temp_r6;
    s32 temp_r0_2;
    s32 temp_r0_3;
    u32 temp_r0;

    temp_r6 = arg1->data.mapObject;
    if (sub_0205F0F8(temp_r6, arg1->spriteId, arg1->objectId, arg1->mapId) == 0) {
        ov01_021F1640(arg0);
        return;
    }
    if (arg1->unk14 == 0) {
        sub_02068DB8((void *)arg0, &spC);
        sp0 = arg1->data.pos;
        temp_r0 = sub_0206121C(arg1->data.fieldSystem, &sp0);
        arg1->unk14 = temp_r0;
        if (temp_r0 == 1) {
            spC.y = sp0.y;
            sub_02068DA8(arg0, &spC);
        }
    }
    temp_r0_2 = arg1->state;
    switch (temp_r0_2) {
    case 0:
        temp_r0_3 = arg1->counter + 1;
        arg1->counter = temp_r0_3;
        if (temp_r0_3 >= 2) {
            arg1->state = 1;
            return;
        }
        return;
    case 1:
        Field3dObject_SetActiveFlag(&arg1->data.manager->obj0, 0);
        Field3dObject_SetActiveFlag(&arg1->data.manager->obj1, 1);
        sub_02069DC8(temp_r6, 0);
        sub_0205F484(temp_r6);
        Field3dModelAnimation_FrameSet(&arg1->data.manager->anim, 0);
        arg1->state = 2;
    case 2:
        if (Field3dModelAnimation_FrameAdvanceAndCheck(&arg1->data.manager->anim, 0x1000) != 0) {
            ov01_021F1640(arg0);
        }
        break;
    }
}
