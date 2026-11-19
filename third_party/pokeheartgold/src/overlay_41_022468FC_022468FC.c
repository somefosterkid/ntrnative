#include "global.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "heap.h"
#include "pokepic.h"
#include "unk_0200B150.h"

struct BgConfig;

struct BgTemplate;

struct GF_2DGfxRawResMan;

struct NARC;

struct UnkStruct_ov41_02245EA0;

struct UnkStruct_ov41_022467E4_Template;

typedef struct UnkStruct_ov41_02245EA0 {
    void *unk00;
    u32 *unk04;
    int unk08;
    volatile int unk0C;
    u32 *unk10;
    int unk14;
    volatile int unk18;
    int unk1C;
    void *unk20;
    u8 unk24[0x8];
    int unk2C;
    u8 unk30[0x4];
    GF_2DGfxRawResMan *unk34;
    NNSG2dCharacterData **unk38;
    int unk3C;
    BgConfig *unk40;
    u8 unk44[0x13C];
    NARC *unk180;
} UnkStruct_ov41_02245EA0;

typedef struct UnkStruct_ov41_022467E4_Template UnkStruct_ov41_022467E4_Template;

void ov41_022464AC(void *param0, enum HeapID heapID);

void ov41_02246518(UnkStruct_ov41_02245EA0 *param0, const UnkStruct_ov41_022467E4_Template *param1, enum HeapID heapID);

void ov41_022468FC(UnkStruct_ov41_02245EA0 *param0);

extern const BgTemplate ov41_0224BFE0;

extern const BgTemplate ov41_0224BFFC;

extern const BgTemplate ov41_0224C034;

extern const BgTemplate ov41_0224BFC4;

extern const BgTemplate ov41_0224C050;

void ov41_022468FC(UnkStruct_ov41_02245EA0 *param0) {
    {
        BgTemplate v0 = ov41_0224BFE0;

        InitBgFromTemplate(param0->unk40, GF_BG_LYR_MAIN_1, &v0, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(param0->unk40, GF_BG_LYR_MAIN_1);
    }

    {
        BgTemplate v1 = ov41_0224BFFC;

        InitBgFromTemplate(param0->unk40, GF_BG_LYR_MAIN_2, &v1, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(param0->unk40, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v2 = ov41_0224C034;

        InitBgFromTemplate(param0->unk40, GF_BG_LYR_MAIN_3, &v2, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(param0->unk40, GF_BG_LYR_MAIN_3);
    }

    {
        BgTemplate v3 = ov41_0224BFC4;

        InitBgFromTemplate(param0->unk40, GF_BG_LYR_SUB_0, &v3, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 32, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(param0->unk40, GF_BG_LYR_SUB_0);
    }

    {
        BgTemplate v4 = ov41_0224C050;

        InitBgFromTemplate(param0->unk40, GF_BG_LYR_SUB_1, &v4, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 32, 0, HEAP_ID_14);
        BgClearTilemapBufferAndCommit(param0->unk40, GF_BG_LYR_SUB_1);
    }
}
