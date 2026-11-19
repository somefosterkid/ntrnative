#include "global.h"

#include "error_handling.h"
#include "sprite.h"

void ov111_021E62E0(Sprite **arg0, void *arg1, u32 arg2);

void ov111_021E62E0(Sprite **arg0, void *pSrc, u32 szByte) {
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
