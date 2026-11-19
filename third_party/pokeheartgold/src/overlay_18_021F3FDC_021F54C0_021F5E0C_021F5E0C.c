#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F5EFC(PokedexAppData *, u32, u32);

void ov18_021F61DC(PokedexAppData *, u32, s32, u8 *, u32);

extern u32 ov18_021FA348;

extern u8 ov18_021FAC28[];

void ov18_021F5E0C(PokedexAppData *param_1);

void ov18_021F5E0C(PokedexAppData *param_1) {
    u32 uVar5;
    s32 iVar4;
    void **ppvVar3;

    uVar5 = 1;
    iVar4 = 0x34;
    ppvVar3 = (void **)&param_1->bgConfig;
    do {
        ppvVar3[0x19c] = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, (ManagedSpriteTemplate *)(ov18_021FAC28 + (iVar4 - 0x34)), 0x200000);
        iVar4 = iVar4 + 0x34;
        ppvVar3 = ppvVar3 + 1;
        uVar5 = uVar5 + 1;
    } while (uVar5 <= 4);
    uVar5 = 5;
    iVar4 = 0x104;
    ppvVar3 = (void **)&param_1->windows[0].height;
    do {
        ppvVar3[0x19c] = SpriteSystem_NewSprite(param_1->unk_0668, param_1->unk_066C, (ManagedSpriteTemplate *)(ov18_021FAC28 + (iVar4 - 0x34)));
        uVar5 = uVar5 + 1;
        iVar4 = iVar4 + 0x34;
        ppvVar3 = ppvVar3 + 1;
    } while (uVar5 < 0x14);
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
