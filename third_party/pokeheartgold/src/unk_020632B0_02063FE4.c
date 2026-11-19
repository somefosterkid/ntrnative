#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

struct MapObject;
typedef struct MapObject MapObject;
void *sub_0205F40C(MapObject *mapObj);
typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov5_021ED01C;
void ov01_021F95CC(const MapObject *param0, Billboard **param1, int param2);
enum BerryGrowthStage {
    BERRY_GROWTH_STAGE_NONE = 0,
    BERRY_GROWTH_STAGE_PLANTED = 1,
    BERRY_GROWTH_STAGE_SPROUTED = 2,
    BERRY_GROWTH_STAGE_GROWING = 3,
    BERRY_GROWTH_STAGE_BLOOMING = 4,
    BERRY_GROWTH_STAGE_FRUIT = 5,
};
typedef struct BerryPatchGraphics {
    int graphicsResourceID;
    enum BerryGrowthStage lastGrowthStage;
    Billboard *graphicsObject;
    UnkStruct_ov5_021ED01C graphicsState;
} BerryPatchGraphics;
void MapObject_SetFlagsBits(MapObject *mapObj, u32 flag);
void ov01_021F9610(Billboard *param0, UnkStruct_ov5_021ED01C *param1);

void sub_02063FE4(MapObject *mapObject);
void sub_02063FFC(MapObject *mapObject);

void sub_02063FE4(MapObject *mapObject) {
    BerryPatchGraphics *graphicsData = sub_0205F40C(mapObject);
    ov01_021F95CC(mapObject, &graphicsData->graphicsObject, graphicsData->graphicsResourceID);
}

void sub_02063FFC(MapObject *mapObject) {
    BerryPatchGraphics *graphicsData = sub_0205F40C(mapObject);

    if (graphicsData->graphicsObject != 0L) {
        ov01_021F9610(graphicsData->graphicsObject, &graphicsData->graphicsState);
    }

    ov01_021F95CC(mapObject, &graphicsData->graphicsObject, graphicsData->graphicsResourceID);
    MapObject_SetFlagsBits(mapObject, (1 << 21));
}
