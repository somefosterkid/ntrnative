#include "global.h"

#include "heap.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "sprite_system.h"

typedef struct {
    u32 species : 9;
    u32 form : 5;
    u32 rest : 18;
} MonIconParam98;

void ov98_0221E9FC(ManagedSprite *, void *, u32);

void *ov98_0221EAA8(u8 *, u32, u8 **);

void ov98_0221E970(u8 *param_1, ManagedSprite *sprite, MonIconParam98 *mon, u32 param_4, BOOL animate);

void ov98_0221E970(u8 *param_1, ManagedSprite *sprite, MonIconParam98 *mon, u32 param_4, BOOL animate) {
    u32 size;
    u32 species;
    u32 form;
    void *ptr;
    u8 *data;
    u8 pal;

    size = 0x200;
    if (animate) {
        size = 0x400;
    }
    species = mon->species;
    form = mon->form;
    ptr = ov98_0221EAA8(param_1, GetMonIconNaixEx(species, FALSE, form), &data);
    ov98_0221E9FC(sprite, *(void **)(data + 0x14), size);
    Heap_Free(ptr);
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    pal = GetMonIconPaletteEx(species, form, FALSE);
    ManagedSprite_SetPaletteOverride(sprite, pal + (s8)param_1[0x14]);
    if (animate) {
        ManagedSprite_SetAnim(sprite, 1);
        ManagedSprite_SetAnimSpeed(sprite, FX32_ONE);
        ManagedSprite_SetAnimateFlag(sprite, TRUE);
    }
}
