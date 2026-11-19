#include "global.h"

#include "heap.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "wild_encounter.h"

int TranslateAgbSpecies(int species);

void *ov74_02231E00(s32, u32, void **, s32);

u8 ov74_02231E54(s32, u32, s32);

void ov74_02231EC4(s32 species, s32 isEgg, u32 personality, s32 gbaVersion, s32 param4, Sprite *iconSprite);

void ov74_02231EC4(s32 species, s32 isEgg, u32 personality, s32 gbaVersion, s32 param4, Sprite *iconSprite) {
    void *v0;
    s32 v1;
    u32 form;
    void *ptr;

    v1 = TranslateAgbSpecies(species);
    form = ov74_02231E54(v1, personality, gbaVersion);
    ptr = ov74_02231E00(0x14, GetMonIconNaixEx((u32)v1, isEgg, form), &v0, 0x4C);
    DC_FlushRange(*(void **)((u8 *)v0 + 0x14), 0x200U);
    GX_LoadOBJ(*(void **)((u8 *)v0 + 0x14), ((param4 * 0x10) + 0x64) << 5, 0x200U);
    Sprite_SetPaletteOverride(iconSprite, GetMonIconPaletteEx((u32)v1, form, (u32)isEgg) + 8);
    Heap_Free(ptr);
}
