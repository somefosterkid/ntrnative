#include "global.h"

#include "overlay_01_map_load_manager.h"
#include "overlay_01_021FAC44.h"

void ov01_021FACF8(u32 param_1, u32 param_2, u32 param_3, MapLoadBuffers *param_4) {
    MI_CpuFill8(param_4->primary[param_1], 0, param_2);
    MI_CpuFill8(param_4->secondary[param_1], 0, param_3);
    return;
}
