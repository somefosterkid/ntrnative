#include "global.h"

#include "map_object.h"
#include "overlay_01_021FD41C.h"
#include "unk_020689C8.h"


int sub_02068D90(const OverworldAnimManager *manager);

int ov01_021FD784(OverworldAnimManager *param0, void *param1);

int ov01_021FD784(OverworldAnimManager *param0, void *param1) {
    UnkStruct_ov5_021F15B4 *v0;
    const UnkStruct_ov5_021F1570 *v1;
    const void *userData;

    v0 = param1;
    userData = sub_02068D98(param0);
    v1 = userData;

    v0->unk_14 = *v1;
    v0->unk_10 = sub_02068D90(param0);
    v0->unk_00 = MapObject_GetSpriteID(v0->unk_14.unk_08);
    v0->unk_04 = MapObject_GetID(v0->unk_14.unk_08);

    if (MapObject_CheckFlag25(v0->unk_14.unk_08) == 1) {
        v0->unk_08 = sub_0205F544(v0->unk_14.unk_08);
    } else {
        v0->unk_08 = MapObject_GetMapID(v0->unk_14.unk_08);
    }

    v0->unk_0C = 1;
    return 1;
}
