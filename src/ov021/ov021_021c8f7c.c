// Black's code at ov021 0x021c8f7c-0x021c939c. No name is known: the module and address.

#include "types.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "gfl/g3d.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "field/fldeff_d06denki.h"

// What func_ov021_021c8f7c's and a mark's tasks start from
typedef struct {
    FieldEffectD06Denki *effect;
    VecFx32 pos;
} D06DenkiTaskParams;

// func_ov021_021c8f7c's task: the effect's animation, played once
typedef struct {
    FieldEffectD06Denki *effect;
    fx32 frame;
    BOOL done;
} D06DenkiTask;

// What a stone's task starts from
typedef struct {
    FieldEffectD06Denki *effect;
    VecFx32 pos;
    FieldActor *actor;
} D06StoneTaskParams;

// A stone's task: its model follows the actor, with a second model below it
typedef struct {
    D06StoneTaskParams params;
    // Set to stop the first animation's loop
    u8 stopped;
    // The second animation: playing, and played to its end
    u8 playing;
    u8 finished;
    G3DActor *actor;
    void *anms[2];
    G3DModel *mdl;
    VecFx32 unk28;
    G3DActor *unk34;
    G3DModel *unk38;
} D06StoneTask;

// A mark's task
typedef struct {
    D06DenkiTaskParams params;
    // The second animation is playing
    u8 playing;
    G3DActor *actor;
    void *anms[2];
    G3DModel *mdl;
} D06MarkTask;

void func_ov021_021c8fb8(FieldEffectTask *task, void *work);
void func_ov021_021c8fdc(FieldEffectTask *task, void *work);
void func_ov021_021c8fe0(FieldEffectTask *task, void *work);
void func_ov021_021c8ff0(FieldEffectTask *task, void *work);
void func_ov021_021c90e4(FieldEffectTask *task, void *work);
void func_ov021_021c9164(FieldEffectTask *task, void *work);
void func_ov021_021c9190(FieldEffectTask *task, void *work);
void func_ov021_021c9208(FieldEffectTask *task, void *work);
void func_ov021_021c92b0(FieldEffectTask *task, void *work);
void func_ov021_021c9314(FieldEffectTask *task, void *work);
void func_ov021_021c9334(FieldEffectTask *task, void *work);
void func_ov021_021c9360(FieldEffectTask *task, void *work);

const FieldEffectTaskVTable data_ov021_021dab90 = {
    sizeof(D06MarkTask),
    func_ov021_021c92b0,
    func_ov021_021c9314,
    func_ov021_021c9334,
    func_ov021_021c9360,
};

const FieldEffectTaskVTable data_ov021_021daba4 = {
    sizeof(D06DenkiTask),
    func_ov021_021c8fb8,
    func_ov021_021c8fdc,
    func_ov021_021c8fe0,
    func_ov021_021c8ff0,
};

const FieldEffectTaskVTable data_ov021_021dabb8 = {
    sizeof(D06StoneTask),
    func_ov021_021c90e4,
    func_ov021_021c9164,
    func_ov021_021c9190,
    func_ov021_021c9208,
};

const SRTMatrix data_ov021_021dabcc = {
    { 0, 0, 0 },
    { 0x1000, 0x1000, 0x1000 },
};

const SRTMatrix data_ov021_021dac08 = {
    { 0, 0, 0 },
    { 0x1000, 0x1000, 0x1000 },
};

const SRTMatrix data_ov021_021dac44 = {
    { 0, 0, 0 },
    { 0x1000, 0x1000, 0x1000 },
};

void func_ov021_021c8f7c(FieldActor *actor, FieldEffects *effects) {
    VecFx32 pos;
    D06DenkiTaskParams params;

    CopyActorWPos(actor, &pos);
    params.effect = FieldEffects_GetHandleData(effects, 0xb);
    params.pos = pos;
    FieldEffects_TCBCreate(effects, &data_ov021_021daba4, NULL, 0, &params, 0);
}

void func_ov021_021c8fb8(FieldEffectTask *task, void *work) {
    D06DenkiTask *wk = work;
    D06DenkiTaskParams *params = func_ov021_021ac1ac(task);

    wk->effect = params->effect;
    func_ov021_021ac1cc(task, &params->pos);
    GFL_SndSEPlay(0x6a2);
}

void func_ov021_021c8fdc(FieldEffectTask *task, void *work) {
}

void func_ov021_021c8fe0(FieldEffectTask *task, void *work) {
    D06DenkiTask *wk = work;

    if (wk->done == TRUE) {
        func_ov021_021ac154(task);
    }
}

void func_ov021_021c8ff0(FieldEffectTask *task, void *work) {
    SRTMatrix srt;
    D06DenkiTask *wk = work;
    G3DActor *actor;

    if (wk->done == FALSE) {
        srt = data_ov021_021dabcc;
        actor = wk->effect->actor;
        GFL_G3DActorSetAnmFrame(actor, 0, &wk->frame);
        if (GFL_G3DActorStepAnmFrame(actor, 0, FX32_ONE) == FALSE) {
            wk->done = TRUE;
        }
        if (GFL_G3DActorGetAnmFrame(actor, 0, &wk->frame) == FALSE) {
            wk->done = TRUE;
        }
        MTX_Identity33_(&srt.rotation);
        func_ov021_021ac1b8(task, &srt.translation);
        GFL_G3DSysDrawObjBBoxCull(actor, &srt);
    }
}

FieldEffectTask *func_ov021_021c9064(FieldEffects *effects, const VecFx32 *pos, FieldActor *actor) {
    D06StoneTaskParams params;

    params.effect = FieldEffects_GetHandleData(effects, 0xb);
    params.pos = *pos;
    params.actor = actor;
    return FieldEffects_TCBCreate(effects, &data_ov021_021dabb8, NULL, 0, &params, 0);
}

void func_ov021_021c909c(FieldEffectTask *task) {
    D06StoneTask *wk;

    if (task != NULL) {
        wk = func_ov021_021ac1e0(task);
        wk->stopped = TRUE;
    }
}

void func_ov021_021c90ac(FieldEffectTask *task) {
    D06StoneTask *wk;
    fx32 frame;

    if (task != NULL) {
        frame = 0;
        wk = func_ov021_021ac1e0(task);
        wk->playing = TRUE;
        wk->finished = FALSE;
        GFL_G3DActorSetAnmFrame(wk->actor, 1, &frame);
    }
}

u8 func_ov021_021c90d0(FieldEffectTask *task) {
    D06StoneTask *wk;

    if (task != NULL) {
        wk = func_ov021_021ac1e0(task);
        return wk->finished;
    }
    return TRUE;
}

void func_ov021_021c90e4(FieldEffectTask *task, void *work) {
    D06StoneTask *wk = work;
    D06StoneTaskParams *params = func_ov021_021ac1ac(task);

    wk->params = *params;
    func_ov021_021ac1cc(task, &wk->params.pos);
    wk->mdl = GFL_G3DMdlCreate(wk->params.effect->mdlRes[1], 0, wk->params.effect->mdlRes[1]);
    wk->anms[0] = GFL_G3DAnmCreate(wk->mdl, wk->params.effect->anmRes[1], 0);
    wk->anms[1] = GFL_G3DAnmCreate(wk->mdl, wk->params.effect->anmRes[2], 0);
    wk->actor = GFL_G3DActorCreate(wk->mdl, wk->anms, 2);
    GFL_G3DActorBindAnm(wk->actor, 0);
    GFL_G3DActorBindAnm(wk->actor, 1);
    wk->unk38 = GFL_G3DMdlCreate(wk->params.effect->mdlRes[3], 0, wk->params.effect->mdlRes[3]);
    wk->unk34 = GFL_G3DActorCreate(wk->unk38, NULL, 0);
}

void func_ov021_021c9164(FieldEffectTask *task, void *work) {
    D06StoneTask *wk = work;

    GFL_G3DAnmFree(wk->anms[0]);
    GFL_G3DAnmFree(wk->anms[1]);
    GFL_G3DActorFree(wk->actor);
    GFL_G3DMdlFree(wk->mdl);
    GFL_G3DActorFree(wk->unk34);
    GFL_G3DMdlFree(wk->unk38);
}

void func_ov021_021c9190(FieldEffectTask *task, void *work) {
    D06StoneTask *wk = work;
    VecFx32 pos;

    if (func_ov010_0216da30(wk->params.actor) != FALSE) {
        CopyActorWPos(wk->params.actor, &pos);
        wk->unk28 = pos;
        func_ov021_021ac1cc(task, &pos);
        wk->unk28.y -= 0x4000;
        wk->unk28.z += 0x2800;
        if (wk->stopped == FALSE) {
            GFL_G3DActorStepAnmFrameLoop(wk->actor, 0, FX32_ONE);
        }
        if (wk->playing == TRUE) {
            if (wk->finished == FALSE) {
                if (GFL_G3DActorStepAnmFrame(wk->actor, 1, FX32_ONE) == FALSE) {
                    wk->finished = TRUE;
                }
            }
        }
    }
}

void func_ov021_021c9208(FieldEffectTask *task, void *work) {
    SRTMatrix srt;
    D06StoneTask *wk = work;

    srt = data_ov021_021dac08;
    MTX_Identity33_(&srt.rotation);
    func_ov021_021ac1b8(task, &srt.translation);
    GFL_G3DSysDrawObjBBoxCull(wk->actor, &srt);
    srt.translation = wk->unk28;
    GFL_G3DSysDrawObjBBoxCull(wk->unk34, &srt);
}

FieldEffectTask *func_ov021_021c9258(FieldEffects *effects, const VecFx32 *pos) {
    D06DenkiTaskParams params;

    params.effect = FieldEffects_GetHandleData(effects, 0xb);
    params.pos = *pos;
    return FieldEffects_TCBCreate(effects, &data_ov021_021dab90, NULL, 0, &params, 0);
}

void func_ov021_021c928c(FieldEffectTask *task) {
    D06MarkTask *wk;
    fx32 frame;

    if (task != NULL) {
        wk = func_ov021_021ac1e0(task);
        if (wk->playing == FALSE) {
            frame = 0;
            wk->playing = TRUE;
            GFL_G3DActorSetAnmFrame(wk->actor, 1, &frame);
        }
    }
}

void func_ov021_021c92b0(FieldEffectTask *task, void *work) {
    D06MarkTask *wk = work;
    D06DenkiTaskParams *params = func_ov021_021ac1ac(task);

    wk->params = *params;
    func_ov021_021ac1cc(task, &wk->params.pos);
    wk->mdl = GFL_G3DMdlCreate(wk->params.effect->mdlRes[2], 0, wk->params.effect->mdlRes[2]);
    wk->anms[0] = GFL_G3DAnmCreate(wk->mdl, wk->params.effect->anmRes[3], 0);
    wk->anms[1] = GFL_G3DAnmCreate(wk->mdl, wk->params.effect->anmRes[4], 0);
    wk->actor = GFL_G3DActorCreate(wk->mdl, wk->anms, 2);
    GFL_G3DActorBindAnm(wk->actor, 0);
    GFL_G3DActorBindAnm(wk->actor, 1);
}

void func_ov021_021c9314(FieldEffectTask *task, void *work) {
    D06MarkTask *wk = work;

    GFL_G3DAnmFree(wk->anms[0]);
    GFL_G3DAnmFree(wk->anms[1]);
    GFL_G3DActorFree(wk->actor);
    GFL_G3DMdlFree(wk->mdl);
}

void func_ov021_021c9334(FieldEffectTask *task, void *work) {
    D06MarkTask *wk = work;

    GFL_G3DActorStepAnmFrameLoop(wk->actor, 0, FX32_ONE);
    if (wk->playing == TRUE) {
        if (GFL_G3DActorStepAnmFrame(wk->actor, 1, FX32_ONE) == FALSE) {
            wk->playing = FALSE;
        }
    }
}

void func_ov021_021c9360(FieldEffectTask *task, void *work) {
    SRTMatrix srt;
    D06MarkTask *wk = work;

    srt = data_ov021_021dac44;
    MTX_Identity33_(&srt.rotation);
    func_ov021_021ac1b8(task, &srt.translation);
    GFL_G3DSysDrawObjBBoxCull(wk->actor, &srt);
}
