#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"

void *ov96_021E9370(void *, u32);

void *ov96_021E94EC(void *, u32);

void ov96_021E7658(PokeathlonCourseData *arg0, s32 arg1);

void ov96_021E7658(PokeathlonCourseData *arg0, s32 arg1) {
    s32 sp0;
    u8 temp_r7;
    u8 var_r4;
    u8 var_r4_2;
    void *new_var;
    void *temp_r0;
    void *temp_r0_2;
    void *temp_r2;
    void *temp_r2_2;
    sp0 = arg1;
    temp_r7 = (u8)ov96_021E5F24(arg0);
    var_r4 = 0;
    do {
        temp_r0 = ov96_021E9370(arg0->graphicsSystem, var_r4);
        temp_r2 = (((u8 *)arg0) + (sp0 * 2)) + (var_r4 * 8);
        *((u16 *)(((u8 *)temp_r2) + 0x8B4)) = (u16)(*((u16 *)(((u8 *)temp_r0) + 0xA)));
        *((s16 *)(((u8 *)temp_r2) + 0x8D4)) = (s16)(*((s32 *)(((u8 *)temp_r0) + 0xC)));
        var_r4 += 1;
    } while (((u32)var_r4) < 4U);
    var_r4_2 = 0;
loop_3:
    temp_r0_2 = ov96_021E94EC(arg0->graphicsSystem, var_r4_2);

    if ((*((s32 *)temp_r0_2)) != temp_r7) {
        var_r4_2 += 1;
        if (((u32)var_r4_2) < 4U) {
            goto loop_3;
        }
    }
    if (var_r4_2 == 4) {
        GF_AssertFail();
        return;
    }
    if ((*((u8 *)(((u8 *)temp_r0_2) + 9))) == 0) {
        temp_r2_2 = ((u8 *)arg0) + sp0;
        *((s8 *)(((u8 *)temp_r2_2) + 0x8F4)) = 1;
        *((s8 *)(((u8 *)temp_r2_2) + 0x8F8)) = 0;
        return;
    }
    temp_r2 = (u32 *)arg0;
    *((s8 *)(((u8 *)(temp_r2 + sp0)) + 0x8F4)) = 0;
    new_var = ov96_021E94EC(arg0->graphicsSystem, 3U);
    if ((*((u8 *)(((u8 *)new_var) + 9))) == (*((u8 *)(((u8 *)temp_r0_2) + 9)))) {
        *((s8 *)(((u8 *)(((u8 *)arg0) + sp0)) + 0x8F8)) = 1;
        return;
    }
    *((s8 *)(((u8 *)(((u8 *)arg0) + sp0)) + 0x8F8)) = 0;
}
