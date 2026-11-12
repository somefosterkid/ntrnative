// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov108/field_gimmick_bsubway.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_g3d_mapper.h"
#include "field/field_gimmick_bsubway.h"
#include "field/field_map.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "system/game_data.h"
#include "system/game_system.h"

// field_gimmick_bsubway.c, named by the string it passes to GFL_HeapAllocate. Each of the Battle Subway's zones 67
// to 76 keeps this in its gimmick state's user data. Zone 75's train shakes the map now and then; the others load
// the field effect of the train, which script plugin 1 starts and drives

// The field effect of the trains
#define FLDEFF_TRAIN 14

typedef struct {
    u16 unk0;
    // The zone's entry in data_ov052_021f39dc and data_ov052_021f3a2c
    u16 index;
    u16 gimmickId;
    u16 unk6;
    u32 zoneId;
    u16 heapId;
    u16 unkE;
    u8 unk10[12];
    GameSystem *gsys;
    Field *field;
    // A TrainWork, or zone 75's ShakeWork
    void *data;
} BSubwayGimmickWork;

typedef struct {
    void *effect;
} TrainWork;

typedef struct {
    // Set by script to stop the shaking
    u8 stop;
    u8 state;
    u8 waitIndex;
    s32 wait;
    // How far the map is moved up, alternating in sign as it dies down
    s32 shake;
} ShakeWork;

typedef struct {
    u32 zoneId;
    u32 gimmickId;
} BSubwayZoneGimmick;

typedef void (*BSubwayGimmickFunc)(BSubwayGimmickWork *work, Field *field);

typedef struct {
    BSubwayGimmickFunc setup;
    BSubwayGimmickFunc end;
    BSubwayGimmickFunc move;
} BSubwayGimmickFuncs;

static void func_ov052_021f3844(BSubwayGimmickWork *work, Field *field);
static void func_ov052_021f3880(BSubwayGimmickWork *work, Field *field);
static void func_ov052_021f38c0(BSubwayGimmickWork *work, Field *field);
static void func_ov052_021f38e4(BSubwayGimmickWork *work, Field *field);
static void func_ov052_021f38f0(BSubwayGimmickWork *work, Field *field);
static void func_ov052_021f3998(u32 zoneId, u32 *gimmickId, u32 *index);

// pokebw2's sZoneGimmicks
static const BSubwayZoneGimmick data_ov052_021f39dc[10] = {
#include "asset/field_gimmick_bsubway_data_ov052_021f39dc.inc"
};

// pokebw2's sGimmickFuncs
static const BSubwayGimmickFuncs data_ov052_021f3a2c[10] = {
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
    { func_ov052_021f38c0, func_ov052_021f38e4, func_ov052_021f38f0 },
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
    { func_ov052_021f3844, func_ov052_021f3880, NULL },
};

// pokebw2's FieldGimmickBSubway_Setup
void func_ov052_021f36c0(Field *field) {
    u16 heapId;
    GameSystem *gsys;
    GimmickState *state;
    u32 zoneId;
    u32 index;
    u32 gimmickId;
    BSubwayGimmickWork *work;

    heapId = Field_GetHeapID(field);
    gsys = Field_GetGameSystem(field);
    state = GameData_GetGimmickState(GSYS_GetGameData(gsys));
    zoneId = Field_GetPlayerStateZoneID(field);
    func_ov052_021f3998(zoneId, &gimmickId, &index);
    work = GimmickState_GetUserData(state, gimmickId);
    work->index = index;
    work->gimmickId = gimmickId;
    work->zoneId = zoneId;
    work->heapId = heapId;
    work->gsys = gsys;
    work->field = field;
    if (data_ov052_021f3a2c[work->index].setup != NULL) {
        data_ov052_021f3a2c[work->index].setup(work, field);
    }
}

// pokebw2's FieldGimmickBSubway_End
void func_ov052_021f3724(Field *field) {
    u16 heapId;
    GimmickState *state;
    u32 index;
    u32 gimmickId;
    BSubwayGimmickWork *work;

    heapId = Field_GetHeapID(field);
    state = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    func_ov052_021f3998(Field_GetPlayerStateZoneID(field), &gimmickId, &index);
    work = GimmickState_GetUserData(state, gimmickId);
    if (data_ov052_021f3a2c[work->index].end != NULL) {
        data_ov052_021f3a2c[work->index].end(work, field);
    }
}

// pokebw2's FieldGimmickBSubway_Move
void func_ov052_021f3770(Field *field) {
    GimmickState *state;
    u32 index;
    u32 gimmickId;

    state = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    func_ov052_021f3998(Field_GetPlayerStateZoneID(field), &gimmickId, &index);
    if (data_ov052_021f3a2c[index].move != NULL) {
        data_ov052_021f3a2c[index].move(GimmickState_GetUserData(state, gimmickId), field);
    }
}

// pokebw2's FieldGimmickBSubway_StartTrainEffect
void func_ov052_021f37c0(Field *field, u32 a1, const VecFx32 *pos) {
    u16 heapId;
    GimmickState *state;
    u32 index;
    u32 gimmickId;
    TrainWork *train;

    heapId = Field_GetHeapID(field);
    state = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    func_ov052_021f3998(Field_GetPlayerStateZoneID(field), &gimmickId, &index);
    train = ((BSubwayGimmickWork *)GimmickState_GetUserData(state, gimmickId))->data;
    train->effect = func_ov021_021d0218(Field_GetFieldEffects(field), a1, pos);
}

// pokebw2's FieldGimmickBSubway_GetTrainEffect
void *func_ov052_021f380c(Field *field) {
    u16 heapId;
    GimmickState *state;
    u32 index;
    u32 gimmickId;
    TrainWork *train;

    heapId = Field_GetHeapID(field);
    state = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    func_ov052_021f3998(Field_GetPlayerStateZoneID(field), &gimmickId, &index);
    train = ((BSubwayGimmickWork *)GimmickState_GetUserData(state, gimmickId))->data;
    return train->effect;
}

// pokebw2's TrainSetup
static void func_ov052_021f3844(BSubwayGimmickWork *work, Field *field) {
    FieldEffects *effects;
    u32 id;

    effects = Field_GetFieldEffects(field);
    work->data = GFL_HeapAllocate(work->heapId, sizeof(TrainWork), TRUE, "field_gimmick_bsubway.c", 274);
    id = FLDEFF_TRAIN;
    FieldEffects_Load(effects, &id, 1);
}

// pokebw2's TrainEnd
static void func_ov052_021f3880(BSubwayGimmickWork *work, Field *field) {
    GFL_HeapFree(work->data);
}

// pokebw2's FieldGimmickBSubway_StopShake
void func_ov052_021f388c(Field *field) {
    GimmickState *state;
    BSubwayGimmickWork *work;
    ShakeWork *shake;

    state = GameData_GetGimmickState(GSYS_GetGameData(Field_GetGameSystem(field)));
    if (Field_GetPlayerStateZoneID(field) == 75) {
        work = GimmickState_GetUserData(state, 0x27);
        if (work != NULL) {
            shake = work->data;
            if (shake != NULL) {
                shake->stop = TRUE;
            }
        }
    }
}

// pokebw2's ShakeSetup
static void func_ov052_021f38c0(BSubwayGimmickWork *work, Field *field) {
    work->data = GFL_HeapAllocate(work->heapId, sizeof(ShakeWork), TRUE, "field_gimmick_bsubway.c", 356);
}

// pokebw2's ShakeEnd
static void func_ov052_021f38e4(BSubwayGimmickWork *work, Field *field) {
    GFL_HeapFree(work->data);
}

// pokebw2's ShakeMove
static void func_ov052_021f38f0(BSubwayGimmickWork *work, Field *field) {
    VecFx32 pos = { 0, 0, 0 };
    s32 waits[] = { 60, 10, 60, 10 };
    ShakeWork *shake;
    FieldG3DMapper *mapper;

    shake = work->data;
    mapper = Field_GetG3DMapper(field);
    switch (shake->state) {
    case 0:
        if (shake->stop == TRUE) {
            break;
        }
        shake->wait = waits[shake->waitIndex];
        shake->waitIndex++;
        shake->waitIndex %= NELEMS(waits);
        shake->state++;
    case 1:
        if (--shake->wait > 0) {
            break;
        }
        shake->shake = 1;
        shake->state++;
        break;
    case 2:
        pos.y = shake->shake * FX32_ONE;
        func_ov021_0218dc4c(mapper, &pos);
        if (shake->shake == 0) {
            shake->state = 0;
        }
        if (shake->shake < 0) {
            shake->shake += 2;
            if (shake->shake > 0) {
                shake->shake = 0;
            }
        }
        shake->shake = -shake->shake;
        break;
    }
}

// pokebw2's GetZoneGimmick
static void func_ov052_021f3998(u32 zoneId, u32 *gimmickId, u32 *index) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (zoneId == data_ov052_021f39dc[i].zoneId) {
            *gimmickId = data_ov052_021f39dc[i].gimmickId;
            *index = i;
            return;
        }
    }
    *gimmickId = 0;
    *index = 0;
}
