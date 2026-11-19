#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "filesystem.h"
#include "gf_3d_render.h"

typedef struct UnkStruct_ov45_02230A4C {
    u8 unk_0[0x8];
    NNSG3dResMdl *unk_8;
    u8 unk_C[0x4];
    s32 unk_10;
} UnkStruct_ov45_02230A4C;

void ov49_02258830(void **param0, NARC *param1, u32 param2, u32 heapID);

typedef struct {
    Field3dModel unk_00;
    u32 unk_10;
} UnkStruct_ov66_02234958;

void ov45_022309E8(UnkStruct_ov66_02234958 *param0, NARC *param1, u32 param2, enum HeapID heapID);

s32 sub_02018068();
void ov45_02230A44(void);
void ov45_02230A4C(UnkStruct_ov45_02230A4C *arg0, s32 arg1);
s32 ov45_02230A58(void *arg0);
s32 sub_020181B0(void *, void *);
void ov45_02230A5C(void *arg0, void *arg1);

void ov45_022309E8(UnkStruct_ov66_02234958 *param0, NARC *param1, u32 param2, enum HeapID heapID) {
    void *v0;

    ov49_02258830(&v0, param1, param2, heapID);

    param0->unk_00.dataRaw = v0;
    param0->unk_00.modelSet = NNS_G3dGetMdlSet(param0->unk_00.dataRaw);
    param0->unk_00.model = NNS_G3dGetMdlByIdx(param0->unk_00.modelSet, 0);
    param0->unk_00.texture = NNS_G3dGetTex(param0->unk_00.dataRaw);

    GF3dRender_BindModelSet(param0->unk_00.dataRaw, param0->unk_00.texture);

    NNS_G3dMdlSetMdlPolygonIDAll(param0->unk_00.model, 20);
}

void ov45_02230A44(void) {
    sub_02018068();
}

void ov45_02230A4C(UnkStruct_ov45_02230A4C *arg0, s32 alpha) {
    arg0->unk_10 = alpha;
    NNS_G3dMdlSetMdlAlphaAll(arg0->unk_8, alpha);
}

s32 ov45_02230A58(void *arg0) {
    return *(s32 *)((u8 *)arg0 + 0x10);
}

void ov45_02230A5C(void *arg0, void *arg1) {
    sub_020181B0(arg1, arg0);
}
