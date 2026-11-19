#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F17FC(void *arg0);

void ov18_021F1CB4(void *arg0);

void ov18_021F1E70(void *arg0);

void ov18_021F2348(void *arg0);

void ov18_021F2724(void *arg0);

s32 ov18_021F1A30(PokedexAppData *, s32);

s32 ov18_021F1D98(PokedexAppData *, s32);

s32 ov18_021F1FDC(PokedexAppData *, s32);

s32 ov18_021F2424(PokedexAppData *, u32, s16 *);

extern s32 ov18_021FA484;

extern ManagedSpriteTemplate ov18_021FAB58;

extern ManagedSpriteTemplate ov18_021FAB8C;

typedef struct Words13 {
    u32 w[13];
} Words13;

void ov18_021F3190(PokedexAppData *pokedexApp) {
    s16 sp0[25];
    u8 *var_r4;
    u8 *var_r3;
    s32 temp_r0;
    u32 var_r4_3;
    u32 var_r7;

    ov18_021F17FC(pokedexApp);
    ov18_021F1CB4(pokedexApp);
    ov18_021F1E70(pokedexApp);
    ov18_021F2724(pokedexApp);
    ov18_021F2348(pokedexApp);
    pokedexApp->unk_0670[44] = SpriteSystem_NewSpriteWithYOffset(pokedexApp->unk_0668, pokedexApp->unk_066C, &ov18_021FAB58, FX32_CONST(512));
    pokedexApp->unk_0670[45] = SpriteSystem_NewSpriteWithYOffset(pokedexApp->unk_0668, pokedexApp->unk_066C, &ov18_021FAB8C, FX32_CONST(512));
    ov18_021F1A30(pokedexApp, 0x2E);
    ov18_021F1D98(pokedexApp, 0x30);
    ov18_021F1FDC(pokedexApp, 0x31);
    var_r4 = (u8 *)&ov18_021FA484;
    var_r3 = (u8 *)sp0;
    *(Words13 *)var_r3 = *(Words13 *)var_r4;
    temp_r0 = 0x4F8;
    var_r7 = 0x35;
    do {
        sp0[0] = temp_r0 - 0x47C;
        ov18_021F2424(pokedexApp, var_r7, sp0);
        ManagedSprite_SetPaletteOverride(pokedexApp->unk_0670[var_r7], SpriteManager_FindPlttResourceOffset(pokedexApp->unk_066C, 0xC55A, 2));
        temp_r0 += 0x18;
        var_r7 += 1;
    } while (var_r7 <= 0x3AU);
    var_r4_3 = 0x2C;
    do {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[var_r4_3], 0);
        ManagedSprite_SetPriority(pokedexApp->unk_0670[var_r4_3], 2);
        var_r4_3 += 1;
    } while (var_r4_3 <= 0x3AU);
}
