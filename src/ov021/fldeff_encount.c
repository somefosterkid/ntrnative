// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fldeff_encount.c, ported by tools/port_file.py.
// The overworld patches of effect encounters: a grass patch, a dust cloud, a water ripple or the shadow of a flying Pokémon
// that mark a rare encounter, as 3D models with their sound. The name is the ROM's own, from GFL_HeapAllocate's file
// argument. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/encounter.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_sound.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "system/game_data.h"

// The models and animations of one kind of phenomenon. A kind can have variants, such as one per season, whose files
// follow the first at variantStep apart
typedef struct {
    u8 anmCount;
    u8 variantCount : 3;
    u8 variantStep : 3;
    u8 bySeason : 1;
    u8 byExterior : 1;
    u16 se;
    u16 mdlId;
    u16 anmIds[4];
} EncountEffectSpec;

typedef struct {
    G3DActor *actor;
    void *anms[4];
    G3DModel *mdl;
} EncountEffectObj;

typedef struct {
    FieldEffects *effects;
    u32 unk04;
    void *mdlRes[4];
    void *anmRes[4];
    EncountEffectObj objs[4];
    EncountEffectSpec spec;
} FieldEffectEncount;

// What a phenomenon starts from
typedef struct {
    EncountSystem *system;
    FieldEffectEncount *effect;
    MMSys *actorSys;
    u32 unk0C;
    u16 x;
    u16 z;
    u32 effectIndex;
    u8 variant;
    VecFx32 pos;
} EncountTaskParams;

typedef struct {
    u16 unk00;
    // Set while the phenomenon is still, and while it is hidden
    u8 paused;
    u8 hidden;
    EncountTaskParams params;
    u8 unk2C[8];
    EncountSystem *system;
} EncountTask;

static void *func_ov021_021ad7b4(FieldEffects *effects, HeapID heapId, u32 kind);
static void func_ov021_021ad7e4(FieldEffectEncount *effect, u32 kind);
static void func_ov021_021ad82c(EncountEffectObj *obj, FieldEffectEncount *effect, u8 variant);
static void func_ov021_021ad8a4(EncountEffectObj *obj, FieldEffectEncount *effect);
static void func_ov021_021ad8dc(FieldEffectEncount *effect, EncountEffectSpec *spec, ArcTool *arc);
static void func_ov021_021ad96c(FieldEffectEncount *effect, EncountEffectSpec *spec, ArcTool *arc);
static void func_ov021_021ada1c(FieldEffectEncount *effect);
static void func_ov021_021adb6c(FieldEffectTask *task, void *work);
static void func_ov021_021adbac(FieldEffectTask *task, void *work);
static void func_ov021_021adbb0(FieldEffectTask *task, void *work);
static void func_ov021_021adc04(EncountTask *encountTask, FieldEffectEncount *effect);
static void func_ov021_021adc6c(FieldEffectTask *task, void *work);

// The volume of the phenomenon's sound by the player's distance, in steps of 5 tiles
// pokebw2's ENCOUNT_EFFECT_VOLUMES
static const u8 data_ov021_021d9234[4] = { 127, 90, 60, 40 };

// The effect of each kind of phenomenon; the first three are variants of the first effect
// pokebw2's ENCOUNT_EFFECT_INDICES
static const u8 data_ov021_021d9238[8] = { 0, 0, 0, 1, 2, 3, 4, 5 };

// pokebw2's ENCOUNT_TASK_VTABLE
static const FieldEffectTaskVTable data_ov021_021d9240 = {
    sizeof(EncountTask),
    func_ov021_021adb6c,
    func_ov021_021adbac,
    func_ov021_021adbb0,
    func_ov021_021adc6c,
};

// pokebw2's ENCOUNT_EFFECT_SPECS
static EncountEffectSpec data_ov021_021de094[6] = {
    { 1, 3, 4, TRUE, FALSE, 1749, 60, { 134, 0, 0, 0 } },
    { 1, 1, 4, TRUE, FALSE, 1749, 68, { 142, 0, 0, 0 } },
    { 2, 1, 1, FALSE, FALSE, 1752, 76, { 147, 119, 0, 0 } },
    { 1, 1, 2, FALSE, TRUE, 1750, 72, { 115, 0, 0, 0 } },
    { 1, 1, 1, FALSE, FALSE, 1750, 74, { 117, 0, 0, 0 } },
    { 3, 1, 1, FALSE, FALSE, 1751, 75, { 146, 132, 118, 0 } },
};

void *func_ov021_021ad758(FieldEffects *effects, HeapID heapId) {
    return func_ov021_021ad7b4(effects, heapId, 0);
}

void *func_ov021_021ad764(FieldEffects *effects, HeapID heapId) {
    return func_ov021_021ad7b4(effects, heapId, 1);
}

void *func_ov021_021ad770(FieldEffects *effects, HeapID heapId) {
    return func_ov021_021ad7b4(effects, heapId, 2);
}

void *func_ov021_021ad77c(FieldEffects *effects, HeapID heapId) {
    return func_ov021_021ad7b4(effects, heapId, 3);
}

void *func_ov021_021ad788(FieldEffects *effects, HeapID heapId) {
    return func_ov021_021ad7b4(effects, heapId, 4);
}

void *func_ov021_021ad794(FieldEffects *effects, HeapID heapId) {
    return func_ov021_021ad7b4(effects, heapId, 5);
}

void func_ov021_021ad7a0(FieldEffects *effects, void *data) {
    func_ov021_021ada1c(data);
    GFL_HeapFree(data);
}

static void *func_ov021_021ad7b4(FieldEffects *effects, HeapID heapId, u32 kind) {
    FieldEffectEncount *effect = GFL_HeapAllocate(heapId, sizeof(FieldEffectEncount), TRUE, "fldeff_encount.c", 286);

    effect->effects = effects;
    func_ov021_021ad7e4(effect, kind);
    return effect;
}

static void func_ov021_021ad7e4(FieldEffectEncount *effect, u32 kind) {
    ArcTool *arc;

    effect->spec = data_ov021_021de094[kind];
    arc = FieldEffects_GetArc(effect->effects);
    if (kind == 0) {
        func_ov021_021ad8dc(effect, &effect->spec, arc);
    } else {
        func_ov021_021ad96c(effect, &effect->spec, arc);
    }
}

static void func_ov021_021ad82c(EncountEffectObj *obj, FieldEffectEncount *effect, u8 variant) {
    int i;

    obj->mdl = GFL_G3DMdlCreate(effect->mdlRes[variant], 0, effect->mdlRes[variant]);
    for (i = 0; i < effect->spec.anmCount; i++) {
        obj->anms[i] = GFL_G3DAnmCreate(obj->mdl, effect->anmRes[variant * effect->spec.anmCount + i], 0);
    }
    obj->actor = GFL_G3DActorCreate(obj->mdl, obj->anms, effect->spec.anmCount);
    for (i = 0; i < effect->spec.anmCount; i++) {
        GFL_G3DActorBindAnm(obj->actor, i);
    }
}

static void func_ov021_021ad8a4(EncountEffectObj *obj, FieldEffectEncount *effect) {
    int i;

    for (i = 0; i < effect->spec.anmCount; i++) {
        GFL_G3DAnmFree(obj->anms[i]);
    }
    GFL_G3DActorFree(obj->actor);
    GFL_G3DMdlFree(obj->mdl);
}

// The grass of the first kind: this season's, the next variant's, and the first
static void func_ov021_021ad8dc(FieldEffectEncount *effect, EncountEffectSpec *spec, ArcTool *arc) {
    u32 season = FieldEffects_GetSeason(effect->effects);

    effect->mdlRes[0] = GFL_G3DSysReadArcToolResource(arc, spec->mdlId + season);
    GFL_G3DResUploadTexData(effect->mdlRes[0]);
    effect->anmRes[0] = GFL_G3DSysReadArcToolResource(arc, spec->anmIds[0] + season);
    func_ov021_021ad82c(&effect->objs[0], effect, 0);
    season += spec->variantStep;
    effect->mdlRes[1] = GFL_G3DSysReadArcToolResource(arc, spec->mdlId + season);
    GFL_G3DResUploadTexData(effect->mdlRes[1]);
    effect->anmRes[1] = GFL_G3DSysReadArcToolResource(arc, spec->anmIds[0] + season);
    func_ov021_021ad82c(&effect->objs[1], effect, 1);
    effect->mdlRes[2] = GFL_G3DSysReadArcToolResource(arc, spec->mdlId);
    GFL_G3DResUploadTexData(effect->mdlRes[2]);
    effect->anmRes[2] = GFL_G3DSysReadArcToolResource(arc, spec->anmIds[0]);
    func_ov021_021ad82c(&effect->objs[2], effect, 2);
}

static void func_ov021_021ad96c(FieldEffectEncount *effect, EncountEffectSpec *spec, ArcTool *arc) {
    int i;
    int j;
    u32 offset = 0;
    u32 fileOffset;

    if (spec->bySeason) {
        offset = FieldEffects_GetSeason(effect->effects);
    } else if (spec->byExterior) {
        offset = FieldEffects_GetAreaIsExterior(effect->effects);
    }
    for (i = 0; i < spec->variantCount; i++) {
        fileOffset = offset + i * spec->variantStep;
        effect->mdlRes[i] = GFL_G3DSysReadArcToolResource(arc, spec->mdlId + fileOffset);
        GFL_G3DResUploadTexData(effect->mdlRes[i]);
        for (j = 0; j < spec->anmCount; j++) {
            effect->anmRes[i * spec->anmCount + j] = GFL_G3DSysReadArcToolResource(arc, fileOffset + spec->anmIds[j]);
        }
        func_ov021_021ad82c(&effect->objs[i], effect, i);
    }
}

static void func_ov021_021ada1c(FieldEffectEncount *effect) {
    int i;

    for (i = 0; i < effect->spec.variantCount; i++) {
        func_ov021_021ad8a4(&effect->objs[i], effect);
    }
    for (i = 0; i < effect->spec.anmCount * effect->spec.variantCount; i++) {
        GFL_G3DResFree(effect->anmRes[i]);
    }
    for (i = 0; i < effect->spec.variantCount; i++) {
        GFL_G3DResFreeTexData(effect->mdlRes[i]);
        GFL_G3DResFree(effect->mdlRes[i]);
    }
}

// Start the phenomenon of the given kind on the tile, unless the controls are of another type
FieldEffectTask *func_ov021_021adaac(EncountSystem *system, FieldEffects *effects, u16 x, u16 z, fx32 height,
                                     u32 kind) {
    Field *field = FieldEffects_GetField(effects);
    u32 effectIndex;
    FieldEffectEncount *effect;
    EncountTaskParams params;

    if (Field_GetResolvedControllerTypeID(field) != 0) {
        return NULL;
    }
    effectIndex = data_ov021_021d9238[kind];
    effect = FieldEffects_GetHandleData(effects, effectIndex + 18);
    if (effect == NULL) {
        return NULL;
    }
    MI_CpuFill8(&params, 0, sizeof(EncountTaskParams));
    params.system = system;
    params.effect = effect;
    params.actorSys = Field_GetActorSystem(field);
    params.x = x;
    params.z = z;
    params.effectIndex = effectIndex;
    if (effectIndex == 0) {
        params.variant = kind;
    } else {
        params.variant = 0;
    }
    params.pos.x = (x << 16) + FX32_CONST(8);
    params.pos.z = (z << 16) + FX32_CONST(8);
    params.pos.y = height;
    return FieldEffects_TCBCreate(effects, &data_ov021_021d9240, NULL, TRUE, &params, 0);
}

void func_ov021_021adb4c(FieldEffectTask *task, BOOL paused) {
    if (task != NULL) {
        EncountTask *encountTask = func_ov021_021ac1e0(task);

        encountTask->paused = paused;
    }
}

void func_ov021_021adb5c(FieldEffectTask *task, BOOL hidden) {
    if (task != NULL) {
        EncountTask *encountTask = func_ov021_021ac1e0(task);

        encountTask->hidden = hidden;
    }
}

static void func_ov021_021adb6c(FieldEffectTask *task, void *work) {
    EncountTask *encountTask = work;
    EncountTaskParams *params = func_ov021_021ac1ac(task);

    encountTask->system = params->system;
    encountTask->params = *params;
    func_ov021_021ac1cc(task, &params->pos);
    if (func_ov021_021ac1a0(task) == FALSE) {
        encountTask->unk00 = 1;
    }
    func_ov021_021ac178(task);
}

static void func_ov021_021adbac(FieldEffectTask *task, void *work) {
}

static void func_ov021_021adbb0(FieldEffectTask *task, void *work) {
    int i;
    EncountTask *encountTask = work;
    FieldEffectEncount *effect = encountTask->params.effect;

    if (Field_CheckMapLoadFinished(FieldEffects_GetField(effect->effects)) && !encountTask->paused) {
        func_ov021_021adc04(encountTask, effect);
        for (i = 0; i < effect->spec.anmCount; i++) {
            GFL_G3DActorStepAnmFrameLoop(effect->objs[encountTask->params.variant].actor, i, FX32_ONE);
        }
    }
}

// Play the sound at the start of each loop, quieter the further away the player is
static void func_ov021_021adc04(EncountTask *encountTask, FieldEffectEncount *effect) {
    u16 distance = 0;
    fx32 frame;

    GFL_G3DActorGetAnmFrame(effect->objs[encountTask->params.variant].actor, 0, &frame);
    if (frame == 0 && func_ov021_021aaa4c(encountTask->system, &distance)) {
        u8 volume;
        u16 se;
        FieldSound *sound;

        distance /= 5;
        if (distance > 3) {
            distance = 3;
        }
        volume = data_ov021_021d9234[distance];
        se = effect->spec.se;
        sound = GameData_GetFieldSoundSystem(encountTask->system->gameData);
        FieldSnd_PlayAmbienceEx(sound, se, volume);
    }
}

static void func_ov021_021adc6c(FieldEffectTask *task, void *work) {
    SRTMatrix srt = { { 0, 0, 0 }, { FX32_ONE, FX32_ONE, FX32_ONE } };
    EncountTask *encountTask = work;
    FieldEffectEncount *effect = encountTask->params.effect;

    if (!encountTask->hidden) {
        MTX_Identity33_(&srt.rotation);
        func_ov021_021ac1b8(task, &srt.translation);
        GFL_G3DSysDrawObjBBoxCull(effect->objs[encountTask->params.variant].actor, &srt);
    }
}
