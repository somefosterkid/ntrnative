#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF464.h"
#include "overworld_anim_manager.h"
#include "unk_020689C8.h"

void sub_02023EA4(Billboard *billboard, u8 draw);
void sub_02023F04(Billboard *billboard, fx32 numFrames);
void sub_02023F1C(Billboard *billboard, fx32 frameNum);
fx32 sub_02023F70(const Billboard *billboard);

void ov01_021FF5C4(OverworldAnimManager *param_1, UnkOv01_021FF464Work *work) {
    BOOL BVar1;
    s32 iVar2;
    s32 iVar3;
    LocalMapObject *object;

    object = work->args.mapObject;
    BVar1 = sub_0205F0F8(object, work->spriteId, work->objectId, work->mapId);
    if (BVar1 == 0) {
        ov01_021F1640(param_1);
        return;
    }
    iVar2 = sub_02068D90(param_1);
    if ((iVar2 == 1) && (BVar1 = MapObject_CheckFlag26(object), BVar1 == 0)) {
        ov01_021F1640(param_1);
        return;
    }
    BVar1 = MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE);
    if (BVar1 == 1) {
        sub_02023EA4(work->billboard, 0);
    } else {
        sub_02023EA4(work->billboard, 1);
    }
    if (work->state == 0) {
        sub_02023F04(work->billboard, 0x1000);
        iVar3 = sub_02023F70(work->billboard);
        if ((iVar3 / 0x1000) >= 0xC) {
            if (iVar2 == 0) {
                ov01_021F1640(param_1);
                return;
            }
            sub_02023F1C(work->billboard, 0);
        }
    }
    return;
}
