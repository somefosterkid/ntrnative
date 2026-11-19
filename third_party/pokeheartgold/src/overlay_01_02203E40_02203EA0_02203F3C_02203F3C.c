#include "global.h"

#include "overlay_01_02203E40.h"
#include "overworld_anim_manager.h"

BOOL ov01_02203F3C(OverworldAnimManager *manager, UnkOv01_02203E40_Work *work) {
    work->data = *(const UnkOv01_02203E40_Data *)sub_02068D98(manager);
    work->unk0 = sub_02068D90(manager);
    return TRUE;
}
