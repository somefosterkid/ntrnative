#include "global.h"

#include "sys_task_api.h"
#include "overlay_96_unk_struct_ov96_0220_be28.h"

void ov96_0220C15C(SysTask *task, void *data);

void ov96_0220C3E0(void *arg0);

void ov96_0220C3E0(void *arg0) {
    *((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4)) = (s32)(0x4000 | (*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4)) & ~0x3C & 0xFFFFC03F));
    *((SysTask **)&((UnkStruct_ov96_0220BE28 *)arg0)->w34) = SysTask_CreateOnMainQueue(ov96_0220C15C, arg0, 1U);
}
