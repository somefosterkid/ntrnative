#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_resources_header.h"
#include "unk_02009D48.h"
#include "overlay_96_sprite_res_man.h"

Sprite *ov96_021EAA88(Ov96SpriteSet *set, s32 arg1, NNS_G2D_VRAM_TYPE screen, s32 resId) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader header;
    SpriteTemplate template;

    charMan = ov96_021E9D08(set->resSrc, 0);
    plttMan = ov96_021E9D08(set->resSrc, 1);
    cellMan = ov96_021E9D08(set->resSrc, 2);
    cellAnmMan = ov96_021E9D08(set->resSrc, 3);
    CreateSpriteResourcesHeader(&header, resId, 0x17, resId, resId, -1, -1, 0, arg1, charMan, plttMan, cellMan, cellAnmMan, NULL, NULL);
    if (screen == NNS_G2D_VRAM_TYPE_2DBOTH) {
        screen = NNS_G2D_VRAM_TYPE_2DMAIN;
    }
    template.spriteList = set->spriteList;
    template.header = &header;
    template.heapID = set->heapID;
    template.position.x = 0;
    template.position.y = 0;
    template.position.z = 0;
    template.scale.x = 0x1000;
    template.scale.y = 0x1000;
    template.scale.z = 0x1000;
    template.rotation = 0;
    template.drawPriority = 0;
    template.whichScreen = screen;
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

void ov96_021EAB38(Ov96SpriteSetElem *elem, s32 flag) {
    Sprite *v0;
    Sprite *v1;

    Sprite_SetDrawFlag(ov96_021E8BAC(elem->entry), flag);
    v0 = elem->spriteA;
    if (v0 != NULL) {
        if (elem->hideSpriteA != 0) {
            Sprite_SetDrawFlag(v0, 0);
        } else {
            Sprite_SetDrawFlag(v0, flag);
        }
    }
    v1 = elem->spriteB;
    if (v1 != NULL) {
        Sprite_SetDrawFlag(v1, flag);
    }
}

void ov96_021EAB74(Ov96SpriteSetElem *elem, s32 flag) {
    Sprite *v0;

    v0 = elem->spriteA;
    if (v0 != NULL) {
        if (elem->hideSpriteA != 0) {
            Sprite_SetDrawFlag(v0, 0);
            return;
        }
        Sprite_SetDrawFlag(v0, flag);
    }
}

void ov96_021EAB94(Ov96SpriteSetElem *elem, s32 flag) {
    Sprite_SetDrawFlag(ov96_021E8BAC(elem->entry), flag);
}

void ov96_021EABA8(Ov96SpriteSetElem *elem, u32 priority) {
    Sprite *v0;
    Sprite *v1;

    Sprite_SetDrawPriority(ov96_021E8BAC(elem->entry), priority);
    v0 = elem->spriteA;
    if (v0 != NULL) {
        Sprite_SetDrawPriority(v0, priority + (elem->priorityStep * 2));
    }
    v1 = elem->spriteB;
    if (v1 != NULL) {
        Sprite_SetDrawPriority(v1, priority + elem->priorityStep);
    }
}

void ov96_021EABDC(Ov96SpriteSetElem *elem, s32 step) {
    elem->priorityStep = step;
}

void ov96_021EABE0(Ov96SpriteSetElem *elem, u8 affine) {
    Sprite_SetAffineOverwriteMode(ov96_021E8BAC(elem->entry), affine);
}

void ov96_021EABF4(Ov96SpriteSetElem *elem, VecFx32 *vec) {
    Sprite_SetAffineMatrix(ov96_021E8BAC(elem->entry), vec);
}

void ov96_021EAC08(Ov96SpriteSetElem *elem, s32 state) {
    elem->unk_34 = state;
}

void ov96_021EAC0C(Ov96SpriteSetElem *elem, s32 state) {
    Sprite *sprite;
    s32 seq;

    elem->unk_34 = state;
    if (state != 0) {
        sprite = ov96_021E8BAC(elem->entry);
        switch (state) {
        case 1:
            seq = 0;
            break;
        case 2:
            seq = 1;
            break;
        case 3:
            seq = 2;
            break;
        case 4:
            seq = 3;
            break;
        case 5:
            seq = 4;
            break;
        case 6:
            seq = 8;
            break;
        default:
            GF_AssertFail();
            return;
        }
        Sprite_TryChangeAnimSeq(sprite, seq);
    }
    return;
}
