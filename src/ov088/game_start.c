// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov162/game_start.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/game_start.h"
#include "app/name_entry.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "demo/intro.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "save/config.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/save_control_intr.h"
#include "save/save_outside.h"
#include "system/bmp_winframe.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "constants/pokemon.h"
#include "save/player_savedata.h"
#include "save/pokedex.h"
#include "save/trainer_data_savedata.h"
#include "nnsys/gfd.h"

extern u32 OVERLAY_162_ID[];

#define HEAPID_DEBUG_GENDER_SELECT 0x39

// Starting the game: a new game runs the intro and the name entries while it creates the save data, and a continue
// loads the save. Both then start the game system. A debug screen, whose questions are blank outside Japan, can set
// the text, the gender and whether the C-Gear is on first

// The most sound sequences that the intro and the name entry can load together
#define PRELOAD_SEQ_MAX 30

// The new game's steps
enum {
    NEW_GAME_INTRO,
    NEW_GAME_WAIT_INTRO,
    NEW_GAME_NAME_ENTRY,
    NEW_GAME_WAIT_NAME_ENTRY,
    NEW_GAME_INTRO_PLAYER_NAMED,
    NEW_GAME_AFTER_PLAYER_NAMED,
    NEW_GAME_WAIT_INTRO_PLAYER_NAMED,
    NEW_GAME_RIVAL_NAME_ENTRY,
    NEW_GAME_WAIT_RIVAL_NAME_ENTRY,
    NEW_GAME_INTRO_RIVAL_NAMED,
    NEW_GAME_AFTER_RIVAL_NAMED,
    NEW_GAME_WAIT_INTRO_RIVAL_NAMED,
    NEW_GAME_END,
};

// The continue's parameters: show the debug screen first, or start with the C-Gear off or on, as the start menu asks
enum {
    CONTINUE_DEBUG,
    CONTINUE_CGEAR_OFF,
    CONTINUE_CGEAR_ON,
};

// The debug screen's questions
enum {
    DEBUG_QUESTION_TEXT,
    DEBUG_QUESTION_GENDER,
    DEBUG_QUESTION_CGEAR,
    DEBUG_QUESTION_END,
};

// The work of the new game and the continue. The first fields are the debug screen's parameter
typedef struct {
    BOOL onlyCGearQuestion;
    Config *config;
    PlayerInfo *playerInfo;
    TrainerGameInfoSave *gameInfo;
    IntroParam introParam;
    NameEntryParam *nameEntryParam;
    SaveControlIntr *saveTask;
    GameProcManager *procManager;
    BGMStackPreload *preloadedSeqs;
    u8 unk_38[0x18];
    u32 pokeVoice;
    PlayerInfo *newPlayerInfo;
    Config *newConfig;
} GameStartWork;

typedef struct {
    TouchRect rects[3];
} TouchRectTable;

typedef struct {
    u8 cursor;
    u32 question;
    Font *font;
    BmpWin *questionWindow;
    BmpWin *answerWindows[2];
    u16 cursorColorAngle;
    GXRgb cursorColor;
} DebugGameStartWork;

static BOOL func_ov088_021bc12c(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov088_021bc230(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov088_021bc2fc(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov088_021bc3b8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov088_021bc3e4(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov088_021bc430(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov088_021bc4a4(PlayerInfo *info, StrBuf *name, u32 gender);
static BOOL func_ov088_021bc4ec(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov088_021bc52c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov088_021bc568(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov088_021bc62c(DebugGameStartWork *wk);
static void func_ov088_021bc734(const BGSetup *setup, u8 bg);
static void func_ov088_021bc75c(DebugGameStartWork *wk);
static void func_ov088_021bc84c(DebugGameStartWork *wk);
static void func_ov088_021bc870(DebugGameStartWork *wk);
static u32 func_ov088_021bc8ac(DebugGameStartWork *wk);
static void func_ov088_021bc944(DebugGameStartWork *wk);

// The static data is declared in this order to keep the original layout (tools/decomp/rodata_order.py)
// pokebw2's sDebugTouchRects
static const TouchRectTable data_ov088_021bd83c = { {
    { 96, 112, 48, 208 },
    { 128, 144, 48, 208 },
    { TOUCH_RECT_END, 0, 0, 0 },
} };

// pokebw2's sDebugVRAMConfig
static const BGSysVRAMConfig data_ov088_021bd8b8 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,      GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sDebugLCDConfig
static const BGSysLCDConfig data_ov088_021bd848 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

// pokebw2's sDebugBG3Setup
static const BGSetup data_ov088_021bd858 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x0c000, 0x8000,
    GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sDebugBG1Setup
static const BGSetup data_ov088_021bd878 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sDebugBG5Setup
static const BGSetup data_ov088_021bd898 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, 0x8000,
    GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's CONTINUE_PROC_FUNCTIONS
// pokebw2's Continue_Exit
// pokebw2's Continue_Init
// pokebw2's Continue_Main
const GameProcFunctions data_ov088_021bd824 = { func_ov088_021bc3b8, func_ov088_021bc3e4, func_ov088_021bc430 };
// pokebw2's NEW_GAME_PROC_FUNCTIONS
// pokebw2's NewGame_Exit
// pokebw2's NewGame_Init
// pokebw2's NewGame_Main
const GameProcFunctions data_ov088_021bd818 = { func_ov088_021bc12c, func_ov088_021bc230, func_ov088_021bc2fc };
// pokebw2's DEBUG_GAME_START_PROC_FUNCTIONS
// pokebw2's DebugGameStart_Init
// pokebw2's DebugGameStart_Main
const GameProcFunctions data_ov088_021bd830 = { func_ov088_021bc4ec, func_ov088_021bc568,
                                                            func_ov088_021bc52c };

// pokebw2's GameStart_NewGame
void func_ov088_021bc0e4(void) {
    GCTX_ProcMgrReplaceProc(OVERLAY_ID(88), &data_ov088_021bd818, NULL);
}

// pokebw2's GameStart_ContinueCGearOff
void func_ov088_021bc0fc(void) {
    GCTX_ProcMgrReplaceProc(OVERLAY_ID(88), &data_ov088_021bd824, (void *)CONTINUE_CGEAR_OFF);
}

// pokebw2's GameStart_ContinueCGearOn
void func_ov088_021bc114(void) {
    GCTX_ProcMgrReplaceProc(OVERLAY_ID(88), &data_ov088_021bd824, (void *)CONTINUE_CGEAR_ON);
}

static BOOL func_ov088_021bc12c(GameProc *proc, u32 *state, void *param, void *work) {
    SaveControl *save = SaveControl_GetInstance();
    GameStartWork *wk = GFL_ProcInitSubsystem(proc, sizeof(GameStartWork), HEAPID_USER);
    u32 seqs[PRELOAD_SEQ_MAX];
    u32 nameEntryCount;
    u32 introCount;
    u32 i;

    wk->saveTask = func_ov088_021bd25c(HEAPID_USER, save);
    func_02007220(save);
    wk->newPlayerInfo = func_0200846c(HEAPID_USER);
    wk->newConfig = func_020082ec(HEAPID_USER);
    wk->onlyCGearQuestion = FALSE;
    wk->config = wk->newConfig;
    wk->playerInfo = wk->newPlayerInfo;
    wk->nameEntryParam = setupNameEntry(HEAPID_USER, 0, 0, 0, 7, NULL, 0);
    wk->procManager = CreateGameProcManager(HEAPID_USER);

    GFL_OvlLoad(OVERLAY_ID(194));
    nameEntryCount = data_ov194_021f557c;
    for (i = 0; i < nameEntryCount; i++) {
        seqs[i] = data_ov194_021f558c[i];
    }
    GFL_OvlUnload(OVERLAY_ID(194));
    GFL_OvlLoad(OVERLAY_ID(204));
    introCount = data_ov204_021bfd20;
    for (i = 0; i < introCount; i++) {
        seqs[nameEntryCount + i] = data_ov204_021bfd30[i];
    }
    GFL_OvlUnload(OVERLAY_ID(204));
    wk->preloadedSeqs = func_02005a98(seqs, nameEntryCount + introCount);

    wk->pokeVoice = PokeVoice_Load(SPECIES_MINCCINO, 0, 0x40, 0, 0, 0, 0, 0);
    wk->introParam.playerInfo = wk->newPlayerInfo;
    wk->introParam.config = wk->newConfig;
    wk->introParam.mode = INTRO_MODE_START;
    wk->introParam.saveTask = wk->saveTask;
    wk->introParam.pokeVoice = wk->pokeVoice;
    return TRUE;
}



extern const GameProcFunctions data_ov204_021bfd24;
extern const GameProcFunctions GAMESYSTEM_PROC_FUNCTIONS;

struct IntroParam {
    PlayerInfo *playerInfo;
    Config *config;
    u32 mode;
    SaveControlIntr *saveTask;
    u32 pokeVoice;
    u32 result;
    const u16 *rivalName;
    u32 unk_18;
};
BOOL func_ov088_021bc230(GameProc *proc, u32 *state, void *param, void *work) {
    GameStartWork *wk = work;
    BOOL running;

    func_ov088_021bd2f4(wk->saveTask);
    running = GFL_ProcMgrUpdate(wk->procManager);
    switch (*state) {
    case NEW_GAME_INTRO:
        QueueGameProc(wk->procManager, OVERLAY_ID(204), &data_ov204_021bfd24, &wk->introParam);
        *state = NEW_GAME_WAIT_INTRO;
        break;
    case NEW_GAME_WAIT_INTRO:
        if (running == TRUE) {
            break;
        }
        *state = NEW_GAME_NAME_ENTRY;
        break;
    case NEW_GAME_NAME_ENTRY:
        wk->nameEntryParam->gender = getTrainerGender(wk->playerInfo);
        wk->nameEntryParam->saveTask = wk->saveTask;
        QueueGameProc(wk->procManager, OVERLAY_ID(194), &data_ov194_021f5580, wk->nameEntryParam);
        *state = NEW_GAME_WAIT_NAME_ENTRY;
        break;
    case NEW_GAME_WAIT_NAME_ENTRY:
        if (running == TRUE) {
            break;
        }
        *state = NEW_GAME_INTRO_PLAYER_NAMED;
        break;
    case NEW_GAME_INTRO_PLAYER_NAMED:
        func_ov088_021bc4a4(wk->newPlayerInfo, wk->nameEntryParam->name, getTrainerGender(wk->newPlayerInfo));
        wk->introParam.mode = 7;
        QueueGameProc(wk->procManager, OVERLAY_ID(204), &data_ov204_021bfd24, &wk->introParam);
        *state = NEW_GAME_WAIT_INTRO_PLAYER_NAMED;
        break;
    case NEW_GAME_WAIT_INTRO_PLAYER_NAMED:
        if (running == TRUE) {
            break;
        }
        *state = NEW_GAME_AFTER_PLAYER_NAMED;
        break;
    case NEW_GAME_AFTER_PLAYER_NAMED:
        if (wk->introParam.result == 0) {
            *state = NEW_GAME_RIVAL_NAME_ENTRY;
        } else {
            *state = NEW_GAME_NAME_ENTRY;
        }
        break;
    case NEW_GAME_RIVAL_NAME_ENTRY:
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov088_021bc2fc(GameProc *proc, u32 *state, void *param, void *work) {
    GameStartWork *wk = work;
    SaveControl *save = SaveControl_GetInstance();
    VecFx32 pos = { 0, 0, 0 };
    GameSystemProcData *procData;
    SaveOutside *outside;

    func_02005b04(wk->preloadedSeqs);
    func_ov088_021bd29c(wk->saveTask);
    func_02008494(wk->newPlayerInfo, func_02008730(save));
    initConfig(wk->newConfig, (Config *)getTrainerDataBlkAddress(save));
    GFL_HeapFree(wk->newPlayerInfo);
    GFL_HeapFree(wk->newConfig);
    FreeGameProcManager(wk->procManager);
    if (func_02007210(save) == TRUE) {
        GFL_OvlLoad(OVERLAY_ID(228));
        outside = func_ov228_021dfc20(HEAPID_USER);
        if (func_ov228_021dfe90(outside) == TRUE) {
            func_ov228_021dfeb0(outside, save);
        }
        func_ov228_021dfdb8(outside);
        GFL_OvlUnload(OVERLAY_ID(228));
    }
    procData = GameSystem_CreateProcData(0, 0, &pos, 0);
    func_ov010_0216c0c0(wk->nameEntryParam);
    GFL_ProcReleaseSubsystem(proc);
    GCTX_ProcMgrReplaceProc(OVERLAY_NONE, &GAMESYSTEM_PROC_FUNCTIONS, procData);
    return TRUE;
}



static BOOL func_ov088_021bc3b8(GameProc *proc, u32 *state, void *param, void *work) {
    GameStartWork *wk = GFL_ProcInitSubsystem(proc, sizeof(GameStartWork), HEAPID_USER);

    wk->onlyCGearQuestion = TRUE;
    wk->config = (Config *)getTrainerDataBlkAddress(SaveControl_GetInstance());
    wk->gameInfo = getTrainerGameInfoAddress(SaveControl_GetInstance());
    wk->playerInfo = NULL;
    return TRUE;
}

static BOOL func_ov088_021bc3e4(GameProc *proc, u32 *state, void *param, void *work) {
    GameStartWork *wk = work;

    switch (*state) {
    case 0:
        if ((uintptr_t)param == CONTINUE_DEBUG) {
            GCTX_ProcMgrQueueProc(OVERLAY_NONE, &data_ov088_021bd830, wk);
        } else if ((uintptr_t)param == CONTINUE_CGEAR_OFF) {
            func_02008450(wk->config, FALSE);
        } else if ((uintptr_t)param == CONTINUE_CGEAR_ON) {
            func_02008450(wk->config, TRUE);
        }
        (*state)++;
        break;
    case 1:
        return TRUE;
    }
    return TRUE;
}

u32 func_020070c8(SaveControl *save);
BOOL func_ov088_021bc430(GameProc *proc, u32 *state, void *param, void *work) {
    GameStartWork *wk = work;
    SaveControl *save = SaveControl_GetInstance();
    u32 cgearOn = func_02008448(wk->config);
    u32 gameInfoValue = func_0200c31c(wk->gameInfo);
    func_020070c8(save);
    func_02008414((Config *)getTrainerDataBlkAddress(save));
    SaveLocation location;
    func_0200890c(save, &location);
    GCTX_ProcMgrReplaceProc(OVERLAY_NONE, &GAMESYSTEM_PROC_FUNCTIONS,
                            GameSystem_CreateProcData(1, location.zoneId, &location.pos, location.unk18));
    func_02008450(wk->config, cgearOn);
    func_0200c324(wk->gameInfo, gameInfoValue);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}


// pokebw2's NewGame_SetUpPlayer
static void func_ov088_021bc4a4(PlayerInfo *info, StrBuf *name, u32 gender) {
    copyTrainerNameFromStrbuf(info, name);
    setTrainerGender(info, gender);
    setIDAsUInt(info, GFL_RandomLCAlt(0xffffffff));
    func_02008558(info, GFL_RandomMTRange(8) + getTrainerGender(info) * 8);
}

static BOOL func_ov088_021bc4ec(GameProc *proc, u32 *state, void *param, void *work) {
    GameStartWork *gameStart = param;
    DebugGameStartWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_DEBUG_GENDER_SELECT, 0x80000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(DebugGameStartWork), HEAPID_DEBUG_GENDER_SELECT);
    if (gameStart->onlyCGearQuestion == FALSE) {
        wk->question = DEBUG_QUESTION_TEXT;
    } else {
        wk->question = DEBUG_QUESTION_CGEAR;
    }
    wk->cursorColorAngle = 0;
    func_ov088_021bc62c(wk);
    func_ov088_021bc75c(wk);
    return TRUE;
}

// pokebw2's DebugGameStart_Exit
static BOOL func_ov088_021bc52c(GameProc *proc, u32 *state, void *param, void *work) {
    DebugGameStartWork *wk = work;

    func_ov088_021bc84c(wk);
    GFL_FontFree(wk->font);
    BmpWin_FreeAllocator();
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysFree();
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_DEBUG_GENDER_SELECT);
    return TRUE;
}

// Answers come as 1 for the first option and 2 for the second
static BOOL func_ov088_021bc568(GameProc *proc, u32 *state, void *param, void *work) {
    GameStartWork *gameStart = param;
    DebugGameStartWork *wk = work;
    u32 answer;

    switch (wk->question) {
    case DEBUG_QUESTION_TEXT:
        answer = func_ov088_021bc8ac(wk);
        if (answer == 0) {
            break;
        }
        if (answer == 1) {
            func_020083ec(gameStart->config, 0);
        } else {
            func_020083ec(gameStart->config, 1);
        }
        func_ov088_021bc84c(wk);
        wk->question = DEBUG_QUESTION_GENDER;
        func_ov088_021bc75c(wk);
        break;
    case DEBUG_QUESTION_GENDER:
        answer = func_ov088_021bc8ac(wk);
        if (answer == 0) {
            break;
        }
        if (answer == 1) {
            setTrainerGender(gameStart->playerInfo, GENDER_MALE);
        } else {
            setTrainerGender(gameStart->playerInfo, GENDER_FEMALE);
        }
        func_ov088_021bc84c(wk);
        wk->question = DEBUG_QUESTION_CGEAR;
        func_ov088_021bc75c(wk);
        break;
    case DEBUG_QUESTION_CGEAR:
        answer = func_ov088_021bc8ac(wk);
        if (answer == 0) {
            break;
        }
        if (answer == 1) {
            func_02008450(gameStart->config, TRUE);
        } else {
            func_02008450(gameStart->config, FALSE);
        }
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 12, 1,
                    HEAPID_DEBUG_GENDER_SELECT);
        wk->question = DEBUG_QUESTION_END;
        break;
    case DEBUG_QUESTION_END:
        if (GFL_WipeIsFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    func_ov088_021bc944(wk);
    return FALSE;
}

// pokebw2's DebugGameStart_InitBG
static void func_ov088_021bc62c(DebugGameStartWork *wk) {
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GFL_BGSysSetVRAMBanks(&data_ov088_021bd8b8);
    GFL_BGSysCreate(HEAPID_DEBUG_GENDER_SELECT);
    GFL_BGSysSetLCDConfig(&data_ov088_021bd848);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    func_ov088_021bc734(&data_ov088_021bd858, 3);
    func_ov088_021bc734(&data_ov088_021bd878, 1);
    func_ov088_021bc734(&data_ov088_021bd898, 5);
    BmpWin_InitAllocator(HEAPID_DEBUG_GENDER_SELECT);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0, 0x20, HEAPID_DEBUG_GENDER_SELECT);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0, 0x20, HEAPID_DEBUG_GENDER_SELECT);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, HEAPID_DEBUG_GENDER_SELECT);
    LoadSysMsgBox(1, 1, 3, 0, HEAPID_DEBUG_GENDER_SELECT);
    LoadSysMsgBox(1, 31, 4, 0, HEAPID_DEBUG_GENDER_SELECT);
    LoadSysMsgBox(5, 1, 1, 0, HEAPID_DEBUG_GENDER_SELECT);
    *(vu16 *)HW_BG_PLTT = GX_RGB(12, 12, 31);
    *(vu16 *)HW_DB_BG_PLTT = GX_RGB(12, 12, 31);
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
}

// pokebw2's DebugGameStart_CreateBG
static void func_ov088_021bc734(const BGSetup *setup, u8 bg) {
    GFL_BGSysCreateBG(bg, setup, BGMODE_TEXT);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

// The question on the sub screen, and the two answers on the main screen. The messages are those of system message
// file 363: the questions from 0, and their answers from 5, two to a question
// pokebw2's DebugGameStart_CreateWindows
static void func_ov088_021bc75c(DebugGameStartWork *wk) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, 0xab, HEAPID_DEBUG_GENDER_SELECT);
    StrBuf *strbuf;
    u8 i;

    wk->cursor = 0;
    wk->questionWindow = BmpWin_CreateDynamic(5, 1, 19, 29, 4, 0, 1);
    BmpWin_FlushMap(wk->questionWindow);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->questionWindow), 15);
    strbuf = GFL_MsgDataLoadStrbufNew(msgData, wk->question);
    BmpWin_DrawFrame(wk->questionWindow, WINFRAME_TRANSFER_NOW, 1, 1);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->questionWindow), 2, 2, strbuf, wk->font);
    GFL_StrBufFree(strbuf);
    BmpWin_FlushChar(wk->questionWindow);
    for (i = 0; i < 2; i++) {
        wk->answerWindows[i] = BmpWin_CreateDynamic(1, 6, 12 + i * 4, 20, 2, 0, 1);
        BmpWin_FlushMap(wk->answerWindows[i]);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->answerWindows[i]), 15);
        strbuf = GFL_MsgDataLoadStrbufNew(msgData, 5 + i + wk->question * 2);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->answerWindows[i]), 2, 2, strbuf, wk->font);
        GFL_StrBufFree(strbuf);
        BmpWin_FlushChar(wk->answerWindows[i]);
    }
    GFL_MsgDataFree(msgData);
    func_ov088_021bc870(wk);
}

// pokebw2's DebugGameStart_FreeWindows
static void func_ov088_021bc84c(DebugGameStartWork *wk) {
    u8 i;

    BmpWin_Free(wk->questionWindow);
    for (i = 0; i < 2; i++) {
        BmpWin_Free(wk->answerWindows[i]);
    }
}

// The chosen answer gets a frame in palette 3, whose color cycles, and the other in palette 4
// pokebw2's DebugGameStart_DrawCursor
static void func_ov088_021bc870(DebugGameStartWork *wk) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (i == wk->cursor) {
            BmpWin_DrawFrame(wk->answerWindows[i], WINFRAME_TRANSFER_NOW, 1, 3);
        } else {
            BmpWin_DrawFrame(wk->answerWindows[i], WINFRAME_TRANSFER_NOW, 31, 4);
        }
    }
}

// Returns 1 or 2 for the answer that A or a touch chose, or 0
// pokebw2's DebugGameStart_UpdateMenu
static u32 func_ov088_021bc8ac(DebugGameStartWork *wk) {
    u32 answer;
    s32 touched;

    if ((GCTX_HIDGetPressedKeys() & PAD_KEY_UP) || (GCTX_HIDGetPressedKeys() & PAD_KEY_DOWN)) {
        wk->cursor = wk->cursor == 0 ? 1 : 0;
        func_ov088_021bc870(wk);
        GFL_SndSEPlay(SEQ_SE_SELECT1);
    }
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
        answer = 1;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (wk->cursor != 0) {
            answer = 2;
        }
        return answer;
    }
    {
        TouchRectTable table = data_ov088_021bd83c;

        touched = func_02035c08(table.rects);
    }
    if (touched != TOUCH_RECT_NONE) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (touched == 0) {
            return 1;
        }
        return 2;
    }
    return 0;
}

// Cycles the chosen answer's frame color, through palette 3 of the main screen
// pokebw2's DebugGameStart_CycleCursorColor
static void func_ov088_021bc944(DebugGameStartWork *wk) {
    fx16 t;
    u8 r;
    u8 g;
    u8 b;

    if (wk->cursorColorAngle + 0x400 >= 0x10000) {
        wk->cursorColorAngle = wk->cursorColorAngle + 0x400 - 0x10000;
    } else {
        wk->cursorColorAngle += 0x400;
    }
    t = (FX_CosIdx(wk->cursorColorAngle) + FX16_ONE) / 2;
    b = 29 + ((-8 * t) >> FX32_SHIFT);
    r = 25 + ((-20 * t) >> FX32_SHIFT);
    g = 30 + ((-15 * t) >> FX32_SHIFT);
    wk->cursorColor = GX_RGB(r, g, b);
    NNS_GfdRegisterNewVramTransferTask(15, 3 * 0x20 + 6 * 2, &wk->cursorColor, sizeof(GXRgb));
}
