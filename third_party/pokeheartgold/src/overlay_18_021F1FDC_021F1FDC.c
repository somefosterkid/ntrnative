#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "application/pokedex/pokedex_internal.h"

extern ManagedSpriteTemplate ov18_021FA41C;

void ov18_021F1FDC(PokedexAppData *param_1, s32 param_2);


#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F1FDC(PokedexAppData *param_1, s32 param_2) {
    ManagedSprite *pMVar2;
    ManagedSpriteTemplate aMStack_48;

    aMStack_48 = ov18_021FA41C;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &aMStack_48, FX32_CONST(512));
    param_1->unk_0670[param_2] = pMVar2;
    aMStack_48.resIdList[0] = 0xc595;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &aMStack_48, FX32_CONST(512));
    param_1->unk_0670[param_2 + 2] = pMVar2;
    aMStack_48.x = aMStack_48.x + 0x31;
    aMStack_48.resIdList[0] = 0xc594;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &aMStack_48, FX32_CONST(512));
    param_1->unk_0670[param_2 + 1] = pMVar2;
    aMStack_48.resIdList[0] = 0xc596;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &aMStack_48, FX32_CONST(512));
    param_1->unk_0670[param_2 + 3] = pMVar2;
    return;
}
#else
void ov18_021F1FDC(PokedexAppData *param_1, s32 param_2) {
    ManagedSprite *pMVar2;
    u32 iVar4;
    ManagedSpriteTemplate aMStack_48;

    aMStack_48 = ov18_021FA41C;
    iVar4 = param_2 * 4;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &aMStack_48, FX32_CONST(512));
    *(ManagedSprite **)((u8 *)param_1 + (iVar4 + offsetof(PokedexAppData, unk_0670[0]))) = pMVar2;
    aMStack_48.resIdList[0] = 0xc595;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &aMStack_48, FX32_CONST(512));
    *(ManagedSprite **)((u8 *)param_1 + (iVar4 + offsetof(PokedexAppData, unk_0670[2]))) = pMVar2;
    aMStack_48.x = aMStack_48.x + 0x31;
    aMStack_48.resIdList[0] = 0xc594;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &aMStack_48, FX32_CONST(512));
    *(ManagedSprite **)((u8 *)param_1 + (iVar4 + offsetof(PokedexAppData, unk_0670[1]))) = pMVar2;
    aMStack_48.resIdList[0] = 0xc596;
    pMVar2 = SpriteSystem_NewSpriteWithYOffset(param_1->unk_0668, param_1->unk_066C, &aMStack_48, FX32_CONST(512));
    *(ManagedSprite **)((u8 *)param_1 + iVar4 + offsetof(PokedexAppData, unk_0670[3])) = pMVar2;
    return;
}
#endif
