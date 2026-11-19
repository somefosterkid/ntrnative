#include "global.h"

static void sub_0201FAA4(const fx16 x, const fx16 y, const fx16 z, const fx16 width, const fx16 height, const fx16 depth, GXBoxTestParam *boxTestParam);
static u32 sub_0201FAC8(const GXBoxTestParam *boxTestParam);
typedef struct GFXTestBox {
    fx16 width;
    fx16 height;
    fx16 depth;
    u8 padding[2];
    int xScale;
    int yScale;
    int zScale;
} GFXTestBox;

u32 sub_0201F990(const NNSG3dResMdl *model, const VecFx32 *positionPtr, const MtxFx33 *rotation, const VecFx32 *scale);
u32 sub_0201FA34(const VecFx32 *position, const GFXTestBox *box);
static void sub_0201FAA4(const fx16 x, const fx16 y, const fx16 z, const fx16 width, const fx16 height, const fx16 depth, GXBoxTestParam *boxTestParam);
static u32 sub_0201FAC8(const GXBoxTestParam *boxTestParam);

u32 sub_0201F990(const NNSG3dResMdl *model, const VecFx32 *positionPtr, const MtxFx33 *rotation, const VecFx32 *scale) {
    GXBoxTestParam boxTestParam;

    VecFx32 position = *positionPtr;
    NNSG3dResMdlInfo *modelInfo = NNS_G3dGetMdlInfo(model);

    sub_0201FAA4(modelInfo->boxX, modelInfo->boxY, modelInfo->boxZ, modelInfo->boxW, modelInfo->boxH, modelInfo->boxD, &boxTestParam);

    NNS_G3dGlbSetBaseTrans(&position);
    NNS_G3dGlbSetBaseRot(rotation);
    NNS_G3dGlbSetBaseScale(scale);
    NNS_G3dGlbFlush();
    NNS_G3dGePushMtx();
    NNS_G3dGeScale(modelInfo->boxPosScale, modelInfo->boxPosScale, modelInfo->boxPosScale);

    u32 boxTestResult = sub_0201FAC8(&boxTestParam);
    NNS_G3dGePopMtx(1);

    return boxTestResult;
}

u32 sub_0201FA34(const VecFx32 *position, const GFXTestBox *box) {
    GXBoxTestParam boxTestParam;
    sub_0201FAA4(0, 0, 0, box->width, box->height, box->depth, &boxTestParam);

    NNS_G3dGlbSetBaseTrans(position);
    NNS_G3dGlbFlush();
    NNS_G3dGePushMtx();
    NNS_G3dGeScale(box->xScale * ((fx32)0x0000000000001000L), box->yScale * ((fx32)0x0000000000001000L), box->zScale * ((fx32)0x0000000000001000L));

    u32 boxTestResult = sub_0201FAC8(&boxTestParam);
    NNS_G3dGePopMtx(1);

    return boxTestResult;
}

static void sub_0201FAA4(const fx16 x, const fx16 y, const fx16 z, const fx16 width, const fx16 height, const fx16 depth, GXBoxTestParam *boxTestParam) {
    boxTestParam->x = x;
    boxTestParam->y = y;
    boxTestParam->z = z;
    boxTestParam->width = width;
    boxTestParam->height = height;
    boxTestParam->depth = depth;
}

static u32 sub_0201FAC8(const GXBoxTestParam *boxTestParam) {
    s32 boxTestResult = 1;

    NNS_G3dGePolygonAttr(GX_LIGHTMASK_0, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 0, GX_POLYGON_ATTR_MISC_FAR_CLIPPING | GX_POLYGON_ATTR_MISC_DISP_1DOT);
    NNS_G3dGeBegin(GX_BEGIN_TRIANGLES);
    NNS_G3dGeEnd();
    NNS_G3dGeBoxTest(boxTestParam);
    NNS_G3dGeFlushBuffer();

    while (G3X_GetBoxTestResult(&boxTestResult) != 0) {}

    return boxTestResult;
}
