#include "global.h"

#include "heap.h"
#include "sprite.h"
#include "overlay_97_course_sub_screen.h"

Sprite *ov97_0221FAEC(Ov97CourseSubScreenSprites *, s32, s32, s32, s32, s32, u32, s32);

s32 ov97_0221FBA0(Ov97CourseSubScreenSprites *);

extern u8 ov97_0221FCF4;

void ov97_0221F9E0(Ov97CourseSubScreenSprites *arg0);

void ov97_0221F9E0(Ov97CourseSubScreenSprites *arg0) {
    s32 sp10;
    s32 temp_r6;
    s32 temp_r6_2;
    u32 var_r4;
    u8 temp_r0;
    u8 var_r4_2;
    var_r4 = 0;
    do {
        temp_r6 = (var_r4 << 5) + 0x68;
        arg0->unk_1B4[var_r4] = ov97_0221FAEC(arg0, 1, 2, 0, temp_r6, 0xA8, var_r4, 0);
        arg0->unk_1C8[var_r4] = ov97_0221FAEC(arg0, 1, 1, 0, temp_r6, 0xA8, 5U, 0);
        var_r4 = (u32)((u8)(var_r4 + 1));
    } while (var_r4 < 5U);
    sp10 = 0;
    do {
        temp_r6_2 = ((*((u8 *)((&ov97_0221FCF4) + sp10))) * 0x10) + 0x30;
        arg0->statRows[sp10].arrow = ov97_0221FAEC(arg0, 2, 1, 0, 0x60, temp_r6_2, 4U, 0);
        var_r4_2 = 0;
loop_4:
        arg0->statRows[sp10].marks[var_r4_2] = ov97_0221FAEC(arg0, 2, 2, 0, (var_r4_2 * 0x10) + 0xA8, temp_r6_2, 3U, 0);

        var_r4_2 += 1;
        if (((u32)var_r4_2) < 5U) {
            goto loop_4;
        }
        temp_r0 = sp10 + 1;
        sp10 = (s32)temp_r0;
    } while (((u32)temp_r0) < 5U);
    arg0->monSprite = ov97_0221FAEC(arg0, 3, 0, 0, 0x30, 0x48, 0U, 0);
    arg0->charData = Heap_Alloc(arg0->heapID, 0xC80U);
    ov97_0221FBA0(arg0);
}
