#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"

void sub_0200F478(void *taskData);

void ov90_02259570(UnkStruct_ov114_0225D338 *param0, u32 param1);

void ov90_02259570(UnkStruct_ov114_0225D338 *param0, u32 param1) {
    if (param0->unk_2C[param1]) {
        sub_0200F478(param0->unk_2C[param1]);
        param0->unk_2C[param1] = NULL;
    }
}
