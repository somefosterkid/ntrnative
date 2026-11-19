#include "global.h"

#include "constants/heap.h"

#include "pm_string.h"
#include "sys_task.h"
#include "overlay_01_021EFB38.h"
#include "sys_task_api.h"
#include "screen_scroll_manager.h"
#include "screen_shake_effect.h"

struct ScreenScrollManager;

void sub_02014C08(ScreenScrollManager *screenScrollMgr);

void sub_02014C40(ScreenScrollManager *screenScrollMgr);

void ov01_021F1290(SysTask *task, void *param);

SysTask *ov01_021F127C(ScreenShakeEffect *screenShake);
ScreenScrollManager *sub_02014AD8(enum HeapID heapID);

void sub_02014B9C(ScreenScrollManager *screenScrollMgr);
void sub_02014BD8(ScreenScrollManager *screenScrollMgr);
void ov01_021F12D0(ScreenShakeEffect *screenShake);

void ov01_021F1290(SysTask *task, void *param) {
    ScreenShakeEffect *screenShakeEfx = param;

    if (screenShakeEfx->dmaCounter >= 2) {
        sub_02014C08(screenShakeEfx->screenScrollMgr);
        screenShakeEfx->dmaCounter = 0;
    }

    sub_02014C40(screenShakeEfx->screenScrollMgr);
    screenShakeEfx->dmaCounter++;
}

void ov01_021F12B4(ScreenShakeEffect *screenShake, enum HeapID heapID) {
    screenShake->screenScrollMgr = sub_02014AD8(heapID);
    screenShake->dmaCounter = 0;
    screenShake->dmaTransferTask = ov01_021F127C(screenShake);
}

void ov01_021F12D0(ScreenShakeEffect *screenShake) {
    SysTask_Destroy(screenShake->dmaTransferTask);
    sub_02014B9C(screenShake->screenScrollMgr);
    sub_02014BD8(screenShake->screenScrollMgr);
}
