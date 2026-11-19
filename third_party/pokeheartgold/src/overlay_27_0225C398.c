#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "overlay_27.h"
#include "render_text.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "unk_0200A090.h"
#include "overlay_manager.h"

struct OverlayManager;
BOOL ov27_0225C418(OverlayManager *appMan, int *state);

void ov27_0225C398(BgConfig *arg0, SysTask *task) {
    SysTask *sysTask;
    BgConfig *bgConfig;
    s32 i;
    Ov27TaskData *data;

    bgConfig = arg0;
    sysTask = task;
    data = SysTask_GetData(task);
    SpriteTransfer_DeleteCharTransferTask(data->resObj[0]);
    SpriteTransfer_DeletePlttTransferTask(data->resObj[1]);
    i = 0;
    do {
        Destroy2DGfxResObjMan(data->resMan[i]);
        i += 1;
    } while (i < 4);
    SpriteList_Delete(data->spriteList);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    ov27_0225C930(data);
    FontID_Release(FONT_UNK_4);
    DestroySysTaskAndEnvironment(sysTask);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    Heap_Destroy(HEAP_ID_8);
}

BOOL ov27_0225C418(OverlayManager *appMan, int *state) {
    return 1;
}

void ov27_0225C41C(SysTask *task, Ov27TouchMenuCallback callback, struct FieldMenuManager *menu) {
    Ov27TaskData *data;

    data = SysTask_GetData(task);
    data->callback = callback;
    data->callbackMenu = menu;
}
