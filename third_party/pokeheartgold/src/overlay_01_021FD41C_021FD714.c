#include "global.h"

#include "overlay_01_021FD41C.h"
#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"

void ov01_021FD718(OverworldAnimManager *a0, UnkStruct_ov5_021F15B4 *arg1);

void ov01_021FD714(void) {
}

void ov01_021FD718(OverworldAnimManager *a0, UnkStruct_ov5_021F15B4 *arg1) {
    VecFx32 positionVector;
    LocalMapObject *object;

    object = arg1->unk_14.unk_08;
    if (sub_0205F0F8(object, arg1->unk_00, arg1->unk_04, arg1->unk_08) == 0) {
        ov01_021F1640(a0);
        return;
    }
    if (sub_0205F5E8(object, MAPOBJECTFLAG_END_MOVEMENT) != 0) {
        ov01_021F1640(a0);
        return;
    }
    arg1->unk_0C = 0;
    if (MapObject_TestFlagsBits(object, (MapObjectFlagBits)(0x100200)) == 1) {
        arg1->unk_0C = 1;
        return;
    }
    MapObject_CopyPositionVector(object, &positionVector);
    sub_02068DA8(a0, &positionVector);
}
