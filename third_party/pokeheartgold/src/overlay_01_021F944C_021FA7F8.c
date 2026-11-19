#include "global.h"

#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"
#include "sys_task.h"

void ov01_021FA7C8(UnkStruct_ov5_021ED0A4 *gfx);

void ov01_021FA7F8(SysTask *task, void *param1);

void ov01_021FA7F8(SysTask *task, void *param1) {
    UnkStruct_ov5_021ED0A4 *gfx = param1;
    ov01_021FA7C8(gfx);
}
