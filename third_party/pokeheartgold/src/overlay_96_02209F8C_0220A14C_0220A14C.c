#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_0220_data.h"

s32 ov96_0220A254(PokeathlonCourseData *, s32, s32);

typedef struct PAF {
    u32 lo : 24;
    u32 b : 2;
    u32 c : 1;
    u32 flag : 1;
    u32 rest : 4;
} PAF;

void ov96_0220A14C(PokeathlonCourseData *arg0);

void ov96_0220A14C(PokeathlonCourseData *arg0) {
    s32 sp8;
    PAF *sp4;
    PokeathlonCourseData *sp0;
    s32 temp_r0;
    s32 var_r4;
    u32 temp_r2;
    u32 var_r6;
    s32 var_r5;
    Ov96_0220DataLane *var_r7;
    Ov96_0220Data *heap;

    sp0 = arg0;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    heap = PokeathlonCourse_GetHeapAllocPtr4(sp0);
    sp4 = (PAF *)ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(sp0) + 0xF0);
    sp8 = 0;
    var_r7 = heap->lanes;
    do {
        var_r6 = (u8) * (u32 *)sp4 / 5U;
        if ((s32)var_r6 > 0x14) {
            if (ManagedSprite_GetDrawFlag(var_r7->unk_5C[0]) == 0) {
                var_r4 = 0;
                do {
                    ManagedSprite_SetDrawFlag(var_r7->unk_5C[var_r4], 1);
                    var_r4 += 1;
                } while (var_r4 < 5);
                var_r4 = 0;
                do {
                    ManagedSprite_SetDrawFlag(var_r7->unk_C[var_r4], 0);
                    var_r4 += 1;
                } while (var_r4 < 0x14);
            }
            var_r6 -= 0x14;
        }
        if ((s32)var_r6 > 0x14) {
            var_r6 = 0x14;
        }
        var_r5 = 0;
        if ((s32)var_r6 > 0) {
            do {
                if (ManagedSprite_GetDrawFlag(var_r7->unk_C[var_r5]) == 0) {
                    ManagedSprite_ResetSpriteAnimCtrlState(var_r7->unk_C[var_r5]);
                    ManagedSprite_SetDrawFlag(var_r7->unk_C[var_r5], 1);
                }
                var_r5 += 1;
            } while (var_r5 < (s32)var_r6);
        }
        if (sp4->flag) {
            temp_r2 = sp4->b;
            if (temp_r2 != var_r7->unk_0) {
                ov96_0220A254(sp0, (u8)sp8, (u8)temp_r2);
                var_r7->unk_0 = sp4->b;
            }
            ManagedSprite_SetDrawFlag(var_r7->unk_4, 0);
            ManagedSprite_SetDrawFlag(var_r7->unk_8, 1);
        } else {
            ManagedSprite_SetDrawFlag(var_r7->unk_8, 0);
            ManagedSprite_SetDrawFlag(var_r7->unk_4, 1);
        }
        var_r7++;
        sp4 += 1;
        temp_r0 = sp8 + 1;
        sp8 = temp_r0;
    } while (temp_r0 < 4);
}
