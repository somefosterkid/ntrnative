// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov128/gimmick_passing_vehicles.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/sound.h"
#include "field/field.h"
#include "field/field_exp_obj.h"
#include "field/field_player.h"
#include "field/field_sound.h"
#include "field/gimmick_passing_vehicles.h"
#include "gfl/g3d.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/math.h"
#include "system/game_data.h"
#include "system/game_system.h"

// A gimmick of models that cross the zone along a road, one way in one lane and the other way in the other, waiting a
// random time between crossings. The models are the six of archive 217, each twice: a pair of models make one vehicle,
// and the lanes use the copies. A vehicle's sound plays once, when it is next to the player. The file name is a guess

#define GIMMICK_WORK_ID 1
// The archive of the models, which has no name yet
#define GIMMICK_ARCID 217
#define MODEL_COUNT 6
#define LANE_COUNT 3
// The vehicles' road: where they start and where they end, along x
#define ROAD_START_X FX32_CONST(6000)
#define ROAD_END_X FX32_CONST(7264)
// A vehicle is two models, the second behind the first
#define VEHICLE_LENGTH FX32_CONST(-96)

// A vehicle on its way across
typedef struct {
    // How many frames it has moved, in units of 16
    s32 distance;
    // How many frames it still waits before the next crossing
    u16 wait;
    // Which pair of models it is
    u16 lane;
    // Its sound has played
    BOOL soundPlayed;
} Vehicle;

typedef struct {
    u32 unused[2];
    // The one that goes up the road, and the one that comes down
    Vehicle vehicles[2];
} GimmickWork;

static void func_ov128_021eecfc(FieldSound *sound, const VecFx32 *playerPos, Vehicle *vehicle, fx32 vehicleX, u32 se);
static void func_ov058_021f38f4(GimmickWork *work, Field *field);
static void func_ov058_021f391c(GimmickWork *work, Field *field, u32 first, u32 second);
void func_ov021_021be4a4(FieldExpObjSystem *system, u16 scene, u16 actor, BOOL value);

// pokebw2's sResources
static const G3DSceneResourceSetup data_ov058_021f3a3c[MODEL_COUNT] = {
#include "asset/gimmick_passing_vehicles_data_ov058_021f3a3c.inc"
};

// pokebw2's sActors
static const G3DSceneActorSetup data_ov058_021f3a84[MODEL_COUNT * 2] = {
#include "asset/gimmick_passing_vehicles_data_ov058_021f3a84.inc"
};

// pokebw2's sSceneSetup
static G3DSceneSetup data_ov058_021f3b60 = { data_ov058_021f3a3c, NELEMS(data_ov058_021f3a3c), data_ov058_021f3a84, NELEMS(data_ov058_021f3a84) };

void func_ov058_021f36c0(Field *field) {
    FieldExpObjSystem *expObj;
    GimmickWork *work;
    HeapID heapId;

    heapId = Field_GetHeapID(field);
    expObj = Field_GetExpObjSystem(field);
    GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    Field_AllocGimmickWorkBlock(field, GIMMICK_WORK_ID, Field_GetHeapID(field), sizeof(GimmickWork));
    work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);
    LoadFieldExpandObjData(expObj, &data_ov058_021f3b60, 0);
    func_ov058_021f38f4(work, field);
}

void func_ov058_021f3714(Field *field) {
    GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    FieldExpObj_FreeScene(Field_GetExpObjSystem(field), 0);
    Field_DeleteGimmickWorkBlock(field, GIMMICK_WORK_ID);
}

void func_ov058_021f373c(Field *field) {
    VecFx32 playerPos;
    fx32 x0;
    fx32 x1;
    GameData *gameData;
    FieldExpObjSystem *expObj;
    GimmickWork *work;
    Vehicle *vehicle;
    SRTMatrix *matrix;
    FieldSound *sound;
    u32 index;

    gameData = GSYS_GetGameData(Field_GetGameSystem(field));
    GameData_GetGimmickState(gameData);
    expObj = Field_GetExpObjSystem(field);
    work = Field_GetGimmickWorkBlock(field, GIMMICK_WORK_ID);

    vehicle = &work->vehicles[0];
    if (vehicle->wait != 0) {
        vehicle->wait--;
    } else {
        index = vehicle->lane * 4;
        vehicle->distance++;
        matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, index);
        matrix->translation.x = (vehicle->distance << 16) + ROAD_START_X;
        if (matrix->translation.x >= ROAD_END_X) {
            vehicle->distance = 0;
            work->vehicles[0].soundPlayed = FALSE;
            vehicle->wait = GFL_RandomLC(30) + 30;
            vehicle->lane = GFL_RandomLC(3);
        }
        x0 = matrix->translation.x;
        matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, index + 1);
        matrix->translation.x = (vehicle->distance << 16) + ROAD_START_X + VEHICLE_LENGTH;
        if (matrix->translation.x >= ROAD_END_X) {
            vehicle->distance = 0;
        }
    }

    vehicle = &work->vehicles[1];
    if (vehicle->wait != 0) {
        vehicle->wait--;
    } else {
        index = vehicle->lane * 4 + 2;
        vehicle->distance++;
        matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, index);
        matrix->translation.x = ROAD_END_X - (vehicle->distance << 16);
        if (matrix->translation.x <= ROAD_START_X) {
            vehicle->distance = 0;
            work->vehicles[1].soundPlayed = FALSE;
            vehicle->wait = GFL_RandomLC(30);
            vehicle->lane = GFL_RandomLC(3);
        }
        x1 = matrix->translation.x;
        matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, index + 1);
        matrix->translation.x = ROAD_END_X - VEHICLE_LENGTH - (vehicle->distance << 16);
        if (matrix->translation.x <= ROAD_START_X) {
            vehicle->distance = 0;
        }
    }

    FieldPlayer_GetWPos(Field_GetPlayer(field), &playerPos);
    if (playerPos.z >= FX32_CONST(8224) && playerPos.z <= FX32_CONST(8512)) {
        sound = GameData_GetFieldSoundSystem(gameData);
        if (playerPos.x - x0 >= 0 && playerPos.x - x0 <= FX32_CONST(64) && work->vehicles[0].soundPlayed == FALSE) {
            if (!GFL_SndPlayerIsActive(GFL_SndSeqGetPlayerIndex(SEQ_SE_FLD_140))) {
                work->vehicles[0].soundPlayed = TRUE;
                FieldSnd_PlayAmbience(sound, SEQ_SE_FLD_140);
            }
        }
        if (x1 - playerPos.x >= 0 && x1 - playerPos.x <= FX32_CONST(64) && work->vehicles[1].soundPlayed == FALSE) {
            if (!GFL_SndPlayerIsActive(GFL_SndSeqGetPlayerIndex(SEQ_SE_FLD_141))) {
                work->vehicles[1].soundPlayed = TRUE;
                FieldSnd_PlayAmbience(sound, SEQ_SE_FLD_141);
            }
        }
    }
}

static void func_ov058_021f38f4(GimmickWork *work, Field *field) {
    func_ov058_021f391c(work, field, 0, 1);
    func_ov058_021f391c(work, field, 4, 5);
    func_ov058_021f391c(work, field, 8, 9);
}

static void func_ov058_021f391c(GimmickWork *work, Field *field, u32 first, u32 second) {
    FieldExpObjSystem *expObj;
    SRTMatrix *matrix;
    fx32 cos;
    fx32 sin;
    fx32 cos2;
    fx32 sin2;

    Field_GetHeapID(field);
    expObj = Field_GetExpObjSystem(field);

    matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, first);
    matrix->translation.x = ROAD_START_X;
    matrix->translation.y = FX32_CONST(80);
    matrix->translation.z = FX32_CONST(8448);
    sin = FX_SinIdx(DEG_TO_IDX(90));
    cos = FX_CosIdx(DEG_TO_IDX(90));
    MTX_RotY33_(&matrix->rotation, sin, cos);
    func_ov021_021be4a4(expObj, 0, first, 1);

    matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, second);
    matrix->translation.x = ROAD_START_X + VEHICLE_LENGTH;
    matrix->translation.y = FX32_CONST(80);
    matrix->translation.z = FX32_CONST(8448);
    MTX_RotY33_(&matrix->rotation, sin, cos);
    func_ov021_021be4a4(expObj, 0, second, 1);

    matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, first + 2);
    matrix->translation.x = ROAD_END_X;
    matrix->translation.y = FX32_CONST(80);
    matrix->translation.z = FX32_CONST(8368);
    sin2 = FX_SinIdx(DEG_TO_IDX(270));
    cos2 = FX_CosIdx(DEG_TO_IDX(270));
    MTX_RotY33_(&matrix->rotation, sin2, cos2);
    func_ov021_021be4a4(expObj, 0, first + 2, 1);

    matrix = FieldExpObj_GetActorMatrixPtr(expObj, 0, second + 2);
    matrix->translation.x = ROAD_END_X - VEHICLE_LENGTH;
    matrix->translation.y = FX32_CONST(80);
    matrix->translation.z = FX32_CONST(8368);
    MTX_RotY33_(&matrix->rotation, sin2, cos2);
    func_ov021_021be4a4(expObj, 0, second + 2, 1);
}
