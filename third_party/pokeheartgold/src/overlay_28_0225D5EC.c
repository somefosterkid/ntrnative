#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_28.h"
#include "sys_task.h"
#include "systask_environment.h"

void ov28_0225D5EC(BgConfig *bgConfig, SysTask *appTask) {
    DowsingMchnApp *app;

    app = SysTask_GetData(appTask);
    DowsingMchn_FreeHiddenItemLocs(app);
    ov28_0225D8D0(app);
    ov28_0225D878(app);
    ov28_0225D7C4(app);
    ov28_0225D6E0(app->bgConfig);
    DestroySysTaskAndEnvironment(appTask);
    Heap_Destroy(HEAP_ID_8);
    return;
}

BOOL ov28_0225D624(BgConfig *bgConfig) {
    return 1;
}

void ov28_0225D628(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    reg_GXS_DB_DISPCNT = 0x10 | (reg_GXS_DB_DISPCNT & 0xFFCFFFEF);
}
