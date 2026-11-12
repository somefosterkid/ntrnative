// Black's code at ov021 0x021c94bc-0x021c9550. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "field/field_effect.h"
#include "field/field_internal.h"
#include "field/fldeff_iaigiri.h"
#include "gfl/tcb.h"
#include "nitro/fs.h"
#include "struct_decls.h"

void func_ov021_021c94f4(FieldEffectIaigiri *effect);
void func_ov021_021c952c(FieldEffectIaigiri *effect);

static u32 data_ov021_021de56c[IAIGIRI_ANM_COUNT] = { 114, 130, 149 };

void *func_ov021_021c94bc(FieldEffects *param_1, HeapID param_2) {
    FieldEffectIaigiri *effect;

    effect = GFL_HeapAllocate(param_2, sizeof(FieldEffectIaigiri), 1, "fldeff_iaigiri.c", 0x66);
    effect->effects = param_1;
    func_ov021_021c94f4(effect);
    return effect;
}

void func_ov021_021c94e0(FieldEffects *param_1, void *param_2) {
    func_ov021_021c952c(param_2);
    GFL_HeapFree(param_2);
}

void func_ov021_021c94f4(FieldEffectIaigiri *effect) {
    ArcTool *handle;
    s32 i;

    handle = FieldEffects_GetArc(effect->effects);
    effect->mdlRes = GFL_G3DSysReadArcToolResource(handle, 0x52U);
    GFL_G3DResUploadTexData(effect->mdlRes);
    i = 0;
    do {
        effect->anmRes[i] = GFL_G3DSysReadArcToolResource(handle, data_ov021_021de56c[i]);
        i += 1;
    } while (i < IAIGIRI_ANM_COUNT);
}

void func_ov021_021c952c(FieldEffectIaigiri *effect) {
    s32 i;

    i = 0;
    do {
        GFL_G3DResFree(effect->anmRes[i]);
        i = i + 1;
    } while (i < IAIGIRI_ANM_COUNT);
    GFL_G3DResFreeTexData(effect->mdlRes);
    GFL_G3DResFree(effect->mdlRes);
}
