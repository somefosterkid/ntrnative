// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_voice.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"
#include "constants/arc.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "nitro/os.h"
#include "pml/poke_graphic.h"
#include "pml/species_names.h"
#include "save/pokedex.h"
#include "system/game_data.h"
#include "system/printsys.h"

// The detail screen's cry page, zukan_detail_voice.c by the ROM's own string: the Pokémon on the top screen, which
// stretches while its cry plays, with the cry's length and its wave scrolling by on the touch screen

// The Pokédex's shortcut for the Y button, which the touch bar's check box registers
#define SHORTCUT_POKEDEX_VOICE 19 // Black's

enum {
    VOICE_SEQ_INIT,
    VOICE_SEQ_FADE_IN,
    VOICE_SEQ_START_PALFADE,
    VOICE_SEQ_WAIT_FADE_IN,
    VOICE_SEQ_MAIN,
    VOICE_SEQ_WAIT_FADE_OUT,
    VOICE_SEQ_FADE_OUT,
    VOICE_SEQ_EXIT,
};

// How the page ends
enum {
    VOICE_EXIT_NONE,
    VOICE_EXIT_PAGE,
    VOICE_EXIT_SCREEN,
};

// The background music fades out while the page is shown, so that the cries can be heard
enum {
    BGM_PLAYING,
    BGM_FADING_OUT,
    BGM_PAUSED,
    BGM_RESTORED,
};

// The Pokémon stretches up as its cry starts and back down once it ends
enum {
    STRETCH_NONE,
    STRETCH_GROWING,
    STRETCH_GROWN,
    STRETCH_SHRINKING,
};

// The resources of the actors, for each screen
enum {
    RES_MAIN_CHARS,
    RES_MAIN_PALETTE,
    RES_MAIN_CELL_ANIMS,
    RES_SUB_CHARS,
    RES_SUB_PALETTE,
    RES_SUB_CELL_ANIMS,
    RES_COUNT,
};

// The four digits of the time the cry has played, its point and its unit, and an animated icon on the top screen
#define ACTOR_COUNT 7
#define DIGIT_COUNT 4
#define ACTOR_ICON 6

// The longest length shown, 9.99 seconds
#define TIME_MAX 9990

// The cry's wave, drawn a column for every sample's half frame into a bitmap, and scrolled into a window of 31 by 15
// tiles of BG 3 two columns a frame
#define WAVE_BG 3
#define WAVE_WINDOW_WIDTH 31
#define WAVE_WINDOW_HEIGHT 15
#define WAVE_WIDTH (WAVE_WINDOW_WIDTH * 8)
// How much of the window shows, from its left edge, where the new columns are drawn
#define WAVE_SHOWN_WIDTH (WAVE_WIDTH - 10)
#define WAVE_HEIGHT (WAVE_WINDOW_HEIGHT * 8)
#define WAVE_COLUMNS 960
#define WAVE_STEP 2
// The middle line of the wave and the farthest it reaches either way
#define WAVE_CENTER 55
#define WAVE_COLOR 15

typedef struct {
    u8 x;
    u8 y;
    u8 anim;
    u8 priority;
    u8 bgPriority;
    // RES_*
    u8 chars;
    u8 palette;
    u8 cellAnims;
    u8 surface;
} ActorData;

typedef struct {
    ClActUnit *unit;
    Font *font;
    PrintQueue *printQueue;
    BmpWin *nameWindow;
    BOOL nameFlushPending;
    // The wave's window's screen, its scroll within a column and the column at its left edge
    u16 *waveScreen;
    s16 waveScroll;
    s16 waveColumn;
    BOOL waveScreenPending;
    BmpWin *waveWindow;
    u8 waveWait;
    // The Pokémon whose cry the wave shows
    u16 waveSpecies;
    u16 waveForm;
    // The whole wave, its width in columns, and how many columns of it have scrolled into the window
    GFLBitmap *waveBitmap;
    s16 waveWidth;
    s16 waveShown;
    const s8 *samples;
    // The cry's length in milliseconds
    u32 duration;
    // Two Pokémon, the one shown and the one loaded when the list moves
    u32 pokemonPalettes[2];
    u32 pokemonChars[2];
    u32 pokemonCellAnims[2];
    ClActor *pokemonActors[2];
    int pokemon;
    int stretch;
    u32 voice;
    BOOL playing;
    u64 startTime;
    // The time played in milliseconds, and the time that the digits show
    u16 time;
    u16 shownTime;
    u32 bgChars;
    ZukanDetailBackground *backgroundMain;
    ZukanDetailBackground *backgroundSub;
    u32 resources[RES_COUNT];
    ClActor *actors[ACTOR_COUNT];
    int bgm;
    TCB *vblankTcb;
    ZukanDetailBlend *blendMain;
    ZukanDetailBlend *blendSub;
    ZukanDetailPalFade *palFade;
    int exit;
    BOOL inputEnabled;
} ZukanDetailVoiceWork;

// Not referenced: eight bytes and eighteen floats, which may belong to an unused 3D setup
typedef struct {
    u8 unk0[4];
    u16 unk4;
    u16 unk6;
    f32 unk8[6][3];
} ZukanDetailVoiceUnkData;

static BOOL func_ov208_021c2560(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                  ZukanDetailCommon *common);
static BOOL func_ov208_021c270c(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                  ZukanDetailCommon *common);
static BOOL func_ov208_021c2644(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                  ZukanDetailCommon *common);
static void func_ov208_021c2a70(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                     ZukanDetailCommon *common, int command);
static void func_ov208_021c2ba8(TCB *tcb, void *data);
static void func_ov208_021c2be0(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c2bf4(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c2c1c(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common);
static void func_ov208_021c2c44(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                          ZukanDetailCommon *common);
static void func_ov208_021c2d88(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c2de0(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                           ZukanDetailCommon *common, u16 seconds, u16 hundredths);
static void func_ov208_021c2e6c(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                           ZukanDetailCommon *common, int index);
static void func_ov208_021c2fec(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                         ZukanDetailCommon *common, int index);
static void func_ov208_021c301c(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                           ZukanDetailCommon *common);
static void func_ov208_021c31cc(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                          ZukanDetailCommon *common);
static void func_ov208_021c3208(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c3218(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c3268(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                      ZukanDetailCommon *common);
static void func_ov208_021c3280(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common);
static void func_ov208_021c32fc(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common);
static void func_ov208_021c3318(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common);
static void func_ov208_021c3354(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c3374(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                      ZukanDetailCommon *common);
static void func_ov208_021c3378(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common, u16 seconds, u16 hundredths);
static void func_ov208_021c337c(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common);
static void func_ov208_021c3380(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c3480(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common);
static void func_ov208_021c34a4(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                           ZukanDetailCommon *common);
static void func_ov208_021c3514(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c35dc(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c3638(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                      ZukanDetailCommon *common);
static void func_ov208_021c3650(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c3678(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common);
static void func_ov208_021c36cc(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common);
static void func_ov208_021c3840(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk, ZukanDetailCommon *common,
                                      u16 species, u16 form);
static void func_ov208_021c3a24(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                            ZukanDetailCommon *common);
static void func_ov208_021c3aa8(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                             ZukanDetailCommon *common);
static void func_ov208_021c3b14(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                              ZukanDetailCommon *common);

// pokebw2's ZUKAN_DETAIL_VOICE_PROC_FUNCS
const ZukanDetailProcFuncs data_ov208_021c86f0 = {
    func_ov208_021c2560, func_ov208_021c270c, func_ov208_021c2644, func_ov208_021c2a70, NULL,
};

// pokebw2's sZukanDetailVoiceActors
static const ActorData data_ov208_021c8704[ACTOR_COUNT] = {
    { 200, 120, 28, 0, 1, RES_MAIN_CHARS, RES_MAIN_PALETTE, RES_MAIN_CELL_ANIMS, CLACT_SURFACE_MAIN },
    { 208, 120, 28, 0, 1, RES_MAIN_CHARS, RES_MAIN_PALETTE, RES_MAIN_CELL_ANIMS, CLACT_SURFACE_MAIN },
    { 224, 120, 28, 0, 1, RES_MAIN_CHARS, RES_MAIN_PALETTE, RES_MAIN_CELL_ANIMS, CLACT_SURFACE_MAIN },
    { 232, 120, 28, 0, 1, RES_MAIN_CHARS, RES_MAIN_PALETTE, RES_MAIN_CELL_ANIMS, CLACT_SURFACE_MAIN },
    { 216, 120, 29, 0, 1, RES_MAIN_CHARS, RES_MAIN_PALETTE, RES_MAIN_CELL_ANIMS, CLACT_SURFACE_MAIN },
    { 240, 120, 30, 0, 1, RES_MAIN_CHARS, RES_MAIN_PALETTE, RES_MAIN_CELL_ANIMS, CLACT_SURFACE_MAIN },
    { 128, 96, 0, 0, 3, RES_SUB_CHARS, RES_SUB_PALETTE, RES_SUB_CELL_ANIMS, CLACT_SURFACE_SUB },
};

const ZukanDetailVoiceUnkData ZUKAN_DETAIL_VOICE_UNK_DATA = {
    { 3, 2, 128, 128 },
    1,
    17,
    {
        { 16.0f, 0.0f, 64.0f },
        { -16.0f, -64.0f, 0.0f },
        { -64.0f, 0.0f, -16.0f },
        { 0.0f, 0.0f, 64.0f },
        { 0.0f, 16.0f, 64.0f },
        { 0.0f, 16.0f, 0.0f },
    },
};

static BOOL func_ov208_021c2560(ZukanDetailProcSys *sys, int *seq, void *param_, void *work,
                                  ZukanDetailCommon *common) {
    ZukanDetailVoiceParam *param = param_;
    ZukanDetailVoiceWork *wk = func_ov208_021bc32c(sys, sizeof(ZukanDetailVoiceWork), param->heapId);
    u8 i;

    MI_CpuFill8(wk, 0, sizeof(ZukanDetailVoiceWork));
    wk->unit = func_ov208_021bcf4c(func_ov208_021bc3a8(common));
    wk->font = func_ov208_021bc3ac(common);
    wk->printQueue = func_ov208_021bc3b0(common);
    wk->waveScreenPending = FALSE;
    wk->playing = FALSE;
    wk->time = 0;
    wk->shownTime = 1;
    wk->bgm = BGM_PLAYING;
    func_ov208_021c2be0(param, wk, common);
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov208_021c2ba8, wk, 1);
    wk->blendMain = func_ov208_021bc448(param->heapId);
    wk->blendSub = func_ov208_021bc448(param->heapId);
    func_ov208_021bc518(wk->blendMain);
    func_ov208_021bc518(wk->blendSub);
    func_ov208_021bc508(0, wk->blendMain);
    func_ov208_021bc508(1, wk->blendSub);
    wk->palFade = func_ov208_021bc610(param->heapId);
    wk->exit = VOICE_EXIT_NONE;
    wk->inputEnabled = TRUE;
    for (i = 0; i < 2; i++) {
        wk->pokemonActors[i] = NULL;
    }
    wk->pokemon = 0;
    return TRUE;
}

static BOOL func_ov208_021c2644(ZukanDetailProcSys *sys, int *seq, void *param_, void *work,
                                  ZukanDetailCommon *common) {
    ZukanDetailVoiceParam *param = param_;
    ZukanDetailVoiceWork *wk = work;
    u8 i;

    if (PokeVoice_IsPlayingAny()) {
        PokeVoice_ReleaseAll();
    }
    func_ov208_021c3638(param, wk, common);
    func_ov208_021bc97c(wk->backgroundSub);
    func_ov208_021bc97c(wk->backgroundMain);
    func_ov208_021bca54(2, wk->bgChars);
    for (i = 0; i < 2; i++) {
        func_ov208_021c2fec(param, wk, common, i);
    }
    func_ov208_021c32fc(param, wk, common);
    func_ov208_021c3268(param, wk, common);
    func_ov208_021c337c(param, wk, common);
    func_ov208_021c3374(param, wk, common);
    func_ov208_021c2d88(param, wk, common);
    func_ov208_021bc61c(wk->palFade);
    func_ov208_021bc464(wk->blendSub);
    func_ov208_021bc464(wk->blendMain);
    if (wk->bgm != BGM_RESTORED) {
        func_ov208_021c2bf4(param, wk, common);
    }
    GFL_TCBRemove(wk->vblankTcb);
    func_ov208_021bc34c(sys);
    return TRUE;
}

static BOOL func_ov208_021c270c(ZukanDetailProcSys *sys, int *seq, void *param_, void *work,
                                  ZukanDetailCommon *common) {
    ZukanDetailVoiceParam *param = param_;
    ZukanDetailVoiceWork *wk = work;
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
    ZukanDetailHeadbar *headbar = func_ov208_021bc3b8(common);

    switch (*seq) {
    case VOICE_SEQ_INIT: {
        u8 bg;

        *seq = VOICE_SEQ_FADE_IN;
        for (bg = 0; bg <= 7; bg++) {
            if (bg != 1 && bg != 5) {
                GFL_BGSysMoveBG(bg, BG_MOVE_SET_X, 0);
                GFL_BGSysMoveBG(bg, BG_MOVE_SET_Y, 0);
                GFL_BGSysClearBG(bg);
            }
        }
        GFL_BGSysSetBGPriority(2, 1);
        GFL_BGSysSetBGPriority(WAVE_BG, 2);
        GFL_BGSysSetBGPriority(0, 3);
        GFL_BGSysSetBGPriority(6, 1);
        GFL_BGSysSetBGPriority(4, 3);
        func_ov208_021c2c44(param, wk, common);
        func_ov208_021c2e6c(param, wk, common, wk->pokemon);
        func_0204b688(wk->pokemonActors[wk->pokemon], 1);
        func_ov208_021c3218(param, wk, common);
        func_ov208_021c3280(param, wk, common);
        func_ov208_021c3354(param, wk, common);
        func_ov208_021c3378(param, wk, common, 0, 0);
        wk->bgChars = func_ov208_021bc9bc(FALSE, param->heapId, 2, 1, 0, 5, ARCID_ZUKAN_GRA, 1, 11, 42, 0);
        wk->backgroundMain = func_ov208_021bc844(param->heapId, 2, 0, 2, 3);
        wk->backgroundSub = func_ov208_021bc844(param->heapId, 2, 4, 1, 2);
        func_ov208_021c2de0(param, wk, common, 0, 0);
        func_ov208_021c35dc(param, wk, common);
        func_ov208_021bc758(wk->palFade);
        func_ov208_021bc718(wk->palFade);
        break;
    }
    case VOICE_SEQ_FADE_IN:
        *seq = VOICE_SEQ_START_PALFADE;
        func_ov208_021bc4f4(0, wk->blendMain);
        func_ov208_021bc4f4(1, wk->blendSub);
        func_ov208_021c3aa8(param, wk, common);
        break;
    case VOICE_SEQ_START_PALFADE:
        *seq = VOICE_SEQ_WAIT_FADE_IN;
        func_ov208_021bc6d8(wk->palFade);
        if (func_ov208_021bdad0(touchbar) != ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021bdaac(touchbar, ZUKAN_DETAIL_TOUCHBAR_GENERAL, ZUKAN_DETAIL_PAGE_VOICE - 1,
                                        func_ov208_021bc3bc(common) > 1 ? TRUE : FALSE);
            func_ov208_021bdad4(touchbar, 0);
        } else {
            func_ov208_021bdbb8(touchbar, ZUKAN_DETAIL_PAGE_VOICE - 1);
        }
        func_ov208_021bdc48(touchbar, FALSE);
        func_ov208_021bdc14(
            touchbar, GameData_IsShortcutRegistered(func_ov208_021bc3a4(common), SHORTCUT_POKEDEX_VOICE));
        if (func_ov208_021be840(headbar) != ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021be82c(headbar, 2);
            func_ov208_021be844(headbar);
        }
        break;
    case VOICE_SEQ_WAIT_FADE_IN:
        if (!func_ov208_021bc6c8(wk->palFade) &&
            func_ov208_021bdad0(touchbar) == ZUKAN_DETAIL_TOUCHBAR_SHOWN &&
            func_ov208_021be840(headbar) == ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021bdba0(touchbar);
            func_ov208_021bdc48(touchbar, TRUE);
            *seq = VOICE_SEQ_MAIN;
        }
        break;
    case VOICE_SEQ_MAIN:
        if (wk->exit != VOICE_EXIT_NONE) {
            *seq = VOICE_SEQ_WAIT_FADE_OUT;
            func_ov208_021bc6f8(wk->palFade);
            func_ov208_021be854(headbar);
            if (wk->exit == VOICE_EXIT_SCREEN) {
                func_ov208_021bdae8(touchbar, 0);
            }
            func_ov208_021c2bf4(param, wk, common);
        } else {
            func_ov208_021c3514(param, wk, common);
        }
        break;
    case VOICE_SEQ_WAIT_FADE_OUT: {
        BOOL done = FALSE;

        if (!func_ov208_021bc6c8(wk->palFade) &&
            func_ov208_021be840(headbar) == ZUKAN_DETAIL_TOUCHBAR_HIDDEN) {
            if (wk->exit == VOICE_EXIT_SCREEN) {
                if (func_ov208_021bdad0(touchbar) == ZUKAN_DETAIL_TOUCHBAR_HIDDEN) {
                    done = TRUE;
                }
            } else {
                done = TRUE;
            }
        }
        if (done) {
            *seq = VOICE_SEQ_FADE_OUT;
        }
        break;
    }
    case VOICE_SEQ_FADE_OUT:
        *seq = VOICE_SEQ_EXIT;
        func_ov208_021c3b14(param, wk, common);
        func_ov208_021bc508(0, wk->blendMain);
        func_ov208_021bc508(1, wk->blendSub);
        break;
    case VOICE_SEQ_EXIT:
        return TRUE;
    }

    if (*seq >= VOICE_SEQ_WAIT_FADE_IN) {
        func_ov208_021c3380(param, wk, common);
        func_ov208_021c3650(param, wk, common);
        func_ov208_021c301c(param, wk, common);
    }
    if (*seq >= VOICE_SEQ_START_PALFADE) {
        func_ov208_021c3318(param, wk, common);
        func_ov208_021bc99c(wk->backgroundMain);
        func_ov208_021bc99c(wk->backgroundSub);
    }
    func_ov208_021bc46c(wk->blendMain, wk->blendSub);
    func_ov208_021bc678(wk->palFade);
    func_ov208_021c2c1c(param, wk, common);
    return FALSE;
}

static void func_ov208_021c2a70(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                     ZukanDetailCommon *common, int command) {
    ZukanDetailVoiceWork *wk = work;

    if (wk != NULL) {
        ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
        BOOL unlock = FALSE;

        // Input waits while the bar's icons play their animations
        switch (command) {
        case ZUKAN_DETAIL_CMD_CLOSE_TOUCH:
        case ZUKAN_DETAIL_CMD_RETURN_TOUCH:
        case ZUKAN_DETAIL_CMD_CUR_D_TOUCH:
        case ZUKAN_DETAIL_CMD_CUR_U_TOUCH:
        case ZUKAN_DETAIL_CMD_CHECK_TOUCH:
        case ZUKAN_DETAIL_CMD_INFO_TOUCH:
        case ZUKAN_DETAIL_CMD_MAP_TOUCH:
        case ZUKAN_DETAIL_CMD_FORM_TOUCH:
            wk->inputEnabled = FALSE;
            break;
        }
        switch (command) {
        case ZUKAN_DETAIL_CMD_CLOSE:
        case ZUKAN_DETAIL_CMD_RETURN:
        case ZUKAN_DETAIL_CMD_CUR_D:
        case ZUKAN_DETAIL_CMD_CUR_U:
        case ZUKAN_DETAIL_CMD_CHECK:
        case ZUKAN_DETAIL_CMD_INFO:
        case ZUKAN_DETAIL_CMD_MAP:
        case ZUKAN_DETAIL_CMD_FORM:
            unlock = TRUE;
            wk->inputEnabled = TRUE;
            break;
        }

        switch (command) {
        case ZUKAN_DETAIL_CMD_CUR_D_TOUCH: {
            u16 species = func_ov208_021bc3c0(common);

            func_ov208_021bc3cc(common);
            if (species != func_ov208_021bc3c0(common)) {
                func_ov208_021c34a4(param, wk, common);
            }
            break;
        }
        case ZUKAN_DETAIL_CMD_CUR_U_TOUCH: {
            u16 species = func_ov208_021bc3c0(common);

            func_ov208_021bc40c(common);
            if (species != func_ov208_021bc3c0(common)) {
                func_ov208_021c34a4(param, wk, common);
            }
            break;
        }
        }

        switch (command) {
        case ZUKAN_DETAIL_CMD_NONE:
            break;
        case ZUKAN_DETAIL_CMD_CLOSE:
        case ZUKAN_DETAIL_CMD_RETURN:
            wk->exit = VOICE_EXIT_SCREEN;
            break;
        case ZUKAN_DETAIL_CMD_INFO:
        case ZUKAN_DETAIL_CMD_MAP:
        case ZUKAN_DETAIL_CMD_FORM:
            wk->exit = VOICE_EXIT_PAGE;
            break;
        case ZUKAN_DETAIL_CMD_CUR_D:
            func_ov208_021bdba0(touchbar);
            break;
        case ZUKAN_DETAIL_CMD_CUR_U:
            func_ov208_021bdba0(touchbar);
            break;
        case ZUKAN_DETAIL_CMD_CHECK:
            GameData_SetKeyItemRegistration(func_ov208_021bc3a4(common), SHORTCUT_POKEDEX_VOICE,
                                            func_ov208_021bdc2c(touchbar));
            func_ov208_021bdba0(touchbar);
            break;
        default:
            if (unlock) {
                func_ov208_021bdba0(touchbar);
            }
            break;
        }
    }
}

// Scrolls the wave's BG to where its screen was moved to
// pokebw2's ZukanDetailVoice_VBlank
static void func_ov208_021c2ba8(TCB *tcb, void *data) {
    ZukanDetailVoiceWork *wk = data;

    if (wk->waveScreenPending) {
        GFL_BGSysMoveBG(WAVE_BG, BG_MOVE_SET_X, wk->waveScroll - 1);
        GFL_BGSysMoveBG(WAVE_BG, BG_MOVE_SET_Y, 0);
        GFL_BGSysLoadScr(WAVE_BG);
        wk->waveScreenPending = FALSE;
    }
    func_ov208_021bc6bc(wk->palFade);
}

static void func_ov208_021c2be0(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    GFL_SndBGMFadeOut(30);
    wk->bgm = BGM_FADING_OUT;
}

static void func_ov208_021c2bf4(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    if (wk->bgm == BGM_PAUSED) {
        GFL_SndBGMPop();
        GFL_SndBGMSetPaused(FALSE);
    }
    GFL_SndBGMFadeIn(60);
    wk->bgm = BGM_RESTORED;
}

// Pauses the music once it has faded out
static void func_ov208_021c2c1c(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common) {
    if (wk->bgm == BGM_FADING_OUT && !GFL_SndBGMIsFading()) {
        GFL_SndBGMSetPaused(TRUE);
        GFL_SndBGMPush();
        wk->bgm = BGM_PAUSED;
    }
}

static void func_ov208_021c2c44(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                          ZukanDetailCommon *common) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, param->heapId);
    ClActorSetup setup;
    u8 i;

    wk->resources[RES_MAIN_PALETTE] = func_0204ac18(arc, 3, CLACT_VRAM_MAIN, 0, 0, 4, param->heapId);
    wk->resources[RES_MAIN_CHARS] = func_0204a6c8(arc, 13, FALSE, CLACT_VRAM_MAIN, param->heapId);
    wk->resources[RES_MAIN_CELL_ANIMS] = func_0204af28(arc, 27, 44, param->heapId);
    wk->resources[RES_SUB_PALETTE] = func_0204ac18(arc, 8, CLACT_VRAM_SUB, 2 * 32, 0, 1, param->heapId);
    wk->resources[RES_SUB_CHARS] = func_0204a6c8(arc, 24, FALSE, CLACT_VRAM_SUB, param->heapId);
    wk->resources[RES_SUB_CELL_ANIMS] = func_0204af28(arc, 30, 47, param->heapId);
    GFL_ArcToolFree(arc);

    for (i = 0; i < ACTOR_COUNT; i++) {
        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        setup.x = data_ov208_021c8704[i].x;
        setup.y = data_ov208_021c8704[i].y;
        setup.sequence = data_ov208_021c8704[i].anim;
        setup.priority = data_ov208_021c8704[i].priority;
        setup.bgPriority = data_ov208_021c8704[i].bgPriority;
        wk->actors[i] = func_0204b294(wk->unit, wk->resources[data_ov208_021c8704[i].chars],
                                      wk->resources[data_ov208_021c8704[i].palette],
                                      wk->resources[data_ov208_021c8704[i].cellAnims], &setup,
                                      data_ov208_021c8704[i].surface, param->heapId);
        func_0204b3dc(wk->actors[i], TRUE);
        func_0204b688(wk->actors[i], 1);
    }
    func_0204b92c(wk->actors[ACTOR_ICON], TRUE);
}

static void func_ov208_021c2d88(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        func_0204b3b4(wk->actors[i]);
    }
    func_0204ada4(wk->resources[RES_MAIN_PALETTE]);
    func_0204a8d4(wk->resources[RES_MAIN_CHARS]);
    func_0204afd8(wk->resources[RES_MAIN_CELL_ANIMS]);
    func_0204ada4(wk->resources[RES_SUB_PALETTE]);
    func_0204a8d4(wk->resources[RES_SUB_CHARS]);
    func_0204afd8(wk->resources[RES_SUB_CELL_ANIMS]);
}

// Shows the time on the digits' frames, up to 99.99
static void func_ov208_021c2de0(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                           ZukanDetailCommon *common, u16 seconds, u16 hundredths) {
    u8 actors[DIGIT_COUNT] = { 0, 1, 2, 3 };
    u16 digits[DIGIT_COUNT];
    u8 i;

    digits[0] = seconds / 10;
    if (digits[0] > 9) {
        digits[0] = 9;
    }
    digits[1] = seconds % 10;
    digits[2] = hundredths / 10;
    if (digits[2] > 9) {
        digits[2] = 9;
    }
    digits[3] = hundredths % 10;
    for (i = 0; i < DIGIT_COUNT; i++) {
        func_0204b908(wk->actors[actors[i]], digits[i]);
    }
}

// Loads the Pokémon's sprite into one of the two slots, its palette into the palette fade's OBJ palette of the slot
static void func_ov208_021c2e6c(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                           ZukanDetailCommon *common, int index) {
    u16 species = func_ov208_021bc3c0(common);
    u32 personality = 0;
    PokeDexSave *pokedex = GameData_GetPokedex(func_ov208_021bc3a4(common));
    u32 sex;
    u32 rare;
    u32 form;
    ArcTool *arc;
    ClActorSetupEx setup;

    func_0200cc40(pokedex, species, &sex, &rare, &form, param->heapId);
    if (species == SPECIES_SPINDA) {
        personality = func_0200d288(pokedex, personality);
    }
    arc = MakePokeGraArcHandle(param->heapId);
    wk->pokemonChars[index] =
        func_0202d81c(arc, species, form, sex, rare, 0, 0, personality, CLACT_VRAM_SUB, param->heapId);
    wk->pokemonPalettes[index] =
        func_0202d7d8(arc, species, form, sex, rare, 0, 0, CLACT_VRAM_SUB, index * 32, param->heapId);
    wk->pokemonCellAnims[index] = func_0202d898(species, form, sex, rare, 0, 0, 2, CLACT_VRAM_SUB, param->heapId);
    func_ov208_021bc738(wk->palFade, arc,
                                   GetPokemonPaletteDataNo(GetPokemonGraphicsARCID(), species, form, sex, rare, 0, 0),
                                   param->heapId, 3, 32, index * 16, 0);
    GFL_ArcToolFree(arc);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetupEx));
    setup.base.x = 128;
    setup.base.y = 128;
    setup.base.sequence = 0;
    setup.base.priority = 0;
    setup.base.bgPriority = 0;
    setup.affineCenter.x = 0;
    setup.affineCenter.y = 0;
    setup.scaleX = FX32_ONE;
    setup.scaleY = FX32_ONE;
    setup.rotation = 0;
    setup.affineMode = 2;
    wk->pokemonActors[index] = func_0204b324(wk->unit, wk->pokemonChars[index], wk->pokemonPalettes[index],
                                             wk->pokemonCellAnims[index], &setup, CLACT_SURFACE_SUB, param->heapId);
    func_0204b688(wk->pokemonActors[index], 0);
    wk->stretch = STRETCH_NONE;
}

static void func_ov208_021c2fec(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                         ZukanDetailCommon *common, int index) {
    if (wk->pokemonActors[index] != NULL) {
        func_0204b3b4(wk->pokemonActors[index]);
        wk->pokemonActors[index] = NULL;
        func_0204ada4(wk->pokemonPalettes[index]);
        func_0204a8d4(wk->pokemonChars[index]);
        func_0204afd8(wk->pokemonCellAnims[index]);
    }
}

// Stretches the Pokémon up by a hundredth a frame to 1.1 times its height, or back down, keeping its feet in place
static void func_ov208_021c301c(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                           ZukanDetailCommon *common) {
    ClActorPos pos;
    ClActorScale scale;
    f32 scaleY;

    switch (wk->stretch) {
    case STRETCH_NONE:
        break;
    case STRETCH_GROWING:
        func_0204b454(wk->pokemonActors[wk->pokemon], &pos, CLACT_SURFACE_SUB);
        func_0204b5c0(wk->pokemonActors[wk->pokemon], &scale);
        scaleY = scale.y / (f32)FX32_ONE;
        scaleY += 0.01f;
        if (scaleY >= 1.1f) {
            scaleY = 1.1f;
            wk->stretch = STRETCH_GROWN;
        }
        pos.y = 128 - (int)((scaleY - 1.0f) * 96.0f / 2.0f);
        scale.y = FX32_CONST(scaleY);
        func_0204b404(wk->pokemonActors[wk->pokemon], &pos, CLACT_SURFACE_SUB);
        func_0204b5ac(wk->pokemonActors[wk->pokemon], &scale);
        break;
    case STRETCH_GROWN:
        break;
    case STRETCH_SHRINKING:
        func_0204b454(wk->pokemonActors[wk->pokemon], &pos, CLACT_SURFACE_SUB);
        func_0204b5c0(wk->pokemonActors[wk->pokemon], &scale);
        scaleY = scale.y / (f32)FX32_ONE;
        scaleY -= 0.01f;
        if (scaleY <= 1.0f) {
            scaleY = 1.0f;
            wk->stretch = STRETCH_NONE;
            pos.y = 128;
        } else {
            pos.y = 128 - (int)((scaleY - 1.0f) * 96.0f / 2.0f);
        }
        scale.y = FX32_CONST(scaleY);
        func_0204b404(wk->pokemonActors[wk->pokemon], &pos, CLACT_SURFACE_SUB);
        func_0204b5ac(wk->pokemonActors[wk->pokemon], &scale);
        break;
    }
}

static void func_ov208_021c31cc(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                          ZukanDetailCommon *common) {
    ClActorPos pos;
    ClActorScale scale;

    pos.x = 128;
    pos.y = 128;
    scale.x = FX32_ONE;
    scale.y = FX32_ONE;
    func_0204b404(wk->pokemonActors[wk->pokemon], &pos, CLACT_SURFACE_SUB);
    func_0204b5ac(wk->pokemonActors[wk->pokemon], &scale);
    wk->stretch = STRETCH_GROWING;
}

static void func_ov208_021c3208(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    if (wk->stretch == STRETCH_GROWING || wk->stretch == STRETCH_GROWN) {
        wk->stretch = STRETCH_SHRINKING;
    }
}

static void func_ov208_021c3218(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    GFL_G2DIOLoadNCLR(ARCID_FONT, 5, PALTYPE_SUB_BG, 0, 0, 0x20, param->heapId);
    wk->nameWindow = BmpWin_CreateDynamic(6, 10, 5, 12, 4, 0, 1);
    wk->nameFlushPending = FALSE;
    func_ov208_021c32fc(param, wk, common);
    BmpWin_FlushChar(wk->nameWindow);
}

static void func_ov208_021c3268(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                      ZukanDetailCommon *common) {
    wk->nameFlushPending = FALSE;
    func_0201c6ec(wk->printQueue);
    BmpWin_Free(wk->nameWindow);
}

static void func_ov208_021c3280(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common) {
    StrBuf *name = GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, func_ov208_021bc3c0(common));
    u16 width = GFL_FontGetBlockWidth(name, wk->font, 0);
    int windowWidth = GFL_BitmapGetWidth(BmpWin_GetBitmap(wk->nameWindow));
    u16 x = (windowWidth - width) / 2;

    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->nameWindow), x, 1, name, wk->font, PRINT_COLOR(15, 2, 0));
    GFL_StrBufFree(name);
    wk->nameFlushPending = TRUE;
    func_ov208_021c3318(param, wk, common);
}

static void func_ov208_021c32fc(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common) {
    wk->nameFlushPending = FALSE;
    func_0201c6ec(wk->printQueue);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->nameWindow), 0);
}

static void func_ov208_021c3318(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common) {
    if (wk->nameFlushPending && !func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(wk->nameWindow))) {
        BmpWin_Transfer(wk->nameWindow);
        wk->nameFlushPending = FALSE;
    }
}

// The time was once printed as text, which the digits' actors replaced: only the palette is left
static void func_ov208_021c3354(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    GFL_G2DIOLoadNCLR(ARCID_FONT, 5, PALTYPE_MAIN_BG, 0, 0x20, 0x20, param->heapId);
}

static void func_ov208_021c3374(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                      ZukanDetailCommon *common) {
}

static void func_ov208_021c3378(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common, u16 seconds, u16 hundredths) {
}

static void func_ov208_021c337c(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common) {
}

// Counts the time the cry has played, up to its length, and shows it rounded to hundredths of a second
static void func_ov208_021c3380(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    if (wk->playing) {
        u64 now = OS_GetTick();
        u64 elapsed;
        u64 time;

        if (now >= wk->startTime) {
            elapsed = now - wk->startTime;
        } else {
            elapsed = now + (0xffffffffffffffff - wk->startTime);
        }
        time = OS_TicksToMilliSeconds(elapsed);
        if (time > TIME_MAX) {
            time = TIME_MAX;
        }
        wk->time = time;
        if (wk->time >= wk->duration) {
            wk->time = wk->duration;
            wk->playing = FALSE;
            func_ov208_021c3208(param, wk, common);
        }
    }

    if (wk->time != wk->shownTime) {
        u16 seconds = wk->time / 1000;
        u16 milliseconds = wk->time % 1000;
        u16 hundredths = milliseconds / 10 + (milliseconds % 10 >= 5 ? 1 : 0);

        func_ov208_021c337c(param, wk, common);
        func_ov208_021c3378(param, wk, common, seconds, hundredths);
        func_ov208_021c2de0(param, wk, common, seconds, hundredths);
        wk->shownTime = wk->time;
    }
}

static void func_ov208_021c3480(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common) {
    if (wk->playing) {
        if (PokeVoice_IsPlayingAny()) {
            PokeVoice_ReleaseAll();
        }
        wk->playing = FALSE;
    }
    wk->time = 0;
}

// Loads the Pokémon the list moved to into the other slot
static void func_ov208_021c34a4(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                           ZukanDetailCommon *common) {
    if (wk->pokemonActors[wk->pokemon] != NULL) {
        func_0204b3dc(wk->pokemonActors[wk->pokemon], FALSE);
    }
    wk->pokemon = (wk->pokemon + 1) % 2;
    func_ov208_021c32fc(param, wk, common);
    func_ov208_021c2fec(param, wk, common, wk->pokemon);
    func_ov208_021c2e6c(param, wk, common, wk->pokemon);
    func_ov208_021c3280(param, wk, common);
    func_ov208_021c3480(param, wk, common);
    func_ov208_021c3678(param, wk, common);
}

// A or a touch above the touch bar plays the cry
static void func_ov208_021c3514(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    BOOL play = FALSE;

    if (wk->inputEnabled) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            func_020355d8(FALSE);
            play = TRUE;
        } else {
            u32 x;
            u32 y;

            if (func_02035d20(&x, &y) && x < 256 && y < 168) {
                play = TRUE;
                func_020355d8(TRUE);
            }
        }
    }

    if (play) {
        PokeDexSave *pokedex = GameData_GetPokedex(func_ov208_021bc3a4(common));
        u16 species = func_ov208_021bc3c0(common);
        u32 form;
        u32 sex;
        u32 rare;

        func_0200cc40(pokedex, species, &sex, &rare, &form, param->heapId);
        wk->voice = PokeVoice_Play(species, form, 64, 0, 0, 0, 0, NULL);
        wk->time = 0;
        wk->startTime = OS_GetTick();
        wk->playing = TRUE;
        func_ov208_021c3840(param, wk, common, species, form);
        func_ov208_021c31cc(param, wk, common);
    }
}

static void func_ov208_021c35dc(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    wk->waveScreen = GFL_HeapAllocate(param->heapId, WAVE_WINDOW_WIDTH * WAVE_WINDOW_HEIGHT * sizeof(u16), TRUE,
                                      "zukan_detail_voice.c", 2209);
    wk->waveWindow = BmpWin_CreateDynamic(WAVE_BG, 1, 2, WAVE_WINDOW_WIDTH, WAVE_WINDOW_HEIGHT, 1, 1);
    func_ov208_021c3678(param, wk, common);
    wk->waveBitmap = GFL_BitmapCreate(WAVE_COLUMNS / 8 + 1, WAVE_WINDOW_HEIGHT, 0x20, param->heapId);
}

static void func_ov208_021c3638(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                      ZukanDetailCommon *common) {
    GFL_BitmapFree(wk->waveBitmap);
    BmpWin_Free(wk->waveWindow);
    GFL_HeapFree(wk->waveScreen);
}

static void func_ov208_021c3650(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    if (wk->waveWait != 0) {
        wk->waveWait--;
    } else {
        func_ov208_021c36cc(param, wk, common);
        wk->waveWait = 0;
    }
}

// Clears the window to the middle line
static void func_ov208_021c3678(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                       ZukanDetailCommon *common) {
    GFLBitmap *bitmap;

    wk->waveWait = 0;
    wk->waveScroll = 0;
    wk->waveColumn = 0;
    wk->waveSpecies = 0;
    wk->waveForm = 0;
    wk->waveWidth = 0;
    wk->waveShown = wk->waveWidth;
    wk->duration = 0;
    bitmap = BmpWin_GetBitmap(wk->waveWindow);
    GFL_BitmapFill(bitmap, 0);
    GFL_BitmapFillArea(bitmap, 0, WAVE_CENTER, WAVE_SHOWN_WIDTH, 1, WAVE_COLOR);
    func_ov208_021c3a24(param, wk, common);
}

// Scrolls the window by two columns, and draws the wave's next two columns at its right edge, or the middle line once
// the wave has been shown.
static void func_ov208_021c36cc(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                        ZukanDetailCommon *common) {
    GFLBitmap *bitmap = BmpWin_GetBitmap(wk->waveWindow);
    s16 left = wk->waveScroll + wk->waveColumn * 8;
    s16 x = (left + WAVE_SHOWN_WIDTH) % WAVE_WIDTH;
    s16 drawn = 0;
    s16 end;
    int width;

    wk->waveScroll += WAVE_STEP;
    while (wk->waveScroll >= 8) {
        GFL_BitmapFillArea(bitmap, wk->waveColumn * 8, 0, 8, WAVE_HEIGHT, 0);
        wk->waveColumn = (wk->waveColumn + 1) % WAVE_WINDOW_WIDTH;
        wk->waveScroll -= 8;
    }
    if (wk->waveScroll > 0) {
        GFL_BitmapFillArea(bitmap, wk->waveColumn * 8, 0, wk->waveScroll, WAVE_HEIGHT, 0);
    }

    while (drawn < WAVE_STEP) {
        s16 rest = wk->waveWidth - wk->waveShown;

        end = x + WAVE_STEP - drawn;
        if (rest <= 0) {
            break;
        }
        if (end > WAVE_WIDTH) {
            end = WAVE_WIDTH;
        }
        if (end - x > rest) {
            end = x + rest;
        }
        width = end - x;
        GFL_BitmapCopyArea(wk->waveBitmap, bitmap, wk->waveShown, 0, x, 0, width, WAVE_HEIGHT, GFL_BITMAP_NO_COLOR_KEY);
        drawn += width;
        x = end % WAVE_WIDTH;
#ifdef BUGFIX
        wk->waveShown += width;
#else
        // BUG: Adds all the columns drawn this frame, so a frame whose two columns are split at the window's edge
        // skips a column of the wave
        wk->waveShown += drawn;
#endif
    }
    while (drawn < WAVE_STEP) {
        end = x + WAVE_STEP - drawn;
        if (end > WAVE_WIDTH) {
            end = WAVE_WIDTH;
        }
        width = end - x;
        GFL_BitmapFillArea(bitmap, x, WAVE_CENTER, width, 1, WAVE_COLOR);
        drawn += width;
        x = end % WAVE_WIDTH;
    }
    func_ov208_021c3a24(param, wk, common);
}

// Draws the cry's whole wave into the bitmap, a column of the samples' range for every half frame
static void func_ov208_021c3840(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk, ZukanDetailCommon *common,
                                      u16 species, u16 form) {
    u32 column;
    u32 end;
    u32 i;

    wk->samples = PokeVoice_GetSamples(wk->voice);
    wk->waveShown = 0;
    if (species == wk->waveSpecies && form == wk->waveForm) {
        return;
    }
    wk->waveSpecies = species;
    wk->waveForm = form;
    {
        int rate = PokeVoice_GetSampleRate(wk->voice);
        int speed = PokeVoice_GetSpeed(wk->voice);

        wk->duration = PokeVoice_GetSampleCount(wk->voice) * 32768.0f / rate / speed * 1000.0f + 0.5f;
    }

    GFL_BitmapFill(wk->waveBitmap, 0);
    column = 0;
    i = 0;
    while (i < PokeVoice_GetSampleCount(wk->voice)) {
        int speed = PokeVoice_GetSpeed(wk->voice);
        int rate = PokeVoice_GetSampleRate(wk->voice);

        end = (column + 1) * ((f32)rate * speed / 32768.0f / 60.0f / 2.0f);
        if (end > PokeVoice_GetSampleCount(wk->voice)) {
            end = PokeVoice_GetSampleCount(wk->voice);
        }
        if (column < WAVE_COLUMNS) {
            s16 min = 500;
            s16 max = -500;

            for (; i < end; i++) {
                s16 sample = wk->samples[i] * 47 * PokeVoice_GetVolume(wk->voice) / 128 / 128;

                sample = MATH_CLAMP(sample, -500, 500);
                if (sample < min) {
                    min = sample;
                }
                if (sample > max) {
                    max = sample;
                }
            }
            if (min <= max) {
                if (min < -WAVE_CENTER) {
                    min = -WAVE_CENTER;
                }
                if (max > WAVE_CENTER) {
                    max = WAVE_CENTER;
                }
                GFL_BitmapFillArea(wk->waveBitmap, column, WAVE_CENTER - max, 1, max - min + 1, WAVE_COLOR);
            }
        }
        column++;
        i = end + 1;
    }
    if (column > WAVE_COLUMNS) {
        column = WAVE_COLUMNS;
    }
    wk->waveWidth = column;
}

// Points the window's screen at its characters from the column at its left edge on, with BG 3 scrolled at VBlank
static void func_ov208_021c3a24(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                            ZukanDetailCommon *common) {
    u16 chars = func_02045768(wk->waveWindow);
    u16 n = 0;
    u16 y;
    u16 x;

    for (y = 0; y < WAVE_WINDOW_HEIGHT; y++) {
        for (x = 0; x < WAVE_WINDOW_WIDTH; x++) {
            wk->waveScreen[n] =
                (u16)(chars + y * WAVE_WINDOW_WIDTH + (x + wk->waveColumn) % WAVE_WINDOW_WIDTH) | (1 << 12);
            n++;
        }
    }
    GFL_BGSysLoadScrAreaAll(WAVE_BG, wk->waveScreen, 1, 2, WAVE_WINDOW_WIDTH, WAVE_WINDOW_HEIGHT);
    wk->waveScreenPending = TRUE;
    BmpWin_FlushChar(wk->waveWindow);
}

static void func_ov208_021c3aa8(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                             ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (wk->pokemonActors[i] != NULL) {
            func_0204b688(wk->pokemonActors[i], 0);
        }
    }
    for (i = 0; i < ACTOR_ICON; i++) {
        func_0204b688(wk->actors[i], 0);
    }
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG2,
                     GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 |
                         GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD,
                     16, 8);
    G2S_SetBlendAlpha(0,
                      GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 |
                          GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD,
                      12, 4);
}

static void func_ov208_021c3b14(ZukanDetailVoiceParam *param, ZukanDetailVoiceWork *wk,
                                              ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (wk->pokemonActors[i] != NULL) {
            func_0204b688(wk->pokemonActors[i], 1);
        }
    }
    for (i = 0; i < ACTOR_ICON; i++) {
        func_0204b688(wk->actors[i], 1);
    }
    func_ov208_021bc518(wk->blendMain);
    func_ov208_021bc518(wk->blendSub);
}
