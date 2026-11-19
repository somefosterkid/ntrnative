#include "global.h"

#include "billboard.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_022053EC.h"
#include "overlay_01_unk_ov01_021fda14_work_a.h"
#include "overworld_anim_manager.h"

int ov01_021F9744(const MapObjectManager *mapObjectManager, u32 spriteId, BillboardResources *res);
void *ov01_021F18F0(FieldEffectManager *fieldEffMan, u32 slotID);
Billboard *ov01_021F16EC(FieldEffectManager *fieldEffMan, const BillboardResources *res, const VecFx32 *pos);
void ov01_02205870(s32 param_1, LocalMapObject *param_2, Billboard *param_3, BillboardResources *res);

void ov01_021FE190(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work) {
    VecFx32 pos;
    BillboardResources res;
    void *modelRes;

    if (ov01_021F9744(MapObject_GetManager(work->data.mapObject), work->spriteId, &res) != 0 && ov01_021FA2D4(work->data.mapObject) != 1) {
        if (work->type <= 2) {
            modelRes = ov01_021F18F0(work->data.fieldEffMan, 2);
        } else {
            modelRes = ov01_021F18F0(work->data.fieldEffMan, 0xD);
        }
        res.modelRes = modelRes;
        if (ov01_0220553C(work->data.mapObject) != 0) {
            ov01_02205870(1, work->data.mapObject, work->billboard, &res);
        }
        sub_02068DB8(manager, &pos);
        work->billboard = ov01_021F16EC(work->data.fieldEffMan, &res, &pos);
        work->hasBillboard = 1;
    }
}
