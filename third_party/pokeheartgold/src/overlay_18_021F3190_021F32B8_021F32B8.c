#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F1424(PokedexAppData *, u32);

extern ManagedSpriteTemplate ov18_021FA4EC;

extern u32 ov18_021FA520;

extern ManagedSpriteTemplate ov18_021FB004[];

extern u32 ov18_021FB54C;

extern u32 ov18_021FB580;

void ov18_021F32B8(PokedexAppData *param_1);

void ov18_021F32B8(PokedexAppData *param_1) {
    int sVar1;
    ManagedSprite *pMVar2;
    u32 remainder;
    u16 uVar7;
    u16 a2;
    ManagedSpriteTemplate MStack_48;

    uVar7 = 0;
    do {
        pMVar2 = SpriteSystem_NewSprite(param_1->unk_0668, param_1->unk_066C, &ov18_021FB004[uVar7]);
        param_1->unk_0670[uVar7] = pMVar2;
        ov18_021F11C0(param_1, uVar7, 0);
        uVar7 = uVar7 + 1;
    } while (uVar7 <= 0x19);
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, (ManagedSpriteTemplate *)&ov18_021FA520, FX32_CONST(512));
    param_1->unk_0670[43] = pMVar2;
    ov18_021F11C0(param_1, 0x2b, 0);
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, (ManagedSpriteTemplate *)&ov18_021FB54C, FX32_CONST(512));
    param_1->unk_0670[26] = pMVar2;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, (ManagedSpriteTemplate *)&ov18_021FB580, FX32_CONST(512));
    param_1->unk_0670[27] = pMVar2;
    ov18_021F11C0(param_1, 0x1b, 0);
    MStack_48 = ov18_021FA4EC;
    uVar7 = 0x1c;
    do {
        if (uVar7 == 0x1c) {
            MStack_48.x = 0xe0;
            MStack_48.y = 0x48;
            pMVar2 = SpriteSystem_NewSprite(param_1->unk_0668, param_1->unk_066C, &MStack_48);
            param_1->unk_0670[uVar7] = pMVar2;
            if (param_1->dexSearchCriteria[9] == 0xe) {
                ov18_021F11C0(param_1, uVar7, 0);
            } else {
                ov18_021F118C(param_1, uVar7, param_1->dexSearchCriteria[9]);
            }
        } else {
            a2 = uVar7 - 0x1d;
            remainder = a2 % 5;
            MStack_48.x = remainder * 0x30 + 0x20;
            sVar1 = a2 / 5;
            MStack_48.y = sVar1 * 0x28 + 0x38;
            pMVar2 = SpriteSystem_NewSprite(param_1->unk_0668, param_1->unk_066C, &MStack_48);
            param_1->unk_0670[uVar7] = pMVar2;
            ov18_021F118C(param_1, uVar7, a2);
            ov18_021F11C0(param_1, uVar7, 0);
        }
        uVar7 = uVar7 + 1;
    } while (uVar7 <= 0x2A);
    ov18_021F1424(param_1, 0x3b);
    return;
}
