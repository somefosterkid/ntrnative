#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

extern u8 ov14_021F8174[];

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F4380(UnkStruct_ov14_boxsys *param_1);

void ov14_021F4380(UnkStruct_ov14_boxsys *param_1) {
    ManagedSprite *pMVar2;
    u32 uVar10;
    s32 sVar8;

    ManagedSpriteTemplate MStack_48;
    ManagedSpriteTemplate aiStack_7c;

    uVar10 = 0;
    sVar8 = 0;

    aiStack_7c = *(ManagedSpriteTemplate *)ov14_021F8174;
    do {
        MStack_48 = aiStack_7c;
        MStack_48.x = MStack_48.x + sVar8;
        MStack_48.resIdList[0] = uVar10 + 0xc129;
        pMVar2 = SpriteSystem_NewSprite(param_1->unk_2F4, param_1->unk_2F8, &MStack_48);
        param_1->sprites[21 + uVar10] = pMVar2;
        sVar8 = sVar8 + 0x2e;
        uVar10 = uVar10 + 1;

    } while (uVar10 < 4);
    return;
}
#else
void ov14_021F4380(u8 *param_1);

void ov14_021F4380(u8 *param_1) {
    ManagedSprite *pMVar2;
    u32 uVar10;
    s32 sVar8;

    u8 *pp;
    ManagedSpriteTemplate MStack_48;
    ManagedSpriteTemplate aiStack_7c;

    uVar10 = 0;
    sVar8 = 0;
    pp = param_1;

    aiStack_7c = *(ManagedSpriteTemplate *)ov14_021F8174;
    do {
        MStack_48 = aiStack_7c;
        MStack_48.x = MStack_48.x + sVar8;
        MStack_48.resIdList[0] = uVar10 + 0xc129;
        pMVar2 = SpriteSystem_NewSprite(*(SpriteSystem **)(param_1 + 0x2f4), ((SpriteManager *)((UnkStruct_ov14_boxsys *)param_1)->unk_2F8), &MStack_48);
        *(ManagedSprite **)(pp + 0x350) = pMVar2;
        sVar8 = sVar8 + 0x2e;
        pp = pp + 4;
        uVar10 = uVar10 + 1;

    } while (uVar10 < 4);
    return;
}
#endif
