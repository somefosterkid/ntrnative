#include "global.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sys_task_api.h"
#include "system.h"
#include "overlay_46_unk_struct_ov67_0225d154.h"

s32 ov46_022592B8(void *);

s32 ov46_02259494(void *);

s32 ov46_02258EFC(OverlayManager *arg0);

s32 ov46_02258EFC(OverlayManager *arg0) {
    SysTask *temp_r0;
    void *temp_r4;

    temp_r4 = OverlayManager_GetData(arg0);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    temp_r0 = ((UnkStruct_ov67_0225D154 *)temp_r4)->unk_D4.unk_00;
    if (temp_r0 != NULL) {
        SysTask_Destroy(temp_r0);
        ((UnkStruct_ov67_0225D154 *)temp_r4)->unk_D4.unk_00 = NULL;
        *((s32 *)&((UnkStruct_ov67_0225D154 *)temp_r4)->unk_D4.unk_04) = 0;
    }
    ov46_02259494(temp_r4 + 0x40);
    ov46_02259494(temp_r4 + 0xA0);
    ov46_02259494(temp_r4 + 0x10);
    ov46_022592B8(temp_r4);
    *((s32 *)&((UnkStruct_ov67_0225D154 *)temp_r4)->unk_D4.unk_04) = 0;
    Heap_Free(temp_r4);
    Heap_Destroy(HEAP_ID_119);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    return 1;
}
