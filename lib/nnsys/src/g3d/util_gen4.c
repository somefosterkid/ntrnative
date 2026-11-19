// NitroSystem's G3D helpers that the Gen 4 games call and Black doesn't (g3d.h, "What the Gen 4 games call"), written
// from their behaviour. Not in Black's ROM, so the DS build doesn't list this file
#include "nnsys/g3d.h"

// NitroSystem's inlines that HeartGold calls and nnsys/g3d.h (which every game's file includes) lacks, as functions:
// the viewport of the global state (sent with the next NNS_G3dGlbFlush), and a texture block's texel and palette data.
// NOTE: HeartGold declares them implicitly (an int return): the two data getters need a prototype on its side
void NNS_G3dGlbSetViewPort(int x1, int y1, int x2, int y2);
const void *NNS_G3dGetTexData(const NNSG3dResTex *tex);
const void *NNS_G3dGetPlttData(const NNSG3dResTex *tex);

void NNS_G3dGlbSetViewPort(int x1, int y1, int x2, int y2) {
    NNS_G3dGlb.prmViewPort = GX_PACK_VIEWPORT_PARAM(x1, y1, x2, y2);
}

const void *NNS_G3dGetTexData(const NNSG3dResTex *tex) {
    return (const u8 *)tex + tex->texInfo.ofsTex;
}

const void *NNS_G3dGetPlttData(const NNSG3dResTex *tex) {
    return (const u8 *)tex + tex->plttInfo.ofsPlttData;
}

// Loads a texture block's textures and palettes into VRAM through the default managers. A NULL block is nothing to
// load; FALSE when VRAM is short, with what was allocated freed again
static BOOL setupTex_(NNSG3dResTex *tex) {
    u32 sizeTex;
    u32 sizeTex4x4;
    u32 sizePltt;
    NNSGfdTexKey keyTex = 0;
    NNSGfdTexKey keyTex4x4 = 0;
    NNSGfdPlttKey keyPltt = 0;

    if (tex == NULL) {
        return TRUE;
    }
    sizeTex = NNS_G3dTexGetRequiredSize(tex);
    sizeTex4x4 = NNS_G3dTex4x4GetRequiredSize(tex);
    sizePltt = NNS_G3dPlttGetRequiredSize(tex);

    if (sizeTex > 0) {
        keyTex = NNS_GfdAllocTexVram(sizeTex, FALSE, 0);
        if (keyTex == NNS_GFD_ALLOC_ERROR_TEXKEY) {
            return FALSE;
        }
    }
    if (sizeTex4x4 > 0) {
        keyTex4x4 = NNS_GfdAllocTexVram(sizeTex4x4, TRUE, 0);
        if (keyTex4x4 == NNS_GFD_ALLOC_ERROR_TEXKEY) {
            if (keyTex != 0) {
                NNS_GfdFreeTexVram(keyTex);
            }
            return FALSE;
        }
    }
    if (sizePltt > 0) {
        keyPltt = NNS_GfdAllocPlttVram(sizePltt, tex->plttInfo.flag & NNS_G3D_RESPLTT_USEPLTT4, 0);
        if (keyPltt == NNS_GFD_ALLOC_ERROR_PLTTKEY) {
            if (keyTex4x4 != 0) {
                NNS_GfdFreeTexVram(keyTex4x4);
            }
            if (keyTex != 0) {
                NNS_GfdFreeTexVram(keyTex);
            }
            return FALSE;
        }
    }

    NNS_G3dTexSetTexKey(tex, keyTex, keyTex4x4);
    NNS_G3dPlttSetPlttKey(tex, keyPltt);
    NNS_G3dTexLoad(tex, TRUE);
    NNS_G3dPlttLoad(tex, TRUE);
    return TRUE;
}

BOOL NNS_G3dResDefaultSetup(void *resData) {
    NNSG3dResFileHeader *header = resData;

    switch (*(u32 *)resData) {
    case NNS_G3D_SIGNATURE_NSBMD: {
        NNSG3dResTex *tex = NNS_G3dGetTex(header);
        if (!setupTex_(tex)) {
            return FALSE;
        }
        if (tex != NULL) {
            NNS_G3dBindMdlSet(NNS_G3dGetMdlSet(header), tex);
        }
        return TRUE;
    }
    case NNS_G3D_SIGNATURE_NSBTX:
        return setupTex_(NNS_G3dGetTex(header));
    case NNS_G3D_SIGNATURE_NSBCA:
    case NNS_G3D_SIGNATURE_NSBVA:
    case NNS_G3D_SIGNATURE_NSBMA:
    case NNS_G3D_SIGNATURE_NSBTP:
    case NNS_G3D_SIGNATURE_NSBTA:
        return TRUE;
    default:
        return FALSE;
    }
}

// The color bits a material's flag says it sets itself (sbc.c's MAT_COLOR_MASKS): DIFFUSE or SPECULAR the low color,
// AMBIENT or EMISSION the high one, VTXCOLOR or SHININESS bit 15
static u32 colorMask_(u32 flagBits) {
    return ((flagBits & 1) ? 0x00007fff : 0) | ((flagBits & 2) ? 0x7fff0000 : 0) | ((flagBits & 4) ? 0x00008000 : 0);
}

void NNS_G3dDraw1Mat1Shp(const NNSG3dResMdl *mdl, u32 matID, u32 shpID, BOOL sendMat) {
    if (sendMat) {
        const NNSG3dResMat *mat = NNS_G3dGetMat(mdl);
        if (matID < mat->dict.numEntry) {
            const NNSG3dResMatData *data = NNS_G3dGetMatDataByIdx(mat, matID);
            u32 maskDiffAmb = colorMask_(data->flag >> NNS_G3D_MATFLAG_DIFFAMB_SHIFT);
            u32 maskSpecEmi = colorMask_(data->flag >> NNS_G3D_MATFLAG_SPECEMI_SHIFT);
            u32 cmd[6];

            // As the MAT command: the global state where the material leaves a part to it
            cmd[0] = (NNS_G3dGlb.prmMatColor0 & ~maskDiffAmb) | (data->diffAmb & maskDiffAmb);
            cmd[1] = (NNS_G3dGlb.prmMatColor1 & ~maskSpecEmi) | (data->specEmi & maskSpecEmi);
            cmd[2] = (NNS_G3dGlb.prmPolygonAttr & ~data->polyAttrMask) | (data->polyAttr & data->polyAttrMask);
            cmd[3] = 0x2b2a; // TEXIMAGE_PARAM, TEXPLTT_BASE
            cmd[4] = data->texImageParam;
            cmd[5] = data->texPlttBase;
            NNS_G3dGeBufferOP_N(0x293130, cmd, 6); // DIF_AMB, SPE_EMI, POLYGON_ATTR
        }
    }
    {
        const NNSG3dResShp *shp = NNS_G3dGetShp(mdl);
        if (shpID < shp->dict.numEntry) {
            const NNSG3dResShpData *shpData = NNS_G3dGetShpDataByIdx(shp, shpID);
            NNS_G3dGeSendDL((const u8 *)shpData + shpData->ofsDL, shpData->sizeDL);
        }
    }
}
