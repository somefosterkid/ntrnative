#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

s16 _u32_div_f(u32, u32);

extern ManagedSpriteTemplate ov14_021F810C;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F2D1C(UnkStruct_ov14_boxsys *param_1);

void ov14_021F2D1C(UnkStruct_ov14_boxsys *param_1) {
    s32 sVar1;
    ManagedSprite *pMVar3;
    u32 uVar9;

    s32 iStack_80;
    ManagedSpriteTemplate MStack_48;
    ManagedSpriteTemplate aiStack_7c;

    uVar9 = 0;
    iStack_80 = 0;
    aiStack_7c = ov14_021F810C;
    do {
        MStack_48 = aiStack_7c;
        MStack_48.x = MStack_48.x + ((u32)uVar9 % (u32)6) * 0x18;
        sVar1 = (u32)uVar9 / (u32)6;
        MStack_48.y = MStack_48.y + sVar1 * 0x18;
        MStack_48.drawPriority = 0x74 - iStack_80;
        MStack_48.resIdList[0] = uVar9 + 0xc0f9;
        pMVar3 = SpriteSystem_NewSprite(param_1->unk_2F4, param_1->unk_2F8, &MStack_48);
        param_1->sprites[25 + uVar9] = pMVar3;
        param_1->unk_4094[uVar9] = uVar9 + 0x19;
        uVar9 = uVar9 + 1;
        iStack_80 = iStack_80 + 2;
    } while (uVar9 < 0x24);
    return;
}
#else
void ov14_021F2D1C(u32 param_1);

void ov14_021F2D1C(u32 param_1) {
    s32 sVar1;
    ManagedSprite *pMVar3;
    u32 uVar9;

    s32 iStack_80;
    u32 iVar10;
    ManagedSpriteTemplate MStack_48;
    ManagedSpriteTemplate aiStack_7c;

    uVar9 = 0;
    iStack_80 = 0;
    iVar10 = param_1;
    aiStack_7c = ov14_021F810C;
    do {
        MStack_48 = aiStack_7c;
        MStack_48.x = MStack_48.x + ((u32)uVar9 % (u32)6) * 0x18;
        sVar1 = (u32)uVar9 / (u32)6;
        MStack_48.y = MStack_48.y + sVar1 * 0x18;
        MStack_48.drawPriority = 0x74 - iStack_80;
        MStack_48.resIdList[0] = uVar9 + 0xc0f9;
        pMVar3 = SpriteSystem_NewSprite(((UnkStruct_ov14_boxsys *)param_1)->unk_2F4, ((SpriteManager *)((UnkStruct_ov14_boxsys *)param_1)->unk_2F8), &MStack_48);
        *(ManagedSprite **)(iVar10 + 0x360) = pMVar3;
        *(s8 *)(param_1 + uVar9 + 0x4094) = uVar9 + 0x19;
        uVar9 = uVar9 + 1;
        iStack_80 = iStack_80 + 2;
        iVar10 = iVar10 + 4;
    } while (uVar9 < 0x24);
    return;
}
#endif
