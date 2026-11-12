// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov307/egg_demo.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/name_entry.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/script_text_banks.h"
#include "constants/sound.h"
#include "demo/egg_demo.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/ioreg.h"
#include "pml/poke_party.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/bmp_menu.h"
#include "system/bmp_menulist.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "field/beacon_view.h"

#define HEAPID_EGG_DEMO 0x8f

// The egg hatching process: its steps, the music, the message and the question whether to give a nickname

#define ACTOR_COUNT 6

// The steps of the process
enum {
    EGG_DEMO_WAIT,
    EGG_DEMO_FADE_IN,
    EGG_DEMO_WAIT_ACTORS,
    EGG_DEMO_WAIT_BGM,
    EGG_DEMO_WAIT_KOUKAN,
    EGG_DEMO_HATCH,
    EGG_DEMO_HATCHED,
    EGG_DEMO_WAIT_WHITE,
    EGG_DEMO_REVEAL,
    EGG_DEMO_SHOW_WINDOW,
    EGG_DEMO_PRINT_HATCHED,
    EGG_DEMO_WAIT_HATCHED,
    EGG_DEMO_WAIT_QUESTION,
    EGG_DEMO_WAIT_ANSWER,
    EGG_DEMO_WAIT_FADE_NAME,
    EGG_DEMO_NAME_ENTRY,
    EGG_DEMO_WAIT_BGM_NAMED,
    EGG_DEMO_END_NAMED,
    EGG_DEMO_WAIT_BGM_END,
    EGG_DEMO_WAIT_FADE_END,
    EGG_DEMO_END,
};

// What the music is doing
enum {
    BGM_IDLE,
    BGM_FADE_OUT,
    BGM_PLAY,
    BGM_LOAD_KOUKAN,
    BGM_KOUKAN,
    BGM_LOAD_FANFARE,
    BGM_FANFARE,
    BGM_FADE_OUT_END,
};

// The answer to whether to give a nickname
enum {
    ANSWER_NO,
    ANSWER_YES,
    ANSWER_NONE,
};

// Black 2's; twinned code confirms the offsets of heapId, graphic, font, printQueue, state, wait, bgm, unk1C,
// dialogState, bgmStep, vblankTask, chars, palette, cellAnims, actors, printStream, tcbManager, window, blankWindow,
// frameChars, msgData, message, showBG1, dialogSetup, dialog, answer, view and procManager in Black
typedef struct {
    HeapID heapId;
    EggDemoGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    u32 state;
    u8 wait;
    u32 bgm;
    u32 unk1C;
    u32 dialogState;
    u32 bgmStep;
    TCB *vblankTask;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    ClActor *actors[ACTOR_COUNT];
    PrintStream *printStream;
    TCBExManager *tcbManager;
    BmpWin *window;
    BmpWin *blankWindow;
    u32 frameChars;
    MsgData *msgData;
    StrBuf *message;
    // Enables BG 1 at the next VBlank
    BOOL showBG1;
    ConfirmDialogSetup dialogSetup;
    BmpMenu *dialog;
    u32 answer;
    EggDemoView *view;
    NameEntryParam *nameEntryParam;
    GameProcManager *procManager;
} EggDemoWork;

static BOOL func_ov215_021f57e0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov215_021f5884(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov215_021f584c(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov215_021f5c08(TCB *tcb, void *data);
static void func_ov215_021f5c20(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f5cfc(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f5d4c(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f5da8(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f5dac(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f5e38(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f5e68(EggDemoParam *param, EggDemoWork *wk);
BOOL func_ov215_021f5e90(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f5eb8(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f5ec8(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f5ecc(EggDemoParam *param, EggDemoWork *wk);
BOOL func_ov215_021f5f74(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f5f84(EggDemoParam *param, EggDemoWork *wk);
BOOL func_ov215_021f5fa0(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f5fb0(EggDemoParam *param, EggDemoWork *wk);
BOOL func_ov215_021f5fcc(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f5fdc(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f5fec(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f6000(EggDemoParam *param, EggDemoWork *wk);
BOOL func_ov215_021f601c(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f602c(EggDemoParam *param, EggDemoWork *wk);
BOOL func_ov215_021f603c(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f604c(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f6060(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f6114(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f6158(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f6164(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f6194(EggDemoParam *param, EggDemoWork *wk, u32 messageId);
BOOL func_ov215_021f6228(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f6280(EggDemoParam *param, EggDemoWork *wk);
static void func_ov215_021f62c8(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f62cc(EggDemoParam *param, EggDemoWork *wk);
void func_ov215_021f62fc(EggDemoParam *param, EggDemoWork *wk);
u32 func_ov215_021f6324(EggDemoParam *param, EggDemoWork *wk);

// pokebw2's EGG_DEMO_PROC_FUNCTIONS
// pokebw2's EggDemo_Exit
// pokebw2's EggDemo_Init
// pokebw2's EggDemo_Main
const GameProcFunctions data_ov215_021f7118 = { func_ov215_021f57e0, func_ov215_021f5884, func_ov215_021f584c };

// pokebw2's sActorSetups
static const ClActorSetup data_ov215_021f7124[ACTOR_COUNT] = {
    { 128, 96, 0, 0, 1 }, { 128, 96, 1, 0, 1 }, { 128, 96, 2, 0, 1 },
    { 128, 96, 3, 0, 1 }, { 128, 96, 4, 0, 1 }, { 128, 96, 5, 0, 1 },
};

static BOOL func_ov215_021f57e0(GameProc *proc, u32 *state, void *param, void *work) {
    EggDemoWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_EGG_DEMO, 0x50000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(EggDemoWork), HEAPID_EGG_DEMO);
    MI_CpuFill8(wk, 0, sizeof(EggDemoWork));
    wk->heapId = HEAPID_EGG_DEMO;
    wk->state = EGG_DEMO_WAIT;
    wk->wait = 5;
    wk->bgm = BGM_IDLE;
    wk->unk1C = 0;
    wk->dialogState = 0;
    func_ov215_021f5c20(param, wk);
    func_ov215_021f5eb8(param, wk);
    GFL_FadeSet(3, 16, 16, 0);
    wk->procManager = CreateGameProcManager(wk->heapId);
    func_0203d34c(0, wk->heapId);
    return TRUE;
}

static BOOL func_ov215_021f584c(GameProc *proc, u32 *state, void *param, void *work) {
    EggDemoWork *wk = work;

    FreeGameProcManager(wk->procManager);
    func_ov215_021f5ec8(param, wk);
    if (wk->state != EGG_DEMO_END_NAMED) {
        func_ov215_021f5cfc(param, wk);
    }
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_EGG_DEMO);
    return TRUE;
}

// pokebw2's EggDemo_Main
static BOOL func_ov215_021f5884(GameProc *proc, u32 *state, void *param, void *work) {
    EggDemoParam *demoParam = param;
    EggDemoWork *wk = work;
    PlayerState *playerState;
    PlayerInfo *playerInfo;
    PokeDexSave *pokedex;
    TrainerGameInfoSave *gameInfo;
    StrBuf *name;
    BOOL procRunning;
    u32 answer;

    GCTX_HIDGetPressedKeys();
    procRunning = GFL_ProcMgrUpdate(wk->procManager);
    if (procRunning == TRUE) {
        return FALSE;
    }
    switch (wk->state) {
    case EGG_DEMO_WAIT:
        if (wk->wait == 0) {
            wk->state = EGG_DEMO_FADE_IN;
            GFL_FadeSet(3, 16, 0, -16);
        } else {
            wk->wait--;
        }
        break;
    case EGG_DEMO_FADE_IN:
        if (GFL_FadeIsRunning() == FALSE) {
            wk->state = EGG_DEMO_WAIT_ACTORS;
            func_ov215_021f5e68(demoParam, wk);
        }
        break;
    case EGG_DEMO_WAIT_ACTORS:
        if (func_ov215_021f5f74(demoParam, wk) == FALSE && func_ov215_021f5e90(demoParam, wk)) {
            wk->state = EGG_DEMO_WAIT_BGM;
            func_ov215_021f5f84(demoParam, wk);
        }
        break;
    case EGG_DEMO_WAIT_BGM:
        if (func_ov215_021f5fa0(demoParam, wk) == FALSE) {
            wk->state = EGG_DEMO_WAIT_KOUKAN;
            func_ov215_021f5fb0(demoParam, wk);
        }
        break;
    case EGG_DEMO_WAIT_KOUKAN:
        if (func_ov215_021f5fcc(demoParam, wk)) {
            wk->state = EGG_DEMO_HATCH;
            func_ov215_021f6a3c(wk->view);
        }
        break;
    case EGG_DEMO_HATCH:
        if (func_ov215_021f6a44(wk->view)) {
            wk->state = EGG_DEMO_HATCHED;
            func_ov215_021f5fdc(demoParam, wk);
        }
        break;
    case EGG_DEMO_HATCHED:
        wk->state = EGG_DEMO_WAIT_WHITE;
        playerState = GameData_GetPlayerState(demoParam->gameData);
        playerInfo = GetGameDataPlayerInfo(demoParam->gameData);
        hatchEgg(demoParam->pkm, playerInfo, ZoneData_GetPlaceNameID(PlayerState_GetZoneID(playerState)), wk->heapId);
        pokedex = GameData_GetPokedex(demoParam->gameData);
        PokeDex_RegistPkm(pokedex, demoParam->pkm);
        addPkmToDex(pokedex, demoParam->pkm);
        func_ov215_021f6a48(wk->view, demoParam->pkm);
        break;
    case EGG_DEMO_WAIT_WHITE:
        if (func_ov215_021f6a84(wk->view)) {
            wk->state = EGG_DEMO_REVEAL;
            func_ov215_021f6a98(wk->view);
        }
        break;
    case EGG_DEMO_REVEAL:
        if (func_ov215_021f6aa4(wk->view)) {
            wk->state = EGG_DEMO_SHOW_WINDOW;
            func_ov215_021f6164(demoParam, wk);
            func_ov215_021f6000(demoParam, wk);
        }
        break;
    case EGG_DEMO_SHOW_WINDOW:
        wk->showBG1 = TRUE;
        wk->state = EGG_DEMO_PRINT_HATCHED;
        break;
    case EGG_DEMO_PRINT_HATCHED:
        func_ov215_021f6194(demoParam, wk, 1);
        wk->state = EGG_DEMO_WAIT_HATCHED;
        break;
    case EGG_DEMO_WAIT_HATCHED:
        if (func_ov215_021f6228(demoParam, wk) && func_ov215_021f601c(demoParam, wk) == FALSE) {
            wk->state = EGG_DEMO_WAIT_QUESTION;
            func_ov215_021f6194(demoParam, wk, 2);
        }
        break;
    case EGG_DEMO_WAIT_QUESTION:
        if (func_ov215_021f6228(demoParam, wk)) {
            wk->state = EGG_DEMO_WAIT_ANSWER;
            func_ov215_021f62fc(demoParam, wk);
        }
        break;
    case EGG_DEMO_WAIT_ANSWER:
        answer = func_ov215_021f6324(demoParam, wk);
        if (answer == ANSWER_NO) {
            wk->state = EGG_DEMO_WAIT_BGM_END;
            GFL_FadeSet(3, 0, 16, 2);
            func_ov215_021f602c(demoParam, wk);
            func_020355d8(FALSE);
        } else if (answer == ANSWER_YES) {
            wk->state = EGG_DEMO_WAIT_FADE_NAME;
            GFL_FadeSet(3, 0, 16, 0);
            func_020355d8(FALSE);
        }
        break;
    case EGG_DEMO_WAIT_FADE_NAME:
        if (GFL_FadeIsRunning() == FALSE) {
            gameInfo = getTrainerGameInfoAddress(GameData_GetSaveControl(demoParam->gameData));
            func_ov215_021f5cfc(demoParam, wk);
            wk->state = EGG_DEMO_NAME_ENTRY;
            wk->nameEntryParam = setupPokemonNameEntry(wk->heapId, demoParam->pkm, 10, NULL, gameInfo);
            QueueGameProc(wk->procManager, OVERLAY_ID(194), &data_ov194_021f5580, wk->nameEntryParam);
        }
        break;
    case EGG_DEMO_NAME_ENTRY:
        if (procRunning != TRUE) {
            if (func_ov010_0216c0e4(wk->nameEntryParam) == FALSE) {
                name = GFL_StrBufCreate(32, wk->heapId);
                func_ov010_0216c0d4(wk->nameEntryParam, name);
                PokeParty_SetParam(demoParam->pkm, PKM_PARAM_NICKNAME, (uintptr_t)name);
                GFL_StrBufFree(name);
                RecordAddOne(GameData_GetRecords(demoParam->gameData), 30);
            }
            func_ov010_0216c0c0(wk->nameEntryParam);
            wk->state = EGG_DEMO_WAIT_BGM_NAMED;
            func_ov215_021f602c(demoParam, wk);
            break;
        }
        return FALSE;
    case EGG_DEMO_WAIT_BGM_NAMED:
        if (func_ov215_021f603c(demoParam, wk) == FALSE) {
            wk->state = EGG_DEMO_END_NAMED;
            func_ov215_021f604c(demoParam, wk);
            return TRUE;
        }
        break;
    case EGG_DEMO_END_NAMED:
        break;
    case EGG_DEMO_WAIT_BGM_END:
        if (func_ov215_021f603c(demoParam, wk) == FALSE) {
            wk->state = EGG_DEMO_WAIT_FADE_END;
            func_ov215_021f604c(demoParam, wk);
        }
        break;
    case EGG_DEMO_WAIT_FADE_END:
        if (GFL_FadeIsRunning() == FALSE) {
            wk->state = EGG_DEMO_END;
            return TRUE;
        }
        break;
    }
    func_ov215_021f5ecc(demoParam, wk);
    if (wk->state != EGG_DEMO_NAME_ENTRY && wk->state != EGG_DEMO_WAIT_BGM_NAMED && wk->state != EGG_DEMO_END_NAMED &&
        wk->state != EGG_DEMO_END) {
        func_ov215_021f6158(demoParam, wk);
        func_ov215_021f62cc(demoParam, wk);
        func_ov215_021f67e0(wk->view);
        func_0201c4e4(wk->printQueue);
        func_ov215_021f64d0(wk->graphic);
        func_ov215_021f64e4(wk->graphic);
        func_ov215_021f6a24(wk->view);
        func_ov215_021f64f0(wk->graphic);
    }
    return FALSE;
}

// pokebw2's EggDemo_VBlank
static void func_ov215_021f5c08(TCB *tcb, void *data) {
    EggDemoWork *wk = data;

    if (wk->showBG1) {
        GFL_BGSysSetBGEnabled(1, TRUE);
        wk->showBG1 = FALSE;
    }
}

// pokebw2's EggDemo_InitGraphics
static void func_ov215_021f5c20(EggDemoParam *param, EggDemoWork *wk) {
    u8 i;

    for (i = 0; i <= 7; i++) {
        GFL_BGSysSetBGEnabled(i, FALSE);
    }
    wk->graphic = func_ov215_021f63cc(1, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov215_021f5c08, wk, 1);
    func_ov215_021f5d4c(param, wk);
    func_ov215_021f5dac(param, wk);
    func_ov215_021f6060(param, wk);
    func_ov215_021f6280(param, wk);
    GFL_BGSysSetBGPriority(0, 2);
    GFL_BGSysSetBGPriority(2, 1);
    GFL_BGSysSetBGPriority(1, 0);
    GFL_BGSysSetBGPriority(4, 0);
    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_BGSysSetBGEnabled(2, TRUE);
    GFL_BGSysSetBGEnabled(4, TRUE);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0,
                     GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 |
                         GX_BLEND_PLANEMASK_BD,
                     0, 0);
    GFL_BGSysResetStdPalette(0, GX_RGB(16, 16, 16));
    GFL_BGSysResetStdPalette(4, GX_RGB(0, 0, 0));
    wk->view = func_ov215_021f6720(wk->heapId, param->pkm);
}

// pokebw2's EggDemo_FreeGraphics
static void func_ov215_021f5cfc(EggDemoParam *param, EggDemoWork *wk) {
    func_ov215_021f67b0(wk->view);
    func_ov215_021f62c8(param, wk);
    func_ov215_021f6114(param, wk);
    func_ov215_021f5e38(param, wk);
    func_ov215_021f5da8(param, wk);
    GFL_TCBRemove(wk->vblankTask);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov215_021f6474(wk->graphic);
}

// pokebw2's EggDemo_InitBG
static void func_ov215_021f5d4c(EggDemoParam *param, EggDemoWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_EGG_DEMO, wk->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 3, 0, 0, 0x20, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 5, 2, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 2, 0, 0, FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(2);
}

// pokebw2's EggDemo_FreeBG
static void func_ov215_021f5da8(EggDemoParam *param, EggDemoWork *wk) {
}

// pokebw2's EggDemo_InitActors
static void func_ov215_021f5dac(EggDemoParam *param, EggDemoWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_EGG_DEMO, wk->heapId);
    u8 i;

    wk->palette = func_0204ac18(arc, 3, 0, 0, 0, 1, wk->heapId);
    wk->chars = func_0204a6c8(arc, 2, 0, 0, wk->heapId);
    wk->cellAnims = func_0204af28(arc, 1, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    for (i = 0; i < ACTOR_COUNT; i++) {
        wk->actors[i] = func_0204b294(func_ov215_021f64fc(wk->graphic), wk->chars, wk->palette, wk->cellAnims,
                                      &data_ov215_021f7124[i], 0, wk->heapId);
        func_0204b92c(wk->actors[i], FALSE);
    }
}

// pokebw2's EggDemo_FreeActors
static void func_ov215_021f5e38(EggDemoParam *param, EggDemoWork *wk) {
    u8 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_0204afd8(wk->cellAnims);
    func_0204a8d4(wk->chars);
    func_0204ada4(wk->palette);
}

// pokebw2's EggDemo_StartActors
void func_ov215_021f5e68(EggDemoParam *param, EggDemoWork *wk) {
    u8 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b908(wk->actors[i], 0);
        func_0204b92c(wk->actors[i], TRUE);
    }
}

// pokebw2's EggDemo_AreActorsDone
BOOL func_ov215_021f5e90(EggDemoParam *param, EggDemoWork *wk) {
    BOOL done = TRUE;
    u8 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        if (func_0204b98c(wk->actors[i])) {
            done = FALSE;
            break;
        }
    }
    return done;
}

// Fades out the field's music, to push it once it is quiet
// pokebw2's EggDemo_InitBGM
static void func_ov215_021f5eb8(EggDemoParam *param, EggDemoWork *wk) {
    GFL_SndBGMFadeOut(30);
    wk->bgm = BGM_FADE_OUT;
}

// pokebw2's EggDemo_FreeBGM
static void func_ov215_021f5ec8(EggDemoParam *param, EggDemoWork *wk) {
}

// pokebw2's EggDemo_UpdateBGM
void func_ov215_021f5ecc(EggDemoParam *param, EggDemoWork *wk) {
    switch (wk->bgm) {
    case BGM_FADE_OUT:
        if (GFL_SndBGMIsFading() == FALSE) {
            GFL_SndBGMSetPaused(TRUE);
            GFL_SndBGMPush();
            wk->bgm = BGM_IDLE;
        }
        break;
    case BGM_PLAY:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            wk->bgm = BGM_IDLE;
        }
        break;
    case BGM_LOAD_KOUKAN:
        if (func_020063b4(SEQ_BGM_KOUKAN, &wk->bgmStep, FALSE)) {
            wk->bgm = BGM_KOUKAN;
        }
        break;
    case BGM_LOAD_FANFARE:
        if (func_020063b4(SEQ_ME_SHINKAOME, &wk->bgmStep, FALSE)) {
            wk->bgm = BGM_FANFARE;
        }
        break;
    case BGM_FANFARE:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            func_ov215_021f5fec(param, wk);
            wk->bgm = BGM_KOUKAN;
        }
        break;
    case BGM_FADE_OUT_END:
        if (GFL_SndBGMIsFading() == FALSE) {
            func_02005d28();
            wk->bgm = BGM_IDLE;
        }
        break;
    }
}

// pokebw2's EggDemo_IsBGMFadingOut
BOOL func_ov215_021f5f74(EggDemoParam *param, EggDemoWork *wk) {
    return wk->bgm == BGM_FADE_OUT;
}

// pokebw2's EggDemo_PlayBGM
void func_ov215_021f5f84(EggDemoParam *param, EggDemoWork *wk) {
    GFL_SndBGMPlay(SEQ_BGM_SHINKA, SND_CHANNEL_MASK_ALL);
    wk->bgm = BGM_PLAY;
}

// pokebw2's EggDemo_IsBGMPlaying
BOOL func_ov215_021f5fa0(EggDemoParam *param, EggDemoWork *wk) {
    return wk->bgm == BGM_PLAY;
}

// pokebw2's EggDemo_PlayKoukan
void func_ov215_021f5fb0(EggDemoParam *param, EggDemoWork *wk) {
    wk->bgmStep = 0;
    func_020063b4(SEQ_BGM_KOUKAN, &wk->bgmStep, TRUE);
    wk->bgm = BGM_LOAD_KOUKAN;
}

// pokebw2's EggDemo_IsKoukanPlaying
BOOL func_ov215_021f5fcc(EggDemoParam *param, EggDemoWork *wk) {
    return wk->bgm == BGM_KOUKAN;
}

// pokebw2's EggDemo_PushBGM
void func_ov215_021f5fdc(EggDemoParam *param, EggDemoWork *wk) {
    GFL_SndBGMSetPaused(TRUE);
    GFL_SndBGMPush();
}

// pokebw2's EggDemo_PopBGM
static void func_ov215_021f5fec(EggDemoParam *param, EggDemoWork *wk) {
    GFL_SndBGMPop();
    GFL_SndBGMSetPaused(FALSE);
    GFL_SndBGMFadeIn(6);
}

// pokebw2's EggDemo_PlayFanfare
void func_ov215_021f6000(EggDemoParam *param, EggDemoWork *wk) {
    wk->bgmStep = 0;
    func_020063b4(SEQ_ME_SHINKAOME, &wk->bgmStep, TRUE);
    wk->bgm = BGM_LOAD_FANFARE;
}

// pokebw2's EggDemo_IsFanfarePlaying
BOOL func_ov215_021f601c(EggDemoParam *param, EggDemoWork *wk) {
    return wk->bgm == BGM_LOAD_FANFARE || wk->bgm == BGM_FANFARE;
}

// pokebw2's EggDemo_FadeOutBGM
void func_ov215_021f602c(EggDemoParam *param, EggDemoWork *wk) {
    GFL_SndBGMFadeOut(60);
    wk->bgm = BGM_FADE_OUT_END;
}

// pokebw2's EggDemo_IsBGMFadingOutEnd
BOOL func_ov215_021f603c(EggDemoParam *param, EggDemoWork *wk) {
    return wk->bgm == BGM_FADE_OUT_END;
}

// Brings back the field's music
// pokebw2's EggDemo_RestoreBGM
void func_ov215_021f604c(EggDemoParam *param, EggDemoWork *wk) {
    GFL_SndBGMPop();
    GFL_SndBGMSetPaused(FALSE);
    GFL_SndBGMFadeIn(60);
}

// pokebw2's EggDemo_InitMsg
static void func_ov215_021f6060(EggDemoParam *param, EggDemoWork *wk) {
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x20, 0x20, wk->heapId);
    wk->blankWindow = BmpWin_CreateDynamic(1, 0, 0, 1, 1, 1, 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->blankWindow), 0);
    BmpWin_FlushChar(wk->blankWindow);
    wk->window = BmpWin_CreateDynamic(1, 1, 19, 30, 4, 1, 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->window), 15);
    BmpWin_FlushChar(wk->window);
    wk->frameChars = LoadCursorImageEndOfHeap(1, 2, 0, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_GLOBAL_2250, wk->heapId);
    wk->tcbManager = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 1, 0);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_BGSysQueueScrLoad(1);
    wk->printStream = NULL;
    wk->message = NULL;
    wk->showBG1 = FALSE;
    GFL_BGSysSetBGEnabled(1, FALSE);
}

// pokebw2's EggDemo_FreeMsg
static void func_ov215_021f6114(EggDemoParam *param, EggDemoWork *wk) {
    if (wk->printStream != NULL) {
        func_0201ce48(wk->printStream);
    }
    if (wk->message != NULL) {
        GFL_StrBufFree(wk->message);
    }
    GFL_TCBExMgrFree(wk->tcbManager);
    GFL_MsgDataFree(wk->msgData);
    GFL_BGSysFreeCharMemory(1, CHAR_POS(wk->frameChars), CHAR_SIZE(wk->frameChars));
    BmpWin_Free(wk->window);
    BmpWin_Free(wk->blankWindow);
}

// pokebw2's EggDemo_UpdateMsg
void func_ov215_021f6158(EggDemoParam *param, EggDemoWork *wk) {
    GFL_TCBExMgrUpdate(wk->tcbManager);
}

// pokebw2's EggDemo_ShowWindow
void func_ov215_021f6164(EggDemoParam *param, EggDemoWork *wk) {
    BmpWin *window;

    BmpWin_DrawFrame(wk->window, WINFRAME_TRANSFER_NOW, CHAR_POS(wk->frameChars), 2);
    window = wk->window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
}

// Prints a message with the Pokémon's species name
// pokebw2's EggDemo_Print
void func_ov215_021f6194(EggDemoParam *param, EggDemoWork *wk, u32 messageId) {
    StrBuf *format;
    WordSet *wordSet;

    GFL_BitmapFill(BmpWin_GetBitmap(wk->window), 15);
    if (wk->printStream != NULL) {
        func_0201ce48(wk->printStream);
    }
    if (wk->message != NULL) {
        GFL_StrBufFree(wk->message);
    }
    format = GFL_MsgDataLoadStrbufNew(wk->msgData, messageId);
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    setPartyPokemonSpeciesNameToStrbuf(wordSet, 0, param->pkm);
    wk->message = GFL_StrBufCreate(256, wk->heapId);
    GFL_WordSetFormatStrbuf(wordSet, wk->message, format);
    GFL_WordSetSystemFree(wordSet);
    GFL_StrBufFree(format);
    wk->printStream = func_0201cce4(wk->window, 0, 0, wk->message, wk->font, func_02012ffc(), wk->tcbManager, 2,
                                    wk->heapId, 15);
}

// pokebw2's EggDemo_IsPrintDone
BOOL func_ov215_021f6228(EggDemoParam *param, EggDemoWork *wk) {
    BOOL done = FALSE;

    switch (func_0201ce30(wk->printStream)) {
    case PRINT_STREAM_RUNNING:
        if ((GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c38()) {
            func_0201ce5c(wk->printStream, 0);
        }
        break;
    case PRINT_STREAM_PAUSED:
        if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
            func_0201ce38(wk->printStream);
        }
        break;
    case PRINT_STREAM_DONE:
        done = TRUE;
        break;
    }
    return done;
}

// The yes/no dialog goes where some characters are free
// pokebw2's EggDemo_InitDialog
static void func_ov215_021f6280(EggDemoParam *param, EggDemoWork *wk) {
    u32 pos = GFL_BGSysAllocChar(1, 0x500, 1);

    GFL_BGSysFreeCharMemory(1, pos, 0x500);
    wk->dialogSetup.bg = 1;
    wk->dialogSetup.x = 24;
    wk->dialogSetup.y = 13;
    wk->dialogSetup.palette = 1;
    wk->dialogSetup.unk4 = pos;
    wk->answer = ANSWER_NONE;
}

// pokebw2's EggDemo_FreeDialog
static void func_ov215_021f62c8(EggDemoParam *param, EggDemoWork *wk) {
}

// pokebw2's EggDemo_UpdateDialog
void func_ov215_021f62cc(EggDemoParam *param, EggDemoWork *wk) {
    u32 result;

    switch (wk->dialogState) {
    case 0:
        break;
    case 1:
        result = func_0201febc(wk->dialog);
        if (result != BMPMENU_NULL) {
            wk->answer = result == 0 ? ANSWER_YES : ANSWER_NO;
            wk->dialogState = 2;
        }
        break;
    }
}

// pokebw2's EggDemo_OpenDialog
void func_ov215_021f62fc(EggDemoParam *param, EggDemoWork *wk) {
    wk->dialog = ShopUI_CreateConfirmDialog(&wk->dialogSetup, CHAR_POS(wk->frameChars), 2, 0, wk->heapId);
    wk->answer = ANSWER_NONE;
    wk->dialogState = 1;
}

// pokebw2's EggDemo_GetAnswer
u32 func_ov215_021f6324(EggDemoParam *param, EggDemoWork *wk) {
    return wk->answer;
}
