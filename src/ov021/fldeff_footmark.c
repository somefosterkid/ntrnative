// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/fldeff_footmark.c, ported by tools/port_file.py.
// The footprints and tracks that actors leave in sand, snow and deep sand, which fade after a while. The name is the
// ROM's own, from GFL_HeapAllocate's file argument.
#include "types.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/blact.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"

enum {
    FOOTMARK_TEXTURE_COUNT = 26,
};

typedef struct {
    FieldEffects *effects;
    BlActSys *blact;
    u16 mats[FOOTMARK_TEXTURE_COUNT];
    // How far deep sand tracks sink
    fx32 sinkDepth;
} FieldEffectFootmark;

// What a mark starts from
typedef struct {
    FieldEffectFootmark *footmark;
    u16 texture;
} FootmarkTaskParams;

typedef struct {
    FootmarkTaskParams params;
    u16 actor;
    u16 state;
    u16 unk0C;
    s16 alpha;
    int timer;
} FootmarkTask;

static void func_ov021_021bbd0c(FieldEffectFootmark *footmark);
static void func_ov021_021bbd64(FieldEffectFootmark *footmark);
static u32 func_ov021_021bbd84(FieldEffectFootmark *footmark, u32 kind, u32 dir, u32 prevDir);
static void func_ov021_021bbe84(FieldEffectTask *task, void *work);
static void func_ov021_021bbef4(FieldEffectTask *task, void *work);
static void func_ov021_021bbf10(FieldEffectTask *task, void *work);
static void func_ov021_021bbf74(FieldEffectTask *task, void *work);

// The tracks' texture, from 4, by the direction the actor came from and the one it goes to
// pokebw2's FOOTMARK_TRACK_TEXTURES
static const u8 data_ov021_021d9ae8[4][4] = {
    { 4, 4, 9, 8 },
    { 4, 4, 7, 6 },
    { 6, 8, 5, 5 },
    { 7, 9, 5, 5 },
};

// pokebw2's FOOTMARK_TASK_VTABLE
static const FieldEffectTaskVTable data_ov021_021d9af8 = {
    sizeof(FootmarkTask),
    func_ov021_021bbe84,
    func_ov021_021bbef4,
    func_ov021_021bbf10,
    func_ov021_021bbf74,
};

// The textures in the field effects' archive
// pokebw2's FOOTMARK_TEXTURE_FILES
static const u16 data_ov021_021d9b0c[FOOTMARK_TEXTURE_COUNT] = {
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 39, 40, 41, 42, 43, 44, 33, 34, 35, 36, 37, 38,
};

void *func_ov021_021bbcbc(FieldEffects *effects, HeapID heapId) {
    FieldEffectFootmark *footmark =
        GFL_HeapAllocate(heapId, sizeof(FieldEffectFootmark), TRUE, "fldeff_footmark.c", 157);

    footmark->effects = effects;
    footmark->blact = Field_GetEffectBlAct(FieldEffects_GetField(effects));
    func_ov021_021bbd0c(footmark);
    footmark->sinkDepth = FX32_CONST(2);
    return footmark;
}

void func_ov021_021bbcf8(FieldEffects *effects, void *data) {
    func_ov021_021bbd64(data);
    GFL_HeapFree(data);
}

static void func_ov021_021bbd0c(FieldEffectFootmark *footmark) {
    const u16 *file = data_ov021_021d9b0c;
    ArcTool *arc = FieldEffects_GetArc(footmark->effects);
    BlActMatRequest request;
    int i;

    request.format = 2;
    request.size = 17;
    request.faceWidth = 16;
    request.faceHeight = 16;
    request.mode = BLACT_LOAD_TRIM;
    for (i = 0; i < FOOTMARK_TEXTURE_COUNT; i++) {
        request.texResource = GFL_G3DSysReadArcToolResource(arc, *file);
        FieldEffects_ApplyLuminanceTable(footmark->effects, request.texResource);
        footmark->mats[i] = BlActSys_ExecMatLoadRequests(footmark->blact, &request, 1);
        file++;
    }
}

static void func_ov021_021bbd64(FieldEffectFootmark *footmark) {
    int i;

    for (i = 0; i < FOOTMARK_TEXTURE_COUNT; i++) {
        BlActSys_FreeMaterials(footmark->blact, footmark->mats[i], 1);
    }
}

// The texture of a mark of a kind, which the caller picks by the ground and the footprint type of the actor's model
static u32 func_ov021_021bbd84(FieldEffectFootmark *footmark, u32 kind, u32 dir, u32 prevDir) {
    u32 texture = 0;
    u32 track;

    switch (kind) {
    case 0:
        texture = dir;
        break;
    case 1:
        texture = data_ov021_021d9ae8[prevDir][dir];
        break;
    case 2:
        texture = dir + 10;
        break;
    case 3:
        track = data_ov021_021d9ae8[prevDir][dir] - 4;
        texture = track + 14;
        break;
    case 4:
        track = data_ov021_021d9ae8[prevDir][dir] - 4;
        texture = track + 20;
        break;
    }
    return texture;
}

void func_ov021_021bbdd4(FieldActor *actor, FieldEffects *effects, u32 kind) {
    FootmarkTaskParams params;
    VecFx32 pos;
    u16 texture;
    FieldEffectFootmark *footmark = FieldEffects_GetHandleData(effects, 5);

    params.footmark = footmark;
    texture = func_ov021_021bbd84(footmark, kind, GetActorFaceDir(actor), func_ov010_0216d5fc(actor));
    CopyActorWPos(actor, &pos);
    ConvGXZToVector(FldAct_GetInitGPosX(actor), FldAct_GetInitGPosZ(actor), &pos);
    // Footprints to the left or right, whose texture is the direction, sit a little further back.
    // BUG: Deep sand's textures start at 20, so the second test never passes and its tracks never move back
    if (kind == 0 && (texture == 2 || texture == 3)) {
        pos.z -= FX32_CONST(2);
    } else if (kind == 4 && (texture == 2 || texture == 3)) {
        pos.z -= FX32_CONST(2);
    }
    if (kind == 4) {
        pos.y -= footmark->sinkDepth;
    }
    params.texture = texture;
    FieldEffects_TCBCreate(effects, &data_ov021_021d9af8, &pos, 0, &params, 0);
}

static void func_ov021_021bbe84(FieldEffectTask *task, void *work) {
    FootmarkTask *fmTask = work;
    FootmarkTaskParams *params = func_ov021_021ac1ac(task);
    BlActActorRequest request;

    fmTask->params = *params;
    fmTask->alpha = 31;
    request.texMat = fmTask->params.footmark->mats[fmTask->params.texture];
    request.scaleX = FX16_ONE;
    request.scaleY = FX16_ONE;
    request.alpha = fmTask->alpha;
    request.visible = TRUE;
    request.lights = 1;
    request.callback = NULL;
    request.callbackData = fmTask;
    func_ov021_021ac1b8(task, &request.pos);
    fmTask->actor = BlActSys_ExecActorRequests(fmTask->params.footmark->blact, 0, &request, 1, 3);
    if (fmTask->actor == BLACT_NONE) {
        func_ov021_021ac154(task);
    }
}

static void func_ov021_021bbef4(FieldEffectTask *task, void *work) {
    FootmarkTask *fmTask = work;
    BlActSys *blact = fmTask->params.footmark->blact;

    if (fmTask->actor != BLACT_NONE) {
        BlActSys_DeleteActors(blact, fmTask->actor, 1);
    }
}

// Stay for 16 frames, then fade out
static void func_ov021_021bbf10(FieldEffectTask *task, void *work) {
    FootmarkTask *fmTask = work;
    u8 alpha;
    BlActSys *blact;
    BlActScene *scene;
    u16 sceneActor;

    switch (fmTask->state) {
    case 0:
        fmTask->timer++;
        if (fmTask->timer >= 16) {
            fmTask->state++;
        }
        break;
    case 1:
        fmTask->alpha -= 2;
        if (fmTask->alpha < 0) {
            func_ov021_021ac154(task);
            return;
        }
        alpha = fmTask->alpha;
        blact = fmTask->params.footmark->blact;
        scene = BlActSys_GetScene(blact);
        sceneActor = func_020508c4(blact, fmTask->actor);
        func_0204f6b4(scene, sceneActor, &alpha);
        break;
    }
}

static inline void func_ov021_021bbf74(FieldEffectTask *task, void *work) {
}
