#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_unk_ov01_021fda14_work_a.h"
#include "overworld_anim_manager.h"

extern const OverworldAnimManagerFuncs ov01_02208F88;

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: the funcs table the DS keeps in asm/overlay_01_021FDA14.s (same label), with the context size from the type, not the DS literal.
BOOL ov01_021FDAC0(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FDB34(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FDB44(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
void ov01_021FDBCC(OverworldAnimManager *manager, UnkOv01_021FDA14_WorkA *work);
const OverworldAnimManagerFuncs ov01_02208F88 = OVERWORLD_ANIM_MANAGER_FUNCS(UnkOv01_021FDA14_WorkA, ov01_021FDAC0, ov01_021FDB34, ov01_021FDB44, ov01_021FDBCC);
#endif

void ov01_021FDA74(LocalMapObject *mapObject, int type) {
    int priority;
    VecFx32 pos;
    UnkOv01_021FDA14_DataA data;

    data.fieldSystem = MapObject_GetFieldSystem(mapObject);
    data.fieldEffMan = ov01_021F146C(mapObject);
    data.renderer = ov01_021F1450(data.fieldEffMan, 1);
    data.mapObject = mapObject;

    MapObject_CopyPositionVector(mapObject, &pos);

    priority = MapObject_GetPriorityPlusValue(mapObject, 2);
    ov01_021F1620(data.fieldEffMan, &ov01_02208F88, &pos, type, &data, priority);
}
