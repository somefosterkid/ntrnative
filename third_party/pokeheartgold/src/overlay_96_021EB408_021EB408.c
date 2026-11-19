#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_96_ov96_sprite_mgr.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

Sprite *ov96_021EB408(Ov96SpriteMgr *mgr, s32 param_2, s32 vram, u8 resId, u8 group) {
    Sprite *sprite;
    SpriteResourcesHeader header;
    SpriteTemplate template;

    if (mgr->numSprites >= mgr->param.maxSprites) {
        GF_AssertFail();
    }
    CreateSpriteResourcesHeader(&header, resId, resId, resId, resId, -1, -1, 0, param_2, mgr->resMan[GF_GFX_RES_TYPE_CHAR], mgr->resMan[GF_GFX_RES_TYPE_PLTT], mgr->resMan[GF_GFX_RES_TYPE_CELL], mgr->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = mgr->spriteList;
    template.header = &header;
    template.heapID = mgr->heapID;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = 0x1000;
    template.scale.y = 0x1000;
    template.scale.z = 0x1000;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = (NNS_G2D_VRAM_TYPE)vram;
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    mgr->slots[mgr->numSprites].inUse = 0;
    mgr->slots[mgr->numSprites].unk_2 = 0;
    mgr->slots[mgr->numSprites].sprite = sprite;
    mgr->slots[mgr->numSprites].group = (u16)group;
    mgr->slots[mgr->numSprites].resId = (u16)resId;
    mgr->numSprites += 1;
    return sprite;
}
