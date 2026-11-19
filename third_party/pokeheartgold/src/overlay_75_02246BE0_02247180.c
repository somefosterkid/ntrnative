#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "render_text.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_0200B150.h"
#include "unk_020210A0.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_75_unk_struct_ov98_02247704.h"

void ov75_0224741C(BgConfig *param0);

void ov75_02247838(UnkStruct_ov98_02247704 *param0);

s32 ov75_02247180(OverlayManager *arg0, int *arg1);

s32 ov75_02247180(OverlayManager *man, int *arg1) {
    SysTask *task;
    UnkStruct_ov98_02247704 *v6;

    v6 = OverlayManager_GetData(man);
    task = v6->unk_EC.unk_00;
    if (task != NULL) {
        SysTask_Destroy(task);
        v6->unk_EC.unk_00 = NULL;
        v6->unk_EC.unk_04 = 0;
    }
    DestroyMsgData(v6->unk_34);
    DestroyMsgData(v6->unk_28);
    DestroyMsgData(v6->unk_30);
    DestroyMsgData(v6->unk_2C);
    DestroyMsgData(v6->unk_24);
    MessageFormat_Delete(v6->unk_20);
    String_Delete(v6->unk_3C);
    String_Delete(v6->unk_40);
    String_Delete(v6->unk_38);
    ov75_02247838(v6);
    Heap_Free(v6->unk_04);
    ov75_0224741C(v6->unk_04);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GF_DestroyVramTransferManager();
    DisableTouchPad();
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    sub_0203A914();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_116);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    return 1;
}

void ov75_02247234(void *param0);

void ov75_02247234(void *param0) {
    UnkStruct_ov98_02247704 *v0 = param0;

    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();

    DoScheduledBgGpuUpdates(v0->unk_04);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
