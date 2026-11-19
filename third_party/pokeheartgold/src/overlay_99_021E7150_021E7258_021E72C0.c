#include "global.h"

#include "error_handling.h"
#include "overlay_99_pokeathlon_medals.h"

u8 ov99_021E7180(void *, s32);

void *ov99_021E728C(void *, u32 *, s32, s32);

void ov99_021E72C0(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov99_021E72C0(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 sp0;
    s32 temp_r1;
    s32 temp_r3;
    s32 var_r6;
    u16 temp_r4;
    u32 temp_r0;
    u32 var_r7;
    u8 *sum;

    *((void **)&((PokeathlonMedals *)arg0)->speciesOrder) = ov99_021E728C(arg0, &sp0, arg2, arg3);
    var_r7 = 0;
    if (var_r7 < sp0) {
        var_r6 = var_r7;
        do {
            temp_r4 = *(u16 *)(((u8 *)((PokeathlonMedals *)arg0)->speciesOrder) + var_r6);
            if ((temp_r4 == 0) || ((u32)temp_r4 > 0x1EDU)) {
                GF_AssertFail();
            }
            if (ov99_021E7180(arg0, temp_r4) != 0) {
                ((PokeathlonMedals *)arg0)->species[((u32)(*((s32 *)&((PokeathlonMedals *)arg0)->flags) << 0x12) >> 0x17)] = temp_r4;
                temp_r1 = *((s32 *)&((PokeathlonMedals *)arg0)->flags);
                *((s32 *)&((PokeathlonMedals *)arg0)->flags) = (s32)((0xFFFFC01F & temp_r1) | ((u32)((((u32)(temp_r1 << 0x12) >> 0x17) + 1) << 0x17) >> 0x12));
            }
            var_r6 += 2;
            var_r7 += 1;
        } while (var_r7 < sp0);
    }
    temp_r0 = (u32)(*((u32 *)&((PokeathlonMedals *)arg0)->flags) << 0x12) >> 0x17;
    if (temp_r0 != 0) {
        sum = (u8 *)&((PokeathlonMedals *)arg0)->flags;
        *(s32 *)(sum) = (s32)((*(s32 *)(sum) & ~0x1F) | ((temp_r0 / 30U) & 0x1F));
        if (((u32)((u32)(*(s32 *)(sum) << 0x12) >> 0x17) % 30U) == 0) {
            temp_r3 = *((s32 *)&((PokeathlonMedals *)arg0)->flags);
            *((s32 *)&((PokeathlonMedals *)arg0)->flags) = (s32)((temp_r3 & ~0x1F) | (0x1F & (((u32)(temp_r3 << 0x1B) >> 0x1B) - 1)));
        }
    }
}
