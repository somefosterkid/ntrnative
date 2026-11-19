#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"

struct BgConfig;

struct BgTemplate;

struct GXBanks;

struct GraphicsModes;

void SetBothScreensModesAndDisable(const GraphicsModes *modes);

void ToggleBgLayer(u8 bgLayer, u8 enable);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

typedef struct GXBanks {
    GXVRamBG unk_00;
    GXVRamBGExtPltt unk_04;
    GXVRamSubBG unk_08;
    GXVRamSubBGExtPltt unk_0C;
    GXVRamOBJ unk_10;
    GXVRamOBJExtPltt unk_14;
    GXVRamSubOBJ unk_18;
    GXVRamSubOBJExtPltt unk_1C;
    GXVRamTex unk_20;
    GXVRamTexPltt unk_24;
} GXBanks;

void GfGfx_DisableEngineAPlanes(void);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

BgConfig *BgConfig_Alloc(enum HeapID heapId);

void *NARC_New(u32 narcId, enum HeapID heapId);

extern const u8 _021E954C[];

typedef struct UnkStruct_ov99_021E5C88 {
    u32 unk00;
    BgConfig *bgConfig;
    void *narc;
    enum HeapID heapId;
} UnkStruct_ov99_021E5C88;

typedef struct BgTemplateSet {
    BgTemplate t[8];
} BgTemplateSet;

extern const GraphicsModes ov99_021E9564;

extern const BgTemplateSet ov99_021E9760;

void ov99_021E5C88(UnkStruct_ov99_021E5C88 *param0);

void ov99_021E5C88(UnkStruct_ov99_021E5C88 *param0) {
    param0->bgConfig = BgConfig_Alloc(param0->heapId);

    {
        GraphicsModes v1 = ov99_021E9564;

        SetBothScreensModesAndDisable(&v1);
    }

    {
        int v3;
        BgTemplateSet v2 = ov99_021E9760;

        for (v3 = 0; v3 < 8; v3++) {
            InitBgFromTemplate(param0->bgConfig, _021E954C[v3], &v2.t[v3], 0);
            BgClearTilemapBufferAndCommit(param0->bgConfig, _021E954C[v3]);
            BG_ClearCharDataRange(_021E954C[v3], 0x20, 0, param0->heapId);
        }

        G2_SetBlendAlpha(8, 4, 15, 0);
        G2S_SetBlendAlpha(8, 2, 15, 0);
    }

    param0->narc = NARC_New(0xb1, param0->heapId);
}
