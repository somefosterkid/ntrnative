#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F47C0(u32, u32 *);

void ov18_021F48AC(PokedexAppData *, u32, u32, u32, s32);

void ov18_021F463C(PokedexAppData *pokedexApp) {
    u32 uVar1;
    BOOL BVar2;
    u32 uVar3;
    u32 uStack_18;
    u32 uStack_1c;

    uStack_18 = 0;
    uStack_1c = 0;
    if (pokedexApp->unk_18CA == 0) {
        uVar3 = 1;
        if (1 < (pokedexApp->unk_18DC).unk_20.nMaps) {
            do {
                uVar1 = ov18_021E8ACC(pokedexApp, uVar3, pokedexApp->unk_18CB_6);
                if (uVar1 != 0) {
                    ManagedSprite_SetPaletteOverride(pokedexApp->unk_0670[uVar3 + 8],
                        4 + pokedexApp->unk_18CB_6);
                    ov18_021F11C0(pokedexApp, uVar3 + 8, 1);
                } else {
                    ov18_021F11C0(pokedexApp, uVar3 + 8, 0);
                    BVar2 = ov18_021E8AB0(pokedexApp, uVar3);
                    if (BVar2 != 0) {
                        uVar1 = ov18_021E8B18((pokedexApp->unk_18DC).unk_20.maps[uVar3]);
                        ov18_021F47C0(uVar1, &uStack_1c);
                    }
                }
                BVar2 = ov18_021E8AB0(pokedexApp, uVar3);
                if (BVar2 != 0) {
                    uVar1 = ov18_021E8B18((pokedexApp->unk_18DC).unk_20.maps[uVar3]);
                    ov18_021F47C0(uVar1, &uStack_18);
                }
                uVar3 = uVar3 + 1 & 0xffff;
            } while ((s32)uVar3 < (pokedexApp->unk_18DC).unk_20.nMaps);
        }
        ov18_021F48AC(pokedexApp, uStack_18, uStack_1c, uVar3 + 8, 4 + pokedexApp->unk_18CB_6);
        return;
    }
    ov18_021F4620(pokedexApp);
    uVar3 = ov18_021E8ACC(pokedexApp, (s32)pokedexApp->unk_18CA, pokedexApp->unk_18CB_6);
    if (uVar3 != 0) {
        ManagedSprite_SetPaletteOverride(pokedexApp->unk_0670[9],
            4 + pokedexApp->unk_18CB_6);
        ov18_021F11C0(pokedexApp, 9, 1);
    } else {
        ov18_021F11C0(pokedexApp, 9, 0);
        BVar2 = ov18_021E8AB0(pokedexApp, (s32)pokedexApp->unk_18CA);
        if (BVar2 != 0) {
            uVar3 = ov18_021E8B18((pokedexApp->unk_18DC).unk_20.maps[pokedexApp->unk_18CA]);
            ov18_021F47C0(uVar3, &uStack_1c);
        }
    }
    BVar2 = ov18_021E8AB0(pokedexApp, (s32)pokedexApp->unk_18CA);
    if (BVar2 != 0) {
        uVar3 = ov18_021E8B18((pokedexApp->unk_18DC).unk_20.maps[pokedexApp->unk_18CA]);
        ov18_021F47C0(uVar3, &uStack_18);
    }
    ov18_021F48AC(pokedexApp, uStack_18, uStack_1c, 10, 4 + pokedexApp->unk_18CB_6);
    return;
}
