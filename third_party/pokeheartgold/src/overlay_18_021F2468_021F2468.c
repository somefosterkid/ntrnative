#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021F2424(PokedexAppData *, u32, s16 *);

typedef struct {
    u32 a[13];
} ov18_021FA484_Array;

extern const ov18_021FA484_Array ov18_021FA484;

extern u32 ov18_021FAB24;

void ov18_021F2468(PokedexAppData *param_1);

void ov18_021F2468(PokedexAppData *param_1) {
    s32 index;
    u32 uVar7;
    s32 sVar5;
    ov18_021FA484_Array buf;
    buf = ov18_021FA484;
    uVar7 = 0x12;
    sVar5 = 0x1b0;
    do {
        *(s16 *)buf.a = sVar5 + (-0x134);
        ov18_021F2424(param_1, uVar7, (s16 *)buf.a);
        index = SpriteManager_FindPlttResourceOffset(param_1->unk_066C, 0xc55a, 2);
        ManagedSprite_SetPaletteOverride(param_1->unk_0670[uVar7], index);
        sVar5 = sVar5 + 0x18;
        uVar7 = uVar7 + 1;
    } while (uVar7 <= (0x18 - 1));
    ov18_021F2424(param_1, 8, (s16 *)(&ov18_021FAB24));
}
