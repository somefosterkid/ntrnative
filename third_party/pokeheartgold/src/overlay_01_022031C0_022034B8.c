#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_022031C0.h"
#include "unk_020689C8.h"

void ov01_022034B8(OverworldAnimManager *param_1, UnkOv01_022031C0_Work *param_2);

void ov01_022034B8(OverworldAnimManager *param_1, UnkOv01_022031C0_Work *param_2) {
    BOOL BVar1;
    VecFx32 VStack_18;

    BVar1 = sub_0205F0F8(param_2->data.mapObject, param_2->spriteId, param_2->objectId, param_2->mapId);
    if (BVar1 == 0) {
        ov01_021F1640(param_1);
        return;
    }
    sub_02068DB8(param_1, &VStack_18);
    Field3dObject_Draw(&param_2->data.manager->obj0);
    Field3dObject_Draw(&param_2->data.manager->obj1);
    return;
}
