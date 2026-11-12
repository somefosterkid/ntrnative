// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/research_radar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar.h"
#include "app/research_radar/research_common.h"
#include "app/research_radar/research_graph.h"
#include "app/research_radar/research_list.h"
#include "app/research_radar/research_list_recovery.h"
#include "app/research_radar/research_top.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "nitro/gx.h"
#include "nnsys/g2d.h"
#include "system/game_data.h"
#include "system/game_system.h"

// The Research Radar's proc, which shows the results of the surveys. The game doesn't name this file; it is named
// after the app. It runs one of three screens at a time, the top screen, the list of surveys and a survey's graph,
// which share the work in research_common.c, and draws the BGs that stay behind all of them

// The pattern's palette cycles: it changes every this many frames, through this many palettes
#define PATTERN_FRAMES 10
#define PATTERN_PALETTES 6

typedef struct {
    HeapID heapId;
    GameSystem *gsys;
    u32 frames;
    ResearchCommon *common;
    ResearchTop *top;
    ResearchList *list;
    ResearchGraph *graph;
    ResearchListRecovery *recovery;
} ResearchRadarWork;

static BOOL func_ov217_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov217_021b9600(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov217_021b9620(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov217_021b96a0(ResearchRadarWork *wk);
static void func_ov217_021b96c8(ResearchRadarWork *wk, u32 *state, u32 next);
static void func_ov217_021b970c(ResearchRadarWork *wk, u32 *state);
static void func_ov217_021b9774(ResearchRadarWork *wk, u32 *state, u32 next);
static u32 func_ov217_021b97b8(ResearchRadarWork *wk);
static u32 func_ov217_021b97bc(ResearchRadarWork *wk);
static u32 func_ov217_021b97f8(ResearchRadarWork *wk);
static u32 func_ov217_021b9824(ResearchRadarWork *wk);
static u32 func_ov217_021b9850(ResearchRadarWork *wk);
static void func_ov217_021b9854(ResearchRadarWork *wk);
static void func_ov217_021b985c(ResearchRadarWork *wk, GameSystem *gsys);
static void func_ov217_021b9874(ResearchRadarWork *wk);
static void func_ov217_021b9884(ResearchRadarWork *wk);
static void func_ov217_021b9894(ResearchRadarWork *wk);
static void func_ov217_021b98a8(ResearchRadarWork *wk);
static void func_ov217_021b98b8(HeapID heapId);
static void func_ov217_021b990c(void);
static void func_ov217_021b9920(HeapID heapId);
static void func_ov217_021b99ec(ResearchRadarWork *wk);
static void func_ov217_021b9a24(HeapID heapId);

// pokebw2's sRadarLCDConfig
static const BGSysLCDConfig data_ov217_021c2e30 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sRadarBGSubBack
static const BGSetup data_ov217_021c2e60 = { 0,
                                         0,
                                         0x800,
                                         0,
                                         BGRES_256x256,
                                         GX_BG_COLORMODE_16,
                                         GX_BG_SCRBASE_0x0000,
                                         GX_BG_CHARBASE_0x04000,
                                         0x8000,
                                         GX_BG_EXTPLTT_01,
                                         3,
                                         GX_BG_AREAOVER_XLU,
                                         FALSE };

// pokebw2's sRadarBGMainFrame
static const BGSetup data_ov217_021c2e80 = { 0,
                                           0,
                                           0x800,
                                           0,
                                           BGRES_256x256,
                                           GX_BG_COLORMODE_16,
                                           GX_BG_SCRBASE_0x0000,
                                           GX_BG_CHARBASE_0x04000,
                                           0x8000,
                                           GX_BG_EXTPLTT_01,
                                           3,
                                           GX_BG_AREAOVER_XLU,
                                           FALSE };

// pokebw2's sRadarBGSubPattern
static const BGSetup data_ov217_021c2e40 = { 0,
                                            0,
                                            0x800,
                                            0,
                                            BGRES_256x256,
                                            GX_BG_COLORMODE_16,
                                            GX_BG_SCRBASE_0x0800,
                                            GX_BG_CHARBASE_0x04000,
                                            0x8000,
                                            GX_BG_EXTPLTT_01,
                                            2,
                                            GX_BG_AREAOVER_XLU,
                                            FALSE };

// pokebw2's sRadarVRAMConfig
static const BGSysVRAMConfig data_ov217_021c2ea0 = {
    GX_VRAM_BG_128_A, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_B,  GX_VRAM_TEXPLTT_0_G,     GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's RESEARCH_RADAR_PROC_FUNCTIONS
// pokebw2's ResearchRadar_Exit
// pokebw2's ResearchRadar_Init
// pokebw2's ResearchRadar_Main
GameProcFunctions data_ov217_021c3fe0 = { func_ov217_021b95a0, func_ov217_021b9620, func_ov217_021b9600 };

static BOOL func_ov217_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    ResearchRadarWork *wk = work;
    GameSystem **gsys = param;

    switch (*state) {
    case 0:
        GFL_ProcInitSubsystem(proc, sizeof(ResearchRadarWork), HEAPID_USER);
        (*state)++;
        break;
    case 1:
        func_ov217_021b985c(wk, *gsys);
        GFL_BGSysSetVRAMBanks(&data_ov217_021c2ea0);
        GFL_BGSysSetDisplayLayout(0);
        func_ov217_021b98b8(wk->heapId);
        func_ov217_021b9a24(wk->heapId);
        func_ov217_021b9920(wk->heapId);
        func_ov217_021b9874(wk);
        func_ov217_021b9894(wk);
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov217_021b9600(GameProc *proc, u32 *state, void *param, void *work) {
    ResearchRadarWork *wk = work;

    func_ov217_021b98a8(wk);
    func_ov217_021b9884(wk);
    func_ov217_021b990c();
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

static BOOL func_ov217_021b9620(GameProc *proc, u32 *state, void *param, void *work) {
    ResearchRadarWork *wk = work;
    u32 next;

    func_ov217_021b96a0(wk);
    if (func_02012204(wk->gsys)) {
        func_ov217_021c09fc(wk->common);
    }

    switch (*state) {
    case RESEARCH_SEQ_INIT:
        next = func_ov217_021b97b8(wk);
        break;
    case RESEARCH_SEQ_TOP:
        next = func_ov217_021b97bc(wk);
        break;
    case RESEARCH_SEQ_LIST:
        next = func_ov217_021b97f8(wk);
        break;
    case RESEARCH_SEQ_GRAPH:
        next = func_ov217_021b9824(wk);
        break;
    case RESEARCH_SEQ_END:
        return TRUE;
    }

    func_ov217_021b99ec(wk);
    func_ov217_021b9854(wk);
    if (*state != next) {
        func_ov217_021b96c8(wk, state, next);
    }
    return FALSE;
}

// pokebw2's ResearchRadar_UpdateComm
static void func_ov217_021b96a0(ResearchRadarWork *wk) {
    func_0201058c(func_0201283c(GSYS_GetGameData(wk->gsys)));
    if (func_020121cc(wk->gsys)) {
        func_0203d34c(TRUE, wk->heapId);
    }
}

// pokebw2's ResearchRadar_ChangeSeq
static void func_ov217_021b96c8(ResearchRadarWork *wk, u32 *state, u32 next) {
    func_ov217_021b970c(wk, state);
    func_ov217_021b9774(wk, state, next);

    switch (*state) {
    case RESEARCH_SEQ_TOP:
        func_ov217_021c09bc(wk->common, RESEARCH_SEQ_TOP);
        break;
    case RESEARCH_SEQ_LIST:
        func_ov217_021c09bc(wk->common, RESEARCH_SEQ_LIST);
        break;
    case RESEARCH_SEQ_GRAPH:
        func_ov217_021c09bc(wk->common, RESEARCH_SEQ_GRAPH);
        break;
    }
}

// pokebw2's ResearchRadar_EndSeq
static void func_ov217_021b970c(ResearchRadarWork *wk, u32 *state) {
    switch (*state) {
    case RESEARCH_SEQ_INIT:
        break;
    case RESEARCH_SEQ_TOP:
        func_ov217_021b9af8(wk->top);
        break;
    case RESEARCH_SEQ_LIST:
        func_ov217_021babb4(wk->list);
        break;
    case RESEARCH_SEQ_GRAPH:
        func_ov217_021bd61c(wk->graph);
        break;
    case RESEARCH_SEQ_END:
        break;
    }

    switch (*state) {
    case RESEARCH_SEQ_INIT:
        break;
    case RESEARCH_SEQ_TOP:
        wk->top = NULL;
        break;
    case RESEARCH_SEQ_LIST:
        wk->list = NULL;
        break;
    case RESEARCH_SEQ_GRAPH:
        wk->graph = NULL;
        break;
    case RESEARCH_SEQ_END:
        break;
    }
}

// pokebw2's ResearchRadar_StartSeq
static void func_ov217_021b9774(ResearchRadarWork *wk, u32 *state, u32 next) {
    switch (next) {
    case RESEARCH_SEQ_INIT:
        break;
    case RESEARCH_SEQ_TOP:
        wk->top = func_ov217_021b9ad0(wk->common);
        break;
    case RESEARCH_SEQ_LIST:
        wk->list = func_ov217_021bab80(wk->common, wk->recovery);
        break;
    case RESEARCH_SEQ_GRAPH:
        wk->graph = func_ov217_021bd5f4(wk->common);
        break;
    case RESEARCH_SEQ_END:
        break;
    }
    *state = next;
}

// pokebw2's ResearchRadar_SeqInit
static u32 func_ov217_021b97b8(ResearchRadarWork *wk) {
    return RESEARCH_SEQ_TOP;
}

// pokebw2's ResearchRadar_SeqTop
static u32 func_ov217_021b97bc(ResearchRadarWork *wk) {
    func_ov217_021b9b08(wk->top);
    if (func_ov217_021b9b80(wk->top)) {
        switch (func_ov217_021b9b90(wk->top)) {
        case RESEARCH_TOP_NEXT_LIST:
            return RESEARCH_SEQ_LIST;
        case RESEARCH_TOP_NEXT_GRAPH:
            return RESEARCH_SEQ_GRAPH;
        case RESEARCH_TOP_NEXT_EXIT:
            return RESEARCH_SEQ_END;
        }
        return RESEARCH_SEQ_END;
    }
    return RESEARCH_SEQ_TOP;
}

// pokebw2's ResearchRadar_SeqList
static u32 func_ov217_021b97f8(ResearchRadarWork *wk) {
    func_ov217_021babc4(wk->list);
    if (func_ov217_021bac90(wk->list)) {
        if (func_ov217_021bac98(wk->list) == RESEARCH_LIST_NEXT_TOP) {
            return RESEARCH_SEQ_TOP;
        }
        return RESEARCH_SEQ_END;
    }
    return RESEARCH_SEQ_LIST;
}

// pokebw2's ResearchRadar_SeqGraph
static u32 func_ov217_021b9824(ResearchRadarWork *wk) {
    func_ov217_021bd62c(wk->graph);
    if (func_ov217_021bd708(wk->graph)) {
        if (func_ov217_021bd710(wk->graph) == RESEARCH_GRAPH_NEXT_TOP) {
            return RESEARCH_SEQ_TOP;
        }
        return RESEARCH_SEQ_END;
    }
    return RESEARCH_SEQ_GRAPH;
}

// pokebw2's ResearchRadar_GetFrames
static u32 func_ov217_021b9850(ResearchRadarWork *wk) {
    return wk->frames;
}

// pokebw2's ResearchRadar_CountFrame
static void func_ov217_021b9854(ResearchRadarWork *wk) {
    wk->frames++;
}

// pokebw2's ResearchRadar_InitWork
static void func_ov217_021b985c(ResearchRadarWork *wk, GameSystem *gsys) {
    wk->heapId = HEAPID_USER;
    wk->gsys = gsys;
    wk->frames = 0;
    wk->common = NULL;
    wk->top = NULL;
    wk->list = NULL;
    wk->graph = NULL;
    wk->recovery = NULL;
}

// pokebw2's ResearchRadar_CreateCommon
static void func_ov217_021b9874(ResearchRadarWork *wk) {
    wk->common = func_ov217_021c097c(wk->heapId, wk->gsys);
}

// pokebw2's ResearchRadar_DeleteCommon
static void func_ov217_021b9884(ResearchRadarWork *wk) {
    func_ov217_021c099c(wk->common);
    wk->common = NULL;
}

// pokebw2's ResearchRadar_CreateRecovery
static void func_ov217_021b9894(ResearchRadarWork *wk) {
    wk->recovery = func_ov217_021c0dc4(wk->heapId);
    func_ov217_021c0de4(wk->recovery);
}

// pokebw2's ResearchRadar_DeleteRecovery
static void func_ov217_021b98a8(ResearchRadarWork *wk) {
    func_ov217_021c0ddc(wk->recovery);
    wk->recovery = NULL;
}

// pokebw2's ResearchRadar_InitBG
static void func_ov217_021b98b8(HeapID heapId) {
    GFL_BGSysCreate(heapId);
    GFL_BGSysSetLCDConfig(&data_ov217_021c2e30);
    GFL_BGSysCreateBG(RESEARCH_BG_SUB_BACK, &data_ov217_021c2e60, BGMODE_TEXT);
    GFL_BGSysCreateBG(RESEARCH_BG_SUB_PATTERN, &data_ov217_021c2e40, BGMODE_TEXT);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_SUB_BACK, TRUE);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_SUB_PATTERN, TRUE);
    GFL_BGSysCreateBG(RESEARCH_BG_MAIN_FRAME, &data_ov217_021c2e80, BGMODE_TEXT);
    GFL_BGSysSetBGEnabled(RESEARCH_BG_MAIN_FRAME, TRUE);
}

// pokebw2's ResearchRadar_ExitBG
static void func_ov217_021b990c(void) {
    GFL_BGSysReleaseBG(RESEARCH_BG_MAIN_FRAME);
    GFL_BGSysReleaseBG(RESEARCH_BG_SUB_BACK);
    GFL_BGSysFree();
}

// pokebw2's ResearchRadar_LoadSubBG
static void func_ov217_021b9920(HeapID heapId) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, heapId);
    void *file;
    NNSG2dPaletteData *palette;
    NNSG2dCharacterData *character;
    NNSG2dScreenData *screen;
    NNSG2dScreenData *patternScreen;

    file = GFL_ArcToolReadHeapNew(handle, 9, heapId);
    NNS_G2dGetUnpackedPaletteData(file, &palette);
    GFL_BGSysUploadStdPalette(RESEARCH_BG_SUB_BACK, palette->pRawData, 0x200, 0);
    GFL_HeapFree(file);

    file = GFL_ArcToolReadHeapNew(handle, 10, heapId);
    NNS_G2dGetUnpackedBGCharacterData(file, &character);
    GFL_BGSysLoadChar(RESEARCH_BG_SUB_BACK, character->pRawData, character->szByte, 0);
    GFL_HeapFree(file);

    file = GFL_ArcToolReadHeapNew(handle, 11, heapId);
    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(RESEARCH_BG_SUB_BACK, screen->rawData, 0, 0, 32, 24);
    GFL_BGSysLoadScr(RESEARCH_BG_SUB_BACK);
    GFL_HeapFree(file);

    file = GFL_ArcToolReadHeapNew(handle, 12, heapId);
    NNS_G2dGetUnpackedScreenData(file, &patternScreen);
    GFL_BGSysLoadScrAreaAll(RESEARCH_BG_SUB_PATTERN, patternScreen->rawData, 0, 0, 32, 24);
    GFL_BGSysLoadScr(RESEARCH_BG_SUB_PATTERN);
    GFL_HeapFree(file);

    GFL_ArcToolFree(handle);
}

// pokebw2's ResearchRadar_AnimateSubBG
static void func_ov217_021b99ec(ResearchRadarWork *wk) {
    // the palette is narrowed twice: a u8 widened again before the u8 parameter
    GFL_BGSysSetScrPaletteNo(RESEARCH_BG_SUB_PATTERN, 0, 0, 32, 24,
                             (int)(u8)(func_ov217_021b9850(wk) / PATTERN_FRAMES % PATTERN_PALETTES));
    GFL_BGSysLoadScr(RESEARCH_BG_SUB_PATTERN);
}

// pokebw2's ResearchRadar_LoadMainBG
static void func_ov217_021b9a24(HeapID heapId) {
    ArcTool *handle = GFL_ArcSysCreateFileHandle(ARCID_RESEARCH_RADAR, HEAPID_TAIL(heapId));
    void *file;
    NNSG2dPaletteData *palette;
    NNSG2dCharacterData *character;
    NNSG2dScreenData *screen;

    file = GFL_ArcToolReadHeapNew(handle, 0, heapId);
    NNS_G2dGetUnpackedPaletteData(file, &palette);
    GFL_BGSysUploadStdPalette(RESEARCH_BG_MAIN_FRAME, palette->pRawData, 0x200, 0);
    GFL_HeapFree(file);

    file = GFL_ArcToolReadHeapNew(handle, 1, heapId);
    NNS_G2dGetUnpackedBGCharacterData(file, &character);
    GFL_BGSysLoadChar(RESEARCH_BG_MAIN_FRAME, character->pRawData, character->szByte, 0);
    GFL_HeapFree(file);

    file = GFL_ArcToolReadHeapNew(handle, 2, heapId);
    NNS_G2dGetUnpackedScreenData(file, &screen);
    GFL_BGSysLoadScrAreaAll(RESEARCH_BG_MAIN_FRAME, screen->rawData, 0, 0, 32, 24);
    GFL_BGSysLoadScr(RESEARCH_BG_MAIN_FRAME);
    GFL_HeapFree(file);

    GFL_ArcToolFree(handle);
}
