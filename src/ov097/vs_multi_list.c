// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov171/vs_multi_list.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/vs_multi_list.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"
#include "system/bgwinfrm.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/egg_demo.h"
#include "gfl/proc.h"
#include "gfl/std.h"

// vs_multi_list.c: the list of both teams' Pokémon that a multi battle shows before it starts. The Pokémon of the
// teams' first three slots show on two boards that slide in from the sides, with their icons, names, levels and HP

// The steps of the screen
enum {
    STATE_SETUP,
    STATE_TEARDOWN,
    STATE_WAIT_WIPE,
    STATE_WAIT_FRAMES,
    STATE_SLIDE,
    STATE_DONE,
};

// The steps of the slide
enum {
    SLIDE_START,
    SLIDE_MOVE,
    SLIDE_END,
};

#define POKEMON_COUNT 6
#define BOARD_POKEMON_COUNT 3
#define FRAME_COUNT 12
#define ACTOR_COUNT 24
#define CHAR_RES_COUNT 9
#define PLTT_RES_COUNT 4
#define CELL_RES_COUNT 4
#define BG_HEAP HEAPID_TAIL(HEAPID_VS_MULTI_LIST)

typedef struct {
    ClActorSetup setup;
    u32 charIndex;
    u32 palIndex;
    u32 cellIndex;
} VsMultiListActor;

typedef struct {
    VsMultiListParam *param;
    TCB *vblankTcb;
    BGWinFrame *frames;
    Font *font;
    Font *smallFont;
    MsgData *msgData;
    WordSet *wordSet;
    PrintQueue *printQueue;
    PrintWindow printWindows[POKEMON_COUNT];
    ClActUnit *clUnit;
    ClActor *actors[ACTOR_COUNT];
    u32 charRes[CHAR_RES_COUNT];
    u32 palRes[PLTT_RES_COUNT];
    u32 cellRes[CELL_RES_COUNT];
    PartyPkm *pkm[POKEMON_COUNT];
    int slideStep;
    int waitNext;
    int wipeNext;
    int wait;
} VsMultiListWork;

BOOL func_ov097_021f6560(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov097_021f6590(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov097_021f65c4(GameProc *proc, u32 *state, void *param, void *work);
int func_ov097_021f65d4(VsMultiListWork *wk);
static int func_ov097_021f6654(VsMultiListWork *wk);
static int func_ov097_021f6684(VsMultiListWork *wk);
static int func_ov097_021f669c(VsMultiListWork *wk);
static int func_ov097_021f66b4(VsMultiListWork *wk);
int func_ov097_021f67c8(VsMultiListWork *work, int next);
int func_ov097_021f67f0(VsMultiListWork *work, int next);
static int func_ov097_021f681c(VsMultiListWork *wk, int frames, int next);
void func_ov097_021f682c(void);
void func_ov097_021f683c(void);
static void func_ov097_021f68e0(void);
void func_ov097_021f68fc(VsMultiListWork *wk);
void func_ov097_021f6970(VsMultiListWork *wk);
static void func_ov097_021f69c0(VsMultiListWork *wk);
void func_ov097_021f69e4(VsMultiListWork *wk);
static void func_ov097_021f69fc(VsMultiListWork *wk);
void func_ov097_021f6a08(TCB *tcb, void *data);
void func_ov097_021f6a28(VsMultiListWork *wk);
void func_ov097_021f6a98(VsMultiListWork *wk);
static void func_ov097_021f6b1c(VsMultiListWork *wk);
static void func_ov097_021f6b38(VsMultiListWork *wk);
static void func_ov097_021f6b7c(VsMultiListWork *wk, u32 index);
static void func_ov097_021f6ce4(VsMultiListWork *wk, u32 index);
void func_ov097_021f6d70(VsMultiListWork *wk, u32 index);
void func_ov097_021f6ee8(VsMultiListWork *wk, u32 index);
void func_ov097_021f6f24(VsMultiListWork *wk);
static void func_ov097_021f6f3c(VsMultiListWork *wk);
static void func_ov097_021f6f50(VsMultiListWork *wk);
static void func_ov097_021f6f80(void);
void func_ov097_021f6f88(VsMultiListWork *wk);
static void func_ov097_021f7108(VsMultiListWork *wk);
void func_ov097_021f714c(VsMultiListWork *wk);
static void func_ov097_021f72bc(VsMultiListWork *wk);
static void func_ov097_021f72dc(VsMultiListWork *wk, u32 index, s16 dx);
static void func_ov097_021f7308(VsMultiListWork *wk, u32 first, s16 dx);
void func_ov097_021f734c(VsMultiListWork *wk);
static void func_ov097_021f75e0(VsMultiListWork *wk);

// Declared in an order that gives the original layout of the data (tools/decomp/rodata_order.py)

// pokebw2's VS_MULTI_LIST_PROC_FUNCTIONS
const GameProcFunctions data_ov097_021f75ec = {
    func_ov097_021f6560,
    func_ov097_021f6590,
    func_ov097_021f65c4,
};

// pokebw2's sVsMultiListStateFuncs
static int (*const data_ov097_021f7608[])(VsMultiListWork *wk) = {
    func_ov097_021f65d4,      func_ov097_021f6654, func_ov097_021f6684,
    func_ov097_021f669c, func_ov097_021f66b4,
};

// pokebw2's sVsMultiListVRAMConfig
static const BGSysVRAMConfig data_ov097_021f761c = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// The 24 actors: for each of the six Pokémon its icon, its item's icon and two more
// pokebw2's sVsMultiListActors
static const VsMultiListActor data_ov097_021f76c8[ACTOR_COUNT] = {
    { { 16, 16, 1, 1, 1 }, 0, 0, 0 },  { { 16, 64, 1, 1, 1 }, 1, 0, 0 },  { { 16, 112, 1, 1, 1 }, 2, 0, 0 },
    { { 144, 24, 1, 1, 1 }, 3, 0, 0 }, { { 144, 72, 1, 1, 1 }, 4, 0, 0 }, { { 144, 120, 1, 1, 1 }, 5, 0, 0 },
    { { 24, 24, 0, 0, 1 }, 6, 1, 1 },  { { 24, 72, 0, 0, 1 }, 6, 1, 1 },  { { 24, 120, 0, 0, 1 }, 6, 1, 1 },
    { { 152, 32, 0, 0, 1 }, 6, 1, 1 }, { { 152, 80, 0, 0, 1 }, 6, 1, 1 }, { { 152, 128, 0, 0, 1 }, 6, 1, 1 },
    { { 28, 40, 0, 2, 1 }, 7, 2, 2 },  { { 28, 88, 0, 2, 1 }, 7, 2, 2 },  { { 28, 136, 0, 2, 1 }, 7, 2, 2 },
    { { 156, 48, 0, 2, 1 }, 7, 2, 2 }, { { 156, 96, 0, 2, 1 }, 7, 2, 2 }, { { 156, 144, 0, 2, 1 }, 7, 2, 2 },
    { { 87, 28, 0, 2, 1 }, 8, 3, 3 },  { { 87, 76, 0, 2, 1 }, 8, 3, 3 },  { { 87, 124, 0, 2, 1 }, 8, 3, 3 },
    { { 215, 36, 0, 2, 1 }, 8, 3, 3 }, { { 215, 84, 0, 2, 1 }, 8, 3, 3 }, { { 215, 132, 0, 2, 1 }, 8, 3, 3 },
};

// pokebw2's VsMultiList_Main

BOOL func_ov097_021f6560(GameProc *proc, u32 *state, void *param, void *work) {
    VsMultiListWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_VS_MULTI_LIST, 0x18000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(VsMultiListWork), HEAPID_VS_MULTI_LIST);
    MI_CpuFill8(wk, 0, sizeof(VsMultiListWork));
    wk->param = param;
    return TRUE;
}

static BOOL func_ov097_021f6590(GameProc *proc, u32 *state, void *param, void *work) {
    VsMultiListWork *wk = work;

    *state = data_ov097_021f7608[*state](wk);
    if (*state == STATE_DONE) {
        return TRUE;
    }
    func_0202cd94(wk->frames);
    func_ov097_021f6b38(wk);
    func_0204a600();
    return FALSE;
}

static BOOL func_ov097_021f65c4(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_VS_MULTI_LIST);
    return TRUE;
}

int func_ov097_021f65d4(VsMultiListWork *wk) {
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GFL_BGSysSetDisplayLayout(GX_DISP_SELECT_MAIN_SUB);
    func_ov097_021f6a28(wk);
    func_ov097_021f682c();
    func_ov097_021f683c();
    func_ov097_021f68fc(wk);
    func_ov097_021f6970(wk);
    func_ov097_021f6a98(wk);
    func_ov097_021f6f24(wk);
    func_ov097_021f734c(wk);
    func_0203d34c(TRUE, HEAPID_VS_MULTI_LIST);
    GFL_BGSysSetBGEnabledA(0x17, TRUE);
    func_ov097_021f69e4(wk);
    return func_ov097_021f67c8(wk, func_ov097_021f681c(wk, 128, STATE_SLIDE));
}



// pokebw2's VsMultiList_StateTeardown
static int func_ov097_021f6654(VsMultiListWork *wk) {
    func_ov097_021f69fc(wk);
    GFL_BGSysSetBGEnabledA(0x17, FALSE);
    func_ov097_021f75e0(wk);
    func_ov097_021f6f3c(wk);
    func_ov097_021f6b1c(wk);
    func_ov097_021f69c0(wk);
    func_ov097_021f68e0();
    return STATE_DONE;
}

// pokebw2's VsMultiList_StateWaitWipe
static int func_ov097_021f6684(VsMultiListWork *wk) {
    if (GFL_WipeIsFinished() == TRUE) {
        return wk->wipeNext;
    }
    return STATE_WAIT_WIPE;
}

// pokebw2's VsMultiList_StateWaitFrames
static int func_ov097_021f669c(VsMultiListWork *wk) {
    if (wk->wait == 0) {
        return wk->waitNext;
    }
    wk->wait--;
    return STATE_WAIT_FRAMES;
}

// Slides the boards in from the sides, waits, and slides the screen out
// pokebw2's VsMultiList_StateSlide
static int func_ov097_021f66b4(VsMultiListWork *wk) {
    u32 i;

    switch (wk->slideStep) {
    case SLIDE_START:
        if (!func_0201c6b4(wk->printQueue)) {
            break;
        }
        if (wk->param->flipped == 0) {
            for (i = 0; i < BOARD_POKEMON_COUNT; i++) {
                func_0202cd64(wk->frames, i + 3, -1, 0, 16);
                func_0202cd64(wk->frames, i + 9, -1, 0, 16);
            }
        } else {
            for (i = 0; i < BOARD_POKEMON_COUNT; i++) {
                func_0202cd64(wk->frames, i, 1, 0, 16);
                func_0202cd64(wk->frames, i + 6, 1, 0, 16);
            }
        }
        GFL_SndSEPlay(SEQ_SE_SYS_03);
        wk->slideStep++;
        // fall through
    case SLIDE_MOVE:
        if (wk->param->flipped == 0) {
            if (!func_0202ce40(wk->frames, 3)) {
                wk->slideStep++;
                GFL_SndSEPlay(SEQ_SE_ROTATION_B);
                return func_ov097_021f681c(wk, 128, STATE_SLIDE);
            }
            func_ov097_021f7308(wk, 3, -8);
        } else {
            if (!func_0202ce40(wk->frames, 0)) {
                wk->slideStep++;
                GFL_SndSEPlay(SEQ_SE_ROTATION_B);
                return func_ov097_021f681c(wk, 128, STATE_SLIDE);
            }
            func_ov097_021f7308(wk, 0, 8);
        }
        break;
    case SLIDE_END:
        return func_ov097_021f67f0(wk, STATE_TEARDOWN);
    }
    return STATE_SLIDE;
}

int func_ov097_021f67c8(VsMultiListWork *work, int next) {
    GFL_WipeSet(0, 1, 1, 0, 6, 1, HEAPID_VS_MULTI_LIST);
    work->wipeNext = next;
    return 2;
}

int func_ov097_021f67f0(VsMultiListWork *work, int next) {
    GFL_WipeSet(0, 0, 0, 0, 6, 1, HEAPID_VS_MULTI_LIST);
    work->wipeNext = next;
    return 2;
}



// pokebw2's VsMultiList_StartWait
static int func_ov097_021f681c(VsMultiListWork *wk, int frames, int next) {
    wk->wait = frames;
    wk->waitNext = next;
    return STATE_WAIT_FRAMES;
}

// pokebw2's VsMultiList_InitVRAM
void func_ov097_021f682c(void) {
    GFL_BGSysSetVRAMBanks(&data_ov097_021f761c);
}

void func_ov097_021f683c(void) {
    GFL_BGSysCreate(HEAPID_VS_MULTI_LIST);
    {
        BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&lcdConfig);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(0);
        GFL_BGSysClearCharCore(0, BGSYS_TILE_SIZE_16, 0, HEAPID_VS_MULTI_LIST);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
    }
}


// pokebw2's VsMultiList_FreeBG
static void func_ov097_021f68e0(void) {
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

void func_ov097_021f68fc(VsMultiListWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(100, BG_HEAP);

    GFL_BGSysLoadArcNCGRStatic(arc, 18, 2, 0, 0, TRUE, BG_HEAP);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0, 2, 0, 0, TRUE, BG_HEAP);
    GFL_G2DIOLoadArcNCLRDefault(arc, 19, 0, 0, 0, BG_HEAP);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 16, 1, 0, 0x800, TRUE, BG_HEAP);
    GFL_ArcToolFree(arc);
    GFL_BGSysResetStdPalette(4, 0);
}


void func_ov097_021f6970(VsMultiListWork *wk) {
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EGG_DEMO, HEAPID_VS_MULTI_LIST);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_VS_MULTI_LIST);
    wk->smallFont = GFL_FontCreate(ARCID_FONT, 3, 0, FALSE, HEAPID_VS_MULTI_LIST);
    wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_VS_MULTI_LIST);
    wk->printQueue = func_0201c450(0x800, HEAPID_VS_MULTI_LIST);
}


// pokebw2's VsMultiList_FreeText
static void func_ov097_021f69c0(VsMultiListWork *wk) {
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
    GFL_FontFree(wk->smallFont);
    GFL_WordSetSystemFree(wk->wordSet);
    func_0201c4c0(wk->printQueue);
}

// pokebw2's VsMultiList_AddVBlankTask
void func_ov097_021f69e4(VsMultiListWork *wk) {
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov097_021f6a08, wk, 0);
}

// pokebw2's VsMultiList_RemoveVBlankTask
static void func_ov097_021f69fc(VsMultiListWork *wk) {
    GFL_TCBRemove(wk->vblankTcb);
}

// pokebw2's VsMultiList_VBlank
void func_ov097_021f6a08(TCB *tcb, void *data) {
    GFL_BGSysUpdate();
    func_0204a648();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// Takes the first three Pokémon of each team, or none for a slot that the team has no Pokémon for
// pokebw2's VsMultiList_InitParties
void func_ov097_021f6a28(VsMultiListWork *wk) {
    PokeParty *party0;
    PokeParty *party1;
    u32 i;

    if (wk->param->flipped == 0) {
        party0 = wk->param->party0;
        party1 = wk->param->party1;
    } else {
        party0 = wk->param->party1;
        party1 = wk->param->party0;
    }
    for (i = 0; i < BOARD_POKEMON_COUNT; i++) {
        if (PokeParty_GetPkmCount(party0) > i) {
            wk->pkm[i] = PokeParty_GetPkm(party0, i);
        } else {
            wk->pkm[i] = NULL;
        }
        if (PokeParty_GetPkmCount(party1) > i) {
            wk->pkm[i + BOARD_POKEMON_COUNT] = PokeParty_GetPkm(party1, i);
        } else {
            wk->pkm[i + BOARD_POKEMON_COUNT] = NULL;
        }
    }
}

void func_ov097_021f6a98(VsMultiListWork *wk) {
    u32 i;

    BmpWin_InitAllocator(HEAPID_VS_MULTI_LIST);
    for (i = 0; i < POKEMON_COUNT; i++) {
        wk->printWindows[i].window = BmpWin_CreateDynamic(0, 0, 0, 15, 5, 9, TRUE);
    }
    for (i = 0; i < POKEMON_COUNT; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->printWindows[i].window), 0);
        if (wk->pkm[i] != NULL) {
            func_ov097_021f6b7c(wk, i);
            func_ov097_021f6ce4(wk, i);
            func_ov097_021f6d70(wk, i);
            func_ov097_021f6ee8(wk, i);
        } else {
            BmpWin_FlushChar(wk->printWindows[i].window);
        }
    }
}


// pokebw2's VsMultiList_FreeWindows
static void func_ov097_021f6b1c(VsMultiListWork *wk) {
    u32 i;

    for (i = 0; i < POKEMON_COUNT; i++) {
        BmpWin_Free(wk->printWindows[i].window);
    }
    BmpWin_FreeAllocator();
}

// pokebw2's VsMultiList_FlushText
static void func_ov097_021f6b38(VsMultiListWork *wk) {
    u32 i;

    func_0201c4e4(wk->printQueue);
    for (i = 0; i < POKEMON_COUNT; i++) {
        PrintWindow_Flush(&wk->printWindows[i], wk->printQueue);
    }
}

static void func_ov097_021f6b7c(VsMultiListWork *wk, u32 index) {
    BmpWin *window = wk->printWindows[index].window;
    StrBuf *name = GFL_StrBufCreate(12, BG_HEAP);
    StrBuf *template = GFL_MsgDataLoadStrbufNew(wk->msgData, EggDemo_Text_000);

    loadPokemonNicknameToStrbuf(wk->wordSet, 0, wk->pkm[index]);
    GFL_WordSetFormatStrbuf(wk->wordSet, name, template);
    PrintWindow_PrintColor(&wk->printWindows[index], wk->printQueue, 32, 7, name, wk->font, PRINT_COLOR(15, 14, 0));
    GFL_StrBufFree(template);
    GFL_StrBufFree(name);
    if (PokeParty_GetParam(wk->pkm[index], PKM_PARAM_SHOW_SEX, NULL) == 1 &&
        PokeParty_GetParam(wk->pkm[index], PKM_PARAM_IS_EGG, NULL) == 0) {
        u32 sex = PokeParty_GetSex(wk->pkm[index]);

        if (sex == GENDER_MALE) {
            StrBuf *symbol = GFL_MsgDataLoadStrbufNew(wk->msgData, EggDemo_Text_022);
            Font *font = wk->font;

            PrintWindow_PrintColor(&wk->printWindows[index], wk->printQueue,
                func_02045738(window) * 8 - GFL_FontGetBlockWidth(symbol, wk->font, 0), 7, symbol, font,
                PRINT_COLOR(10, 11, 0));
            GFL_StrBufFree(symbol);
        } else if (sex == GENDER_FEMALE) {
            StrBuf *symbol = GFL_MsgDataLoadStrbufNew(wk->msgData, EggDemo_Text_023);
            Font *font = wk->font;

            PrintWindow_PrintColor(&wk->printWindows[index], wk->printQueue,
                func_02045738(window) * 8 - GFL_FontGetBlockWidth(symbol, wk->font, 0), 7, symbol, font,
                PRINT_COLOR(12, 13, 0));
            GFL_StrBufFree(symbol);
        }
    }
}


static void func_ov097_021f6ce4(VsMultiListWork *wk, u32 index) {
    StrBuf *text = GFL_StrBufCreate(32, BG_HEAP);
    StrBuf *template = GFL_MsgDataLoadStrbufNew(wk->msgData, EggDemo_Text_011);

    WordSetNumber(wk->wordSet, 0, PokeParty_GetParam(wk->pkm[index], PKM_PARAM_LEVEL, NULL), 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, text, template);
    PrintWindow_PrintColor(&wk->printWindows[index], wk->printQueue, 8, 32, text, wk->smallFont, PRINT_COLOR(15, 14, 0));
    GFL_StrBufFree(template);
    GFL_StrBufFree(text);
}

void func_ov097_021f6d70(VsMultiListWork *wk, u32 index) {
    StrBuf *text = GFL_StrBufCreate(32, BG_HEAP);
    StrBuf *slash = GFL_MsgDataLoadStrbufNew(wk->msgData, EggDemo_Text_012);
    u8 slashWidth = GFL_FontGetBlockWidth(slash, wk->smallFont, 0);
    u8 x = 92 - slashWidth / 2;
    StrBuf *number;

    PrintWindow_PrintColor(&wk->printWindows[index], wk->printQueue, x, 32, slash, wk->smallFont, PRINT_COLOR(15, 14, 0));
    GFL_StrBufFree(slash);
    number = GFL_MsgDataLoadStrbufNew(wk->msgData, EggDemo_Text_013);
    WordSetNumber(wk->wordSet, 0, PokeParty_GetParam(wk->pkm[index], PKM_PARAM_HP, NULL), 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, text, number);
    PrintWindow_PrintColor(&wk->printWindows[index], wk->printQueue, x - GFL_FontGetBlockWidth(text, wk->smallFont, 0), 32,
        text, wk->smallFont, PRINT_COLOR(15, 14, 0));
    GFL_StrBufFree(number);
    number = GFL_MsgDataLoadStrbufNew(wk->msgData, EggDemo_Text_013);
    WordSetNumber(wk->wordSet, 0, PokeParty_GetParam(wk->pkm[index], PKM_PARAM_MAX_HP, NULL), 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, text, number);
    PrintWindow_PrintColor(&wk->printWindows[index], wk->printQueue, x + slashWidth, 32, text, wk->smallFont,
        PRINT_COLOR(15, 14, 0));
    GFL_StrBufFree(number);
    GFL_StrBufFree(text);
}



// Draws the bar that the HP is shown in, two lines of pixels
// pokebw2's VsMultiList_DrawHPBar
void func_ov097_021f6ee8(VsMultiListWork *wk, u32 index) {
    GFL_BitmapFillArea(BmpWin_GetBitmap(wk->printWindows[index].window), 64, 27, 48, 1, 1);
    GFL_BitmapFillArea(BmpWin_GetBitmap(wk->printWindows[index].window), 64, 28, 48, 1, 2);
}

// pokebw2's VsMultiList_CreateObjs
void func_ov097_021f6f24(VsMultiListWork *wk) {
    func_ov097_021f6f50(wk);
    func_ov097_021f6f88(wk);
    func_ov097_021f714c(wk);
}

// pokebw2's VsMultiList_FreeObjs
static void func_ov097_021f6f3c(VsMultiListWork *wk) {
    func_ov097_021f72bc(wk);
    func_ov097_021f7108(wk);
    func_ov097_021f6f80();
}

static void func_ov097_021f6f50(VsMultiListWork *wk) {
    ClActSysSetup setup = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 9, 4, 4, 0, 16, 16 };

    ClActSys_Create(&setup, &data_ov097_021f761c, HEAPID_VS_MULTI_LIST);
}


// pokebw2's VsMultiList_FreeClActSys
static void func_ov097_021f6f80(void) {
    func_0204a5ac();
}

void func_ov097_021f6f88(VsMultiListWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, BG_HEAP);
    ArcTool *uiArc;
    u32 *charRes = wk->charRes;
    u16 i;

    for (i = 0; i < POKEMON_COUNT; i++) {
        u16 icon;

        if (wk->pkm[i] != NULL) {
            icon = func_0201ba68(func_0201d624(wk->pkm[i]));
        } else {
            icon = PokeParty_GetIconIndex(0, 0, 0, FALSE);
        }
        charRes[i] = func_0204a6c8(arc, icon, FALSE, CLACT_VRAM_MAIN, HEAPID_VS_MULTI_LIST);
    }
    wk->palRes[0] = func_0204acec(arc, func_0201bc3c(), CLACT_VRAM_MAIN, 0, HEAPID_VS_MULTI_LIST);
    wk->cellRes[0] = func_0204af28(arc, func_0201bc78(), getOBJTileMapping_SubEng(), HEAPID_VS_MULTI_LIST);
    GFL_ArcToolFree(arc);
    uiArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), BG_HEAP);
    wk->charRes[7] = func_0204a6c8(uiArc, 12, FALSE, CLACT_VRAM_MAIN, HEAPID_VS_MULTI_LIST);
    wk->palRes[2] = func_0204abf0(uiArc, 11, CLACT_VRAM_MAIN, 96, HEAPID_VS_MULTI_LIST);
    wk->cellRes[2] = func_0204af28(uiArc, 15, 18, HEAPID_VS_MULTI_LIST);
    wk->charRes[6] = func_0204a6c8(uiArc, 65, FALSE, CLACT_VRAM_MAIN, HEAPID_VS_MULTI_LIST);
    wk->palRes[1] = func_0204abf0(uiArc, 66, CLACT_VRAM_MAIN, 128, HEAPID_VS_MULTI_LIST);
    wk->cellRes[1] = func_0204af28(uiArc, 69, 72, HEAPID_VS_MULTI_LIST);
    wk->charRes[8] = func_0204a6c8(uiArc, func_0202775c(2), FALSE, CLACT_VRAM_MAIN, HEAPID_VS_MULTI_LIST);
    wk->palRes[3] = func_0204abf0(uiArc, func_02027758(), CLACT_VRAM_MAIN, 160, HEAPID_VS_MULTI_LIST);
    wk->cellRes[3] = func_0204af28(uiArc, func_02027760(2), func_02027764(2), HEAPID_VS_MULTI_LIST);
    GFL_ArcToolFree(uiArc);
}


// pokebw2's VsMultiList_FreeObjRes
static void func_ov097_021f7108(VsMultiListWork *wk) {
    u32 i;

    for (i = 0; i < CHAR_RES_COUNT; i++) {
        func_0204a8d4(wk->charRes[i]);
    }
    for (i = 0; i < PLTT_RES_COUNT; i++) {
        func_0204ada4(wk->palRes[i]);
    }
    for (i = 0; i < CELL_RES_COUNT; i++) {
        func_0204afd8(wk->cellRes[i]);
    }
}

void func_ov097_021f714c(VsMultiListWork *wk) {
    u32 i;

    wk->clUnit = func_0204b100(ACTOR_COUNT, 0, HEAPID_VS_MULTI_LIST);
    for (i = 0; i < ACTOR_COUNT; i++) {
        const VsMultiListActor *def = &data_ov097_021f76c8[i];

        wk->actors[i] = func_0204b294(wk->clUnit, wk->charRes[def->charIndex], wk->palRes[def->palIndex],
            wk->cellRes[def->cellIndex], &def->setup, 0, HEAPID_VS_MULTI_LIST);
    }
    for (i = 0; i < POKEMON_COUNT; i++) {
        if (wk->pkm[i] == NULL) {
            func_0204b3dc(wk->actors[i], FALSE);
            func_0204b3dc(wk->actors[i + 6], FALSE);
            func_0204b3dc(wk->actors[i + 18], FALSE);
        } else {
            u16 item;

            func_0204b92c(wk->actors[i], TRUE);
            func_0204b6f4(wk->actors[i], func_0201bbe8(func_0201d624(wk->pkm[i])), TRUE);
            item = PokeParty_GetParam(wk->pkm[i], PKM_PARAM_ITEM, NULL);
            if (item == 0) {
                func_0204b3dc(wk->actors[i + 6], FALSE);
            } else if (PML_ItemIsMail(item) == TRUE) {
                func_0204b8c4(wk->actors[i + 6], 0);
                func_0204b878(wk->actors[i + 6], 1);
            } else {
                func_0204b8c4(wk->actors[i + 6], 0);
                func_0204b878(wk->actors[i + 6], 0);
            }
        }
        func_0204b3dc(wk->actors[i + 12], FALSE);
    }
    if (wk->param->flipped == 0) {
        for (i = 0; i < BOARD_POKEMON_COUNT; i++) {
            func_ov097_021f72dc(wk, i + 3, 128);
            func_ov097_021f72dc(wk, i + 9, 128);
            func_ov097_021f72dc(wk, i + 15, 128);
            func_ov097_021f72dc(wk, i + 21, 128);
        }
    } else {
        for (i = 0; i < BOARD_POKEMON_COUNT; i++) {
            func_ov097_021f72dc(wk, i, -128);
            func_ov097_021f72dc(wk, i + 6, -128);
            func_ov097_021f72dc(wk, i + 12, -128);
            func_ov097_021f72dc(wk, i + 18, -128);
        }
    }
}


// pokebw2's VsMultiList_FreeActors
static void func_ov097_021f72bc(VsMultiListWork *wk) {
    u32 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_0204b1cc(wk->clUnit);
}

// pokebw2's VsMultiList_MoveActor
static void func_ov097_021f72dc(VsMultiListWork *wk, u32 index, s16 dx) {
    ClActorPos pos;

    func_0204b454(wk->actors[index], &pos, 0);
    pos.x += dx;
    func_0204b404(wk->actors[index], &pos, 0);
}

// Moves the three Pokémon's four actors each, starting at the one given
// pokebw2's VsMultiList_MoveActors
static void func_ov097_021f7308(VsMultiListWork *wk, u32 first, s16 dx) {
    u32 i;

    for (i = first; i < first + 3; i++) {
        func_ov097_021f72dc(wk, i, dx);
        func_ov097_021f72dc(wk, i + 6, dx);
        func_ov097_021f72dc(wk, i + 12, dx);
        func_ov097_021f72dc(wk, i + 18, dx);
    }
}

void func_ov097_021f734c(VsMultiListWork *wk) {
    u16 i;
    u16 *screen;
    u16 *buffer;

    wk->frames = func_0202c9b4(BGWINFRAME_TRANSFER_VBLANK, FRAME_COUNT, HEAPID_VS_MULTI_LIST);
    for (i = 0; i < POKEMON_COUNT; i++) {
        func_0202ca3c(wk->frames, i, 1, 16, 6);
        func_0202ca3c(wk->frames, i + 6, 0, 16, 6);
        func_0202cebc(wk->frames, i + 6, wk->printWindows[i].window);
    }
    screen = GFL_BGSysIsScrHeapExists(1);
    buffer = GFL_HeapAllocate(BG_HEAP, 0xc0, FALSE, "vs_multi_list.c", 1542);
    for (i = 0; i < POKEMON_COUNT; i++) {
        MIi_CpuCopy16(screen + i * 0x20, buffer + i * 0x10, 0x20);
    }
    for (i = 0; i < POKEMON_COUNT; i++) {
        if (wk->pkm[i] != NULL) {
            func_0202cac4(wk->frames, i, buffer);
        }
    }
    for (i = 0; i < POKEMON_COUNT; i++) {
        MIi_CpuCopy16(screen + (i + 18) * 0x20, buffer + i * 0x10, 0x20);
    }
    for (i = 0; i < POKEMON_COUNT; i++) {
        if (wk->pkm[i] == NULL) {
            func_0202cac4(wk->frames, i, buffer);
        }
    }
    GFL_HeapFree(buffer);
    GFL_BGSysClearScr(1);
    if (wk->param->flipped == 0) {
        for (i = 3; i < POKEMON_COUNT; i++) {
            if (wk->pkm[i] != NULL) {
                func_0202ce50(wk->frames, i, 0, 0, 16, 6, 5);
            }
        }
    } else {
        for (i = 0; i < BOARD_POKEMON_COUNT; i++) {
            if (wk->pkm[i] != NULL) {
                func_0202ce50(wk->frames, i, 0, 0, 16, 6, 5);
            }
        }
    }
    if (wk->param->flipped == 0) {
        func_0202cb4c(wk->frames, 0, 0, 0);
        func_0202cb4c(wk->frames, 1, 0, 6);
        func_0202cb4c(wk->frames, 2, 0, 12);
        func_0202cb4c(wk->frames, 6, 0, 0);
        func_0202cb4c(wk->frames, 7, 0, 6);
        func_0202cb4c(wk->frames, 8, 0, 12);
        func_0202cb4c(wk->frames, 3, 32, 1);
        func_0202cb4c(wk->frames, 4, 32, 7);
        func_0202cb4c(wk->frames, 5, 32, 13);
        func_0202cb4c(wk->frames, 9, 32, 1);
        func_0202cb4c(wk->frames, 10, 32, 7);
        func_0202cb4c(wk->frames, 11, 32, 13);
    } else {
        func_0202cb4c(wk->frames, 0, -16, 0);
        func_0202cb4c(wk->frames, 1, -16, 6);
        func_0202cb4c(wk->frames, 2, -16, 12);
        func_0202cb4c(wk->frames, 6, -16, 0);
        func_0202cb4c(wk->frames, 7, -16, 6);
        func_0202cb4c(wk->frames, 8, -16, 12);
        func_0202cb4c(wk->frames, 3, 16, 1);
        func_0202cb4c(wk->frames, 4, 16, 7);
        func_0202cb4c(wk->frames, 5, 16, 13);
        func_0202cb4c(wk->frames, 9, 16, 1);
        func_0202cb4c(wk->frames, 10, 16, 7);
        func_0202cb4c(wk->frames, 11, 16, 13);
    }
}


// pokebw2's VsMultiList_FreeFrames
static void func_ov097_021f75e0(VsMultiListWork *wk) {
    func_0202ca0c(wk->frames);
}
