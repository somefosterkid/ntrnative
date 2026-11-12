// Black's code at ov021 0x021c9550-0x021c96cc. No name is known: the module and address.

#include "types.h"
#include "field/field_effect.h"
#include "gfl/g3d.h"
#include "field/field_actor.h"
#include "field/fldeff_iaigiri.h"
#include "nitro/fx/fx_mtx.h"

// the params func_ov021_021ac1ac returns
typedef struct {
    FieldEffectIaigiri *effect;
    VecFx32 pos;
} Ov021C9550Params;

// the work of the field effect task data_ov021_021dac80 runs (FieldEffectTaskFunc's work)
typedef struct {
    FieldEffectIaigiri *effect;
    G3DActor *actor;
    void *anm[3];
    G3DModel *model;
} Ov021C9550Work;



void func_ov021_021c95d8(FieldEffectTask *task, void *work);
void func_ov021_021c963c(FieldEffectTask *task, void *work);
void func_ov021_021c9660(FieldEffectTask *task, void *work);
void func_ov021_021c9690(FieldEffectTask *task, void *work);

const FieldEffectTaskVTable data_ov021_021dac80 = {
    sizeof(Ov021C9550Work),
    func_ov021_021c95d8,
    func_ov021_021c963c,
    func_ov021_021c9660,
    func_ov021_021c9690,
};

void func_ov021_021c9550(FieldActor *actor, FieldEffects *effects) {
    VecFx32 pos;
    Ov021C9550Params params;

    CopyActorWPos(actor, &pos);
    switch (GetActorFaceDir(actor)) {
    case 0:
        pos.z -= 0x10000;
        break;
    case 1:
        pos.z += 0x10000;
        break;
    case 2:
        pos.x -= 0x10000;
        break;
    case 3:
        pos.x += 0x10000;
        break;
    }
    params.effect = FieldEffects_GetHandleData(effects, 0xc);
    params.pos = pos;
    FieldEffects_TCBCreate(effects, &data_ov021_021dac80, NULL, 0, &params, 0);
}

void func_ov021_021c95d8(FieldEffectTask *task, void *work) {
    Ov021C9550Work *w = work;
    int i;
    Ov021C9550Params *params = func_ov021_021ac1ac(task);

    w->effect = params->effect;
    func_ov021_021ac1cc(task, &params->pos);
    w->model = GFL_G3DMdlCreate(w->effect->mdlRes, 0, w->effect->mdlRes);
    for (i = 0; i < 3; i++) {
        w->anm[i] = GFL_G3DAnmCreate(w->model, w->effect->anmRes[i], 0);
    }
    w->actor = GFL_G3DActorCreate(w->model, w->anm, 3);
    for (i = 0; i < 3; i++) {
        GFL_G3DActorBindAnm(w->actor, i);
    }
}

void func_ov021_021c963c(FieldEffectTask *task, void *work) {
    Ov021C9550Work *w = work;
    s32 i;

    i = 0;
    do {
        GFL_G3DAnmFree(w->anm[i]);
        i++;
    } while (i < 3);
    GFL_G3DActorFree(w->actor);
    GFL_G3DMdlFree(w->model);
}

void func_ov021_021c9660(FieldEffectTask *task, void *work) {
    Ov021C9550Work *w = work;
    BOOL done;
    s32 i;

    done = TRUE;
    i = 0;
    do {
        if (GFL_G3DActorStepAnmFrame(w->actor, (u16)i, 0x1000) == TRUE) {
            done = FALSE;
        }
        i++;
    } while (i < 3);
    if (done) {
        func_ov021_021ac154(task);
    }
}

void func_ov021_021c9690(FieldEffectTask *task, void *work) {
    Ov021C9550Work *w = work;
    SRTMatrix srt = { { 0, 0, 0 }, { FX32_ONE, FX32_ONE, FX32_ONE } };

    MTX_Identity33_(&srt.rotation);
    func_ov021_021ac1b8(task, &srt.translation);
    GFL_G3DSysDrawObjBBoxCull(w->actor, &srt);
}


