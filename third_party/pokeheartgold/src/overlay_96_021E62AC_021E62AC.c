#include "global.h"

#include "sprite.h"
#include "pokeathlon/pokeathlon.h"

Sprite *ov96_021EA6E4(s32, s32, s32, s32, s32);

void ov96_021E62AC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 arg5, u8 *arg6);

void ov96_021E62AC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 arg5, u8 *arg6) {
    VecFx32 sp10;
    s32 spC;
    s32 sp8;
    s32 sp4;
    s32 temp_r6;
    u8 var_r4;
    void *temp_r5;

    sp4 = arg1;
    sp8 = arg2;
    spC = arg3;
    ((PokeathlonCourseData *)arg0)->unk_728 = (s8)arg5;
    for (var_r4 = 0; (u16)var_r4 < arg5; var_r4 += 1) {
        temp_r6 = var_r4 * 4;
        temp_r5 = arg0 + temp_r6;
        *(Sprite **)((u8 *)temp_r5 + 0x71C) = ov96_021EA6E4(sp8, spC, arg4, 0, *((s32 *)&((PokeathlonCourseData *)arg0)->heapId));
        (*(s32 *)((u8 *)&sp10 + 8)) = 0;
        (*(s32 *)&sp10) = *(u16 *)(arg6 + temp_r6) << 0xC;
        (*(s32 *)((u8 *)&sp10 + 4)) = (sp4 + *(u16 *)((u8 *)(arg6 + temp_r6) + 2)) << 0xC;
        Sprite_SetMatrix(*(Sprite **)((u8 *)temp_r5 + 0x71C), (VecFx32 *)&sp10);
    }
    if ((u16)var_r4 < 3U) {
        do {
            *(s32 *)((u8 *)(arg0 + (var_r4 * 4)) + 0x71C) = 0;
            var_r4 += 1;
        } while ((u16)var_r4 < 3U);
    }
    ((PokeathlonCourseData *)arg0)->unk_729 = 0;
}
