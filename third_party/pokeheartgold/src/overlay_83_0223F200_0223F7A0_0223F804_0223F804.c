#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "overlay_manager.h"
#include "unk_0202C034.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "message_format.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "unk_02013534.h"
#include "touch_hitbox_controller.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "overlay_45_unk_struct_ov66_02234798.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct OverlayManager;

struct BgConfig;

struct BgTemplate;

struct CharTransferAllocation;

struct ListMenuCursor;

struct TextOBJ;

struct G2dRenderer;

struct GraphicsModes;

struct ListMenu;

struct ListMenu2D;

struct MsgData;

struct NARC;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct TouchHitboxController;

struct PlayerProfile;

struct UnkStruct_0207E060_t;

struct UnkStruct_ov63_0222AE60_t;

struct UnkStruct_ov63_0222BB38_t;

struct UnkStruct_ov63_0222BCE8_t;

struct UnkStruct_ov63_0222BE18_t;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov63_0222D1C0_t;

struct UnkStruct_ov63_0222D848_t;

struct UnkStruct_ov65_0222EBE0_t;

struct UnkStruct_ov65_02235130_t;

struct UnkStruct_ov65_022354D8_t;

struct WiFiList;

struct Window;

void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

void SetBothScreensModesAndDisable(const GraphicsModes *graphicsModes);

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void SetBgPriority(u8 bgLayer, u8 priority);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

typedef struct UnkStruct_0207E060_t {
    u16 unk_00[6];
    u16 unk_0C[6];
    u8 unk_18;
    u8 language;
    u8 unk_1A;
    u8 unk_1B;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E;
    u8 unk_1F;
    u8 unk_20;
    u8 unk_21;
    u8 unk_22;
} UnkStruct_0207E060;

typedef struct {
    UnkStruct_0207E060 unk_00;
    UnkStruct_0207E060 unk_24[32];
} UnkStruct_0207DFAC;

typedef struct UnkStruct_ov65_02235130_t UnkStruct_ov65_02235130;

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

typedef struct {
    const UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222BB38 *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u8 unk_12;
    u8 unk_13;
} UnkStruct_ov63_0222CE44;

typedef struct UnkStruct_ov65_022354D8_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222CE44 *unk_04;
    s16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    u16 unk_0C;
    u16 unk_0E;

    BOOL (*unk_10)
    (UnkStruct_ov65_02235130 *, struct UnkStruct_ov65_022354D8_t *);
    BOOL (*unk_14)
    (UnkStruct_ov65_02235130 *, struct UnkStruct_ov65_022354D8_t *);

    void (*unk_18)(UnkStruct_ov65_02235130 *, struct UnkStruct_ov65_022354D8_t *);
} UnkStruct_ov65_022354D8;

typedef struct {
    void *unk_00;
    NNSG2dScreenData *unk_04;
    void *unk_08;
    NNSG2dCharacterData *unk_0C;
} UnkStruct_ov65_0222E99C;

typedef struct UnkStruct_ov65_0222EBE0_t UnkStruct_ov65_0222EBE0;

typedef struct {
    MessageFormat *unk_00;
    void *unk_04;
    NNSG2dScreenData *unk_08;
    void *unk_0C[7];
    NNSG2dScreenData *unk_28[7];
    void *unk_44;
    NNSG2dScreenData *unk_48;
    TouchHitboxController *unk_4C;
    u8 unk_50[32];
    u8 unk_70;
    u8 unk_71;
    u8 unk_72;
    u8 unk_73;
    s8 unk_74;
    u8 unk_75;
    u8 unk_76;
    u8 unk_77;
    BOOL unk_78;
    Window unk_7C[8];
    Window unk_FC[8][2];
    Window unk_1FC;
    SpriteResource *unk_20C[4];
    Sprite *unk_21C[3];
    CharTransferAllocation unk_228;
    TextOBJ *unk_234;
    u32 unk_238;
    u32 unk_23C;
    u32 unk_240;
    TouchHitboxController *unk_244;
} UnkStruct_ov65_02234E50;

typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;

typedef struct {
    UnkStruct_ov63_0222AE60 *unk_00;
    UnkStruct_ov63_0222CE44 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov63_0222CD2C;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222D160;

typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    Sprite *unk_38;
} UnkStruct_ov65_022358CC_sub1;

typedef struct {
    SpriteList *unk_00;
    G2dRenderer unk_04;
    SpriteResourceCollection *unk_190[4];
    UnkStruct_ov65_022358CC_sub1 unk_1A0;
} UnkStruct_ov65_022358CC;

typedef struct {
    void *unk_00;
    NNSG2dPaletteData *unk_04;
    u8 unk_08;
    u8 unk_09;
    s16 unk_0A;
    u8 unk_0C;
    u8 unk_0D;
    s16 unk_0E;
    u8 unk_10;
    u8 unk_11[3];
} UnkStruct_ov65_02236318;

struct UnkStruct_ov65_02235130_t {
    BOOL unk_00;
    u32 heapID;
    u32 unk_08;
    BOOL unk_0C;
    BgConfig *unk_10;
    UnkStruct_ov65_022358CC unk_14;
    UnkStruct_ov65_022354D8 unk_1F0[48];
    UnkStruct_ov65_022354D8 *unk_730;
    UnkStruct_ov65_02236318 unk_734;
    UnkStruct_ov63_0222BCE8 *unk_748;
    UnkStruct_ov63_0222BE18 *unk_74C;
    UnkStruct_ov63_0222CD2C *unk_750;
    UnkStruct_ov63_0222D160 unk_754;
    UnkStruct_ov63_0222D1C0 *unk_758;
    UnkStruct_ov63_0222D848 *unk_75C;
};

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2D ListMenu2D;

struct UnkStruct_ov65_0222EBE0_t {
    WiFiList *unk_00;
    UnkStruct_0207DFAC *unk_04;
    int unk_08;
    u8 unk_0C[32];
    u8 unk_2C[32];
    int unk_4C[32];
    int unk_CC[32];
    ListMenuItem *unk_14C;
    ListMenu *unk_150;
    ListMenuItem *unk_154;
    ListMenu *unk_158;
    BgConfig *unk_15C;
    SaveData *saveData;
    MessageFormat *unk_164;
    MsgData *unk_168;
    MsgData *unk_16C;
    String *unk_170;
    String *unk_174;
    String *unk_178;
    String *unk_17C;
    int unk_180;
    ListMenu2D *unk_184;
    void *unk_188;
    SpriteList *unk_18C;
    G2dRenderer unk_190;
    SpriteResourceCollection *unk_31C[4];
    UnkStruct_02012744 *unk_32C;
    Window unk_330;
    Window unk_340;
    Window unk_350;
    Window unk_360;
    Window unk_370;
    Window unk_380;
    Window unk_390;
    int unk_3A0;
    int unk_3A4;
    int unk_3A8;
    int unk_3AC;
    int unk_3B0;
    int unk_3B4;
    int unk_3B8;
    int unk_3BC;
    PlayerProfile *unk_3C0;
    u8 unk_3C4;
    u8 unk_3C5;
    BOOL unk_3C8;
    OverlayManager *appMan;
    int unk_3D0;
    u16 unk_3D4;
    u16 unk_3D6[3];
    u16 unk_3DC;
    u16 unk_3DE;
    u16 unk_3E0;
    u16 unk_3E2;
    BOOL unk_3E4;
    int unk_3E8;
    UnkStruct_ov65_02235130 unk_3EC;
    UnkStruct_ov65_022354D8 *unk_B4C[33];
    UnkStruct_ov65_0222E99C unk_BD0;
    UnkStruct_ov65_02234E50 unk_BE0;
    BOOL unk_E28;
    UnkStruct_ov61_0222C3B0 unk_E2C;
    u16 unk_1158;
    u16 unk_115A;
};

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov44_0222C658(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_022302B0(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov44_0222F950(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov44_0222F970(UnkStruct_ov65_0222EBE0 *param0, u32 param1, enum HeapID heapID);

u32 ov65_022330C0(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_022332C4(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov65_022332FC(UnkStruct_ov65_0222EBE0 *param0, NARC *param1, enum HeapID heapID);

void ov44_02230060(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_02233940(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_02233E20(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_02233EF0(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_0223416C(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_022342A8(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_022343CC(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_0223449C(UnkStruct_ov65_0222EBE0 *param0, enum HeapID heapID);

void ov65_02234E40(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov83_0223F804(BgConfig *param0);

extern const GraphicsModes ov83_02247D38;

extern const BgTemplate ov83_02247DA0;

extern const BgTemplate ov83_02247DBC;

extern const BgTemplate ov83_02247DF4;

extern const BgTemplate ov83_02247E10;

extern const BgTemplate ov83_02247E2C;

extern const BgTemplate ov83_02247E48;

extern const BgTemplate ov83_02247D84;

extern const BgTemplate ov83_02247DD8;

void ov83_0223F804(BgConfig *param0) {
    {
        GraphicsModes v0 = ov83_02247D38;

        SetBothScreensModesAndDisable(&v0);
    }

    {
        BgTemplate v1 = ov83_02247DA0;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_1, &v1, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_107);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_1);
    }

    {
        BgTemplate v2 = ov83_02247DBC;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_0, &v2, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_0, 32, 0, HEAP_ID_107);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_0);
    }

    {
        BgTemplate v3 = ov83_02247DF4;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_2, &v3, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_2, 32, 0, HEAP_ID_107);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_2);
    }

    {
        BgTemplate v4 = ov83_02247E10;

        InitBgFromTemplate(param0, GF_BG_LYR_MAIN_3, &v4, 0);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_3, 32, 0, HEAP_ID_107);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_MAIN_3);
    }

    {
        BgTemplate v5 = ov83_02247E2C;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_0, &v5, 0);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_0);
    }

    {
        BgTemplate v6 = ov83_02247E48;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_1, &v6, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 32, GF_BG_LYR_MAIN_0, HEAP_ID_107);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_1);
    }

    {
        BgTemplate v7 = ov83_02247D84;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_2, &v7, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 32, GF_BG_LYR_MAIN_0, HEAP_ID_107);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_2);
    }

    {
        BgTemplate v8 = ov83_02247DD8;

        InitBgFromTemplate(param0, GF_BG_LYR_SUB_3, &v8, 0);
        BG_ClearCharDataRange(GF_BG_LYR_SUB_3, 32, 0, HEAP_ID_107);
        BgClearTilemapBufferAndCommit(param0, GF_BG_LYR_SUB_3);
    }

    reg_G2_BG0CNT = reg_G2_BG0CNT & ~3;
    GfGfx_EngineATogglePlanes(2, 1);
    GfGfx_EngineBTogglePlanes(4, 0);
    GfGfx_EngineBTogglePlanes(8, 0);
}
