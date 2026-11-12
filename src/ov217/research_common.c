// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/research_common.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/palette_anime.h"
#include "app/research_radar/research_common.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "nitro/gx.h"
#include "nitro/memorymap.h"
#include "system/game_system.h"
#include "system/palanm.h"

#define OBJ_RES_COUNT 3
#define UNIT_COUNT 1
#define ACTOR_COUNT 1
#define ANIME_COUNT 1
#define TOUCH_RECT_COUNT 2

// The return button's resources in ARCID_APP_MENU_COMMON
enum {
    OBJ_RES_CHARS,
    OBJ_RES_PALETTE,
    OBJ_RES_CELL_ANIMS,
};

struct ResearchCommon {
    HeapID heapId;
    GameSystem *gsys;
    GameData *gameData;
    TouchRect touchRects[TOUCH_RECT_COUNT];
    u32 objRes[OBJ_RES_COUNT];
    ClActUnit *units[UNIT_COUNT];
    ClActor *actors[ACTOR_COUNT];
    PaletteFade *paletteFade;
    PaletteAnime *animes[ANIME_COUNT];
    u32 prevSeq;
    u32 seq;
    BOOL touchMode;
    BOOL forceExit;
};

static void func_ov217_021c0a04(ResearchCommon *common);
static ResearchCommon *func_ov217_021c0a38(HeapID heapId);
static void func_ov217_021c0a54(ResearchCommon *common);
static void func_ov217_021c0a5c(ResearchCommon *common, GameSystem *gsys, HeapID heapId);
static void func_ov217_021c0aa4(ResearchCommon *common);
static void func_ov217_021c0ad8(ResearchCommon *common);
static void func_ov217_021c0af0(ResearchCommon *common);
static void func_ov217_021c0af8(ResearchCommon *common);
static void func_ov217_021c0b0c(ResearchCommon *common);
static void func_ov217_021c0b74(ResearchCommon *common);
static void func_ov217_021c0b8c(ResearchCommon *common);
static void func_ov217_021c0b94(ResearchCommon *common);
static void func_ov217_021c0ba8(ResearchCommon *common);
static void func_ov217_021c0bb4(ResearchCommon *common);
static void func_ov217_021c0bbc(ResearchCommon *common);
static void func_ov217_021c0c2c(ResearchCommon *common);
static void func_ov217_021c0c38(ResearchCommon *common);
static void func_ov217_021c0c40(ResearchCommon *common);
static void func_ov217_021c0c50(ResearchCommon *common);
static void func_ov217_021c0c5c(ResearchCommon *common);
static void func_ov217_021c0cc8(ResearchCommon *common);
static void func_ov217_021c0cf0(ResearchCommon *common);
static void func_ov217_021c0cf8(ResearchCommon *common);
static void func_ov217_021c0d08(ResearchCommon *common);
static void func_ov217_021c0d14(ResearchCommon *common);
static void func_ov217_021c0d2c(ResearchCommon *common);
static void func_ov217_021c0d38(ResearchCommon *common, u32 index);
static void func_ov217_021c0d5c(ResearchCommon *common, u32 index);
static void func_ov217_021c0d6c(ResearchCommon *common, u32 index);
static void func_ov217_021c0d7c(ResearchCommon *common);
static void func_ov217_021c0d88(ResearchCommon *common);
static u32 func_ov217_021c0db4(ResearchCommon *common, u32 index);
static ClActUnit *func_ov217_021c0dbc(ResearchCommon *common, u32 index);

// Nothing reads it. It is a global, since an unreferenced static would not be emitted; as the smallest object, its
// section of its own comes first in the file's .rodata
const u32 ResearchCommon_Unused = 0x00010002;

static const ResearchRect data_ov217_021c3f48[TOUCH_RECT_COUNT] = {
    { 216, 240, 168, 192 },
#ifdef BUGFIX
    { 0, 0, TOUCH_RECT_END, 0 },
#else
    // BUG: As in research_top.c, the end of the table is written as a TouchRect, so the copy in the work has no end
    { TOUCH_RECT_END, 0, 0, 0 },
#endif
};

static const ResearchPaletteAnimeSetup data_ov217_021c3f50[ANIME_COUNT] = {
    { (u16 *)HW_OBJ_PLTT, (const u16 *)HW_OBJ_PLTT, 16, 6, 0xffff },
};

static const ClActSysSetup data_ov217_021c3f64 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 256, 32, 32, 32, 16, 16 };

// The return button
static const ResearchActorSetup data_ov217_021c3f80[ACTOR_COUNT] = {
    { 216, 168, 1, 3, 1, 0, OBJ_RES_CHARS, OBJ_RES_PALETTE, OBJ_RES_CELL_ANIMS, 0 },
};

static const BGSysVRAMConfig data_ov217_021c3f9c = {
    GX_VRAM_BG_128_A, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_B,  GX_VRAM_TEXPLTT_0_G,     GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's ResearchCommon_Create
ResearchCommon *func_ov217_021c097c(HeapID heapId, GameSystem *gsys) {
    ResearchCommon *common = func_ov217_021c0a38(heapId);

    func_ov217_021c0a04(common);
    func_ov217_021c0a5c(common, gsys, heapId);
    return common;
}

// pokebw2's ResearchCommon_Delete
void func_ov217_021c099c(ResearchCommon *common) {
    func_ov217_021c0aa4(common);
    func_ov217_021c0a54(common);
}

// pokebw2's ResearchCommon_GetHeapID
HeapID func_ov217_021c09ac(ResearchCommon *common) {
    return common->heapId;
}

// pokebw2's ResearchCommon_GetGameSystem
GameSystem *func_ov217_021c09b0(ResearchCommon *common) {
    return common->gsys;
}

// pokebw2's ResearchCommon_GetGameData
GameData *func_ov217_021c09b4(ResearchCommon *common) {
    return common->gameData;
}

// pokebw2's ResearchCommon_GetTouchRects
const TouchRect *func_ov217_021c09b8(ResearchCommon *common) {
    return common->touchRects;
}

// pokebw2's ResearchCommon_SetSeq
void func_ov217_021c09bc(ResearchCommon *common, u32 seq) {
    common->prevSeq = common->seq;
    common->seq = seq;
}

// pokebw2's ResearchCommon_SetTouchMode
void func_ov217_021c09c4(ResearchCommon *common, BOOL touch) {
    common->touchMode = touch;
}

// pokebw2's ResearchCommon_GetPrevSeq
u32 func_ov217_021c09c8(ResearchCommon *common) {
    return common->prevSeq;
}

// pokebw2's ResearchCommon_GetTouchMode
BOOL func_ov217_021c09cc(ResearchCommon *common) {
    return common->touchMode;
}

// pokebw2's ResearchCommon_UpdatePaletteAnime
void func_ov217_021c09d0(ResearchCommon *common) {
    func_ov217_021c0d7c(common);
}

// pokebw2's ResearchCommon_StartPaletteAnime
void func_ov217_021c09d8(ResearchCommon *common, u32 index) {
    func_ov217_021c0d38(common, index);
}

// pokebw2's ResearchCommon_StopPaletteAnime
void func_ov217_021c09e0(ResearchCommon *common) {
    func_ov217_021c0d5c(common, 0);
}

// pokebw2's ResearchCommon_RestorePalette
void func_ov217_021c09ec(ResearchCommon *common) {
    func_ov217_021c0d6c(common, 0);
}

// pokebw2's ResearchCommon_IsForceExit
BOOL func_ov217_021c09f8(ResearchCommon *common) {
    return common->forceExit;
}

// pokebw2's ResearchCommon_SetForceExit
void func_ov217_021c09fc(ResearchCommon *common) {
    common->forceExit = TRUE;
}

// pokebw2's ResearchCommon_InitWork
static void func_ov217_021c0a04(ResearchCommon *common) {
    MI_CpuFill8(common, 0, sizeof(ResearchCommon));
    common->prevSeq = RESEARCH_SEQ_INIT;
    common->seq = RESEARCH_SEQ_INIT;
    func_ov217_021c0af8(common);
    func_ov217_021c0b8c(common);
    func_ov217_021c0bb4(common);
    func_ov217_021c0c38(common);
    func_ov217_021c0cf0(common);
}

// pokebw2's ResearchCommon_Alloc
static ResearchCommon *func_ov217_021c0a38(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(ResearchCommon), FALSE, "research_common.c", 481);
}

// pokebw2's ResearchCommon_Free
static void func_ov217_021c0a54(ResearchCommon *common) {
    GFL_HeapFree(common);
}

// pokebw2's ResearchCommon_Setup
static void func_ov217_021c0a5c(ResearchCommon *common, GameSystem *gsys, HeapID heapId) {
    common->heapId = heapId;
    common->gsys = gsys;
    common->gameData = GSYS_GetGameData(gsys);
    func_ov217_021c0ad8(common);
    func_ov217_021c0b0c(common);
    func_ov217_021c0b94(common);
    func_ov217_021c0bbc(common);
    func_ov217_021c0c40(common);
    func_ov217_021c0c5c(common);
    func_ov217_021c0d88(common);
    func_ov217_021c0cf8(common);
    func_ov217_021c0d14(common);
}

// pokebw2's ResearchCommon_Teardown
static void func_ov217_021c0aa4(ResearchCommon *common) {
    func_ov217_021c0d2c(common);
    func_ov217_021c0d08(common);
    func_ov217_021c0cc8(common);
    func_ov217_021c0c50(common);
    func_ov217_021c0c2c(common);
    func_ov217_021c0ba8(common);
    func_ov217_021c0b74(common);
    func_ov217_021c0af0(common);
}

// pokebw2's ResearchCommon_InitClAct
static void func_ov217_021c0ad8(ResearchCommon *common) {
    ClActSys_Create(&data_ov217_021c3f64, &data_ov217_021c3f9c, common->heapId);
}

// pokebw2's ResearchCommon_ExitClAct
static void func_ov217_021c0af0(ResearchCommon *common) {
    func_0204a5ac();
}

// pokebw2's ResearchCommon_ClearObjRes
static void func_ov217_021c0af8(ResearchCommon *common) {
    int i;

    for (i = 0; i < OBJ_RES_COUNT; i++) {
        common->objRes[i] = 0;
    }
}

// pokebw2's ResearchCommon_LoadObjRes
static void func_ov217_021c0b0c(ResearchCommon *common) {
    HeapID heapId = common->heapId;
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_APP_MENU_COMMON, HEAPID_TAIL(heapId));
    u32 chars = func_0204a6c8(handle, 20, FALSE, CLACT_VRAM_MAIN, heapId);
    u32 palette = func_0204ac18(handle, 19, CLACT_VRAM_MAIN, 0, 0, 4, heapId);
    u32 cellAnims = func_0204af28(handle, 21, 24, heapId);

    common->objRes[OBJ_RES_CHARS] = chars;
    common->objRes[OBJ_RES_PALETTE] = palette;
    common->objRes[OBJ_RES_CELL_ANIMS] = cellAnims;
    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchCommon_FreeObjRes
static void func_ov217_021c0b74(ResearchCommon *common) {
    func_0204a8d4(common->objRes[OBJ_RES_CHARS]);
    func_0204ada4(common->objRes[OBJ_RES_PALETTE]);
    func_0204afd8(common->objRes[OBJ_RES_CELL_ANIMS]);
}

// pokebw2's ResearchCommon_ClearUnits
static void func_ov217_021c0b8c(ResearchCommon *common) {
    common->units[0] = NULL;
}

// pokebw2's ResearchCommon_CreateUnits
static void func_ov217_021c0b94(ResearchCommon *common) {
    common->units[0] = func_0204b100(1, 2, common->heapId);
}

// pokebw2's ResearchCommon_DeleteUnits
static void func_ov217_021c0ba8(ResearchCommon *common) {
    func_0204b1cc(common->units[0]);
}

// pokebw2's ResearchCommon_ClearActors
static void func_ov217_021c0bb4(ResearchCommon *common) {
    common->actors[0] = NULL;
}

// pokebw2's ResearchCommon_CreateActors
static void func_ov217_021c0bbc(ResearchCommon *common) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        const ResearchActorSetup *entry = &data_ov217_021c3f80[i];
        ClActorSetup setup;
        ClActUnit *unit;
        u32 chars;
        u32 palette;
        u32 cellAnims;

        setup.x = entry->x;
        setup.y = entry->y;
        setup.sequence = entry->sequence;
        setup.priority = entry->priority;
        setup.bgPriority = entry->bgPriority;
        unit = func_ov217_021c0dbc(common, entry->unit);
        chars = func_ov217_021c0db4(common, entry->chars);
        palette = func_ov217_021c0db4(common, entry->palette);
        cellAnims = func_ov217_021c0db4(common, entry->cellAnims);
        common->actors[i] = func_0204b294(unit, chars, palette, cellAnims, &setup, entry->surface, common->heapId);
        func_0204b3dc(common->actors[i], TRUE);
    }
}

// pokebw2's ResearchCommon_DeleteActors
static void func_ov217_021c0c2c(ResearchCommon *common) {
    func_0204b3b4(common->actors[0]);
}

// pokebw2's ResearchCommon_ClearPaletteFade
static void func_ov217_021c0c38(ResearchCommon *common) {
    common->paletteFade = NULL;
}

// pokebw2's ResearchCommon_CreatePaletteFade
static void func_ov217_021c0c40(ResearchCommon *common) {
    common->paletteFade = func_02021518(common->heapId);
}

// pokebw2's ResearchCommon_DeletePaletteFade
static void func_ov217_021c0c50(ResearchCommon *common) {
    func_02021540(common->paletteFade);
}

// pokebw2's ResearchCommon_SetupPaletteFade
static void func_ov217_021c0c5c(ResearchCommon *common) {
    PaletteFade *fade = common->paletteFade;

    func_0202155c(fade, PALFADE_BUFFER_MAIN_BG, 0x200, common->heapId);
    func_0202155c(fade, PALFADE_BUFFER_MAIN_OBJ, 0x200, common->heapId);
    func_0202155c(fade, PALFADE_BUFFER_SUB_BG, 0x200, common->heapId);
    func_0202155c(fade, PALFADE_BUFFER_SUB_OBJ, 0x200, common->heapId);
    func_020216d4(fade, PALFADE_VRAM_MAIN_BG, 0, 0x200);
    func_020216d4(fade, PALFADE_VRAM_MAIN_OBJ, 0, 0x200);
    func_020216d4(fade, PALFADE_VRAM_SUB_BG, 0, 0x200);
    func_020216d4(fade, PALFADE_VRAM_SUB_OBJ, 0, 0x200);
}

// pokebw2's ResearchCommon_FreePaletteFade
static void func_ov217_021c0cc8(ResearchCommon *common) {
    PaletteFade *fade = common->paletteFade;

    func_020215a0(fade, PALFADE_BUFFER_MAIN_BG);
    func_020215a0(fade, PALFADE_BUFFER_MAIN_OBJ);
    func_020215a0(fade, PALFADE_BUFFER_SUB_BG);
    func_020215a0(fade, PALFADE_BUFFER_SUB_OBJ);
}

// pokebw2's ResearchCommon_ClearPaletteAnimes
static void func_ov217_021c0cf0(ResearchCommon *common) {
    common->animes[0] = NULL;
}

// pokebw2's ResearchCommon_CreatePaletteAnimes
static void func_ov217_021c0cf8(ResearchCommon *common) {
    common->animes[0] = func_ov217_021c29c8(common->heapId);
}

// pokebw2's ResearchCommon_DeletePaletteAnimes
static void func_ov217_021c0d08(ResearchCommon *common) {
    func_ov217_021c29d8(common->animes[0]);
}

// pokebw2's ResearchCommon_SetupPaletteAnimes
static void func_ov217_021c0d14(ResearchCommon *common) {
    int i;

    for (i = 0; i < ANIME_COUNT; i++) {
        func_ov217_021c29e8(common->animes[i], data_ov217_021c3f50[i].dst, data_ov217_021c3f50[i].src,
                           data_ov217_021c3f50[i].count);
    }
}

// pokebw2's ResearchCommon_RestorePalettes
static void func_ov217_021c0d2c(ResearchCommon *common) {
    func_ov217_021c2a08(common->animes[0]);
}

// pokebw2's ResearchCommon_StartPaletteAnimeCore
static void func_ov217_021c0d38(ResearchCommon *common, u32 index) {
    func_ov217_021c29f8(common->animes[index], data_ov217_021c3f50[index].mode, data_ov217_021c3f50[index].color);
}

// pokebw2's ResearchCommon_StopPaletteAnimeCore
static void func_ov217_021c0d5c(ResearchCommon *common, u32 index) {
    func_ov217_021c2a00(common->animes[index]);
}

// pokebw2's ResearchCommon_RestorePaletteCore
static void func_ov217_021c0d6c(ResearchCommon *common, u32 index) {
    func_ov217_021c2a08(common->animes[index]);
}

// pokebw2's ResearchCommon_UpdatePaletteAnimes
static void func_ov217_021c0d7c(ResearchCommon *common) {
    func_ov217_021c29f0(common->animes[0]);
}

// pokebw2's ResearchCommon_InitTouchRects
static void func_ov217_021c0d88(ResearchCommon *common) {
    int i;

    for (i = 0; i < TOUCH_RECT_COUNT; i++) {
        common->touchRects[i].left = data_ov217_021c3f48[i].left;
        common->touchRects[i].right = data_ov217_021c3f48[i].right;
        common->touchRects[i].top = data_ov217_021c3f48[i].top;
        common->touchRects[i].bottom = data_ov217_021c3f48[i].bottom;
    }
}

// pokebw2's ResearchCommon_GetObjRes
static u32 func_ov217_021c0db4(ResearchCommon *common, u32 index) {
    return common->objRes[index];
}

// pokebw2's ResearchCommon_GetUnit
static ClActUnit *func_ov217_021c0dbc(ResearchCommon *common, u32 index) {
    return common->units[index];
}
