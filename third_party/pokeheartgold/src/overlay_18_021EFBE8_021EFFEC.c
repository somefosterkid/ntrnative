#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "unk_0201956C.h"

s32 ov18_021EE44C(PokedexAppData *, s32, s32);

s32 ov18_021EE520(PokedexAppData *, s32, u32);

s32 ov18_021F006C(PokedexAppData *, s32);

void ov18_021EFFEC(PokedexAppData *pokedexApp) {
    sub_02019B08(pokedexApp->unk_0008, 0xF);
    ov18_021EE44C(pokedexApp, 0x5F, 0);
    ov18_021EE44C(pokedexApp, 0x60, 1);
    ov18_021EE520(pokedexApp, pokedexApp->unk_185D + 0x61, pokedexApp->unk_0878.unk_7B4);
    ov18_021EE520(pokedexApp, pokedexApp->unk_185D + 0x63, pokedexApp->unk_0878.unk_7B6);
    ov18_021F006C(pokedexApp, 0x5F);
    ov18_021F006C(pokedexApp, 0x60);
    ov18_021F006C(pokedexApp, pokedexApp->unk_185D + 0x61);
    ov18_021F006C(pokedexApp, pokedexApp->unk_185D + 0x63);
    pokedexApp->unk_185D ^= 1;
}
