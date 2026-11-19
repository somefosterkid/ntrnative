#include "global.h"

typedef struct UnkStruct_ov89_0225B784_arg1 {
    u8 unk_0[0x14];
    NNSG3dResMdl *unk_14;
    u8 unk_18[0x4];
    u8 unk_1C;
    u8 unk_1D[0x77];
    u8 unk_94;
    u8 unk_95[0xD7];
    u8 unk_16C;
} UnkStruct_ov89_0225B784_arg1;

s32 sub_020181EC(void *);

void ov89_0225B784(s32 arg0, UnkStruct_ov89_0225B784_arg1 *arg1);

void ov89_0225B784(s32 arg0, UnkStruct_ov89_0225B784_arg1 *arg1) {
    s32 v2;
    u8 polygonID;
    u8 v0;
    UnkStruct_ov89_0225B784_arg1 *v1;
    void *v3;

    v1 = (void *)&arg1->unk_94;
    polygonID = v1->unk_16C;
    if (polygonID != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, (s32)polygonID, 8, 0);
        NNSi_G3dModifyPolygonAttrMask(arg1->unk_14, 0, 0x3F000000U);
        NNSi_G3dModifyPolygonAttrMask(arg1->unk_14, 0, 0x1F0000U);
    }
    v2 = 0;
    v3 = v1;
    do {
        sub_020181EC(v3);
        v2 += 1;
        v3 += 0x78;
    } while (v2 < 3);
    v0 = v1->unk_16C;
    if (v0 != 0xFF) {
        NNS_G3dGlbPolygonAttr(0, GX_POLYGONMODE_MODULATE, GX_CULL_ALL, (s32)v0, 0x1F, 0);
        NNSi_G3dModifyPolygonAttrMask(arg1->unk_14, 0, 0x3F000000U);
        NNSi_G3dModifyPolygonAttrMask(arg1->unk_14, 0, 0x1F0000U);
    }
    sub_020181EC((void *)&arg1->unk_1C);
}
