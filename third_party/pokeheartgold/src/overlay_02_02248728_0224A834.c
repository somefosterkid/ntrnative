#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "overlay_02_hmcut_in.h"
#include "unk_0200A090.h"
#include "struct_defs/resdat_narc_structs.h"

struct HMCutIn;

struct SpriteResource;

NNSG2dImageProxy *SpriteTransfer_GetCharProxy(const SpriteResource *resource);

void ov02_0224A834(HMCutIn *cutIn, void *pokemonSpriteSource);

void ov02_0224A834(HMCutIn *cutIn, void *pokemonSpriteSource) {
    u32 imageLocationInSource;
    SpriteResource *charResource;
    const NNSG2dImageProxy *imageProxy;

    charResource = SpriteResourceCollection_Find(cutIn->charLocation, 3);
    imageProxy = SpriteTransfer_GetCharProxy(charResource);
    imageLocationInSource = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DMAIN);

    DC_FlushRange((void *)pokemonSpriteSource, (32 * 10) * 10);
    GX_LoadOBJ(pokemonSpriteSource, imageLocationInSource, (32 * 10) * 10);
}
