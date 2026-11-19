#include "global.h"

#include "constants/heap.h"

#include "sprite.h"
#include "trainer_card_main.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

extern u32 ov51_021E801C;

void ov51_021E7AF4(TrainerCardMainSprites *sprites, const u8 *badgeFlags, int showKantoBadges);

void ov51_021E7AF4(TrainerCardMainSprites *sprites, const u8 *badgeFlags, int showKantoBadges) {
    Sprite *pSVar1;
    s32 seq;
    s32 *piVar2;
    s32 iVar3;
    SpriteResourcesHeader SStack_3c;
    SpriteTemplate SStack_6c;
    CreateSpriteResourcesHeader(&SStack_3c, 0, 0, 0, 0, -1, -1, 0, 1, sprites->resourceManagers[0][0], sprites->resourceManagers[0][1], sprites->resourceManagers[0][2], sprites->resourceManagers[0][3], NULL, NULL);
    SStack_6c.spriteList = sprites->spriteList;
    iVar3 = 0;
    SStack_6c.header = &SStack_3c;
    SStack_6c.position.x = 0;
    SStack_6c.position.y = 0;
    SStack_6c.position.z = 0;
    SStack_6c.scale.x = 0x1000;
    SStack_6c.scale.y = 0x1000;
    SStack_6c.scale.z = 0x1000;
    SStack_6c.rotation = 0;
    SStack_6c.drawPriority = 2;
    SStack_6c.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_6c.heapID = HEAP_ID_25;
    if (showKantoBadges == 0) {
        iVar3 = 0x28;
        piVar2 = (s32 *)(&ov51_021E801C);
    } else {
    }
    seq = 0;
    do {
        SStack_6c.position.x = ((s32 *)&ov51_021E801C)[seq * 2] << 0xc;
        SStack_6c.position.y = (iVar3 + ((s32 *)&ov51_021E801C)[seq * 2 + 1]) * 0x1000;
        pSVar1 = Sprite_CreateAffine(&SStack_6c);
        sprites->badgeSprites[seq] = pSVar1;
        Sprite_SetAnimActiveFlag(sprites->badgeSprites[seq], 1);
        Sprite_SetAnimCtrlSeq(sprites->badgeSprites[seq], seq);
        if ((badgeFlags[seq] == 0) || ((showKantoBadges == 0) && (7 < seq))) {
            Sprite_SetDrawFlag(sprites->badgeSprites[seq], 0);
        }
        seq = seq + 1;
    } while (seq < 0x10);
    return;
}

extern u32 _021E80A0;

extern u32 ov51_021E80A4;

void ov51_021E7BD0(TrainerCardMainSprites *sprites);

void ov51_021E7BD0(TrainerCardMainSprites *sprites) {
    Sprite *pSVar1;
    u8 *pbVar2;
    s32 iVar3;
    u8 *pbVar4;
    SpriteResourcesHeader SStack_3c;
    SpriteTemplate SStack_6c;

    CreateSpriteResourcesHeader(&SStack_3c, 1, 1, 1, 1, -1, -1, 0, 1, sprites->resourceManagers[1][0], sprites->resourceManagers[1][1], sprites->resourceManagers[1][2], sprites->resourceManagers[1][3], NULL, NULL);
    SStack_6c.spriteList = sprites->spriteList;
    iVar3 = 0;
    SStack_6c.header = &SStack_3c;
    SStack_6c.position.x = 0;
    SStack_6c.position.y = 0;
    SStack_6c.position.z = 0;
    SStack_6c.scale.x = 0x1000;
    SStack_6c.scale.y = 0x1000;
    SStack_6c.scale.z = 0x1000;
    SStack_6c.rotation = 0;
    SStack_6c.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    pbVar2 = (unsigned char *)(&ov51_021E80A4);
    pbVar4 = (unsigned char *)(&_021E80A0);
    SStack_6c.drawPriority = 0;
    SStack_6c.heapID = HEAP_ID_25;
    do {
        SStack_6c.position.x = (u32)*pbVar2 << 0xc;
        SStack_6c.position.y = (u32)pbVar2[1] * 0x1000 + 0xe0000;
        pSVar1 = Sprite_CreateAffine(&SStack_6c);
        sprites->buttonSprites[iVar3] = pSVar1;
        Sprite_SetAnimActiveFlag(sprites->buttonSprites[iVar3], 1);
        Sprite_SetAnimCtrlSeq(sprites->buttonSprites[iVar3], ((u8 *)&_021E80A0)[iVar3]);
        Sprite_SetDrawPriority(sprites->buttonSprites[iVar3], 2 - iVar3);
        Sprite_SetDrawFlag(sprites->buttonSprites[iVar3], 0);
        pbVar2 = pbVar2 + 2;
        iVar3 = iVar3 + 1;
    } while (iVar3 < 2);
    return;
}
