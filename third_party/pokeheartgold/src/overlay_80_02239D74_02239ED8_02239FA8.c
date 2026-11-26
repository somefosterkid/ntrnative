#include "global.h"

#include "overlay_80_02239F48.h"
#include "palette.h"
#include "sys_task.h"

void ov80_02239FA8(SysTask *task, void *param);

void ov80_02239FA8(SysTask *task, void *param) {
    UnkStruct_ov80_02239F48 *arg1 = param;
    u8 *new_var;
    if (((u32)(arg1->unk_69)) < 8U) {
        new_var = &arg1->unk_69;
        *new_var = (u8)((*new_var) + 1);
        return;
    }
    arg1->unk_69 = 0U;
    PaletteData_LoadPalette(arg1->unk_04, ((void *)arg1->unk_08) + (((arg1->unk_68) << 4) << 1), PLTTBUF_MAIN_BG, 0x60U, 0x20U);
    arg1->unk_68 = (u8)(arg1->unk_68 + 1);
    if (((u32)(arg1->unk_68)) >= 3U) {
        arg1->unk_68 = 0U;
    }
}
