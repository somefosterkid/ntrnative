#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "overlay_02_hmcut_in.h"
#include "unk_0200A090.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"

struct HMCutIn;

struct Sprite;

struct SpriteResource;

Sprite *ov02_0224A33C(HMCutIn *cutIn, const VecFx32 *position, u32 charResourceID, u32 paletteResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority);

Sprite *ov02_0224A9B8(HMCutIn *cutIn, const VecFx32 *position);

void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);

void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

void ov02_0224A8D4(HMCutIn *cutIn);

void ov02_0224A8D4(HMCutIn *cutIn) {
    int i;
    SpriteResource *charResource = SpriteResourceCollection_Find(cutIn->charLocation, 3);

    SpriteTransfer_DeleteCharTransferTask(charResource);
    DestroySingle2DGfxResObj(cutIn->charLocation, charResource);

    for (i = 0; i < 4; i++) {
        if (cutIn->charSource[i] == charResource) {
            cutIn->charSource[i] = NULL;
            break;
        }
    }

    GF_ASSERT(i < 4);

    charResource = SpriteResourceCollection_Find(cutIn->paletteLocation, 3);
    SpriteTransfer_DeletePlttTransferTask(charResource);
    DestroySingle2DGfxResObj(cutIn->paletteLocation, charResource);

    for (i = 0; i < 3; i++) {
        if (cutIn->paletteSource[i] == charResource) {
            cutIn->paletteSource[i] = NULL;
            break;
        }
    }

    GF_ASSERT(i < 3);
    charResource = SpriteResourceCollection_Find(cutIn->cellLocation, 3);

    sub_0200A740(charResource);
    DestroySingle2DGfxResObj(cutIn->cellLocation, charResource);

    for (i = 0; i < 4; i++) {
        if (cutIn->cellSource[i] == charResource) {
            cutIn->cellSource[i] = NULL;
            break;
        }
    }

    GF_ASSERT(i < 4);
}

Sprite *ov02_0224A9B8(HMCutIn *cutIn, const VecFx32 *position) {
    return ov02_0224A33C(cutIn, position, 3, 3, 3, -1, 0, 0x81);
}
