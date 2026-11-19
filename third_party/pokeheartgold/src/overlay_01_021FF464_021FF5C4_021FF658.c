#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF464.h"
#include "overworld_anim_manager.h"
#include "unk_020689C8.h"

void sub_02023E50(Billboard *billboard, const VecFx32 *pos);

void ov01_021FF658(OverworldAnimManager *param_1, UnkOv01_021FF464Work *work) {
    BOOL BVar1;
    LocalMapObject *object;
    VecFx32 VStack_1c;
    VecFx32 VStack_28;

    object = work->args.mapObject;
    BVar1 = sub_0205F0F8(object, work->spriteId, work->objectId, work->mapId);
    if (BVar1 == 0) {
        ov01_021F1640(param_1);
        return;
    }
    MapObject_CopyPositionVector(object, &VStack_1c);
    MapObject_CopyFacingVector(object, &VStack_28);
    VStack_1c.x = VStack_1c.x + VStack_28.x;
    VStack_1c.z = VStack_1c.z + VStack_28.z;
    VStack_1c.z += 0x8000;
    sub_02023E50(work->billboard, &VStack_1c);
    return;
}
