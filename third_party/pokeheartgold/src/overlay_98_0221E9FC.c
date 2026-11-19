#include "global.h"

#include "error_handling.h"
#include "overlay_98.h"
#include "sprite.h"

void ov98_0221E9FC(Sprite **arg0, void *pSrc, u32 szByte) {
    enum NNS_G2D_VRAM_TYPE type;
    u32 offset;

    type = Sprite_GetVramType(*arg0);
    offset = NNS_G2dGetImageLocation(Sprite_GetImageProxy(*arg0), type);
    DC_FlushRange(pSrc, szByte);
    switch (type) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ(pSrc, offset, szByte);
        return;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ(pSrc, offset, szByte);
        return;
    default:
        GF_AssertFail();
        return;
    }
}
