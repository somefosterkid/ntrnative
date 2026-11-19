#include "global.h"

#include "map_object.h"
#include "overlay_01_02203E40.h"
#include "overworld_anim_manager.h"

void ov01_02203F68(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work) {
}

void ov01_02203F6C(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work) {
    VecFx32 pos;
    VecFx32 dest;

    MapObject_CopyPositionVector(work->data.mapObject, &pos);
    VEC_MultAdd(work->data.dist, &work->data.direction, &pos, &dest);
    sub_02068DA8(manager, &dest);
}
