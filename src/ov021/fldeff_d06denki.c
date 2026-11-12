// Black's code at ov021 0x021c8e68-0x021c8f7c. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "field/field_effect.h"
#include "field/fldeff_d06denki.h"

void func_ov021_021c8ea0(FieldEffectD06Denki *effect);
void func_ov021_021c8f38(FieldEffectD06Denki *effect);

void *func_ov021_021c8e68(FieldEffects *param_1, HeapID param_2) {
    FieldEffectD06Denki *effect;

    effect = GFL_HeapAllocate(param_2, sizeof(FieldEffectD06Denki), 1, "fldeff_d06denki.c", 0xa9);
    effect->effects = param_1;
    func_ov021_021c8ea0(effect);
    return effect;
}

void func_ov021_021c8e8c(FieldEffects *param_1, void *param_2) {
    func_ov021_021c8f38(param_2);
    GFL_HeapFree(param_2);
}

void func_ov021_021c8ea0(FieldEffectD06Denki *effect) {
    u32 mdlIds[4] = { 80, 78, 79, 81 };
    u32 anmIds[5] = { 102, 120, 121, 122, 123 };
    u32 i;
    ArcTool *arc = FieldEffects_GetArc(effect->effects);

    for (i = 0; i < 4; i++) {
        effect->mdlRes[i] = GFL_G3DSysReadArcToolResource(arc, mdlIds[i]);
        GFL_G3DResUploadTexData(effect->mdlRes[i]);
    }
    for (i = 0; i < 5; i++) {
        effect->anmRes[i] = GFL_G3DSysReadArcToolResource(arc, anmIds[i]);
    }
    effect->mdl = GFL_G3DMdlCreate(effect->mdlRes[0], 0, effect->mdlRes[0]);
    effect->anm = GFL_G3DAnmCreate(effect->mdl, effect->anmRes[0], 0);
    effect->actor = GFL_G3DActorCreate(effect->mdl, &effect->anm, 1);
    GFL_G3DActorBindAnm(effect->actor, 0);
}


static const u32 data_ov021_021dab7c[5] = { 102, 120, 121, 122, 123 };

void func_ov021_021c8f38(FieldEffectD06Denki *effect) {
    u32 i;

    GFL_G3DAnmFree(effect->anm);
    GFL_G3DActorFree(effect->actor);
    GFL_G3DMdlFree(effect->mdl);
    i = 0;
    do {
        GFL_G3DResFreeTexData(effect->mdlRes[i]);
        GFL_G3DResFree(effect->mdlRes[i]);
        i = i + 1;
    } while (i < 4);
    i = 0;
    do {
        GFL_G3DResFree(effect->anmRes[i]);
        i = i + 1;
    } while (i < 5);
}
