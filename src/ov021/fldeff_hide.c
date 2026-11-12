// Black's code at ov021 0x021d038c-0x021d046c. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_effect.h"
#include "field/field_g3dobj.h"
#include "field/fldeff_hide.h"
#include "gfl/heap.h"

// A kind's model and its animation (pokebw2's HideResIds)
typedef struct {
    u16 modelId;
    u16 anmId;
} HideResIds;

void func_ov021_021d03c4(FieldEffectHide *effect);
void func_ov021_021d0440(FieldEffectHide *effect);

void *func_ov021_021d038c(FieldEffects *param_1, HeapID param_2) {
    FieldEffectHide *effect;

    effect = GFL_HeapAllocate(param_2, sizeof(FieldEffectHide), 1, "fldeff_hide.c", 0x6b);
    effect->effects = param_1;
    func_ov021_021d03c4(effect);
    return effect;
}

void func_ov021_021d03b0(FieldEffects *param_1, void *param_2) {
    func_ov021_021d0440(param_2);
    GFL_HeapFree(param_2);
}

static const HideResIds data_ov021_021db92c[HIDE_KIND_COUNT] = { { 0x005e, 0x0069 }, { 0x005d, 0x0068 } };
void func_ov021_021d03c4(FieldEffectHide *effect) {
    FieldG3DObjResRequest req;
    ArcTool *arc;
    FieldG3DObjSystem *sys;
    s32 i;
    const HideResIds *ids;

    i = 0;
    ids = data_ov021_021db92c;
    arc = FieldEffects_GetArc(effect->effects);
    sys = func_ov021_021abe18(effect->effects);
    do {
        FieldG3DObjResRequest_Clear(&req);
        FieldG3DObjResRequest_SetModel(&req, arc, ids->modelId);
        FieldG3DObjResRequest_SetAnmArc(&req, arc);
        FieldG3DObjResRequest_AddAnm(&req, ids->anmId);
        effect->resGroup[i] = FieldG3DObjSystem_AddResGroup(sys, &req, 0);
        effect->objIdx[i] = FieldG3DObjSystem_AddObj(sys, effect->resGroup[i], 0, NULL);
        func_ov021_021c67d8(sys, effect->objIdx[i], 1);
        i++;
        ids++;
    } while (i < HIDE_KIND_COUNT);
}


void func_ov021_021d0440(FieldEffectHide *effect) {
    FieldG3DObjSystem *sys;
    s32 i;

    i = 0;
    sys = func_ov021_021abe18(effect->effects);
    do {
        FieldG3DObjSystem_FreeObj(sys, effect->objIdx[i]);
        FieldG3DObjSystem_FreeResGroup(sys, effect->resGroup[i]);
        i = i + 1;
    } while (i < HIDE_KIND_COUNT);
}
