#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021FEEEC.h"
#include "overworld_anim_manager.h"
#include "unk_0205FD20.h"
#include "unk_020689C8.h"

Billboard *ov01_021F1740(FieldEffectManager *fieldEffMan, u32 id, const VecFx32 *position);
void sub_02023EA4(Billboard *billboard, u8 draw);
void sub_02023F1C(Billboard *billboard, fx32 frameNum);

BOOL ov01_021FFCA8(OverworldAnimManager *manager, FieldEffectAnimWork *work);

BOOL ov01_021FFCA8(OverworldAnimManager *manager, FieldEffectAnimWork *work) {
    VecFx32 VStack_20;

    work->args = *(const FieldEffectAnimArgs *)sub_02068D98(manager);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    VStack_20.x = work->args.pos.x << 0x10;
    VStack_20.z = work->args.pos.z << 0x10;
    VStack_20.y = MapObject_GetPositionVectorYCoord(work->args.mapObject);
    work->heightSet = sub_0206121C(work->args.fieldSystem, &VStack_20);
    VStack_20.x = VStack_20.x + 0x7800;
    VStack_20.y += -0x8000;
    VStack_20.z = VStack_20.z + 0xd000;
    sub_02068DA8(manager, &VStack_20);
    work->billboard = ov01_021F1740(work->args.fieldEffMan, 10, &VStack_20);
    if (MapObject_TestFlagsBits(work->args.mapObject, MAPOBJECTFLAG_VISIBLE) == 1) {
        sub_02023EA4(work->billboard, 0);
    }
    if (sub_02068D90(manager) == 0) {
        sub_02023F1C(work->billboard, 0x2000);
        work->state = 2;
    }
    return TRUE;
}
