#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_96_sprite_res_man.h"
#include "sprite.h"
#include "unk_02009D48.h"

Sprite *ov96_021EA214(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, NNS_G2D_VRAM_TYPE param_4, enum HeapID param_5);

Sprite *ov96_021EA214(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, NNS_G2D_VRAM_TYPE param_4, enum HeapID param_5) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader SStack_38;
    SpriteTemplate SStack_68;

    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_38, 0x14, 0x17, 0x14, 0x14, -1, -1, 0, param_3, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_68.spriteList = param_2;
    SStack_68.header = &SStack_38;
    SStack_68.heapID = param_5;
    SStack_68.position.x = 0;
    SStack_68.position.y = 0;
    SStack_68.position.z = 0;
    SStack_68.scale.x = 0x1000;
    SStack_68.scale.y = 0x1000;
    SStack_68.scale.z = 0x1000;
    SStack_68.rotation = 0;
    SStack_68.drawPriority = 0;
    SStack_68.whichScreen = param_4;
    sprite = Sprite_CreateAffine(&SStack_68);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

Sprite *ov96_021EA2C4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite *ov96_021EA2C4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader SStack_38;
    SpriteTemplate SStack_68;

    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_38, 0x19, 0x18, 0x19, 0x19, -1, -1, 0, param_3, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_68.spriteList = param_2;
    SStack_68.heapID = param_4;
    SStack_68.header = &SStack_38;
    SStack_68.position.x = 0;
    SStack_68.position.y = 0;
    SStack_68.position.z = 0;
    SStack_68.scale.x = 0x1000;
    SStack_68.scale.y = 0x1000;
    SStack_68.scale.z = 0x1000;
    SStack_68.rotation = 0;
    SStack_68.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    SStack_68.drawPriority = 0;
    sprite = Sprite_CreateAffine(&SStack_68);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

Sprite *ov96_021EA374(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite *ov96_021EA374(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader SStack_38;
    SpriteTemplate SStack_68;

    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_38, 0x1c, 0x17, 0x1c, 0x1c, -1, -1, 0, param_3, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_68.spriteList = param_2;
    SStack_68.heapID = param_4;
    SStack_68.header = &SStack_38;
    SStack_68.position.x = 0;
    SStack_68.position.y = 0;
    SStack_68.position.z = 0;
    SStack_68.scale.x = 0x1000;
    SStack_68.scale.y = 0x1000;
    SStack_68.scale.z = 0x1000;
    SStack_68.rotation = 0;
    SStack_68.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_68.drawPriority = 0;
    sprite = Sprite_CreateAffine(&SStack_68);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

Sprite *ov96_021EA424(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite *ov96_021EA424(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader SStack_38;
    SpriteTemplate SStack_68;

    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_38, 0x1a, 0x17, 0x1a, 0x1a, -1, -1, 0, param_3, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_68.spriteList = param_2;
    SStack_68.heapID = param_4;
    SStack_68.header = &SStack_38;
    SStack_68.position.x = 0;
    SStack_68.position.y = 0;
    SStack_68.position.z = 0;
    SStack_68.scale.x = 0x1000;
    SStack_68.scale.y = 0x1000;
    SStack_68.scale.z = 0x1000;
    SStack_68.rotation = 0;
    SStack_68.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_68.drawPriority = 0;
    sprite = Sprite_CreateAffine(&SStack_68);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

Sprite *ov96_021EA4D4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite *ov96_021EA4D4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader SStack_38;
    SpriteTemplate SStack_68;

    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_38, 0x1b, 0x17, 0x1b, 0x1b, -1, -1, 0, param_3, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_68.spriteList = param_2;
    SStack_68.heapID = param_4;
    SStack_68.header = &SStack_38;
    SStack_68.position.x = 0;
    SStack_68.position.y = 0;
    SStack_68.position.z = 0;
    SStack_68.scale.x = 0x1000;
    SStack_68.scale.y = 0x1000;
    SStack_68.scale.z = 0x1000;
    SStack_68.rotation = 0;
    SStack_68.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_68.drawPriority = 0;
    sprite = Sprite_CreateAffine(&SStack_68);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

Sprite *ov96_021EA584(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite *ov96_021EA584(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader SStack_38;
    SpriteTemplate SStack_68;

    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_38, 0x1d, 0x17, 0x1d, 0x1d, -1, -1, 0, param_3, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_68.spriteList = param_2;
    SStack_68.heapID = param_4;
    SStack_68.header = &SStack_38;
    SStack_68.position.x = 0;
    SStack_68.position.y = 0;
    SStack_68.position.z = 0;
    SStack_68.scale.x = 0x1000;
    SStack_68.scale.y = 0x1000;
    SStack_68.scale.z = 0x1000;
    SStack_68.rotation = 0;
    SStack_68.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    SStack_68.drawPriority = 0;
    sprite = Sprite_CreateAffine(&SStack_68);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

Sprite *ov96_021EA634(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite *ov96_021EA634(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader SStack_38;
    SpriteTemplate SStack_68;

    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_38, 0x1e, 0x18, 0x1e, 0x1e, -1, -1, 0, param_3, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_68.spriteList = param_2;
    SStack_68.heapID = param_4;
    SStack_68.header = &SStack_38;
    SStack_68.position.x = 0;
    SStack_68.position.y = 0;
    SStack_68.position.z = 0;
    SStack_68.scale.x = 0x1000;
    SStack_68.scale.y = 0x1000;
    SStack_68.scale.z = 0x1000;
    SStack_68.rotation = 0;
    SStack_68.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    SStack_68.drawPriority = 0;
    sprite = Sprite_CreateAffine(&SStack_68);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

Sprite *ov96_021EA6E4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, NNS_G2D_VRAM_TYPE param_3, s32 param_4, enum HeapID param_5);

Sprite *ov96_021EA6E4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, NNS_G2D_VRAM_TYPE param_3, s32 param_4, enum HeapID param_5) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    s32 plttId;
    SpriteResourcesHeader SStack_3c;
    SpriteTemplate SStack_6c;

    if (param_3 == NNS_G2D_VRAM_TYPE_2DMAIN) {
        plttId = 0x17;
    } else {
        plttId = 0x18;
    }
    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_3c, 0x1f, plttId, 0x1f, 0x1f, -1, -1, 0, param_4, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_6c.spriteList = param_2;
    SStack_6c.header = &SStack_3c;
    SStack_6c.heapID = param_5;
    SStack_6c.position.x = 0;
    SStack_6c.position.y = 0;
    SStack_6c.position.z = 0;
    SStack_6c.scale.x = 0x1000;
    SStack_6c.scale.y = 0x1000;
    SStack_6c.scale.z = 0x1000;
    SStack_6c.rotation = 0;
    SStack_6c.drawPriority = 1;
    SStack_6c.whichScreen = param_3;
    sprite = Sprite_CreateAffine(&SStack_6c);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}

Sprite *ov96_021EA7A4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4);

Sprite *ov96_021EA7A4(UnkStruct_ov96_021E9A78 *param_1, SpriteList *param_2, s32 param_3, enum HeapID param_4) {
    GF_2DGfxResMan *charMan;
    GF_2DGfxResMan *plttMan;
    GF_2DGfxResMan *cellMan;
    GF_2DGfxResMan *cellAnmMan;
    Sprite *sprite;
    SpriteResourcesHeader SStack_38;
    SpriteTemplate SStack_68;

    charMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 0);
    plttMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 1);
    cellMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 2);
    cellAnmMan = (GF_2DGfxResMan *)ov96_021E9D08(param_1, 3);
    CreateSpriteResourcesHeader(&SStack_38, 0x20, 0x17, 0x20, 0x20, -1, -1, 0, param_3, charMan, plttMan, cellMan, cellAnmMan, (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_68.spriteList = param_2;
    SStack_68.heapID = param_4;
    SStack_68.header = &SStack_38;
    SStack_68.position.x = 0;
    SStack_68.position.y = 0;
    SStack_68.position.z = 0;
    SStack_68.scale.x = 0x1000;
    SStack_68.scale.y = 0x1000;
    SStack_68.scale.z = 0x1000;
    SStack_68.rotation = 0;
    SStack_68.drawPriority = 2;
    SStack_68.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    sprite = Sprite_CreateAffine(&SStack_68);
    if (sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_SetAnimActiveFlag(sprite, 1);
    Sprite_SetAnimCtrlSeq(sprite, 0);
    Sprite_SetDrawFlag(sprite, 0);
    return sprite;
}
