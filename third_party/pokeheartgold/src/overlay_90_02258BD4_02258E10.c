#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"

struct SpriteResource;

struct SpriteResourceCollection;


void ov90_02258FB8(UnkStruct_ov114_0225CCD0 *param0, UnkStruct_ov114_0225CDB4 *param1);

void ov90_02258E10(UnkStruct_ov114_0225CCD0 *param0);

void ov90_02258E10(UnkStruct_ov114_0225CCD0 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_14; v0++) {
        if (param0->unk_10[v0].unk_00 == 1) {
            ov90_02258FB8(param0, &param0->unk_10[v0]);
        }
    }

    for (v0 = 0; v0 < 4; v0++) {
        Destroy2DGfxResObjMan(param0->unk_00[v0]);
    }

    Heap_Free(param0->unk_10);
}

void ov90_02258EE0(UnkStruct_ov114_0225CCD0 *param0, UnkStruct_ov114_0225CDB4 *param1, NARC *param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9);

UnkStruct_ov114_0225CDB4 *ov90_02258E54(UnkStruct_ov114_0225CCD0 *param0, NARC *param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8);

UnkStruct_ov114_0225CDB4 *ov90_02258E54(UnkStruct_ov114_0225CCD0 *param0, NARC *param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8) {
    int v0;
    UnkStruct_ov114_0225CDB4 *v1 = NULL;

    for (v0 = 0; v0 < param0->unk_14; v0++) {
        if (param0->unk_10[v0].unk_00 == 0) {
            v1 = &param0->unk_10[v0];
            break;
        }
    }

    GF_ASSERT(v1);

    ov90_02258EE0(param0, v1, param1, param2, param3, param4, param5, param6, param7, param8);

    return v1;
}
