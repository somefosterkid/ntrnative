#include "global.h"

#include "sys_task.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"

u8 ov01_021FA298(s32 id);

const BillboardAnim *ov01_021FA2AC(s32 id);
void ov01_021FA2B8(LocalMapObject *arg0, s32 arg1);

u8 ov01_021FA298(s32 id) {
    return ov01_021FA28C(id);
}

u8 ov01_021FA2A0(s32 id) {
    return sub_021FA248(id)->animId;
}

const BillboardAnim *ov01_021FA2AC(s32 id) {
    return sub_021FA248(id)->anims;
}

void ov01_021FA2B8(LocalMapObject *object, s32 arg1) {
    if (arg1 == 1) {
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK22);
        return;
    }
    MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK22);
}

BOOL ov01_021FA2D4(LocalMapObject *obj) {
    if (MapObject_GetFlagsBitsMask(obj, MAPOBJECTFLAG_UNK22) != 0) {
        return 1;
    }
    return 0;
}
