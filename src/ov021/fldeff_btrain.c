// Black's code at ov021 0x021d0140-0x021d0160. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_effect.h"
#include "field/fldeff_btrain.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

void *func_ov021_021d0140(FieldEffects *param_1, HeapID param_2) {
    FieldEffectBTrain *ptr;

    ptr = GFL_HeapAllocate(param_2, sizeof(FieldEffectBTrain), 1, "fldeff_btrain.c", 0x5f);
    ptr->effects = param_1;
    return ptr;
}
