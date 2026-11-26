#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "sprite.h"

#include "overlay_96_ov96_course_hud.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"

Sprite *ov96_021EA2C4(s32, s32, s32, s32);

void *ov96_021EB3E4(void *, s32, s32, u8, u8);

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, s32 *);

s32 ov96_021EB5E8(void *);

extern s32 ov96_0221D648;

extern u8 ov96_0221D64C[];

extern s32 ov96_0221D660;

extern s32 ov96_0221D66C;

void ov96_02214718(void *arg0, Ov96CourseHud *arg1, s32 arg2, s32 arg3) {
    s32 sp14[6];
    s32 sp8[3];
    s32 sp4;
    Sprite *temp_r0_3;
    void *temp_r0;
    void *temp_r0_2;
    u8 var_r4;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    void *temp_r5;
#endif

    sp4 = arg2;
    *(VecFx32 *)(&sp14[3]) = *(VecFx32 *)(&ov96_0221D660);
    *(VecFx32 *)(&sp14[0]) = *(VecFx32 *)(&ov96_0221D66C);
    temp_r0 = ov96_021EB3E4(arg0, 0, 2, 0x66, 9);
    ov96_021EB52C(temp_r0, 1, 1);
    ov96_021EB588(temp_r0, &sp14[3]);
    ov96_021EB564(temp_r0, ov96_021E5F24(arg1->course) + 1);
    temp_r0_2 = ov96_021EB3E4(arg0, 0, 2, 0x66, 0xA);
    ov96_021EB52C(temp_r0_2, 1, 1);
    ov96_021EB588(temp_r0_2, &sp14[0]);
    ov96_021EB564(temp_r0_2, 5);
    arg1->unk_4C = temp_r0_2;
    var_r4 = 0;
    do {
#if defined(SDK_PORT) || defined(NONMATCHING)
        temp_r0_3 = ov96_021EA2C4(sp4, ov96_021EB5E8(arg0), 0, arg1->heapID);
        arg1->digits[var_r4] = temp_r0_3;
        Sprite_SetDrawFlag(temp_r0_3, 1);
        sp8[0] = *(u8 *)(((u8 *)&ov96_0221D648) + var_r4) << 0xC;
        sp8[1] = 0x280000;
        sp8[2] = 0;
        Sprite_SetMatrix(arg1->digits[var_r4], (VecFx32 *)&sp8[0]);
        Sprite_SetAnimCtrlSeq(arg1->digits[var_r4], ov96_0221D64C[var_r4]);
#else
        temp_r5 = (u8 *)arg1 + (var_r4 * 4);
        temp_r0_3 = ov96_021EA2C4(sp4, ov96_021EB5E8(arg0), 0, arg1->heapID);
        *(Sprite **)((u8 *)temp_r5 + 8) = temp_r0_3;
        Sprite_SetDrawFlag(temp_r0_3, 1);
        sp8[0] = *(u8 *)(((u8 *)&ov96_0221D648) + var_r4) << 0xC;
        sp8[1] = 0x280000;
        sp8[2] = 0;
        Sprite_SetMatrix(*(Sprite **)((u8 *)temp_r5 + 8), (VecFx32 *)&sp8[0]);
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)temp_r5 + 8), ov96_0221D64C[var_r4]);
#endif
        var_r4 += 1;
    } while ((u32)var_r4 < 2U);
}
