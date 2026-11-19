#include "global.h"

#include "screen_fade_fx.h"

#include "constants/heap.h"

#include "sys_task.h"
#include "sys_task_api.h"






void sub_02011068(HardwareWindowSettings *param0, int param1, enum DSScreen screen, int param3);

s32 sub_0200FF88(ScreenFadeHBlanks *, UnkStruct_02011E04 *, s32 *, enum DSScreen, enum HeapID);

s32 sub_02010E64(UnkStruct_02011E04 *, s32, enum DSScreen, enum HeapID);

UnkStruct_02010FC0 *sub_02010EE0(UnkStruct_02011E04 *, s32);

void sub_02010F00(SysTask *task, void *data);

s32 sub_02010F84(HardwareWindowSettings *, s32, s32, enum DSScreen, enum DSScreen, s32, s32, s32, s32, s32);

s32 sub_020127B8(UnkStruct_02011E04 *);

extern s32 sub_02010C38;

void sub_0201268C(UnkStruct_02011E04 *param0, UnkStruct_0200FF30 *param1, s32 param2, s32 param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID);

void sub_0201268C(UnkStruct_02011E04 *param0, UnkStruct_0200FF30 *param1, s32 param2, s32 param3, enum DSScreen screen, HardwareWindowSettings *param5, ScreenFadeHBlanks *param6, enum HeapID heapID) {
    s32 sp1C;
    s32 sp18;
    sp1C = param3;
    param0->unk_0C.unk_00 = (s32)param1->unk_00;
    sp18 = param2;
    param0->unk_0C.unk_04 = (s32)param1->unk_00;
    param0->unk_0C.unk_08 = param1->unk_02 - param1->unk_00;
    sub_02010E64(param0, 2, screen, heapID);
    param0->unk_18 = sp18;
    param0->unk_1C = 0;
    param0->unk_20 = sp1C;
    param0->unk_24 = 0;
    param0->unk_30 = param5;
    param0->unk_34 = param6;
    param0->heapID = heapID;
    param0->unk_28 = (s32)param1->unk_06;
    sub_020127B8(param0);
    SysTask_CreateOnVWaitQueue(sub_02010F00, param0, 0x3FFU);
    sub_02010EE0(param0, 0);
    sub_02010EE0(param0, 1);
    sub_02010F84(param5, param1->unk_04, param1->unk_05, DS_SCREEN_MAIN, screen, 0, 0, 0xFF, 0xC0, param0->unk_28);
    sub_02010F84(param5, param1->unk_04, param1->unk_05, DS_SCREEN_SUB, screen, 0, 0, 0xFF, 0xC0, param0->unk_28);
    sub_02011068(param5, 3, screen, param0->unk_28);
    sub_0200FF88(param0->unk_34, param0, &sub_02010C38, screen, heapID);
}
