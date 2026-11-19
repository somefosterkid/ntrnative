#include "global.h"

#include "error_handling.h"
#include "field_system.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_34.h"
#include "player_data.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "unk_0200A090.h"

Ov34Work *ov34_0225D7A8(FieldSystem *fieldSystem) {
    SysTask *sysTask;
    Ov34Work *work;

    sysTask = CreateSysTaskAndEnvironment(ov34_0225D5A0, sizeof(Ov34Work), 4U, HEAP_ID_FIELD1);
    work = SysTask_GetData(sysTask);
    work->fieldSystem = fieldSystem;
    work->bgConfig = fieldSystem->bgConfig;
    work->unk4 = fieldSystem->unk84;
    work->unk8 = fieldSystem->unk80;
    work->profile = Save_PlayerData_GetProfile(fieldSystem->saveData);
    work->state = 2;
    work->task = sysTask;
    work->chatLog = fieldSystem->unk84->chatLog;
    ov34_0225D924(work->bgConfig);
    ov34_0225D5F8(work);
    SetKeyRepeatTimers(4, 8);
    ov34_0225E56C(work);
    ov34_0225E5DC(work, 1);
    ov34_0225DA50(work);
    ov34_0225DB20(work);
    FontID_SetAccessDirect(FONT_MESSAGE, HEAP_ID_FIELD1);
    ov34_0225D650(work->bgConfig, work->rows, &work->titleWindow);
    ov34_0225D520(work);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(1U, 1U);
    GfGfx_EngineBTogglePlanes(2U, 1U);
    GfGfx_EngineBTogglePlanes(4U, 1U);
    GfGfx_EngineBTogglePlanes(8U, 1U);
    ov34_0225E560(work);
    return work;
}

void ov34_0225D87C(Ov34Work *work) {
    BgConfig *bgConfig;
    s32 i;

    if ((u32)(work->state - 2) <= 2U) {
        bgConfig = work->bgConfig;
        work->state = 5;
        FontID_SetAccessLazy(FONT_MESSAGE);
        SpriteTransfer_DeleteCharTransferTask(work->resObjs[0]);
        SpriteTransfer_DeletePlttTransferTask(work->resObjs[1]);
        i = 0;
        do {
            Destroy2DGfxResObjMan(work->resMans[i]);
            i += 1;
        } while (i < 4);
        SpriteList_Delete(work->spriteList);
        DestroyMsgData(work->msgData);
        MessageFormat_Delete(work->msgFormat);
        ov34_0225D77C(work->rows, &work->titleWindow);
        ov34_0225D900(bgConfig);
        Heap_Free(work->scrnBuf);
        DestroySysTaskAndEnvironment(work->task);
        return;
    }
    GF_AssertFail();
}
