#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_29.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_manager.h"
#include "overlay_29_number_entry_data.h"

s32 ov29_0225D620(void);

s32 ov29_0225D648(void *);

s32 ov29_0225D6C8(NumberEntryData *, NARC *);

s32 ov29_0225D714(NumberEntryData *);

s32 ov29_0225D7F0(NumberEntryData *, NARC *);

s32 ov29_0225DBF0(NumberEntryData *);

void ov29_0225DCD0(SysTask *task, void *data);

struct OverlayManager;
BOOL ov29_0225D61C(OverlayManager *appMan, int *state);

SysTask *ov29_0225D520(void *arg0, s32 arg1, s32 arg2, NumberEntryArgs *arg3) {
    SysTask *sysTask;
    void *v0;
    NARC *narc;
    SysTask *v1;
    u32 v3;
    NumberEntryData *v2;
    u32 v4;

    v0 = arg0;
    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000U);
    reg_G2S_DB_BLDCNT = 0;
    v1 = CreateSysTaskAndEnvironment(ov29_0225DCD0, sizeof(NumberEntryData), 0xAU, HEAP_ID_8);
    sysTask = v1;
    v2 = SysTask_GetData(v1);
    v2->bgConfig = arg0;
    v2->unk4 = arg1;
    v2->unk8 = arg2;
    v2->unkC = arg3;
    v2->task = v1;
    v2->args = arg3;
    arg3->result = -1;
    v4 = 0;
    v3 = 0x186A0;
    v2->numDigits = 0;
loop_1:
    if (v2->args->max >= v3) {
        v2->numDigits = (s32)(6 - v4);
    } else {
        v3 = v3 / 10U;
        v4 += 1;
        if (v4 < 6U) {
            goto loop_1;
        }
    }
    if (v2->numDigits == 0) {
        v2->numDigits = 1;
    }
    narc = NARC_New(NARC_a_2_3_6, HEAP_ID_8);
    ov29_0225D620();
    ov29_0225D648(v0);
    ov29_0225D6C8(v2, narc);
    ov29_0225D714(v2);
    ov29_0225D7F0(v2, narc);
    ov29_0225DBF0(v2);
    NARC_Delete(narc);
    return sysTask;
}

void ov29_0225D6B4(BgConfig *);

void ov29_0225D7D4(NumberEntryData *);

void ov29_0225D828(NumberEntryData *);

void ov29_0225DC34(NumberEntryData *arg0);

void ov29_0225D5EC(u32 param_1, SysTask *param_2) {
    NumberEntryData *puVar1;

    puVar1 = SysTask_GetData(param_2);
    ov29_0225DC34(puVar1);
    ov29_0225D828(puVar1);
    ov29_0225D7D4(puVar1);
    ov29_0225D6B4(puVar1->bgConfig);
    DestroySysTaskAndEnvironment(param_2);
    Heap_Destroy(HEAP_ID_8);
    return;
}

BOOL ov29_0225D61C(OverlayManager *appMan, int *state) {
    return 1;
}
