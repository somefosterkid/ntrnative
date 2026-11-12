// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/mcss.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "math.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "system/mcss.h"
#include "system/palanm.h"

// MCSS, the multi-cell sprite system: each sprite is a multi-cell animation whose cells are drawn as textured quads of
// one image in texture VRAM, described by the sprite's bin file, with a shadow drawn from the same quads. The file's
// name is the ROM's string, from GFL_HeapAllocate's calls. The MCSS* names are swan's. The functions that fill in a
// sprite's files, after func_02016fc8, are a file of their own, mcss_tool.c: the `.data` has this file's name
// before their table, where a file's literals come after its tables

// The bin file: each cell's quads, then some bytes that the battle reads
typedef struct {
    fx32 x;
    fx32 y;
    fx32 width;
    fx32 height;
    fx32 s;
    fx32 t;
} MCSSQuad;

typedef struct {
    MCSSQuad quad;
    // Drawn when its width is not 0, before the first quad or after it
    MCSSQuad quad2;
} MCSSCell;

typedef struct {
    u32 count;
    u16 unk4;
    u16 unk6;
    s16 unk8;
    s16 unkA;
    MCSSCell cells[];
} MCSSBin;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2[];
} MCSSBinTail;

struct MCSSSystem {
    // The archive that func_02014cb8 opened, which the sprites of that archive are read from
    ArcTool *arc;
    u32 arcId;
    // Runs the sprites' loads when set, and the V-blank's tasks otherwise
    TCBManager *tcbMgr;
    // The load of the shadows' palette
    TCB *shadowTcb;
    int count;
    MCSS **mcss;
    NNSG2dImagePaletteProxy shadowPltt;
    // Whether the sprites that ask for it are drawn with an orthographic projection of their own
    u32 ortho : 1;
    // Halves the depth of that projection
    u32 orthoNear : 1;
    u32 texBase;
    u32 plttBase;
    MCSSLoadCallback loadCallback;
    u32 loadCallbackWork;
    u32 heapId;
    // The scale of the multi-cell's own offset
    fx32 offsetScale;
    // How far apart in depth the cells are drawn, 0 for the default
    fx32 cellDepth;
};

struct MCSS {
    // The tasks that load the sprite's graphics, its fading palette and its blended palette
    TCB *loadTcb;
    TCB *fadeTcb;
    TCB *blendTcb;
    void *cellFile;
    NNSG2dCellDataBank *cellBank;
    void *mcFile;
    NNSG2dMultiCellDataBank *mcBank;
    void *animFile;
    NNSG2dAnimBankData *animBank;
    void *mcAnimFile;
    NNSG2dAnimBankData *mcAnimBank;
    MCSSBin *bin;
    MCSSBinTail *binTail;
    NNSG2dMultiCellAnimation mcAnim;
    void *mcWork;
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy plttProxy;
    // The sprite's colors, and the colors that func_020160d4 blended them into
    u16 *pltt;
    u16 *blendedPltt;
    int plttSize;
    VecFx32 pos;
    VecFx32 scale;
    // Only z is used, as the sprite's rotation
    VecFx32 rotation;
    VecFx32 shadowScale;
    VecFx32 unk110;
    // Added to the position, and multiplied into the scale
    VecFx32 posOffset;
    VecFx32 scaleOffset;
    // The palette's fade: its level, the level it ends at, the frames until its next step and between steps
    s8 fadeLevel;
    s8 fadeEnd;
    s8 fadeWaitCount;
    s8 fadeWait;
    s32 fadeStep;
    u32 fadeColor;
    u32 alpha : 8;
    u32 flipV : 1;
    u32 pause : 2;
    u32 hidden : 1;
    u32 fading : 1;
    u32 loaded : 1;
    // 0x20 for half the sprite's alpha
    u32 shadowAlpha : 9;
    u32 noShadow : 1;
    u32 mosaic : 4;
    u32 blended : 1;
    u32 ortho : 1;
    // Draws the cells from the last, and the second quads after the first
    u32 reverse : 1;
    u32 unk140_31 : 1;
    // Set as the animation ends, to restart it at the next update
    u32 restart : 1;
    // Its slot in the system
    u32 index;
    u32 heapId;
    MCSSLoadInfo info;
    fx32 animSpeed;
    u16 shadowRotX;
    u16 shadowRotZ;
    VecFx32 shadowOffset;
};

// The shadows' palette, and where it goes in texture palette VRAM
#define MCSS_SHADOW_ARCID 11
#define MCSS_SHADOW_PLTT 88
#define MCSS_SHADOW_PLTT_ADDR 0x1100

// Each sprite's share of the texture image and of the texture palette
#define MCSS_TEX_SIZE 0x4000
#define MCSS_PLTT_SIZE 0x20

#define MCSS_ALPHA_MAX 31
#define MCSS_SHADOW_ALPHA_HALF 0x20
#define MCSS_MOSAIC_MAX 15

// The scanlines in which a V-blank task may load to VRAM
#define MCSS_LOAD_LINE_MIN 192
#define MCSS_LOAD_LINE_MAX 200

void func_020155a0(MCSSSystem *system, MCSS *mcss, fx32 x, fx32 y, fx32 width, fx32 height, fx32 s, fx32 t,
                          const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt,
                          NNSG2dImagePaletteProxy *shadowPltt, int node, BOOL ortho, fx32 *z, u8 flip);
static void LoadMCSSGraphicsData(MCSSSystem *system, int index, const MCSSLoadInfo *info);
static void func_02016630(MCSSSystem *system, int index, const MCSSLoadInfo *info);
static void func_02016880(TCB *tcb, void *data);
static void func_0201691c(TCB *tcb, void *data);
static void func_02016984(MCSS *mcss, u32 mcType);
void func_020169d4(void);
static void func_020169e8(MCSSSystem *system, MCSS *mcss);
static void func_02016ba0(MCSS *mcss);
static void func_02016c0c(TCB *tcb);
static void func_02016c38(TCB *tcb);
static void func_02016c60(MCSS *mcss, MCSSLoad *load);
void func_02016fc8(const VecFx32 *vec, const MtxFx44 *mtx, VecFx32 *dest, fx32 *w);

// pokebw2's MCSSSys_Create
MCSSSystem *func_02014b08(u32 capacity, HeapID heapId) {
    MCSSSystem *system = GFL_HeapAllocate(heapId, sizeof(MCSSSystem), TRUE, "mcss.c", 120);
    MCSSLoad *load;

    system->count = capacity;
    system->heapId = heapId;
    system->offsetScale = FX32_ONE;
    system->mcss = GFL_HeapAllocate(heapId, capacity * sizeof(MCSS *), TRUE, "mcss.c", 127);
    system->texBase = 0x24000;
    system->plttBase = 0x1000;
    NNS_G2dInitImagePaletteProxy(&system->shadowPltt);

    load = GFL_HeapAllocate(HEAPID_TAIL(heapId), sizeof(MCSSLoad), TRUE, "mcss.c", 142);
    load->system = system;
    load->plttProxy = &system->shadowPltt;
    load->plttAddr = MCSS_SHADOW_PLTT_ADDR;
    load->plttFile = GFL_G2DIOReadNCLR(MCSS_SHADOW_ARCID, MCSS_SHADOW_PLTT, &load->pltt, HEAPID_TAIL(heapId));
    system->shadowTcb = GFL_VBlankTCBAdd(func_02016880, load, 0);
    return system;
}

// pokebw2's MCSSSys_Free
void func_02014ba8(MCSSSystem *system) {
    int i;

    func_02014cd4(system);
    if (system->shadowTcb != NULL) {
        MCSSLoad *load = GFL_TCBGetData(system->shadowTcb);

        GFL_HeapFree(load->pltt->pRawData);
        GFL_HeapFree(load->pltt);
        GFL_HeapFree(load);
        GFL_TCBRemove(system->shadowTcb);
        system->shadowTcb = NULL;
    }
    for (i = 0; i < system->count; i++) {
        if (system->mcss[i] != NULL) {
            func_02015ba4(system, system->mcss[i]);
        }
    }
    GFL_HeapFree(system->mcss);
    GFL_HeapFree(system);
}

// pokebw2's MCSSSys_Update
void func_02014c08(MCSSSystem *system) {
    int i;

    for (i = 0; i < system->count; i++) {
        if (system->mcss[i] != NULL) {
            if (system->mcss[i]->pause == 0) {
                NNS_G2dTickMCAnimation(&system->mcss[i]->mcAnim, system->mcss[i]->animSpeed);
            }
            if (system->mcss[i]->fading) {
                func_020169e8(system, system->mcss[i]);
            }
            if (system->mcss[i]->restart == TRUE) {
                func_02016374(system->mcss[i]);
                NNS_G2dTickMCAnimation(&system->mcss[i]->mcAnim, 0);
                system->mcss[i]->restart = FALSE;
            }
        }
    }
}

void func_02014cb8(MCSSSystem *system, u32 arcId) {
    if (system->arc == NULL) {
        system->arcId = arcId;
        system->arc = GFL_ArcSysCreateFileHandle(arcId, system->heapId);
    }
}

void func_02014cd4(MCSSSystem *system) {
    if (system->arc != NULL) {
        GFL_ArcToolFree(system->arc);
        system->arcId = 0;
        system->arc = NULL;
    }
}

void func_02014cec(MCSSSystem *system, TCBManager *tcbMgr) {
    system->tcbMgr = tcbMgr;
}

// pokebw2's MCSSSys_SetLoadCallback
void func_02014cf0(MCSSSystem *system, MCSSLoadCallback callback, u32 work) {
    system->loadCallback = callback;
    system->loadCallbackWork = work;
}

void func_02014cf8(MCSSSystem *system, MCSS *mcss, const MCSSLoadInfo *info) {
    func_02016ba0(mcss);
    mcss->info = *info;
    LoadMCSSGraphicsData(system, mcss->index, info);
}

// The current frame of an animation, as the SRT it may hold
static inline void MCSS_GetAnimSrt(const NNSG2dAnimController *animCtrl, NNSG2dAnimDataSRT *srt) {
    switch ((u16)animCtrl->pAnimSequence->animType) {
    case NNS_G2D_ANIMELEMENT_INDEX:
        srt->index = *(u16 *)animCtrl->pCurrent->pContent;
        srt->rotZ = 0;
        srt->sx = FX32_ONE;
        srt->sy = FX32_ONE;
        srt->px = 0;
        srt->py = 0;
        break;
    case NNS_G2D_ANIMELEMENT_INDEX_SRT:
        *srt = *(NNSG2dAnimDataSRT *)animCtrl->pCurrent->pContent;
        break;
    case NNS_G2D_ANIMELEMENT_INDEX_T: {
        NNSG2dAnimDataT *data = animCtrl->pCurrent->pContent;

        srt->index = data->index;
        srt->rotZ = 0;
        srt->sx = FX32_ONE;
        srt->sy = FX32_ONE;
        srt->px = data->px;
        srt->py = data->py;
        break;
    }
    }
}

void func_02014d2c(MCSSSystem *system) {
    MCSS *mcss;
    NNSG2dImageProxy *imageProxy;
    int i;
    int j;
    NNSG2dMCCellAnimation **cellAnims;
    NNSG2dAnimDataSRT mcSrt;
    NNSG2dAnimDataSRT cellSrt;
    VecFx32 pos;
    MtxFx44 billboard;
    MtxFx43 invCamera;
    MtxFx33 rotation;
    fx32 x;
    fx32 y;
    u16 rot;
    u8 flip;
    fx32 rotCos;
    fx32 rotSin;
    BOOL ortho;

    NNS_G3dGeFlushBuffer();
    G3_PushMtx();
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_StoreMtx(0);
    G3_MtxMode(GX_MTXMODE_TEXTURE);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    MTX_Inverse43(&NNS_G3dGlb.cameraMtx, &invCamera);
    MI_Copy36B(&invCamera, &rotation);
    MTX_Copy33To44_(&rotation, &billboard);

    for (i = 0; i < system->count; i++) {
        NNSG2dMultiCellAnimation *mcAnim;
        fx32 z;

        if (system->mcss[i] == NULL || system->mcss[i]->hidden || system->mcss[i]->loaded != TRUE) {
            continue;
        }
        ortho = FALSE;
        z = 0;
        if (system->ortho) {
            ortho = system->mcss[i]->ortho;
        }
        mcss = system->mcss[i];
        imageProxy = &mcss->imageProxy;
        if (ortho == FALSE) {
            G3i_LookAt_(&NNS_G3dGlb.camPos, &NNS_G3dGlb.camUp, &NNS_G3dGlb.camTarget, TRUE, NULL);
        } else {
            VecFx32 camPos = { 0, 0, 0 };
            VecFx32 camUp = { 0, FX32_ONE, 0 };
            VecFx32 camTarget = { 0, 0, -FX32_ONE };

            G3i_LookAt_(&camPos, &camUp, &camTarget, TRUE, NULL);
        }
        mcAnim = &mcss->mcAnim;
        cellAnims = (NNSG2dMCCellAnimation **)&mcss->mcAnim.multiCellInstance.pCellAnimInstasnces;
        MCSS_GetAnimSrt(&mcAnim->animCtrl, &mcSrt);
        pos.x = mcss->pos.x + mcss->posOffset.x;
        pos.y = mcss->pos.y + mcss->posOffset.y;
        pos.z = mcss->pos.z + mcss->posOffset.z;
        if (ortho == FALSE) {
            x = FX_Mul(mcSrt.px << 8, system->offsetScale);
            y = FX_Mul(-mcSrt.py << 8, system->offsetScale);
        } else {
            fx32 scaleX = FX_Mul(mcss->scale.x, mcss->scaleOffset.x);
            fx32 scaleY = FX_Mul(mcss->scale.y, mcss->scaleOffset.y);
            fx32 offsetX;
            fx32 offsetY;
            MtxFx44 viewProj;
            fx32 w;

            MTX_Copy43To44_(&NNS_G3dGlb.cameraMtx, &viewProj);
            MTX_Concat44(&viewProj, &NNS_G3dGlb.projMtx, &viewProj);
            func_02016fc8(&pos, &viewProj, &pos, &w);
            pos.x = FX_Mul(FX_Div(pos.x, w), 128 * FX32_ONE);
            pos.y = FX_Mul(FX_Div(pos.y, w), 96 * FX32_ONE);
            pos.z = -pos.z;
            offsetX = FX32_CONST(mcSrt.px);
            offsetY = FX32_CONST(-mcSrt.py);
            x = FX_Mul(offsetX, scaleX / 16);
            y = FX_Mul(offsetY, scaleY / 16);
        }
        G3_Translate(pos.x, pos.y, pos.z);
        if (ortho == FALSE) {
            G3_MultMtx44(&billboard);
        }
        rot = mcSrt.rotZ + (mcss->rotation.z >> FX32_SHIFT);
        G3_Translate(x, y, 0);
        flip = 0;
        rotCos = FX_CosIdx(rot);
        rotSin = -FX_SinIdx(rot);
        G3_RotZ(rotSin, rotCos);
        G3_Scale(FX_Mul(FX_Mul(mcSrt.sx, mcss->scale.x), mcss->scaleOffset.x),
                 FX_Mul(FX_Mul(mcSrt.sy, mcss->scale.y), mcss->scaleOffset.y), FX32_ONE);
        if (mcss->scale.x < 0) {
            flip++;
        }
        G3_StoreMtx(29);

        // The shadow's matrix
        G3i_LookAt_(&NNS_G3dGlb.camPos, &NNS_G3dGlb.camUp, &NNS_G3dGlb.camTarget, TRUE, NULL);
        G3_Translate(mcss->pos.x + mcss->posOffset.x + mcss->shadowOffset.x, mcss->pos.y + mcss->shadowOffset.y,
                     mcss->pos.z + mcss->posOffset.z + mcss->shadowOffset.z);
        G3_RotX(FX_SinIdx(mcss->shadowRotX), FX_CosIdx(mcss->shadowRotX));
        G3_Translate(FX_Mul(FX_Mul(mcSrt.px << 8, system->offsetScale), mcss->shadowScale.x),
                     FX_Mul(FX_Mul(-mcSrt.py << 8, system->offsetScale), mcss->shadowScale.y), 0);
        G3_RotZ(-FX_SinIdx(mcss->shadowRotZ), FX_CosIdx(mcss->shadowRotZ));
        G3_RotZ(rotSin, rotCos);
        G3_Scale(FX_Mul(FX_Mul(FX_Mul(mcSrt.sx, mcss->shadowScale.x), mcss->unk110.x), mcss->scaleOffset.x),
                 FX_Mul(FX_Mul(FX_Mul(mcSrt.sy, mcss->shadowScale.y), mcss->unk110.y), mcss->scaleOffset.y), FX32_ONE);
        G3_StoreMtx(30);

        func_020169d4();
        G3_TexImageParam(imageProxy->attr.fmt, GX_TEXGEN_TEXCOORD, imageProxy->attr.sizeS, imageProxy->attr.sizeT,
                         GX_TEXREPEAT_ST, GX_TEXFLIP_ST, imageProxy->attr.plttUse,
                         imageProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_3DMAIN]);

        for (j = 0; j < mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt.index].numNodes; j++) {
            NNSG2dMCCellAnimation *cellAnim;
            const MCSSCell *cell;
            int node;

            if (mcss->alpha != MCSS_ALPHA_MAX || mcss->reverse) {
                node = mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt.index].numNodes - 1 - j;
            } else {
                node = j;
            }
            cellAnim =
                &(*cellAnims)
                    [(mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt.index].pHierDataArray[node].nodeAttr &
                      0xff00) >>
                     8];
            MCSS_GetAnimSrt(&cellAnim->cellAnim.animCtrl, &cellSrt);
            cell = &mcss->bin->cells[cellSrt.index];
            if (cell->quad2.width != 0 && mcss->reverse == FALSE) {
                if (mcss->flipV) {
                    func_020155a0(system, mcss, cell->quad2.x, cell->quad2.y, cell->quad2.width, cell->quad2.height,
                                  cell->quad2.s, cell->quad2.t + cell->quad2.height, &cellSrt, &mcSrt,
                                  &system->shadowPltt, node, ortho, &z, flip);
                } else {
                    func_020155a0(system, mcss, cell->quad2.x, cell->quad2.y, cell->quad2.width, cell->quad2.height,
                                  cell->quad2.s, cell->quad2.t, &cellSrt, &mcSrt, &system->shadowPltt, node, ortho, &z,
                                  flip);
                }
            }
            func_020155a0(system, mcss, cell->quad.x, cell->quad.y, cell->quad.width, cell->quad.height, cell->quad.s,
                          cell->quad.t, &cellSrt, &mcSrt, &system->shadowPltt, node, ortho, &z, flip);
            if (cell->quad2.width != 0 && mcss->reverse == TRUE) {
                if (mcss->flipV) {
                    func_020155a0(system, mcss, cell->quad2.x, cell->quad2.y, cell->quad2.width, cell->quad2.height,
                                  cell->quad2.s, cell->quad2.t + cell->quad2.height, &cellSrt, &mcSrt,
                                  &system->shadowPltt, node, ortho, &z, flip);
                } else {
                    func_020155a0(system, mcss, cell->quad2.x, cell->quad2.y, cell->quad2.width, cell->quad2.height,
                                  cell->quad2.s, cell->quad2.t, &cellSrt, &mcSrt, &system->shadowPltt, node, ortho, &z,
                                  flip);
                }
            }
        }
    }
    G3_PopMtx(1);
}


// Draws a quad of a cell and its shadow, and steps the depth for the next
void func_020155a0(MCSSSystem *system, MCSS *mcss, fx32 x, fx32 y, fx32 width, fx32 height, fx32 s, fx32 t,
                          const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt,
                          NNSG2dImagePaletteProxy *shadowPltt, int node, BOOL ortho, fx32 *z, u8 flip) {
    u8 shadowAlpha;
    fx32 nodeX;
    fx32 nodeY;
    u32 st0;
    u32 st1;
    u32 st2;
    u32 st3;
    int polygonId;

    shadowAlpha = mcss->shadowAlpha == MCSS_SHADOW_ALPHA_HALF ? mcss->alpha / 2 : mcss->shadowAlpha;
    if (mcss->alpha == MCSS_ALPHA_MAX || mcss->unk140_31) {
        polygonId = 0;
    } else {
        polygonId = node;
        if (polygonId > 63) {
            polygonId = 63;
        }
    }
    if (ortho) {
        G3i_OrthoW_(FX32_CONST(96), FX32_CONST(-96), FX32_CONST(-128), FX32_CONST(128), FX32_ONE,
                 FX32_ONE * (system->orthoNear ? 512 : 1024), FX32_ONE, TRUE, NULL);
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    }
    G3_RestoreMtx(29);
    G3_TexPlttBase(mcss->plttProxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_3DMAIN], mcss->plttProxy.fmt);
    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, polygonId, mcss->alpha, 0);
    nodeX =
        (cellSrt->px + mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt->index].pHierDataArray[node].posX)
        << 8;
    nodeY =
        -(cellSrt->py + mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt->index].pHierDataArray[node].posY)
        << 8;
    G3_Translate(nodeX, nodeY, *z);
    G3_RotZ(-FX_SinIdx(cellSrt->rotZ), FX_CosIdx(cellSrt->rotZ));
    if (flip == 0) {
        G3_Scale(cellSrt->sx, cellSrt->sy, FX32_ONE);
        G3_Translate(x, y, 0);
        G3_Scale(width, height, FX32_ONE);
    } else if (flip & 1) {
        G3_Scale(-cellSrt->sx, cellSrt->sy, FX32_ONE);
        G3_Translate(-x, y, 0);
        G3_Scale(width, height, FX32_ONE);
        G3_Translate(-0x100, 0, 0);
        s = FX32_CONST(512) - s - width;
    }
    G3_Begin(GX_BEGIN_QUADS);
    st0 = GX_ST(s, t);
    reg_G3_TEXCOORD = st0;
    G3_Vtx(0, 0, 0);
    st1 = GX_ST(s, t + height);
    reg_G3_TEXCOORD = st1;
    G3_Vtx(0, -0x100, 0);
    st2 = GX_ST(s + width, t + height);
    reg_G3_TEXCOORD = st2;
    G3_Vtx(0x100, -0x100, 0);
    st3 = GX_ST(s + width, t);
    reg_G3_TEXCOORD = st3;
    G3_Vtx(0x100, 0, 0);
    G3_End();
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_RestoreMtx(0);
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    G3_RestoreMtx(30);

    if (mcss->noShadow == FALSE && shadowAlpha != 0) {
        G3_TexPlttBase(shadowPltt->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_3DMAIN], shadowPltt->fmt);
        G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 1, shadowAlpha, 0);
        G3_Translate(nodeX, nodeY, 0);
        G3_RotZ(-FX_SinIdx(cellSrt->rotZ), FX_CosIdx(cellSrt->rotZ));
        G3_Scale(cellSrt->sx, cellSrt->sy, FX32_ONE);
        G3_Translate(x, y, 0);
        G3_Scale(width, height, FX32_ONE);
        G3_Begin(GX_BEGIN_QUADS);
        reg_G3_TEXCOORD = st0;
        G3_Vtx(0, 0, 0);
        reg_G3_TEXCOORD = st1;
        G3_Vtx(0, -0x100, 0);
        reg_G3_TEXCOORD = st2;
        G3_Vtx(0x100, -0x100, 0);
        reg_G3_TEXCOORD = st3;
        G3_Vtx(0x100, 0, 0);
        G3_End();
    }

    if (ortho == FALSE) {
        if (system->cellDepth == 0) {
            *z -= ((mcss->alpha != MCSS_ALPHA_MAX) | mcss->reverse) ? -0x100L : 0x100L;
        } else {
            *z -= ((mcss->alpha != MCSS_ALPHA_MAX) | mcss->reverse) ? -system->cellDepth : system->cellDepth;
        }
    } else if (system->orthoNear) {
        *z -= ((mcss->alpha != MCSS_ALPHA_MAX) | mcss->reverse) ? -0x100L : 0x100L;
    } else {
        *z -= ((mcss->alpha != MCSS_ALPHA_MAX) | mcss->reverse) ? -0x400 : 0x400;
    }
}

// pokebw2's MCSSSys_Add
MCSS *func_020159c8(MCSSSystem *system, fx32 x, fx32 y, fx32 z, const MCSSLoadInfo *info) {
    int i;

    for (i = 0; i < system->count; i++) {
        if (system->mcss[i] == NULL) {
            system->mcss[i] = GFL_HeapAllocate(system->heapId, sizeof(MCSS), TRUE, "mcss.c", 906);
            system->mcss[i]->index = i;
            system->mcss[i]->heapId = system->heapId;
            system->mcss[i]->pos.x = x;
            system->mcss[i]->pos.y = y;
            system->mcss[i]->pos.z = z;
            system->mcss[i]->scale.x = FX32_ONE;
            system->mcss[i]->scale.y = FX32_ONE;
            system->mcss[i]->scale.z = FX32_ONE;
            system->mcss[i]->posOffset.x = 0;
            system->mcss[i]->posOffset.y = 0;
            system->mcss[i]->posOffset.z = 0;
            system->mcss[i]->scaleOffset.x = FX32_ONE;
            system->mcss[i]->scaleOffset.y = FX32_ONE;
            system->mcss[i]->scaleOffset.z = FX32_ONE;
            system->mcss[i]->alpha = MCSS_ALPHA_MAX;
            system->mcss[i]->shadowAlpha = MCSS_SHADOW_ALPHA_HALF;
            system->mcss[i]->hidden = FALSE;
            system->mcss[i]->shadowRotX = 0xc400;
            system->mcss[i]->shadowRotZ = 0;
            system->mcss[i]->shadowOffset.x = 0;
            system->mcss[i]->shadowOffset.y = 0;
            system->mcss[i]->shadowOffset.z = -FX32_ONE;
            system->mcss[i]->unk110.x = FX32_ONE;
            system->mcss[i]->unk110.y = FX32_ONE;
            system->mcss[i]->unk110.z = FX32_ONE;
            system->mcss[i]->info = *info;
            system->mcss[i]->animSpeed = FX32_ONE;
            system->mcss[i]->ortho = TRUE;
            LoadMCSSGraphicsData(system, i, info);
            break;
        }
    }
    return system->mcss[i];
}

// pokebw2's MCSSSys_Remove
void func_02015ba4(MCSSSystem *system, MCSS *mcss) {
    func_02016ba0(mcss);
    if (mcss->loadTcb != NULL) {
        func_02016c0c(mcss->loadTcb);
        mcss->loadTcb = NULL;
    }
    if (mcss->fadeTcb != NULL) {
        func_02016c38(mcss->fadeTcb);
        mcss->fadeTcb = NULL;
    }
    if (mcss->blendTcb != NULL) {
        func_02016c38(mcss->blendTcb);
        mcss->blendTcb = NULL;
    }
    system->mcss[mcss->index] = NULL;
    GFL_HeapFree(mcss);
}

void func_02015bf0(MCSSSystem *system) {
    system->ortho = TRUE;
}

void func_02015c00(MCSSSystem *system) {
    system->ortho = FALSE;
}

void func_02015c0c(MCSS *mcss) {
    mcss->ortho = TRUE;
}

void func_02015c1c(MCSS *mcss) {
    mcss->ortho = FALSE;
}

// pokebw2's MCSS_GetPosition
void func_02015c30(MCSS *mcss, VecFx32 *position) {
    position->x = mcss->pos.x;
    position->y = mcss->pos.y;
    position->z = mcss->pos.z;
}

void func_02015c48(MCSS *mcss, VecFx32 *position) {
    mcss->pos.x = position->x;
    mcss->pos.y = position->y;
    mcss->pos.z = position->z;
}

void func_02015c60(MCSS *mcss, VecFx32 *out) {
    out->x = mcss->posOffset.x;
    out->y = mcss->posOffset.y;
    out->z = mcss->posOffset.z;
}

void func_02015c78(MCSS *mcss, VecFx32 *a1) {
    mcss->posOffset.x = a1->x;
    mcss->posOffset.y = a1->y;
    mcss->posOffset.z = a1->z;
}

void func_02015c94(MCSS *mcss, VecFx32 *out) {
    out->x = mcss->scale.x;
    out->y = mcss->scale.y;
    out->z = mcss->scale.z;
}

void func_02015cac(MCSS *mcss, VecFx32 *scale) {
    mcss->scale.x = scale->x;
    mcss->scale.y = scale->y;
    mcss->scale.z = scale->z;
}

void func_02015cc4(MCSS *mcss, VecFx32 *out) {
    out->x = mcss->scaleOffset.x;
    out->y = mcss->scaleOffset.y;
    out->z = mcss->scaleOffset.z;
}

void func_02015cdc(MCSS *mcss, VecFx32 *a1) {
    mcss->scaleOffset.x = a1->x;
    mcss->scaleOffset.y = a1->y;
    mcss->scaleOffset.z = a1->z;
}

void func_02015cf8(MCSS *mcss, VecFx32 *out) {
    out->x = mcss->rotation.x;
    out->y = mcss->rotation.y;
    out->z = mcss->rotation.z;
}

void func_02015d14(MCSS *mcss, const VecFx32 *in) {
    mcss->rotation.x = in->x;
    mcss->rotation.y = in->y;
    mcss->rotation.z = in->z;
}

void func_02015d30(MCSS *mcss, VecFx32 *a1) {
    mcss->shadowScale.x = a1->x;
    mcss->shadowScale.y = a1->y;
    mcss->shadowScale.z = a1->z;
}

void func_02015d4c(MCSS *mcss, VecFx32 *out) {
    out->x = mcss->unk110.x;
    out->y = mcss->unk110.y;
    out->z = mcss->unk110.z;
}

void func_02015d64(MCSS *mcss, VecFx32 *in) {
    mcss->unk110.x = in->x;
    mcss->unk110.y = in->y;
    mcss->unk110.z = in->z;
}

void func_02015d80(MCSS *mcss, u16 a1) {
    mcss->shadowRotX = a1;
}

void func_02015d88(MCSS *mcss, u16 value) {
    mcss->shadowRotZ = value;
}

void func_02015d94(MCSS *mcss, VecFx32 *a1) {
    mcss->shadowOffset.x = a1->x;
    mcss->shadowOffset.y = a1->y;
    mcss->shadowOffset.z = a1->z;
}

void func_02015db0(MCSS *mcss) {
    mcss->flipV = TRUE;
}

void func_02015dc0(MCSS *mcss) {
    mcss->flipV = FALSE;
}

void func_02015dd4(MCSS *mcss) {
    mcss->flipV ^= 1;
}

BOOL func_02015df8(MCSS *mcss) {
    return mcss->pause;
}

// pokebw2's MCSS_PauseAnimation
void func_02015e04(MCSS *mcss) {
    mcss->pause |= 1;
}

void func_02015e28(MCSS *mcss) {
    mcss->pause |= 2;
}

// pokebw2's MCSS_ResumeAnimation
void func_02015e4c(MCSS *mcss) {
    mcss->pause &= ~1;
}

void func_02015e70(MCSS *mcss) {
    mcss->pause &= ~2;
}

BOOL func_02015e94(MCSS *mcss) {
    return mcss->hidden;
}

// pokebw2's MCSS_Hide
void func_02015ea0(MCSS *mcss) {
    mcss->hidden = TRUE;
}

// pokebw2's MCSS_Show
void func_02015eb0(MCSS *mcss) {
    mcss->hidden = FALSE;
}

void func_02015ec4(MCSS *mcss) {
    mcss->hidden ^= 1;
}

NNSG2dMultiCellAnimation *func_02015ee8(MCSS *mcss) {
    return &mcss->mcAnim;
}

u16 func_02015eec(MCSS *mcss) {
    return mcss->mcAnimBank->numSequences;
}

u8 func_02015ef4(MCSS *mcss) {
    return mcss->binTail->unk0;
}

u16 func_02015efc(MCSS *mcss, u8 index) {
    return mcss->binTail->unk2[index];
}

u16 func_02015f04(MCSS *mcss) {
    return mcss->bin->unk4;
}

u16 func_02015f0c(MCSS *mcss) {
    return mcss->bin->unk6;
}

s16 func_02015f14(MCSS *mcss) {
    return mcss->bin->unk8;
}

s16 func_02015f1c(MCSS *mcss) {
    return mcss->bin->unkA;
}

u8 func_02015f24(MCSS *mcss) {
    return mcss->binTail->unk1;
}

// pokebw2's MCSS_SetAnimationEndCallback
void func_02015f2c(MCSS *mcss, uintptr_t param, MCSSAnimationCallback callback, u32 unused) {
    NNS_G2dSetAnimCtrlCallBackFunctor(&mcss->mcAnim.animCtrl, 1, param, (NNSG2dAnmCallBackPtr)callback);
}

void func_02015f40(MCSS *mcss, u32 param,
                   BOOL (*func)(u32 param, const NNSG2dMultiCellHierarchyData *node, NNSG2dCellAnimation *cellAnim,
                                u16 nodeIdx)) {
    NNS_G2dTraverseMCNodes(&mcss->mcAnim.multiCellInstance, func, param);
}

void func_02015f50(MCSS *mcss, u8 startLevel, u8 endLevel, s32 wait, u32 color) {
    mcss->fading = TRUE;
    mcss->fadeLevel = startLevel;
    mcss->fadeEnd = endLevel;
    mcss->fadeColor = color;
    mcss->fadeWaitCount = 0;
    if (wait < 0) {
        mcss->fadeWait = 0;
        mcss->fadeStep = -(wait - 1);
    } else {
        mcss->fadeWait = wait;
        mcss->fadeStep = 1;
    }
    if (startLevel > endLevel) {
        mcss->fadeStep *= -1;
    }
}

u8 func_02015fac(MCSS *mcss) {
    return mcss->alpha;
}

// pokebw2's MCSS_SetAlpha
void func_02015fbc(MCSS *mcss, u8 alpha) {
    mcss->alpha = alpha;
}

void func_02015fd4(MCSS *mcss, u32 a1) {
    mcss->shadowAlpha = a1;
}

void func_02015ff0(MCSS *mcss, u32 a1) {
    mcss->noShadow = a1;
}

BOOL func_0201600c(MCSS *mcss) {
    if (mcss->fading) {
        return TRUE;
    }
    return FALSE;
}

void func_02016020(MCSSSystem *system, u32 characterOffset) {
    system->texBase = characterOffset;
}

void func_02016024(MCSSSystem *system, u32 paletteOffset) {
    system->plttBase = paletteOffset;
}

// pokebw2's MCSS_SetAnimation
void func_02016028(MCSS *mcss, u32 animation) {
    const NNSG2dAnimSequence *seq = NNS_G2dGetAnimSequenceByIdx(mcss->mcAnimBank, animation);

    if (mcss->mcWork != NULL) {
        GFL_HeapFree(mcss->mcWork);
    }
    func_02016984(mcss, NNS_G2D_MCTYPE_SHARE_CELLANIM);
    NNS_G2dSetAnimSequenceToMCAnimation(&mcss->mcAnim, seq);
    mcss->restart = FALSE;
}

u8 func_02016068(MCSS *mcss) {
    return mcss->mosaic;
}

// Sets the size of the sprite's mosaic and loads its graphics again
void func_02016078(MCSSSystem *system, MCSS *mcss, int level) {
    if (mcss->mosaic != level) {
        if (level > MCSS_MOSAIC_MAX) {
            level = MCSS_MOSAIC_MAX;
        }
        mcss->mosaic = level;
        func_02016ba0(mcss);
        LoadMCSSGraphicsData(system, mcss->index, &mcss->info);
    }
}

void func_020160c0(MCSSSystem *system, u32 offset) {
    system->offsetScale = offset;
}

void func_020160c4(MCSS *mcss, fx32 speed) {
    mcss->animSpeed = speed;
}

u16 func_020160cc(MCSS *mcss) {
    return mcss->mcAnimBank->numSequences;
}

// Blends the sprite's colors with a color, for the fade to start from
void func_020160d4(MCSSSystem *system, MCSS *mcss, u8 evy, u32 color) {
    MCSSLoad *load = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), sizeof(MCSSLoad), TRUE, "mcss.c", 1889);

    load->system = system;
    load->inTcbMgr = system->tcbMgr != NULL;
    load->mcss = mcss;
    load->plttProxy = &mcss->plttProxy;
    load->plttAddr = system->plttBase + mcss->index * MCSS_PLTT_SIZE;
    load->pltt = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), sizeof(NNSG2dPaletteData), FALSE, "mcss.c", 1897);
    load->pltt->fmt = mcss->plttProxy.fmt;
    load->pltt->bExtendedPlt = FALSE;
    load->pltt->szByte = mcss->plttSize;
    load->pltt->pRawData = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), mcss->plttSize, FALSE, "mcss.c", 1902);
    func_02021f64(mcss->pltt, load->pltt->pRawData, mcss->plttSize / 2, evy, color);
    MI_CpuCopy16(load->pltt->pRawData, mcss->blendedPltt, mcss->plttSize);
    if (mcss->blendTcb != NULL) {
        func_02016c38(mcss->blendTcb);
        mcss->blendTcb = NULL;
    }
    if (mcss->fading == FALSE) {
        if (system->tcbMgr != NULL) {
            mcss->blendTcb = GFL_TCBMgrAddTask(system->tcbMgr, func_0201691c, load, 0);
        } else {
            mcss->blendTcb = GFL_VBlankTCBAdd(func_0201691c, load, 0);
        }
    } else {
        GFL_HeapFree(load->pltt->pRawData);
        GFL_HeapFree(load->pltt);
        GFL_HeapFree(load);
    }
    mcss->blended = TRUE;
}

// Puts the sprite's own colors back
void func_0201624c(MCSSSystem *system, MCSS *mcss) {
    MCSSLoad *load = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), sizeof(MCSSLoad), TRUE, "mcss.c", 1942);

    load->system = system;
    load->inTcbMgr = system->tcbMgr != NULL;
    load->mcss = mcss;
    load->plttProxy = &mcss->plttProxy;
    load->plttAddr = system->plttBase + mcss->index * MCSS_PLTT_SIZE;
    load->pltt = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), sizeof(NNSG2dPaletteData), FALSE, "mcss.c", 1950);
    load->pltt->fmt = mcss->plttProxy.fmt;
    load->pltt->bExtendedPlt = FALSE;
    load->pltt->szByte = mcss->plttSize;
    load->pltt->pRawData = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), mcss->plttSize, FALSE, "mcss.c", 1955);
    MI_CpuCopy16(mcss->pltt, load->pltt->pRawData, mcss->plttSize);
    if (mcss->blendTcb != NULL) {
        func_02016c38(mcss->blendTcb);
        mcss->blendTcb = NULL;
    }
    if (system->tcbMgr != NULL) {
        mcss->blendTcb = GFL_TCBMgrAddTask(system->tcbMgr, func_0201691c, load, 0);
    } else {
        mcss->blendTcb = GFL_VBlankTCBAdd(func_0201691c, load, 0);
    }
    mcss->blended = FALSE;
}

// pokebw2's MCSS_RestartAnimation
void func_02016374(MCSS *mcss) {
    NNS_G2dRestartMCAnimation(&mcss->mcAnim);
}

void func_02016380(MCSS *mcss) {
    mcss->restart = TRUE;
}

void func_02016394(MCSS *mcss, BOOL flag) {
    mcss->reverse = flag;
}

void func_020163b0(MCSSSystem *system, u32 value) {
    system->cellDepth = value;
}

void func_020163b4(MCSSSystem *system, BOOL flag) {
    system->orthoNear = flag;
}

// Reads a sprite's files and starts the load of its characters and palette
static void LoadMCSSGraphicsData(MCSSSystem *system, int index, const MCSSLoadInfo *info) {
    MCSS *mcss = system->mcss[index];
    MCSSLoad *load;
    HeapID heapId = info->unk20 == 1 ? HEAPID_TAIL((HeapID)mcss->heapId) : mcss->heapId;

    if (system->arc != NULL && system->arcId == info->arcId) {
        func_02016630(system, index, info);
        return;
    }

    mcss->loaded = FALSE;
    NNS_G2dInitImageProxy(&mcss->imageProxy);
    NNS_G2dInitImagePaletteProxy(&mcss->plttProxy);
    mcss->cellFile = GFL_G2DIOReadNCER(info->arcId, info->cells, FALSE, &mcss->cellBank, heapId);
    mcss->animFile = GFL_G2DIOReadNANR(info->arcId, info->cellAnime, TRUE, &mcss->animBank, heapId);
    mcss->mcFile = GFL_G2DIOReadNMCR(info->arcId, info->multiCells, FALSE, &mcss->mcBank, heapId);
    mcss->mcAnimFile = GFL_G2DIOReadNMAR(info->arcId, info->multiCellAnime, FALSE, &mcss->mcAnimBank, heapId);
    func_02016028(mcss, 0);
    mcss->bin = GFL_ArcSysReadHeapNew(info->arcId, info->bin, heapId);
    mcss->binTail = (MCSSBinTail *)((u8 *)mcss->bin + ((sizeof(MCSSBin) + mcss->bin->count * sizeof(MCSSCell)) & ~3));

    load = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), sizeof(MCSSLoad), TRUE, "mcss.c", 2112);
    load->system = system;
    load->inTcbMgr = system->tcbMgr != NULL;
    load->imageProxy = &mcss->imageProxy;
    load->plttProxy = &mcss->plttProxy;
    load->charAddr = system->texBase + index * MCSS_TEX_SIZE;
    load->plttAddr = system->plttBase + index * MCSS_PLTT_SIZE;
    load->mcss = mcss;
    load->charFile =
        GFL_G2DIOReadBGNCGR(info->arcId, info->character, TRUE, &load->chars, HEAPID_TAIL((HeapID)mcss->heapId));
    load->plttFile = GFL_G2DIOReadNCLR(info->arcId, info->palette, &load->pltt, HEAPID_TAIL((HeapID)mcss->heapId));
    mcss->pltt = GFL_HeapAllocate(heapId, load->pltt->szByte, FALSE, "mcss.c", 2136);
    mcss->blendedPltt = GFL_HeapAllocate(heapId, load->pltt->szByte, FALSE, "mcss.c", 2137);
    mcss->plttSize = load->pltt->szByte;
    MI_CpuCopy16(load->pltt->pRawData, mcss->pltt, load->pltt->szByte);
    if (system->loadCallback != NULL) {
        if (system->loadCallback(info, load, system->loadCallbackWork) == TRUE) {
            system->loadCallback = NULL;
            system->loadCallbackWork = 0;
        }
    }
    if (mcss->mosaic != 0) {
        func_02016c60(mcss, load);
    }
    if (system->tcbMgr != NULL) {
        mcss->loadTcb = GFL_TCBMgrAddTask(system->tcbMgr, func_02016880, load, 0);
    } else {
        mcss->loadTcb = GFL_VBlankTCBAdd(func_02016880, load, 0);
    }
}

// LoadMCSSGraphicsData from the archive that the system has open
// pokebw2's LoadMCSSGraphicsDataArc
static void func_02016630(MCSSSystem *system, int index, const MCSSLoadInfo *info) {
    MCSS *mcss = system->mcss[index];
    ArcTool *arc = system->arc;
    MCSSLoad *load;

    mcss->loaded = FALSE;
    NNS_G2dInitImageProxy(&mcss->imageProxy);
    NNS_G2dInitImagePaletteProxy(&mcss->plttProxy);
    mcss->cellFile = GFL_G2DIOReadNCERArc(arc, info->cells, FALSE, &mcss->cellBank, mcss->heapId);
    mcss->animFile = GFL_G2DIOReadNANRArc(arc, info->cellAnime, TRUE, &mcss->animBank, mcss->heapId);
    mcss->mcFile = GFL_G2DIOReadNMCRArc(arc, info->multiCells, FALSE, &mcss->mcBank, mcss->heapId);
    mcss->mcAnimFile = GFL_G2DIOReadNMARArc(arc, info->multiCellAnime, FALSE, &mcss->mcAnimBank, mcss->heapId);
    func_02016028(mcss, 0);
    mcss->bin = GFL_ArcToolReadHeapNew(arc, info->bin, mcss->heapId);
    mcss->binTail = (MCSSBinTail *)((u8 *)mcss->bin + ((sizeof(MCSSBin) + mcss->bin->count * sizeof(MCSSCell)) & ~3));

    load = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), sizeof(MCSSLoad), TRUE, "mcss.c", 2219);
    load->system = system;
    load->inTcbMgr = system->tcbMgr != NULL;
    load->imageProxy = &mcss->imageProxy;
    load->plttProxy = &mcss->plttProxy;
    load->charAddr = system->texBase + index * MCSS_TEX_SIZE;
    load->plttAddr = system->plttBase + index * MCSS_PLTT_SIZE;
    load->mcss = mcss;
    load->charFile =
        GFL_G2DIOReadBGNCGRArc(arc, info->character, TRUE, &load->chars, HEAPID_TAIL((HeapID)mcss->heapId));
    load->plttFile = GFL_G2DIOReadNCLRArc(arc, info->palette, &load->pltt, HEAPID_TAIL((HeapID)mcss->heapId));
    mcss->pltt = GFL_HeapAllocate(mcss->heapId, load->pltt->szByte, FALSE, "mcss.c", 2243);
    mcss->blendedPltt = GFL_HeapAllocate(mcss->heapId, load->pltt->szByte, FALSE, "mcss.c", 2244);
    mcss->plttSize = load->pltt->szByte;
    MI_CpuCopy16(load->pltt->pRawData, mcss->pltt, load->pltt->szByte);
    if (system->loadCallback != NULL) {
        if (system->loadCallback(info, load, system->loadCallbackWork) == TRUE) {
            system->loadCallback = NULL;
            system->loadCallbackWork = 0;
        }
    }
    if (mcss->mosaic != 0) {
        func_02016c60(mcss, load);
    }
    if (system->tcbMgr != NULL) {
        mcss->loadTcb = GFL_TCBMgrAddTask(system->tcbMgr, func_02016880, load, 0);
    } else {
        mcss->loadTcb = GFL_VBlankTCBAdd(func_02016880, load, 0);
    }
}

// Loads a sprite's characters and palette to VRAM. With a mosaic the characters go a frame before the palette
// pokebw2's MCSS_LoadTask
static void func_02016880(TCB *tcb, void *data) {
    MCSSLoad *load = data;

    if (load->inTcbMgr == FALSE) {
        u32 line = GX_GetVCount();

        if (line < MCSS_LOAD_LINE_MIN || line > MCSS_LOAD_LINE_MAX) {
            return;
        }
    }
    if (load->charFile != NULL) {
        NNS_G2dLoadImage2DMapping(load->chars, load->charAddr, NNS_G2D_VRAM_TYPE_3DMAIN, load->imageProxy);
        GFL_HeapFree(load->charFile);
        if (load->mcss->mosaic == 0) {
            load->charFile = NULL;
            return;
        }
    }
    if (load->mcss != NULL) {
        load->mcss->loaded = TRUE;
    }
    if (load->plttFile != NULL) {
        NNS_G2dLoadPalette(load->pltt, load->plttAddr, NNS_G2D_VRAM_TYPE_3DMAIN, load->plttProxy);
        GFL_HeapFree(load->plttFile);
    }
    if (load->mcss != NULL) {
        load->mcss->loadTcb = NULL;
    }
    if (load->system != NULL && load->system->shadowTcb != NULL) {
        load->system->shadowTcb = NULL;
    }
    GFL_HeapFree(load);
    GFL_TCBRemove(tcb);
}

// Loads a palette that the system made to VRAM
// pokebw2's MCSS_PaletteTask
static void func_0201691c(TCB *tcb, void *data) {
    MCSSLoad *load = data;

    if (load->inTcbMgr == FALSE) {
        u32 line = GX_GetVCount();

        if (line < MCSS_LOAD_LINE_MIN || line > MCSS_LOAD_LINE_MAX) {
            return;
        }
    }
    NNS_G2dInitImagePaletteProxy(load->plttProxy);
    NNS_G2dLoadPalette(load->pltt, load->plttAddr, NNS_G2D_VRAM_TYPE_3DMAIN, load->plttProxy);
    if (load->mcss != NULL) {
        if (load->mcss->fadeTcb == tcb) {
            load->mcss->fadeTcb = NULL;
        }
        if (load->mcss->blendTcb == tcb) {
            load->mcss->blendTcb = NULL;
        }
    }
    GFL_HeapFree(load->pltt->pRawData);
    GFL_HeapFree(load->pltt);
    GFL_HeapFree(load);
    GFL_TCBRemove(tcb);
}

// pokebw2's MCSS_InitMCAnime
static void func_02016984(MCSS *mcss, u32 mcType) {
    u32 size = NNS_G2dGetMCWorkAreaSize(mcss->mcBank, mcType);

    mcss->mcWork = GFL_HeapAllocate(mcss->heapId, size, FALSE, "mcss.c", 2416);
    NNS_G2dInitMCAnimationInstance(&mcss->mcAnim, mcss->mcWork, mcss->animBank, mcss->cellBank, mcss->mcBank, mcType);
}

// pokebw2's MCSS_SetMaterial
void func_020169d4(void) {
    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(16, 16, 16), TRUE);
    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), GX_RGB(0, 0, 0), FALSE);
}

// A step of a sprite's palette fade
// pokebw2's MCSS_UpdatePaletteFade
static void func_020169e8(MCSSSystem *system, MCSS *mcss) {
    if (mcss->fadeWaitCount == 0) {
        MCSSLoad *load = GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), sizeof(MCSSLoad), TRUE, "mcss.c", 2465);

        load->system = system;
        load->inTcbMgr = system->tcbMgr != NULL;
        load->mcss = mcss;
        load->plttProxy = &mcss->plttProxy;
        load->plttAddr = system->plttBase + mcss->index * MCSS_PLTT_SIZE;
        load->pltt =
            GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), sizeof(NNSG2dPaletteData), FALSE, "mcss.c", 2473);
        load->pltt->fmt = mcss->plttProxy.fmt;
        load->pltt->bExtendedPlt = FALSE;
        load->pltt->szByte = mcss->plttSize;
        load->pltt->pRawData =
            GFL_HeapAllocate(HEAPID_TAIL((HeapID)mcss->heapId), mcss->plttSize, FALSE, "mcss.c", 2478);
        if (mcss->blended) {
            func_02021f64(mcss->blendedPltt, load->pltt->pRawData, mcss->plttSize / 2, mcss->fadeLevel, mcss->fadeColor);
        } else {
            func_02021f64(mcss->pltt, load->pltt->pRawData, mcss->plttSize / 2, mcss->fadeLevel, mcss->fadeColor);
        }
        if (mcss->fadeTcb != NULL) {
            func_02016c38(mcss->fadeTcb);
            mcss->fadeTcb = NULL;
        }
        if (system->tcbMgr != NULL) {
            mcss->fadeTcb = GFL_TCBMgrAddTask(system->tcbMgr, func_0201691c, load, 0);
        } else {
            mcss->fadeTcb = GFL_VBlankTCBAdd(func_0201691c, load, 0);
        }
        if (mcss->fadeLevel == mcss->fadeEnd) {
            mcss->fading = FALSE;
        } else {
            mcss->fadeLevel += mcss->fadeStep;
            if ((mcss->fadeStep >= 0 && mcss->fadeLevel >= mcss->fadeEnd) ||
                (mcss->fadeStep < 0 && mcss->fadeLevel <= mcss->fadeEnd)) {
                mcss->fadeLevel = mcss->fadeEnd;
            }
        }
        mcss->fadeWaitCount = mcss->fadeWait;
    } else {
        mcss->fadeWaitCount--;
    }
}

// Frees a sprite's files and stops its load
// pokebw2's MCSS_FreeFiles
static void func_02016ba0(MCSS *mcss) {
    GFL_HeapFree(mcss->cellFile);
    GFL_HeapFree(mcss->animFile);
    GFL_HeapFree(mcss->mcFile);
    GFL_HeapFree(mcss->mcAnimFile);
    GFL_HeapFree(mcss->mcWork);
    GFL_HeapFree(mcss->bin);
    GFL_HeapFree(mcss->pltt);
    GFL_HeapFree(mcss->blendedPltt);
    mcss->cellFile = NULL;
    mcss->animFile = NULL;
    mcss->mcFile = NULL;
    mcss->mcAnimFile = NULL;
    mcss->mcWork = NULL;
    mcss->bin = NULL;
    mcss->pltt = NULL;
    mcss->blendedPltt = NULL;
    if (mcss->loadTcb != NULL) {
        func_02016c0c(mcss->loadTcb);
        mcss->loadTcb = NULL;
    }
}

// Removes a task of func_02016880
static void func_02016c0c(TCB *tcb) {
    MCSSLoad *load = GFL_TCBGetData(tcb);

    if (load->charFile != NULL) {
        GFL_HeapFree(load->charFile);
    }
    if (load->plttFile != NULL) {
        GFL_HeapFree(load->plttFile);
    }
    GFL_HeapFree(load);
    GFL_TCBRemove(tcb);
}

// Removes a task of func_0201691c
static void func_02016c38(TCB *tcb) {
    MCSSLoad *load = GFL_TCBGetData(tcb);

    GFL_HeapFree(load->pltt->pRawData);
    GFL_HeapFree(load->pltt);
    GFL_HeapFree(load);
    GFL_TCBRemove(tcb);
}

// Makes a mosaic of a sprite's characters: fills each block of the mosaic's size in each quad with the first color
// found in it
// pokebw2's MCSS_MakeMosaic
static void func_02016c60(MCSS *mcss, MCSSLoad *load) {
    int texWidth;
    u8 *chars;
    u32 i;
    int s;
    int t;
    int width;
    int height;
    int size;
    int color;
    int found;
    int x;
    int y;
    int blockY;
    int blockX;
    int s2;
    int t2;
    int width2;
    int height2;
    int blockY2;
    int blockX2;
    int blockWidth;
    int blockHeight;
    int k;
    int j;
    int pixel;
    int column;
    int nibble;
    int blockTop;
    int blockTop2;
    int searchRow;
    int blockLeft;
    int fillRow;
    int fillRow2;
    int searchRow2;
    int blockLeft2;

    if (mcss->imageProxy.attr.fmt != GX_TEXFMT_PLTT16) {
        return;
    }
    texWidth = pow(2, mcss->imageProxy.attr.sizeS + 2);
    chars = load->chars->pRawData;
    for (i = 0; i < mcss->bin->count; i++) {
        s = mcss->bin->cells[i].quad.s >> FX32_SHIFT;
        t = mcss->bin->cells[i].quad.t >> FX32_SHIFT;
        width = mcss->bin->cells[i].quad.width >> FX32_SHIFT;
        height = mcss->bin->cells[i].quad.height >> FX32_SHIFT;
        size = mcss->mosaic + 1;
        for (blockY = 0, y = 0; y < height; blockY++, y += size) {
            for (blockX = 0, x = 0; x < width; blockX++, x += size) {
                int colorHi;

                blockWidth = x + size > width ? width - size * blockX : size;
                blockHeight = y + size > height ? height - size * blockY : size;
                found = 0;
                for (k = 0; k < blockHeight; k++) {
                    for (j = 0; j < blockWidth; j++) {
                        column = s + x + j;
                        searchRow = t + y + k;
                        pixel = column / 2 + searchRow * texWidth;
                        // BUG: The pixel's half of the byte is taken from the block's column, not the image's, which
                        // differ where a quad starts at an odd pixel
#ifdef BUGFIX
                        if (column & 1) {
#else
                        if (j & 1) {
#endif
                            nibble = (chars[pixel] & 0xf0) >> 4;
                        } else {
                            nibble = chars[pixel] & 0x0f;
                        }
                        if (nibble != 0) {
                            found = nibble;
                            break;
                        }
                    }
                    if (found != 0) {
                        break;
                    }
                }
                color = (u8)found;
                colorHi = color << 4;
                for (k = 0; k < blockHeight; k++) {
                    for (j = 0; j < blockWidth; j++) {
                        blockLeft = s + x;
                        column = blockLeft + j;
                        blockTop = t + y;
                        fillRow = blockTop + k;
                        pixel = column / 2 + fillRow * texWidth;
                        if (pixel < MCSS_TEX_SIZE) {
                            if (column & 1) {
                                chars[pixel] = (chars[pixel] & 0x0f) | colorHi;
                            } else {
                                chars[pixel] = (chars[pixel] & 0xf0) | color;
                            }
                        }
                    }
                }
            }
        }
        width2 = mcss->bin->cells[i].quad2.width;
        if (width2 != 0) {
            s2 = mcss->bin->cells[i].quad2.s >> FX32_SHIFT;
            t2 = mcss->bin->cells[i].quad2.t >> FX32_SHIFT;
            width2 >>= FX32_SHIFT;
            height2 = (mcss->bin->cells[i].quad2.height >> FX32_SHIFT) * 2;
            for (blockY2 = 0, y = 0; y < height2; blockY2++, y += size) {
                for (blockX2 = 0, x = 0; x < width2; blockX2++, x += size) {
                    int colorHi;

                    blockWidth = x + size > width2 ? width2 - size * blockX2 : size;
                    blockHeight = y + size > height2 ? height2 - size * blockY2 : size;
                    found = 0;
                    for (k = 0; k < blockHeight; k++) {
                        for (j = 0; j < blockWidth; j++) {
                            column = s2 + x + j;
                            searchRow2 = t2 + y + k;
                            pixel = column / 2 + searchRow2 * texWidth;
                            // BUG: The pixel's half of the byte is taken from the block's column, not the image's,
                            // which differ where a quad starts at an odd pixel
#ifdef BUGFIX
                            if (column & 1) {
#else
                            if (j & 1) {
#endif
                                nibble = (chars[pixel] & 0xf0) >> 4;
                            } else {
                                nibble = chars[pixel] & 0x0f;
                            }
                            if (nibble != 0) {
                                found = nibble;
                                break;
                            }
                        }
                        if (found != 0) {
                            break;
                        }
                    }
                    color = (u8)found;
                    colorHi = color << 4;
                    for (k = 0; k < blockHeight; k++) {
                        for (j = 0; j < blockWidth; j++) {
                            blockLeft2 = s2 + x;
                            column = blockLeft2 + j;
                            blockTop2 = t2 + y;
                            fillRow2 = blockTop2 + k;
                            pixel = column / 2 + fillRow2 * texWidth;
                            if (pixel < MCSS_TEX_SIZE) {
                                if (column & 1) {
                                    chars[pixel] = (chars[pixel] & 0x0f) | colorHi;
                                } else {
                                    chars[pixel] = (chars[pixel] & 0xf0) | color;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// A vector times a 4x4 matrix, with the w that the result is to be divided by
// pokebw2's MCSS_MulVecMtx44
void func_02016fc8(const VecFx32 *vec, const MtxFx44 *mtx, VecFx32 *dest, fx32 *w) {
    fx64 y = vec->y;
    fx64 x = vec->x;
    fx64 z = vec->z;

    dest->x = (fx32)((x * mtx->m[0][0] + y * mtx->m[1][0] + z * mtx->m[2][0]) >> FX32_SHIFT);
    dest->x += mtx->m[3][0];
    dest->y = (fx32)((x * mtx->m[0][1] + y * mtx->m[1][1] + z * mtx->m[2][1]) >> FX32_SHIFT);
    dest->y += mtx->m[3][1];
    dest->z = (fx32)((x * mtx->m[0][2] + y * mtx->m[1][2] + z * mtx->m[2][2]) >> FX32_SHIFT);
    dest->z += mtx->m[3][2];
    *w = (fx32)((x * mtx->m[0][3] + y * mtx->m[1][3] + z * mtx->m[2][3]) >> FX32_SHIFT);
    *w += mtx->m[3][3];
}
