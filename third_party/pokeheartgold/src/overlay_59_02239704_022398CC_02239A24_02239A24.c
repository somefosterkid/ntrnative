#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "sprite.h"
#include "sprite_system.h"

extern u32 ov59_0223C6C4;

extern u32 ov59_0223C7F0;

extern u32 ov59_0223C840;

extern u32 ov59_0223C8B8;

extern u32 ov59_0223C8E0;

void ov59_02239A24(Ov59ApricornApp *param_1);

void ov59_02239A24(Ov59ApricornApp *param_1) {
    Sprite *pSVar1;
    s32 iVar2;
    s32 iVar3;
    s32 y;
    UnmanagedSpriteTemplate *template;
    s16 *psVar4;
    u32 iVar5;
    s32 iStack_18;

    template = (UnmanagedSpriteTemplate *)&ov59_0223C7F0;
    iVar5 = 0;
    do {
        pSVar1 = SpriteSystem_CreateSpriteFromResourceHeader(param_1->spriteSystem, param_1->spriteManager, template);
        param_1->sprites[iVar5] = pSVar1;
        Sprite_SetDrawFlag(param_1->sprites[iVar5], 1);
        Sprite_SetAnimActiveFlag(param_1->sprites[iVar5], 1);
        template = template + 1;
        iVar5 = iVar5 + 1;
    } while ((s32)iVar5 <= 1);
    Sprite_SetPriority(param_1->sprites[1], 2);
    psVar4 = (s16 *)&ov59_0223C6C4;
    iVar2 = 0;
    do {
        iVar5 = (iVar2 + 2U) & 0xff;
        param_1->sprites[iVar5] = SpriteSystem_CreateSpriteFromResourceHeader(param_1->spriteSystem, param_1->spriteManager, (UnmanagedSpriteTemplate *)&ov59_0223C840);
        Sprite_SetPositionXY(param_1->sprites[iVar5], *psVar4, psVar4[1]);
        Sprite_SetAnimActiveFlag(param_1->sprites[iVar5], 0);
        Sprite_SetDrawFlag(param_1->sprites[iVar5], 1);
        Sprite_SetAnimCtrlSeq(param_1->sprites[iVar5], iVar2 + 9);
        Sprite_SetPriority(param_1->sprites[iVar5], 2);
        psVar4 = psVar4 + 2;
        iVar2 = iVar2 + 1;
    } while (iVar2 < 7);
    switch (param_1->mode) {
    case 1:
        iVar2 = 0;
        do {
            iVar5 = (iVar2 + 9U) & 0xff;
            iVar3 = iVar2 + 3;
            param_1->sprites[iVar5] = SpriteSystem_CreateSpriteFromResourceHeader(param_1->spriteSystem, param_1->spriteManager, (UnmanagedSpriteTemplate *)&ov59_0223C7F0 + iVar3);
            Sprite_SetDrawFlag(param_1->sprites[iVar5], 1);
            Sprite_SetAnimActiveFlag(param_1->sprites[iVar5], 1);
            iVar2 = iVar2 + 1;
        } while (iVar2 < 2);
        Sprite_SetAnimCtrlSeq(param_1->sprites[9], param_1->unk_14 + 0x10);
        Sprite_SetPaletteOverride(param_1->sprites[9], param_1->unk_17 + 5);
        break;
    case 2:
        iVar2 = 0;
        y = 0x70;
        iStack_18 = 0x1c;
        do {
            iVar5 = (iVar2 + 9U) & 0xff;
            param_1->sprites[iVar5] = SpriteSystem_CreateSpriteFromResourceHeader(param_1->spriteSystem, param_1->spriteManager, (UnmanagedSpriteTemplate *)&ov59_0223C8B8);
            Sprite_SetPositionXY(param_1->sprites[iVar5], 0xa0, (s16)y);
            Sprite_SetDrawFlag(param_1->sprites[iVar5], 0);
            Sprite_SetAnimActiveFlag(param_1->sprites[iVar5], 1);
            Sprite_SetAnimCtrlSeq(param_1->sprites[iVar5], iStack_18);
            Sprite_SetPriority(param_1->sprites[iVar5], 1);
            y = y + 0x18;
            iStack_18 = iStack_18 + 2;
            iVar2 = iVar2 + 1;
        } while (iVar2 < 2);
        iVar2 = 0;
        do {
            iVar3 = (iVar2 + 0xbU) & 0xff;
            param_1->sprites[iVar3] = SpriteSystem_CreateSpriteFromResourceHeader(param_1->spriteSystem, param_1->spriteManager, (UnmanagedSpriteTemplate *)&ov59_0223C8E0);
            iVar5 = iVar2 % 2;
            Sprite_SetPositionXY(param_1->sprites[iVar3],
                (s16)((iVar2 / 2) * -0x16 + 0x67),
                (s16)(iVar5 * 0x28 + 0x60));
            Sprite_SetDrawFlag(param_1->sprites[iVar3], 0);
            Sprite_SetAnimActiveFlag(param_1->sprites[iVar3], 0);
            Sprite_SetAnimCtrlSeq(param_1->sprites[iVar3], iVar5 * 2 + 0x18);
            Sprite_SetPriority(param_1->sprites[iVar3], 1);
            iVar2 = iVar2 + 1;
        } while (iVar2 < 4);
        break;
    }
}
