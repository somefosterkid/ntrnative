#include "global.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save_trainer_house.h"
#include "string_util.h"

void ov112_021F33D8(void *arg0, Party *arg1);

void ov112_021F33D8(void *arg0, Party *arg1) {
    u16 sp20[11];
    s32 sp1C;
    s32 sp18;
    s32 temp_r0;
    s32 sp14;
    s32 sp10;
    s32 spC;
    void *sp8;
    Party *sp0;
    s32 var_r7;
    void *var_r6;
    Pokemon *var_r5;
    s32 var_r4;
    s32 temp_r1_2;
    void *temp_r1_3;
    s32 temp_r1_4;
    s32 sp4;
    Pokemon *temp_r0_2;
    u32 temp_r0_3;
    s32 temp_r0_4;

    var_r6 = arg0;
    sp0 = arg1;
    temp_r0 = Party_GetCount(arg1);
    sp18 = temp_r0;
    sp14 = 0;
    sp1C = 0;
    if (temp_r0 <= 0) {
        return;
    }
loop_2:
    spC = 0;
    temp_r0_2 = Party_GetMonByIndex(sp0, sp1C);
    var_r5 = temp_r0_2;
    if ((GetMonData(temp_r0_2, 3, NULL) != 0) || (GetMonData(var_r5, 0x4C, NULL) != 0)) {
        goto block_15;
    }
    *(u16 *)var_r6 = (u16)(((0xFFFFF800U >> 0x15) & (u16)GetMonData(var_r5, 5, NULL)) | (*(u16 *)var_r6 & 0xFFFFF800));
    *(u16 *)var_r6 = (u16)((u16)((u16)GetMonData(var_r5, 0x70, NULL) << 0xB) | (0xFFFF07FF & *(u16 *)var_r6));
    if (((TrainerHouseMon *)var_r6)->species == 0x1EC && ((TrainerHouseMon *)var_r6)->form == 1) {
        Mon_UpdateShayminForm(var_r5, 0);
        *(u16 *)var_r6 = (u16)(0xFFFF07FF & *(u16 *)var_r6);
        spC = 1;
    }
    *(s16 *)((u8 *)var_r6 + 2) = (s16)GetMonData(var_r5, 6, NULL);
    *(u32 *)((u8 *)var_r6 + 0xC) = GetMonData(var_r5, 7, NULL);
    *(u32 *)((u8 *)var_r6 + 0x10) = GetMonData(var_r5, 0, NULL);
    *(s8 *)((u8 *)var_r6 + 0x1F) = (s8)GetMonData(var_r5, 0xC, NULL);
    *(s8 *)((u8 *)var_r6 + 0x20) = (s8)GetMonData(var_r5, 0xA, NULL);
    *(s8 *)((u8 *)var_r6 + 0x21) = (s8)GetMonData(var_r5, 9, NULL);
    *(s8 *)((u8 *)var_r6 + 0x22) = (s8)GetMonData(var_r5, 0xA1, NULL);
    GetMonData(var_r5, 0x75, sp20);
    CopyU16StringArrayN(var_r6 + 0x24, sp20, 0xAU);
    var_r4 = 0;
    sp10 = 0;
    sp8 = var_r6;
    var_r7 = 0;
    do {
        *(s16 *)((u8 *)sp8 + 4) = (s16)GetMonData(var_r5, var_r4 + 0x36, NULL);
        temp_r1_2 = GetMonData(var_r5, var_r4 + 0x3E, NULL) << var_r7;
        var_r4 += 1;
        sp10 |= temp_r1_2;
        var_r7 += 2;
        sp8 += 2;
    } while (var_r4 < 4);
    var_r4 = 0;
    *(s8 *)((u8 *)var_r6 + 0x1E) = (s8)sp10;
    sp4 = 0;
    var_r7 = 0;
    do {
        sp4 |= GetMonData(var_r5, var_r4 + 0x46, NULL) << var_r7;
        temp_r0_3 = GetMonData(var_r5, var_r4 + 0xD, NULL);
        temp_r1_3 = var_r6 + var_r4;
        var_r4 += 1;
        var_r7 += 5;
        *(s8 *)((u8 *)temp_r1_3 + 0x18) = (s8)temp_r0_3;
    } while (var_r4 < 6);
    *(s32 *)((u8 *)var_r6 + 0x14) = sp4;
    if (spC != 0) {
        Mon_UpdateShayminForm(var_r5, 1);
    }
    var_r6 += 0x38;
    temp_r0_4 = sp14 + 1;
    sp14 = temp_r0_4;
    if (temp_r0_4 < 6) {
block_15:
        temp_r1_4 = sp1C + 1;
        sp1C = temp_r1_4;
        if (temp_r1_4 < sp18) {
            goto loop_2;
        }
    }
}
