#include "global.h"

#include "error_handling.h"
#include "sprite.h"

void ov96_021EED14(Sprite *arg0, void *arg1, u32 arg2, s32 arg3);

void ov96_021EED14(Sprite *sprite, void *pSrc, u32 arg2, s32 arg3) {
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
