// NitroSystem's G3D model setters that the Gen 4 games call and Black doesn't (g3d.h, "What the Gen 4 games call"),
// written from their behaviour the way model.c's are: each changes a material of a model resource, or every one. Not
// in Black's ROM, so the DS build doesn't list this file
#include "nnsys/g3d.h"

// The diffuse, ambient, specular and emission colors are 15 bits each, in the low and high halves of diffAmb and specEmi
#define MATCOLOR_LO 0x00007fff
#define MATCOLOR_HI 0x7fff0000

static NNSG3dResMatData *matData_(NNSG3dResMdl *mdl, u32 matID) {
    return NNS_G3dGetMatDataByIdx(NNS_G3dGetMat(mdl), matID);
}

void NNS_G3dMdlSetMdlDiff(NNSG3dResMdl *mdl, u32 matID, GXRgb col) {
    NNSG3dResMatData *mat = matData_(mdl, matID);
    mat->diffAmb = (mat->diffAmb & ~MATCOLOR_LO) | col;
}

void NNS_G3dMdlSetMdlAmb(NNSG3dResMdl *mdl, u32 matID, GXRgb col) {
    NNSG3dResMatData *mat = matData_(mdl, matID);
    mat->diffAmb = (mat->diffAmb & ~MATCOLOR_HI) | ((u32)col << 16);
}

void NNS_G3dMdlSetMdlSpec(NNSG3dResMdl *mdl, u32 matID, GXRgb col) {
    NNSG3dResMatData *mat = matData_(mdl, matID);
    mat->specEmi = (mat->specEmi & ~MATCOLOR_LO) | col;
}

void NNS_G3dMdlSetMdlEmi(NNSG3dResMdl *mdl, u32 matID, GXRgb col) {
    NNSG3dResMatData *mat = matData_(mdl, matID);
    mat->specEmi = (mat->specEmi & ~MATCOLOR_HI) | ((u32)col << 16);
}

int NNS_G3dMdlGetMdlLightEnableFlag(const NNSG3dResMdl *mdl, u32 matID) {
    return matData_((NNSG3dResMdl *)mdl, matID)->polyAttr & REG_G3_POLYGON_ATTR_LE_MASK;
}

// Sets or clears one GX_POLYGON_ATTR_MISC bit of a material
static void setMisc_(NNSG3dResMdl *mdl, u32 matID, u32 bit, BOOL enable) {
    NNSG3dResMatData *mat = matData_(mdl, matID);
    if (enable) {
        mat->polyAttr |= bit;
    } else {
        mat->polyAttr &= ~bit;
    }
}

// The setters for every material
void NNS_G3dMdlSetMdlDiffAll(NNSG3dResMdl *mdl, GXRgb col) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNS_G3dMdlSetMdlDiff(mdl, i, col);
    }
}

void NNS_G3dMdlSetMdlAmbAll(NNSG3dResMdl *mdl, GXRgb col) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNS_G3dMdlSetMdlAmb(mdl, i, col);
    }
}

void NNS_G3dMdlSetMdlSpecAll(NNSG3dResMdl *mdl, GXRgb col) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNS_G3dMdlSetMdlSpec(mdl, i, col);
    }
}

void NNS_G3dMdlSetMdlEmiAll(NNSG3dResMdl *mdl, GXRgb col) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNS_G3dMdlSetMdlEmi(mdl, i, col);
    }
}

void NNS_G3dMdlSetMdlLightEnableFlagAll(NNSG3dResMdl *mdl, int lights) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNS_G3dMdlSetMdlLightEnableFlag(mdl, i, lights);
    }
}

void NNS_G3dMdlSetMdlPolygonModeAll(NNSG3dResMdl *mdl, GXPolygonMode mode) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNS_G3dMdlSetMdlPolygonMode(mdl, i, mode);
    }
}

void NNS_G3dMdlSetMdlCullModeAll(NNSG3dResMdl *mdl, GXCull cull) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNS_G3dMdlSetMdlCullMode(mdl, i, cull);
    }
}

static void setMiscAll_(NNSG3dResMdl *mdl, u32 bit, BOOL enable) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        setMisc_(mdl, i, bit, enable);
    }
}

void NNS_G3dMdlSetMdl1DotAll(NNSG3dResMdl *mdl, BOOL enable) {
    setMiscAll_(mdl, GX_POLYGON_ATTR_MISC_DISP_1DOT, enable);
}

void NNS_G3dMdlSetMdlFarClipAll(NNSG3dResMdl *mdl, BOOL enable) {
    setMiscAll_(mdl, GX_POLYGON_ATTR_MISC_FAR_CLIPPING, enable);
}

void NNS_G3dMdlSetMdlXLDepthUpdateAll(NNSG3dResMdl *mdl, BOOL enable) {
    setMiscAll_(mdl, GX_POLYGON_ATTR_MISC_XLU_DEPTH_UPDATE, enable);
}

void NNS_G3dMdlSetMdlDepthTestCondAll(NNSG3dResMdl *mdl, BOOL enable) {
    setMiscAll_(mdl, GX_POLYGON_ATTR_MISC_DEPTHTEST_DECAL, enable);
}

// Sets (the model's own value) or clears (the global state's) bits of every material's flag or polyAttrMask
static void modifyMatFlag_(NNSG3dResMdl *mdl, BOOL useMdl, u16 bits) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNSG3dResMatData *mat = matData_(mdl, i);
        if (useMdl) {
            mat->flag |= bits;
        } else {
            mat->flag &= ~bits;
        }
    }
}

static void modifyPolyAttrMask_(NNSG3dResMdl *mdl, BOOL useMdl, u32 bits) {
    u32 i;
    for (i = 0; i < mdl->info.numMat; i++) {
        NNSG3dResMatData *mat = matData_(mdl, i);
        if (useMdl) {
            mat->polyAttrMask |= bits;
        } else {
            mat->polyAttrMask &= ~bits;
        }
    }
}

// The two above under NitroSystem's names, which HeartGold calls itself (declared implicitly; not in nnsys/g3d.h, which
// every game's file includes), and the fog bit's getter
void NNSi_G3dModifyMatFlag(NNSG3dResMdl *mdl, BOOL isOn, u32 flag);
void NNSi_G3dModifyPolygonAttrMask(NNSG3dResMdl *mdl, BOOL isOn, u32 mask);
BOOL NNS_G3dMdlGetMdlFogEnableFlag(const NNSG3dResMdl *mdl, u32 matID);

void NNSi_G3dModifyMatFlag(NNSG3dResMdl *mdl, BOOL isOn, u32 flag) {
    modifyMatFlag_(mdl, isOn, (u16)flag);
}

void NNSi_G3dModifyPolygonAttrMask(NNSG3dResMdl *mdl, BOOL isOn, u32 mask) {
    modifyPolyAttrMask_(mdl, isOn, mask);
}

BOOL NNS_G3dMdlGetMdlFogEnableFlag(const NNSG3dResMdl *mdl, u32 matID) {
    return (matData_((NNSG3dResMdl *)mdl, matID)->polyAttr & GX_POLYGON_ATTR_MISC_FOG) != 0;
}

void NNS_G3dMdlUseGlbDiff(NNSG3dResMdl *mdl) {
    modifyMatFlag_(mdl, FALSE, NNS_G3D_MATFLAG_DIFFUSE);
}

void NNS_G3dMdlUseGlbAmb(NNSG3dResMdl *mdl) {
    modifyMatFlag_(mdl, FALSE, NNS_G3D_MATFLAG_AMBIENT);
}

void NNS_G3dMdlUseGlbSpec(NNSG3dResMdl *mdl) {
    modifyMatFlag_(mdl, FALSE, NNS_G3D_MATFLAG_SPECULAR);
}

void NNS_G3dMdlUseGlbEmi(NNSG3dResMdl *mdl) {
    modifyMatFlag_(mdl, FALSE, NNS_G3D_MATFLAG_EMISSION);
}

void NNS_G3dMdlUseGlbLightEnableFlag(NNSG3dResMdl *mdl) {
    modifyPolyAttrMask_(mdl, FALSE, REG_G3_POLYGON_ATTR_LE_MASK);
}

void NNS_G3dMdlUseGlbPolygonID(NNSG3dResMdl *mdl) {
    modifyPolyAttrMask_(mdl, FALSE, REG_G3_POLYGON_ATTR_ID_MASK);
}

void NNS_G3dMdlUseGlbAlpha(NNSG3dResMdl *mdl) {
    modifyPolyAttrMask_(mdl, FALSE, REG_G3_POLYGON_ATTR_ALPHA_MASK);
}

void NNS_G3dMdlUseMdlDiff(NNSG3dResMdl *mdl) {
    modifyMatFlag_(mdl, TRUE, NNS_G3D_MATFLAG_DIFFUSE);
}

void NNS_G3dMdlUseMdlAmb(NNSG3dResMdl *mdl) {
    modifyMatFlag_(mdl, TRUE, NNS_G3D_MATFLAG_AMBIENT);
}

void NNS_G3dMdlUseMdlSpec(NNSG3dResMdl *mdl) {
    modifyMatFlag_(mdl, TRUE, NNS_G3D_MATFLAG_SPECULAR);
}

void NNS_G3dMdlUseMdlEmi(NNSG3dResMdl *mdl) {
    modifyMatFlag_(mdl, TRUE, NNS_G3D_MATFLAG_EMISSION);
}

void NNS_G3dMdlUseMdlLightEnableFlag(NNSG3dResMdl *mdl) {
    modifyPolyAttrMask_(mdl, TRUE, REG_G3_POLYGON_ATTR_LE_MASK);
}

void NNS_G3dMdlUseMdlPolygonID(NNSG3dResMdl *mdl) {
    modifyPolyAttrMask_(mdl, TRUE, REG_G3_POLYGON_ATTR_ID_MASK);
}

void NNS_G3dMdlUseMdlAlpha(NNSG3dResMdl *mdl) {
    modifyPolyAttrMask_(mdl, TRUE, REG_G3_POLYGON_ATTR_ALPHA_MASK);
}
