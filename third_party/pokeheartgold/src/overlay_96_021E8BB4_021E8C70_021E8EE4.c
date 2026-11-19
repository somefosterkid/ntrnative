#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_96_sprite_res_man.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021E8EE4(Ov96SpriteResMan *man, s32 param_2, s32 param_3, NNS_G2D_VRAM_TYPE param_4, u8 param_5, u8 param_6) {
    Sprite *pSVar1;
    u32 charId;
    SpriteResourcesHeader SStack_34;
    SpriteTemplate SStack_64;

    charId = (u32)param_5;
    CreateSpriteResourcesHeader(&SStack_34, charId, charId, charId, charId, -1, -1, param_3, param_2, man->resMan[0], man->resMan[1], man->resMan[2], man->resMan[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    if (param_4 == NNS_G2D_VRAM_TYPE_2DBOTH) {
        param_4 = NNS_G2D_VRAM_TYPE_2DMAIN;
    }
    SStack_64.spriteList = man->spriteList;
    SStack_64.header = &SStack_34;
    SStack_64.heapID = man->heapID;
    SStack_64.position.x = 0;
    SStack_64.position.y = 0;
    SStack_64.position.z = 0;
    SStack_64.scale.x = 0x1000;
    SStack_64.scale.y = 0x1000;
    SStack_64.scale.z = 0x1000;
    SStack_64.rotation = 0;
    SStack_64.drawPriority = 0;
    SStack_64.whichScreen = param_4;
    pSVar1 = Sprite_CreateAffine(&SStack_64);
    man->entries[param_6].sprite = pSVar1;
    if (man->entries[param_6].sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(man->entries[param_6].sprite, 1);
    Sprite_SetAnimCtrlSeq(man->entries[param_6].sprite, 0);
    Sprite_SetDrawFlag(man->entries[param_6].sprite, 0);
    return;
}
#else
void ov96_021E8EE4(Ov96SpriteResMan *man, s32 param_2, s32 param_3, NNS_G2D_VRAM_TYPE param_4, u8 param_5, u8 param_6) {
    enum HeapID *param_1 = (enum HeapID *)man;
    Sprite *pSVar1;
    u32 charId;
    s32 iVar2;
    SpriteResourcesHeader SStack_34;
    SpriteTemplate SStack_64;

    charId = (u32)param_5;
    CreateSpriteResourcesHeader(&SStack_34, charId, charId, charId, charId, -1, -1, param_3, param_2, (GF_2DGfxResMan *)param_1[0x51], (GF_2DGfxResMan *)param_1[0x52], (GF_2DGfxResMan *)param_1[0x53], (GF_2DGfxResMan *)param_1[0x54], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    if (param_4 == NNS_G2D_VRAM_TYPE_2DBOTH) {
        param_4 = NNS_G2D_VRAM_TYPE_2DMAIN;
    }
    SStack_64.spriteList = (SpriteList *)param_1[6];
    SStack_64.header = &SStack_34;
    SStack_64.heapID = *param_1;
    SStack_64.position.x = 0;
    SStack_64.position.y = 0;
    SStack_64.position.z = 0;
    SStack_64.scale.x = 0x1000;
    SStack_64.scale.y = 0x1000;
    SStack_64.scale.z = 0x1000;
    SStack_64.rotation = 0;
    SStack_64.drawPriority = 0;
    SStack_64.whichScreen = param_4;
    iVar2 = (u32)param_6 * 0x2c;
    pSVar1 = Sprite_CreateAffine(&SStack_64);
    *(Sprite **)(param_1[0x55] + iVar2 + 0x10) = pSVar1;
    if (*(s32 *)(param_1[0x55] + iVar2 + 0x10) == 0) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(*(Sprite **)(param_1[0x55] + iVar2 + 0x10), 1);
    Sprite_SetAnimCtrlSeq(*(Sprite **)(param_1[0x55] + iVar2 + 0x10), 0);
    Sprite_SetDrawFlag(*(Sprite **)(param_1[0x55] + iVar2 + 0x10), 0);
    return;
}
#endif
