// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov268/br_bvsearch_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's battle video search screen: the player chooses a battle mode, a Pokémon and whether the videos
// are of the player's own area, each on a page of the sub screen, with the choices on the main screen. The list
// screen then searches the Global Link with them. The name is a guess: the ROM gives none, and br_core.c runs it as
// BR_PROCID_BV_SEARCH

#include "types.h"
#include "app/battle_recorder/br_btn.h"
#include "app/battle_recorder/br_core.h"
#include "app/battle_recorder/br_fade.h"
#include "app/battle_recorder/br_net.h"
#include "app/battle_recorder/br_pokesearch.h"
#include "app/battle_recorder/br_proc_sys.h"
#include "app/battle_recorder/br_res.h"
#include "app/battle_recorder/br_util.h"
#include "constants/sound.h"
#include "field/unity_tower.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "pml/species_names.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_oam.h"
#include "system/country_region.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "system/wordset.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BrBallEffect *func_ov186_021fd760(ClActUnit *unit, BrRes *res, u32 display, HeapID heapId);
void func_ov186_021fbe54(BrRes *p_wk, u32 objID, HeapID heapId);

// The pages of the sub screen
enum {
    BR_BVSEARCH_PAGE_MENU,
    BR_BVSEARCH_PAGE_MODE,
    BR_BVSEARCH_PAGE_POKE,
    BR_BVSEARCH_PAGE_AREA,
    BR_BVSEARCH_PAGE_MAX,
};

// The next page when the screen ends instead
#define BR_BVSEARCH_PAGE_END (-1)

// The lines of the menu, which are the things to search by
enum {
    BR_BVSEARCH_MENU_MODE,
    BR_BVSEARCH_MENU_POKE,
    BR_BVSEARCH_MENU_AREA,
    BR_BVSEARCH_MENU_MAX,
};

// The menu's buttons
enum {
    BR_BVSEARCH_BTN_RETURN,
    BR_BVSEARCH_BTN_SEARCH,
    BR_BVSEARCH_BTN_MAX,
};

// The main screen's windows: a caption and the choice for each thing to search by
#define BR_BVSEARCH_MSGWIN_MAX (BR_BVSEARCH_MENU_MAX * 2)

#define BR_BVSEARCH_PRINT_COLOR 0x3da0

typedef struct {
    BrMsgWin *menuWin[BR_BVSEARCH_MENU_MAX];
    BrBtn *btn[BR_BVSEARCH_BTN_MAX];
    ListMenuOption *p_list_data;
    BrList *p_list;
    BrPokeSearch *p_search;
    BrMsgWin *msgWin[BR_BVSEARCH_MSGWIN_MAX];
    BrMsgWin *text;
    BrBallEffect *ballEff[2];
    BmpOamSys *bmpoam;
    PrintQueue *que;
    u32 page;
    int nextPage;
    HeapID heapId;
} BrBvSearchWork;

// A page of the sub screen. Each function returns TRUE once it is done
typedef BOOL (*BrBvSearchPageFunc)(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);

// Black 2's, not checked against Black
typedef struct {
    BrBvSearchPageFunc init;
    BrBvSearchPageFunc main;
    BrBvSearchPageFunc exit;
} BrBvSearchPageFuncs;

// Where a button is and its animation and message
// Black 2's, not checked against Black
typedef struct {
    u8 x;
    u8 y;
    u8 anmSeq;
    u16 msgID;
} BrBvSearchBtnData;

// Where a window is, in tiles, and its message
typedef struct {
    u8 x;
    u8 y;
    u8 w;
    u8 h;
    u32 msgID;
} BrBvSearchMsgWinData;

typedef struct {
    u32 left;
    u32 top;
    u32 right;
    u32 bottom;
} BrBvSearchRect;

static BOOL func_ov183_021df920(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021df9c8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021dfa10(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov183_021dfb90(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021dfc88(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021dfcd4(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
BOOL func_ov183_021dfe98(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021dff78(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021dffb0(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021e0044(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021e0080(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021e009c(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021e00d8(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021e0214(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static BOOL func_ov183_021e024c(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static void func_ov183_021e02f8(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static void func_ov183_021e037c(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param);
static void func_ov183_021e03b0(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param, u32 menu);
static u32 func_ov183_021e04c8(void);

// pokebw2's sBrBvSearchModeBtns
static const BrBvSearchBtnData data_ov183_021e3728[] = {
    { 80, 168, 0, 4 },
};

// pokebw2's sBrBvSearchAreaBtns
static const BrBvSearchBtnData data_ov183_021e372e[] = {
    { 80, 168, 0, 4 },
};

// pokebw2's sBrBvSearchMenuBtns
static const BrBvSearchBtnData data_ov183_021e3734[BR_BVSEARCH_BTN_MAX] = {
    { 32, 168, 0, 4 },
    { 128, 168, 1, 0x67 },
};

// pokebw2's sBrBvSearchMenuWins
static const BrBvSearchMsgWinData data_ov183_021e3740[BR_BVSEARCH_MENU_MAX] = {
#include "asset/br_bvsearch_proc_data_ov183_021e3740.inc"
};

// pokebw2's sBrBvSearchModeListParam
static const BrListParam data_ov183_021e3758 = {
    NULL, 0, 5, 3, 22, 12, 14, 4, 4, BR_LIST_TYPE_TOUCH, NULL, NULL, NULL, NULL, NULL,
};

// pokebw2's sBrBvSearchAreaListParam
static const BrListParam data_ov183_021e3780 = {
    NULL, 0, 5, 3, 22, 12, 14, 4, 4, BR_LIST_TYPE_TOUCH, NULL, NULL, NULL, NULL, NULL,
};

// pokebw2's sBrBvSearchMsgWins
static const BrBvSearchMsgWinData data_ov183_021e37d8[BR_BVSEARCH_MSGWIN_MAX] = {
#include "asset/br_bvsearch_proc_data_ov183_021e37d8.inc"
};

// pokebw2's sBrBvSearchPageFuncs
static const BrBvSearchPageFuncs data_ov183_021e37a8[BR_BVSEARCH_PAGE_MAX] = {
    { func_ov183_021dfb90, func_ov183_021dfcd4, func_ov183_021dfc88 },
    { func_ov183_021dfe98, func_ov183_021dffb0, func_ov183_021dff78 },
    { func_ov183_021e0044, func_ov183_021e009c, func_ov183_021e0080 },
    { func_ov183_021e00d8, func_ov183_021e024c, func_ov183_021e0214 },
};

// pokebw2's sBrBvSearchMenuRects
static const BrBvSearchRect data_ov183_021e3808[BR_BVSEARCH_MENU_MAX] = {
#include "asset/br_bvsearch_proc_data_ov183_021e3808.inc"
};

// pokebw2's BR_BVSEARCH_PROC_FUNCTIONS
const GameProcFunctions data_ov183_021e3838 = {
    func_ov183_021df920,
    func_ov183_021dfa10,
    func_ov183_021df9c8,
};

// pokebw2's BrBvSearch_ProcInit
static BOOL func_ov183_021df920(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvSearchProcParam *p_param = param;
    BrBvSearchWork *p_wk;
    int i;

    p_wk = GFL_ProcInitSubsystem(proc, sizeof(BrBvSearchWork), func_ov186_021f9b9c(p_param->procSys));
    MI_CpuFill8(p_wk, 0, sizeof(BrBvSearchWork));
    p_wk->heapId = func_ov186_021f9b9c(p_param->procSys);

    p_wk->que = func_0201c440(p_wk->heapId);
    p_wk->bmpoam = func_02025538(p_wk->heapId, p_param->unit);
    p_wk->text = func_ov186_021fd12c(p_param->res, p_wk->que, p_wk->heapId);
    func_ov186_021fd170(p_wk->text, p_param->res, 0x6b);
    for (i = 0; i < 2; i++) {
        p_wk->ballEff[i] = func_ov186_021fd760(p_param->unit, p_param->res, i, p_wk->heapId);
    }

    p_param->search.monsno = BR_NET_VIDEO_SEARCH_MONSNO_NONE;
    p_param->search.country = BR_NET_VIDEO_SEARCH_AREA_NONE;
    p_param->search.area = BR_NET_VIDEO_SEARCH_AREA_NONE;
    p_param->search.mode = BR_NET_VIDEO_SEARCH_MODE_NONE;

    func_ov186_021fbe54(p_param->res, 6, p_wk->heapId);
    func_ov183_021e02f8(p_wk, p_param);
    return TRUE;
}

// pokebw2's BrBvSearch_ProcExit
static BOOL func_ov183_021df9c8(GameProc *proc, u32 *state, void *param, void *work) {
    BrBvSearchProcParam *p_param = param;
    BrBvSearchWork *p_wk = work;
    int i;

    func_ov183_021e037c(p_wk, p_param);
    func_ov186_021fc0c8(p_param->res, 6);
    for (i = 0; i < 2; i++) {
        func_ov186_021fd820(p_wk->ballEff[i]);
    }
    func_ov186_021fd15c(p_wk->text, p_param->res);
    func_02025588(p_wk->bmpoam);
    func_0201c4c0(p_wk->que);
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// pokebw2's BrBvSearch_ProcMain
static BOOL func_ov183_021dfa10(GameProc *proc, u32 *state, void *param, void *work) {
    enum {
        SEQ_INIT,
        SEQ_FADEIN_START,
        SEQ_FADEIN_WAIT,
        SEQ_MAIN,
        SEQ_CHANGE_FADEOUT_START,
        SEQ_CHANGE_FADEOUT_WAIT,
        SEQ_CHANGE_EXIT,
        SEQ_CHANGE_INIT,
        SEQ_CHANGE_FADEIN_START,
        SEQ_CHANGE_FADEIN_WAIT,
        SEQ_FADEOUT_START,
        SEQ_FADEOUT_WAIT,
        SEQ_EXIT,
    };
    BrBvSearchProcParam *p_param = param;
    BrBvSearchWork *p_wk = work;
    int i;

    switch (*state) {
    case SEQ_INIT:
        if (data_ov183_021e37a8[p_wk->page].init(p_wk, p_param)) {
            (*state)++;
        }
        break;
    case SEQ_FADEIN_START:
        func_ov186_021fe784(p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_IN);
        (*state)++;
        break;
    case SEQ_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_param->fade)) {
            *state = SEQ_MAIN;
        }
        break;
    case SEQ_MAIN:
        if (data_ov183_021e37a8[p_wk->page].main(p_wk, p_param)) {
            if (p_wk->nextPage == BR_BVSEARCH_PAGE_END) {
                *state = SEQ_FADEOUT_START;
            } else {
                *state = SEQ_CHANGE_FADEOUT_START;
            }
        }
        break;
    case SEQ_CHANGE_FADEOUT_START:
        func_ov186_021fe784(p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_SUB, BR_FADE_DIR_OUT);
        (*state)++;
        break;
    case SEQ_CHANGE_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_param->fade)) {
            (*state)++;
        }
        break;
    case SEQ_CHANGE_EXIT:
        if (data_ov183_021e37a8[p_wk->page].exit(p_wk, p_param)) {
            p_wk->page = p_wk->nextPage;
            (*state)++;
        }
        break;
    case SEQ_CHANGE_INIT:
        if (data_ov183_021e37a8[p_wk->page].init(p_wk, p_param)) {
            (*state)++;
        }
        break;
    case SEQ_CHANGE_FADEIN_START:
        func_ov186_021fe784(p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_SUB, BR_FADE_DIR_IN);
        (*state)++;
        break;
    case SEQ_CHANGE_FADEIN_WAIT:
        if (func_ov186_021fe7b0(p_param->fade)) {
            *state = SEQ_MAIN;
        }
        break;
    case SEQ_FADEOUT_START:
        func_ov186_021fe784(p_param->fade, BR_FADE_TYPE_ALPHA_BG012, BR_FADE_DISPLAY_BOTH, BR_FADE_DIR_OUT);
        (*state)++;
        break;
    case SEQ_FADEOUT_WAIT:
        if (func_ov186_021fe7b0(p_param->fade)) {
            *state = SEQ_EXIT;
        }
        break;
    case SEQ_EXIT:
        if (data_ov183_021e37a8[p_wk->page].exit(p_wk, p_param)) {
            return TRUE;
        }
        break;
    }

    for (i = 0; i < BR_BVSEARCH_MENU_MAX; i++) {
        if (p_wk->menuWin[i] != NULL) {
            func_ov186_021fc830(p_wk->menuWin[i]);
        }
    }
    for (i = 0; i < BR_BVSEARCH_MSGWIN_MAX; i++) {
        if (p_wk->msgWin[i] != NULL) {
            func_ov186_021fc830(p_wk->msgWin[i]);
        }
    }
    if (p_wk->text != NULL) {
        func_ov186_021fd1bc(p_wk->text);
    }
    func_0201c4e4(p_wk->que);
    for (i = 0; i < 2; i++) {
        func_ov186_021fd854(p_wk->ballEff[i]);
    }
    return FALSE;
}

// The menu of the things to search by, with the return and search buttons
// pokebw2's BrBvSearch_MenuInit
static BOOL func_ov183_021dfb90(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    Font *font;
    MsgData *msg;
    ClActorSetup setup;
    BrResObjData obj;
    int i;

    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);
    func_ov186_021fb964(p_param->res, 0x1a, p_wk->heapId);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    func_ov186_021fc124(p_param->res, 6, &obj);
    for (i = 0; i < BR_BVSEARCH_BTN_MAX; i++) {
        setup.x = data_ov183_021e3734[i].x;
        setup.y = data_ov183_021e3734[i].y;
        setup.sequence = data_ov183_021e3734[i].anmSeq;
        setup.priority = 1;
        p_wk->btn[i] = func_ov186_021fb378(&setup, data_ov183_021e3734[i].msgID, 96, CLACT_SURFACE_SUB, p_param->unit,
                                    p_wk->bmpoam, font, msg, &obj, p_wk->heapId);
    }

    for (i = 0; i < BR_BVSEARCH_MENU_MAX; i++) {
        const BrBvSearchMsgWinData *data = &data_ov183_021e3740[i];

        p_wk->menuWin[i] = func_ov186_021fc6a4(4, data->x, data->y, data->w, data->h, 14, p_wk->que, p_wk->heapId);
        func_ov186_021fc754(p_wk->menuWin[i], msg, data->msgID, font, BR_BVSEARCH_PRINT_COLOR);
    }

    func_ov186_021fd170(p_wk->text, p_param->res, 0x6b);
    return TRUE;
}

// pokebw2's BrBvSearch_MenuExit
static BOOL func_ov183_021dfc88(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    int i;

    for (i = 0; i < BR_BVSEARCH_MENU_MAX; i++) {
        if (p_wk->menuWin[i] != NULL) {
            func_ov186_021fc734(p_wk->menuWin[i]);
            p_wk->menuWin[i] = NULL;
        }
    }
    for (i = 0; i < BR_BVSEARCH_BTN_MAX; i++) {
        if (p_wk->btn[i] != NULL) {
            func_ov186_021fb494(p_wk->btn[i]);
            p_wk->btn[i] = NULL;
        }
    }
    GFL_BGSysLoadScr(4);
    func_ov186_021fbda4(p_param->res, 0x1a);
    return TRUE;
}

// pokebw2's BrBvSearch_MenuMain
static BOOL func_ov183_021dfcd4(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    u32 menu = func_ov183_021e04c8();

    if (menu != BR_BVSEARCH_MENU_MAX) {
        switch (menu) {
        case BR_BVSEARCH_MENU_MODE: {
            u32 x;
            u32 y;
            BrPoint pos;

            func_02035d20(&x, &y);
            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            GFL_SndSEPlay(SEQ_SE_BREC_06);
            // Choosing a thing again clears it
            p_param->search.mode = BR_NET_VIDEO_SEARCH_MODE_NONE;
            func_ov183_021e03b0(p_wk, p_param, BR_BVSEARCH_MENU_MODE);
            func_ov186_021fd170(p_wk->text, p_param->res, 0x69);
            p_wk->nextPage = BR_BVSEARCH_PAGE_MODE;
            return TRUE;
        }
        case BR_BVSEARCH_MENU_POKE: {
            u32 x;
            u32 y;
            BrPoint pos;

            func_02035d20(&x, &y);
            pos.x = x;
            pos.y = y;
            func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
            GFL_SndSEPlay(SEQ_SE_BREC_06);
            p_param->search.monsno = BR_NET_VIDEO_SEARCH_MONSNO_NONE;
            func_ov183_021e03b0(p_wk, p_param, BR_BVSEARCH_MENU_POKE);
            func_ov186_021fd170(p_wk->text, p_param->res, 0x6a);
            p_wk->nextPage = BR_BVSEARCH_PAGE_POKE;
            return TRUE;
        }
        case BR_BVSEARCH_MENU_AREA: {
            PlayerInfo *player = GetGameDataPlayerInfo(p_param->gameData);
            u32 country = UnityTowerVisitor_GetCountry(player);
            u32 area = UnityTowerVisitor_GetProvince(player);

            if (country == 0 && area == 0) {
                // The player hasn't registered where they live
                func_ov186_021fd170(p_wk->text, p_param->res, 0x78);
                GFL_SndSEPlay(SEQ_SE_BREC_07);
            } else {
                u32 x;
                u32 y;
                BrPoint pos;

                func_02035d20(&x, &y);
                pos.x = x;
                pos.y = y;
                func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
                GFL_SndSEPlay(SEQ_SE_BREC_06);
                // The area is set or cleared here, without its page
                if (p_param->search.country == BR_NET_VIDEO_SEARCH_AREA_NONE) {
                    p_param->search.country = country;
                    p_param->search.area = area;
                } else {
                    p_param->search.country = BR_NET_VIDEO_SEARCH_AREA_NONE;
                    p_param->search.area = BR_NET_VIDEO_SEARCH_AREA_NONE;
                }
                func_ov183_021e03b0(p_wk, p_param, BR_BVSEARCH_MENU_AREA);
                return FALSE;
            }
            break;
        }
        }
    }

    {
        u32 x;
        u32 y;

        if (func_02035d20(&x, &y)) {
            if (func_ov186_021fb4b4(p_wk->btn[BR_BVSEARCH_BTN_RETURN], x, y)) {
                BrPoint pos;

                pos.x = x;
                pos.y = y;
                func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
                p_wk->nextPage = BR_BVSEARCH_PAGE_END;
                func_ov186_021f9ba0(p_param->procSys);
                return TRUE;
            }
            if (func_ov186_021fb4cc(p_wk->btn[BR_BVSEARCH_BTN_SEARCH], x, y)) {
                if (p_param->search.monsno != BR_NET_VIDEO_SEARCH_MONSNO_NONE ||
                    p_param->search.area != BR_NET_VIDEO_SEARCH_AREA_NONE ||
                    p_param->search.mode != BR_NET_VIDEO_SEARCH_MODE_NONE) {
                    BrPoint pos;

                    pos.x = x;
                    pos.y = y;
                    GFL_SndSEPlay(SEQ_SE_BREC_06);
                    func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
                    p_wk->nextPage = BR_BVSEARCH_PAGE_END;
                    func_ov186_021f9bf4(p_param->procSys, BR_PROCID_BV_RANK);
                    return TRUE;
                } else {
                    // Nothing to search by
                    GFL_SndSEPlay(SEQ_SE_BREC_07);
                    func_ov186_021fd170(p_wk->text, p_param->res, 0x6c);
                }
            }
        }
    }
    return FALSE;
}

// The list of the battle modes
// pokebw2's BrBvSearch_ModeInit
BOOL func_ov183_021dfe98(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    Font *font;
    MsgData *msg;
    BrListParam param;
    ClActorSetup setup;
    BrResObjData obj;
    int i;

    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);
    func_ov186_021fb964(p_param->res, 0x1b, p_wk->heapId);

    p_wk->p_list_data = ListMenuCore_CreateOptionList(BR_NET_VIDEO_SEARCH_MODE_NONE, p_wk->heapId);
    for (i = 0; i < BR_NET_VIDEO_SEARCH_MODE_NONE; i++) {
        ListMenuCore_AppendMsgOption(&p_wk->p_list_data[i], msg, 0xcd + i, i, p_wk->heapId);
    }

    param = data_ov183_021e3758;
    param.cp_list = (const BrListItem *)p_wk->p_list_data;
    param.list_max = BR_NET_VIDEO_SEARCH_MODE_NONE;
    param.res = p_param->res;
    param.unit = p_param->unit;
    p_wk->p_list = func_ov186_021fc8c8(&param, p_wk->heapId);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    func_ov186_021fc124(p_param->res, 6, &obj);
    for (i = 0; i < 1; i++) {
        setup.x = data_ov183_021e3728[i].x;
        setup.y = data_ov183_021e3728[i].y;
        setup.sequence = data_ov183_021e3728[i].anmSeq;
        setup.priority = 1;
        p_wk->btn[i] = func_ov186_021fb378(&setup, data_ov183_021e3728[i].msgID, 96, CLACT_SURFACE_SUB, p_param->unit,
                                    p_wk->bmpoam, font, msg, &obj, p_wk->heapId);
    }
    return TRUE;
}

// pokebw2's BrBvSearch_ModeExit
static BOOL func_ov183_021dff78(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    if (p_wk->btn[BR_BVSEARCH_BTN_RETURN] != NULL) {
        func_ov186_021fb494(p_wk->btn[BR_BVSEARCH_BTN_RETURN]);
        p_wk->btn[BR_BVSEARCH_BTN_RETURN] = NULL;
    }
    func_ov186_021fcc80(p_wk->p_list);
    p_wk->p_list = NULL;
    ListMenuCore_FreeOptionList(p_wk->p_list_data);
    p_wk->p_list_data = NULL;
    func_ov186_021fbda4(p_param->res, 0x1b);
    GFL_BGSysLoadScr(4);
    return TRUE;
}

// pokebw2's BrBvSearch_ModeMain
static BOOL func_ov183_021dffb0(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    u32 select;
    u32 x;
    u32 y;
    BOOL isReturn;

    func_ov186_021fcd0c(p_wk->p_list);
    select = func_ov186_021fcda8(p_wk->p_list);
    isReturn = func_02035d20(&x, &y);
    isReturn &= func_ov186_021fb4b4(p_wk->btn[BR_BVSEARCH_BTN_RETURN], x, y);

    if (select != BR_LIST_SELECT_NONE) {
        u32 x;
        u32 y;
        BrPoint pos;

        func_02035d20(&x, &y);
        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        p_param->search.mode = select;
        func_ov183_021e03b0(p_wk, p_param, BR_BVSEARCH_MENU_MODE);
        p_wk->nextPage = BR_BVSEARCH_PAGE_MENU;
        return TRUE;
    } else if (isReturn) {
        u32 x;
        u32 y;
        BrPoint pos;

        func_02035d20(&x, &y);
        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        p_wk->nextPage = BR_BVSEARCH_PAGE_MENU;
        return TRUE;
    }
    return FALSE;
}

// The Pokémon search
// pokebw2's BrBvSearch_PokeInit
static BOOL func_ov183_021e0044(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    p_wk->p_search = func_ov186_021ffbd4(GameData_GetPokedex(p_param->gameData), p_param->res, p_param->unit,
                                       p_wk->bmpoam, p_param->fade, p_wk->ballEff[CLACT_SURFACE_MAIN],
                                       p_wk->ballEff[CLACT_SURFACE_SUB], p_wk->heapId);
    func_ov186_021ffe5c(p_wk->p_search);
    return func_ov186_021ffe34(p_wk->p_search);
}

// pokebw2's BrBvSearch_PokeExit
static BOOL func_ov183_021e0080(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    func_ov186_021ffe70(p_wk->p_search);
    func_ov186_021ffc6c(p_wk->p_search);
    GFL_BGSysLoadScr(4);
    return TRUE;
}

// pokebw2's BrBvSearch_PokeMain
static BOOL func_ov183_021e009c(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    u32 select;
    u16 monsno;

    func_ov186_021ffc9c(p_wk->p_search);
    select = func_ov186_021ffe90(p_wk->p_search, &monsno);
    if (select != BR_POKESEARCH_SELECT_NONE) {
        if (select == BR_POKESEARCH_SELECT_DECIDE) {
            p_param->search.monsno = monsno;
            func_ov183_021e03b0(p_wk, p_param, BR_BVSEARCH_MENU_POKE);
            p_wk->nextPage = BR_BVSEARCH_PAGE_MENU;
        } else {
            p_wk->nextPage = BR_BVSEARCH_PAGE_MENU;
        }
        return TRUE;
    }
    return FALSE;
}

// The list of the areas, which only has the player's own
// pokebw2's BrBvSearch_AreaInit
static BOOL func_ov183_021e00d8(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    Font *font;
    MsgData *msg;
    StrBuf *str;
    PlayerInfo *player;
    u32 country;
    u32 area;
    WordSet *wordSet;
    StrBuf *src;
    BrListParam param;
    ClActorSetup setup;
    BrResObjData obj;
    int i;

    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);
    func_ov186_021fb964(p_param->res, 0x1c, p_wk->heapId);

    str = GFL_StrBufCreate(0x80, p_wk->heapId);
    player = GetGameDataPlayerInfo(p_param->gameData);
    country = UnityTowerVisitor_GetCountry(player);
    area = UnityTowerVisitor_GetProvince(player);
    wordSet = func_ov186_021fc21c(p_param->res);
    src = GFL_MsgDataLoadStrbufNew(msg, 0x7d);
    if (!CountryHasProvinces(country)) {
        loadCountryToStrbuf(wordSet, 0, country);
    } else {
        loadCountryAreaToStrbuf(wordSet, 0, country, area);
    }
    GFL_WordSetFormatStrbuf(wordSet, str, src);
    GFL_StrBufFree(src);

    p_wk->p_list_data = ListMenuCore_CreateOptionList(1, p_wk->heapId);
    ListMenuCore_AppendStrBufOption(p_wk->p_list_data, str, 0, p_wk->heapId);
    GFL_StrBufFree(str);

    param = data_ov183_021e3780;
    param.cp_list = (const BrListItem *)p_wk->p_list_data;
    param.list_max = 1;
    param.res = p_param->res;
    param.unit = p_param->unit;
    p_wk->p_list = func_ov186_021fc8c8(&param, p_wk->heapId);

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    func_ov186_021fc124(p_param->res, 6, &obj);
    for (i = 0; i < 1; i++) {
        setup.x = data_ov183_021e372e[i].x;
        setup.y = data_ov183_021e372e[i].y;
        setup.sequence = data_ov183_021e372e[i].anmSeq;
        setup.priority = 1;
        p_wk->btn[i] = func_ov186_021fb378(&setup, data_ov183_021e372e[i].msgID, 96, CLACT_SURFACE_SUB, p_param->unit,
                                    p_wk->bmpoam, font, msg, &obj, p_wk->heapId);
    }
    return TRUE;
}

// pokebw2's BrBvSearch_AreaExit
static BOOL func_ov183_021e0214(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    if (p_wk->btn[BR_BVSEARCH_BTN_RETURN] != NULL) {
        func_ov186_021fb494(p_wk->btn[BR_BVSEARCH_BTN_RETURN]);
        p_wk->btn[BR_BVSEARCH_BTN_RETURN] = NULL;
    }
    func_ov186_021fcc80(p_wk->p_list);
    p_wk->p_list = NULL;
    ListMenuCore_FreeOptionList(p_wk->p_list_data);
    p_wk->p_list_data = NULL;
    func_ov186_021fbda4(p_param->res, 0x1c);
    GFL_BGSysLoadScr(4);
    return TRUE;
}

// pokebw2's BrBvSearch_AreaMain
static BOOL func_ov183_021e024c(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    u32 select;
    u32 x;
    u32 y;
    BOOL isReturn;

    func_ov186_021fcd0c(p_wk->p_list);
    select = func_ov186_021fcda8(p_wk->p_list);
    isReturn = func_02035d20(&x, &y);
    isReturn &= func_ov186_021fb4b4(p_wk->btn[BR_BVSEARCH_BTN_RETURN], x, y);

    if (select != BR_LIST_SELECT_NONE) {
        u32 x;
        u32 y;
        BrPoint pos;
        PlayerInfo *player;
        u32 country;

        func_02035d20(&x, &y);
        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        player = GetGameDataPlayerInfo(p_param->gameData);
        country = UnityTowerVisitor_GetCountry(player);
        p_param->search.area = UnityTowerVisitor_GetProvince(player);
        p_param->search.country = country;
        func_ov183_021e03b0(p_wk, p_param, BR_BVSEARCH_MENU_AREA);
        p_wk->nextPage = BR_BVSEARCH_PAGE_MENU;
        return TRUE;
    } else if (isReturn) {
        u32 x;
        u32 y;
        BrPoint pos;

        func_02035d20(&x, &y);
        pos.x = x;
        pos.y = y;
        func_ov186_021fd860(p_wk->ballEff[CLACT_SURFACE_SUB], BR_BALL_EFFECT_SPREAD, &pos);
        p_wk->nextPage = BR_BVSEARCH_PAGE_MENU;
        return TRUE;
    }
    return FALSE;
}

// The main screen: the captions of the things to search by, and nothing chosen
// pokebw2's BrBvSearch_CreateMainDisplay
static void func_ov183_021e02f8(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    Font *font;
    MsgData *msg;
    int i;

    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);
    func_ov186_021fb964(p_param->res, 0x19, p_wk->heapId);

    for (i = 0; i < BR_BVSEARCH_MSGWIN_MAX; i++) {
        const BrBvSearchMsgWinData *data = &data_ov183_021e37d8[i];

        p_wk->msgWin[i] = func_ov186_021fc6a4(1, data->x, data->y, data->w, data->h, 14, p_wk->que, p_wk->heapId);
        func_ov186_021fc868(p_wk->msgWin[i], 0, 1, BR_MSGWIN_POS_ABSOLUTE);
        func_ov186_021fc754(p_wk->msgWin[i], msg, data->msgID, font, BR_BVSEARCH_PRINT_COLOR);
    }
}

// pokebw2's BrBvSearch_DeleteMainDisplay
static void func_ov183_021e037c(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param) {
    int i;

    for (i = 0; i < BR_BVSEARCH_MSGWIN_MAX; i++) {
        if (p_wk->msgWin[i] != NULL) {
            func_ov186_021fc734(p_wk->msgWin[i]);
            p_wk->msgWin[i] = NULL;
        }
    }
    func_ov186_021fbda4(p_param->res, 0x19);
    GFL_BGSysLoadScr(1);
}

// Prints the choice of a thing to search by on the main screen
// pokebw2's BrBvSearch_PrintMainDisplay
static void func_ov183_021e03b0(BrBvSearchWork *p_wk, BrBvSearchProcParam *p_param, u32 menu) {
    Font *font;
    MsgData *msg;

    font = func_ov186_021fc20c(p_param->res);
    msg = func_ov186_021fc214(p_param->res);

    switch (menu) {
    case BR_BVSEARCH_MENU_MODE:
        if (p_param->search.mode != BR_NET_VIDEO_SEARCH_MODE_NONE) {
            func_ov186_021fc754(p_wk->msgWin[1], msg, 0xcd + p_param->search.mode, font, BR_BVSEARCH_PRINT_COLOR);
        } else {
            func_ov186_021fc754(p_wk->msgWin[1], msg, 0x75, font, BR_BVSEARCH_PRINT_COLOR);
        }
        break;
    case BR_BVSEARCH_MENU_POKE:
        if (p_param->search.monsno != BR_NET_VIDEO_SEARCH_MONSNO_NONE) {
            func_ov186_021fc754(p_wk->msgWin[3], g_PMLSpeciesNamesResident, p_param->search.monsno, font,
                           BR_BVSEARCH_PRINT_COLOR);
        } else {
            func_ov186_021fc754(p_wk->msgWin[3], msg, 0x75, font, BR_BVSEARCH_PRINT_COLOR);
        }
        break;
    case BR_BVSEARCH_MENU_AREA:
        if (p_param->search.country == BR_NET_VIDEO_SEARCH_AREA_NONE &&
            p_param->search.area == BR_NET_VIDEO_SEARCH_AREA_NONE) {
            func_ov186_021fc754(p_wk->msgWin[5], msg, 0x75, font, BR_BVSEARCH_PRINT_COLOR);
        } else {
            WordSet *wordSet = func_ov186_021fc21c(p_param->res);
            StrBuf *src = GFL_MsgDataLoadStrbufNew(msg, 0x18);
            StrBuf *str = GFL_StrBufCreate(0x80, p_wk->heapId);

            if (!CountryHasProvinces(p_param->search.country)) {
                loadCountryToStrbuf(wordSet, 0, p_param->search.country);
            } else {
                loadCountryAreaToStrbuf(wordSet, 0, p_param->search.country, p_param->search.area);
            }
            GFL_WordSetFormatStrbuf(wordSet, str, src);
            func_ov186_021fc7c4(p_wk->msgWin[5], str, font, BR_BVSEARCH_PRINT_COLOR);
            GFL_StrBufFree(src);
            GFL_StrBufFree(str);
        }
        break;
    }
}

// The line of the menu that the touch is on, or BR_BVSEARCH_MENU_MAX
// pokebw2's BrBvSearch_GetTrgMenu
static u32 func_ov183_021e04c8(void) {
    u32 x;
    u32 y;
    int i;

    if (func_02035d20(&x, &y)) {
        for (i = 0; i < BR_BVSEARCH_MENU_MAX; i++) {
            const BrBvSearchRect *rect = &data_ov183_021e3808[i];

            if ((u32)(x - rect->left) <= (u32)(rect->right - rect->left) &&
                (u32)(y - rect->top) <= (u32)(rect->bottom - rect->top)) {
                return i;
            }
        }
    }
    return BR_BVSEARCH_MENU_MAX;
}
