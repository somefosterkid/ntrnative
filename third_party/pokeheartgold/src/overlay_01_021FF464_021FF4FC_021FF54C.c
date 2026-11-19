#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF464.h"
#include "overworld_anim_manager.h"
#include "unk_020689C8.h"

Billboard *ov01_021F1740(FieldEffectManager *fieldEffMan, u32 id, const VecFx32 *position);
void sub_02023EA4(Billboard *billboard, u8 draw);
void sub_02023F04(Billboard *billboard, fx32 numFrames);
void sub_02023F1C(Billboard *billboard, fx32 frameNum);
fx32 sub_02023F70(const Billboard *billboard);

BOOL ov01_021FF54C(OverworldAnimManager *manager, UnkOv01_021FF464Work *work) {
    VecFx32 vec = { 0, 0, 0 };
    work->args = *(const UnkOv01_021FF464Args *)sub_02068D98(manager);
    work->spriteId = MapObject_GetSpriteID(work->args.mapObject);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    sub_02068DA8(manager, &vec);
    work->billboard = ov01_021F1740(work->args.fieldEffMan, 3, &vec);
    if (MapObject_TestFlagsBits(work->args.mapObject, (MapObjectFlagBits)(2 << 8)) == 1) {
        sub_02023EA4(work->billboard, 0);
    }
    return 1;
}
