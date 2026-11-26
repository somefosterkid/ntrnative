#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_99_pokeathlon_event_record.h"

s32 ov99_021E8FEC(void *, s32);

extern TouchscreenHitbox ov99_021EA3C4;

s32 ov99_021E90F4(void *arg0);

s32 ov99_021E90F4(void *arg0) {
    s32 temp_r0;
    s32 var_r1;
    s32 var_r2_2;
    s8 temp_r0_2;
    s8 temp_r1;
    s8 temp_r4;
    s8 var_r1_2;
    s8 var_r2;
    u8 *sum;

    temp_r4 = ((PokeathlonEventRecord *)arg0)->cursor;
    if (gSystem.touchNew != 0) {
        temp_r0 = TouchscreenHitbox_FindHitboxAtTouchNew(&ov99_021EA3C4);
        if (temp_r0 != -1) {
            ((PokeathlonEventRecord *)arg0)->cursor = (s8)(temp_r0 - 1);
            ManagedSprite_SetAnim(((PokeathlonEventRecord *)arg0)->sprites[0], 0xB);
            PlaySE(0x5DCU);
            if (((PokeathlonEventRecord *)arg0)->cursor == -1) {
                *((s32 *)&((PokeathlonEventRecord *)arg0)->modeSwitchRequested) = 1;
                ManagedSprite_SetAnim(((PokeathlonEventRecord *)arg0)->sprites[0], 0xD);
                PlaySE(0x5DCU);
            }
            ManagedSprite_SetAnimateFlag(((PokeathlonEventRecord *)arg0)->sprites[0], 1);
        }
        goto block_35;
    }
    if (gSystem.newKeys & 1) {
        if (temp_r4 == 0xA) {
            return 1;
        }
        if (temp_r4 == -1) {
            *((s32 *)&((PokeathlonEventRecord *)arg0)->modeSwitchRequested) = 1;
            PlaySE(0x5DCU);
        }
        goto block_11;
    }
block_11:
    temp_r1 = ((PokeathlonEventRecord *)arg0)->cursor;
    if (temp_r1 == -1) {
        if (0x80 & gSystem.newAndRepeatedKeysRaw) {
            ((PokeathlonEventRecord *)arg0)->cursor = (s8) ((PokeathlonEventRecord *)arg0)->lastColumn;
        }
    } else if (temp_r1 == 0xA) {
        if (0x40 & gSystem.newAndRepeatedKeysRaw) {
            ((PokeathlonEventRecord *)arg0)->cursor = (s8)(((PokeathlonEventRecord *)arg0)->lastColumn + 5);
        }
    } else {
        if (0x10 & gSystem.newAndRepeatedKeysRaw) {
            ((PokeathlonEventRecord *)arg0)->cursor = (s8)(temp_r1 + 1);
        } else if (0x20 & gSystem.newAndRepeatedKeysRaw) {
            ((PokeathlonEventRecord *)arg0)->cursor = (s8)(temp_r1 - 1);
        } else if (0x80 & gSystem.newAndRepeatedKeysRaw) {
            ((PokeathlonEventRecord *)arg0)->cursor = (s8)(temp_r1 + 5);
        } else if (gSystem.newAndRepeatedKeysRaw & 0x40) {
            if (temp_r1 == 0xA) {
                var_r1 = 1;
            } else {
                var_r1 = 5;
            }
            sum = (u8 *)&((PokeathlonEventRecord *)arg0)->cursor;
            *(s8 *)(sum) = (s8)(*(s8 *)(sum)-var_r1);
        }
        var_r2 = ((PokeathlonEventRecord *)arg0)->cursor;
        if ((s32)var_r2 > 0xA) {
            var_r2 = 0xA;
        } else if ((s32)var_r2 < -1) {
            var_r2 = -1;
        }
        ((PokeathlonEventRecord *)arg0)->cursor = var_r2;
        temp_r0_2 = ((PokeathlonEventRecord *)arg0)->cursor;
        if ((temp_r0_2 != -1) && (temp_r0_2 != 0xA)) {
            ((PokeathlonEventRecord *)arg0)->lastColumn = (s8)(temp_r0_2 % 5);
        }
    }
block_35:
    if (temp_r4 != ((PokeathlonEventRecord *)arg0)->cursor) {
        PlaySE(0x5DCU);
        var_r1_2 = ((PokeathlonEventRecord *)arg0)->cursor;
        if (var_r1_2 == -1) {
            ManagedSprite_SetPositionXY(((PokeathlonEventRecord *)arg0)->sprites[0], 0xE0, 0x10);
            ManagedSprite_SetAnim(((PokeathlonEventRecord *)arg0)->sprites[0], 0xC);
        } else if (var_r1_2 == 0xA) {
            ManagedSprite_SetPositionXY(((PokeathlonEventRecord *)arg0)->sprites[0], 0xE0, 0xB0);
            ManagedSprite_SetAnim(((PokeathlonEventRecord *)arg0)->sprites[0], 0xC);
        } else {
            if ((s32)var_r1_2 >= 5) {
                var_r2_2 = 1;
            } else {
                var_r2_2 = 0;
            }
            if (var_r2_2 != 0) {
                var_r1_2 -= 5;
            }
            ManagedSprite_SetPositionXY(((PokeathlonEventRecord *)arg0)->sprites[0], (s16)((0x30 * var_r1_2) + 0x20), (s16)((var_r2_2 << 6) + 0x48));
            if (ManagedSprite_GetActiveAnim(((PokeathlonEventRecord *)arg0)->sprites[0]) != 0xB) {
                ManagedSprite_SetAnim(((PokeathlonEventRecord *)arg0)->sprites[0], 0xA);
            }
            if ((temp_r4 != 0xA) || (((PokeathlonEventRecord *)arg0)->cursor != 9)) {
                ov99_021E8FEC(arg0, 0);
                *((s32 *)&((PokeathlonEventRecord *)arg0)->eventChanged) = 1;
            }
        }
    }
    return 0;
}
