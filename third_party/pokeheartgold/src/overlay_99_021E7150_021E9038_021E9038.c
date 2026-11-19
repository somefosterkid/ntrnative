#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_99_pokeathlon_event_record.h"

s32 ov99_021E8518(void *, u32, s32);

s32 ov99_021E86D4(void *, u8, s16, s32, s32, s32);

void ov99_021E9038(void *arg0);

void ov99_021E9038(void *arg0) {
    u32 sp18;
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    s32 temp_r0_2;
    s32 var_r1;
    s32 var_r6;
    int new_var;
    s32 var_r7;
    u32 temp_r0;
    u32 temp_r0_3;
    u8 temp_r4;
    sp18 = 0;
    sp10 = 0;
    spC = 6;
    do {
        sp14 = 0;
        var_r7 = 1;
        sp8 = (s32)((s16)((u16)((long long)spC)));
loop_2:
        temp_r4 = sp14 + sp10;

        temp_r0_2 = *((s32 *)&((PokeathlonEventRecord *)arg0)->linkMode);
        if (temp_r0_2 != 0) {
            var_r6 = 1;
        } else if ((*((s32 *)&((PokeathlonEventRecord *)arg0)->noRecord[temp_r4])) == 0) {
            var_r6 = 1;
        } else {
            var_r6 = 0;
        }
        new_var = temp_r4 + 0xC;
        if (temp_r0_2 != 0) {
            var_r1 = 0;
        } else {
            var_r1 = var_r6;
        }
        ManagedSprite_SetDrawFlag(((PokeathlonEventRecord *)arg0)->sprites[new_var], var_r1);
        ManagedSprite_SetDrawFlag(((PokeathlonEventRecord *)arg0)->sprites[temp_r4 + 2], ov99_021E8518(arg0, (s8)temp_r4, 1));
        ov99_021E86D4(arg0, temp_r4, (u16)var_r7, sp8, var_r6, 0);
        var_r7 += 6;
        temp_r0_3 = sp14 + 1;
        sp14 = temp_r0_3;
        if (temp_r0_3 < 5U) {
            goto loop_2;
        }
        sp10 += 5;
        spC += 8;
        temp_r0 = sp18 + 1;
        sp18 = temp_r0;
    } while (temp_r0 < 2U);
}
