#include "global.h"

#include "billboard.h"
#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FDA14.h"
#include "overlay_01_unk_ov01_021fda14_work_a.h"
#include "overworld_anim_manager.h"

// ov01_021F72DC returns the map object's billboard (its header spells it Sprite *).
Billboard *ov01_021F72DC(LocalMapObject *obj);
void sub_02023E50(Billboard *billboard, const VecFx32 *pos);
void sub_02023E78(Billboard *billboard, const VecFx32 *scale);
void sub_02023EA4(Billboard *billboard, u8 draw);
void sub_02023EE0(Billboard *billboard, int animNum);
int sub_02023EF4(const Billboard *billboard);
void sub_02023F1C(Billboard *billboard, fx32 frameNum);
fx32 sub_02023F30(const Billboard *billboard);

void ov01_021FDBCC(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work) {
    VecFx32 facingVector;
    VecFx32 pos;
    LocalMapObject *object;
    Billboard *objBillboard;
    BOOL show;

    object = work->data.mapObject;
    show = FALSE;
    if (work->spriteId != MapObject_GetSpriteID(object) || sub_0205F0A8(object, work->objectId, work->mapId) == 0 || MapObject_CheckFlag24(object) == 0) {
        ov01_021F1640(manager);
        return;
    }
    if (work->hasBillboard != 0) {
        MapObject_CopyFacingVector(work->data.mapObject, &facingVector);
        if (facingVector.y != 0) {
            show = TRUE;
        }
        if (MapObject_CheckVisible(object) == 1) {
            show = TRUE;
        }
        if (show) {
            sub_02023EA4(work->billboard, 0);
        } else {
            sub_02023EA4(work->billboard, 1);
        }
        sub_02068DB8(manager, &pos);
        sub_02023E50(work->billboard, &pos);
        sub_02023E78(work->billboard, &work->scale);
        objBillboard = ov01_021F72DC(work->data.mapObject);
        sub_02023EE0(work->billboard, sub_02023EF4(objBillboard));
        sub_02023F1C(work->billboard, sub_02023F30(objBillboard));
    }
}
