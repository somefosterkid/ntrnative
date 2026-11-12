// Black's code at ov021 0x021d0160-0x021d038c: the rest of fldeff_btrain.c (pokebw2's src/ov036/fldeff_btrain.c), the
// Battle Subway's trains. No name is known: the module and address.

#include "types.h"
#include "field/field_effect.h"
#include "field/fldeff_btrain.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "nitro/fx/fx_mtx.h"

// What a train starts from
typedef struct {
    FieldEffectBTrain *effect;
    u32 trainIdx;
} BTrainTaskParams;

typedef struct {
    u8 unk0;
    u8 hidden;
    u8 anmIdx;
    u8 anmDone;
    FieldEffectBTrain *effect;
    u32 trainIdx;
    G3DActor *actor;
    void *anm[BTRAIN_ANM_COUNT];
} BTrainTask;

void func_ov021_021d01d8(FieldEffectBTrain *effect);

void func_ov021_021d0160(FieldEffects *effects, void *data) {
    func_ov021_021d01d8(data);
    GFL_HeapFree(data);
}

void func_ov021_021d029c(FieldEffectTask *task, void *work);
void func_ov021_021d0308(FieldEffectTask *task, void *work);
void func_ov021_021d0328(FieldEffectTask *task, void *work);
void func_ov021_021d034c(FieldEffectTask *task, void *work);

static const u16 BATTLE_SUBWAY_TRAIN_EFFECT_DATIDS[8] = { 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c };
static const u16 data_ov021_021db8c4[4] = { 0x7c, 0x7d, 0x7e, 0x7f };

void func_ov021_021d0174(FieldEffectBTrain *effect, s32 trainIdx) {
    const u16 *anmIds;
    const u16 *trainIds;
    ArcTool *arc;
    s32 i;

    arc = FieldEffects_GetArc(effect->effects);
    trainIds = BATTLE_SUBWAY_TRAIN_EFFECT_DATIDS;
    if (effect->mdlRes[trainIdx] == NULL) {
        effect->mdlRes[trainIdx] = GFL_G3DSysReadArcToolResource(arc, trainIds[trainIdx]);
        GFL_G3DResUploadTexData(effect->mdlRes[trainIdx]);
        effect->mdl[trainIdx] = GFL_G3DMdlCreate(effect->mdlRes[trainIdx], 0, effect->mdlRes[trainIdx]);
    }
    anmIds = data_ov021_021db8c4;
    for (i = 0; i < 4; i++) {
        if (effect->anmRes[i] == NULL) {
            effect->anmRes[i] = GFL_G3DSysReadArcToolResource(arc, *anmIds);
        }
        anmIds++;
    }
}

void func_ov021_021d01d8(FieldEffectBTrain *effect) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (effect->mdlRes[i] != NULL) {
            GFL_G3DMdlFree(effect->mdl[i]);
            GFL_G3DResFreeTexData(effect->mdlRes[i]);
            GFL_G3DResFree(effect->mdlRes[i]);
        }
    }
    for (i = 0; i < 4; i++) {
        if (effect->anmRes[i] != NULL) {
            GFL_G3DResFree(effect->anmRes[i]);
        }
    }
}

static const FieldEffectTaskVTable data_ov021_021db8dc = {
    sizeof(BTrainTask), func_ov021_021d029c, func_ov021_021d0308, func_ov021_021d0328, func_ov021_021d034c
};

FieldEffectTask *func_ov021_021d0218(FieldEffects *effects, u32 trainIdx, const VecFx32 *pos) {
    BTrainTaskParams params;

    params.effect = FieldEffects_GetHandleData(effects, 0xe);
    params.trainIdx = trainIdx;
    func_ov021_021d0174(params.effect, trainIdx);
    return FieldEffects_TCBCreate(effects, &data_ov021_021db8dc, pos, 0, &params, 0);
}

void func_ov021_021d024c(FieldEffectTask *task, s32 anmIdx) {
    s32 i;
    BTrainTask *trainTask;

    if (task != NULL) {
        trainTask = func_ov021_021ac1e0(task);
        for (i = 0; i < 4; i++) {
            GFL_G3DActorResetAnmFrame(trainTask->actor, i);
            if (i == anmIdx) {
                GFL_G3DActorBindAnm(trainTask->actor, i);
            } else {
                GFL_G3DActorUnbindAnm(trainTask->actor, i);
            }
        }
        trainTask->anmIdx = anmIdx;
        trainTask->anmDone = FALSE;
    }
}

void func_ov021_021d028c(FieldEffectTask *task, u32 hidden) {
    if (task != NULL) {
        BTrainTask *trainTask = func_ov021_021ac1e0(task);

        trainTask->hidden = hidden;
    }
}

void func_ov021_021d029c(FieldEffectTask *task, void *work) {
    BTrainTask *trainTask = work;
    BTrainTaskParams *params;
    FieldEffectBTrain *effect;
    s32 trainIdx;
    s32 i;
    s32 j;

    params = func_ov021_021ac1ac(task);
    effect = params->effect;
    trainIdx = params->trainIdx;
    trainTask->effect = effect;
    trainTask->trainIdx = trainIdx;
    for (i = 0; i < 4; i++) {
        trainTask->anm[i] = GFL_G3DAnmCreate(effect->mdl[trainIdx], effect->anmRes[i], 0);
    }
    trainTask->anmIdx = 4;
    trainTask->actor = GFL_G3DActorCreate(effect->mdl[trainIdx], trainTask->anm, 4);
    for (j = 0; j < 4; j++) {
        GFL_G3DActorUnbindAnm(trainTask->actor, j);
    }
}

void func_ov021_021d0308(FieldEffectTask *task, void *work) {
    BTrainTask *trainTask = work;
    s32 i;

    GFL_G3DActorFree(trainTask->actor);
    for (i = 0; i < 4; i++) {
        GFL_G3DAnmFree(trainTask->anm[i]);
    }
}

void func_ov021_021d0328(FieldEffectTask *task, void *work) {
    BTrainTask *trainTask = work;

    if (trainTask->anmIdx != 4) {
        trainTask->anmDone = FALSE;
        if (GFL_G3DActorStepAnmFrame(trainTask->actor, trainTask->anmIdx, 0x1000) == FALSE) {
            trainTask->anmDone = TRUE;
        }
    }
}

static const SRTMatrix data_ov021_021db8f0 = { { 0, 0, 0 }, { 0x1000, 0x1000, 0x1000 }, { 0 } };
void func_ov021_021d034c(FieldEffectTask *task, void *work) {
    BTrainTask *trainTask = work;
    SRTMatrix srt;

    if (trainTask->hidden == 0) {
        srt = data_ov021_021db8f0;
        MTX_Identity33_(&srt.rotation);
        func_ov021_021ac1b8(task, &srt.translation);
        GFL_G3DSysDrawObjBBoxCull(trainTask->actor, &srt);
    }
}
