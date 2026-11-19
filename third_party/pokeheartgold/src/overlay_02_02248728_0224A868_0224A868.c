#include "global.h"
struct HMCutIn;

#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

void ov02_0224A868(struct HMCutIn *arg0, NARC *arg1);

void ov02_0224A868(HMCutIn *arg0, NARC *narc) {
    AddPlttResObjFromOpenNarc(arg0->paletteLocation, narc, 6, 0, 3, 1, 1, HEAP_ID_FIELD1);
}

void ov02_0224A88C(struct HMCutIn *arg0, void *arg1);

void ov02_0224A88C(HMCutIn *arg0, void *arg1) {
    NNSG2dImageProxy *imageProxy;
    u32 offset;

    imageProxy = SpriteTransfer_GetCharProxy(SpriteResourceCollection_Find(arg0->charLocation, 3));
    offset = NNS_G2dGetImagePaletteLocation(SpriteTransfer_GetPaletteProxy(SpriteResourceCollection_Find(arg0->paletteLocation, 3), imageProxy), NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(arg1, 0x20U);
    GX_LoadOBJPltt(arg1, offset, 0x20U);
}
