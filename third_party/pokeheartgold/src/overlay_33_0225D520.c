#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_33.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_33_ov33work.h"

typedef struct Ov33Args {
    const u32 *strIds;
    u8 count;
    u8 unk5;
    u16 unk6;
} Ov33Args;

void ov33_0225D5D0(SysTask *task, void *taskEnv);

void ov33_0225D720(BgConfig *bgConfig);

void ov33_0225D7D4(Ov33Work *work);

void ov33_0225D84C(Ov33Work *work);

void ov33_0225D8D4(Ov33Work *work);

SysTask *ov33_0225D520(BgConfig *bgConfig, void *unk4, u32 unk8, Ov33Args *args);

SysTask *ov33_0225D520(BgConfig *bgConfig, void *unk4, u32 unk8, Ov33Args *args) {
    SysTask *task;
    Ov33Work *work;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);
    reg_G2S_DB_BLDCNT = 0;
    task = CreateSysTaskAndEnvironment(ov33_0225D5D0, sizeof(Ov33Work), 10, HEAP_ID_8);
    work = SysTask_GetData(task);
    work->bgConfig = bgConfig;
    work->unk4 = unk4;
    work->unk8 = unk8;
    work->unkC = args;
    work->sysTask = task;
    work->unk10 = args;
    work->count = work->unk10->count;
    work->unk10->unk6 = 0xFFFF;
    work->unk10->unk5 = 0;
    ov33_0225D6F8();
    ov33_0225D720(bgConfig);
    ov33_0225D7D4(work);
    ov33_0225D84C(work);
    ov33_0225D8D4(work);
    return task;
}
