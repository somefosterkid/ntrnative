#include "global.h"

#include "item.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "system.h"

u32 AGB_GetBoxMonAbility(void *, BoxPokemon *);

u32 AGB_GetBoxMonData(void *, s32, s32 *);

s32 ConvertRSStringToDPStringInternational(s32 *, s32 *, s32, u32);

u32 TranslateAgbSpecies(int species);

void MigrateBoxMon(void *arg0, BoxPokemon *arg1);

void MigrateBoxMon(void *arg0, BoxPokemon *arg1) {
    s32 sp24[3];
    s32 spC[6];
    u32 sp8;
    u32 sp4;
    s32 sp0;
    s32 temp_r0_3;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 var_r5;
    s32 var_r5_2;
    s32 var_r5_3;
    s32 var_r5_4;
    s32 var_r5_5;
    s32 var_r5_6;
    s32 var_r7;
    u32 temp_r0;
    u32 temp_r0_2;
    u32 temp_r0_8;

    ZeroBoxMonData(arg1);
    sp0 = AcquireBoxMonLock(arg1);
    sp8 = AGB_GetBoxMonData(arg0, 0, NULL);
    SetBoxMonData(arg1, 0, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0xB, NULL);
    sp8 = TranslateAgbSpecies(sp8);
    SetBoxMonData(arg1, 5, &sp8);
    temp_r0 = AGB_GetBoxMonData(arg0, 0xC, NULL);
    sp8 = temp_r0;
    if (temp_r0 != 0) {
        sp8 = (u32)UpConvertItemId_Gen3to4((u16)temp_r0);
    }
    SetBoxMonData(arg1, 6, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 1, NULL);
    SetBoxMonData(arg1, 7, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x19, NULL);
    SetBoxMonData(arg1, 8, &sp8);
    sp8 = 0x46;
    SetBoxMonData(arg1, 9, &sp8);
    sp8 = AGB_GetBoxMonAbility(arg0, arg1);
    SetBoxMonData(arg1, 0xA, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 8, NULL);
    SetBoxMonData(arg1, 0xB, &sp8);
    sp4 = AGB_GetBoxMonData(arg0, 3, NULL);
    SetBoxMonData(arg1, 0xC, &sp4);
    sp8 = AGB_GetBoxMonData(arg0, 0x1A, NULL);
    SetBoxMonData(arg1, 0xD, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x1B, NULL);
    SetBoxMonData(arg1, 0xE, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x1C, NULL);
    SetBoxMonData(arg1, 0xF, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x1D, NULL);
    SetBoxMonData(arg1, 0x10, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x1E, NULL);
    SetBoxMonData(arg1, 0x11, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x1F, NULL);
    SetBoxMonData(arg1, 0x12, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x16, NULL);
    SetBoxMonData(arg1, 0x13, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x17, NULL);
    SetBoxMonData(arg1, 0x14, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x18, NULL);
    SetBoxMonData(arg1, 0x15, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x21, NULL);
    SetBoxMonData(arg1, 0x16, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x2F, NULL);
    SetBoxMonData(arg1, 0x17, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x30, NULL);
    SetBoxMonData(arg1, 0x18, &sp8);
    var_r5 = 0;
    var_r7 = 0;
    do {
        sp8 = AGB_GetBoxMonData(arg0, var_r5 + 0xD, NULL);
        SetBoxMonData(arg1, var_r5 + 0x36, &sp8);
        temp_r0_2 = AGB_GetBoxMonData(arg0, 0x15, NULL);
        sp8 = temp_r0_2;
        sp8 = (u32)(temp_r0_2 & (3 << var_r7)) >> var_r7;
        SetBoxMonData(arg1, var_r5 + 0x3E, &sp8);
        sp8 = GetBoxMonData(arg1, var_r5 + 0x42, NULL);
        SetBoxMonData(arg1, var_r5 + 0x3A, &sp8);
        var_r5 += 1;
        var_r7 += 2;
    } while (var_r5 < 4);
    sp8 = AGB_GetBoxMonData(arg0, 0x27, NULL);
    SetBoxMonData(arg1, 0x46, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x28, NULL);
    SetBoxMonData(arg1, 0x47, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x29, NULL);
    SetBoxMonData(arg1, 0x48, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x2A, NULL);
    SetBoxMonData(arg1, 0x49, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x2B, NULL);
    SetBoxMonData(arg1, 0x4A, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x2C, NULL);
    SetBoxMonData(arg1, 0x4B, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x2D, NULL);
    SetBoxMonData(arg1, 0x4C, &sp8);
    temp_r0_3 = (s32)AGB_GetBoxMonData(arg0, 0x32, NULL);
    if (temp_r0_3 <= 4) {
        var_r5_2 = 0;
        if (temp_r0_3 > 0) {
            do {
                sp8 = 1;
                SetBoxMonData(arg1, var_r5_2 + 0x4E, &sp8);
                var_r5_2 += 1;
            } while (var_r5_2 < temp_r0_3);
        }
    }
    temp_r0_4 = (s32)AGB_GetBoxMonData(arg0, 0x33, NULL);
    if (temp_r0_4 <= 4) {
        var_r5_3 = 0;
        if (temp_r0_4 > 0) {
            do {
                sp8 = 1;
                SetBoxMonData(arg1, var_r5_3 + 0x52, &sp8);
                var_r5_3 += 1;
            } while (var_r5_3 < temp_r0_4);
        }
    }
    temp_r0_5 = (s32)AGB_GetBoxMonData(arg0, 0x34, NULL);
    if (temp_r0_5 <= 4) {
        var_r5_4 = 0;
        if (temp_r0_5 > 0) {
            do {
                sp8 = 1;
                SetBoxMonData(arg1, var_r5_4 + 0x56, &sp8);
                var_r5_4 += 1;
            } while (var_r5_4 < temp_r0_5);
        }
    }
    temp_r0_6 = (s32)AGB_GetBoxMonData(arg0, 0x35, NULL);
    if (temp_r0_6 <= 4) {
        var_r5_5 = 0;
        if (temp_r0_6 > 0) {
            do {
                sp8 = 1;
                SetBoxMonData(arg1, var_r5_5 + 0x5A, &sp8);
                var_r5_5 += 1;
            } while (var_r5_5 < temp_r0_6);
        }
    }
    temp_r0_7 = (s32)AGB_GetBoxMonData(arg0, 0x36, NULL);
    if (temp_r0_7 <= 4) {
        var_r5_6 = 0;
        if (temp_r0_7 > 0) {
            do {
                sp8 = 1;
                SetBoxMonData(arg1, var_r5_6 + 0x5E, &sp8);
                var_r5_6 += 1;
            } while (var_r5_6 < temp_r0_7);
        }
    }
    sp8 = AGB_GetBoxMonData(arg0, 0x43, NULL);
    SetBoxMonData(arg1, 0x62, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x44, NULL);
    SetBoxMonData(arg1, 0x63, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x45, NULL);
    SetBoxMonData(arg1, 0x64, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x46, NULL);
    SetBoxMonData(arg1, 0x65, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x47, NULL);
    SetBoxMonData(arg1, 0x66, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x48, NULL);
    SetBoxMonData(arg1, 0x67, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x49, NULL);
    SetBoxMonData(arg1, 0x68, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x4A, NULL);
    SetBoxMonData(arg1, 0x69, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x4B, NULL);
    SetBoxMonData(arg1, 0x6A, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x4C, NULL);
    SetBoxMonData(arg1, 0x6B, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x4D, NULL);
    SetBoxMonData(arg1, 0x6C, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x4E, NULL);
    SetBoxMonData(arg1, 0x6D, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x50, NULL);
    SetBoxMonData(arg1, 0x6E, &sp8);
    sp8 = (u32)GetBoxMonGender(arg1);
    SetBoxMonData(arg1, 0x6F, &sp8);
    if (GetBoxMonData(arg1, 5, NULL) == 0xC9) {
        temp_r0_8 = AGB_GetBoxMonData(arg0, 0, NULL);
        sp8 = temp_r0_8;
        sp8 = (u32)(((u32)(0x03000000 & temp_r0_8) >> 0x12) | ((u32)(0x30000 & temp_r0_8) >> 0xC) | ((u32)(0x300 & temp_r0_8) >> 6) | (temp_r0_8 & 3)) % 28U;
        SetBoxMonData(arg1, 0x70, &sp8);
    }
    if (GetBoxMonData(arg1, 5, NULL) == 0x182) {
        switch (gSystem.unk6A) {
        default:
        case 0:
        case 1:
        case 2:
            sp8 = 0;
            break;
        case 4:
            sp8 = 1;
            break;
        case 5:
            sp8 = 2;
            break;
        case 3:
            sp8 = 3;
            break;
        }
        SetBoxMonData(arg1, 0x70, &sp8);
    }
    AGB_GetBoxMonData(arg0, 2, sp24);
    ConvertRSStringToDPStringInternational(sp24, spC, 0xC, sp4);
    SetBoxMonData(arg1, 0x76, spC);
    if (gGameLanguage != AGB_GetBoxMonData(arg0, 3, NULL)) {
        sp8 = 1;
        SetBoxMonData(arg1, 0x4D, &sp8);
    }
    sp8 = AGB_GetBoxMonData(arg0, 0x25, NULL);
    SetBoxMonData(arg1, 0x7A, &sp8);
    AGB_GetBoxMonData(arg0, 7, sp24);
    ConvertRSStringToDPStringInternational(sp24, spC, 8, sp4);
    SetBoxMonData(arg1, 0x90, spC);
    sp8 = AGB_GetBoxMonData(arg0, 0x23, NULL);
    SetBoxMonData(arg1, 0x99, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x22, NULL);
    SetBoxMonData(arg1, 0x9A, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x26, NULL);
    SetBoxMonData(arg1, 0x9B, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x24, NULL);
    SetBoxMonData(arg1, 0x9C, &sp8);
    sp8 = AGB_GetBoxMonData(arg0, 0x31, NULL);
    SetBoxMonData(arg1, 0x9D, &sp8);
    ReleaseBoxMonLock(arg1, sp0);
}
