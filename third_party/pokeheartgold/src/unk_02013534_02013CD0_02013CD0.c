#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "unk_02013534.h"

struct BgConfig;

struct FontOAMBuildState;

struct FontOAMNode;

struct TextOBJ;

struct UnkStruct_02013910;

struct Window;

extern const struct {
    u8 unk_00[1];
    u8 unk_01[23];
} _020F5F2C;

typedef struct FontOAMNode {
    s32 unk_00;
    s32 unk_04;
    int unk_08;
    struct FontOAMNode *unk_0c;
    struct FontOAMNode *unk_10;
} FontOAMNode;

typedef struct FontOAMBuildState FontOAMBuildState;

struct UnkStruct_02013910 {
    u8 unk_00[12];
    FontOAMNode *unk_0c;
    FontOAMNode *unk_10;
    int unk_14;
};

int sub_02013B24(FontOAMBuildState *state, UnkStruct_02013910 *list, enum HeapID heapID);

int sub_02013BD4(int width, int height, enum HeapID heapID, UnkStruct_02013910 *list);

FontOAMNode *sub_02013F78(enum HeapID heapID);

u32 sub_02013CD0(Window *window, FontOAMNode *node, NNSG2dImageProxy *imgProxy, int blockSize, GXOBJVRamModeChar vramMode, u32 vramOffset, NNS_G2D_VRAM_TYPE vramType, enum HeapID heapID);

u32 sub_02013CD0(Window *window, FontOAMNode *node, NNSG2dImageProxy *imgProxy, int blockSize, GXOBJVRamModeChar vramMode, const u32 vramOffset, NNS_G2D_VRAM_TYPE vramType, enum HeapID heapID) {
    void *tmpBuf;
    int shapeW;
    int area;
    int shapeH;
    GXOBJVRamModeChar mode2;

    shapeW = _020F5F2C.unk_00[node->unk_08 * 2];
    shapeH = _020F5F2C.unk_01[node->unk_08 * 2];
    area = shapeW * shapeH;
    if (area < blockSize) {
        area = blockSize;
    }
    area <<= 5;
    tmpBuf = Heap_AllocAtEnd(heapID, (u32)area);
    sub_02013A50(window, shapeW, shapeH, node->unk_00, node->unk_04, tmpBuf);
    DC_FlushRange(tmpBuf, (u32)area);
    if (vramType == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJ(tmpBuf, vramOffset, (s32)area);
        imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN] = vramOffset;
        mode2 = (GXOBJVRamModeChar)(reg_GX_DISPCNT & 0x00300010);
    } else {
        GXS_LoadOBJ(tmpBuf, vramOffset, (u32)area);
        imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DSUB] = vramOffset;
        mode2 = (GXOBJVRamModeChar)(reg_GXS_DB_DISPCNT & 0x00300010);
    }
    imgProxy->attr.mappingType = mode2;
    imgProxy->attr.sizeS = (GXTexSizeS)0x0000FFFF;
    imgProxy->attr.sizeT = (GXTexSizeT)0x0000FFFF;
    imgProxy->attr.fmt = (GXTexFmt)3;
    imgProxy->attr.bExtendedPlt = FALSE;
    imgProxy->attr.plttUse = (GXTexPlttColor0)1;
    imgProxy->attr.mappingType = vramMode;
    Heap_Free(tmpBuf);
    return vramOffset + (u32)area;
}
