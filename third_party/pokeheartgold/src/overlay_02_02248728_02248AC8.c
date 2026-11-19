#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "gf_3d_loader.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "sprite.h"
#include "overlay_02_fly_landing.h"

struct SpriteList;
typedef struct SpriteResourceWithLocation {
    s16 resourceId;
    s16 memberIdx;
    SpriteResource *spriteResource;
} SpriteResourceWithLocation;

void ov02_02248AC8(FlyResources *flyResources, int resourceId);
void ov02_02248AFC(FlyResources *flyResources, int resourceId);

void ov02_02248AC8(FlyResources *flyResources, int resourceId) {
    for (int i = 0; i < flyResources->resource2Count; i++) {
        if (flyResources->paletteSourceWrapper[i].resourceId == resourceId) {
            SpriteTransfer_CreatePlttTransferTask(flyResources->paletteSourceWrapper[i].spriteResource);
            return;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
}

void ov02_02248AFC(FlyResources *flyResources, int resourceId) {
    for (int i = 0; i < flyResources->resource2Count; i++) {
        if (flyResources->paletteSourceWrapper[i].resourceId == resourceId) {
            sub_0200A740(flyResources->paletteSourceWrapper[i].spriteResource);
            return;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
}
