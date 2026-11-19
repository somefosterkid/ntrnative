#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02019BA4.h"
#include "overlay_99_pokeathlon_medals.h"

void ov99_021E77D8(void *arg0);

s32 ov99_021E7450(void *);

s32 ov99_021E7508(void *);

s32 ov99_021E7794(void *);

s32 ov99_021E7598(void *arg0);

s32 ov99_021E7598(void *arg0) {
    s16 sp4[2];
    s32 sp0;
    s32 temp_r3;
    s32 temp_r4;
    s32 var_r0;
    s32 var_r6;
    s32 var_r6_2;
    u8 temp_r1;
    s32 temp_r7;

    if (((u32)(*((s32 *)&((PokeathlonMedals *)arg0)->flags) * 0x10) >> 0x1F) != 0) {
        var_r0 = 1;
    } else {
        var_r0 = -1;
    }
    sp0 = var_r0 << 6;
    temp_r1 = (u8) *((s32 *)&((PokeathlonMedals *)arg0)->scroll);
    if (temp_r1 == 0) {
        temp_r4 = GridInputHandler_GetNextInput(((PokeathlonMedals *)arg0)->grid);
        ov99_021E7508(arg0);
        if (temp_r4 == 0x1E) {
            ov99_021E77D8(arg0);
        } else {
            ov99_021E7794(arg0);
        }
    } else if (temp_r1 == 4) {
        ov99_021E7450(arg0);
        var_r6 = 0;
        do {
            temp_r7 = (u8) *((s32 *)&((PokeathlonMedals *)arg0)->scroll) * sp0;
            ManagedSprite_GetPositionXY(((PokeathlonMedals *)arg0)->sprites[0x14 + var_r6], &sp4[1], &sp4[0]);
            ManagedSprite_SetPositionXY(((PokeathlonMedals *)arg0)->sprites[0x14 + var_r6], (s16)(sp4[1] - (temp_r7 * 2)), sp4[0]);
            var_r6 += 1;
        } while (var_r6 < 0x1E);
    }
    if ((u32)(u8) *((s32 *)&((PokeathlonMedals *)arg0)->scroll) < 8U) {
        temp_r4 = 0 - sp0;
        ScheduleSetBgPosText(((PokeathlonMedals *)arg0)->bgConfig, 2U, BG_POS_OP_ADD_ROT, temp_r4);
        ScheduleSetBgPosText(((PokeathlonMedals *)arg0)->bgConfig, 3U, BG_POS_OP_ADD_ROT, temp_r4);
        var_r6_2 = 0;
        do {
            ManagedSprite_OffsetPositionXY(((PokeathlonMedals *)arg0)->sprites[0x14 + var_r6_2], (s16)sp0, 0);
            var_r6_2 += 1;
        } while (var_r6_2 < 0x1E);
    } else {
        ScheduleSetBgPosText(((PokeathlonMedals *)arg0)->bgConfig, 2U, BG_POS_OP_SET_ROT, 0);
        ScheduleSetBgPosText(((PokeathlonMedals *)arg0)->bgConfig, 3U, BG_POS_OP_SET_ROT, 0);
        *((s32 *)&((PokeathlonMedals *)arg0)->scroll) = (s32)(*((s32 *)&((PokeathlonMedals *)arg0)->scroll) & ~0xFF);
        ManagedSprite_SetDrawFlag(((PokeathlonMedals *)arg0)->sprites[0], 1);
        return 1;
    }
    temp_r3 = *((s32 *)&((PokeathlonMedals *)arg0)->scroll);
    *((s32 *)&((PokeathlonMedals *)arg0)->scroll) = (s32)((temp_r3 & ~0xFF) | (u8)((u8)temp_r3 + 1));
    return 0;
}
