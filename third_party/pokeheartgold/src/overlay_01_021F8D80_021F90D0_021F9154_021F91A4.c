#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "filesystem.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"
#include "billboard.h"
#include "filesystem_files_def.h"

void sub_02023EA4(Billboard *billboard, u8 draw);

BOOL MapObject_TestFlagsBits(const LocalMapObject *mapObj, u32 flag);

void ov01_021F91A4(LocalMapObject *param0, Billboard *param1);

void ov01_021F91A4(LocalMapObject *param0, Billboard *param1) {
    int v0 = 1;

    if (MapObject_TestFlagsBits(param0, (1 << 9)) == 1) {
        v0 = 0;
    }

    if (MapObject_TestFlagsBits(param0, (1 << 12)) == 1) {
        if (!MapObject_TestFlagsBits(param0, (1 << 13))) {
            v0 = 0;
        }
    }

    sub_02023EA4(param1, v0);
}

u8 *sub_0205F40C(LocalMapObject *object);

void ov01_021F91E4(LocalMapObject *object);

void ov01_021F91E4(LocalMapObject *object) {
    MapObjectBillboardWorkC *work;

    work = (MapObjectBillboardWorkC *)sub_0205F40C(object);
    ov01_021F95A8(object, &work->billboard);
}

int MapObjectManager_GetObjectCount(const MapObjectManager *mapObjMan);

void MapObjectManager_SetFlagsBits(MapObjectManager *mapObjMan, u32 flag);

u32 MapObjectManager_GetFlagsBitsMask(const MapObjectManager *mapObjMan, u32 flag);

int MapObjectManager_GetPriority(const MapObjectManager *mapObjMan);

UnkStruct_ov5_021ED0A4 *sub_0205F1A0(const MapObjectManager *mapObjMan);

void ov01_021F944C(UnkStruct_ov5_021ED0A4 *param0, MapObjectManager *param1, int param2, int param3, int param4, int param5, const int *param6, int param7);

static void FldObjSys_OpenMModelNarc(MapObjectManager *param0);

void ov01_021F91F8(MapObjectManager *param0, int param1, int param2, const int *param3, int param4);

void ov01_021F91F8(MapObjectManager *param0, int param1, int param2, const int *param3, int param4) {
    int v0, v1;
    UnkStruct_ov5_021ED0A4 *v2;

    ((MapObjectManager_GetFlagsBitsMask(param0, (1 << 0)) == 0) ? (void)0 : GF_AssertFail());

    FldObjSys_OpenMModelNarc(param0);

    v0 = MapObjectManager_GetObjectCount(param0);
    v1 = MapObjectManager_GetPriority(param0) - 1;
    v2 = sub_0205F1A0(param0);

    ov01_021F944C(v2, param0, v0, v1, param1, param2, param3, param4);
    MapObjectManager_SetFlagsBits(param0, (1 << 0));
}

void MapObjectManager_ClearFlagsBits(MapObjectManager *mapObjMan, u32 flag);

int sub_0205F5D4(const MapObjectManager *mapObjMan);

void ov01_021F94A0(UnkStruct_ov5_021ED0A4 *param0);

void FldObjSys_CloseMModelNarc(MapObjectManager *param0);

void ov01_021F9250(MapObjectManager *param0);

void ov01_021F9250(MapObjectManager *param0) {
    int v0 = sub_0205F5D4(param0);
    ((v0 == 1) ? (void)0 : GF_AssertFail());

    ov01_021F94A0(sub_0205F1A0(param0));

    MapObjectManager_ClearFlagsBits(param0, (1 << 0));
    FldObjSys_CloseMModelNarc(param0);
}

void MapObjectManager_SetMapModelNarc(MapObjectManager *mapObjMan, NARC *narc);

static void FldObjSys_OpenMModelNarc(MapObjectManager *param0) {
    NARC *v0 = NARC_New(NARC_data_mmodel_mmodel, HEAP_ID_FIELD1);
    MapObjectManager_SetMapModelNarc(param0, v0);
}
