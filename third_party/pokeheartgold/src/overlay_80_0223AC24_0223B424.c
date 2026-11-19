#include "global.h"

#include "overlay_80_0223AC24.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "screen_scroll_manager.h"
#include "screen_shake_effect.h"

SysTask *ov80_0223B3EC(ScreenShakeEffect *screenShake);

ScreenScrollManager *sub_02014AD8(enum HeapID heapID);

struct ScreenScrollManager;
void sub_02014B08(ScreenScrollManager *screenScrollMgr, u8 start, u8 end, u16 angleIncrement, fx32 amplitude, s16 speed, void *bg, u32 initValue, u32 priority);
void sub_02014AA0(void);
typedef void (*UnkFuncPtr_ov104_0223F110)(void *);
typedef struct {
    SysTask *unk_00;
    SysTask *unk_04;
    BOOL unk_08;
    BOOL unk_0C;
    UnkFuncPtr_ov104_0223F110 unk_10;
    UnkFuncPtr_ov104_0223F110 unk_14;
    void *unk_18;
} UnkStruct_ov104_0223F0D8;
void ov80_0223B440(ScreenShakeEffect *screenShake, u8 startX, u8 endX, u16 angleIncrement, fx32 amplitude, s16 shakeSpeed, void *bg, u32 defaultValue, u32 priority);
void ov80_0223B4A0(UnkStruct_ov104_0223F0D8 *param0, void *param1, UnkFuncPtr_ov104_0223F110 param2, UnkFuncPtr_ov104_0223F110 param3);
void ov80_0223B4E0(UnkStruct_ov104_0223F0D8 *param0);

void ov80_0223B424(ScreenShakeEffect *screenShake, enum HeapID heapID) {
    screenShake->screenScrollMgr = sub_02014AD8(heapID);
    screenShake->dmaCounter = 0;
    screenShake->dmaTransferTask = ov80_0223B3EC(screenShake);
}

void ov80_0223B440(ScreenShakeEffect *screenShake, u8 startX, u8 endX, u16 angleIncrement, fx32 amplitude, s16 shakeSpeed, void *bg, u32 defaultValue, u32 priority) {
    sub_02014B08(screenShake->screenScrollMgr, startX, endX, angleIncrement, amplitude, shakeSpeed, bg, defaultValue, priority);
}

void ov80_0223B468(SysTask *param0, void *param1) {
    UnkStruct_ov104_0223F0D8 *v0 = param1;

    if (v0->unk_08) {
        v0->unk_0C = 1;

        v0->unk_10(v0->unk_18);
    } else {
        sub_02014AA0();
    }
}

void ov80_0223B484(SysTask *param0, void *param1) {
    UnkStruct_ov104_0223F0D8 *v0 = param1;

    if (v0->unk_08 && v0->unk_0C) {
        v0->unk_14(v0->unk_18);

        v0->unk_0C = 0;
    }
}

void ov80_0223B4A0(UnkStruct_ov104_0223F0D8 *param0, void *param1, UnkFuncPtr_ov104_0223F110 param2, UnkFuncPtr_ov104_0223F110 param3) {
    ((param0) ? (void)0 : GF_AssertFail());

    param0->unk_08 = 1;
    param0->unk_0C = 0;
    param0->unk_18 = param1;
    param0->unk_10 = param2;
    param0->unk_14 = param3;
    param0->unk_04 = SysTask_CreateOnVWaitQueue(ov80_0223B468, param0, 0);
    param0->unk_00 = SysTask_CreateOnVBlankQueue(ov80_0223B484, param0, 0);
}

void ov80_0223B4E0(UnkStruct_ov104_0223F0D8 *param0) {
    ((param0) ? (void)0 : GF_AssertFail());

    if (param0->unk_04 != 0L) {
        SysTask_Destroy(param0->unk_04);
    }

    if (param0->unk_00 != 0L) {
        SysTask_Destroy(param0->unk_00);
    }

    sub_02014AA0();
}
