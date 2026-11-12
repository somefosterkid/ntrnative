// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_info.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/ui_scene.h"
#include "app/zukan_detail.h"
#include "app/zukan_info.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nnsys/g2d.h"
#include "save/pokedex.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "nnsys/gfd.h"

// The detail screen's info page, which the ROM does not name: the Pokémon's entry on both screens, from overlay 206,
// with buttons for the languages it has been seen in, which show the entry on the sub screen in that language

#define LANGUAGE_BUTTON_COUNT 6
// No language button is pushed, or the entry is in the game's language
#define LANGUAGE_NONE LANGUAGE_BUTTON_COUNT

// The Pokédex's shortcut for the Y button, which the touch bar's check box registers
#define SHORTCUT_POKEDEX 17 // Black's

#define SPECIES_SPINDA 327

enum {
    INFO_SEQ_INIT,
    INFO_SEQ_FADE_IN,
    INFO_SEQ_WAIT_FADE_IN,
    INFO_SEQ_MAIN,
    INFO_SEQ_WAIT_FADE_OUT,
    INFO_SEQ_EXIT,
};

// How the page ends
enum {
    INFO_EXIT_NONE,
    INFO_EXIT_PAGE,
    INFO_EXIT_SCREEN,
};

// A language button's animation
enum {
    BUTTON_IDLE,
    BUTTON_PUSHED,
    BUTTON_WAIT_ANIM,
    BUTTON_DONE,
};

// Black 2's, not checked against Black
typedef struct {
    u8 x;
    u8 y;
    u8 rect[4];
    u8 anim;
    u8 pushedAnim;
    u8 language;
} LanguageButtonData;

typedef struct {
    ClActor *actor;
    // The touch area: x, y, width and height
    u8 rect[4];
    u16 anim;
    u16 pushedAnim;
    u8 language;
    int state;
} LanguageButton;

// Black 2's; twinned code confirms the offsets of infoMain, infoSub, buttons, pushed, language, chars, cellAnims,
// glowPalettes, glowPalette, glowPhase and glowStart in Black
typedef struct {
    TCB *vblankTcb;
    ZukanInfo *infoMain;
    ZukanInfo *infoSub;
    ZukanDetailBlend *blendMain;
    ZukanDetailBlend *blendSub;
    int exit;
    BOOL inputEnabled;
    LanguageButton buttons[LANGUAGE_BUTTON_COUNT];
    // The button being pushed, and the button of the language shown
    int pushed;
    int language;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    // The shown language's button glows between two palettes
    u16 glowPalettes[2][16];
    u16 glowPalette[16];
    int glowPhase;
    BOOL glowStart;
    Font *font;
    MsgData *msgData[3];
} ZukanDetailInfoWork;

static BOOL func_ov208_021bea04(ZukanDetailProcSys *sys, int *seq, void *param, void *work, ZukanDetailCommon *common);
static BOOL func_ov208_021bea90(ZukanDetailProcSys *sys, int *seq, void *param, void *work, ZukanDetailCommon *common);
BOOL func_ov208_021beb18(ZukanDetailProcSys *sys, int *seq, void *param_, void *work, ZukanDetailCommon *common);
static void func_ov208_021bee70(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                    ZukanDetailCommon *common, int command);
static void func_ov208_021bef9c(TCB *tcb, void *data);
BOOL func_ov208_021befa0(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common);
static BOOL func_ov208_021bf088(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common);
static void func_ov208_021bf1d0(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                          ZukanDetailCommon *common);
static void func_ov208_021bf280(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common,
                                        int language);
void func_ov208_021bf308(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                          ZukanDetailCommon *common);
static void func_ov208_021bf41c(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                        ZukanDetailCommon *common);
void func_ov208_021bf450(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common);
static void func_ov208_021bf4c4(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                         ZukanDetailCommon *common, const BOOL *visible);
static void func_ov208_021bf4fc(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common,
                                       u16 *species, u16 *form, u16 *sex, u16 *rare, u32 *personality, BOOL *caught,
                                       BOOL *languages);
void func_ov208_021bf5b8(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common);
static void func_ov208_021bf61c(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common);
static void func_ov208_021bf620(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common);
static void func_ov208_021bf63c(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common);
static void func_ov208_021bf664(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common);

// pokebw2's ZUKAN_DETAIL_INFO_PROC_FUNCS
const ZukanDetailProcFuncs data_ov208_021c8508 = {
    func_ov208_021bea04, func_ov208_021beb18, func_ov208_021bea90, func_ov208_021bee70, NULL,
};

// The buttons of Japanese, French, German, Italian, Spanish and Korean
// pokebw2's sZukanDetailInfoLanguageButtons
static const LanguageButtonData data_ov208_021c851c[LANGUAGE_BUTTON_COUNT] = {
#include "asset/zukan_detail_info_data_ov208_021c851c.inc"
};

// pokebw2's ZukanDetailInfo_InitParam
void func_ov208_021bea00(ZukanDetailInfoParam *param, HeapID heapId) {
    param->heapId = heapId;
}

static BOOL func_ov208_021bea04(ZukanDetailProcSys *sys, int *seq, void *param_, void *work,
                                 ZukanDetailCommon *common) {
    ZukanDetailInfoParam *param = param_;
    ZukanDetailInfoWork *wk;

    GFL_OvlLoad(OVERLAY_APP_UI);
    GFL_OvlLoad(OVERLAY_ID(206));
    wk = func_ov208_021bc32c(sys, sizeof(ZukanDetailInfoWork), param->heapId);
    MI_CpuFill8(wk, 0, sizeof(ZukanDetailInfoWork));
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov208_021bef9c, wk, 1);
    wk->pushed = LANGUAGE_NONE;
    wk->language = LANGUAGE_NONE;
    wk->glowStart = FALSE;
    wk->blendMain = func_ov208_021bc448(param->heapId);
    wk->blendSub = func_ov208_021bc448(param->heapId);
    func_ov208_021bc518(wk->blendMain);
    func_ov208_021bc518(wk->blendSub);
    func_ov208_021bc508(0, wk->blendMain);
    func_ov208_021bc508(1, wk->blendSub);
    wk->exit = INFO_EXIT_NONE;
    wk->inputEnabled = TRUE;
    return TRUE;
}

static BOOL func_ov208_021bea90(ZukanDetailProcSys *sys, int *seq, void *param_, void *work,
                                 ZukanDetailCommon *common) {
    ZukanDetailInfoParam *param = param_;
    ZukanDetailInfoWork *wk = work;

    func_ov208_021bf63c(param, wk, common);
    func_ov208_021bf61c(param, wk, common);
    func_ov206_021ba2cc(wk->infoSub);
    func_ov206_021ba2cc(wk->infoMain);
    func_ov208_021bf41c(param, wk, common);
    GFL_FontFree(wk->font);
    GFL_MsgDataFree(wk->msgData[2]);
    GFL_MsgDataFree(wk->msgData[1]);
    GFL_MsgDataFree(wk->msgData[0]);
    func_ov208_021bc464(wk->blendSub);
    func_ov208_021bc464(wk->blendMain);
    GFL_TCBRemove(wk->vblankTcb);
    func_ov208_021bc34c(sys);
    GFL_OvlUnload(OVERLAY_ID(206));
    GFL_OvlUnload(OVERLAY_APP_UI);
    return TRUE;
}

BOOL func_ov208_021beb18(ZukanDetailProcSys *sys, int *seq, void *param_, void *work,
                                 ZukanDetailCommon *common) {
    ZukanDetailInfoParam *param = param_;
    ZukanDetailInfoWork *wk = work;
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
    ZukanDetailHeadbar *headbar = func_ov208_021bc3b8(common);
    u8 bg;

    switch (*seq) {
    case INFO_SEQ_INIT: {
        PokeDexSave *pokedex;
        ClActUnit *unit;
        u32 national;
        u16 species;
        u16 form;
        u16 sex;
        u16 rare;
        u32 personality;
        BOOL caught;
        BOOL languages[LANGUAGE_BUTTON_COUNT];

        *seq = INFO_SEQ_FADE_IN;
        wk->font = GFL_FontCreate(ARCID_FONT, 0, 1, FALSE, param->heapId);
        wk->msgData[0] = GFL_MsgSysLoadData(TRUE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_SPECIES_CATEGORIES, param->heapId);
        wk->msgData[1] = GFL_MsgSysLoadData(TRUE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_POKEDEX_HEIGHTS, param->heapId);
        wk->msgData[2] = GFL_MsgSysLoadData(TRUE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_POKEDEX_WEIGHTS, param->heapId);
        for (bg = 0; bg <= 7; bg++) {
            if (bg != 1 && bg != 5) {
                GFL_BGSysMoveBG(bg, BG_MOVE_SET_X, 0);
                GFL_BGSysMoveBG(bg, BG_MOVE_SET_Y, 0);
                GFL_BGSysClearBG(bg);
            }
        }
        if (func_ov208_021bdad0(touchbar) != ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021bdaac(touchbar, 0, 0, func_ov208_021bc3bc(common) > 1);
            func_ov208_021bdad4(touchbar, 0);
        } else {
            func_ov208_021bdbb8(touchbar, 0);
        }
        func_ov208_021bdc48(touchbar, FALSE);
        func_ov208_021bdc14(touchbar,
                            GameData_IsShortcutRegistered(func_ov208_021bc3a4(common), SHORTCUT_POKEDEX));
        if (func_ov208_021be840(headbar) != ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021be82c(headbar, 0);
            func_ov208_021be844(headbar);
        }
        func_ov208_021bf308(param, wk, common);
        pokedex = GameData_GetPokedex(func_ov208_021bc3a4(common));
        unit = func_ov208_021bcf4c(func_ov208_021bc3a8(common));
        national = func_0200ca70(pokedex);
        func_ov208_021bf4fc(param, wk, common, &species, &form, &sex, &rare, &personality, &caught, languages);
        wk->infoMain = func_ov206_021b9e68(param->heapId, species, form, sex, rare, personality, national, caught, 2,
                                           0, 1, unit, wk->font, func_ov208_021bc3b0(common), wk->msgData[0],
                                           wk->msgData[1], wk->msgData[2]);
        wk->infoSub = func_ov206_021b9e68(param->heapId, species, form, sex, rare, personality, national, caught, 2,
                                          1, 1, unit, wk->font, func_ov208_021bc3b0(common), wk->msgData[0],
                                          wk->msgData[1], wk->msgData[2]);
        func_ov206_021ba7c8(wk->infoSub);
        func_ov208_021bf4c4(param, wk, common, languages);
        func_ov208_021bf5b8(param, wk, common);
        func_ov208_021bf620(param, wk, common);
        break;
    }
    case INFO_SEQ_FADE_IN:
        *seq = INFO_SEQ_WAIT_FADE_IN;
        func_ov208_021bc4d8(wk->blendMain);
        func_ov208_021bc4d8(wk->blendSub);
        break;
    case INFO_SEQ_WAIT_FADE_IN:
        if (!func_ov208_021bc4c8(wk->blendMain) && func_ov208_021bdad0(touchbar) == ZUKAN_DETAIL_TOUCHBAR_SHOWN &&
            func_ov208_021be840(headbar) == ZUKAN_DETAIL_TOUCHBAR_SHOWN) {
            func_ov208_021bdba0(touchbar);
            func_ov208_021bdc48(touchbar, TRUE);
            *seq = INFO_SEQ_MAIN;
        }
        break;
    case INFO_SEQ_MAIN:
        if (wk->exit != INFO_EXIT_NONE) {
            *seq = INFO_SEQ_WAIT_FADE_OUT;
            func_ov208_021bc4e4(wk->blendMain);
            func_ov208_021bc4e4(wk->blendSub);
            func_ov208_021be854(headbar);
            if (wk->exit == INFO_EXIT_SCREEN) {
                func_ov208_021bdae8(touchbar, 0);
            }
        } else if (wk->inputEnabled) {
            if (!func_ov208_021befa0(param, wk, common)) {
                func_ov208_021bf088(param, wk, common);
            }
        }
        break;
    case INFO_SEQ_WAIT_FADE_OUT:
        if (!func_ov208_021bc4c8(wk->blendMain) && !func_ov208_021bc4c8(wk->blendSub) &&
            func_ov208_021be840(headbar) == ZUKAN_DETAIL_TOUCHBAR_HIDDEN) {
            if (wk->exit == INFO_EXIT_SCREEN) {
                if (func_ov208_021bdad0(touchbar) == ZUKAN_DETAIL_TOUCHBAR_HIDDEN) {
                    *seq = INFO_SEQ_EXIT;
                }
            } else {
                *seq = INFO_SEQ_EXIT;
            }
        }
        break;
    case INFO_SEQ_EXIT:
        return TRUE;
    }

    if (*seq >= INFO_SEQ_WAIT_FADE_IN) {
        func_ov208_021bf450(param, wk, common);
        func_ov206_021ba374(wk->infoMain);
        func_ov206_021ba374(wk->infoSub);
        if (wk->pushed == LANGUAGE_NONE) {
            func_ov208_021bf664(param, wk, common);
        }
    }
    func_ov208_021bc46c(wk->blendMain, wk->blendSub);
    return FALSE;
}


static void func_ov208_021bee70(ZukanDetailProcSys *sys, int *seq, void *param, void *work,
                                    ZukanDetailCommon *common, int command) {
    ZukanDetailInfoWork *wk = work;

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
        case ZUKAN_DETAIL_CMD_MAP_TOUCH:
        case ZUKAN_DETAIL_CMD_VOICE_TOUCH:
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
        case ZUKAN_DETAIL_CMD_MAP:
        case ZUKAN_DETAIL_CMD_VOICE:
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
                func_ov208_021bf1d0(param, wk, common);
            }
            break;
        }
        case ZUKAN_DETAIL_CMD_CUR_U_TOUCH: {
            u16 species = func_ov208_021bc3c0(common);

            func_ov208_021bc40c(common);
            if (species != func_ov208_021bc3c0(common)) {
                func_ov208_021bf1d0(param, wk, common);
            }
            break;
        }
        }

        switch (command) {
        case ZUKAN_DETAIL_CMD_NONE:
            break;
        case ZUKAN_DETAIL_CMD_CLOSE:
        case ZUKAN_DETAIL_CMD_RETURN:
            wk->exit = INFO_EXIT_SCREEN;
            break;
        case ZUKAN_DETAIL_CMD_MAP:
        case ZUKAN_DETAIL_CMD_VOICE:
        case ZUKAN_DETAIL_CMD_FORM:
            wk->exit = INFO_EXIT_PAGE;
            break;
        case ZUKAN_DETAIL_CMD_CUR_D:
            func_ov208_021bdba0(touchbar);
            break;
        case ZUKAN_DETAIL_CMD_CUR_U:
            func_ov208_021bdba0(touchbar);
            break;
        case ZUKAN_DETAIL_CMD_CHECK:
            GameData_SetKeyItemRegistration(func_ov208_021bc3a4(common), SHORTCUT_POKEDEX,
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

// pokebw2's ZukanDetailInfo_VBlank
static void func_ov208_021bef9c(TCB *tcb, void *data) {
}

BOOL func_ov208_021befa0(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                  ZukanDetailCommon *common) {
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
    BOOL pushed = FALSE;

    if (wk->pushed == LANGUAGE_NONE) {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            u8 i;

            for (i = 0; i < LANGUAGE_BUTTON_COUNT; i++) {
                if (func_0204b3f4(wk->buttons[i].actor) && wk->buttons[i].rect[0] <= x &&
                    x < wk->buttons[i].rect[0] + wk->buttons[i].rect[2] && wk->buttons[i].rect[1] <= y &&
                    y < wk->buttons[i].rect[1] + wk->buttons[i].rect[3]) {
                    if (wk->language != i) {
                        wk->pushed = i;
                        pushed = TRUE;
                    }
                    break;
                }
            }
        }
    }

    if (pushed) {
        func_ov208_021bf280(param, wk, common, wk->pushed);
        wk->buttons[wk->pushed].state = BUTTON_PUSHED;
        func_0204b878(wk->buttons[wk->pushed].actor, wk->buttons[wk->pushed].pushedAnim);
        GFL_SndSEPlay(SEQ_SE_SELECT3);
        func_020355d8(TRUE);
        func_ov208_021bdc48(touchbar, FALSE);
    }
    return pushed;
}

// L and R move to the previous and next language seen, and with none chosen yet, A or either of them to the first
// pokebw2's ZukanDetailInfo_CheckKeys
static BOOL func_ov208_021bf088(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common) {
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
    BOOL pushed = FALSE;

    if (wk->pushed == LANGUAGE_NONE) {
        u32 typed = GCTX_HIDGetTypedKeys();
        u32 pressed = GCTX_HIDGetPressedKeys();
        u8 i;

        if (wk->language == LANGUAGE_NONE) {
            if ((typed & (PAD_BUTTON_L | PAD_BUTTON_R)) || (pressed & PAD_BUTTON_A)) {
                for (i = 0; i < LANGUAGE_BUTTON_COUNT; i++) {
                    if (func_0204b3f4(wk->buttons[i].actor)) {
                        wk->pushed = i;
                        pushed = TRUE;
                        break;
                    }
                }
            }
        } else if ((typed & PAD_BUTTON_R) || (pressed & PAD_BUTTON_A)) {
            for (i = 1; i < LANGUAGE_BUTTON_COUNT; i++) {
                u8 button = (wk->language + i) % LANGUAGE_BUTTON_COUNT;

                if (func_0204b3f4(wk->buttons[button].actor)) {
                    wk->pushed = button;
                    pushed = TRUE;
                    break;
                }
            }
        } else if (typed & PAD_BUTTON_L) {
            for (i = 1; i < LANGUAGE_BUTTON_COUNT; i++) {
                u8 button = (wk->language + LANGUAGE_BUTTON_COUNT - i) % LANGUAGE_BUTTON_COUNT;

                if (func_0204b3f4(wk->buttons[button].actor)) {
                    wk->pushed = button;
                    pushed = TRUE;
                    break;
                }
            }
        }
    }

    if (pushed) {
        func_ov208_021bf280(param, wk, common, wk->pushed);
        wk->buttons[wk->pushed].state = BUTTON_PUSHED;
        func_0204b878(wk->buttons[wk->pushed].actor, wk->buttons[wk->pushed].pushedAnim);
        GFL_SndSEPlay(SEQ_SE_SELECT3);
        func_020355d8(FALSE);
        func_ov208_021bdc48(touchbar, FALSE);
    }
    return pushed;
}

// Shows the Pokémon the list moved to, in the game's language unless it has been seen in the one shown
static void func_ov208_021bf1d0(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                          ZukanDetailCommon *common) {
    BOOL ownLanguage;
    u16 species;
    u16 form;
    u16 sex;
    u16 rare;
    u32 personality;
    BOOL caught;
    BOOL languages[LANGUAGE_BUTTON_COUNT];

    func_ov208_021bf4fc(param, wk, common, &species, &form, &sex, &rare, &personality, &caught, languages);
    ownLanguage = TRUE;
    func_ov206_021ba550(wk->infoMain, species, form, sex, rare, personality, caught);
    if (wk->language != LANGUAGE_NONE) {
        if (languages[wk->language]) {
            ownLanguage = FALSE;
        } else {
            func_ov208_021bf63c(param, wk, common);
            wk->language = LANGUAGE_NONE;
        }
    }
    if (ownLanguage) {
        func_ov206_021ba7c8(wk->infoSub);
    } else {
        func_ov206_021ba6b0(wk->infoSub, species, form, sex, rare, personality, caught, wk->language);
    }
    func_ov208_021bf4c4(param, wk, common, languages);
}

static void func_ov208_021bf280(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common,
                                        int language) {
    if (wk->language != language) {
        if (wk->language == LANGUAGE_NONE) {
            u16 species;
            u16 form;
            u16 sex;
            u16 rare;
            u32 personality;
            BOOL caught;
            BOOL languages[LANGUAGE_BUTTON_COUNT];

            func_ov208_021bf4fc(param, wk, common, &species, &form, &sex, &rare, &personality, &caught,
                                       languages);
            func_ov206_021ba6b0(wk->infoSub, species, form, sex, rare, personality, caught, language);
            func_ov206_021ba814(wk->infoSub);
        } else {
            func_ov206_021ba64c(wk->infoSub, language);
        }
        func_ov208_021bf63c(param, wk, common);
        wk->language = language;
        func_ov208_021bf620(param, wk, common);
    }
}

void func_ov208_021bf308(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                          ZukanDetailCommon *common) {
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);
    ClActUnit *unit = func_ov208_021bcf4c(func_ov208_021bc3a8(common));
    ClActSurface surface = CLACT_SURFACE_MAIN;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, param->heapId);
    ClActorSetup setup;
    u8 i;

    wk->palette = func_ov208_021bdc80(touchbar);
    wk->chars = func_0204a6c8(arc, 13, FALSE, CLACT_VRAM_MAIN, param->heapId);
    wk->cellAnims = func_0204af28(arc, 27, 44, param->heapId);
    GFL_ArcToolFree(arc);

    for (i = 0; i < LANGUAGE_BUTTON_COUNT; i++) {
        MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
        wk->buttons[i].rect[0] = data_ov208_021c851c[i].rect[0];
        wk->buttons[i].rect[1] = data_ov208_021c851c[i].rect[1];
        wk->buttons[i].rect[2] = data_ov208_021c851c[i].rect[2];
        wk->buttons[i].rect[3] = data_ov208_021c851c[i].rect[3];
        wk->buttons[i].anim = data_ov208_021c851c[i].anim;
        wk->buttons[i].pushedAnim = data_ov208_021c851c[i].pushedAnim;
        wk->buttons[i].language = data_ov208_021c851c[i].language;
        wk->buttons[i].state = BUTTON_IDLE;
        setup.x = data_ov208_021c851c[i].x;
        setup.y = data_ov208_021c851c[i].y;
        setup.sequence = wk->buttons[i].anim;
        wk->buttons[i].actor =
            func_0204b294(unit, wk->chars, wk->palette, wk->cellAnims, &setup, surface, param->heapId);
        func_0204b92c(wk->buttons[i].actor, TRUE);
        func_0204b688(wk->buttons[i].actor, 1);
    }
}

static void func_ov208_021bf41c(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                        ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < LANGUAGE_BUTTON_COUNT; i++) {
        func_0204b3b4(wk->buttons[i].actor);
    }
    func_0204a8d4(wk->chars);
    func_0204afd8(wk->cellAnims);
}

void func_ov208_021bf450(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                     ZukanDetailCommon *common) {
    ZukanDetailTouchbar *touchbar = func_ov208_021bc3b4(common);

    if (wk->pushed != LANGUAGE_NONE) {
        u8 i = wk->pushed;

        switch (wk->buttons[i].state) {
        case BUTTON_IDLE:
            break;
        case BUTTON_PUSHED:
            wk->buttons[i].state = BUTTON_WAIT_ANIM;
            break;
        case BUTTON_WAIT_ANIM:
            if (!func_0204b98c(wk->buttons[i].actor)) {
                wk->buttons[i].state = BUTTON_DONE;
            }
            break;
        case BUTTON_DONE:
            func_0204b878(wk->buttons[i].actor, wk->buttons[i].anim);
            wk->buttons[i].state = BUTTON_IDLE;
            wk->pushed = LANGUAGE_NONE;
            func_ov208_021bdc48(touchbar, TRUE);
            break;
        }
    }
}

// Shows the buttons of the languages the Pokémon has been seen in
static void func_ov208_021bf4c4(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                         ZukanDetailCommon *common, const BOOL *visible) {
    u8 i;

    for (i = 0; i < LANGUAGE_BUTTON_COUNT; i++) {
        func_0204b3dc(wk->buttons[i].actor, visible[i]);
        wk->buttons[i].state = BUTTON_IDLE;
        func_0204b878(wk->buttons[i].actor, wk->buttons[i].anim);
    }
    wk->pushed = LANGUAGE_NONE;
}

static void func_ov208_021bf4fc(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common,
                                       u16 *species, u16 *form, u16 *sex, u16 *rare, u32 *personality, BOOL *caught,
                                       BOOL *languages) {
    PokeDexSave *pokedex = GameData_GetPokedex(func_ov208_021bc3a4(common));
    u16 shown = func_ov208_021bc3c0(common);
    u32 shownForm;
    u32 shownSex;
    u32 shownRare;
    u32 shownPersonality = 0;
    BOOL shownCaught;
    BOOL seenLanguages[LANGUAGE_BUTTON_COUNT];
    u8 i;

    func_0200cc40(pokedex, shown, &shownSex, &shownRare, &shownForm, param->heapId);
    if (shown == SPECIES_SPINDA) {
        shownPersonality = func_0200d288(pokedex, shownPersonality);
    }
    shownCaught = PokeDex_IsCaught(pokedex, shown);
    for (i = 0; i < LANGUAGE_BUTTON_COUNT; i++) {
        seenLanguages[i] = PokeDex_HasLanguage(pokedex, shown, wk->buttons[i].language);
    }

    *species = shown;
    *form = shownForm;
    *sex = shownSex;
    *rare = shownRare;
    *personality = shownPersonality;
    *caught = shownCaught;
    for (i = 0; i < LANGUAGE_BUTTON_COUNT; i++) {
        languages[i] = seenLanguages[i];
    }
}

// Reads the two palettes that the shown language's button glows between
// pokebw2's ZukanDetailInfo_InitGlow
void func_ov208_021bf5b8(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common) {
    NNSG2dPaletteData *palette;
    void *file = GFL_G2DIOReadNCLR(ARCID_ZUKAN_GRA, 3, &palette, param->heapId);
    u16 *data = palette->pRawData;

    MI_CpuCopy8(data + 16, wk->glowPalettes[0], 32);
    MI_CpuCopy8(data + 48, wk->glowPalettes[1], 32);
    GFL_HeapFree(file);
    wk->glowPhase = 0;
    MI_CpuCopy8(wk->glowPalettes[0], wk->glowPalette, 32);
    NNS_GfdRegisterNewVramTransferTask(14, 6 * 32, wk->glowPalette, 32);
}

// pokebw2's ZukanDetailInfo_FreeGlow
static void func_ov208_021bf61c(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common) {
}

// pokebw2's ZukanDetailInfo_StartGlow
static void func_ov208_021bf620(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common) {
    if (wk->language != LANGUAGE_NONE) {
        wk->glowPhase = 0;
        wk->glowStart = TRUE;
    }
}

// pokebw2's ZukanDetailInfo_StopGlow
static void func_ov208_021bf63c(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk, ZukanDetailCommon *common) {
    if (wk->language != LANGUAGE_NONE) {
        func_0204b6f4(wk->buttons[wk->language].actor, 0, 0);
        wk->glowStart = FALSE;
    }
}

// Blends the glowing palette between the two palettes with a cosine, and loads it into OBJ palette 6
static void func_ov208_021bf664(ZukanDetailInfoParam *param, ZukanDetailInfoWork *wk,
                                       ZukanDetailCommon *common) {
    s16 ratio;
    u8 i;

    if (wk->glowStart) {
        if (wk->language != LANGUAGE_NONE) {
            func_0204b6f4(wk->buttons[wk->language].actor, 2, 0);
        }
        wk->glowStart = FALSE;
    }

    if (wk->glowPhase + 0x400 >= 0x10000) {
        wk->glowPhase = wk->glowPhase + 0x400 - 0x10000;
    } else {
        wk->glowPhase += 0x400;
    }
    ratio = (FX_CosIdx(wk->glowPhase) + FX32_ONE) / 2;

    for (i = 0; i < 16; i++) {
        u16 from = wk->glowPalettes[0][i];
        u16 to = wk->glowPalettes[1][i];
        u8 r0 = from & 0x1f;
        u8 g0 = (from & 0x3e0) >> 5;
        u8 b0 = (from & 0x7c00) >> 10;
        u8 r1 = to & 0x1f;
        u8 g1 = (to & 0x3e0) >> 5;
        u8 b1 = (to & 0x7c00) >> 10;
        u8 r = r0 + (((r1 - r0) * ratio) >> FX32_SHIFT);
        u8 g = g0 + (((g1 - g0) * ratio) >> FX32_SHIFT);
        u8 b = b0 + (((b1 - b0) * ratio) >> FX32_SHIFT);

        wk->glowPalette[i] = r | (g << 5) | (b << 10);
    }
    NNS_GfdRegisterNewVramTransferTask(14, 6 * 32, wk->glowPalette, 32);
}
