// Black's code at ov021 0x021bbf78-0x021bbfb8. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "gfl/heap.h"
#include "field/field_effect.h"
#include "field/fldeff_reflect.h"

void func_ov021_021bbfb0(void);
void func_ov021_021bbfb4(void *data);

void *func_ov021_021bbf78(FieldEffects *param_1, HeapID param_2) {
    FieldEffectReflect *effect;

    effect = GFL_HeapAllocate(param_2, sizeof(FieldEffectReflect), 1, "fldeff_reflect.c", 0x78);
    effect->effects = param_1;
    func_ov021_021bbfb0();
    return effect;
}

void func_ov021_021bbf9c(FieldEffects *effects, void *data) {
    func_ov021_021bbfb4(data);
    GFL_HeapFree(data);
}

void func_ov021_021bbfb0(void) {
}

void func_ov021_021bbfb4(void *data) {
}
