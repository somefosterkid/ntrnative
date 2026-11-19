#include "global.h"

#include "palette.h"
#include "sys_task.h"

typedef struct UnkStruct_ov80_02239FA8_arg1 {
    u8 unk_0[0x8];
    u8 unk_8;
    u8 unk_9[0x60];
    u8 unk_69;
} UnkStruct_ov80_02239FA8_arg1;

void ov80_02239FA8(SysTask *task, void *param);

void ov80_02239FA8(SysTask *task, void *param) {
    UnkStruct_ov80_02239FA8_arg1 *arg1 = param;
    u8 *new_var;
    if (((u32)(arg1->unk_69)) < 8U) {
        new_var = (u8 *)(((u8 *)arg1) + 0x69);
        *new_var = (u8)((*new_var) + 1);
        return;
    }
    new_var = (u8 *)arg1;
    *((u8 *)(new_var + 0x69)) = 0U;
    PaletteData_LoadPalette(*((PaletteData **)(new_var + 4)), ((void *)&arg1->unk_8) + (((*((u8 *)(new_var + 0x68))) << 4) << 1), PLTTBUF_MAIN_BG, 0x60U, 0x20U);
    *((u8 *)(new_var + 0x68)) = (u8)((*((u8 *)(new_var + 0x68))) + 1);
    if (((u32)(*((u8 *)(new_var + 0x68)))) >= 3U) {
        *((u8 *)(new_var + 0x68)) = 0U;
    }
}
