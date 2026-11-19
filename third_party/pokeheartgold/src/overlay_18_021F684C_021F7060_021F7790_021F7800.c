#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F77C0(PokedexAppData *, s32);

void ov18_021F7800(PokedexAppData *pokedexApp, s32 arg1, s32 arg2);

void ov18_021F7800(PokedexAppData *pokedexApp, s32 arg1, s32 arg2) {
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;
    s32 temp_r1_4;
    s32 var_r6;

    var_r6 = arg1;
    if (arg2 == 0xF) {
        if (var_r6 == 0) {
            temp_r1 = pokedexApp->unk_189C;
            if ((temp_r1 >= 0) && (temp_r1 <= 2)) {
                var_r6 = temp_r1;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (s32)(u8)temp_r1, (u8)arg2, 0xF);
            } else if ((temp_r1 >= 0xA) && (temp_r1 <= 0xC)) {
                var_r6 = temp_r1 % 5;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (s32)(u8)var_r6, (u8)arg2, 0xF);
            }
        } else if (var_r6 == 0xA) {
            temp_r1_2 = pokedexApp->unk_189C;
            if ((temp_r1_2 >= 0) && (temp_r1_2 <= 2)) {
                var_r6 = temp_r1_2 + 0xA;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (s32)(u8)var_r6, (u8)arg2, 0xF);
            } else if ((temp_r1_2 >= 0xA) && (temp_r1_2 <= 0xC)) {
                var_r6 = temp_r1_2;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (s32)(u8)temp_r1_2, (u8)arg2, 0xF);
            }
        }
    } else if (arg2 == 0x10) {
        if (var_r6 == 4) {
            temp_r1_3 = pokedexApp->unk_189C;
            if ((temp_r1_3 >= 3) && (temp_r1_3 <= 4)) {
                var_r6 = temp_r1_3;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (s32)(u8)temp_r1_3, (u8)arg2, 0x10);
            } else if ((temp_r1_3 >= 0xD) && (temp_r1_3 <= 0xE)) {
                var_r6 = temp_r1_3 % 5;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (s32)(u8)var_r6, (u8)arg2, 0x10);
            }
        } else if (var_r6 == 0xE) {
            temp_r1_4 = pokedexApp->unk_189C;
            if ((temp_r1_4 >= 3) && (temp_r1_4 <= 4)) {
                var_r6 = temp_r1_4 + 0xA;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (s32)(u8)var_r6, (u8)arg2, 0x10);
            } else if ((temp_r1_4 >= 0xD) && (temp_r1_4 <= 0xE)) {
                var_r6 = temp_r1_4;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (s32)(u8)temp_r1_4, (u8)arg2, 0x10);
            }
        }
    }
    ov18_021F77C0(pokedexApp, var_r6);
    pokedexApp->unk_189C = (s32)arg2;
    PlaySE(0x8E8U);
}
