// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov295/game_clear_save.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "field/ev_time.h"
#include "field/game_clear_save.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/fade.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "save/hall_of_fame.h"
#include "save/playtime.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/app_printsys_common.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/rtc.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/version.h"
#include "system/wordset.h"
#include "text/system/0041.h"
#include "gfl/bmp.h"
#include "gfl/graphics.h"

// The records that the first clear sets
#define RECORD_CLEAR_PLAY_TIME 2
#define RECORD_HALL_OF_FAME_ENTRIES 0x47
// The Hall of Fame is extra save block 8
#define HALL_OF_FAME_BLOCK 8
#define MESSAGE_WAIT_FRAMES 120

enum {
    STATE_SETUP,
    STATE_START_FADE_IN,
    STATE_WAIT_FADE_IN,
    STATE_START_SAVE,
    STATE_REGISTER,
    STATE_FREE_BLOCKS,
    STATE_WAIT_SAVE,
    STATE_RESULT_MESSAGE,
    STATE_WAIT,
    STATE_START_FADE_OUT,
    STATE_WAIT_FADE_OUT,
    STATE_CLEANUP,
    STATE_END,
    STATE_WAIT_MESSAGE,
};

typedef struct {
    u16 heapId;
    u16 msgBank;
    GameData *gameData;
    PlayerInfo *playerInfo;
    StrBuf *strBuf;
    Font *font;
    MsgData *msgData;
    WordSet *wordSet;
    BmpWin *window;
    PrintStream *stream;
    TCBExManager *tcbManager;
    AppPrintsysCommon printsys;
    WaitIcon *waitIcon;
    // Whether the save succeeded
    BOOL saved;
    // Whether the team is in the Hall of Fame
    BOOL registered;
    // Whether the intermediate save blocks are freed
    BOOL blocksFreed;
    // Whether the game can't be saved
    BOOL cannotSave;
    // The state once the message is read
    u32 nextState;
    s32 waitFrames;
} GameClearSave;

BOOL func_ov205_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov205_021b95f4(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov205_021b9864(GameProc *proc, u32 *state, void *param, void *work);
void func_ov205_021b9874(GameClearSave *save, GameClearSaveParam *param);
void func_ov205_021b9928(GameClearSave *save);
void func_ov205_021b9a68(GameClearSave *save);
static void func_ov205_021b9970(HeapID heapId);
void func_ov205_021b99d8(void);
void func_ov205_021b99e4(GameClearSave *save, u32 messageId, s16 x, s16 y);
void func_ov205_021b9a94(GameClearSave *save);
void func_ov205_021b9ad8(GameClearSave *save);
void func_ov205_021b9b04(GameClearSave *save);
void func_ov205_021b9b20(GameClearSave *save);
void func_ov205_021b9b78(GameClearSave *save);

// pokebw2's GAME_CLEAR_SAVE_PROC_FUNCTIONS
const GameProcFunctions data_ov205_021b9e54 = {
    func_ov205_021b95a0,
    func_ov205_021b95f4,
    func_ov205_021b9864,
};

const BGSysLCDConfig data_ov205_021b9e60 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, FALSE };
const BGSetup data_ov205_021b9e70 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x8000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE,
};
const BGSysVRAMConfig data_ov205_021b9e90 = {
    GX_VRAM_BG_128_B, GX_VRAM_BGEXTPLTT_NONE, GX_VRAM_SUB_BG_128_C, GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I, GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_A, GX_VRAM_TEXPLTT_01_FG, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's GameClearSave_Setup


BOOL func_ov205_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    GameClearSaveParam *saveParam = param;
    GameSystem *gsys = saveParam->gsys;
    GameClearSave *save;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_GAME_CLEAR_SAVE, 0x40000);
    save = GFL_ProcInitSubsystem(proc, sizeof(GameClearSave), HEAPID_GAME_CLEAR_SAVE);
    MIi_CpuClear32(0, save, sizeof(GameClearSave));
    save->heapId = HEAPID_GAME_CLEAR_SAVE;
    save->msgBank = TEXT_BANK_0041;
    save->gameData = GSYS_GetGameData(gsys);
    save->playerInfo = GetGameDataPlayerInfo(save->gameData);
    if (func_020071f0(GameData_GetSaveControl(save->gameData))) {
        save->cannotSave = TRUE;
    }
    return TRUE;
}

static BOOL func_ov205_021b95f4(GameProc *proc, u32 *state, void *param, void *work) {
    GameClearSave *save = work;
    GameClearSaveParam *saveParam = param;
    BmpWin *window;
    u16 white[1] = { 0xffff };

    GCTX_HIDGetPressedKeys();
    switch (*state) {
    case STATE_SETUP:
        func_ov205_021b9874(save, saveParam);
        (*state)++;
        break;
    case STATE_START_FADE_IN:
        reg_G2_BLDCNT = 0;
        GX_SetVisiblePlane(GX_PLANEMASK_BG0);
        if (getGameVersion() == VERSION_BLACK) {
            GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 0);
        } else {
            GXS_LoadBGPltt(white, 0, 2);
            GFL_FadeSet(FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE, 16, 0, 0);
        }
        (*state)++;
        break;
    case STATE_WAIT_FADE_IN:
        if (!GFL_FadeIsRunning()) {
            *state = STATE_START_SAVE;
        }
        break;
    case STATE_START_SAVE:
        if (save->cannotSave) {
            (*state)++;
            break;
        }
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov205_021b99e4(save, Bank0041_Text_003, 0, 0);
        window = save->window;
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        save->nextState = STATE_REGISTER;
        *state = STATE_WAIT_MESSAGE;
        func_02012dac(save->gameData);
        break;
    case STATE_REGISTER:
        if (!save->cannotSave) {
            if (saveParam->repeat == 0) {
                func_ov205_021b9a68(save);
                func_ov205_021b9a94(save);
            } else {
                func_ov205_021b9b04(save);
                func_ov205_021b9ad8(save);
                func_ov205_021b9b20(save);
            }
        }
        save->waitIcon = func_0202ee8c(save->tcbManager, save->window, 15, 16, save->heapId);
        (*state)++;
        break;
    case STATE_FREE_BLOCKS:
        if (!save->cannotSave && save->registered) {
            func_ov205_021b9b78(save);
            if (save->blocksFreed) {
                (*state)++;
            }
        } else {
            (*state)++;
        }
        break;
    case STATE_WAIT_SAVE:
        if (!save->cannotSave) {
            switch (func_02012dd0(save->gameData)) {
            case 0:
            case 1:
                break;
            case 2:
                save->saved = TRUE;
                (*state)++;
                break;
            case 3:
                save->saved = FALSE;
                (*state)++;
                break;
            }
        } else {
            (*state)++;
        }
        break;
    case STATE_RESULT_MESSAGE:
        func_0202f038(save->waitIcon);
        if (!save->cannotSave && save->saved) {
            func_ov205_021b99e4(save, Bank0041_Text_004, 0, 0);
            window = save->window;
            BmpWin_FlushChar(window);
            BmpWin_FlushMap(window);
            GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
            save->nextState = STATE_WAIT;
            *state = STATE_WAIT_MESSAGE;
            GFL_SndSEPlay(SEQ_SE_SAVE);
        } else {
            func_ov205_021b99e4(save, Bank0041_Text_005, 0, 0);
            window = save->window;
            BmpWin_FlushChar(window);
            BmpWin_FlushMap(window);
            GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
            save->nextState = STATE_WAIT;
            *state = STATE_WAIT_MESSAGE;
        }
        break;
    case STATE_WAIT:
        if (save->waitFrames > MESSAGE_WAIT_FRAMES) {
            (*state)++;
        }
        save->waitFrames++;
        break;
    case STATE_START_FADE_OUT:
        if (getGameVersion() == VERSION_BLACK) {
            GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 0);
        } else {
            GFL_FadeSet(FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE, 0, 16, 0);
        }
        (*state)++;
        break;
    case STATE_WAIT_FADE_OUT:
        if (!GFL_FadeIsRunning()) {
            GFL_BitmapFill(BmpWin_GetBitmap(save->window), 15);
            (*state)++;
        }
        break;
    case STATE_CLEANUP:
        func_ov205_021b9928(save);
        (*state)++;
        break;
    case STATE_END:
        return TRUE;
    case STATE_WAIT_MESSAGE:
        if (func_02028484(&save->printsys, save->stream)) {
            *state = save->nextState;
        }
        break;
    }
    if (save->tcbManager != NULL) {
        GFL_TCBExMgrUpdate(save->tcbManager);
    }
    if (save->waitIcon != NULL) {
        func_0202f0b0(save->waitIcon);
    }
    return FALSE;
}


BOOL func_ov205_021b9864(GameProc *proc, u32 *state, void *param, void *work) {
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_GAME_CLEAR_SAVE);
    return TRUE;
}

void func_ov205_021b9874(GameClearSave *save, GameClearSaveParam *param) {
    GFL_BGSysSetDisplayLayout(GX_DISP_SELECT_MAIN_SUB);
    GX_SetBankForLCDC(GX_VRAM_LCDC_ALL);
    MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM, HW_LCDC_VRAM_SIZE);
    GX_DisableBankForLCDC();
    func_ov205_021b9970(save->heapId);
    save->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, save->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1a0, 0x20, save->heapId);
    save->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, save->msgBank, save->heapId);
    save->wordSet = GFL_WordSetSystemCreateDefault(save->heapId);
    LoadSysMsgBox(0, 1, 12, 0, save->heapId);
    save->window = BmpWin_CreateDynamic(0, 2, 18, 27, 4, 13, 1);
    save->tcbManager = GFL_TCBExMgrCreate(save->heapId, save->heapId, 10, 32);
    save->strBuf = GFL_StrBufCreate(0x400, save->heapId);
}

// pokebw2's GameClearSave_Free
void func_ov205_021b9928(GameClearSave *save) {
    GFL_TCBExMgrFreeTasks(save->tcbManager);
    GFL_TCBExMgrFree(save->tcbManager);
    save->tcbManager = NULL;
    func_0201f7a8(save->window, WINFRAME_TRANSFER_NOW);
    BmpWin_Free(save->window);
    GFL_StrBufFree(save->strBuf);
    GFL_WordSetSystemFree(save->wordSet);
    GFL_MsgDataFree(save->msgData);
    GFL_BGSysReleaseBG(0);
    GFL_FontFree(save->font);
    func_ov205_021b99d8();
}

static void func_ov205_021b9970(HeapID heapId) {
    GFL_BGSysCreate(heapId);
    GFL_BGSysSetVRAMBanks(&data_ov205_021b9e90);
    GFL_BGSysSetLCDConfig(&data_ov205_021b9e60);
    GFL_BGSysCreateBG(0, &data_ov205_021b9e70, BGMODE_TEXT);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 7, 0, 0x1a0, 0x20, heapId);
    if (getGameVersion() == VERSION_WHITE) {
        GFL_BGSysResetStdPalette(0, 0xffff);
    } else {
        GFL_BGSysResetStdPalette(0, 0);
    }
    BmpWin_InitAllocator(heapId);
}


// pokebw2's GameClearSave_FreeBGs
void func_ov205_021b99d8(void) {
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's GameClearSave_PrintMessage
void func_ov205_021b99e4(GameClearSave *save, u32 messageId, s16 x, s16 y) {
    StrBuf *message = GFL_StrBufCreate(0x400, save->heapId);

    GFL_BitmapFill(BmpWin_GetBitmap(save->window), 15);
    GFL_MsgDataLoadStrbuf(save->msgData, messageId, message);
    copyVarForText(save->wordSet, 0, save->playerInfo);
    GFL_WordSetFormatStrbuf(save->wordSet, save->strBuf, message);
    save->stream = func_0201cce4(save->window, x, y, save->strBuf, save->font, func_02012ffc(), save->tcbManager, 0,
                                 save->heapId, 0xf0f);
    GFL_StrBufFree(message);
    BmpWin_DrawFrame(save->window, WINFRAME_TRANSFER_NONE, 1, 12);
}

void func_ov205_021b9a68(GameClearSave *save) {
    RTCDate date;
    PokeParty *party;

    RTC_GetDate(&date);
    party = GameData_GetParty(save->gameData);
    func_0200ea3c(func_02012f68(save->gameData), party, &date, HEAPID_GAMEEVENT);
}


// Sets the record of the play time of the first clear to hours, minutes and seconds in decimal
// pokebw2's GameClearSave_SetClearPlayTime
void func_ov205_021b9a94(GameClearSave *save) {
    GameRecords *records = GameData_GetRecords(save->gameData);
    PlayTime *playTime = func_02012fcc(save->gameData);
    u16 hours = func_0200864c(playTime);
    u8 minutes = func_02008650(playTime);
    u8 seconds = func_02008654(playTime);

    func_02008d1c(records, RECORD_CLEAR_PLAY_TIME, hours * 10000 + minutes * 100 + seconds);
}

// pokebw2's GameClearSave_CountClear
void func_ov205_021b9ad8(GameClearSave *save) {
    SaveControl *saveControl = GameData_GetSaveControl(save->gameData);
    GameRecords *records = GameData_GetRecords(save->gameData);

    if (!func_020071f0(saveControl)) {
        RecordAdd(records, RECORD_HALL_OF_FAME_ENTRIES, 1);
    }
}

// pokebw2's GameClearSave_SetFirstClearTime
void func_ov205_021b9b04(GameClearSave *save) {
    GameRecords *records = GameData_GetRecords(save->gameData);

    if (RecordGet(records, RECORD_HALL_OF_FAME_ENTRIES) == 0) {
        setCurrentSeconds(save->gameData);
    }
}

// pokebw2's GameClearSave_RegisterTeam
void func_ov205_021b9b20(GameClearSave *save) {
    RTCDate date;
    PokeParty *party;
    SaveControl *saveControl;
    HallOfFameSave *hallOfFame;

    RTC_GetDate(&date);
    party = GameData_GetParty(save->gameData);
    saveControl = GameData_GetSaveControl(save->gameData);
    func_0200725c(saveControl, HALL_OF_FAME_BLOCK, HEAPID_GAMEEVENT);
    hallOfFame = getAddressOfExtraSaveBlk(saveControl, HALL_OF_FAME_BLOCK, 0);
    if (!func_020071f0(saveControl)) {
        func_0200ebd0(hallOfFame, party, &date, HEAPID_GAMEEVENT);
        func_02012e4c(save->gameData, HALL_OF_FAME_BLOCK);
        save->registered = TRUE;
    }
}

// pokebw2's GameClearSave_FreeBlocks
void func_ov205_021b9b78(GameClearSave *save) {
    SaveControl *saveControl = GameData_GetSaveControl(save->gameData);

    if (!func_020071f0(saveControl)) {
        u32 state = func_02012e7c(save->gameData, HALL_OF_FAME_BLOCK);

        if (state == 2 || state == 3) {
            freeIntermediateSaveExtraBlksAfterLoad(saveControl, HALL_OF_FAME_BLOCK);
            save->blocksFreed = TRUE;
        }
    }
}
