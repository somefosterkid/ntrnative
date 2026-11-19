#include "global.h"

#include "overlay_96_sprite_res_man.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"

void ov96_021E8BF4(Ov96SpriteResMan *man, s32 index, s32 screens, void *pltt) {
    BOOL loadMain;
    BOOL loadSub;
    NNSG2dImageProxy *imageProxy;
    NNSG2dImagePaletteProxy *plttProxy;
    u32 loc;

    loadSub = FALSE;
    loadMain = FALSE;
    if (screens == 3) {
        loadSub = TRUE;
        loadMain = TRUE;
    } else if (screens == 1) {
        loadMain = TRUE;
    } else {
        loadSub = TRUE;
    }
    imageProxy = SpriteTransfer_GetCharProxy(man->entries[index].charRes);
    plttProxy = SpriteTransfer_GetPaletteProxy(man->entries[index].plttRes, imageProxy);
    DC_FlushRange(pltt, 0x20);
    if (loadMain) {
        loc = NNS_G2dGetImagePaletteLocation(plttProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
        GX_LoadOBJPltt(pltt, loc, 0x20);
    }
    if (loadSub) {
        loc = NNS_G2dGetImagePaletteLocation(plttProxy, NNS_G2D_VRAM_TYPE_2DSUB);
        GXS_LoadOBJPltt(pltt, loc, 0x20);
    }
}
