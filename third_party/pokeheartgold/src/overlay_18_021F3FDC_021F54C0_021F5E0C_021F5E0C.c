#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F5EFC(PokedexAppData *, u32, u32);

void ov18_021F61DC(PokedexAppData *, u32, s32, u8 *, u32);

extern u32 ov18_021FA348;

extern ManagedSpriteTemplate ov18_021FAC28[];

void ov18_021F5E0C(PokedexAppData *param_1);

void ov18_021F5E0C(PokedexAppData *param_1) {
    u32 i;

    for (i = 1; i <= 4; i++) {
        param_1->unk_0670[i] = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &ov18_021FAC28[i - 1], 0x200000);
    }
    for (i = 5; i < 0x14; i++) {
        param_1->unk_0670[i] = SpriteSystem_NewSprite(param_1->unk_0668, param_1->unk_066C, &ov18_021FAC28[i - 1]);
    }
    ov18_021F11C0(param_1, 9, 0);
    ov18_021F11C0(param_1, 10, 0);
    ov18_021F11C0(param_1, 0xb, 0);
    ov18_021F11C0(param_1, 0xc, 0);
    ov18_021F11C0(param_1, 0xd, 0);
    ov18_021F5EFC(param_1, 0, 0);
    ov18_021F6038(param_1);
    ov18_021F61DC(param_1, 5, (s32)param_1->numSeenForms, (u8 *)(&ov18_021FA348), 9);
    ov18_021F65AC(param_1);
    ov18_021F619C(param_1, param_1->unk_18C5, param_1->numSeenForms, 6);
    return;
}
