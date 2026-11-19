#include "global.h"

#include "overlay_42.h"
#include "overlay_49.h"
#include "gf_3d_loader.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"


void ov49_02258830(NNSG3dResFileHeader **arg0, NARC *arg1, s32 arg2, enum HeapID arg3);

void ov49_02258800(const UnkStruct_ov44_02232914 *param0, VecFx32 *param1) {
    param1->x = param0->unk0 * ((fx32)0x0000000000001000L);
    param1->z = param0->unk2 * ((fx32)0x0000000000001000L);
    param1->y = ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f)));
}

void ov49_02258814(const VecFx32 *param0, UnkStruct_ov44_02232914 *param1) {
    param1->unk0 = param0->x / ((fx32)0x0000000000001000L);
    param1->unk2 = param0->z / ((fx32)0x0000000000001000L);
}

void ov49_02258830(NNSG3dResFileHeader **arg0, NARC *narc, s32 fileId, enum HeapID heapID) {
    u32 texKey;
    u32 tex4x4Key;
    u32 plttKey;
    NNSG3dResFileHeader *header;
    NNSG3dResFileHeader *nNSG3dResFileHeader;
    NNSG3dResTex *pTex;
    NNSG3dResTex *nNSG3dResTex;
    u32 size;

    header = GfGfxLoader_LoadFromOpenNarc(narc, fileId, 0, heapID, 1);
    pTex = NNS_G3dGetTex(header);
    GF3dRender_AllocAndLoadTexResources(pTex);
    NNS_G3dTexReleaseTexKey(pTex, &texKey, &tex4x4Key);
    plttKey = NNS_G3dPlttReleasePlttKey(pTex);
    size = G3dResFileHeader_GetSizeWithoutTex(header);
    nNSG3dResFileHeader = Heap_Alloc(heapID, size);
    *arg0 = nNSG3dResFileHeader;
    memcpy(nNSG3dResFileHeader, header, size);
    nNSG3dResTex = NNS_G3dGetTex(*arg0);
    NNS_G3dTexSetTexKey(nNSG3dResTex, texKey, tex4x4Key);
    NNS_G3dPlttSetPlttKey(nNSG3dResTex, plttKey);
    Heap_Free(header);
}
