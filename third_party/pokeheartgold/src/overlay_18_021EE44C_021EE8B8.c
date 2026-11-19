#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "wild_encounter.h"

s32 ov18_021EE984(PokedexAppData *, u32, s32, s32);

s32 ov18_021EE9FC(PokedexAppData *, s32);

s32 ov18_021EEA40(PokedexAppData *, s32);

s32 ov18_021EEAE4(PokedexAppData *, u32, s32, s32);

s32 ov18_021EEB94(PokedexAppData *, u32, s32, s32);

s32 ov18_021EEBE4(PokedexAppData *, s32);

s32 ov18_021EEC34(PokedexAppData *, u32, s32, s32);

s32 ov18_021EECB0(PokedexAppData *, u32, s32);

s32 ov18_021EED00(PokedexAppData *, u32, s32, s32);

void ov18_021EE8B8(PokedexAppData *pokedexApp, u32 species, int a2) {
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0xC]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0xE]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0xB]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0xD]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0xF]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x10]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x11]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x12]);
    ClearWindowTilemapAndScheduleTransfer(&pokedexApp->windows[0x13]);
    if (species != 0) {
        ov18_021EE984(pokedexApp, species, a2, 0xB);
        if (pokedexApp->unk_185C == 2) {
            ov18_021EE9FC(pokedexApp, 0xC);
            ov18_021EEA40(pokedexApp, 0xE);
            ov18_021EEAE4(pokedexApp, species, a2, 0xD);
            ov18_021EEB94(pokedexApp, species, a2, 0xF);
            return;
        }
        ov18_021EEBE4(pokedexApp, 0x10);
        ov18_021EEC34(pokedexApp, species, 0x11, pokedexApp->unk_185C);
        ov18_021EECB0(pokedexApp, species, 0x12);
        ov18_021EED00(pokedexApp, species, a2, 0x13);
    }
}
