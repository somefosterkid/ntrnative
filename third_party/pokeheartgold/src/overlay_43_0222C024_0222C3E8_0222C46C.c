#include "global.h"

#include "bg_window.h"

typedef struct UnkStruct_ov43_0222C46C_arg1 {
    BgConfig *unk_0;
    u8 unk_4[0x208];
    u8 *unk_20C;
} UnkStruct_ov43_0222C46C_arg1;

void ov43_0222C46C(s32 arg0, UnkStruct_ov43_0222C46C_arg1 *arg1, s32 arg2, s32 arg3);

void ov43_0222C46C(s32 arg0, UnkStruct_ov43_0222C46C_arg1 *arg1, s32 arg2, s32 arg3) {
    u32 temp_r2;
    u8 *temp_r0;
    temp_r0 = arg1->unk_20C;
    temp_r2 = (4 * arg3) + 0x10;
    CopyToBgTilemapRect(arg1->unk_0, 0U, temp_r2, 0U, 4U, 4U, temp_r0 + 0xC, temp_r2, 8U, (u8)((u32)(*(u16 *)temp_r0) >> 3), (u8)((u32)(*(u16 *)(temp_r0 + 2)) >> 3));
    ScheduleBgTilemapBufferTransfer(arg1->unk_0, 0U);
}

void ov43_0222C4B8(u32 param_1, void **param_2, s8 param_3);

void ov43_0222C4B8(u32 param_1, void **param_2, s8 param_3) {
    u16 *puVar1;
    puVar1 = (u16 *)param_2[0x83];
    CopyToBgTilemapRect((BgConfig *)(*param_2), 0, 0x10, 0, 0x10, 4, puVar1 + 6, 0, 4, (u8)((*puVar1) >> 3), (u8)((puVar1[1]) >> 3));
    puVar1 = (u16 *)param_2[0x83];
    CopyToBgTilemapRect((BgConfig *)(*param_2), 0, (param_3 * 4) + 0x10, 0, 4, 4, puVar1 + 6, param_3 << 2, 8, (u8)((*puVar1) >> 3), (u8)((puVar1[1] & ((char)0x7ff)) >> 3));
    ScheduleBgTilemapBufferTransfer((BgConfig *)(*param_2), 0);
    return;
}
