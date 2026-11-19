#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F5EFC(PokedexAppData *, s32, s32);

void ov18_021F662C(PokedexAppData *pokedexApp) {
    ov18_021F5EFC(pokedexApp, pokedexApp->unk_18C5, 1);
    ov18_021F6038(pokedexApp);
    ov18_021F65AC(pokedexApp);
    ov18_021F619C(pokedexApp, pokedexApp->unk_18C5, pokedexApp->numSeenForms, 6);
    ov18_021F11C0(pokedexApp, 5, 1);
    ov18_021F11C0(pokedexApp, 6, 1);
    ov18_021F11C0(pokedexApp, 7, 1);
    ov18_021F11C0(pokedexApp, 8, 1);
}

void ov18_021F6684(PokedexAppData *pokedexApp) {
    ov18_021F11C0(pokedexApp, 5, 0);
    ov18_021F11C0(pokedexApp, 6, 0);
    ov18_021F11C0(pokedexApp, 7, 0);
    ov18_021F11C0(pokedexApp, 8, 0);
    ov18_021F11C0(pokedexApp, 0xE, 0);
    ov18_021F11C0(pokedexApp, 0xF, 0);
    ov18_021F11C0(pokedexApp, 0x10, 0);
    ov18_021F11C0(pokedexApp, 0x11, 0);
    ov18_021F11C0(pokedexApp, 0x12, 0);
    ov18_021F11C0(pokedexApp, 0x13, 0);
    ov18_021F11C0(pokedexApp, 1, 0);
    ov18_021F11C0(pokedexApp, 2, 0);
    ov18_021F11C0(pokedexApp, 3, 0);
    ov18_021F11C0(pokedexApp, 4, 0);
}

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);

s32 ov18_021F684C(PokedexAppData *, s32, u32, s32);

void ov18_021F6714(PokedexAppData *pokedexApp) {
    if ((s32)pokedexApp->numSeenForms >= 3) {
        ov18_021F11C0(pokedexApp, 9, 1);
        ov18_021F11C0(pokedexApp, 0xA, 1);
        ov18_021F11C0(pokedexApp, 0xB, 1);
        ov18_021F11C0(pokedexApp, 0xC, 1);
        ov18_021F11C0(pokedexApp, 0xD, 1);
    }
    ov18_021F6844(pokedexApp, 0xE, (s32)pokedexApp->unk_18C5);
    ov18_021F6844(pokedexApp, 0xF, (s32)pokedexApp->unk_18C6);
    ov18_021F1294(pokedexApp, 0xE, 0x40, 0x50, 0);
    ov18_021F1294(pokedexApp, 0xF, 0xC0, 0x50, 0);
    ov18_021F684C(pokedexApp, 1, pokedexApp->unk_18C5, 1);
    ov18_021F684C(pokedexApp, 2, pokedexApp->unk_18C6, 1);
    ov18_021F6990(pokedexApp);
    ov18_021F11C0(pokedexApp, 0xE, 1);
    ov18_021F11C0(pokedexApp, 0xF, 1);
}

void ov18_021F67D0(PokedexAppData *pokedexApp) {
    ov18_021F11C0(pokedexApp, 9, 0);
    ov18_021F11C0(pokedexApp, 0xA, 0);
    ov18_021F11C0(pokedexApp, 0xB, 0);
    ov18_021F11C0(pokedexApp, 0xC, 0);
    ov18_021F11C0(pokedexApp, 0xD, 0);
    ov18_021F11C0(pokedexApp, 0xE, 0);
    ov18_021F11C0(pokedexApp, 0xF, 0);
    ov18_021F11C0(pokedexApp, 1, 0);
    ov18_021F11C0(pokedexApp, 2, 0);
    ov18_021F11C0(pokedexApp, 3, 0);
    ov18_021F11C0(pokedexApp, 4, 0);
}
