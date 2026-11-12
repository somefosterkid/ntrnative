// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov284/shinka_demo.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/ui_scene.h"
#include "app/ui/yesno_menu.h"
#include "app/p_status.h"
#include "battle/b_app_tool.h"
#include "battle/b_plist_main.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "demo/shinka_demo.h"
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
#include "pml/mail.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "save/bag.h"
#include "save/pokedex.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/app_keycursor.h"
#include "system/bmp_winframe.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "field/beacon_view.h"
#include "save/trainer_data_savedata.h"

#define HEAPID_SHINKA_DEMO 0x94
#define HEAPID_SHINKA_DEMO_GRAPHIC 0x68 // Black's (pokebw2: 0x4c)

// The evolution process: its steps, the music, the messages, learning the new form's moves and making a Shedinja

#define ACTOR_COUNT 6

// The steps of the process
enum {
    SHINKA_DEMO_WAIT,
    SHINKA_DEMO_FADE_IN,
    SHINKA_DEMO_WAIT_ACTORS,
    SHINKA_DEMO_SHOW_WINDOW,
    SHINKA_DEMO_PRINT_EVOLVING,
    SHINKA_DEMO_WAIT_EVOLVING,
    SHINKA_DEMO_WAIT_CRY,
    SHINKA_DEMO_WAIT_BGM,
    SHINKA_DEMO_EVOLVE,
    SHINKA_DEMO_EVOLVED,
    SHINKA_DEMO_SHOW_WINDOW_EVOLVED,
    SHINKA_DEMO_PRINT_EVOLVED,
    SHINKA_DEMO_WAIT_EVOLVED,
    SHINKA_DEMO_CANCELLED,
    SHINKA_DEMO_SHOW_WINDOW_CANCELLED,
    SHINKA_DEMO_PRINT_CANCELLED,
    SHINKA_DEMO_WAIT_CANCELLED,
    SHINKA_DEMO_AFTER_EVOLVING,
    SHINKA_DEMO_LEARN_MOVE,
    SHINKA_DEMO_WAIT_LEARNED,
    SHINKA_DEMO_PRINT_WANTS_MOVE,
    SHINKA_DEMO_WAIT_WANTS_MOVE,
    SHINKA_DEMO_WAIT_FORGET_ANSWER,
    SHINKA_DEMO_WAIT_FADE_OV207,
    SHINKA_DEMO_OV207,
    SHINKA_DEMO_OV207_END,
    SHINKA_DEMO_WAIT_FADE_OV207_END,
    SHINKA_DEMO_WAIT_FADE_OV287,
    SHINKA_DEMO_OV287,
    SHINKA_DEMO_WAIT_FADE_OV287_END,
    SHINKA_DEMO_OV287_END,
    SHINKA_DEMO_FORGET,
    SHINKA_DEMO_SHOW_WINDOW_FORGET,
    SHINKA_DEMO_PRINT_FORGOT,
    SHINKA_DEMO_WAIT_FORGOT,
    SHINKA_DEMO_WAIT_LEARNED_NEW,
    SHINKA_DEMO_GIVE_UP,
    SHINKA_DEMO_SHOW_WINDOW_GIVE_UP,
    SHINKA_DEMO_PRINT_GIVE_UP,
    SHINKA_DEMO_WAIT_GIVE_UP,
    SHINKA_DEMO_WAIT_GIVE_UP_ANSWER,
    SHINKA_DEMO_WAIT_NOT_LEARNED,
    SHINKA_DEMO_END,
    SHINKA_DEMO_WAIT_BGM_END,
    SHINKA_DEMO_WAIT_FADE_END,
    SHINKA_DEMO_DONE,
};

// What the music is doing
enum {
    BGM_IDLE,
    BGM_FADE_OUT,
    BGM_PLAY,
    BGM_LOAD_KOUKAN,
    BGM_KOUKAN,
    BGM_PUSHED,
    BGM_FADE_OUT_END,
    BGM_LOAD_FANFARE,
    BGM_FANFARE,
    BGM_LEVEL_UP,
};

// What to do with BG 1, the message window's, at the next VBlank
enum {
    BG1_NONE,
    BG1_SHOW,
    BG1_HIDE,
};

// The kinds of the words in a message
enum {
    WORD_STRBUF,
    WORD_SPECIES,
    WORD_NICKNAME,
    // A pointer to a u16 move
    WORD_MOVE,
    WORD_NONE,
};

typedef struct {
    u32 type;
    void *value;
} MessageWord;

// Black 2's; twinned code confirms the offsets of heapId, graphicHeapId, font, pkm, nickname, cancelled, state, wait,
// bgm, bgmStep, vblankTask, chars, palette, cellAnims, actors, actorsCreated, printStream, tcbExManager, window,
// blankWindow, frameChars, msgData, moveMsgData, message, keyCursor, bg1Request, windowShown, menuMsgData, choices,
// menu, tcbManager, tcbBuffer, paletteFade, usingKeys, pstatusParam, unk10C, unk110, unk114, unk118 and procManager in
// Black
typedef struct {
    HeapID heapId;
    HeapID graphicHeapId;
    ShinkaDemoGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    PartyPkm *pkm;
    // The nickname before evolving
    StrBuf *nickname;
    u16 forgetMove;
    u8 slot;
    u16 move;
    u32 learnIndex;
    BOOL cancelled;
    u32 state;
    u32 wait;
    u32 bgm;
    u32 bgmStep;
    TCB *vblankTask;
    u32 chars;
    u32 palette;
    u32 cellAnims;
    ClActor *actors[ACTOR_COUNT];
    BOOL actorsCreated;
    PrintStream *printStream;
    TCBExManager *tcbExManager;
    BmpWin *window;
    BmpWin *blankWindow;
    u32 frameChars;
    MsgData *msgData;
    MsgData *moveMsgData;
    StrBuf *message;
    KeyCursor *keyCursor;
    u32 bg1Request;
    BOOL windowShown;
    MsgData *menuMsgData;
    StrBuf *choices[2];
    TwoChoiceMenu *menu;
    TCBManager *tcbManager;
    void *tcbBuffer;
    PaletteFade *paletteFade;
    // Whether the player was using the keys rather than the touch screen, for overlay 287's screen
    u8 usingKeys;
    BPlistParam ov287Param;
    PStatusParam *pstatusParam;
    ShinkaDemoView *view;
    ShinkaDemoEffect *effect;
    BOOL unk10C;
    BOOL unk110;
    BOOL unk114;
    BOOL unk118;
    GameProcManager *procManager;
} ShinkaDemoWork;

static BOOL func_ov195_02203fa0(GameProc *proc, u32 *state, void *heapId, void *work);
BOOL func_ov195_022040e0(GameProc *proc, u32 *state, void *heapId, void *work);
static BOOL func_ov195_02204098(GameProc *proc, u32 *state, void *heapId, void *work);
static void func_ov195_02204c24(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_02204d5c(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_02204dc4(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_02204ddc(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204de0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
BOOL func_ov195_02204ea0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204eb0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204ed0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
BOOL func_ov195_02204ef0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204f00(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204f24(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_02204f40(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
BOOL func_ov195_02204f68(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204f78(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
BOOL func_ov195_02204f90(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204fa0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
BOOL func_ov195_02204fc4(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204fd4(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02204ff8(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_02205010(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_0220506c(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_02205070(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_02205100(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02205138(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
BOOL func_ov195_02205168(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_022051a8(TCB *tcb, void *data);
static void func_ov195_022051fc(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_022052e8(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_02205340(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
BOOL func_ov195_02205360(u32 event);
void func_ov195_02205384(ShinkaDemoWork *wk, MsgData *msgData, u32 messageId, PrintStreamCallback callback, u32 type0, void *word0, u32 type1, void *word1);
BOOL func_ov195_022053e8(ShinkaDemoWork *wk);
BOOL func_ov195_0220546c(ShinkaDemoWork *wk);
static void func_ov195_0220549c(ShinkaDemoWork *wk, u8 color);
void func_ov195_022054e8(ShinkaDemoParam *heapId, ShinkaDemoWork *wk, BOOL show);
void func_ov195_02205530(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
static void func_ov195_02205580(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);
void func_ov195_022055bc(ShinkaDemoWork *wk, MsgData *msgData0, u32 messageId0, u32 type00, void *word00, u32 type10, void *word10, MsgData *msgData1, u32 messageId1, u32 type01, void *word01, u32 type11, void *word11);
static StrBuf *func_ov195_02205620(HeapID heapId, MsgData *msgData, u32 messageId, u32 type0, void *word0,
                                        u32 type1, void *word1);
void func_ov195_022056e8(ShinkaDemoParam *param, ShinkaDemoWork *wk);
void func_ov195_02205844(ShinkaDemoParam *heapId, ShinkaDemoWork *wk);

// pokebw2's SHINKA_DEMO_PROC_FUNCTIONS
// pokebw2's ShinkaDemo_Exit
// pokebw2's ShinkaDemo_Init
// pokebw2's ShinkaDemo_Main
const GameProcFunctions data_ov195_02208c40 = { func_ov195_02203fa0, func_ov195_022040e0, func_ov195_02204098 };

// pokebw2's sActorSetups
static const ClActorSetup data_ov195_02208c4c[ACTOR_COUNT] = {
    { 128, 96, 0, 0, 1 }, { 128, 96, 1, 0, 1 }, { 128, 96, 2, 0, 1 },
    { 128, 96, 3, 0, 1 }, { 128, 96, 4, 0, 1 }, { 128, 96, 5, 0, 1 },
};


static BOOL func_ov195_02203fa0(GameProc *proc, u32 *state, void *heapId, void *work) {
    ShinkaDemoParam *demoParam = heapId;
    ShinkaDemoWork *wk;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_SHINKA_DEMO, 0x10000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ShinkaDemoWork), HEAPID_SHINKA_DEMO);
    MI_CpuFill8(wk, 0, sizeof(ShinkaDemoWork));
    wk->heapId = HEAPID_SHINKA_DEMO;
    wk->state = SHINKA_DEMO_WAIT;
    wk->wait = 5;
    wk->bgm = BGM_IDLE;
    wk->pkm = PokeParty_GetPkm(demoParam->party, demoParam->partyIndex);
    wk->nickname = GFL_StrBufCreate(256, wk->heapId);
    PokeParty_GetParam(wk->pkm, PKM_PARAM_NICKNAME, wk->nickname);
    wk->cancelled = FALSE;
    wk->tcbManager = NULL;
    wk->tcbBuffer = NULL;
    wk->paletteFade = NULL;
    wk->usingKeys = FALSE;
    wk->pstatusParam = GFL_HeapAllocate(wk->heapId, sizeof(PStatusParam), TRUE, "shinka_demo.c", 578);
    GFL_FadeSet(3, 16, 16, -16);
    func_ov195_02204dc4(heapId, wk);
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov195_022051a8, wk, 1);
    wk->unk10C = FALSE;
    wk->unk110 = FALSE;
    wk->unk114 = FALSE;
    wk->unk118 = FALSE;
    wk->procManager = CreateGameProcManager(wk->heapId);
    func_ov195_02204c24(heapId, wk);
    func_0203d34c(0, wk->heapId);
    return TRUE;
}

static BOOL func_ov195_02204098(GameProc *proc, u32 *state, void *heapId, void *work) {
    ShinkaDemoWork *wk = work;

    func_ov195_02204d5c(heapId, wk);
    FreeGameProcManager(wk->procManager);
    GFL_TCBRemove(wk->vblankTask);
    func_ov195_02204ddc(heapId, wk);
    GFL_HeapFree(wk->pstatusParam);
    GFL_StrBufFree(wk->nickname);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_SHINKA_DEMO);
    return TRUE;
}

BOOL func_ov195_022040e0(GameProc *proc, u32 *state, void *heapId, void *work) {
    ShinkaDemoParam *demoParam = heapId;
    ShinkaDemoWork *wk = work;
    BOOL procRunning;
    u32 keys;
    u32 result;
    PokeDexSave *pokedex;
    GameRecords *records;
    SaveControl *save;

    procRunning = GFL_ProcMgrUpdate(wk->procManager);
    if (procRunning == TRUE) {
        return FALSE;
    }
    switch (wk->state) {
    case SHINKA_DEMO_WAIT:
        if (wk->wait == 0) {
            wk->state = SHINKA_DEMO_FADE_IN;
            GFL_FadeSet(3, 16, 0, -16);
        } else {
            wk->wait--;
        }
        break;
    case SHINKA_DEMO_FADE_IN:
        if (GFL_FadeIsRunning() == FALSE) {
            wk->state = SHINKA_DEMO_WAIT_ACTORS;
            func_ov195_02205138(heapId, wk);
        }
        break;
    case SHINKA_DEMO_WAIT_ACTORS:
        if (func_ov195_02204ea0(heapId, wk) == FALSE && func_ov195_02205168(heapId, wk)) {
            if (wk->windowShown) {
                wk->state = SHINKA_DEMO_PRINT_EVOLVING;
            } else {
                wk->state = SHINKA_DEMO_SHOW_WINDOW;
                func_ov195_022054e8(heapId, wk, TRUE);
            }
        }
        break;
    case SHINKA_DEMO_SHOW_WINDOW:
        wk->bg1Request = BG1_SHOW;
        wk->state = SHINKA_DEMO_PRINT_EVOLVING;
        break;
    case SHINKA_DEMO_PRINT_EVOLVING:
        func_ov195_02205384(wk, wk->msgData, 0, NULL, WORD_NICKNAME, wk->pkm, WORD_NONE, NULL);
        wk->state = SHINKA_DEMO_WAIT_EVOLVING;
        break;
    case SHINKA_DEMO_WAIT_EVOLVING:
        if (func_ov195_022053e8(wk) && func_ov195_0220546c(wk)) {
            func_ov195_022054e8(heapId, wk, FALSE);
            wk->state = SHINKA_DEMO_WAIT_CRY;
            func_ov195_02206044(wk->view);
        }
        break;
    case SHINKA_DEMO_WAIT_CRY:
        if (func_ov195_0220604c(wk->view)) {
            wk->state = SHINKA_DEMO_WAIT_BGM;
            func_ov195_02204ed0(heapId, wk);
        }
        break;
    case SHINKA_DEMO_WAIT_BGM:
        if (func_ov195_02204ef0(heapId, wk) == FALSE) {
            wk->state = SHINKA_DEMO_EVOLVE;
            func_ov195_02206050(wk->view);
            func_ov195_0220882c(wk->effect);
            GFL_SndSEPlay(SEQ_SE_SHDEMO_01);
            wk->unk10C = TRUE;
            wk->wait = 0;
        }
        break;
    case SHINKA_DEMO_EVOLVE:
        keys = GCTX_HIDGetPressedKeys();
        if (func_ov195_0220606c(wk->view)) {
            if (wk->cancelled) {
                wk->state = SHINKA_DEMO_CANCELLED;
            } else {
                wk->state = SHINKA_DEMO_EVOLVED;
            }
            break;
        }
        if (func_ov195_02206070(wk->view)) {
            func_ov195_02204f00(heapId, wk);
        }
        if (func_ov195_02206074(wk->view)) {
            func_ov195_02204f24(heapId, wk);
        }
        if (func_ov195_02206058(wk->view)) {
            func_ov195_02208844(wk->effect);
            if (wk->unk110 == FALSE) {
                GFL_SndSEPlay(SEQ_SE_SHDEMO_04);
                wk->unk10C = FALSE;
                wk->unk110 = TRUE;
            }
        }
        if (func_ov195_02208850(wk->effect)) {
            func_ov195_0220605c(wk->view);
        }
        if (func_ov195_02208860(wk->effect)) {
            func_ov195_02206064(wk->view);
            if (wk->unk114 == FALSE) {
                if (wk->cancelled == FALSE) {
                    wk->unk10C = TRUE;
                }
                wk->unk114 = TRUE;
            }
        }
        if (wk->unk10C) {
            if (wk->unk118 == FALSE && wk->wait >= 90) {
                if (func_ov195_02204f68(heapId, wk)) {
                    GFL_SndSEPlay(SEQ_SE_SHDEMO_02);
                    wk->unk118 = TRUE;
                }
            } else if (wk->wait == 530 || wk->wait == 585 || wk->wait == 640 || wk->wait == 695) {
                GFL_SndSEPlay(SEQ_SE_SHDEMO_03);
            } else if (wk->wait == 740) {
                GFL_SndSEPlay(SEQ_SE_SHDEMO_05);
            }
            wk->wait++;
        }
        if (demoParam->canCancel && func_ov195_02204f68(heapId, wk) && wk->cancelled == FALSE &&
            (keys & PAD_BUTTON_B) && func_ov195_02206078(wk->view)) {
            wk->cancelled = TRUE;
            func_020355d8(FALSE);
            func_ov195_02208814(wk->effect);
        }
        break;
    case SHINKA_DEMO_EVOLVED:
        setChangedPkmSpecies(wk->pkm, demoParam->species);
        if (demoParam->gameData != NULL) {
            pokedex = GameData_GetPokedex(demoParam->gameData);
            PokeDex_RegistPkm(pokedex, wk->pkm);
            addPkmToDex(pokedex, wk->pkm);
        }
        func_ov195_02204fa0(heapId, wk);
        if (wk->windowShown) {
            wk->state = SHINKA_DEMO_PRINT_EVOLVED;
        } else {
            func_ov195_022054e8(heapId, wk, TRUE);
            wk->state = SHINKA_DEMO_SHOW_WINDOW_EVOLVED;
        }
        break;
    case SHINKA_DEMO_SHOW_WINDOW_EVOLVED:
        wk->bg1Request = BG1_SHOW;
        wk->state = SHINKA_DEMO_PRINT_EVOLVED;
        break;
    case SHINKA_DEMO_PRINT_EVOLVED:
        func_ov195_02205384(wk, wk->msgData, 2, NULL, WORD_STRBUF, wk->nickname, WORD_SPECIES, wk->pkm);
        wk->state = SHINKA_DEMO_WAIT_EVOLVED;
        break;
    case SHINKA_DEMO_WAIT_EVOLVED:
        if (func_ov195_022053e8(wk) && func_ov195_02204fc4(heapId, wk) == FALSE && func_ov195_0220546c(wk)) {
            func_020272a4(PokeParty_GetParam(wk->pkm, PKM_PARAM_SPECIES, NULL), wk->nickname);
            records = GameData_GetRecords(demoParam->gameData);
            RecordAddOne(records, 10);
            RecordAddOne(records, 88);
            wk->state = SHINKA_DEMO_AFTER_EVOLVING;
        }
        break;
    case SHINKA_DEMO_CANCELLED:
        if (wk->cancelled) {
            func_ov195_02204ff8(heapId, wk);
        } else {
            func_ov195_02204f40(heapId, wk);
        }
        if (wk->windowShown) {
            wk->state = SHINKA_DEMO_PRINT_CANCELLED;
        } else {
            func_ov195_022054e8(heapId, wk, TRUE);
            wk->state = SHINKA_DEMO_SHOW_WINDOW_CANCELLED;
        }
        break;
    case SHINKA_DEMO_SHOW_WINDOW_CANCELLED:
        wk->bg1Request = BG1_SHOW;
        wk->state = SHINKA_DEMO_PRINT_CANCELLED;
        break;
    case SHINKA_DEMO_PRINT_CANCELLED:
        func_ov195_02205384(wk, wk->msgData, 1, NULL, WORD_NICKNAME, wk->pkm, WORD_NONE, NULL);
        wk->state = SHINKA_DEMO_WAIT_CANCELLED;
        break;
    case SHINKA_DEMO_WAIT_CANCELLED:
        if (func_ov195_022053e8(wk) && func_ov195_0220546c(wk)) {
            wk->state = SHINKA_DEMO_END;
        }
        break;
    case SHINKA_DEMO_AFTER_EVOLVING:
        switch (demoParam->method) {
        case EVO_METHOD_SHEDINJA:
            func_ov195_022056e8(heapId, wk);
            break;
        case EVO_METHOD_TRADE_WITH_ITEM:
        case EVO_METHOD_HELD_ITEM_DAY:
        case EVO_METHOD_HELD_ITEM_NIGHT:
            func_ov195_02205844(heapId, wk);
            break;
        }
        wk->state = SHINKA_DEMO_LEARN_MOVE;
        wk->learnIndex = 0;
        break;
    case SHINKA_DEMO_LEARN_MOVE:
        wk->move = func_02018328(wk->pkm, &wk->learnIndex, wk->heapId);
        if (wk->move == 0) {
            wk->state = SHINKA_DEMO_END;
        } else if (wk->move != 0xfffe) {
            if (wk->move & 0x8000) {
                wk->move &= 0x7fff;
                wk->state = SHINKA_DEMO_PRINT_WANTS_MOVE;
            } else {
                wk->state = SHINKA_DEMO_WAIT_LEARNED;
                func_ov195_02205384(wk, wk->moveMsgData, 3, NULL, WORD_NICKNAME, wk->pkm, WORD_MOVE, &wk->move);
                func_ov195_02204fd4(heapId, wk);
            }
        }
        break;
    case SHINKA_DEMO_WAIT_LEARNED:
        if (func_ov195_022053e8(wk) && func_ov195_0220546c(wk)) {
            wk->state = SHINKA_DEMO_LEARN_MOVE;
        }
        break;
    case SHINKA_DEMO_PRINT_WANTS_MOVE:
        wk->state = SHINKA_DEMO_WAIT_WANTS_MOVE;
        func_ov195_02205384(wk, wk->moveMsgData, 4, NULL, WORD_NICKNAME, wk->pkm, WORD_MOVE, &wk->move);
        break;
    case SHINKA_DEMO_WAIT_WANTS_MOVE:
        if (func_ov195_022053e8(wk)) {
            wk->state = SHINKA_DEMO_WAIT_FORGET_ANSWER;
            func_ov195_022055bc(wk, wk->menuMsgData, 2, WORD_NONE, NULL, WORD_NONE, NULL, wk->menuMsgData, 3, WORD_NONE,
                                NULL, WORD_NONE, NULL);
            func_ov067_021b70f4(wk->menu, wk->choices[0], wk->choices[1]);
        }
        break;
    case SHINKA_DEMO_WAIT_FORGET_ANSWER:
        result = func_ov067_021b76b0(wk->menu);
        if (result == TWO_CHOICE_MENU_NONE) {
            break;
        }
        if (result == TWO_CHOICE_MENU_FIRST) {
            if (demoParam->unkC) {
                wk->state = SHINKA_DEMO_WAIT_FADE_OV207;
                GFL_FadeSet(3, 0, 16, 0);
            } else {
                wk->state = SHINKA_DEMO_WAIT_FADE_OV287;
                GFL_FadeSet(2, 0, 16, 0);
            }
        } else if (result == TWO_CHOICE_MENU_SECOND) {
            wk->state = SHINKA_DEMO_GIVE_UP;
        }
        func_ov067_021b72dc(wk->menu);
        break;
    case SHINKA_DEMO_WAIT_FADE_OV207:
        if (GFL_FadeIsRunning() == FALSE) {
            func_ov195_02204d5c(heapId, wk);
            wk->state = SHINKA_DEMO_OV207;
            save = GameData_GetSaveControl(demoParam->gameData);
            wk->pstatusParam->dataType = PSTATUS_DATA_PARTY;
            wk->pstatusParam->gameData = demoParam->gameData;
            wk->pstatusParam->forceExit = FALSE;
            wk->pstatusParam->party = demoParam->party;
            wk->pstatusParam->trainerData = getTrainerDataBlkAddress(save);
            wk->pstatusParam->partyCount = PokeParty_GetPkmCount(demoParam->party);
            wk->pstatusParam->partyIndex = demoParam->partyIndex;
            wk->pstatusParam->move = wk->move;
            wk->pstatusParam->mode = PSTATUS_MODE_FORGET_MOVE;
            wk->pstatusParam->page = PSTATUS_PAGE_SKILL;
            wk->pstatusParam->fromFieldMenu = FALSE;
            GFL_OvlLoad(OVERLAY_PSTATUS);
            QueueGameProc(wk->procManager, OVERLAY_NONE, &data_ov131_021dc800, wk->pstatusParam);
        }
        break;
    case SHINKA_DEMO_OV207:
        if (procRunning != TRUE) {
            wk->state = SHINKA_DEMO_OV207_END;
            break;
        }
        return FALSE;
    case SHINKA_DEMO_OV207_END:
        GFL_OvlUnload(OVERLAY_PSTATUS);
        wk->state = SHINKA_DEMO_WAIT_FADE_OV207_END;
        func_ov195_02204c24(heapId, wk);
        func_0203d34c(FALSE, wk->heapId);
        GFL_FadeSet(3, 16, 0, 0);
        break;
    case SHINKA_DEMO_WAIT_FADE_OV207_END:
        if (GFL_FadeIsRunning() == FALSE) {
            switch (wk->pstatusParam->result) {
            case 0:
                wk->slot = wk->pstatusParam->slot;
                wk->forgetMove = PokeParty_GetParam(wk->pkm, PKM_PARAM_MOVE1 + wk->slot, NULL);
                wk->state = SHINKA_DEMO_FORGET;
                break;
            case 1:
            case 2:
            default:
                wk->state = SHINKA_DEMO_GIVE_UP;
                break;
            }
        }
        break;
    case SHINKA_DEMO_WAIT_FADE_OV287:
        if (GFL_FadeIsRunning() == FALSE) {
            wk->state = SHINKA_DEMO_OV287;
            func_ov067_021b709c(wk->menu);
            wk->menu = NULL;
            func_ov195_02205a84(wk->graphic);
            wk->usingKeys = func_020355c4() == FALSE;
            wk->tcbBuffer = GFL_HeapAllocate(wk->heapId, GFL_TCBMgrCalcAllocSize(8), FALSE, "shinka_demo.c", 1352);
            MI_CpuFill8(wk->tcbBuffer, 0, GFL_TCBMgrCalcAllocSize(8));
            wk->tcbManager = GFL_TCBMgrCreate(8, wk->tcbBuffer);
            wk->paletteFade = func_02021518(wk->heapId);
            func_02021ee4(wk->paletteFade, TRUE);
            func_0202155c(wk->paletteFade, PALFADE_BUFFER_SUB_BG, 0x1e0, wk->heapId);
            func_0202155c(wk->paletteFade, PALFADE_BUFFER_SUB_OBJ, 0x1e0, wk->heapId);
            wk->ov287Param.gameData = demoParam->gameData;
            wk->ov287Param.party = demoParam->party;
            wk->ov287Param.font = wk->font;
            wk->ov287Param.heapId = wk->heapId;
            wk->ov287Param.unk1F = 4;
            wk->ov287Param.done = FALSE;
            wk->ov287Param.partyIndex = demoParam->partyIndex;
            wk->ov287Param.move = wk->move;
            wk->ov287Param.unk14 = 0;
            wk->ov287Param.usingKeys = &wk->usingKeys;
            wk->ov287Param.tcbManager = wk->tcbManager;
            wk->ov287Param.paletteFade = wk->paletteFade;
            wk->ov287Param.unk40 = 1;
            GFL_OvlLoad(OVERLAY_ID(196));
            GFL_OvlLoad(OVERLAY_ID(198));
            func_ov198_0220f3e8(&wk->ov287Param);
            GFL_FadeSet(2, 16, 0, 0);
        }
        break;
    case SHINKA_DEMO_OV287:
        GFL_TCBMgrUpdate(wk->tcbManager);
        if (wk->ov287Param.done) {
            wk->state = SHINKA_DEMO_WAIT_FADE_OV287_END;
            GFL_FadeSet(2, 0, 16, 0);
        }
        break;
    case SHINKA_DEMO_WAIT_FADE_OV287_END:
        if (GFL_FadeIsRunning() == FALSE) {
            wk->state = SHINKA_DEMO_OV287_END;
            GFL_OvlUnload(OVERLAY_ID(196));
            GFL_OvlUnload(OVERLAY_ID(198));
            func_020215a0(wk->paletteFade, PALFADE_BUFFER_SUB_BG);
            func_020215a0(wk->paletteFade, PALFADE_BUFFER_SUB_OBJ);
            func_02021540(wk->paletteFade);
            func_02030da4(wk->tcbManager);
            GFL_HeapFree(wk->tcbBuffer);
            wk->tcbManager = NULL;
            wk->tcbBuffer = NULL;
            wk->paletteFade = NULL;
            func_ov195_02205a34(wk->graphic);
            wk->menu = func_ov067_021b6dc0(wk->heapId, 5, 0, 1, 0, func_ov195_02205a28(wk->graphic), wk->font,
                                           wk->printQueue, 0);
            func_0203d34c(FALSE, wk->heapId);
            GFL_FadeSet(2, 16, 0, 0);
        }
        break;
    case SHINKA_DEMO_OV287_END:
        if (GFL_FadeIsRunning() == FALSE) {
            wk->slot = wk->ov287Param.slot;
            if (wk->slot == 4) {
                wk->state = SHINKA_DEMO_GIVE_UP;
            } else {
                wk->forgetMove = PokeParty_GetParam(wk->pkm, PKM_PARAM_MOVE1 + wk->slot, NULL);
                wk->state = SHINKA_DEMO_FORGET;
            }
            func_020355d8(wk->usingKeys ? FALSE : TRUE);
        }
        break;
    case SHINKA_DEMO_FORGET:
        if (wk->windowShown) {
            wk->state = SHINKA_DEMO_PRINT_FORGOT;
        } else {
            wk->state = SHINKA_DEMO_SHOW_WINDOW_FORGET;
            func_ov195_022054e8(heapId, wk, TRUE);
        }
        break;
    case SHINKA_DEMO_SHOW_WINDOW_FORGET:
        wk->bg1Request = BG1_SHOW;
        wk->state = SHINKA_DEMO_PRINT_FORGOT;
        break;
    case SHINKA_DEMO_PRINT_FORGOT:
        func_ov195_02205384(wk, wk->moveMsgData, 5, func_ov195_02205360, WORD_NICKNAME, wk->pkm, WORD_MOVE,
                            &wk->forgetMove);
        wk->state = SHINKA_DEMO_WAIT_FORGOT;
        break;
    case SHINKA_DEMO_WAIT_FORGOT:
        if (func_ov195_022053e8(wk)) {
            PokeParty_SetMove(wk->pkm, wk->move, wk->slot);
            wk->state = SHINKA_DEMO_WAIT_LEARNED_NEW;
            func_ov195_02205384(wk, wk->moveMsgData, 6, NULL, WORD_NICKNAME, wk->pkm, WORD_MOVE, &wk->move);
            func_ov195_02204fd4(heapId, wk);
        }
        break;
    case SHINKA_DEMO_WAIT_LEARNED_NEW:
        if (func_ov195_022053e8(wk)) {
            wk->state = SHINKA_DEMO_LEARN_MOVE;
        }
        break;
    case SHINKA_DEMO_GIVE_UP:
        if (wk->windowShown) {
            wk->state = SHINKA_DEMO_PRINT_GIVE_UP;
        } else {
            wk->state = SHINKA_DEMO_SHOW_WINDOW_GIVE_UP;
            func_ov195_022054e8(heapId, wk, TRUE);
        }
        break;
    case SHINKA_DEMO_SHOW_WINDOW_GIVE_UP:
        wk->bg1Request = BG1_SHOW;
        wk->state = SHINKA_DEMO_PRINT_GIVE_UP;
        break;
    case SHINKA_DEMO_PRINT_GIVE_UP:
        func_ov195_02205384(wk, wk->moveMsgData, 7, NULL, WORD_NONE, NULL, WORD_MOVE, &wk->move);
        wk->state = SHINKA_DEMO_WAIT_GIVE_UP;
        break;
    case SHINKA_DEMO_WAIT_GIVE_UP:
        if (func_ov195_022053e8(wk)) {
            wk->state = SHINKA_DEMO_WAIT_GIVE_UP_ANSWER;
            func_ov195_022055bc(wk, wk->menuMsgData, 4, WORD_MOVE, &wk->move, WORD_NONE, NULL, wk->menuMsgData, 5,
                                WORD_MOVE, &wk->move, WORD_NONE, NULL);
            func_ov067_021b70f4(wk->menu, wk->choices[0], wk->choices[1]);
        }
        break;
    case SHINKA_DEMO_WAIT_GIVE_UP_ANSWER:
        result = func_ov067_021b76b0(wk->menu);
        if (result == TWO_CHOICE_MENU_NONE) {
            break;
        }
        if (result == TWO_CHOICE_MENU_FIRST) {
            wk->state = SHINKA_DEMO_WAIT_NOT_LEARNED;
            func_ov195_02205384(wk, wk->moveMsgData, 8, NULL, WORD_NICKNAME, wk->pkm, WORD_MOVE, &wk->move);
        } else if (result == TWO_CHOICE_MENU_SECOND) {
            wk->state = SHINKA_DEMO_PRINT_WANTS_MOVE;
        }
        func_ov067_021b72dc(wk->menu);
        break;
    case SHINKA_DEMO_WAIT_NOT_LEARNED:
        if (func_ov195_022053e8(wk)) {
            wk->state = SHINKA_DEMO_LEARN_MOVE;
        }
        break;
    case SHINKA_DEMO_END:
        if (wk->cancelled) {
            GFL_FadeSet(3, 0, 16, 2);
            wk->state = SHINKA_DEMO_WAIT_BGM_END;
        } else if (func_ov195_02204f68(heapId, wk)) {
            func_ov195_02204f78(heapId, wk);
            GFL_FadeSet(3, 0, 16, 2);
            wk->state = SHINKA_DEMO_WAIT_BGM_END;
        }
        break;
    case SHINKA_DEMO_WAIT_BGM_END:
        if (func_ov195_02204f90(heapId, wk) == FALSE) {
            wk->state = SHINKA_DEMO_WAIT_FADE_END;
            func_ov195_02204eb0(heapId, wk);
        }
        break;
    case SHINKA_DEMO_WAIT_FADE_END:
        if (GFL_FadeIsRunning() == FALSE) {
            wk->state = SHINKA_DEMO_DONE;
            return TRUE;
        }
        break;
    case SHINKA_DEMO_DONE:
        return TRUE;
    }
    func_ov195_02204de0(heapId, wk);
    if (wk->state != SHINKA_DEMO_OV207 && wk->state != SHINKA_DEMO_OV207_END) {
        if (wk->menu != NULL) {
            func_ov067_021b7378(wk->menu);
        }
        func_ov195_02205340(heapId, wk);
        func_0201c4e4(wk->printQueue);
        func_ov195_02205d50(wk->view);
        func_ov195_0220878c(wk->effect);
        func_ov195_022059fc(wk->graphic);
        func_ov195_02205a10(wk->graphic);
        func_ov195_02208804(wk->effect);
        func_ov195_0220602c(wk->view);
        func_ov195_02205a1c(wk->graphic);
    }
    return FALSE;
}


// pokebw2's ShinkaDemo_InitGraphics
static void func_ov195_02204c24(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    u8 i;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_SHINKA_DEMO_GRAPHIC, 0x70000);
    wk->graphicHeapId = HEAPID_SHINKA_DEMO_GRAPHIC;
    GFL_OvlLoad(OVERLAY_APP_UI);
    for (i = 0; i <= 7; i++) {
        GFL_BGSysSetBGEnabled(i, FALSE);
    }
    wk->graphic = func_ov195_022058f8(1, wk->graphicHeapId);
    func_ov195_02205a34(wk->graphic);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, wk->graphicHeapId);
    wk->printQueue = func_0201c440(wk->graphicHeapId);
    func_ov195_022051fc(heapId, wk);
    wk->menu = NULL;
    func_ov195_02205530(heapId, wk);
    func_ov195_02205010(heapId, wk);
    wk->actorsCreated = FALSE;
    if (wk->state == SHINKA_DEMO_WAIT) {
        func_ov195_02205070(heapId, wk);
    }
    GFL_BGSysSetBGPriority(0, 2);
    GFL_BGSysSetBGPriority(2, 1);
    GFL_BGSysSetBGPriority(1, 0);
    GFL_BGSysSetBGPriority(4, 2);
    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_BGSysSetBGEnabled(2, TRUE);
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysResetStdPalette(0, GX_RGB(0, 0, 0));
    GFL_BGSysResetStdPalette(4, GX_RGB(0, 0, 0));
    if (wk->state == SHINKA_DEMO_WAIT) {
        if (heapId->method == EVO_METHOD_ITEM || heapId->method == EVO_METHOD_ITEM_MALE ||
            heapId->method == EVO_METHOD_ITEM_FEMALE) {
            heapId->canCancel = FALSE;
        }
    }
    if (wk->state == SHINKA_DEMO_WAIT) {
        wk->view = func_ov195_02205c74(wk->graphicHeapId, FALSE, wk->pkm, heapId->species);
    } else {
        wk->view = func_ov195_02205c74(wk->graphicHeapId, TRUE, wk->pkm, heapId->species);
    }
    if (wk->state == SHINKA_DEMO_WAIT) {
        wk->effect = func_ov195_0220871c(wk->graphicHeapId, FALSE);
    } else {
        wk->effect = func_ov195_0220871c(wk->graphicHeapId, TRUE);
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0,
                     GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ |
                         GX_BLEND_PLANEMASK_BD,
                     0, 0);
}

// pokebw2's ShinkaDemo_FreeGraphics
static void func_ov195_02204d5c(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    func_ov195_02208770(wk->effect);
    func_ov195_02205d28(wk->view);
    func_ov195_02205100(heapId, wk);
    func_ov195_0220506c(heapId, wk);
    func_ov195_02205580(heapId, wk);
    func_ov195_022052e8(heapId, wk);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov195_02205a84(wk->graphic);
    func_ov195_022059a0(wk->graphic);
    GFL_OvlUnload(OVERLAY_APP_UI);
    GFL_HeapDelete(0x68);
}

// Fades out the music that was playing, to push it once it is quiet
// pokebw2's ShinkaDemo_InitBGM
static void func_ov195_02204dc4(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (heapId->unkC) {
        GFL_SndBGMFadeOut(30);
    }
    wk->bgm = BGM_FADE_OUT;
}

// pokebw2's ShinkaDemo_FreeBGM
static void func_ov195_02204ddc(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
}

// pokebw2's ShinkaDemo_UpdateBGM
void func_ov195_02204de0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    switch (wk->bgm) {
    case BGM_FADE_OUT:
        if (GFL_SndBGMIsFading() == FALSE) {
            if (heapId->unkC) {
                GFL_SndBGMSetPaused(TRUE);
                GFL_SndBGMPush();
            }
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
    case BGM_FADE_OUT_END:
        if (GFL_SndBGMIsFading() == FALSE) {
            func_02005d28();
            wk->bgm = BGM_IDLE;
        }
        break;
    case BGM_LOAD_FANFARE:
        if (func_020063b4(SEQ_ME_SHINKAOME, &wk->bgmStep, FALSE)) {
            wk->bgm = BGM_FANFARE;
        }
        break;
    case BGM_FANFARE:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            func_ov195_02204f40(heapId, wk);
        }
        break;
    case BGM_LEVEL_UP:
        if (GFL_SndBGMIsPlaying() == FALSE) {
            func_ov195_02204f40(heapId, wk);
        }
        break;
    }
}

// pokebw2's ShinkaDemo_IsBGMFadingOut
BOOL func_ov195_02204ea0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    return wk->bgm == BGM_FADE_OUT;
}

// Brings back the music that was playing
// pokebw2's ShinkaDemo_RestoreBGM
void func_ov195_02204eb0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_IDLE && heapId->unkC) {
        GFL_SndBGMPop();
        GFL_SndBGMSetPaused(FALSE);
        GFL_SndBGMFadeIn(60);
    }
}

// pokebw2's ShinkaDemo_PlayBGM
void func_ov195_02204ed0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_IDLE) {
        GFL_SndBGMPlay(SEQ_BGM_SHINKA, SND_CHANNEL_MASK_ALL);
        wk->bgm = BGM_PLAY;
    }
}

// pokebw2's ShinkaDemo_IsBGMPlaying
BOOL func_ov195_02204ef0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    return wk->bgm == BGM_PLAY;
}

// pokebw2's ShinkaDemo_PlayKoukan
void func_ov195_02204f00(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_IDLE) {
        wk->bgmStep = 0;
        func_020063b4(SEQ_BGM_KOUKAN, &wk->bgmStep, TRUE);
        wk->bgm = BGM_LOAD_KOUKAN;
    }
}

// pokebw2's ShinkaDemo_PushBGM
void func_ov195_02204f24(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_KOUKAN) {
        GFL_SndBGMSetPaused(TRUE);
        GFL_SndBGMPush();
        wk->bgm = BGM_PUSHED;
    }
}

// pokebw2's ShinkaDemo_PopBGM
static void func_ov195_02204f40(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_PUSHED || wk->bgm == BGM_FANFARE || wk->bgm == BGM_LEVEL_UP) {
        GFL_SndBGMPop();
        GFL_SndBGMSetPaused(FALSE);
        GFL_SndBGMFadeIn(6);
        wk->bgm = BGM_KOUKAN;
    }
}

// pokebw2's ShinkaDemo_IsKoukanPlaying
BOOL func_ov195_02204f68(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    return wk->bgm == BGM_KOUKAN;
}

// pokebw2's ShinkaDemo_FadeOutBGM
void func_ov195_02204f78(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_KOUKAN) {
        GFL_SndBGMFadeOut(60);
        wk->bgm = BGM_FADE_OUT_END;
    }
}

// pokebw2's ShinkaDemo_IsBGMFadingOutEnd
BOOL func_ov195_02204f90(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    return wk->bgm == BGM_FADE_OUT_END;
}

// pokebw2's ShinkaDemo_PlayFanfare
void func_ov195_02204fa0(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_PUSHED) {
        wk->bgmStep = 0;
        func_020063b4(SEQ_ME_SHINKAOME, &wk->bgmStep, TRUE);
        wk->bgm = BGM_LOAD_FANFARE;
    }
}

// pokebw2's ShinkaDemo_IsFanfarePlaying
BOOL func_ov195_02204fc4(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    return wk->bgm == BGM_LOAD_FANFARE || wk->bgm == BGM_FANFARE;
}

// pokebw2's ShinkaDemo_PlayLevelUp
void func_ov195_02204fd4(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_KOUKAN) {
        func_ov195_02204f24(heapId, wk);
        GFL_SndBGMPlay(SEQ_ME_LVUP, SND_CHANNEL_MASK_ALL);
        wk->bgm = BGM_LEVEL_UP;
    }
}

// Drops the pushed music, when the evolution has been stopped
// pokebw2's ShinkaDemo_DiscardBGM
void func_ov195_02204ff8(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    if (wk->bgm == BGM_PUSHED) {
        GFL_SndBGMPop();
        func_02005d28();
        wk->bgm = BGM_IDLE;
    }
}

// pokebw2's ShinkaDemo_InitBG
static void func_ov195_02205010(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_SHINKA_DEMO, wk->graphicHeapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 3, 0, 0, 0x20, wk->graphicHeapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 5, 2, 0, 0, FALSE, wk->graphicHeapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 2, 0, 0, FALSE, wk->graphicHeapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(2);
}

// pokebw2's ShinkaDemo_FreeBG
static void func_ov195_0220506c(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
}

// pokebw2's ShinkaDemo_InitActors
static void func_ov195_02205070(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_SHINKA_DEMO, wk->graphicHeapId);
    u8 i;

    wk->palette = func_0204ac18(arc, 3, 0, 0, 0, 1, wk->graphicHeapId);
    wk->chars = func_0204a6c8(arc, 2, 0, 0, wk->graphicHeapId);
    wk->cellAnims = func_0204af28(arc, 1, 0, wk->graphicHeapId);
    GFL_ArcToolFree(arc);
    for (i = 0; i < ACTOR_COUNT; i++) {
        wk->actors[i] = func_0204b294(func_ov195_02205a28(wk->graphic), wk->chars, wk->palette,
                                      wk->cellAnims, &data_ov195_02208c4c[i], 0, wk->graphicHeapId);
        func_0204b92c(wk->actors[i], FALSE);
    }
    wk->actorsCreated = TRUE;
}

// pokebw2's ShinkaDemo_FreeActors
static void func_ov195_02205100(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    u8 i;

    if (wk->actorsCreated) {
        for (i = 0; i < ACTOR_COUNT; i++) {
            func_0204b3b4(wk->actors[i]);
        }
        func_0204afd8(wk->cellAnims);
        func_0204a8d4(wk->chars);
        func_0204ada4(wk->palette);
        wk->actorsCreated = FALSE;
    }
}

// pokebw2's ShinkaDemo_StartActors
void func_ov195_02205138(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    u8 i;

    if (wk->actorsCreated) {
        for (i = 0; i < ACTOR_COUNT; i++) {
            func_0204b908(wk->actors[i], 0);
            func_0204b92c(wk->actors[i], TRUE);
        }
    }
}

// Frees the actors once they are done
// pokebw2's ShinkaDemo_AreActorsDone
BOOL func_ov195_02205168(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    BOOL done;
    u8 i;

    if (wk->actorsCreated) {
        done = TRUE;
        for (i = 0; i < ACTOR_COUNT; i++) {
            if (func_0204b98c(wk->actors[i])) {
                done = FALSE;
                break;
            }
        }
        if (done) {
            func_ov195_02205100(heapId, wk);
        }
        return done;
    }
    return TRUE;
}

// pokebw2's ShinkaDemo_VBlank
static void func_ov195_022051a8(TCB *tcb, void *data) {
    ShinkaDemoWork *wk = data;

    switch (wk->bg1Request) {
    case BG1_SHOW:
        GFL_BGSysSetBGEnabled(1, TRUE);
        wk->windowShown = TRUE;
        wk->bg1Request = BG1_NONE;
        break;
    case BG1_HIDE:
        GFL_BGSysSetBGEnabled(1, FALSE);
        wk->windowShown = FALSE;
        wk->bg1Request = BG1_NONE;
        break;
    }
    if (wk->paletteFade != NULL) {
        func_02021d50(wk->paletteFade);
    }
}

// pokebw2's ShinkaDemo_InitMsg
static void func_ov195_022051fc(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    BmpWin *window;

    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x20, 0x20, wk->graphicHeapId);
    wk->blankWindow = BmpWin_CreateDynamic(1, 0, 0, 1, 1, 1, 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->blankWindow), 0);
    BmpWin_FlushChar(wk->blankWindow);
    wk->window = BmpWin_CreateDynamic(1, 1, 19, 30, 4, 1, 1);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->window), 0);
    window = wk->window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    wk->frameChars = LoadCursorImageEndOfHeap(1, 2, 0, wk->graphicHeapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_SHINKA_DEMO, wk->graphicHeapId);
    wk->moveMsgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_SHINKA_DEMO_BTL_MAIN, wk->graphicHeapId);
    wk->tcbExManager = GFL_TCBExMgrCreate(wk->graphicHeapId, wk->graphicHeapId, 1, 0);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    wk->message = NULL;
    wk->printStream = NULL;
    wk->keyCursor = func_02028590(15, TRUE, TRUE, wk->graphicHeapId);
    wk->bg1Request = BG1_NONE;
    GFL_BGSysSetBGEnabled(1, FALSE);
    wk->windowShown = FALSE;
}

// pokebw2's ShinkaDemo_FreeMsg
static void func_ov195_022052e8(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    func_02028604(wk->keyCursor);
    if (wk->printStream != NULL) {
        func_0201ce48(wk->printStream);
    }
    GFL_TCBExMgrFree(wk->tcbExManager);
    if (wk->message != NULL) {
        GFL_StrBufFree(wk->message);
    }
    GFL_MsgDataFree(wk->msgData);
    GFL_MsgDataFree(wk->moveMsgData);
    GFL_BGSysFreeCharMemory(1, CHAR_POS(wk->frameChars), CHAR_SIZE(wk->frameChars));
    BmpWin_Free(wk->window);
    BmpWin_Free(wk->blankWindow);
}

// pokebw2's ShinkaDemo_UpdateMsg
void func_ov195_02205340(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    GFL_TCBExMgrUpdate(wk->tcbExManager);
    if (wk->printStream != NULL) {
        func_020286c4(wk->keyCursor, wk->printStream, wk->window);
    }
}

// Plays the sounds of the message that the Pokémon forgot a move
// pokebw2's ShinkaDemo_OnPrintEvent
BOOL func_ov195_02205360(u32 event) {
    switch (event) {
    case 3:
        GFL_SndSEPlay(SEQ_SE_KON);
        break;
    case 5:
        return GFL_SndPlayerIsActiveAny();
    }
    return FALSE;
}

// Prints a message with up to two words, WORD_*
void func_ov195_02205384(ShinkaDemoWork *wk, MsgData *msgData, u32 messageId, PrintStreamCallback callback,
                         u32 type0, void *word0, u32 type1, void *word1) {
    func_ov195_0220549c(wk, 15);
    wk->message = func_ov195_02205620(wk->graphicHeapId, msgData, messageId, type0, word0, type1, word1);
    wk->printStream = func_0201cd10(wk->window, 0, 0, wk->message, wk->font, func_02012ffc(), wk->tcbExManager, 2,
                                    wk->graphicHeapId, 15, callback);
}

// pokebw2's ShinkaDemo_IsPrintDone
BOOL func_ov195_022053e8(ShinkaDemoWork *wk) {
    BOOL done = FALSE;

    switch (func_0201ce30(wk->printStream)) {
    case PRINT_STREAM_RUNNING:
        if ((GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c38()) {
            func_0201ce5c(wk->printStream, 0);
            if (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                func_020355d8(FALSE);
            } else {
                func_020355d8(TRUE);
            }
        }
        break;
    case PRINT_STREAM_PAUSED:
        if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            func_0201ce38(wk->printStream);
            if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                func_020355d8(FALSE);
            } else {
                func_020355d8(TRUE);
            }
        }
        break;
    case PRINT_STREAM_DONE:
        done = TRUE;
        break;
    }
    return done;
}

// Whether A or B was pressed or the touch screen touched
// pokebw2's ShinkaDemo_WaitButton
BOOL func_ov195_0220546c(ShinkaDemoWork *wk) {
    if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            func_020355d8(FALSE);
        } else {
            func_020355d8(TRUE);
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ShinkaDemo_ClearWindow
static void func_ov195_0220549c(ShinkaDemoWork *wk, u8 color) {
    BmpWin *window;

    GFL_BitmapFill(BmpWin_GetBitmap(wk->window), color);
    window = wk->window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    if (wk->printStream != NULL) {
        func_0201ce48(wk->printStream);
    }
    if (wk->message != NULL) {
        GFL_StrBufFree(wk->message);
    }
    wk->printStream = NULL;
    wk->message = NULL;
}

// Draws the message window, or hides it at the next VBlank
// pokebw2's ShinkaDemo_ShowWindow
void func_ov195_022054e8(ShinkaDemoParam *heapId, ShinkaDemoWork *wk, BOOL show) {
    BmpWin *window;

    if (show) {
        BmpWin_DrawFrame(wk->window, WINFRAME_TRANSFER_NOW, wk->frameChars, 2);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->window), 15);
        window = wk->window;
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    } else {
        wk->bg1Request = BG1_HIDE;
    }
}

void func_ov195_02205530(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    wk->menuMsgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_YES_NO, wk->graphicHeapId);
    wk->menu = func_ov067_021b6dc0(wk->graphicHeapId, 5, 0, 1, 0, func_ov195_02205a28(wk->graphic), wk->font, wk->printQueue, 0);
    wk->choices[0] = NULL;
    wk->choices[1] = NULL;
}


// pokebw2's ShinkaDemo_FreeMenu
static void func_ov195_02205580(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    func_ov067_021b709c(wk->menu);
    wk->menu = NULL;
    if (wk->choices[0] != NULL) {
        GFL_StrBufFree(wk->choices[0]);
    }
    if (wk->choices[1] != NULL) {
        GFL_StrBufFree(wk->choices[1]);
    }
    GFL_MsgDataFree(wk->menuMsgData);
}

void func_ov195_022055bc(ShinkaDemoWork *wk, MsgData *msgData0, u32 messageId0, u32 type00, void *word00,
                          u32 type10, void *word10, MsgData *msgData1, u32 messageId1, u32 type01,
                          void *word01, u32 type11, void *word11) {
    if (wk->choices[0] != NULL) {
        GFL_StrBufFree(wk->choices[0]);
    }
    if (wk->choices[1] != NULL) {
        GFL_StrBufFree(wk->choices[1]);
    }
    wk->choices[0] = func_ov195_02205620(wk->graphicHeapId, msgData0, messageId0, type00, word00, type10, word10);
    wk->choices[1] = func_ov195_02205620(wk->graphicHeapId, msgData1, messageId1, type01, word01, type11, word11);
}

static StrBuf *func_ov195_02205620(HeapID heapId, MsgData *msgData, u32 messageId, u32 type0, void *word0,
                                        u32 type1, void *word1) {
    StrBuf *message;
    StrBuf *format;
    WordSet *wordSet;
    MessageWord words[2];
    u32 i;

    if (type0 == WORD_NONE && type1 == WORD_NONE) {
        message = GFL_MsgDataLoadStrbufNew(msgData, messageId);
    } else {
        format = GFL_MsgDataLoadStrbufNew(msgData, messageId);
        wordSet = GFL_WordSetSystemCreateDefault(heapId);
        words[0].type = type0;
        words[0].value = word0;
        words[1].type = type1;
        words[1].value = word1;
        for (i = 0; i < NELEMS(words); i++) {
            switch (words[i].type) {
            case WORD_STRBUF:
                func_0201ee0c(wordSet, i, words[i].value, 0, 1, 0);
                break;
            case WORD_SPECIES:
                setPartyPokemonSpeciesNameToStrbuf(wordSet, i, words[i].value);
                break;
            case WORD_NICKNAME:
                loadPokemonNicknameToStrbuf(wordSet, i, words[i].value);
                break;
            case WORD_MOVE:
                loadMoveNameToStrbuf(wordSet, i, *(u16 *)words[i].value);
                break;
            }
        }
        message = GFL_StrBufCreate(256, heapId);
        GFL_WordSetFormatStrbuf(wordSet, message, format);
        GFL_StrBufFree(format);
        GFL_WordSetSystemFree(wordSet);
    }
    return message;
}

// Adds a Shedinja to the party, using up a Poké Ball, if there is room in the party and a Poké Ball in the bag
// pokebw2's ShinkaDemo_MakeShedinja
void func_ov195_022056e8(ShinkaDemoParam *param, ShinkaDemoWork *wk) {
    BagSave *bag;
    BOOL BVar3;
    u32 size;
    PartyPkm *dest;
    StrBuf *strbuf;
    MailData *ptr;
    PokeDexSave *pokedex;
    GameRecords *records;
    PkmField PVar5;

    bag = GameData_GetBag(param->gameData);
    if ((PokeParty_GetPkmCount(param->party) < PokeParty_GetCapacity(param->party)) &&
        (BVar3 = BagSave_CheckAmount(bag, 4, 1, wk->heapId), BVar3 != 0)) {
        size = PokeParty_GetPkmRawSize();
        dest = GFL_HeapAllocate(wk->heapId, size, 1, "shinka_demo.c", 0xaf5);
        copyPartyPkm(wk->pkm, dest);
        setChangedPkmSpecies(dest, 0x124);
        PokeParty_SetParam(dest, 0x98, 4);
        PokeParty_SetParam(dest, 6, 0);
        PokeParty_SetParam(dest, 0xb, 0);
        PVar5 = 0x19;
        do {
            PokeParty_SetParam(dest, PVar5, 0);
            PVar5 = PVar5 + 1;
        } while ((s32)PVar5 < 0x36);
        PVar5 = 0x4d;
        do {
            PokeParty_SetParam(dest, PVar5, 0);
            PVar5 = PVar5 + 1;
        } while ((s32)PVar5 < 0x6d);
        PVar5 = 0x78;
        do {
            PokeParty_SetParam(dest, PVar5, 0);
            PVar5 = PVar5 + 1;
        } while ((s32)PVar5 < 0x8d);
        size = PokeParty_GetParam(dest, 5, (void *)0x0);
        strbuf = GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, size);
        PokeParty_SetParam(dest, 0x73, (uintptr_t)strbuf);
        GFL_StrBufFree(strbuf);
        PokeParty_SetParam(dest, 0x9d, 0);
        ptr = CreateMailData(wk->heapId);
        PokeParty_SetParam(dest, 0xa7, (uintptr_t)ptr);
        GFL_HeapFree(ptr);
        PokeParty_SetParam(dest, 0x9f, 0);
        PokeParty_AddPkm(param->party, dest);
        BagSave_SubItem(bag, 4, 1, wk->heapId);
        GFL_HeapFree(dest);
        pokedex = GameData_GetPokedex(param->gameData);
        PokeDex_RegistPkm(pokedex, dest);
        addPkmToDex(pokedex, dest);
        records = GameData_GetRecords(param->gameData);
        RecordAddOne(records, 10);
        RecordAddOne(records, 0x58);
    }
}

// The held item that the Pokémon evolved with is used up
// pokebw2's ShinkaDemo_RemoveHeldItem
void func_ov195_02205844(ShinkaDemoParam *heapId, ShinkaDemoWork *wk) {
    PokeParty_SetParam(wk->pkm, PKM_PARAM_ITEM, ITEM_NONE);
}
