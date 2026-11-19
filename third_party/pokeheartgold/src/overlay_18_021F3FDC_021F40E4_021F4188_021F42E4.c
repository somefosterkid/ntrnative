#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F42E4(PokedexAppData *arg0, u32 arg1, s16 *arg2, s16 *arg3, s8 *arg4);

void ov18_021F42E4(PokedexAppData *arg0, u32 arg1, s16 *arg2, s16 *arg3, s8 *arg4) {
    s16 *var_r4;
    s16 *var_r6;
    s32 temp_r1;
    s8 *var_r5;
    u32 temp_r0;
    u8 var_r7;

    var_r6 = arg2;
    temp_r1 = arg1 * 4;
    var_r7 = *(u8 *)(*((u8 **)&arg0->unk_1908) + temp_r1);
    var_r4 = arg3;
    var_r5 = arg4;
    *var_r6 = ((var_r7 - (0x16 * arg0->unk_18C8)) * 8) + 0x44;
    *var_r4 = (*(u8 *)((u8 *)(*((u8 **)&arg0->unk_1908) + temp_r1) + 1) * 8) + 0x2C;
    temp_r0 = ov18_021E8B18(arg1);
    if ((temp_r0 == 0x7C) || ((u32)(temp_r0 - 0xB2) <= 1U)) {
        *var_r5 = 6;
        *var_r4 = *var_r4 + 4;
        return;
    }
    if ((temp_r0 == 0x60) || (temp_r0 == 0x1E7)) {
        *var_r5 = 0;
        *var_r6 = *var_r6 + 4;
        *var_r4 = *var_r4 + 4;
        return;
    }
    if ((temp_r0 == 0x71) || (temp_r0 == 0x13B) || ((u32)(temp_r0 - 0x1EA) <= 2U)) {
        *var_r5 = 6;
        *var_r4 = *var_r4 + 4;
        return;
    }
    *var_r5 = 0;
}
