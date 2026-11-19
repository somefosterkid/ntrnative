#include "global.h"

#include "sprite.h"
#include "overlay_96_ov96_02203468work.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
Sprite *ov96_021EA374(void *, s32, s32, s32);

void *ov96_021EB3E4(enum HeapID *, s32, s32, u8, u8);

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, void *);

s32 ov96_021EB5E8(void *);

s32 ov96_021EB630(void *, s32);

s32 ov96_022038A0(Ov96_02203468Work *, u16);
#else
Sprite *ov96_021EA374(s32, s32, s32, s32);

s32 ov96_021EB3E4(enum HeapID *, s32, s32, s32, s32);

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(s32, s32);

s32 ov96_021EB588(s32, void *);

s32 ov96_021EB5E8(void *);

s32 ov96_021EB630(void *, s32);

s32 ov96_022038A0(void *, s32);
#endif

typedef struct {
    VecFx32 pos[3];
} UnkStruct_ov96_02201EF0;

extern UnkStruct_ov96_02201EF0 ov96_0221C910;


#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_02201EF0(Ov96_02203468Work *arg0, void *arg1, s32 arg2);

void ov96_02201EF0(Ov96_02203468Work *arg0, void *arg1, s32 arg2) {
    s32 sp4;
    s32 sp10;
    s32 *spC;
    s32 sp8;
    s32 sp50[3];
    s32 sp44[3];
    s32 sp38[3];
    s32 sp14[9];
    Sprite *temp_r0_2;
    void *temp_r0_3;
    s32 temp_r0_6;
    void *temp_r0_7;
    void *temp_r0_9;
    s32 var_r5_7;
    s32 var_r7_3;
    s32 var_r7_4;
    s32 var_r7_2;
    s32 temp_r0_5;
    u8 var_r7_5;
    Ov96_02203468Work *var_r6;
    void **var_r5_6;

    var_r6 = arg0;
    sp4 = arg2;
    for (sp10 = 0; sp10 < 2; sp10++) {
        temp_r0_2 = ov96_021EA374(var_r6->unk_5D4, ov96_021EB5E8(arg1), 3, var_r6->heapID);
        var_r6->sprites[sp10] = temp_r0_2;
        Sprite_SetDrawFlag(temp_r0_2, 1);
        sp50[0] = ((sp10 * 16) << 0xC) + (6 << 14);
        sp50[1] = 0x8000;
        sp50[2] = 0;
        Sprite_SetMatrix(var_r6->sprites[sp10], (VecFx32 *)sp50);
        Sprite_SetDrawPriority(var_r6->sprites[sp10], 2U);
    }
    ov96_022038A0(var_r6, 0x3C);
    var_r7_2 = 0;
    do {
        var_r6->balls[var_r7_2].index = var_r7_2;
        var_r6->balls[var_r7_2].unk_0 = ov96_021EB3E4(arg1, 3, 3, 0x6A, 3);
        var_r6->balls[var_r7_2].unk_4 = ov96_021EB3E4(arg1, 3, 3, 0x6A, 4);
        var_r6->balls[var_r7_2].unk_8 = ov96_021EB3E4(arg1, 3, 3, 0x6A, 6);
        ov96_021EB564(var_r6->balls[var_r7_2].unk_4, 1);
        ov96_021EB630(var_r6->balls[var_r7_2].unk_0, 0x14);
        ov96_021EB630(var_r6->balls[var_r7_2].unk_4, 0x3E8);
        ov96_021EB630(var_r6->balls[var_r7_2].unk_8, 5);
        var_r7_2 += 1;
    } while ((s32)var_r7_2 < 0xC);
    var_r7_3 = 0;
    do {
        var_r6->items[var_r7_3].unk_0 = ov96_021EB3E4(arg1, 3, 3, 0x6A, 9);
        ov96_021EB564(var_r6->items[var_r7_3].unk_0, 2);
        sp44[0] = 0x1000;
        sp44[1] = 0x1000;
        sp44[2] = 0;
        ov96_021EB588(var_r6->items[var_r7_3].unk_0, sp44);
        ov96_021EB630(var_r6->items[var_r7_3].unk_0, 4);
        var_r7_3 += 1;
    } while (var_r7_3 < 0xC);
    var_r7_4 = 0;
    do {
        temp_r0_3 = ov96_021EB3E4(arg1, 2, 1, 0x69, 0xB);
        var_r6->unk_64[var_r7_4] = temp_r0_3;
        ov96_021EB564(temp_r0_3, 6);
        ov96_021EB630(var_r6->unk_64[var_r7_4], 2);
        var_r7_4 += 1;
    } while (var_r7_4 < 3);
    var_r6->unk_4C = ov96_021EB3E4(arg1, 3, 1, 0x69, 7);
    sp38[0] = 0x8000;
    sp38[1] = 0x8000;
    sp38[2] = 0;
    ov96_021EB588(var_r6->unk_4C, sp38);
    ov96_021EB564(var_r6->unk_4C, 5);
    ov96_021EB52C(var_r6->unk_4C, 1, 1);
    ov96_021EB630(var_r6->unk_4C, 2);
    *(UnkStruct_ov96_02201EF0 *)sp14 = ov96_0221C910;
    var_r5_6 = var_r6->unk_58;
    temp_r0_5 = sp4 + 1;
    var_r7_5 = (temp_r0_5 % 4);
    sp8 = 0;
    spC = sp14;
    do {
        temp_r0_7 = ov96_021EB3E4(arg1, 3, 1, 0x69, 8);
        *var_r5_6 = temp_r0_7;
        ov96_021EB588(temp_r0_7, spC);
        ov96_021EB564(*var_r5_6, var_r7_5 + 1);
        ov96_021EB52C(*var_r5_6, 1, 1);
        var_r7_5 = ((s32)var_r7_5 + 1) % 4;
        ov96_021EB630(*var_r5_6, 3);
        var_r5_6 += 1;
        spC += 3;
        temp_r0_6 = sp8 + 1;
        sp8 = temp_r0_6;
    } while (temp_r0_6 < 3);
    var_r5_7 = 0;
    do {
        temp_r0_9 = ov96_021EB3E4(arg1, 3, 1, 0x69, 0xA);
        var_r6->unk_70[var_r5_7] = temp_r0_9;
        ov96_021EB564(temp_r0_9, 0);
        ov96_021EB630(var_r6->unk_70[var_r5_7], 4);
        var_r5_7 += 1;
    } while (var_r5_7 < 9);
}
#else
void ov96_02201EF0(void *arg0, void *arg1, s32 arg2);

void ov96_02201EF0(void *arg0, void *arg1, s32 arg2) {
    s32 sp4;
    s32 sp10;
    s32 *spC;
    s32 sp8;
    s32 sp50[3];
    s32 sp44[3];
    s32 sp38[3];
    s32 sp14[9];
    Sprite *temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r0_9;
    s32 var_r5_7;
    s32 var_r7_3;
    s32 var_r7_4;
    s32 var_r7_2;
    s32 temp_r0_5;
    u8 var_r7_5;
    void *var_r5_2;
    void *var_r5_3;
    u32 *var_r5_4;
    u32 *var_r5_6;
    void *var_r6;

    var_r6 = arg0;
    sp4 = arg2;
    for (sp10 = 0; sp10 < 2; sp10++) {
        temp_r0_2 = ov96_021EA374(*(s32 *)((u8 *)var_r6 + 0x5D4), ov96_021EB5E8(arg1), 3, *(s32 *)((u8 *)var_r6 + 0x44));
        *(Sprite **)((u8 *)var_r6 + sp10 * 4 + 0x50) = temp_r0_2;
        Sprite_SetDrawFlag(temp_r0_2, 1);
        sp50[0] = ((sp10 * 16) << 0xC) + (6 << 14);
        sp50[1] = 0x8000;
        sp50[2] = 0;
        Sprite_SetMatrix(*(Sprite **)((u8 *)var_r6 + sp10 * 4 + 0x50), (VecFx32 *)sp50);
        Sprite_SetDrawPriority(*(Sprite **)((u8 *)var_r6 + sp10 * 4 + 0x50), 2U);
    }
    ov96_022038A0(var_r6, 0x3C);
    var_r7_2 = 0;
    var_r5_2 = var_r6;
    do {
        *(s8 *)((u8 *)var_r5_2 + 0xF9) = var_r7_2;
        *(s32 *)((u8 *)var_r5_2 + 0xB8) = ov96_021EB3E4(arg1, 3, 3, 0x6A, 3);
        *(s32 *)((u8 *)var_r5_2 + 0xBC) = ov96_021EB3E4(arg1, 3, 3, 0x6A, 4);
        *(s32 *)((u8 *)var_r5_2 + 0xC0) = ov96_021EB3E4(arg1, 3, 3, 0x6A, 6);
        ov96_021EB564(*(s32 *)((u8 *)var_r5_2 + 0xBC), 1);
        ov96_021EB630(*(void **)((u8 *)var_r5_2 + 0xB8), 0x14);
        ov96_021EB630(*(void **)((u8 *)var_r5_2 + 0xBC), 0x3E8);
        ov96_021EB630(*(void **)((u8 *)var_r5_2 + 0xC0), 5);
        var_r7_2 += 1;
        var_r5_2 += 0x48;
    } while ((s32)var_r7_2 < 0xC);
    var_r7_3 = 0;
    var_r5_3 = var_r6;
    do {
        *(s32 *)((u8 *)var_r5_3 + 0x418) = ov96_021EB3E4(arg1, 3, 3, 0x6A, 9);
        ov96_021EB564(*(s32 *)((u8 *)var_r5_3 + 0x418), 2);
        sp44[0] = 0x1000;
        sp44[1] = 0x1000;
        sp44[2] = 0;
        ov96_021EB588(*(s32 *)((u8 *)var_r5_3 + 0x418), sp44);
        ov96_021EB630(*(void **)((u8 *)var_r5_3 + 0x418), 4);
        var_r7_3 += 1;
        var_r5_3 += 0x20;
    } while (var_r7_3 < 0xC);
    var_r7_4 = 0;
    var_r5_4 = (u32 *)var_r6;
    do {
        temp_r0_3 = ov96_021EB3E4(arg1, 2, 1, 0x69, 0xB);
        *(s32 *)((u8 *)var_r5_4 + offsetof(Ov96_02203468Work, unk_64)) = temp_r0_3;
        ov96_021EB564(temp_r0_3, 6);
        ov96_021EB630(*(void **)((u8 *)var_r5_4 + offsetof(Ov96_02203468Work, unk_64)), 2);
        var_r7_4 += 1;
        var_r5_4 += 1;
    } while (var_r7_4 < 3);
    *(s32 *)((u8 *)var_r6 + 0x4C) = ov96_021EB3E4(arg1, 3, 1, 0x69, 7);
    sp38[0] = 0x8000;
    sp38[1] = 0x8000;
    sp38[2] = 0;
    ov96_021EB588(*(s32 *)((u8 *)var_r6 + 0x4C), sp38);
    ov96_021EB564(*(s32 *)((u8 *)var_r6 + 0x4C), 5);
    ov96_021EB52C(*(void **)((u8 *)var_r6 + 0x4C), 1, 1);
    ov96_021EB630(*(void **)((u8 *)var_r6 + 0x4C), 2);
    *(UnkStruct_ov96_02201EF0 *)sp14 = ov96_0221C910;
    var_r5_6 = (u32 *)var_r6;
    temp_r0_5 = sp4 + 1;
    var_r7_5 = (temp_r0_5 % 4);
    sp8 = 0;
    spC = sp14;
    do {
        temp_r0_7 = ov96_021EB3E4(arg1, 3, 1, 0x69, 8);
        *(s32 *)((u8 *)var_r5_6 + offsetof(Ov96_02203468Work, unk_58)) = temp_r0_7;
        ov96_021EB588(temp_r0_7, spC);
        ov96_021EB564(*(s32 *)((u8 *)var_r5_6 + offsetof(Ov96_02203468Work, unk_58)), var_r7_5 + 1);
        ov96_021EB52C(*(void **)((u8 *)var_r5_6 + offsetof(Ov96_02203468Work, unk_58)), 1, 1);
        var_r7_5 = ((s32)var_r7_5 + 1) % 4;
        ov96_021EB630(*(void **)((u8 *)var_r5_6 + offsetof(Ov96_02203468Work, unk_58)), 3);
        var_r5_6 += 1;
        spC += 3;
        temp_r0_6 = sp8 + 1;
        sp8 = temp_r0_6;
    } while (temp_r0_6 < 3);
    var_r5_7 = 0;
    do {
        temp_r0_9 = ov96_021EB3E4(arg1, 3, 1, 0x69, 0xA);
        *(s32 *)((u8 *)var_r6 + 0x70) = temp_r0_9;
        ov96_021EB564(temp_r0_9, 0);
        ov96_021EB630(*(void **)((u8 *)var_r6 + 0x70), 4);
        var_r5_7 += 1;
        var_r6 += 4;
    } while (var_r5_7 < 9);
}
#endif
