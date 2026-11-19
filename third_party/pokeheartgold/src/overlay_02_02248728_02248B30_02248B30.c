#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "error_handling.h"
#include "filesystem.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_02_fly_landing.h"

typedef struct SpriteResourceWithLocation SpriteResourceWithLocation;

struct SpriteResourceWithLocation {
    s16 resourceId;
    s16 memberIdx;
    SpriteResource *spriteResource;
};

void ov02_02248B30(FlyResources *flyResources, NARC *narc, s32 memberIdx, s32 resourceId);

void ov02_02248B30(FlyResources *flyResources, NARC *narc, s32 memberIdx, s32 resourceId) {
    SpriteResource *pSVar1;
    s32 iVar2;
    SpriteResourceWithLocation *pSVar3;

    iVar2 = 0;
    if ((s32)flyResources->resource1Count > 0) {
        pSVar3 = flyResources->cellSourceWrapper;
        do {
            if ((s32)flyResources->resource3ID == (s32)pSVar3->resourceId) {
                flyResources->cellSourceWrapper[iVar2].resourceId = (s16)resourceId;
                flyResources->cellSourceWrapper[iVar2].memberIdx = 0;
                pSVar1 = AddCellOrAnimResObjFromOpenNarc((GF_2DGfxResMan *)flyResources->cellLocation, narc, memberIdx, 0, resourceId, GF_GFX_RES_TYPE_CELL, HEAP_ID_FIELD1);
                flyResources->cellSourceWrapper[iVar2].spriteResource = pSVar1;
                return;
            }
            iVar2 = iVar2 + 1;
            pSVar3 = pSVar3 + 1;
        } while (iVar2 < (s32)(u32)flyResources->resource1Count);
    }
    GF_AssertFail();
    return;
}
