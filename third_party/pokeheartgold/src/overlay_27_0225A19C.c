#include "global.h"

#include <dsprot.h>

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_27.h"
#include "overlay_27_ov27task.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

FS_EXTERN_OVERLAY(ds_protect);

void ov27_0225A19C(BgConfig *arg0, SysTask *arg1);

void ov27_0225A19C(BgConfig *bgConfig, SysTask *sysTask) {
    Ov27Task *task;
    s32 i;

    task = SysTask_GetData(sysTask);
    FS_LoadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(ds_protect));
    if (DSProt_DetectFlashcart(ov27_0225C238) != 0) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    ov27_0225BEB0(&task->unk_520);
    DestroyMsgData(task->unk_4A8);
    MessageFormat_Delete(task->unk_4AC);
    for (i = 0; i < 11; i++) {
        SpriteTransfer_DeleteCharTransferTask(task->unk_154[i][0]);
    }
    for (i = 0; i < 11; i++) {
        SpriteTransfer_DeletePlttTransferTask(task->unk_154[i][1]);
    }
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(task->unk_144[i]);
    }
    if (DSProt_DetectNotEmulator(ov27_0225C248) == 0) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    SpriteList_Delete(task->unk_18);
    for (i = 0; i < 8; i++) {
        RemoveWindow(&task->unk_3F0[i]);
    }
    RemoveWindow(&task->unk_3E0);
    RemoveWindow(&task->unk_3D0);
    ov27_0225BC34(task);
    FontID_Release(FONT_UNK_4);
    DestroySysTaskAndEnvironment(sysTask);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(bgConfig, GF_BG_LYR_SUB_0);
    Heap_Destroy(HEAP_ID_8);
    if (DSProt_DetectNotDummy(ov27_0225C24C) == 0) {
        Heap_AllocAtEnd(HEAP_ID_3, 1000);
    }
    FS_UnloadOverlay(MI_PROCESSOR_ARM9, FS_OVERLAY_ID(ds_protect));
}
