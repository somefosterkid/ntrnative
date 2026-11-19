#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_112_sprite_app.h"

void ov112_021F1D58(Ov112SpriteApp *arg0, u32 arg1);

void ov112_021F1D58(Ov112SpriteApp *arg0, u32 seqno) {
    if (seqno >= 8U) {
        GF_AssertFail();
    }
    ManagedSprite_SetAnimNoRestart(arg0->sprites[3], (s32)seqno);
}

void ov112_021F1D70(Sprite *arg0, void *arg1, u32 arg2, s32 arg3);

void ov112_021F1D70(Sprite *sprite, void *pSrc, u32 arg2, s32 arg3) {
    u32 szByte;
    enum NNS_G2D_VRAM_TYPE type;
    u32 v0;

    szByte = arg2;
    type = Sprite_GetVramType(sprite);
    v0 = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite), type);
    DC_FlushRange(pSrc, szByte);
    switch (type) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ(pSrc, v0 + arg3, szByte);
        return;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ(pSrc, v0 + arg3, szByte);
        return;
    default:
        GF_AssertFail();
        return;
    }
}
