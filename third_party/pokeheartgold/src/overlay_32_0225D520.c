#include "global.h"

#include "filesystem.h"
#include "font.h"
#include "heap.h"
#include "overlay_32.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "unk_0202C034.h"
#include "overlay_32_name_picker_data.h"

SysTask *ov32_0225D520(void *arg0, s32 arg1, void *arg2, s32 *arg3) {
    void *v1;
    s32 v0;
    NARC *narc;
    SysTask *sysTask;
    NamePickerData *data;

    v0 = arg1;
    v1 = arg2;
    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000U);
    reg_G2S_DB_BLDCNT = 0;
    sysTask = CreateSysTaskAndEnvironment(ov32_0225DDC4, sizeof(NamePickerData), 0xAU, HEAP_ID_8);
    data = SysTask_GetData(sysTask);
    data->bgConfig = arg0;
    data->unk4 = v0;
    data->fieldSystem = v1;
    data->unkC = arg3;
    data->task = sysTask;
    data->names = sub_0202C6F4(*(SaveData **)((u8 *)v1 + 0xC));
    data->result = arg3;
    *data->result = -1;
    FontID_Alloc(FONT_UNK_4, HEAP_ID_8);
    narc = NARC_New(NARC_a_2_5_9, HEAP_ID_8);
    ov32_0225D60C();
    ov32_0225D634(arg0);
    ov32_0225D6E0(data, narc);
    ov32_0225D748(data);
    ov32_0225D788(data);
    ov32_0225DA88(data, narc);
    ov32_0225DD74(data);
    NARC_Delete(narc);
    return sysTask;
}
