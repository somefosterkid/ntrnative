// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/musical_mcss.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/musical_mcss.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "pml/poke_graphic.h"
#include "system/mcss.h"

// Overlay 209's musical_mcss.c: the musical's copy of MCSS. Each sprite is a multi-cell animation whose cells are drawn
// as textured quads of one image in texture VRAM, described by the sprite's bin file: for each cell, one or two quads
// and the marker cells that it carries. A marker cell is not drawn but reported to the draw callback, which places the
// props by them

// The bin file: a cell's quads, then the marker cells' table
typedef struct {
    fx32 x;
    fx32 y;
    fx32 width;
    fx32 height;
    fx32 s;
    fx32 t;
} MusicalMcssQuad;

typedef struct {
    MusicalMcssQuad quad;
    // Drawn when its width is not 0
    MusicalMcssQuad quad2;
    // The marker cells, 0xff after the last
    u8 marks[8];
} MusicalMcssCell;

typedef struct {
    u32 unk0;
    u32 kind;
    fx32 x;
    fx32 y;
    // In units of 0x800
    u32 rotation;
} MusicalMcssMark;

typedef struct {
    u32 count;
    MusicalMcssCell cells[];
} MusicalMcssBin;

struct MusicalMcssSys {
    int count;
    MusicalMcss **mcss;
    // Whether the sprites are drawn with an orthographic projection of their own
    u32 ortho : 1;
    u32 texBase;
    u32 plttBase;
    u32 heapId;
    // The V-blank count at the last update, to step the animations by the frames since
    u32 vblankCount;
};

struct MusicalMcss {
    void *cellFile;
    NNSG2dCellDataBank *cellBank;
    void *mcFile;
    NNSG2dMultiCellDataBank *mcBank;
    void *animFile;
    NNSG2dAnimBankData *animBank;
    void *mcAnimFile;
    NNSG2dAnimBankData *mcAnimBank;
    MusicalMcssBin *bin;
    MusicalMcssMark *marks;
    NNSG2dMultiCellAnimation mcAnim;
    void *mcWork;
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy plttProxy;
    VecFx32 pos;
    VecFx32 scale;
    VecFx32 unkE0;
    u32 flip : 1;
    u32 stopAnime : 1;
    u32 hidden : 1;
    // Its slot in the system
    u32 index;
    u32 heapId;
    void *work;
    u16 rotation;
    // The center of rotation, which marker cell 5 sets, and how far rotating moves it
    VecFx32 center;
    VecFx32 centerOffset;
    VecFx32 unk118;
};

// A sprite's characters and palette, loaded into VRAM at once or at the next V-blank
typedef struct {
    NNSG2dCharacterData *chars;
    NNSG2dPaletteData *pltt;
    NNSG2dImageProxy *imageProxy;
    NNSG2dImagePaletteProxy *plttProxy;
    void *charFile;
    void *plttFile;
    u32 charAddr;
    u32 plttAddr;
    MusicalMcss *mcss;
} MusicalMcssLoad;

// The files of Black and White's layout of the Pokémon graphics archive
#define MUSICAL_POKEGRA_EGG 650
#define MUSICAL_POKEGRA_FORM_FILES 0x32f0
#define MUSICAL_POKEGRA_FORM_PALETTES 0x37ad
#define MUSICAL_POKEGRA_SPINDA_BIN (SPECIES_SPINDA * POKEGRA_FILE_COUNT + POKEGRA_FILE_BIN)

// The files of each species' sprite, as in pokegra.c
#define POKEGRA_FILE_SINGLE_CELL_CHARS 0
#define POKEGRA_FILE_CHARS 2
#define POKEGRA_FILE_CELLS 4
#define POKEGRA_FILE_CELL_ANIMS 5
#define POKEGRA_FILE_MULTI_CELLS 6
#define POKEGRA_FILE_MULTI_CELL_ANIMS 7
#define POKEGRA_FILE_BIN 8
#define POKEGRA_FILE_BACK 9
#define POKEGRA_FILE_PALETTE 18
#define POKEGRA_FILE_COUNT 20

void func_ov112_021f5edc(MusicalMcss *mcss, fx32 x, fx32 y, fx32 width, fx32 height, fx32 s, fx32 t,
                                 const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt, u32 node, BOOL ortho,
                                 fx32 *z, u8 flip);
static void func_ov112_021f63b8(MusicalMcssSys *sys, int index, MCSSLoadInfo *info, BOOL atVBlank);
static void func_ov112_021f6540(TCB *tcb, void *data);
static void func_ov112_021f659c(MusicalMcss *mcss, u32 mcType);
void func_ov112_021f65ec(void);
void func_ov112_021f660c(const VecFx32 *vec, const MtxFx44 *mtx, VecFx32 *dest, fx32 *w);

// pokebw2's MUSICAL_MCSS_ORTHO_POS
static const VecFx32 data_ov112_021f8310 = { 0, 0, FX32_CONST(301) };
// pokebw2's MUSICAL_MCSS_ORTHO_TARGET
static const VecFx32 data_ov112_021f8304 = { 0, 0, 0 };
// pokebw2's MUSICAL_MCSS_ORTHO_UP
static const VecFx32 data_ov112_021f831c = { 0, FX32_ONE, 0 };

// pokebw2's MusicalMcss_InitSystem
MusicalMcssSys *func_ov112_021f55f8(u32 count, HeapID heapId) {
    MusicalMcssSys *sys = GFL_HeapAllocate(heapId, sizeof(MusicalMcssSys), TRUE, "musical_mcss.c", 132);

    sys->count = count;
    sys->heapId = heapId;
    sys->mcss = GFL_HeapAllocate(heapId, count * sizeof(MusicalMcss *), TRUE, "musical_mcss.c", 137);
    sys->vblankCount = OS_GetVBlankCount();
    sys->texBase = 0x30000;
    sys->plttBase = 0x1000;
    return sys;
}

// pokebw2's MusicalMcss_TermSystem
void func_ov112_021f5644(MusicalMcssSys *sys) {
    int i;

    for (i = 0; i < sys->count; i++) {
        if (sys->mcss[i] != NULL) {
            func_ov112_021f61d8(sys, sys->mcss[i]);
        }
    }
    GFL_HeapFree(sys->mcss);
    GFL_HeapFree(sys);
}

// pokebw2's MusicalMcss_UpdateSystem
void func_ov112_021f5678(MusicalMcssSys *sys) {
    int i;
    u32 vblankCount = OS_GetVBlankCount();
    u32 frames = vblankCount - sys->vblankCount;

    for (i = 0; i < sys->count; i++) {
        if (sys->mcss[i] != NULL && sys->mcss[i]->stopAnime == FALSE) {
            NNS_G2dTickMCAnimation(&sys->mcss[i]->mcAnim, frames * FX32_ONE);
        }
    }
    sys->vblankCount = vblankCount;
}

// The current frame of an animation, as the SRT it may hold
static inline void MusicalMcss_GetAnimSrt(const NNSG2dAnimController *animCtrl, NNSG2dAnimDataSRT *srt) {
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

// pokebw2's MusicalMcss_DrawSystem
void func_ov112_021f56bc(MusicalMcssSys *sys, MusicalMcssCellCallback callback) {
    const MusicalMcssCell *cell;
    MusicalMcss *mcss;
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
    MtxFx44 viewProj;
    MtxFx44 viewport;
    MtxFx33 centerRotation;
    MusicalMcssCellInfo info;
    fx32 x;
    fx32 y;
    fx32 z;
    fx32 w;
    u8 k;
    int rot;
    fx32 markX;
    fx32 markY;
    fx32 nodeX;
    fx32 nodeY;
    u8 flip;

    G3_PushMtx();
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_StoreMtx(0);
    G3_MtxMode(GX_MTXMODE_TEXTURE);
    G3_Identity();
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    if (sys->ortho == FALSE) {
        G3i_LookAt_(&NNS_G3dGlb.camPos, &NNS_G3dGlb.camUp, &NNS_G3dGlb.camTarget, TRUE, NULL);
    } else {
        G3i_LookAt_(&data_ov112_021f8310, &data_ov112_021f831c, &data_ov112_021f8304, TRUE, NULL);
    }
    MTX_Inverse43(&NNS_G3dGlb.cameraMtx, &invCamera);
    MI_Copy36B(&invCamera, &rotation);
    MTX_Copy33To44_(&rotation, &billboard);

    for (i = 0; i < sys->count; i++) {
        NNSG2dMultiCellAnimation *mcAnim;

        if (sys->mcss[i] == NULL || sys->mcss[i]->hidden) {
            continue;
        }
        G3_PushMtx();
        mcss = sys->mcss[i];
        imageProxy = &mcss->imageProxy;
        mcAnim = &mcss->mcAnim;
        cellAnims = (NNSG2dMCCellAnimation **)&mcss->mcAnim.multiCellInstance.pCellAnimInstasnces;
        z = 0;
        MusicalMcss_GetAnimSrt(&mcAnim->animCtrl, &mcSrt);
        pos.x = mcss->pos.x;
        pos.y = mcss->pos.y;
        pos.z = mcss->pos.z;
        if (sys->ortho == FALSE) {
            x = mcSrt.px << 8;
            y = -mcSrt.py << 8;
        } else {
            viewport.m[0][0] = FX32_CONST(128);
            viewport.m[0][1] = 0;
            viewport.m[0][2] = 0;
            viewport.m[0][3] = FX32_CONST(128);
            viewport.m[1][0] = 0;
            viewport.m[1][1] = FX32_CONST(96);
            viewport.m[1][2] = 0;
            viewport.m[1][3] = FX32_CONST(-96);
            viewport.m[2][0] = 0;
            viewport.m[2][1] = 0;
            viewport.m[2][2] = -FX32_ONE;
            viewport.m[2][3] = FX32_ONE;
            viewport.m[3][0] = 0;
            viewport.m[3][1] = 0;
            viewport.m[3][2] = 0;
            viewport.m[3][3] = -FX32_ONE;
            MTX_Copy43To44_(&NNS_G3dGlb.cameraMtx, &viewProj);
            MTX_Concat44(&viewProj, &NNS_G3dGlb.projMtx, &viewProj);
            func_ov112_021f660c(&pos, &viewProj, &pos, &w);
            pos.x = FX_Div(pos.x, w);
            pos.y = FX_Div(pos.y, w);
            func_ov112_021f660c(&pos, &viewport, &pos, &w);
            x = FX32_CONST(mcSrt.px);
            y = FX32_CONST(-mcSrt.py);
        }
        pos.z = mcss->pos.z;
        flip = 0;
        if (mcss->scale.x < 0) {
            flip++;
        }
        G3_Translate(pos.x, pos.y, pos.z);
        if (sys->ortho == FALSE) {
            G3_MultMtx44(&billboard);
        }
        G3_Translate(x, y, 0);
        {
            u16 rot = (flip & 1) ? 0x10000 - mcSrt.rotZ : mcSrt.rotZ;

            G3_RotZ(-FX_SinIdx(rot), FX_CosIdx(rot));
        }
        G3_Scale(FX_Mul(mcSrt.sx, mcss->scale.x), FX_Mul(mcSrt.sy, mcss->scale.y), FX32_ONE);
        MTX_RotZ33_(&centerRotation, -FX_SinIdx(mcss->rotation), FX_CosIdx(mcss->rotation));
        MTX_MultVec33(&mcss->center, &centerRotation, &mcss->centerOffset);
        VEC_Subtract(&mcss->center, &mcss->centerOffset, &mcss->centerOffset);
        mcss->centerOffset.x = FX_Div(mcss->centerOffset.x, mcss->scale.x);
        mcss->centerOffset.y = FX_Div(mcss->centerOffset.y, mcss->scale.y);
        G3_StoreMtx(29);
        func_ov112_021f65ec();
        G3_TexImageParam(imageProxy->attr.fmt, GX_TEXGEN_TEXCOORD, imageProxy->attr.sizeS, imageProxy->attr.sizeT,
                         GX_TEXREPEAT_ST, GX_TEXFLIP_ST, imageProxy->attr.plttUse,
                         imageProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_3DMAIN]);

        for (j = 0; j < mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt.index].numNodes; j++) {
            NNSG2dMCCellAnimation *cellAnim;

            cellAnim =
                &(*cellAnims)
                    [(mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt.index].pHierDataArray[j].nodeAttr &
                      0xff00) >>
                     8];
            MusicalMcss_GetAnimSrt(&cellAnim->cellAnim.animCtrl, &cellSrt);
            cell = &mcss->bin->cells[cellSrt.index];
            if (cell->quad2.width != 0) {
                if (mcss->flip) {
                    func_ov112_021f5edc(mcss, cell->quad2.x, cell->quad2.y, cell->quad2.width, cell->quad2.height,
                                        cell->quad2.s, cell->quad2.t + cell->quad2.height, &cellSrt, &mcSrt, j,
                                        sys->ortho, &z, flip);
                } else {
                    func_ov112_021f5edc(mcss, cell->quad2.x, cell->quad2.y, cell->quad2.width, cell->quad2.height,
                                        cell->quad2.s, cell->quad2.t, &cellSrt, &mcSrt, j, sys->ortho, &z, flip);
                }
            }
            func_ov112_021f5edc(mcss, cell->quad.x, cell->quad.y, cell->quad.width, cell->quad.height, cell->quad.s,
                                cell->quad.t, &cellSrt, &mcSrt, j, sys->ortho, &z, flip);

            for (k = 0; k < 8; k++) {
                u8 mark;
                fx32 offsetX;
                fx32 offsetY;
                fx32 cos;
                fx32 sin;
                fx32 scaleX;
                fx32 scaleY;

                mark = cell->marks[k];
                if (mark == 0xff) {
                    break;
                }
                rot = (u16)(mcSrt.rotZ + cellSrt.rotZ);
                markX = mcss->marks[mark].x;
                markY = mcss->marks[mark].y;
                sin = FX_SinIdx(rot);
                cos = FX_CosIdx(rot);
                scaleX = FX_Mul(mcSrt.sx, cellSrt.sx);
                scaleY = FX_Mul(mcSrt.sy, cellSrt.sy);
                offsetX = FX_Mul(markX * cos - markY * sin, scaleX);
                offsetY = FX_Mul(markX * sin + markY * cos, scaleY);
                nodeX = mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt.index].pHierDataArray[j].posX;
                nodeX += mcSrt.px;
                nodeY = mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt.index].pHierDataArray[j].posY;
                nodeY += mcSrt.py;
                offsetX += FX_Mul(nodeX * FX_CosIdx(mcSrt.rotZ) - nodeY * FX_SinIdx(mcSrt.rotZ), mcSrt.sx);
                offsetY += FX_Mul(nodeX * FX_SinIdx(mcSrt.rotZ) + nodeY * FX_CosIdx(mcSrt.rotZ), mcSrt.sy);
                offsetX += FX32_CONST(cellSrt.px);
                offsetY += FX32_CONST(cellSrt.py);

                info.pos = pos;
                info.pos.y = -info.pos.y;
                info.offset.x = FX_Mul(offsetX, mcss->scale.x / 16);
                info.offset.y = FX_Mul(offsetY, mcss->scale.y / 16);
                info.offset.z = z;
                info.center = mcss->center;
                info.rotation = mcss->rotation;
                info.scale = mcss->scale;
                info.cellRotation = rot + mcss->marks[mark].rotation * 0x800;
                callback((u8)mcss->marks[mark].kind, &info, mcss->work);
                if (mcss->marks[mark].kind == 5) {
                    VEC_Set(&mcss->center, info.offset.x, info.offset.y, 0);
                }
            }
        }
        G3_PopMtx(1);
    }
    G3_PopMtx(1);
}

void func_ov112_021f5edc(MusicalMcss *mcss, fx32 x, fx32 y, fx32 width, fx32 height, fx32 s, fx32 t,
                                 const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt, u32 node, BOOL ortho,
                                 fx32 *z, u8 flip) {
    u16 rot;
    fx32 nodeX;
    fx32 nodeY;

    if (ortho) {
        G3i_OrthoW_(FX32_CONST(96), FX32_CONST(-95), FX32_CONST(-127), FX32_CONST(128), FX32_ONE, FX32_CONST(1000),
                 FX32_ONE, TRUE, NULL);
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    }
    G3_RestoreMtx(29);
    G3_TexPlttBase(mcss->plttProxy.vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_3DMAIN], mcss->plttProxy.fmt);
    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);
    nodeX =
        (cellSrt->px + mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt->index].pHierDataArray[node].posX)
        << 8;
    nodeY =
        -(cellSrt->py + mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt->index].pHierDataArray[node].posY)
        << 8;
    if (flip == 0) {
        rot = mcss->rotation;
    } else if (flip & 1) {
        rot = 0x10000 - mcss->rotation;
    }
    G3_RotZ(-FX_SinIdx(rot), FX_CosIdx(rot));
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
    G3_TexCoord(s, t);
    G3_Vtx(0, 0, 0);
    G3_TexCoord(s + width, t);
    G3_Vtx(0x100, 0, 0);
    G3_TexCoord(s + width, t + height);
    G3_Vtx(0x100, -0x100, 0);
    G3_TexCoord(s, t + height);
    G3_Vtx(0, -0x100, 0);
    G3_End();
    G3_MtxMode(GX_MTXMODE_PROJECTION);
    G3_RestoreMtx(0);
    G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
    if (ortho == FALSE) {
        *z -= 0x100;
    } else {
        *z -= 0x400;
    }
}

MusicalMcss *func_ov112_021f611c(MusicalMcssSys *sys, fx32 x, fx32 y, fx32 z, MCSSLoadInfo *info, void *work,
                             BOOL loadAtVBlank) {
    int i;

    for (i = 0; i < sys->count; i++) {
        if (sys->mcss[i] == NULL) {
            sys->mcss[i] = GFL_HeapAllocate(sys->heapId, sizeof(MusicalMcss), TRUE, "musical_mcss.c", 763);
            sys->mcss[i]->index = i;
            sys->mcss[i]->heapId = sys->heapId;
            sys->mcss[i]->pos.x = x;
            sys->mcss[i]->pos.y = y;
            sys->mcss[i]->pos.z = z;
            sys->mcss[i]->rotation = 0;
            sys->mcss[i]->scale.x = FX32_ONE;
            sys->mcss[i]->scale.y = FX32_ONE;
            sys->mcss[i]->scale.z = FX32_ONE;
            sys->mcss[i]->work = work;
            func_ov112_021f63b8(sys, i, info, loadAtVBlank);
            break;
        }
    }
    return sys->mcss[i];
}

// pokebw2's MusicalMcss_Del
void func_ov112_021f61d8(MusicalMcssSys *sys, MusicalMcss *mcss) {
    GFL_HeapFree(mcss->cellFile);
    GFL_HeapFree(mcss->animFile);
    GFL_HeapFree(mcss->mcFile);
    GFL_HeapFree(mcss->mcAnimFile);
    GFL_HeapFree(mcss->mcWork);
    GFL_HeapFree(mcss->bin);
    sys->mcss[mcss->index] = NULL;
    GFL_HeapFree(mcss);
}

// pokebw2's MusicalMcss_SetOrthoMode
void func_ov112_021f621c(MusicalMcssSys *sys) {
    sys->ortho = TRUE;
}

// pokebw2's MusicalMcss_SetTexBase
void func_ov112_021f622c(MusicalMcssSys *sys, u32 base) {
    sys->texBase = base;
}

// pokebw2's MusicalMcss_SetPlttBase
void func_ov112_021f6230(MusicalMcssSys *sys, u32 base) {
    sys->plttBase = base;
}

// pokebw2's MusicalMcss_SetPosition
void func_ov112_021f6234(MusicalMcss *mcss, VecFx32 *pos) {
    mcss->pos.x = pos->x;
    mcss->pos.y = pos->y;
    mcss->pos.z = pos->z;
}

// pokebw2's MusicalMcss_SetScale
void func_ov112_021f624c(MusicalMcss *mcss, VecFx32 *scale) {
    mcss->scale.x = scale->x;
    mcss->scale.y = scale->y;
    mcss->scale.z = scale->z;
}

// pokebw2's MusicalMcss_SetRotation
void func_ov112_021f6264(MusicalMcss *mcss, u16 rotation) {
    mcss->rotation = rotation;
}

// pokebw2's MusicalMcss_SetFlip
void func_ov112_021f626c(MusicalMcss *mcss) {
    mcss->flip = TRUE;
}

// pokebw2's MusicalMcss_ResetFlip
void func_ov112_021f6280(MusicalMcss *mcss) {
    mcss->flip = FALSE;
}

// pokebw2's MusicalMcss_StopAnime
void func_ov112_021f6290(MusicalMcss *mcss) {
    mcss->stopAnime = TRUE;
}

// pokebw2's MusicalMcss_StartAnime
void func_ov112_021f62a0(MusicalMcss *mcss) {
    mcss->stopAnime = FALSE;
}

// pokebw2's MusicalMcss_ChangeAnime
void func_ov112_021f62b0(MusicalMcss *mcss, u16 anime) {
    const NNSG2dAnimSequence *seq = NNS_G2dGetAnimSequenceByIdx(mcss->mcAnimBank, anime);

    // BUG: The work of the old animation is never freed
#ifdef BUGFIX
    GFL_HeapFree(mcss->mcWork);
#endif
    func_ov112_021f659c(mcss, NNS_G2D_MCTYPE_SHARE_CELLANIM);
    NNS_G2dSetAnimSequenceToMCAnimation(&mcss->mcAnim, seq);
}

// pokebw2's MusicalMcss_IsHidden
BOOL func_ov112_021f62d0(MusicalMcss *mcss) {
    return mcss->hidden;
}

// pokebw2's MusicalMcss_Hide
void func_ov112_021f62dc(MusicalMcss *mcss) {
    mcss->hidden = TRUE;
}

// pokebw2's MusicalMcss_Show
void func_ov112_021f62ec(MusicalMcss *mcss) {
    mcss->hidden = FALSE;
}

// pokebw2's MusicalMcss_CopyState
void func_ov112_021f62fc(MusicalMcss *src, MusicalMcss *dst) {
    dst->pos = src->pos;
    dst->scale = src->scale;
    dst->unkE0 = src->unkE0;
    dst->flip = src->flip;
    dst->stopAnime = src->stopAnime;
    dst->hidden = src->hidden;
    dst->rotation = src->rotation;
    dst->center = src->center;
    dst->centerOffset = src->centerOffset;
}

// pokebw2's MusicalMcss_Load
static void func_ov112_021f63b8(MusicalMcssSys *sys, int index, MCSSLoadInfo *info, BOOL atVBlank) {
    MusicalMcss *mcss = sys->mcss[index];
    const NNSG2dAnimSequence *seq;
    MusicalMcssLoad *load;

    mcss->hidden = TRUE;
    NNS_G2dInitImageProxy(&mcss->imageProxy);
    NNS_G2dInitImagePaletteProxy(&mcss->plttProxy);
    mcss->cellFile = GFL_G2DIOReadNCER(info->arcId, info->cells, FALSE, &mcss->cellBank, mcss->heapId);
    mcss->animFile = GFL_G2DIOReadNANR(info->arcId, info->cellAnime, TRUE, &mcss->animBank, mcss->heapId);
    mcss->mcFile = GFL_G2DIOReadNMCR(info->arcId, info->multiCells, FALSE, &mcss->mcBank, mcss->heapId);
    mcss->mcAnimFile = GFL_G2DIOReadNMAR(info->arcId, info->multiCellAnime, FALSE, &mcss->mcAnimBank, mcss->heapId);
    seq = NNS_G2dGetAnimSequenceByIdx(mcss->mcAnimBank, 0);
    func_ov112_021f659c(mcss, NNS_G2D_MCTYPE_SHARE_CELLANIM);
    NNS_G2dSetAnimSequenceToMCAnimation(&mcss->mcAnim, seq);
    mcss->bin = GFL_ArcSysReadHeapNew(info->arcId, info->bin, mcss->heapId);
    mcss->marks = (MusicalMcssMark *)&mcss->bin->cells[mcss->bin->count];

    load = GFL_HeapAllocate(mcss->heapId, sizeof(MusicalMcssLoad), TRUE, "musical_mcss.c", 1104);
    load->imageProxy = &mcss->imageProxy;
    load->plttProxy = &mcss->plttProxy;
    load->charAddr = sys->texBase + index * 0x4000;
    load->plttAddr = sys->plttBase + index * 0x20;
    load->mcss = mcss;
    load->charFile = GFL_G2DIOReadBGNCGR(info->arcId, info->character, TRUE, &load->chars, mcss->heapId);
    load->plttFile = GFL_G2DIOReadNCLR(info->arcId, info->palette, &load->pltt, mcss->heapId);
    if (info->bin == MUSICAL_POKEGRA_SPINDA_BIN) {
        func_020172d8(load->chars->pRawData, info->unk20);
    }
    if (atVBlank == TRUE) {
        GFL_VBlankTCBAdd(func_ov112_021f6540, load, 0);
    } else {
        func_ov112_021f6540(NULL, load);
    }
}

// pokebw2's MusicalMcss_LoadTask
static void func_ov112_021f6540(TCB *tcb, void *data) {
    MusicalMcssLoad *load = data;

    if (load->mcss != NULL) {
        load->mcss->hidden = FALSE;
    }
    if (load->charFile != NULL) {
        NNS_G2dLoadImage2DMapping(load->chars, load->charAddr, NNS_G2D_VRAM_TYPE_3DMAIN, load->imageProxy);
        GFL_HeapFree(load->charFile);
    }
    if (load->pltt != NULL) {
        NNS_G2dLoadPalette(load->pltt, load->plttAddr, NNS_G2D_VRAM_TYPE_3DMAIN, load->plttProxy);
        GFL_HeapFree(load->plttFile);
    }
    GFL_HeapFree(load);
    if (tcb != NULL) {
        GFL_TCBRemove(tcb);
    }
}

// pokebw2's MusicalMcss_InitMCAnime
static void func_ov112_021f659c(MusicalMcss *mcss, u32 mcType) {
    u32 size = NNS_G2dGetMCWorkAreaSize(mcss->mcBank, mcType);

    mcss->mcWork = GFL_HeapAllocate(mcss->heapId, size, FALSE, "musical_mcss.c", 1216);
    NNS_G2dInitMCAnimationInstance(&mcss->mcAnim, mcss->mcWork, mcss->animBank, mcss->cellBank, mcss->mcBank, mcType);
}

// pokebw2's MusicalMcss_SetMaterial
void func_ov112_021f65ec(void) {
    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(16, 16, 16), TRUE);
    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), GX_RGB(0, 0, 0), FALSE);
    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);
}

void func_ov112_021f660c(const VecFx32 *vec, const MtxFx44 *mtx, VecFx32 *dest, fx32 *w) {
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

BOOL func_ov112_021f6744(PartyPkm *pkm) {
    u32 result;
    result = PokeParty_GetParam(pkm, 0xaa, NULL);
    return result != 1;
}
