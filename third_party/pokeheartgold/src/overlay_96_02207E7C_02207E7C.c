#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"
#include "sprite.h"
#include "overlay_96_ov96_sprite_mgr.h"

extern s32 ov96_0221CBCC[];

void ov96_02207E7C(void *arg0, void *arg1);

void ov96_02207E7C(void *arg0, void *arg1) {
    s32 sp8[2];
    s32 *var_r4;
    s32 var_r6;

    var_r4 = sp8;
    sp8[0] = ov96_0221CBCC[2];
    sp8[1] = ov96_0221CBCC[3];
    var_r6 = 0;
    do {
        ov96_021EB2BC(arg1, 0x5D, 9, *var_r4, 2);
        ov96_021EB2F4(arg1, 0x5D, 6, *var_r4, 2, 1);
        ov96_021EB334(arg1, 0x5D, 0xA, *var_r4);
        ov96_021EB36C(arg1, 0x5D, 0xA, *var_r4);
        var_r6 += 1;
        var_r4 += 1;
    } while (var_r6 < 2);
    ov96_021EB2BC(arg1, 0xE2, 0xD, 0x65, 2);
    ov96_021EB2F4(arg1, 0xE2, 0xA, 0x65, 2, 8);
    ov96_021EB334(arg1, 0xE2, 0xC, 0x65);
    ov96_021EB36C(arg1, 0xE2, 0xB, 0x65);
}

Sprite *ov96_021EA2C4(UnkStruct_ov96_021E9A78 *, SpriteList *, s32, enum HeapID);

SpriteList *ov96_021EB5E8(void *);

void ov96_02208914(void *, Ov96HurdleHud *);

extern u8 ov96_0221CBC4[2];

extern u8 ov96_0221CBC8[2];

extern VecFx32 ov96_0221CBF4;

void ov96_02207F18(Ov96HurdleHud *hud, void *arg1, UnkStruct_ov96_021E9A78 *arg2);

void ov96_02207F18(Ov96HurdleHud *hud, void *arg1, UnkStruct_ov96_021E9A78 *arg2) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    VecFx32 pos[2];
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        hud->players[i].unk_0C = ov96_021EB3E4(arg1, 3, 2, 0x65, 2);
        hud->players[i].unk_14 = ov96_021EB3E4(arg1, 3, 2, 0x65, 3);
        hud->players[i].unk_10 = ov96_021EB3E4(arg1, 3, 2, 0x65, 0xA);
        hud->badges[i].unk_8 = ov96_021EB3E4(arg1, 2, 2, 0x65, 0x13);
        ov96_021EB564(hud->badges[i].unk_8, 0);
        ov96_021EB52C(hud->badges[i].unk_8, 1, 1);
        hud->badges[i].unk_4 = ov96_021EB3E4(arg1, 2, 2, 0x65, 0x15);
        ov96_021EB564(hud->badges[i].unk_4, 9);
        ov96_021EB52C(hud->badges[i].unk_4, 1, 1);
        hud->badges[i].unk_0 = ov96_021EB3E4(arg1, 2, 2, 0x65, 0x14);
        ov96_021EB564(hud->players[i].unk_0C, i + 0x13);
        ov96_021EB564(hud->players[i].unk_14, i + 5);
        ov96_021EB564(hud->badges[i].unk_0, 3);
        ov96_021EB52C(hud->badges[i].unk_0, 1, 1);
        for (j = 0; j < 2; j++) {
            hud->unk_17C[i][j] = ov96_021EB3E4(arg1, 2, 2, 0x65, 0x18);
            ov96_021EB564(hud->unk_17C[i][j], 2);
        }
    }
    pos[0] = ov96_0221CBF4;
    hud->unk_138 = ov96_021EB3E4(arg1, 3, 2, 0x65, 0x16);
    ov96_021EB564(hud->unk_138, 4);
    ov96_021EB52C(hud->unk_138, 1, 1);
    ov96_021EB588(hud->unk_138, &pos[0]);
    ov96_02208914(arg1, hud);
    for (i = 0; i < 2; i++) {
        hud->digits[i] = ov96_021EA2C4(arg2, ov96_021EB5E8(arg1), 3, hud->heapId);
        Sprite_SetDrawFlag(hud->digits[i], 1);
        pos[1].x = ov96_0221CBC4[i] << 12;
        pos[1].y = 0x2C4000;
        pos[1].z = 0;
        Sprite_SetMatrix(hud->digits[i], &pos[1]);
        Sprite_SetAnimCtrlSeq(hud->digits[i], ov96_0221CBC8[i]);
    }
#else
    s32 sp1C[6];
    s32 sp18;
    void *sp14;
    s32 *sp10;
    s32 spC;
    UnkStruct_ov96_021E9A78 *sp8;
    Ov96HurdleHud *sp4;
    s32 *var_r4;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 var_r6;
    u8 *var_r7_2;
    u8 *var_r6_2;
    void *var_r4_2;
    void *var_r7;

    sp4 = hud;
    sp18 = 0;
    sp14 = hud;
    var_r7 = (u8 *)hud + 0x13C;
    sp8 = arg2;
    sp10 = (s32 *)((u8 *)hud + 0x17C);
    do {
        *(s32 *)((u8 *)sp14 + 0x38) = (s32)ov96_021EB3E4(arg1, 3, 2, 0x65, 2);
        *(s32 *)((u8 *)sp14 + 0x40) = (s32)ov96_021EB3E4(arg1, 3, 2, 0x65, 3);
        *(s32 *)((u8 *)sp14 + 0x3C) = (s32)ov96_021EB3E4(arg1, 3, 2, 0x65, 0xA);
        temp_r0_2 = (s32)ov96_021EB3E4(arg1, 2, 2, 0x65, 0x13);
        *(s32 *)((u8 *)var_r7 + 8) = temp_r0_2;
        ov96_021EB564((void *)temp_r0_2, 0);
        ov96_021EB52C(*(void **)((u8 *)var_r7 + 8), 1, 1);
        temp_r0_3 = (s32)ov96_021EB3E4(arg1, 2, 2, 0x65, 0x15);
        *(s32 *)((u8 *)var_r7 + 4) = temp_r0_3;
        ov96_021EB564((void *)temp_r0_3, 9);
        ov96_021EB52C(*(void **)((u8 *)var_r7 + 4), 1, 1);
        *(s32 *)var_r7 = (s32)ov96_021EB3E4(arg1, 2, 2, 0x65, 0x14);
        ov96_021EB564(*(void **)((u8 *)sp14 + 0x38), sp18 + 0x13);
        ov96_021EB564(*(void **)((u8 *)sp14 + 0x40), sp18 + 5);
        ov96_021EB564(*(void **)var_r7, 3);
        ov96_021EB52C(*(void **)var_r7, 1, 1);
        var_r4 = sp10;
        var_r6 = 0;
loop_2:
        temp_r0_4 = (s32)ov96_021EB3E4(arg1, 2, 2, 0x65, 0x18);
        *var_r4 = temp_r0_4;
        var_r4 += 1;
        ov96_021EB564((void *)temp_r0_4, 2);
        var_r6 += 1;
        if (var_r6 < 2) {
            goto loop_2;
        }
        var_r7 += 0x10;
        sp14 += 0x1C;
        sp10 += 2;
        temp_r0 = sp18 + 1;
        sp18 = temp_r0;
    } while (temp_r0 < 4);
    *(VecFx32 *)&sp1C[0] = ov96_0221CBF4;
    *(void **)((u8 *)sp4 + 0x138) = ov96_021EB3E4(arg1, 3, 2, 0x65, 0x16);
    ov96_021EB564(*(void **)((u8 *)sp4 + 0x138), 4);
    ov96_021EB52C(*(void **)((u8 *)sp4 + 0x138), 1, 1);
    ov96_021EB588(*(void **)((u8 *)sp4 + 0x138), (VecFx32 *)&sp1C[0]);
    ov96_02208914(arg1, sp4);
    var_r4_2 = sp4;
    var_r7_2 = ov96_0221CBC4;
    var_r6_2 = ov96_0221CBC8;
    spC = 0;
    do {
        *(Sprite **)((u8 *)var_r4_2 + 0x1B4) = ov96_021EA2C4(sp8, ov96_021EB5E8(arg1), 3, sp4->heapId);
        Sprite_SetDrawFlag(*(Sprite **)((u8 *)var_r4_2 + 0x1B4), 1);
        sp1C[3] = *var_r7_2 << 0xC;
        sp1C[4] = 0x2C4000;
        sp1C[5] = 0;
        Sprite_SetMatrix(*(Sprite **)((u8 *)var_r4_2 + 0x1B4), (VecFx32 *)&sp1C[3]);
        Sprite_SetAnimCtrlSeq(*(Sprite **)((u8 *)var_r4_2 + 0x1B4), (s32)*var_r6_2);
        var_r4_2 += 4;
        var_r7_2 += 1;
        temp_r0_5 = spC + 1;
        var_r6_2 += 1;
        spC = temp_r0_5;
    } while (temp_r0_5 < 2);
#endif
}
