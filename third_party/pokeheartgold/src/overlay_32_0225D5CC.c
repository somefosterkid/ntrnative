#include "global.h"

#include "constants/heap.h"

#include "font.h"
#include "heap.h"
#include "overlay_32.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_manager.h"
#include "overlay_32_name_picker_data.h"

struct OverlayManager;
BOOL ov32_0225D608(OverlayManager *appMan, int *state);

void ov32_0225D5CC(u32 param_1, SysTask *param_2) {
    NamePickerData *data;

    data = SysTask_GetData(param_2);
    ov32_0225DDAC(data);
    ov32_0225DAC0(data);
    ov32_0225D834(data);
    ov32_0225D76C(data);
    ov32_0225D6C4(data->bgConfig);
    FontID_Release(FONT_UNK_4);
    DestroySysTaskAndEnvironment(param_2);
    Heap_Destroy(HEAP_ID_8);
    return;
}

BOOL ov32_0225D608(OverlayManager *appMan, int *state) {
    return 1;
}

void ov32_0225D60C(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    reg_GXS_DB_DISPCNT = 0x10 | (reg_GXS_DB_DISPCNT & 0xFFCFFFEF);
}
