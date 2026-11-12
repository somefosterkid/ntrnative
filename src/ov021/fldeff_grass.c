// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fldeff_grass.c, ported by tools/port_file.py.
// The grass that shakes where a field actor steps: a billboard per step that plays the shaking and stays until the
// actor leaves the tile. The name is the ROM's own, from GFL_HeapAllocate's file argument. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/blact.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

#define GRASS_KIND_COUNT 8

typedef struct {
    FieldEffects *effects;
    BlActSys *blact;
    u16 matIndices[GRASS_KIND_COUNT];
} FieldEffectGrass;

// What a step's grass starts from
typedef struct {
    FieldEffectGrass *grass;
    FieldActor *actor;
    int gx;
    int gz;
    u16 uid;
    u16 zoneId;
    VecFx32 pos;
    int kind;
} GrassTaskParams;

typedef struct {
    int state;
    GrassTaskParams params;
    FieldActorIdentity identity;
    u16 unk2E;
    u16 actorIndex;
} GrassTask;

static void func_ov021_021ac6d4(FieldEffectGrass *grass);
static void func_ov021_021ac740(FieldEffectGrass *grass);
static void func_ov021_021ac810(FieldEffectTask *task, void *work);
static void func_ov021_021ac8e4(FieldEffectTask *task, void *work);
static void func_ov021_021ac900(FieldEffectTask *task, void *work);
static void func_ov021_021ac9d4(FieldEffectTask *task, void *work);
static BOOL func_ov021_021ac9d8(GrassTask *grassTask);

// Whether each kind of grass ends as soon as the actor walks on, and the step of its animation it settles on
// pokebw2's GRASS_ENDS_ON_LEAVE
static const u8 data_ov021_021d90c0[GRASS_KIND_COUNT] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE };

// pokebw2's GRASS_REST_STEPS
static const u16 data_ov021_021d90c8[GRASS_KIND_COUNT] = { 3, 3, 5, 5, 3, 3, 3, 3 };

// pokebw2's GRASS_TASK_VTABLE
static const FieldEffectTaskVTable data_ov021_021d90ec = {
    sizeof(GrassTask),
    func_ov021_021ac810,
    func_ov021_021ac8e4,
    func_ov021_021ac900,
    func_ov021_021ac9d4,
};

// pokebw2's GRASS_ANIM_SHAKE
static const BlActAnimStep data_ov021_021d90d8[] = {
    { 0, 0, 0, 3 }, { 1, 0, 0, 3 }, { 2, 0, 0, 3 }, { 3, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

// pokebw2's GRASS_ANIM_SHAKE_LONG
static const BlActAnimStep data_ov021_021d9100[] = {
    { 0, 0, 0, 2 }, { 1, 0, 0, 2 }, { 0, 0, 0, 2 }, { 1, 0, 0, 2 },
    { 0, 0, 0, 2 }, { 1, 0, 0, 2 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

// Each kind's texture in each season
// pokebw2's GRASS_TEXTURES
static const u16 data_ov021_021d911c[GRASS_KIND_COUNT][4] = {
#include "asset/fldeff_grass_data_ov021_021d911c.inc"
};

// pokebw2's GRASS_ANIMS
static const BlActAnimStep *data_ov021_021de04c[] = { data_ov021_021d90d8 };

// pokebw2's GRASS_ANIMS_LONG
static const BlActAnimStep *data_ov021_021de048[] = { data_ov021_021d9100 };

// pokebw2's GRASS_KIND_ANIMS
static const BlActAnimStep **data_ov021_021de050[GRASS_KIND_COUNT] = {
    data_ov021_021de04c, data_ov021_021de04c, data_ov021_021de048, data_ov021_021de048, data_ov021_021de04c, data_ov021_021de04c, data_ov021_021de04c, data_ov021_021de04c,
};

void *loadGrassEffects(FieldEffects *effects, HeapID heapId) {
    FieldEffectGrass *grass = GFL_HeapAllocate(heapId, sizeof(FieldEffectGrass), TRUE, "fldeff_grass.c", 108);

    grass->effects = effects;
    grass->blact = Field_GetWildEffectBlAct(FieldEffects_GetField(effects));
    func_ov021_021ac6d4(grass);
    return grass;
}

void freeGrassEffects(FieldEffects *effects, void *data) {
    func_ov021_021ac740(data);
    GFL_HeapFree(data);
}

static void func_ov021_021ac6d4(FieldEffectGrass *grass) {
    int i;
    BlActMatRequest request;
    ArcTool *arc = FieldEffects_GetArc(grass->effects);
    Field_GetGameSystem(FieldEffects_GetField(grass->effects));
    u32 season = FieldEffects_GetSeason(grass->effects);

    request.format = 0;
    request.size = 36;
    request.faceWidth = 32;
    request.faceHeight = 32;
    request.mode = BLACT_LOAD_TRIM;
    for (i = 0; i < GRASS_KIND_COUNT; i++) {
        request.texResource = GFL_G3DSysReadArcToolResource(arc, data_ov021_021d911c[i][season]);
        FieldEffects_ApplyLuminanceTable(grass->effects, request.texResource);
        grass->matIndices[i] = BlActSys_ExecMatLoadRequests(grass->blact, &request, 1);
    }
}

static void func_ov021_021ac740(FieldEffectGrass *grass) {
    int i;

    for (i = 0; i < GRASS_KIND_COUNT; i++) {
        BlActSys_FreeMaterials(grass->blact, grass->matIndices[i], 1);
    }
}

// Shake the grass of the given kind on the actor's tile; it settles at once unless animate
void func_ov021_021ac760(FieldEffects *effects, FieldActor *actor, BOOL animate, int kind) {
    VecFx32 pos;
    GrassTaskParams params;
    fx32 height;

    if (kind < GRASS_KIND_COUNT) {
        params.grass = FieldEffects_GetHandleData(effects, 2);
        params.actor = actor;
        params.kind = kind;
        params.uid = GetActorUID(actor);
        params.zoneId = GetActorZoneID(actor);
        params.gx = GetGPosX(actor);
        params.gz = GetGPosZ(actor);
        ConvGXZToVector(params.gx, params.gz, &pos);
        pos.y = GetActorPosY(actor);
        if (CheckActorFlag(actor, 0x2000)) {
            pos.y += FX32_CONST(7);
            pos.z -= FX32_CONST(2);
        } else {
            GetHeightFromMap(actor, &pos, &height);
            pos.y = height + FX32_CONST(9);
            pos.z -= FX32_CONST(2);
        }
        params.pos = pos;
        FieldEffects_TCBCreate(effects, &data_ov021_021d90ec, NULL, animate, &params, 0);
    }
}

static void func_ov021_021ac810(FieldEffectTask *task, void *work) {
    GrassTask *grassTask = work;
    GrassTaskParams *params = func_ov021_021ac1ac(task);
    BlActActorRequest request;
    u16 kind;
    BlActSys *blact;
    u16 actorIndex;

    grassTask->params = *params;
    func_ov010_0216e1a4(params->actor, &grassTask->identity);
    func_ov021_021ac1cc(task, &params->pos);
    kind = grassTask->params.kind;
    request.texMat = grassTask->params.grass->matIndices[kind];
    request.scaleX = FX32_CONST(2);
    request.scaleY = FX32_CONST(2);
    request.alpha = 31;
    request.visible = TRUE;
    request.lights = 1;
    request.pos = params->pos;
    request.callback = NULL;
    request.callbackData = grassTask;
    blact = grassTask->params.grass->blact;
    grassTask->actorIndex = BlActSys_ExecActorRequests(blact, 0, &request, 1, 0);
    if (grassTask->actorIndex == 0xffff) {
        func_ov021_021ac154(task);
        return;
    }
    func_02050804(blact, grassTask->actorIndex, data_ov021_021de050[kind], 1);
    actorIndex = grassTask->actorIndex;
    func_0205082c(blact, actorIndex, 0);
    func_020507f0(blact, actorIndex, TRUE);
    if (func_ov021_021ac1a0(task) == FALSE) {
        grassTask->state = 1;
    }
    func_ov021_021ac178(task);
}

static void func_ov021_021ac8e4(FieldEffectTask *task, void *work) {
    GrassTask *grassTask = work;
    BlActSys *blact = grassTask->params.grass->blact;

    if (grassTask->actorIndex != 0xffff) {
        BlActSys_DeleteActors(blact, grassTask->actorIndex, 1);
    }
}

static void func_ov021_021ac900(FieldEffectTask *task, void *work) {
    u16 cmd;
    GrassTask *grassTask = work;
    BlActSys *blact = grassTask->params.grass->blact;

    switch (grassTask->state) {
    case 0:
        // Shaking
        if (data_ov021_021d90c0[grassTask->params.kind] == TRUE) {
            FieldActor *actor = grassTask->params.actor;

            if (func_ov010_0216e1c4(actor, &grassTask->identity) == TRUE && !func_ov021_021ac9d8(grassTask) &&
                GetActorMotionDir(actor) == 1) {
                func_ov021_021ac154(task);
                return;
            }
        }
        if (func_020508ec(blact, grassTask->actorIndex, &cmd)) {
            grassTask->state++;
        }
        break;
    case 1:
        // Settle
        if (!func_ov010_0216e1c4(grassTask->params.actor, &grassTask->identity)) {
            func_ov021_021ac154(task);
            return;
        }
        func_0205085c(blact, grassTask->actorIndex, data_ov021_021d90c8[grassTask->params.kind]);
        grassTask->state++;
        break;
    case 2:
        grassTask->state++;
    case 3:
        // Stay until the actor leaves the tile
        if (!func_ov010_0216e1c4(grassTask->params.actor, &grassTask->identity)) {
            func_ov021_021ac154(task);
            return;
        }
        if (!func_ov021_021ac9d8(grassTask)) {
            func_ov021_021ac154(task);
        }
        break;
    }
}

static void func_ov021_021ac9d4(FieldEffectTask *task, void *work) {
}

// Whether the actor is still on the grass's tile, or standing still on its first tile
static BOOL func_ov021_021ac9d8(GrassTask *grassTask) {
    FieldActor *actor = grassTask->params.actor;
    s16 gx = GetGPosX(actor);
    s16 gz = GetGPosZ(actor);
    u8 endsOnLeave = data_ov021_021d90c0[grassTask->params.kind];

    if (grassTask->params.gx != gx || grassTask->params.gz != gz) {
        if (endsOnLeave == FALSE && GetActorMotionDir(actor) == 0) {
            gx = FldAct_GetInitGPosX(actor);
            gz = FldAct_GetInitGPosZ(actor);
            if (grassTask->params.gx == gx && grassTask->params.gz == gz) {
                return TRUE;
            }
        }
        return FALSE;
    }
    return TRUE;
}
