#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "sprite.h"

void ov70_02238B54(SpriteTemplate *, GTSApplicationState *, SpriteResourcesHeader *, u32);

void ov70_02238F9C(Sprite *, u32, s32);

void ov70_022410F0(Sprite *, int, int);

void ov70_0224127C(GTSApplicationState *);

extern u32 ov70_02245D0A;

void ov70_02240D74(GTSApplicationState *param_1, s32 param_2);

void ov70_02240D74(GTSApplicationState *param_1, s32 param_2) {
    Sprite *pSVar1;
    VecFx32 *pVVar2;
    s32 iVar3;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u8 *walk;
#endif
    u16 *puVar4;
    s32 seq;
    s32 iStack_48;
    SpriteTemplate SStack_44;

    ov70_0224127C(param_1);
    ov70_02238B54(&SStack_44, param_1, &param_1->avatarSpriteResourceHeader, 2);
    SStack_44.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    SStack_44.position.x = 0x80000;
    SStack_44.position.y = 0x182000;
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    param_1->avatarSprites[0] = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->avatarSprites[0], 1);
    Sprite_SetPriority(param_1->avatarSprites[0], 2);
    Sprite_SetAnimCtrlSeq(param_1->avatarSprites[0], param_2 * 7 + 3);
    Sprite_SetDrawFlag(param_1->avatarSprites[0], 1);
    puVar4 = (u16 *)&ov70_02245D0A;
    iStack_48 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    seq = 0xe;
    do {
        pSVar1 = Sprite_CreateAffine(&SStack_44);
        param_1->avatarSprites[iStack_48 + 1] = pSVar1;
        Sprite_SetAnimActiveFlag(param_1->avatarSprites[iStack_48 + 1], 1);
        Sprite_SetAnimCtrlSeq(param_1->avatarSprites[iStack_48 + 1], seq);
        Sprite_SetDrawFlag(param_1->avatarSprites[iStack_48 + 1], 0);
        ov70_022410F0(param_1->avatarSprites[iStack_48 + 1], *puVar4, puVar4[1]);
        Sprite_SetPriority(param_1->avatarSprites[iStack_48 + 1], 2);
        seq = seq + 4;
        puVar4 = puVar4 + 2;
        iStack_48 = iStack_48 + 1;
    } while (iStack_48 < 7);
#else
    // the DS build walks a byte pointer over avatarSprites[1..7] (no indexed form found that matches)
    walk = (u8 *)param_1;
    seq = 0xe;
    do {
        pSVar1 = Sprite_CreateAffine(&SStack_44);
        *(Sprite **)(walk + 0xee8) = pSVar1;
        Sprite_SetAnimActiveFlag(*(Sprite **)(walk + 0xee8), 1);
        Sprite_SetAnimCtrlSeq(*(Sprite **)(walk + 0xee8), seq);
        Sprite_SetDrawFlag(*(Sprite **)(walk + 0xee8), 0);
        ov70_022410F0(*(Sprite **)(walk + 0xee8), *puVar4, puVar4[1]);
        Sprite_SetPriority(*(Sprite **)(walk + 0xee8), 2);
        walk = walk + 4;
        seq = seq + 4;
        puVar4 = puVar4 + 2;
        iStack_48 = iStack_48 + 1;
    } while (iStack_48 < 7);
#endif
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    param_1->unk_F0C = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->unk_F0C, 1);
    Sprite_SetAnimCtrlSeq(param_1->unk_F0C, 0x2b);
    Sprite_SetDrawFlag(param_1->unk_F0C, 0);
    ov70_022410F0(param_1->unk_F0C, 0x80, 0x56);
    Sprite_SetPriority(param_1->unk_F0C, 1);
    pSVar1 = Sprite_CreateAffine(&SStack_44);
    param_1->unk_F10 = pSVar1;
    Sprite_SetAnimActiveFlag(param_1->unk_F10, 1);
    Sprite_SetAnimCtrlSeq(param_1->unk_F10, 0x2a);
    ov70_02238F9C(param_1->unk_F10, 0x37, param_1->unk_F14 + 0x1a8);
    Sprite_SetDrawFlag(param_1->unk_F10, 0);
    iVar3 = 0;
    do {
        pVVar2 = Sprite_GetMatrixPtr(param_1->avatarSprites[iVar3]);
        param_1->avatarPositions[iVar3][0] = (s16)(pVVar2->x / 0x1000);
        param_1->avatarPositions[iVar3][1] = (s16)((s32)(pVVar2->y + ((u32)(pVVar2->y >> 0xb) >> 0x14)) >> 0xc);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 8);
    return;
}
