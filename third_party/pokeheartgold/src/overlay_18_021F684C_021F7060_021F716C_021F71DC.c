#include "global.h"

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F719C(PokedexAppData *, s32);

void ov18_021F71DC(PokedexAppData *pokedexApp, s32 arg1, s32 arg2);

void ov18_021F71DC(PokedexAppData *pokedexApp, s32 arg1, s32 arg2) {
    s32 x;

    if (arg2 == 0x12) {
        if (arg1 == 0) {
            x = pokedexApp->unk_189C;
            if (x >= 0x10 && x <= 0x11) {
                arg1 = x % 4;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)arg1, (u8)arg2, 0x12);
            } else if (x >= 0 && x <= 1) {
                arg1 = x;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)arg1, (u8)arg2, 0x12);
            }
        } else if (arg1 == 0x10) {
            x = pokedexApp->unk_189C;
            if (x >= 0 && x <= 1) {
                arg1 = x + 0x10;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)arg1, (u8)arg2, 0x12);
            } else if (x >= 0x10 && x <= 0x11) {
                arg1 = x;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)arg1, (u8)arg2, 0x12);
            }
        }
    } else if (arg2 == 0x13) {
        if (arg1 == 3) {
            x = pokedexApp->unk_189C;
            if (x >= 0xE && x <= 0xF) {
                arg1 = x % 4;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)arg1, (u8)arg2, 0x13);
            } else if (x >= 2 && x <= 3) {
                arg1 = x;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)arg1, (u8)arg2, 0x13);
            }
        } else if (arg1 == 0xF) {
            x = pokedexApp->unk_189C;
            if (x >= 2 && x <= 3) {
                arg1 = x + 0xC;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)arg1, (u8)arg2, 0x13);
            } else if (x >= 0xE && x <= 0xF) {
                arg1 = x;
                GridInputHandler_SetNextLastUnk0FInputs(pokedexApp->unk_1864, (u8)arg1, (u8)arg2, 0x13);
            }
        }
    }
    ov18_021F719C(pokedexApp, arg1);
    pokedexApp->unk_189C = arg2;
    PlaySE(0x8E8);
}
