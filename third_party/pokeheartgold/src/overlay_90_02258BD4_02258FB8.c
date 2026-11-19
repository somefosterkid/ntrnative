#include "global.h"
#include "sprite_resources_header.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"

struct SpriteResource;

struct SpriteResourceCollection;


void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);

void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

void ov90_02258FB8(UnkStruct_ov114_0225CCD0 *param0, UnkStruct_ov114_0225CDB4 *param1);

void ov90_02258FB8(UnkStruct_ov114_0225CCD0 *param0, UnkStruct_ov114_0225CDB4 *param1) {
    int v0;

    GF_ASSERT(param1->unk_00 == 1);

    param1->unk_00 = 0;

    SpriteTransfer_DeleteCharTransferTask(param1->unk_04[0]);
    SpriteTransfer_DeletePlttTransferTask(param1->unk_04[1]);

    for (v0 = 0; v0 < 4; v0++) {
        DestroySingle2DGfxResObj(param0->unk_00[v0], param1->unk_04[v0]);
    }
}
