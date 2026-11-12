// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov306/btl_rec_sel.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/btl_rec_sel.h"
#include "app/btl_rec_sel_graphic.h"
#include "app/ui/ui_scene.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "save/bag.h"
#include "save/battle_rec.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/bmp_menu.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wordset.h"
#include "text/system/title.h"
#include "pml/poke_party.h"

// The screen that offers to keep the last battle as a Battle Video, named after the ROM's "btl_rec_sel.c".
// With a video saved already it shows that one's battle mode and parties before asking whether to overwrite it.
//
// The file ends before BtlRecSelGraphic_G3DInit: the three palette fade functions after func_ov214_021bae9c,
// which the survey gave to btl_rec_sel_graphic.c, use this file's work, and that file's siblings of the same
// template all start at the 3D system's callback

#define TEXT_WINDOW_COUNT 4
#define SIDE_COUNT 2
#define PARTY_SIZE 6

// The time to choose in when the other player is choosing too, in seconds
#define TIME_LIMIT 60
// How long the result of the save stays, in frames
#define RESULT_WAIT 120

// The palettes of BG1's windows, and that of the window frame
#define PALETTE_TEXT 9
#define PALETTE_FRAME 10
#define PALETTE_YESNO 11
#define PALETTE_MESSAGE 12

#define COLOR_TEXT PRINT_COLOR(1, 2, 0)
#define COLOR_TIME_LOW PRINT_COLOR(3, 4, 0)

// The steps of func_ov214_021b9834
enum {
    SEQ_FADE,
    SEQ_FADE_WAIT,
    SEQ_QUESTION,
    SEQ_QUESTION_WAIT,
    SEQ_MESSAGE,
    SEQ_SAVE_YESNO,
    SEQ_NO_SAVE_YESNO,
    SEQ_OVERWRITE_YESNO,
    SEQ_SHOW_PREVIOUS,
    SEQ_HIDE_PREVIOUS,
    SEQ_SAVE_MESSAGE,
    SEQ_SAVE_MESSAGE_WAIT,
    SEQ_SAVE,
    SEQ_SAVED_WAIT,
    SEQ_SAVE_FAILED_WAIT,
    SEQ_END_MESSAGE,
    SEQ_END_MESSAGE_WAIT,
    SEQ_END_SYNC,
    SEQ_END,
};

// What func_ov214_021baf64 is asked to do, and where it is
enum {
    FADE_REQ_NONE,
    FADE_REQ_IN,
    FADE_REQ_OUT,
};

enum {
    FADE_STATE_OUT,
    FADE_STATE_FADING_IN,
    FADE_STATE_IN,
    FADE_STATE_FADING_OUT,
};

// The text windows other than the message's: the title and the battle mode of the saved video, and the time limit
enum {
    TEXT_WINDOW_TITLE,
    TEXT_WINDOW_MODE,
    TEXT_WINDOW_TIME_LABEL,
    TEXT_WINDOW_TIME,
};

// A Pokémon of the saved video and its icon
typedef struct {
    u32 species;
    u32 form;
    u32 sex;
    BOOL egg;
    UIObjRes res;
    ClActor *actor;
    u8 x;
    u8 y;
    u8 anim;
    u8 priority;
} BtlRecSelIcon;

typedef struct {
    u16 heapId;
    BtlRecSelGraphic *graphic;
    Font *font;
    // The queues of the message window, the saved video's windows and the two windows of the time limit
    PrintQueue *messageQueue;
    PrintQueue *titleQueue;
    PrintQueue *timeLabelQueue;
    PrintQueue *timeQueue;
    // The fade SEQ_FADE starts and the step after it. With paletteFade the palettes fade instead of the brightness,
    // which leaves the sub screen as it is
    u32 fadeNextSeq;
    u32 fadeMode;
    s32 fadeStart;
    s32 fadeEnd;
    s32 fadeWait;
    BOOL paletteFade;
    // The question SEQ_QUESTION asks and the step that reads its answer
    u32 yesNoSeq;
    u32 questionMessage;
    // Whether stopping the time limit clears BG2
    BOOL clearBGOnStop;
    // The message SEQ_MESSAGE prints and the step after it
    u32 messageNextSeq;
    u32 message;
    // Where SEQ_HIDE_PREVIOUS goes on to
    u32 hideNextSeq;
    TCB *vblankTask;
    BOOL hasVsRecorder;
    // Set when there is nothing to show: no Vs. Recorder and no other player to wait for
    BOOL skip;
    MsgData *msgData;
    PrintStream *stream;
    // Whether the message waits for a button once it is printed
    BOOL waitKey;
    u32 waitTimer;
    TCBExManager *tcbExManager;
    BmpWin *messageWindow;
    BmpWin *blankWindow;
    u32 frameChars;
    StrBuf *messageStr;
    BOOL messagePrinted;
    ConfirmDialogSetup yesNoSetup;
    BmpMenu *yesNo;
    BmpWin *textWindows[TEXT_WINDOW_COUNT];
    BOOL textPrinted[TEXT_WINDOW_COUNT];
    // The time limit in frames, on top of a second that the last number stays for, and the frames left after it
    u32 timer;
    u32 timerEnd;
    BOOL timerStopped;
    BOOL timeUp;
    // The color and the seconds the time limit was last printed with
    u32 timeColor;
    u16 timeSeconds;
    BtlRecSelIcon icons[SIDE_COUNT][PARTY_SIZE];
    // The saved video's battle mode as a message, and whether each side is one Trainer's party
    u32 modeMessage;
    BOOL oneTrainerPerSide;
    BattleRecVideo *video;
    u32 mode;
    u32 unk2B4;
    u16 saveSeq;
    u16 saveSubSeq;
    u32 saveResult;
    u32 bgChars;
    // An icon that is loaded and freed again at the start
    UIObjRes dummyRes;
    ClActor *dummyActor;
    TCBManager *tcbManager;
    void *tcbManagerWork;
    PaletteFade *fade;
    u32 fadeRequest;
    u32 fadeState;
    WaitIcon *waitIcon;
} BtlRecSelWork;

typedef struct {
    u32 bg;
    BGSetup setup;
    u32 mode;
    u32 enabled;
} BtlRecSelBGSetup;

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 fromEnd;
} BtlRecSelWindowSetup;

// A battle mode's name, and whether two Trainers share each side
typedef struct {
    u8 message;
    u8 multi;
} BtlRecSelMode;

BOOL func_ov214_021b95a0(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov214_021b9798(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov214_021b9834(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov214_021b9eb8(u32 *state, BtlRecSelParam *param, BtlRecSelWork *work, u32 nextSeq, u32 mode, s32 start,
                              s32 end, s32 wait, BOOL paletteFade);
static void func_ov214_021b9ed4(u32 *state, BtlRecSelParam *param, BtlRecSelWork *work, u32 yesNoSeq, u32 message,
                                  BOOL clearBGOnStop);
static void func_ov214_021b9ef0(BtlRecSelParam *param, BtlRecSelWork *work, u32 yesNoSeq, u32 message,
                                  BOOL clearBGOnStop);
static void func_ov214_021b9efc(u32 *state, BtlRecSelParam *param, BtlRecSelWork *work, u32 nextSeq, u32 message);
static void func_ov214_021b9f10(BtlRecSelParam *param, BtlRecSelWork *work, u32 nextSeq, u32 message);
void func_ov214_021b9f18(TCB *tcb, void *data);
void func_ov214_021b9f24(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021ba018(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba058(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021ba074(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba0a4(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba0b0(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba0dc(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba108(BtlRecSelParam *param, BtlRecSelWork *work, u32 message, BOOL waitKey);
static BOOL func_ov214_021ba16c(BtlRecSelParam *param, BtlRecSelWork *work);
static BOOL func_ov214_021ba190(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba254(BtlRecSelParam *param, BtlRecSelWork *work, u32 message);
static BOOL func_ov214_021ba2b8(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021ba2f4(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021ba39c(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba3a0(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021ba3c4(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021ba43c(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba45c(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba4e8(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba5c8(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba650(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021ba6a0(BtlRecSelParam *param, BtlRecSelWork *work, u16 seconds);
static void func_ov214_021ba6f0(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021ba794(BtlRecSelParam *param, BtlRecSelWork *work, u16 seconds);
static void func_ov214_021ba8dc(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021baab0(BtlRecSelParam *param, BtlRecSelWork *work);
static ClActor *func_ov214_021bab0c(ClActUnit *unit, HeapID heapId, u32 vramType, u8 paletteOffset, u32 species,
                                     u32 form, u32 sex, BOOL egg, UIObjRes *res, u8 x, u8 y, u8 anim, u8 bgPriority,
                                     u8 priority);
static void func_ov214_021baba4(UIObjRes *res, ClActor *actor);
static void func_ov214_021babb8(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021babf4(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021bac3c(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021bac58(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021bac90(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021baccc(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021bad08(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021bad14(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021bae10(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021bae4c(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021bae88(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021bae9c(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021baf30(BtlRecSelParam *param, BtlRecSelWork *work);
static void func_ov214_021baf64(BtlRecSelParam *param, BtlRecSelWork *work);
void func_ov214_021bb0c4(BtlRecSelWork *work);

// pokebw2's BtlRecSelProcFunctions
// pokebw2's BtlRecSelProc_Exit
// pokebw2's BtlRecSelProc_Init
// pokebw2's BtlRecSelProc_Main
const GameProcFunctions data_ov214_021bb474 = { func_ov214_021b95a0, func_ov214_021b9834, func_ov214_021b9798 };

// BG0 of the sub screen
// pokebw2's sBtlRecSelSubBGSetups
static const BtlRecSelBGSetup data_ov214_021bb498[1] = {
    { 4,
      { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x8000, GX_BG_CHARBASE_0x00000, 0x8000,
        GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
      BGMODE_TEXT,
      TRUE },
};

// By battle mode, as the video's header has it
// pokebw2's sModes
static const BtlRecSelMode data_ov214_021bb4c4[] = {
    { Title_Text_020, FALSE },
    { Title_Text_021, FALSE },
    { Title_Text_022, FALSE },
    { Title_Text_023, FALSE },
    { Title_Text_024, FALSE },
    { Title_Text_025, FALSE },
    { Title_Text_026, FALSE },
    { Title_Text_027, FALSE },
    { Title_Text_028, FALSE },
    { Title_Text_029, FALSE },
    { Title_Text_030, FALSE },
    { Title_Text_031, FALSE },
    { Title_Text_032, FALSE },
    { Title_Text_033, FALSE },
    { Title_Text_034, FALSE },
    { Title_Text_035, FALSE },
    { Title_Text_036, TRUE },
    { Title_Text_037, TRUE },
    { Title_Text_038, TRUE },
    { Title_Text_039, TRUE },
    { Title_Text_040, FALSE },
    { Title_Text_041, FALSE },
    { Title_Text_042, TRUE },
    { Title_Text_043, FALSE },
    { Title_Text_044, FALSE },
    { Title_Text_045, FALSE },
    { Title_Text_046, FALSE },
    { Title_Text_047, FALSE },
    { Title_Text_048, FALSE },
    { Title_Text_049, FALSE },
    { Title_Text_050, FALSE },
    { Title_Text_051, FALSE },
    { Title_Text_052, FALSE },
    { Title_Text_053, FALSE },
    { Title_Text_054, FALSE },
    { Title_Text_055, FALSE },
    { Title_Text_056, FALSE },
    { Title_Text_057, FALSE },
    { Title_Text_058, FALSE },
    { Title_Text_059, FALSE },
    { Title_Text_060, FALSE },
};



BOOL func_ov214_021b95a0(GameProc *proc, u32 *state, void *param, void *work) {
    BtlRecSelParam *prm = param;
    BtlRecSelWork *wk;
    u8 i;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_BTL_REC_SEL, 0x50000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(BtlRecSelWork), HEAPID_BTL_REC_SEL);
    MI_CpuFill8(wk, 0, sizeof(BtlRecSelWork));
    wk->heapId = HEAPID_BTL_REC_SEL;
    for (i = 0; i <= BGSYS_BG_COUNT - 1; i++) {
        GFL_BGSysSetBGEnabled(i, FALSE);
    }
    wk->graphic = func_ov214_021bb178(1, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->messageQueue = func_0201c440(wk->heapId);
    wk->titleQueue = func_0201c440(wk->heapId);
    wk->timeLabelQueue = func_0201c440(wk->heapId);
    wk->timeQueue = func_0201c440(wk->heapId);
    wk->hasVsRecorder = BagSave_CheckAmount(GameData_GetBag(prm->gameData), ITEM_VS_RECORDER, 1, wk->heapId);
    wk->skip = FALSE;
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov214_021b9f18, wk, 1);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE, wk->heapId);
    func_ov214_021babf4(prm, wk);
    func_ov214_021bad08(prm, wk);
    func_ov214_021bad14(prm, wk);
    func_ov214_021b9f24(prm, wk);
    func_ov214_021ba074(prm, wk);
    func_ov214_021ba2f4(prm, wk);
    func_ov214_021ba3c4(prm, wk);
    GFL_BGSysSetBGPriority(2, 2);
    GFL_BGSysSetBGPriority(3, 1);
    GFL_BGSysSetBGPriority(1, 0);
    GFL_BGSysSetBGEnabled(2, TRUE);
    GFL_BGSysSetBGEnabled(3, TRUE);
    GFL_BGSysSetBGEnabled(1, TRUE);
    GFL_BGSysResetStdPalette(0, GX_RGB(0, 0, 0));
    GFL_BGSysResetStdPalette(4, GX_RGB(0, 0, 0));
    GFL_FadeSet(3, 16, 16, 0);
    if (wk->hasVsRecorder) {
        if (prm->recorded) {
            if (prm->compatible) {
                func_ov214_021b9eb8(state, prm, wk, SEQ_QUESTION, 3, 16, 0, 2, FALSE);
                func_ov214_021b9ef0(prm, wk, SEQ_SAVE_YESNO, 0, TRUE);
                func_ov214_021ba6a0(prm, wk, TIME_LIMIT);
                if (prm->comm) {
                    func_ov214_021bac58(prm, wk);
                }
                wk->timerStopped = TRUE;
            } else {
                func_ov214_021b9eb8(state, prm, wk, SEQ_END_MESSAGE, 3, 16, 0, 2, FALSE);
            }
        } else {
            func_ov214_021b9eb8(state, prm, wk, SEQ_END_MESSAGE, 3, 16, 0, 2, FALSE);
        }
    } else {
        if (prm->comm) {
            func_ov214_021b9eb8(state, prm, wk, SEQ_END_MESSAGE, 3, 16, 0, 2, FALSE);
        } else {
            wk->skip = TRUE;
        }
    }
    func_ov214_021bae4c(prm, wk);
    func_ov214_021bae9c(prm, wk);
    func_ov214_021bae88(prm, wk);
    func_0203d34c(TRUE, wk->heapId);
    return TRUE;
}

BOOL func_ov214_021b9798(GameProc *proc, u32 *state, void *param, void *work) {
    BtlRecSelParam *prm = param;
    BtlRecSelWork *wk = work;

    func_ov214_021baf30(prm, wk);
    func_ov214_021ba43c(prm, wk);
    func_ov214_021ba39c(prm, wk);
    func_ov214_021ba018(prm, wk);
    func_ov214_021bae10(prm, wk);
    func_ov214_021bac3c(prm, wk);
    GFL_MsgDataFree(wk->msgData);
    GFL_TCBRemove(wk->vblankTask);
    func_0201c6ec(wk->timeQueue);
    func_0201c4c0(wk->timeQueue);
    func_0201c6ec(wk->timeLabelQueue);
    func_0201c4c0(wk->timeLabelQueue);
    func_0201c6ec(wk->titleQueue);
    func_0201c4c0(wk->titleQueue);
    func_0201c6ec(wk->messageQueue);
    func_0201c4c0(wk->messageQueue);
    GFL_FontFree(wk->font);
    func_ov214_021bb220(wk->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_BTL_REC_SEL);
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

static BOOL func_ov214_021b9834(GameProc *proc, u32 *state, void *param, void *work) {
    BtlRecSelParam *prm = param;
    BtlRecSelWork *wk = work;
    SaveControl *save = GameData_GetSaveControl(prm->gameData);
    u32 saved;
    u32 result;
    BOOL done;
    u32 message;

    GCTX_HIDGetPressedKeys();
    switch (*state) {
    case SEQ_FADE:
        if (wk->skip) {
            *state = SEQ_END;
            break;
        }
        if (wk->paletteFade == TRUE) {
            if (wk->fadeStart == 0) {
                wk->fadeRequest = FADE_REQ_OUT;
            } else {
                wk->fadeRequest = FADE_REQ_IN;
            }
        } else {
            GFL_FadeSet(wk->fadeMode, wk->fadeStart, wk->fadeEnd, wk->fadeWait);
        }
        *state = SEQ_FADE_WAIT;
        break;
    case SEQ_FADE_WAIT:
        done = FALSE;
        if (wk->paletteFade == TRUE) {
            if (wk->fadeState == FADE_STATE_OUT || wk->fadeState == FADE_STATE_IN) {
                done = TRUE;
            }
        } else if (!GFL_FadeIsRunning()) {
            done = TRUE;
        }
        if (done) {
            wk->timerStopped = FALSE;
            *state = wk->fadeNextSeq;
        }
        break;
    case SEQ_QUESTION:
        func_ov214_021ba108(prm, wk, wk->questionMessage, FALSE);
        *state = SEQ_QUESTION_WAIT;
        break;
    case SEQ_QUESTION_WAIT:
        if (func_ov214_021ba16c(prm, wk)) {
            func_ov214_021ba3a0(prm, wk);
            *state = wk->yesNoSeq;
        }
        break;
    case SEQ_MESSAGE:
        func_ov214_021ba108(prm, wk, wk->message, FALSE);
        *state = wk->messageNextSeq;
        break;
    case SEQ_SAVE_YESNO: {
        u32 choice = func_0201febc(wk->yesNo);

        if (choice != BMPMENU_NULL) {
            func_020355d8(FALSE);
            if (choice == 0) {
                // With a video saved already, show it and ask whether to overwrite it
                if (func_0200b5cc(save, wk->heapId, &saved, 0)) {
                    wk->timerStopped = TRUE;
                    func_ov214_021b9eb8(state, prm, wk, SEQ_SHOW_PREVIOUS, 1, 0, 16, -2, TRUE);
                } else {
                    func_ov214_021ba6f0(prm, wk);
                    *state = SEQ_SAVE_MESSAGE;
                }
            } else {
                func_ov214_021b9ed4(state, prm, wk, SEQ_NO_SAVE_YESNO, Title_Text_003, TRUE);
            }
        } else if (wk->timeUp) {
            func_0201fef4(wk->yesNo);
            *state = SEQ_END_MESSAGE;
        }
        break;
    }
    case SEQ_NO_SAVE_YESNO: {
        u32 choice = func_0201febc(wk->yesNo);

        if (choice != BMPMENU_NULL) {
            func_020355d8(FALSE);
            if (choice == 0) {
                func_ov214_021ba6f0(prm, wk);
                *state = SEQ_END_MESSAGE;
            } else {
                func_ov214_021b9ed4(state, prm, wk, SEQ_SAVE_YESNO, Title_Text_000, TRUE);
            }
        } else if (wk->timeUp) {
            func_0201fef4(wk->yesNo);
            *state = SEQ_END_MESSAGE;
        }
        break;
    }
    case SEQ_OVERWRITE_YESNO: {
        u32 choice = func_0201febc(wk->yesNo);

        if (choice != BMPMENU_NULL) {
            func_020355d8(FALSE);
            if (choice == 0) {
                wk->hideNextSeq = SEQ_SAVE_MESSAGE;
            } else {
                wk->hideNextSeq = SEQ_END_MESSAGE;
            }
            func_ov214_021ba6f0(prm, wk);
            func_ov214_021b9eb8(state, prm, wk, SEQ_HIDE_PREVIOUS, 1, 0, 16, -2, TRUE);
        } else if (wk->timeUp) {
            func_0201fef4(wk->yesNo);
            wk->hideNextSeq = SEQ_END_MESSAGE;
            func_ov214_021b9eb8(state, prm, wk, SEQ_HIDE_PREVIOUS, 1, 0, 16, -2, TRUE);
        }
        break;
    }
    case SEQ_SHOW_PREVIOUS: {
        s32 i = 0;

        wk->video = func_0200b5a4(save, wk->heapId, &result, 0);
        func_ov214_021bad08(prm, wk);
        func_ov214_021ba0b0(prm, wk);
        if (result == 1) {
            BattleRecHeader *header = func_0200b934(wk->video);

            wk->mode = func_0200b98c(header, BATTLE_REC_HEADER_MODE, 0);
            func_ov214_021babb8(prm, wk);
            for (i = 0; i < SIDE_COUNT * PARTY_SIZE; i++) {
                u32 species;
                u32 form = 0;
                u32 sex = 0;

                species = func_0200b98c(header, BATTLE_REC_HEADER_SPECIES, i);
                if (species != 0) {
                    form = func_0200b98c(header, BATTLE_REC_HEADER_FORM, i);
                    sex = func_0200b98c(header, BATTLE_REC_HEADER_GENDER, i);
                }
                if (wk->oneTrainerPerSide) {
                    if (i < PARTY_SIZE) {
                        wk->icons[1][i % PARTY_SIZE].species = species;
                        wk->icons[1][i % PARTY_SIZE].form = form;
                        wk->icons[1][i % PARTY_SIZE].sex = sex;
                    } else {
                        wk->icons[0][i % PARTY_SIZE].species = species;
                        wk->icons[0][i % PARTY_SIZE].form = form;
                        wk->icons[0][i % PARTY_SIZE].sex = sex;
                    }
                } else {
                    // Three of each Trainer, the partners in the second half of a side
                    u32 side;
                    u32 index;

                    if (i < 3) {
                        side = 1;
                        index = i;
                    } else if (i < 6) {
                        side = 0;
                        index = i - 3;
                    } else if (i < 9) {
                        side = 1;
                        index = i - 3;
                    } else {
                        side = 0;
                        index = i - 6;
                    }
                    wk->icons[side][index].species = species;
                    wk->icons[side][index].form = form;
                    wk->icons[side][index].sex = sex;
                }
            }
            func_ov214_021ba5c8(prm, wk);
            func_ov214_021ba8dc(prm, wk);
            if (wk->oneTrainerPerSide) {
                func_ov214_021bac90(prm, wk);
            } else {
                func_ov214_021baccc(prm, wk);
            }
            func_ov214_021b9eb8(state, prm, wk, SEQ_QUESTION, 1, 16, 0, -2, TRUE);
            func_ov214_021b9ef0(prm, wk, SEQ_OVERWRITE_YESNO, Title_Text_006, FALSE);
        } else {
            func_ov214_021b9eb8(state, prm, wk, SEQ_SAVE_MESSAGE, 1, 16, 0, -2, TRUE);
        }
        func_0200b438(wk->video);
        break;
    }
    case SEQ_HIDE_PREVIOUS:
        func_ov214_021ba650(prm, wk);
        func_ov214_021baab0(prm, wk);
        func_ov214_021bad08(prm, wk);
        func_ov214_021b9eb8(state, prm, wk, wk->hideNextSeq, 1, 16, 0, -2, TRUE);
        func_ov214_021ba0b0(prm, wk);
        if (wk->hideNextSeq == SEQ_END_MESSAGE && wk->hasVsRecorder && prm->compatible && !prm->comm) {
            func_ov214_021ba0dc(prm, wk);
            func_ov214_021ba0a4(prm, wk);
        }
        func_ov214_021ba6f0(prm, wk);
        break;
    case SEQ_SAVE_MESSAGE:
        func_ov214_021ba254(prm, wk, Title_Text_009);
        *state = SEQ_SAVE_MESSAGE_WAIT;
        break;
    case SEQ_SAVE_MESSAGE_WAIT:
        if (func_ov214_021ba2b8(prm, wk)) {
            wk->waitIcon = func_0202ee30(GFL_VBlankGetTCBMgr(), wk->messageWindow, 15, 16, wk->heapId);
            wk->unk2B4 = 0;
            wk->saveSeq = 0;
            wk->saveSubSeq = 0;
            wk->saveResult = 0;
            *state = SEQ_SAVE;
        }
        break;
    case SEQ_SAVE:
        if (wk->saveResult != 2 && wk->saveResult != 3) {
            wk->saveResult =
                func_0200b6e0(prm->gameData, wk->heapId, prm->mode, prm->counter, 0, &wk->saveSeq, &wk->saveSubSeq);
        }
        if (wk->saveResult == 2 || wk->saveResult == 3) {
            func_0200b490();
            func_0202f038(wk->waitIcon);
            if (wk->saveResult == 2) {
                GFL_SndSEPlay(SEQ_SE_SAVE);
                func_ov214_021b9efc(state, prm, wk, SEQ_SAVED_WAIT, Title_Text_010);
                wk->waitTimer = RESULT_WAIT;
            } else if (wk->saveResult == 3) {
                func_ov214_021b9efc(state, prm, wk, SEQ_SAVE_FAILED_WAIT, Title_Text_011);
                wk->waitTimer = RESULT_WAIT;
            }
        }
        break;
    case SEQ_SAVED_WAIT:
        if (func_ov214_021ba16c(prm, wk)) {
            *state = SEQ_END_MESSAGE;
        }
        break;
    case SEQ_SAVE_FAILED_WAIT:
        if (func_ov214_021ba16c(prm, wk)) {
            *state = SEQ_END_MESSAGE;
        }
        break;
    case SEQ_END_MESSAGE:
        if (wk->hasVsRecorder) {
            if (prm->recorded) {
                if (prm->compatible) {
                    if (prm->comm) {
                        message = Title_Text_012;
                    } else {
                        func_ov214_021b9eb8(state, prm, wk, SEQ_END, 3, 0, 16, 2, FALSE);
                        break;
                    }
                } else {
                    if (prm->comm) {
                        message = Title_Text_013;
                    } else {
                        message = Title_Text_014;
                    }
                }
            } else {
                message = Title_Text_015;
            }
        } else {
            if (prm->comm) {
                message = Title_Text_012;
            } else {
                *state = SEQ_END;
                break;
            }
        }
        if (prm->comm) {
            func_ov214_021ba254(prm, wk, message);
        } else {
            func_ov214_021ba108(prm, wk, message, TRUE);
        }
        *state = SEQ_END_MESSAGE_WAIT;
        break;
    case SEQ_END_MESSAGE_WAIT:
        done = FALSE;
        if (prm->comm) {
            if (func_ov214_021ba2b8(prm, wk)) {
                done = TRUE;
            }
        } else if (func_ov214_021ba16c(prm, wk)) {
            done = TRUE;
        }
        if (done) {
            if (prm->comm) {
                func_02039c68(func_020399a8(), 150, 43);
                wk->waitIcon = func_0202ee30(GFL_VBlankGetTCBMgr(), wk->messageWindow, 15, 16, wk->heapId);
            }
            *state = SEQ_END_SYNC;
        }
        break;
    case SEQ_END_SYNC:
        // Until the other player is done too
        done = TRUE;
        if (prm->comm) {
            if (!func_0200fb24()) {
                done = func_02039cb4(func_020399a8(), 150, 43);
            }
            if (done) {
                func_0202f038(wk->waitIcon);
            }
        }
        if (done) {
            func_ov214_021b9eb8(state, prm, wk, SEQ_END, 3, 0, 16, 2, FALSE);
        }
        break;
    case SEQ_END:
        return TRUE;
    }
    func_ov214_021ba058(prm, wk);
    func_ov214_021ba45c(prm, wk);
    func_ov214_021ba4e8(prm, wk);
    func_ov214_021baf64(prm, wk);
    func_0201c4e4(wk->messageQueue);
    func_0201c4e4(wk->titleQueue);
    func_0201c4e4(wk->timeLabelQueue);
    func_0201c4e4(wk->timeQueue);
    func_ov214_021bb27c(wk->graphic);
    func_ov214_021bb290(wk->graphic);
    func_ov214_021bb29c(wk->graphic);
    return FALSE;
}

// Sets the fade SEQ_FADE starts, and the step after it
static void func_ov214_021b9eb8(u32 *state, BtlRecSelParam *param, BtlRecSelWork *work, u32 nextSeq, u32 mode, s32 start,
                              s32 end, s32 wait, BOOL paletteFade) {
    *state = SEQ_FADE;
    work->fadeNextSeq = nextSeq;
    work->fadeMode = mode;
    work->fadeStart = start;
    work->fadeEnd = end;
    work->fadeWait = wait;
    work->paletteFade = paletteFade;
}

static void func_ov214_021b9ed4(u32 *state, BtlRecSelParam *param, BtlRecSelWork *work, u32 yesNoSeq, u32 message,
                                  BOOL clearBGOnStop) {
    *state = SEQ_QUESTION;
    func_ov214_021b9ef0(param, work, yesNoSeq, message, clearBGOnStop);
}

static void func_ov214_021b9ef0(BtlRecSelParam *param, BtlRecSelWork *work, u32 yesNoSeq, u32 message,
                                  BOOL clearBGOnStop) {
    work->yesNoSeq = yesNoSeq;
    work->questionMessage = message;
    work->clearBGOnStop = clearBGOnStop;
}

// pokebw2's BtlRecSel_ShowMessage
static void func_ov214_021b9efc(u32 *state, BtlRecSelParam *param, BtlRecSelWork *work, u32 nextSeq, u32 message) {
    *state = SEQ_MESSAGE;
    func_ov214_021b9f10(param, work, nextSeq, message);
}

// pokebw2's BtlRecSel_SetMessage
static void func_ov214_021b9f10(BtlRecSelParam *param, BtlRecSelWork *work, u32 nextSeq, u32 message) {
    work->messageNextSeq = nextSeq;
    work->message = message;
}

// pokebw2's BtlRecSel_VBlank
void func_ov214_021b9f18(TCB *tcb, void *data) {
    func_ov214_021bb0c4(data);
}

// pokebw2's BtlRecSel_InitMessage
void func_ov214_021b9f24(BtlRecSelParam *param, BtlRecSelWork *work) {
    u16 *palette = GFL_HeapAllocate(work->heapId, 3 * sizeof(u16), TRUE, "btl_rec_sel.c", 1513);

    // The text colors of the windows' palette
    palette[0] = GX_RGB(11, 10, 10);
    palette[1] = GX_RGB(20, 20, 21);
    palette[2] = GX_RGB(31, 31, 31);
    GFL_BGSysUploadStdPalette(2, palette, 3 * sizeof(u16), (PALETTE_TEXT * 16 + 13) * sizeof(u16));
    GFL_HeapFree(palette);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, PALETTE_MESSAGE * 0x20, 0x20, work->heapId);
    work->blankWindow = BmpWin_CreateDynamic(1, 0, 0, 1, 1, PALETTE_TEXT, FALSE);
    GFL_BitmapFill(BmpWin_GetBitmap(work->blankWindow), 0);
    BmpWin_FlushChar(work->blankWindow);
    work->messageWindow = BmpWin_CreateDynamic(1, 1, 19, 30, 4, PALETTE_MESSAGE, FALSE);
    GFL_BitmapFill(BmpWin_GetBitmap(work->messageWindow), 15);
    BmpWin_FlushChar(work->messageWindow);
    work->frameChars = LoadCursorImageEndOfHeap(1, PALETTE_FRAME, 0, work->heapId);
    work->tcbExManager = GFL_TCBExMgrCreate(work->heapId, work->heapId, 1, 0);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    GFL_BGSysQueueScrLoad(1);
    work->stream = NULL;
    work->waitKey = FALSE;
    work->waitTimer = 0;
    work->messageStr = NULL;
}

// pokebw2's BtlRecSel_FreeMessage
void func_ov214_021ba018(BtlRecSelParam *param, BtlRecSelWork *work) {
    if (work->stream != NULL) {
        func_0201ce48(work->stream);
    }
    if (work->messageStr != NULL) {
        GFL_StrBufFree(work->messageStr);
    }
    GFL_TCBExMgrFree(work->tcbExManager);
    GFL_BGSysFreeCharMemory(1, CHAR_POS(work->frameChars), CHAR_SIZE(work->frameChars));
    BmpWin_Free(work->messageWindow);
    BmpWin_Free(work->blankWindow);
}

// pokebw2's BtlRecSel_UpdateMessage
static void func_ov214_021ba058(BtlRecSelParam *param, BtlRecSelWork *work) {
    GFL_TCBExMgrUpdate(work->tcbExManager);
    if (GCTX_HIDGetUpdateRate() == 30) {
        GFL_TCBExMgrUpdate(work->tcbExManager);
    }
}

// pokebw2's BtlRecSel_ShowMessageWindow
void func_ov214_021ba074(BtlRecSelParam *param, BtlRecSelWork *work) {
    BmpWin_DrawFrame(work->messageWindow, TRUE, CHAR_POS(work->frameChars), PALETTE_FRAME);
    BmpWin_Transfer(work->messageWindow);
}

// pokebw2's BtlRecSel_HideMessageFrame
static void func_ov214_021ba0a4(BtlRecSelParam *param, BtlRecSelWork *work) {
    func_0201f7a8(work->messageWindow, TRUE);
}

// pokebw2's BtlRecSel_ClearMessageWindow
static void func_ov214_021ba0b0(BtlRecSelParam *param, BtlRecSelWork *work) {
    GFL_BitmapFill(BmpWin_GetBitmap(work->messageWindow), 15);
    BmpWin_Transfer(work->messageWindow);
}

// pokebw2's BtlRecSel_EraseMessageWindow
static void func_ov214_021ba0dc(BtlRecSelParam *param, BtlRecSelWork *work) {
    GFL_BitmapFill(BmpWin_GetBitmap(work->messageWindow), 0);
    BmpWin_Transfer(work->messageWindow);
}

// Prints a message a character at a time. With waitKey it stays until a button or the touch screen is pressed
// pokebw2's BtlRecSel_StartStream
static void func_ov214_021ba108(BtlRecSelParam *param, BtlRecSelWork *work, u32 message, BOOL waitKey) {
    GFL_BitmapFill(BmpWin_GetBitmap(work->messageWindow), 15);
    if (work->stream != NULL) {
        func_0201ce48(work->stream);
    }
    if (work->messageStr != NULL) {
        GFL_StrBufFree(work->messageStr);
    }
    work->messageStr = GFL_MsgDataLoadStrbufNew(work->msgData, message);
    work->stream = func_0201cce4(work->messageWindow, 0, 0, work->messageStr, work->font, func_02012ffc(),
                                 work->tcbExManager, 2, work->heapId, 15);
    work->waitKey = waitKey;
}

// Twice a frame at 30 frames a second
// pokebw2's BtlRecSel_WaitStream
static BOOL func_ov214_021ba16c(BtlRecSelParam *param, BtlRecSelWork *work) {
    BOOL done = func_ov214_021ba190(param, work);

    if (!done && GCTX_HIDGetUpdateRate() == 30) {
        done = func_ov214_021ba190(param, work);
    }
    return done;
}

// pokebw2's BtlRecSel_UpdateStream
static BOOL func_ov214_021ba190(BtlRecSelParam *param, BtlRecSelWork *work) {
    BOOL done = FALSE;

    switch (func_0201ce30(work->stream)) {
    case PRINT_STREAM_RUNNING:
        if ((GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c38()) {
            func_0201ce5c(work->stream, 0);
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
            func_0201ce38(work->stream);
            if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                func_020355d8(FALSE);
            } else {
                func_020355d8(TRUE);
            }
        }
        break;
    case PRINT_STREAM_DONE:
        if (work->waitTimer != 0) {
            work->waitTimer--;
        } else {
            if (work->waitKey) {
                if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60()) {
                    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                        func_020355d8(FALSE);
                    } else {
                        func_020355d8(TRUE);
                    }
                    work->waitKey = FALSE;
                    done = TRUE;
                }
            } else {
                done = TRUE;
            }
        }
        break;
    }
    return done;
}

// Prints a message at once, through the queue
// pokebw2's BtlRecSel_PrintMessage
static void func_ov214_021ba254(BtlRecSelParam *param, BtlRecSelWork *work, u32 message) {
    GFL_BitmapFill(BmpWin_GetBitmap(work->messageWindow), 15);
    if (work->stream != NULL) {
        func_0201ce48(work->stream);
    }
    work->stream = NULL;
    if (work->messageStr != NULL) {
        GFL_StrBufFree(work->messageStr);
    }
    work->messageStr = GFL_MsgDataLoadStrbufNew(work->msgData, message);
    work->messagePrinted = FALSE;
    func_0201c6fc(work->messageQueue, BmpWin_GetBitmap(work->messageWindow), 0, 0, work->messageStr, work->font);
    func_ov214_021ba2b8(param, work);
}

// pokebw2's BtlRecSel_IsMessagePrinted
static BOOL func_ov214_021ba2b8(BtlRecSelParam *param, BtlRecSelWork *work) {
    if (!work->messagePrinted && !func_0201c6c4(work->messageQueue, BmpWin_GetBitmap(work->messageWindow))) {
        BmpWin_Transfer(work->messageWindow);
        work->messagePrinted = TRUE;
    }
    return work->messagePrinted;
}

// pokebw2's BtlRecSel_InitYesNo
void func_ov214_021ba2f4(BtlRecSelParam *param, BtlRecSelWork *work) {
    // Where the menu's window will be, at the end of BG1's characters
    u32 chars = GFL_BGSysAllocChar(1, 0x500, TRUE);
    u16 *palette;

    GFL_BGSysFreeCharMemory(1, chars, 0x500);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, PALETTE_YESNO * 0x20, 0x20, work->heapId);
    palette = GFL_HeapAllocate(work->heapId, 3 * sizeof(u16), TRUE, "btl_rec_sel.c", 1785);
    palette[0] = GX_RGB(11, 10, 10);
    palette[1] = GX_RGB(20, 20, 21);
    palette[2] = GX_RGB(31, 31, 31);
    GFL_BGSysUploadStdPalette(2, palette, 3 * sizeof(u16), (PALETTE_YESNO * 16 + 13) * sizeof(u16));
    GFL_HeapFree(palette);
    work->yesNoSetup.bg = 1;
    work->yesNoSetup.x = 24;
    work->yesNoSetup.y = 13;
    work->yesNoSetup.palette = PALETTE_YESNO;
    work->yesNoSetup.unk4 = chars;
}

// pokebw2's BtlRecSel_FreeYesNo
void func_ov214_021ba39c(BtlRecSelParam *param, BtlRecSelWork *work) {
}

// pokebw2's BtlRecSel_CreateYesNo
static void func_ov214_021ba3a0(BtlRecSelParam *param, BtlRecSelWork *work) {
    work->yesNo =
        ShopUI_CreateConfirmDialog(&work->yesNoSetup, CHAR_POS(work->frameChars), PALETTE_FRAME, 0, work->heapId);
}

// pokebw2's BtlRecSel_InitTextWindows
void func_ov214_021ba3c4(BtlRecSelParam *param, BtlRecSelWork *work) {
    // The title and the battle mode, and the time limit's label and number
    BtlRecSelWindowSetup setups[TEXT_WINDOW_COUNT] = {
        { 1, 2, 1, 13, 2, FALSE },
        { 1, 3, 3, 26, 4, FALSE },
        { 3, 2, 15, 11, 2, TRUE },
        { 3, 13, 15, 5, 3, TRUE },
    };
    u8 i;

    for (i = 0; i < TEXT_WINDOW_COUNT; i++) {
        work->textWindows[i] = BmpWin_CreateDynamic(setups[i].bg, setups[i].x, setups[i].y, setups[i].width,
                                                    setups[i].height, PALETTE_TEXT, setups[i].fromEnd);
        GFL_BitmapFill(BmpWin_GetBitmap(work->textWindows[i]), 0);
        work->textPrinted[i] = TRUE;
    }
    work->timeColor = 0;
    work->timeSeconds = 0;
}

// pokebw2's BtlRecSel_FreeTextWindows
void func_ov214_021ba43c(BtlRecSelParam *param, BtlRecSelWork *work) {
    u8 i;

    for (i = 0; i < TEXT_WINDOW_COUNT; i++) {
        BmpWin_Free(work->textWindows[i]);
    }
}

// Counts the time limit down while the other player is choosing too. timeUp is set when it reaches 0; the number
// stays for half a second more
// pokebw2's BtlRecSel_UpdateTimer
static void func_ov214_021ba45c(BtlRecSelParam *param, BtlRecSelWork *work) {
    u32 fps;

    if (GCTX_HIDGetUpdateRate() == 30) {
        fps = 30;
    } else {
        fps = 60;
    }
    if (param->comm && !work->timerStopped) {
        if (work->timer >= fps || work->timerEnd != 0) {
            func_ov214_021ba794(param, work, work->timer / fps);
            if (work->timer >= fps) {
                work->timer--;
                if (work->timer == fps - 1) {
                    work->timeUp = TRUE;
                }
            } else {
                work->timerEnd--;
                if (work->timerEnd == 0) {
                    func_ov214_021ba6f0(param, work);
                }
            }
        }
    }
}

// pokebw2's BtlRecSel_FlushTextWindows
static void func_ov214_021ba4e8(BtlRecSelParam *param, BtlRecSelWork *work) {
    u8 i;

    for (i = TEXT_WINDOW_TITLE; i <= TEXT_WINDOW_MODE; i++) {
        if (!work->textPrinted[i] && !func_0201c6c4(work->titleQueue, BmpWin_GetBitmap(work->textWindows[i]))) {
            BmpWin_Transfer(work->textWindows[i]);
            work->textPrinted[i] = TRUE;
        }
    }
    if (!work->textPrinted[TEXT_WINDOW_TIME_LABEL] &&
        !func_0201c6c4(work->timeLabelQueue, BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME_LABEL]))) {
        BmpWin_Transfer(work->textWindows[TEXT_WINDOW_TIME_LABEL]);
        work->textPrinted[TEXT_WINDOW_TIME_LABEL] = TRUE;
    }
    if (!work->textPrinted[TEXT_WINDOW_TIME] &&
        !func_0201c6c4(work->timeQueue, BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME]))) {
        BmpWin_Transfer(work->textWindows[TEXT_WINDOW_TIME]);
        work->textPrinted[TEXT_WINDOW_TIME] = TRUE;
    }
}

// Prints the previous record's label and the saved video's battle mode
// pokebw2's BtlRecSel_PrintPrevious
static void func_ov214_021ba5c8(BtlRecSelParam *param, BtlRecSelWork *work) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, Title_Text_019);

    func_0201c724(work->titleQueue, BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TITLE]), 4, 1, str, work->font,
                  COLOR_TEXT);
    GFL_StrBufFree(str);
    work->textPrinted[TEXT_WINDOW_TITLE] = FALSE;
    str = GFL_MsgDataLoadStrbufNew(work->msgData, work->modeMessage);
    func_0201c724(work->titleQueue, BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_MODE]), 4, 1, str, work->font,
                  COLOR_TEXT);
    GFL_StrBufFree(str);
    work->textPrinted[TEXT_WINDOW_MODE] = FALSE;
    func_ov214_021ba4e8(param, work);
}

// pokebw2's BtlRecSel_ClearPrevious
static void func_ov214_021ba650(BtlRecSelParam *param, BtlRecSelWork *work) {
    u8 i;

    for (i = TEXT_WINDOW_TITLE; i <= TEXT_WINDOW_MODE; i++) {
        func_0201c6ec(work->titleQueue);
        GFL_BitmapFill(BmpWin_GetBitmap(work->textWindows[i]), 0);
        BmpWin_Transfer(work->textWindows[i]);
        work->textPrinted[i] = TRUE;
    }
}

// pokebw2's BtlRecSel_StartTimer
void func_ov214_021ba6a0(BtlRecSelParam *param, BtlRecSelWork *work, u16 seconds) {
    u32 fps;

    if (GCTX_HIDGetUpdateRate() == 30) {
        fps = 30;
    } else {
        fps = 60;
    }
    work->timer = fps * (seconds + 1) - 1;
    work->timerEnd = fps / 2;
    work->timerStopped = FALSE;
    work->timeUp = FALSE;
    work->timeColor = 0;
    work->timeSeconds = 0;
    func_ov214_021ba794(param, work, seconds);
}

// pokebw2's BtlRecSel_StopTimer
static void func_ov214_021ba6f0(BtlRecSelParam *param, BtlRecSelWork *work) {
    work->timer = 0;
    work->timerEnd = 0;
    func_0201c6ec(work->timeLabelQueue);
    GFL_BitmapFill(BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME_LABEL]), 0);
    BmpWin_Transfer(work->textWindows[TEXT_WINDOW_TIME_LABEL]);
    work->textPrinted[TEXT_WINDOW_TIME_LABEL] = TRUE;
    func_0201c6ec(work->timeQueue);
    GFL_BitmapFill(BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME]), 0);
    BmpWin_Transfer(work->textWindows[TEXT_WINDOW_TIME]);
    work->textPrinted[TEXT_WINDOW_TIME] = TRUE;
    if (work->clearBGOnStop) {
        func_ov214_021bad08(param, work);
    }
    work->timeColor = 0;
    work->timeSeconds = 0;
}

// Prints the time limit, in another color for the last 10 seconds
// pokebw2's BtlRecSel_PrintTimer
static void func_ov214_021ba794(BtlRecSelParam *param, BtlRecSelWork *work, u16 seconds) {
    u16 color;
    u32 colorType;

    if (!param->comm) {
        return;
    }
    if (seconds > 10) {
        color = COLOR_TEXT;
        colorType = 1;
    } else {
        color = COLOR_TIME_LOW;
        colorType = 2;
    }
    if (colorType != work->timeColor) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME_LABEL]), 0);
        GFL_BitmapFill(BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME]), 0);
    } else if (seconds != work->timeSeconds) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME]), 0);
    }
    if (colorType != work->timeColor) {
        StrBuf *str;

        func_0201c6ec(work->timeLabelQueue);
        str = GFL_MsgDataLoadStrbufNew(work->msgData, Title_Text_016);
        func_0201c724(work->timeLabelQueue, BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME_LABEL]), 4, 1, str,
                      work->font, color);
        work->textPrinted[TEXT_WINDOW_TIME_LABEL] = FALSE;
        GFL_StrBufFree(str);
    }
    if (colorType != work->timeColor || seconds != work->timeSeconds) {
        WordSet *wordSet = GFL_WordSetSystemCreateDefault(work->heapId);
        StrBuf *format = GFL_MsgDataLoadStrbufNew(work->msgData, Title_Text_018);
        StrBuf *str = GFL_StrBufCreate(8, work->heapId);

        func_0201c6ec(work->timeQueue);
        WordSetNumber(wordSet, 1, seconds, 2, 2, TRUE);
        GFL_WordSetFormatStrbuf(wordSet, str, format);
        func_0201c724(work->timeQueue, BmpWin_GetBitmap(work->textWindows[TEXT_WINDOW_TIME]), 2, 2, str, work->font,
                      color);
        work->textPrinted[TEXT_WINDOW_TIME] = FALSE;
        GFL_StrBufFree(str);
        GFL_StrBufFree(format);
        GFL_WordSetSystemFree(wordSet);
    }
    work->timeColor = colorType;
    work->timeSeconds = seconds;
    func_ov214_021ba4e8(param, work);
}

// Creates the icons of the saved video's Pokémon: a row for each side, or two rows of three with two Trainers a side
// pokebw2's BtlRecSel_CreateIcons
static void func_ov214_021ba8dc(BtlRecSelParam *param, BtlRecSelWork *work) {
    ClActUnit *unit = func_ov214_021bb2a8(work->graphic);
    u8 side;
    u8 i;

    for (side = 0; side < SIDE_COUNT; side++) {
        for (i = 0; i < PARTY_SIZE; i++) {
            work->icons[side][i].egg = FALSE;
            work->icons[side][i].anim = 1;
        }
    }
    for (side = 0; side < SIDE_COUNT; side++) {
        for (i = 0; i < PARTY_SIZE; i++) {
            if (work->oneTrainerPerSide) {
                if (side == 0) {
                    work->icons[side][i].x = i * 24 + 100;
                    work->icons[side][i].y = 64;
                    work->icons[side][i].priority = PARTY_SIZE - i;
                } else {
                    work->icons[side][i].x = i * 24 + 36;
                    work->icons[side][i].y = 112;
                    work->icons[side][i].priority = PARTY_SIZE - i;
                }
            } else {
                if (side == 0) {
                    if (i < 3) {
                        work->icons[side][i].x = i * 24 + 172;
                        work->icons[side][i].y = 64;
                        work->icons[side][i].priority = PARTY_SIZE - i;
                    } else {
                        work->icons[side][i].x = (i - 3) * 24 + 172;
                        work->icons[side][i].y = 88;
                        work->icons[side][i].priority = PARTY_SIZE - i;
                    }
                } else {
                    if (i < 3) {
                        work->icons[side][i].x = i * 24 + 36;
                        work->icons[side][i].y = 88;
                        work->icons[side][i].priority = PARTY_SIZE - i;
                    } else {
                        work->icons[side][i].x = (i - 3) * 24 + 36;
                        work->icons[side][i].y = 112;
                        work->icons[side][i].priority = PARTY_SIZE - i;
                    }
                }
            }
        }
    }
    for (side = 0; side < SIDE_COUNT; side++) {
        for (i = 0; i < PARTY_SIZE; i++) {
            if (work->icons[side][i].species != 0) {
                work->icons[side][i].actor =
                    func_ov214_021bab0c(unit, work->heapId, CLACT_VRAM_MAIN, 0, work->icons[side][i].species,
                                         work->icons[side][i].form, work->icons[side][i].sex, work->icons[side][i].egg,
                                         &work->icons[side][i].res, work->icons[side][i].x, work->icons[side][i].y,
                                         work->icons[side][i].anim, 1, work->icons[side][i].priority);
            }
        }
    }
}

// pokebw2's BtlRecSel_FreeIcons
static void func_ov214_021baab0(BtlRecSelParam *param, BtlRecSelWork *work) {
    u8 side;
    u8 i;

    for (side = 0; side < SIDE_COUNT; side++) {
        for (i = 0; i < PARTY_SIZE; i++) {
            if (work->icons[side][i].species != 0) {
                func_ov214_021baba4(&work->icons[side][i].res, work->icons[side][i].actor);
            }
        }
    }
}

// Loads a Pokémon's icon and creates its actor
static ClActor *func_ov214_021bab0c(ClActUnit *unit, HeapID heapId, u32 vramType, u8 paletteOffset, u32 species,
                                     u32 form, u32 sex, BOOL egg, UIObjRes *res, u8 x, u8 y, u8 anim, u8 bgPriority,
                                     u8 priority) {
    UIObjResSetup setup;
    ClActor *actor;

    setup.vramType = vramType;
    setup.flags = 1;
    setup.arcId = ARCID_POKEICON;
    setup.paletteFile = func_0201bc3c();
    setup.charFile = PokeParty_GetIconIndex(species, form, sex, egg);
    setup.cellFile = func_0201bc40();
    setup.animFile = getOBJTileMapping_MainEng();
    setup.paletteOffset = paletteOffset;
    setup.paletteStart = 0;
    setup.paletteCount = 3;
    func_ov067_021b6208(res, &setup, unit, heapId);
    actor = func_ov067_021b629c(res, unit, x, y, anim, heapId);
    func_0204b84c(actor, bgPriority);
    func_0204b800(actor, priority);
    func_0204b92c(actor, TRUE);
    func_0204b6f4(actor, func_0201bb5c(species, form, sex, egg), 0);
    return actor;
}

// pokebw2's BtlRecSel_FreeIcon
static void func_ov214_021baba4(UIObjRes *res, ClActor *actor) {
    func_0204b3b4(actor);
    func_ov067_021b6284(res);
}

// Sets the saved video's battle mode message and party layout from its mode
// pokebw2's BtlRecSel_SetMode
static void func_ov214_021babb8(BtlRecSelParam *param, BtlRecSelWork *work) {
    u8 mode = work->mode;

    if (mode >= NELEMS(data_ov214_021bb4c4)) {
        mode = 0;
    }
    work->modeMessage = data_ov214_021bb4c4[mode].message;
    if (!data_ov214_021bb4c4[mode].multi) {
        work->oneTrainerPerSide = TRUE;
    } else {
        work->oneTrainerPerSide = FALSE;
    }
}

// pokebw2's BtlRecSel_LoadBG
void func_ov214_021babf4(BtlRecSelParam *param, BtlRecSelWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_BTL_REC_SEL, work->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0, 0x140, work->heapId);
    work->bgChars = GFL_BGSysLoadArcNCGRDynamic(arc, 1, 2, 0, FALSE, work->heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's BtlRecSel_FreeBG
void func_ov214_021bac3c(BtlRecSelParam *param, BtlRecSelWork *work) {
    GFL_BGSysFreeCharMemory(2, CHAR_POS(work->bgChars), CHAR_SIZE(work->bgChars));
}

// BG2's screens: the time limit's frame, and the frames of the saved video's parties
// pokebw2's BtlRecSel_ShowTimerBG
void func_ov214_021bac58(BtlRecSelParam *param, BtlRecSelWork *work) {
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_BTL_REC_SEL, 2, 2, CHAR_POS(work->bgChars), 0, FALSE, work->heapId);
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysMoveBGReq(3, BG_MOVE_SET_Y, 0);
}

// pokebw2's BtlRecSel_ShowPartiesBG
static void func_ov214_021bac90(BtlRecSelParam *param, BtlRecSelWork *work) {
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_BTL_REC_SEL, 3, 2, CHAR_POS(work->bgChars), 0, FALSE, work->heapId);
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, -8);
}

// pokebw2's BtlRecSel_ShowMultiPartiesBG
static void func_ov214_021baccc(BtlRecSelParam *param, BtlRecSelWork *work) {
    loadBGScrToVramByNarcNoReserveNegAlign(ARCID_BTL_REC_SEL, 4, 2, CHAR_POS(work->bgChars), 0, FALSE, work->heapId);
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysMoveBG(3, BG_MOVE_SET_Y, -8);
}

// pokebw2's BtlRecSel_ClearBG
static void func_ov214_021bad08(BtlRecSelParam *param, BtlRecSelWork *work) {
    GFL_BGSysFillScrAsync(2, 0);
}

// The arc files of the C-Gear background that func_ov214_021bad14 loads, by palette: the palette, the character
// and the screen
typedef struct {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
} BtlRecSelFileIds;

static BtlRecSelFileIds data_ov214_021bb640[2] = {
    { 0xd, 0xe, 3 },
    { 6, 7, 3 },
};

// The sub screen's background, in the player's color
// pokebw2's BtlRecSel_InitSubBG
void func_ov214_021bad14(BtlRecSelParam *param, BtlRecSelWork *work) {
    u8 gender = getTrainerGender(GetGameDataPlayerInfo(param->gameData));
    int i;
    HeapID heapId;
    u32 palette;
    ArcTool *arc;

    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GXS_DispOn();
    GFL_BGSysDisableAllB();
    GXS_SetGraphicsMode(0);
    GFL_BGSysDisableBGsB();
    for (i = 0; i < NELEMS(data_ov214_021bb498); i++) {
        GFL_BGSysCreateBG(data_ov214_021bb498[i].bg, &data_ov214_021bb498[i].setup, data_ov214_021bb498[i].mode);
        GFL_BGSysClearBG(data_ov214_021bb498[i].bg);
        GFL_BGSysSetBGEnabled(data_ov214_021bb498[i].bg, data_ov214_021bb498[i].enabled);
    }
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    heapId = work->heapId;
    palette = 1;
    if (gender != 1) {
        palette = 0;
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_C_GEAR, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, data_ov214_021bb640[palette].unk_0, 4, 0, 0, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, data_ov214_021bb640[palette].unk_4, 4, 0, 0, FALSE, heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, data_ov214_021bb640[palette].unk_8, 4, 0, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(4);
}

// pokebw2's BtlRecSel_FreeSubBG
void func_ov214_021bae10(BtlRecSelParam *param, BtlRecSelWork *work) {
    int i;

    for (i = 0; i < NELEMS(data_ov214_021bb498); i++) {
        GFL_BGSysReleaseBG(data_ov214_021bb498[i].bg);
    }
    G2S_BlendNone();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
}

// Loads an icon, which func_ov214_021bae88 frees once the palettes are copied for the fade
// pokebw2's BtlRecSel_CreateDummyIcon
void func_ov214_021bae4c(BtlRecSelParam *param, BtlRecSelWork *work) {
    ClActUnit *unit = func_ov214_021bb2a8(work->graphic);

    work->dummyActor =
        func_ov214_021bab0c(unit, work->heapId, CLACT_VRAM_MAIN, 0, 1, 0, 0, FALSE, &work->dummyRes, 0, 0, 0, 1, 1);
}

// pokebw2's BtlRecSel_FreeDummyIcon
void func_ov214_021bae88(BtlRecSelParam *param, BtlRecSelWork *work) {
    func_ov214_021baba4(&work->dummyRes, work->dummyActor);
}

// The fade of the main screen's BG and OBJ palettes, other than the last two of each
// pokebw2's BtlRecSel_InitPaletteFade
void func_ov214_021bae9c(BtlRecSelParam *param, BtlRecSelWork *work) {
    work->tcbManagerWork = GFL_HeapAllocate(work->heapId, GFL_TCBMgrCalcAllocSize(2), TRUE, "btl_rec_sel.c", 2403);
    work->tcbManager = GFL_TCBMgrCreate(2, work->tcbManagerWork);
    work->fade = func_02021518(work->heapId);
    func_02021ee4(work->fade, TRUE);
    func_0202155c(work->fade, 0, 14 * 0x20, work->heapId);
    func_0202155c(work->fade, 2, 14 * 0x20, work->heapId);
    func_020216d4(work->fade, 0, 0, 14 * 0x20);
    func_020216d4(work->fade, 2, 0, 14 * 0x20);
    work->fadeState = FADE_STATE_IN;
    work->fadeRequest = FADE_REQ_NONE;
}

// pokebw2's BtlRecSel_FreePaletteFade
void func_ov214_021baf30(BtlRecSelParam *param, BtlRecSelWork *work) {
    func_020215a0(work->fade, 0);
    func_020215a0(work->fade, 2);
    func_02021540(work->fade);
    func_02030da4(work->tcbManager);
    GFL_HeapFree(work->tcbManagerWork);
}

// pokebw2's BtlRecSel_UpdatePaletteFade
static void func_ov214_021baf64(BtlRecSelParam *param, BtlRecSelWork *work) {
    u16 mask;
    u8 i;

    GFL_TCBMgrUpdate(work->tcbManager);
    if (func_02021ed8(work->fade) == 0) {
        switch (work->fadeState) {
        case FADE_STATE_FADING_IN:
            work->fadeState = FADE_STATE_IN;
            break;
        case FADE_STATE_FADING_OUT:
            work->fadeState = FADE_STATE_OUT;
            break;
        }
        switch (work->fadeRequest) {
        case FADE_REQ_IN:
            if (work->fadeState != FADE_STATE_FADING_IN) {
                mask = 0;
                for (i = 0; i < 14; i++) {
                    mask |= 1 << i;
                }
                func_0202173c(work->fade, 1 << 0, mask, -2, 16, 0, GX_RGB(0, 0, 0), work->tcbManager);
                mask = 0;
                for (i = 0; i < 14; i++) {
                    mask |= 1 << i;
                }
                func_0202173c(work->fade, 1 << 2, mask, -2, 16, 0, GX_RGB(0, 0, 0), work->tcbManager);
                work->fadeState = FADE_STATE_FADING_IN;
                work->fadeRequest = FADE_REQ_NONE;
            }
            break;
        case FADE_REQ_OUT:
            if (work->fadeState != FADE_STATE_FADING_OUT) {
                mask = 0;
                for (i = 0; i < 14; i++) {
                    mask |= 1 << i;
                }
                func_0202173c(work->fade, 1 << 0, mask, -2, 0, 16, GX_RGB(0, 0, 0), work->tcbManager);
                mask = 0;
                for (i = 0; i < 14; i++) {
                    mask |= 1 << i;
                }
                func_0202173c(work->fade, 1 << 2, mask, -2, 0, 16, GX_RGB(0, 0, 0), work->tcbManager);
                work->fadeState = FADE_STATE_FADING_OUT;
                work->fadeRequest = FADE_REQ_NONE;
            }
            break;
        }
    }
}

// pokebw2's BtlRecSel_TransferPaletteFade
void func_ov214_021bb0c4(BtlRecSelWork *work) {
    func_02021d50(work->fade);
}
