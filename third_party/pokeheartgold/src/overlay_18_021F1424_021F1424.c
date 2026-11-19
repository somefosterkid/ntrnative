#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

extern u32 ov18_021FA3E8;

void ov18_021F1424(PokedexAppData *param_1, s32 param_2);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F1424(PokedexAppData *param_1, s32 param_2) {
    ManagedSprite *pMVar3;
    u32 uVar9;
    ManagedSpriteTemplate aMStack_4c;
    ManagedSpriteTemplate aiStack_80;

    uVar9 = 0;
    aiStack_80 = *(ManagedSpriteTemplate *)&ov18_021FA3E8;
    do {
        aMStack_4c = aiStack_80;
        aMStack_4c.resIdList[0] = uVar9 + 0xC550;
        pMVar3 = SpriteSystem_NewSprite(param_1->unk_0668, param_1->unk_066C, &aMStack_4c);
        param_1->unk_0670[param_2 + uVar9] = pMVar3;
        uVar9 = uVar9 + 1;
    } while (uVar9 < 0x3c);
    return;
}
#else
void ov18_021F1424(PokedexAppData *param_1, s32 param_2) {
    ManagedSprite *pMVar3;
    u32 uVar9;
    u8 *iVar10;
    ManagedSpriteTemplate aMStack_4c;
    ManagedSpriteTemplate aiStack_80;

    uVar9 = 0;
    iVar10 = (u8 *)param_1 + param_2 * 4;
    aiStack_80 = *(ManagedSpriteTemplate *)&ov18_021FA3E8;
    do {
        aMStack_4c = aiStack_80;
        aMStack_4c.resIdList[0] = uVar9 + 0xC550;
        pMVar3 = SpriteSystem_NewSprite(param_1->unk_0668, param_1->unk_066C, &aMStack_4c);
        *(ManagedSprite **)(iVar10 + offsetof(PokedexAppData, unk_0670)) = pMVar3;
        uVar9 = uVar9 + 1;
        iVar10 = iVar10 + 4;
    } while (uVar9 < 0x3c);
    return;
}
#endif
