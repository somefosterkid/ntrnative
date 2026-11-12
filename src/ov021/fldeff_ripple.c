// Black's code at ov021 0x021c46f8-0x021c4978. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_effect.h"
#include "gfl/blact.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "system/game_event.h"
#include "field/field_gimmick_gate.h"
#include "field/rail_slipdown.h"
#include "struct_decls.h"
#include "field/field.h"
#include "field/field_actor.h"

// What handle 7 holds: the effects and the blact system, then the material (pokebw2's names)
typedef struct {
    FieldEffects *effects;
    BlActSys *blact;
    u16 matIndex;
} FieldEffectRipple;

// What a ripple starts from
typedef struct {
    FieldEffectRipple *ripple;
    VecFx32 pos;
} RippleTaskParams;

typedef struct {
    FieldEffectRipple *ripple;
    u16 actorIndex;
} RippleTask;

void func_ov021_021c47fc(FieldEffectRipple *ripple);
void func_ov021_021c483c(FieldEffectRipple *ripple);
GameEventReturnCode func_ov021_021c4788(GameEvent *event, u32 *state, void *data);



// The slide's event (pokebw2's event_dive.c): its work is the pointer rail_slipdown.c's slide returns
GameEvent *func_ov021_021c46f8(GameSystem *gsys, Field *field, u16 direction) {
    GameEvent *event;
    void **data;
    FieldPlayer *player;
    FieldActor *actor;

    event = GameEvent_Create(gsys, NULL, func_ov021_021c4788, sizeof(void *));
    data = GameEvent_GetData(event);
    GFL_OvlLoad(OVERLAY_ID(63));
    player = Field_GetPlayer(field);
    actor = FieldPlayer_GetActor(player);
    FieldPlayer_SetDirection(player, direction);
    *data = func_ov063_021f36c0(gsys, field, actor, TRUE);
    return event;
}

GameEvent *func_ov021_021c474c(GameSystem *gsys, Field *field, FieldActor *actor) {
    GameEvent *event;
    void **data;

    event = GameEvent_Create(gsys, NULL, func_ov021_021c4788, sizeof(void *));
    data = GameEvent_GetData(event);
    GFL_OvlLoad(OVERLAY_ID(63));
    *data = func_ov063_021f36c0(gsys, field, actor, FALSE);
    return event;
}


GameEventReturnCode func_ov021_021c4788(GameEvent *event, u32 *state, void *data) {
    void **work;

    work = GameEvent_GetData(event);
    if (func_ov063_021f376c(*work)) {
        func_ov063_021f3758(*work);
        GFL_OvlUnload(OVERLAY_ID(63));
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

void *func_ov021_021c47b4(FieldEffects *param_1, HeapID param_2) {
    FieldEffectRipple *ripple;
    Field *field;

    ripple = GFL_HeapAllocate(param_2, sizeof(FieldEffectRipple), 1, "fldeff_ripple.c", 0x56);
    ripple->effects = param_1;
    field = FieldEffects_GetField(param_1);
    ripple->blact = Field_GetWildEffectBlAct(field);
    func_ov021_021c47fc(ripple);
    return ripple;
}

void func_ov021_021c47e8(FieldEffects *param_1, void *param_2) {
    func_ov021_021c483c(param_2);
    GFL_HeapFree(param_2);
}

void func_ov021_021c47fc(FieldEffectRipple *ripple) {
    ArcTool *handle;
    BlActMatRequest BStack_18;

    handle = FieldEffects_GetArc(ripple->effects);
    BStack_18.format = 2;
    BStack_18.size = 0x13;
    BStack_18.faceWidth = 0x10;
    BStack_18.faceHeight = 0x10;
    BStack_18.mode = 1;
    BStack_18.texResource = GFL_G3DSysReadArcToolResource(handle, 0x2e);
    FieldEffects_ApplyLuminanceTable(ripple->effects, BStack_18.texResource);
    ripple->matIndex = BlActSys_ExecMatLoadRequests(ripple->blact, &BStack_18, 1);
}

void func_ov021_021c483c(FieldEffectRipple *ripple) {
    BlActSys_FreeMaterials(ripple->blact, ripple->matIndex, 1);
}

void func_ov021_021c48a0(FieldEffectTask *task, void *work);
void func_ov021_021c4938(FieldEffectTask *param_1, void *param_2);
void func_ov021_021c4954(FieldEffectTask *param_1, void *param_2);
void func_ov021_021c4974(FieldEffectTask *param_1, void *param_2);

const FieldEffectTaskVTable data_ov021_021da9f0 = {
    sizeof(RippleTask),
    func_ov021_021c48a0,
    func_ov021_021c4938,
    func_ov021_021c4954,
    func_ov021_021c4974,
};

static const BlActAnimStep data_ov021_021daa04[] = {
    { 0, 0, 0, 3 },
    { 1, 0, 0, 3 },
    { 2, 0, 0, 3 },
    { 3, 0, 0, 3 },
    { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep *data_ov021_021de464[] = {
    data_ov021_021daa04,
};

void func_ov021_021c484c(FieldEffects *effects, s16 x, s16 z, fx32 y) {
    VecFx32 pos;
    RippleTaskParams params;

    ConvGXZToVector(x, z, &pos);
    pos.y = y - 0x8000;
    pos.z = pos.z - 0x5000;
    params.ripple = FieldEffects_GetHandleData(effects, 7);
    params.pos = pos;
    FieldEffects_TCBCreate(effects, &data_ov021_021da9f0, NULL, 0, &params, 0);
}

void func_ov021_021c48a0(FieldEffectTask *task, void *work) {
    u16 actor;
    RippleTaskParams *params;
    RippleTask *data;
    BlActSys *sys;
    BlActActorRequest request;

    params = func_ov021_021ac1ac(task);
    data = work;
    data->ripple = params->ripple;
    func_ov021_021ac1cc(task, &params->pos);
    sys = data->ripple->blact;
    request.texMat = data->ripple->matIndex;
    request.scaleX = 0x1000;
    request.scaleY = 0x1000;
    request.alpha = 0x1f;
    request.visible = 1;
    request.lights = 1;
    request.pos = params->pos;
    request.callback = NULL;
    request.callbackData = data;
    data->actorIndex = BlActSys_ExecActorRequests(sys, 0, &request, 1, 3);
    if (data->actorIndex == 0xffff) {
        func_ov021_021ac154(task);
        return;
    }
    func_02050804(sys, data->actorIndex, data_ov021_021de464, 1);
    func_0205082c(sys, actor = data->actorIndex, 0);
    func_020507f0(sys, actor, 1);
}



void func_ov021_021c4938(FieldEffectTask *param_1, void *param_2) {
    RippleTask *work = param_2;
    BlActSys *sys;
    u16 first;

    sys = work->ripple->blact;
    first = work->actorIndex;
    if (first != 0xffff) {
        BlActSys_DeleteActors(sys, (u32)first, 1);
    }
}


void func_ov021_021c4954(FieldEffectTask *param_1, void *param_2) {
    RippleTask *work = param_2;
    BOOL BVar1;
    u16 auStack_10 [2];

    BVar1 = func_020508ec(work->ripple->blact, (u32)work->actorIndex, auStack_10);
    if (BVar1 != 0) {
        func_ov021_021ac154(param_1);
    }
}

void func_ov021_021c4974(FieldEffectTask *param_1, void *param_2) {
}
