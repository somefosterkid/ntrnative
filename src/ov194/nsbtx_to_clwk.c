// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/nsbtx_to_clwk.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "system/palanm.h"

// Shows a texture of a model file (NSBTX), such as an overworld sprite, on an actor: the texture's bitmap is cut
// into the 8x8 tiles of OBJ characters

void func_ov194_021f513c(const u8 *src, int width, int x, int y, int tilesX, int tilesY, void *dest);
void func_ov194_021f51a4(const u8 *src, u8 *dest, int *srcOffset, int *destOffset, int size, int stride);

// Cuts tilesX by tilesY tiles out of a 16-color bitmap that is width tiles wide, starting at the tile (x, y)
// pokebw2's NsbtxToClwk_ConvertTiles
void func_ov194_021f513c(const u8 *src, int width, int x, int y, int tilesX, int tilesY, void *dest) {
    u8 *out = dest;
    int i, j;
    int srcOffset;
    int destOffset = 0;
    int stride = width * 4;

    for (i = y; i < y + tilesY; i++) {
        for (j = x; j < x + tilesX; j++) {
            srcOffset = j * 4 + i * stride * 8;
            func_ov194_021f51a4(src, out, &srcOffset, &destOffset, 4, stride);
        }
    }
}

// Copies the 8 rows of a tile
// pokebw2's NsbtxToClwk_CopyTile
void func_ov194_021f51a4(const u8 *src, u8 *dest, int *srcOffset, int *destOffset, int size, int stride) {
    int i;

    for (i = 0; i < 8; i++) {
        MI_CpuCopy8(src + *srcOffset, dest + *destOffset, size);
        *srcOffset += stride;
        *destOffset += size;
    }
}

// pokebw2's NsbtxToClwk_LoadTextureEx, which Black lacks the fade and palette of
void func_ov194_021f51e4(ClActor *actor, u32 arcId, u32 fileId, u8 texture, u16 width, u16 height, u32 offset,
                         u32 vramType, HeapID heapId) {
    NNSG2dImagePaletteProxy proxy;
    u32 size = width * height * 32;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(arcId, heapId);
    void *file = GFL_ArcToolReadHeapNew(arc, fileId, heapId);
    NNSG3dResTex *tex = NNS_G3dGetTex(file);
    u8 *bitmap;
    u32 texOffset;
    u8 *chars;
    void *colors;

    texOffset = (NNS_G3dGetTexDataByIdx(tex, texture)->texImageParam & 0xffff) << 3;
    bitmap = (u8 *)tex + tex->texInfo.ofsTex;
    chars = GFL_HeapAllocate(HEAPID_TAIL(heapId), size, FALSE, "nsbtx_to_clwk.c", 95);
    func_ov194_021f513c(bitmap + texOffset, width, 0, 0, width, height, chars);
    func_0204aa94(func_0204b7f0(actor), chars, size, offset, vramType);
    GFL_HeapFree(chars);

    colors = (u8 *)tex + tex->plttInfo.ofsPlttData;
    DC_FlushRange(colors, 32);
    func_0204b764(actor, &proxy);

    if (vramType == CLACT_VRAM_MAIN || vramType == CLACT_VRAM_BOTH) {
        GX_LoadOBJPltt(colors, NNS_G2dGetImagePaletteLocation(&proxy, NNS_G2D_VRAM_TYPE_2DMAIN), 32);
    }
    if (vramType == CLACT_VRAM_SUB || vramType == CLACT_VRAM_BOTH) {
        GXS_LoadOBJPltt(colors, NNS_G2dGetImagePaletteLocation(&proxy, NNS_G2D_VRAM_TYPE_2DSUB), 32);
    }

    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
}

