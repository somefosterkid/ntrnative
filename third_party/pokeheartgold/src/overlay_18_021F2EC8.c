#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"


s32 ov18_021F1104(void *arg0);
s32 ov18_021F13DC(PokedexAppData *, s32);
s32 ov18_021F18E0(PokedexAppData *);
s32 ov18_021F1D58(PokedexAppData *);
s32 ov18_021F1F74(PokedexAppData *);
s32 ov18_021F2308(PokedexAppData *);
s32 ov18_021F23E4(PokedexAppData *);
s32 ov18_021F26E4(PokedexAppData *);
s32 ov18_021F281C(PokedexAppData *);
s32 ov18_021F2F4C(PokedexAppData *arg0);
s32 ov18_021F32B8(PokedexAppData *);

void ov18_021F2EC8(PokedexAppData *pokedexApp, int a1, int a2) {
    if (pokedexApp->unk_1030[a1].unk_2 == 2) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a2], 1);
        return;
    }
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a2], 0);
    return;
}

void ov18_021F2F00(PokedexAppData *pokedexApp) {
    ov18_021F1104(pokedexApp);
    ov18_021F13DC(pokedexApp, 0x3C);
    ov18_021F26E4(pokedexApp);
    ov18_021F2308(pokedexApp);
    ov18_021F18E0(pokedexApp);
    ov18_021F1D58(pokedexApp);
    ov18_021F1F74(pokedexApp);
    ov18_021F281C(pokedexApp);
    ov18_021F23E4(pokedexApp);
}

void ov18_021F2F3C(PokedexAppData *pokedexApp) {
    ov18_021F2F4C(pokedexApp);
    ov18_021F32B8(pokedexApp);
}
