#include "global.h"

#include "overlay_01_02203E40.h"
#include "overworld_anim_manager.h"

void ov01_02203F2C(OverworldAnimManager *animMan, f32 a0) {
    UnkOv01_02203E40_Work *work = sub_02068D74(animMan);
    work->data.obj->unk_64 = a0;
}
