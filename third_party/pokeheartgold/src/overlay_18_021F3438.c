#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite_system.h"

s32 ov18_021F1104(void *arg0);

s32 ov18_021F310C(PokedexAppData *);

void ov18_021F34C4(PokedexAppData *arg0, s32 arg1);
void ov18_021F34EC(PokedexAppData *arg0, s32 arg1);
void ov18_021F3544(PokedexAppData *arg0, s32 arg1);
void ov18_021F3560(PokedexAppData *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov18_021F3438(PokedexAppData *pokedexApp) {
    ov18_021F1104(pokedexApp);
    ov18_021F310C(pokedexApp);
}

s32 ov18_021F10E8(PokedexAppData *, s32);

void ov18_021F3448(PokedexAppData *pokedexApp) {
    ov18_021F10E8(pokedexApp, 0x1A);
    ov18_021F10E8(pokedexApp, 0x1B);
    SpriteManager_UnloadCharObjById(pokedexApp->unk_066C, 0xC59EU);
    SpriteManager_UnloadPlttObjById(pokedexApp->unk_066C, 0xC55FU);
    SpriteManager_UnloadCellObjById(pokedexApp->unk_066C, 0xC55CU);
    SpriteManager_UnloadAnimObjById(pokedexApp->unk_066C, 0xC55CU);
}

void ov18_021F3494(PokedexAppData *pokedexApp) {
    if (pokedexApp->dexSearchCriteria[9] == 0xE) {
        ov18_021F11C0(pokedexApp, 0x1C, 0);
        return;
    }
    ov18_021F11C0(pokedexApp, 0x1C, 1);
    ov18_021F118C(pokedexApp, 0x1C, (s32)pokedexApp->dexSearchCriteria[9]);
}

void ov18_021F34C4(PokedexAppData *arg0, s32 arg1) {
    s32 a1;

    if (arg1 == 1) {
        ov18_021F11C0(arg0, 0x1C, 0);
    }
    a1 = 0x1D;
    do {
        ov18_021F11C0(arg0, a1, 0);
        a1 += 1;
    } while ((u32)a1 <= 0x2AU);
}

void ov18_021F34EC(PokedexAppData *arg0, s32 arg1) {
    s32 a1;
    s32 v0;

    ov18_021F3494(arg0);
    if (arg1 == 1) {
        ManagedSprite_SetPositionXY(arg0->unk_0670[28], 0xE0, 0x48);
        a1 = 0x1D;
        do {
            ov18_021F11C0(arg0, a1, 0);
            a1 += 1;
        } while ((u32)a1 <= 0x2AU);
        return;
    }
    ManagedSprite_SetPositionXY(arg0->unk_0670[28], 0x98, 0x14);
    v0 = 0x1D;
    do {
        ov18_021F11C0(arg0, v0, 1);
        v0 += 1;
    } while ((u32)v0 <= 0x2AU);
}

void ov18_021F3544(PokedexAppData *arg0, s32 a2) {
    s32 a1;

    a1 = 1;
    do {
        ov18_021F11C0(arg0, a1, a2);
        a1 += 1;
    } while ((u32)a1 <= 0x10U);
}

void ov18_021F3560(PokedexAppData *arg0, s32 a1, s32 arg2, s32 arg3) {
    if (arg3 == 0) {
        ov18_021F36D4(arg0, ov18_021F3AD0(arg0, arg2), 5, 1);
        ov18_021F36D4(arg0, ov18_021F3AD0(arg0, a1), 0xB, 0);
        ov18_021F38F0(arg0, 6, arg0->unk_1850[arg2].unk_0);
        ov18_021F38F0(arg0, 0xC, arg0->unk_1850[a1].unk_0);
    } else {
        ov18_021F37D4(arg0, ov18_021F3AD0(arg0, arg2), 5, 1);
        ov18_021F37D4(arg0, ov18_021F3AD0(arg0, a1), 0xB, 0);
        ov18_021F39C4(arg0, 6, arg0->unk_1850[arg2].unk_2);
        ov18_021F39C4(arg0, 0xC, arg0->unk_1850[a1].unk_2);
    }
    ov18_021F3A64(arg0, (u32)arg2, 1);
    ov18_021F3A64(arg0, (u32)a1, 3);
}
