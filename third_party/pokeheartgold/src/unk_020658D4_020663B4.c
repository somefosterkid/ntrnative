#include "global.h"

#include "overworld_anim_manager.h"
#include "sys_task.h"

struct MapObject;
typedef struct MapObject MapObject;
void sub_02066420(MapObject *param0, OverworldAnimManager *param1);
OverworldAnimManager *ov01_021FFF5C(MapObject *param0, int param1);
void MapObject_ClearSingleMovement(MapObject *mapObj);
void MapObject_ClearEndMovement(MapObject *mapObj);
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    OverworldAnimManager *unk_04;
} UnkStruct_0206A0BC;
void sub_02066420(MapObject *mapObj, OverworldAnimManager *param1);
OverworldAnimManager *sub_0206642C(MapObject *param0);
void MapObject_SetFlagsBits(MapObject *mapObj, u32 flag);
int sub_0205F73C(const MapObject *mapObj);
OverworldAnimManager *sub_0206642C(MapObject *mapObj);
void *sub_0205F394(MapObject *mapObj);

int sub_020663B4(MapObject *mapObj, UnkStruct_0206A0BC *param1);
int sub_020663E4(MapObject *mapObj, UnkStruct_0206A0BC *param1);
void sub_02066420(MapObject *mapObj, OverworldAnimManager *param1);

int sub_020663B4(MapObject *mapObj, UnkStruct_0206A0BC *param1) {
    if (param1->unk_02 == 0) {
        OverworldAnimManager *v0 = ov01_021FFF5C(mapObj, param1->unk_01);

        sub_02066420(mapObj, v0);
    }

    MapObject_ClearSingleMovement(mapObj);
    MapObject_ClearEndMovement(mapObj);
    param1->unk_00++;

    return 0;
}

int sub_020663E4(MapObject *mapObj, UnkStruct_0206A0BC *param1) {
    if (param1->unk_02 == 0) {
        OverworldAnimManager *v0 = sub_0206642C(mapObj);

        if (v0 == 0L) {
            if (sub_0205F73C(mapObj) == 1) {
                v0 = ov01_021FFF5C(mapObj, param1->unk_01);
                sub_02066420(mapObj, v0);
            }
        }

        MapObject_SetFlagsBits(mapObj, (1 << 20));
    }

    return 0;
}

void sub_02066420(MapObject *mapObj, OverworldAnimManager *param1) {
    UnkStruct_0206A0BC *v0 = sub_0205F394(mapObj);
    v0->unk_04 = param1;
}
