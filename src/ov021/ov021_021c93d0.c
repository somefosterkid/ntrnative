// Black's code at ov021 0x021c93d0-0x021c94bc: the rest of land_data_patch.c (pokebw2's src/ov036/land_data_patch.c).
// No name is known: the module and address.

#include "types.h"
#include "field/field_map.h"
#include "field/field_prop.h"
#include "gfl/heap.h"
#include "nitro/mi.h"
#include "struct_decls.h"

// The buildings of a patch: how many, then where each is
typedef struct LandPatchBuildings {
    u32 count;
    FieldPropSourceInfo infos[1];
} LandPatchBuildings;

// pokebw2's LandDataPatch_GetAttrBlock
LandPatchGrid *func_ov021_021c94ac(LandDataPatch *patch);
LandPatchBuildings *GetLandDataPatchBMDataPtr(LandDataPatch *patch);

void func_ov021_021c93d0(LandDataPatch *patch) {
    GFL_HeapFree(patch->data);
    GFL_HeapFree(patch);
}

void func_ov021_021c93e4(LandDataPatch *patch, LandPatchGrid *src, u32 srcX, u32 srcY, u32 destX, u32 destY,
                         u32 width, u32 height) {
    LandPatchGrid *dest = func_ov021_021c94ac(patch);
    LandPatchCell *srcCells = src->cells;
    LandPatchCell *destCells = dest->cells;
    u32 i;

    for (i = 0; i < height; i++) {
        MI_CpuCopy8(&destCells[srcX + (srcY + i) * dest->width], &srcCells[destX + (destY + i) * src->width],
                    width * sizeof(LandPatchCell));
    }
}

s32 func_ov021_021c9450(LandDataPatch *patch, FieldPropSystem *propSystem, FieldChunk *chunk, s32 count, s32 x,
                        s32 y) {
    LandPatchBuildings *buildings = GetLandDataPatchBMDataPtr(patch);
    FieldPropSourceInfo *infos = buildings->infos;
    u32 i;

    for (i = 0; i < buildings->count; i++) {
        FieldPropSourceInfo info;

        MI_CpuCopy8(&infos[i], &info, sizeof(FieldPropSourceInfo));
        info.x += x << 16;
        info.z -= y << 16;
        FieldPropSystem_InstantiateFromInfo(propSystem, chunk, &info, count + i);
    }
    return count + buildings->count;
}

LandPatchGrid *func_ov021_021c94ac(LandDataPatch *patch) {
    return (LandPatchGrid *)(patch->data + *(u16 *)patch->data);
}

LandPatchBuildings *GetLandDataPatchBMDataPtr(LandDataPatch *patch) {
    return (LandPatchBuildings *)(patch->data + ((u16 *)patch->data)[1]);
}
