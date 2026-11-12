// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov300/zknsearch_seq.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/ui/ui_scene.h"
#include "app/zknsearch_bmp.h"
#include "app/zknsearch_list.h"
#include "app/zknsearch_main.h"
#include "app/zknsearch_obj.h"
#include "app/zknsearch_seq.h"
#include "app/zknsearch_ui.h"
#include "app/zukan_common.h"
#include "app/zukan_search_engine.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "save/pokedex.h"
#include "system/blink_palanm.h"
#include "system/cursor_move.h"
#include "system/game_data.h"
#include "system/palanm.h"
#include "app/zukansearch.h"

// The sequences of the Pokédex's search screen: the main page with the criteria, a page with a list for each
// criterion, and the search. The ROM doesn't name this file; zknsearch_seq.c is a guess after zknsearch_main.c

enum {
    SEQ_INIT,
    SEQ_EXIT,
    SEQ_WAIT_FADE,
    SEQ_MAIN_INIT,
    SEQ_MAIN,
    SEQ_PAGE_END,
    SEQ_RESET,
    SEQ_SEARCH,
    SEQ_SEARCH_RESULT,
    SEQ_ORDER_INIT,
    SEQ_ORDER,
    SEQ_ORDER_EXIT,
    SEQ_INITIAL_INIT,
    SEQ_INITIAL,
    SEQ_INITIAL_EXIT,
    SEQ_TYPE_INIT,
    SEQ_TYPE,
    SEQ_TYPE_EXIT,
    SEQ_COLOR_INIT,
    SEQ_COLOR,
    SEQ_COLOR_EXIT,
    SEQ_SHAPE_INIT,
    SEQ_SHAPE,
    SEQ_SHAPE_EXIT,
    SEQ_LIST_SCROLL,
    SEQ_BUTTON_ANM,
    SEQ_FINISH,
    SEQ_END,
};

// How long the search takes at least, and when its sound stops
#define SEARCH_FRAMES 121
#define SEARCH_SOUND_FRAMES 100

// No button to wait for while the list jumps
#define BUTTON_NONE ZKNSEARCH_OBJ_COUNT

typedef int (*ZknSearchSeqFunc)(ZknSearchWork *wk);

int func_ov210_021bcf8c(ZknSearchWork *wk);
int func_ov210_021bd064(ZknSearchWork *wk);
static int func_ov210_021bd100(ZknSearchWork *wk);
static int func_ov210_021bd118(ZknSearchWork *wk);
static int func_ov210_021bd19c(ZknSearchWork *wk);
static int func_ov210_021bd36c(ZknSearchWork *wk);
static int func_ov210_021bd388(ZknSearchWork *wk);
int func_ov210_021bd3ac(ZknSearchWork *wk);
static int func_ov210_021bd4ac(ZknSearchWork *wk);
static int func_ov210_021bd500(ZknSearchWork *wk);
static int func_ov210_021bd568(ZknSearchWork *wk);
static int func_ov210_021bd640(ZknSearchWork *wk);
static int func_ov210_021bd658(ZknSearchWork *wk);
static int func_ov210_021bd6c4(ZknSearchWork *wk);
static int func_ov210_021bd898(ZknSearchWork *wk);
static int func_ov210_021bd8b0(ZknSearchWork *wk);
static int func_ov210_021bd920(ZknSearchWork *wk);
static int func_ov210_021bdb1c(ZknSearchWork *wk);
static int func_ov210_021bdb34(ZknSearchWork *wk);
static int func_ov210_021bdba4(ZknSearchWork *wk);
static int func_ov210_021bdd78(ZknSearchWork *wk);
static int func_ov210_021bdd90(ZknSearchWork *wk);
static int func_ov210_021bde00(ZknSearchWork *wk);
static int func_ov210_021bdfa8(ZknSearchWork *wk);
static int func_ov210_021bdfc0(ZknSearchWork *wk);
static int func_ov210_021be010(ZknSearchWork *wk);
static int func_ov210_021be0c4(ZknSearchWork *wk);
static void func_ov210_021be0e4(ZknSearchWork *wk, u32 row);
static void func_ov210_021be110(ZknSearchWork *wk);
static void func_ov210_021be12c(ZknSearchWork *wk, u32 row);
static void func_ov210_021be178(ZknSearchWork *wk);
static void func_ov210_021be190(ZknSearchWork *wk, u32 row);
static void func_ov210_021be20c(ZknSearchWork *wk);
static void func_ov210_021be22c(ZknSearchWork *wk, u32 row);
static void func_ov210_021be278(ZknSearchWork *wk);
static void func_ov210_021be290(ZknSearchWork *wk, u32 row);
static void func_ov210_021be2dc(ZknSearchWork *wk);
static int func_ov210_021be2f4(ZknSearchWork *wk, u32 isActor, u32 id, int next);
static int func_ov210_021be314(ZknSearchWork *wk, int next);
static void func_ov210_021be324(ZknSearchWork *wk);

// pokebw2's sZknSearchSeqFuncs
static const ZknSearchSeqFunc data_ov210_021c0894[] = {
    func_ov210_021bcf8c,         func_ov210_021bd064,      func_ov210_021bd100,    func_ov210_021bd118,
    func_ov210_021bd19c,     func_ov210_021bd36c,   func_ov210_021bd388,       func_ov210_021bd3ac,
    func_ov210_021bd4ac, func_ov210_021bd500, func_ov210_021bd568,       func_ov210_021bd640,
    func_ov210_021bd658,  func_ov210_021bd6c4,   func_ov210_021bd898, func_ov210_021bd8b0,
    func_ov210_021bd920,         func_ov210_021bdb1c,  func_ov210_021bdb34,   func_ov210_021bdba4,
    func_ov210_021bdd78,    func_ov210_021bdd90, func_ov210_021bde00,       func_ov210_021bdfa8,
    func_ov210_021bdfc0,   func_ov210_021be010, func_ov210_021be0c4,
};

// pokebw2's ZknSearchSeq_Main
BOOL func_ov210_021bcf50(ZknSearchWork *wk) {
    wk->seq = data_ov210_021c0894[wk->seq](wk);
    if (wk->seq == SEQ_END) {
        return FALSE;
    }
    func_ov210_021bed68(wk);
    func_ov210_021be3d4(wk);
    func_ov212_021c93e4(2, 6, &wk->scrollTimer);
    return TRUE;
}

int func_ov210_021bcf8c(ZknSearchWork *wk) {
    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetDisplayLayout(0);
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    if (func_0200ca70(wk->param->pokedex) == 1) {
        wk->param->criteria->regional = 0;
    } else {
        wk->param->criteria->regional = 1;
    }
    func_ov210_021bc3d4();
    func_ov210_021bc3f0();
    func_ov210_021bc5dc();
    func_ov210_021bc940(wk);
    func_ov210_021be368(wk);
    func_ov210_021bed2c(wk);
    func_ov210_021bc9d0(wk);
    func_0203d34c(TRUE, HEAPID_ZUKAN_SEARCH);
    func_ov210_021bca20(wk);
    func_ov210_021bcda4(wk);
    func_ov210_021bc790(wk);
    func_ov210_021bc82c(wk, 16, 16);
    func_ov210_021bcbb8(wk);
    func_ov210_021bc8ec();
    func_ov210_021bc294(wk);
    func_ov210_021bc318(wk);
    wk->page = ZKNSEARCH_NONE;
    func_ov210_021bcf0c(wk);
    return SEQ_MAIN_INIT;
}


int func_ov210_021bd064(ZknSearchWork *wk) {
    if (func_ov210_021bce58(wk) == TRUE || func_02021ed8(wk->paletteFade) != 0) {
        return SEQ_EXIT;
    }
    func_ov210_021bc330(wk);
    func_ov210_021bc2ac(wk);
    func_ov210_021bc92c();
    func_ov210_021bcc1c(wk);
    func_ov210_021bca78(wk);
    func_ov210_021bc800(wk);
    func_ov210_021bca10(wk);
    func_ov210_021bed54(wk);
    func_ov210_021be3b4(wk);
    func_ov210_021bc99c(wk);
    func_ov210_021bc578();
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GFL_OvlUnload(OVERLAY_ID(67));
    return SEQ_END;
}


// pokebw2's ZknSearchSeq_WaitFade
static int func_ov210_021bd100(ZknSearchWork *wk) {
    if (func_02021ed8(wk->paletteFade) == 0) {
        return wk->buttonNextSeq;
    }
    return SEQ_WAIT_FADE;
}

// pokebw2's ZknSearchSeq_MainInit
static int func_ov210_021bd118(ZknSearchWork *wk) {
    if (func_02021ed8(wk->paletteFade) != 0) {
        return SEQ_MAIN_INIT;
    }
    func_ov210_021bc8b4(TRUE);
    func_ov210_021bca84(wk);
    func_ov210_021be564(wk);
    func_ov210_021bf418(wk);
    func_ov210_021bc5b4();
    func_ov210_021bcd30(wk);
    if (wk->page == ZKNSEARCH_NONE) {
        func_ov210_021bfeac(wk, ZKNSEARCH_UI_ORDER);
        GX_SetMasterBrightness(0);
        GXS_SetMasterBrightness(0);
        func_ov210_021bce0c(wk, -8);
        func_ov210_021bc82c(wk, 16, 0);
        return SEQ_MAIN;
    }
    func_ov210_021bfeac(wk, wk->page);
    return SEQ_MAIN;
}

// pokebw2's ZknSearchSeq_MainPage
static int func_ov210_021bd19c(ZknSearchWork *wk) {
    if (func_ov210_021bce58(wk) == TRUE || func_02021ed8(wk->paletteFade) != 0) {
        return SEQ_MAIN;
    }
    switch (func_ov210_021bff68(wk)) {
    case ZKNSEARCH_UI_ORDER:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov210_021be2f4(wk, FALSE, ZKNSEARCH_UI_ORDER, func_ov210_021be314(wk, SEQ_ORDER_INIT));
    case ZKNSEARCH_UI_INITIAL:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov210_021be2f4(wk, FALSE, ZKNSEARCH_UI_INITIAL,
                                         func_ov210_021be314(wk, SEQ_INITIAL_INIT));
    case ZKNSEARCH_UI_TYPE:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov210_021be2f4(wk, FALSE, ZKNSEARCH_UI_TYPE, func_ov210_021be314(wk, SEQ_TYPE_INIT));
    case ZKNSEARCH_UI_COLOR:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov210_021be2f4(wk, FALSE, ZKNSEARCH_UI_COLOR, func_ov210_021be314(wk, SEQ_COLOR_INIT));
    case ZKNSEARCH_UI_SHAPE:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov210_021be2f4(wk, FALSE, ZKNSEARCH_UI_SHAPE, func_ov210_021be314(wk, SEQ_SHAPE_INIT));
    case ZKNSEARCH_UI_START:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov210_021be2f4(wk, FALSE, ZKNSEARCH_UI_START, SEQ_SEARCH);
    case ZKNSEARCH_UI_RESET:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov210_021be2f4(wk, FALSE, ZKNSEARCH_UI_RESET, SEQ_RESET);
    case ZKNSEARCH_UI_SHORTCUT:
        GFL_SndSEPlay(SEQ_SE_SYS_07);
        func_ov210_021be324(wk);
        break;
    case ZKNSEARCH_UI_EXIT:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        wk->param->result = ZUKAN_SEARCH_RESULT_CLOSE;
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_EXIT, 8);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_EXIT, SEQ_FINISH);
    case ZKNSEARCH_UI_RETURN:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov210_021bcf38(wk);
        wk->param->result = ZUKAN_SEARCH_RESULT_CANCEL;
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_FINISH);
    case CURSOR_MOVE_CURSOR_ON:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_MOVED:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov210_021bcf38(wk);
        wk->param->result = ZUKAN_SEARCH_RESULT_CANCEL;
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_FINISH);
    }
    func_0202ea40(wk->blinkAnm);
    return SEQ_MAIN;
}

// pokebw2's ZknSearchSeq_PageEnd
static int func_ov210_021bd36c(ZknSearchWork *wk) {
    func_ov210_021bcd3c(wk);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE, FALSE);
    func_ov210_021bff18(wk);
    return wk->nextSeq;
}

// pokebw2's ZknSearchSeq_Reset
static int func_ov210_021bd388(ZknSearchWork *wk) {
    func_ov212_021c9380(wk->param->pokedex, wk->param->criteria);
    func_ov210_021be564(wk);
    func_ov210_021bedf0(wk, ZKNSEARCH_OBJ_SHAPE, FALSE);
    return SEQ_MAIN;
}

int func_ov210_021bd3ac(ZknSearchWork *wk) {
    u16 *list;
    u16 count;

    if (wk->timer == 0) {
        GFL_SndSEPlay(SEQ_SE_SYS_78);
        func_ov210_021bcc2c(wk);
        func_ov210_021becf8(wk);
        func_ov210_021be820(wk);
    }
    if (wk->timer == SEARCH_SOUND_FRAMES) {
        GFL_SndStop();
    }
    if (wk->timer == 1) {
        func_ov210_021bcf38(wk);
        wk->engine = func_ov212_021c9428(wk->param->pokedex, wk->param->criteria, HEAPID_ZUKAN_SEARCH | HEAPID_TAIL_BIT);
        wk->searchDone = FALSE;
    }
    if (wk->timer >= 2 && wk->searchDone == FALSE) {
        wk->searchDone = func_ov212_021c9468(wk->engine, 0x69, &count, &list);
        if (wk->searchDone == TRUE) {
            wk->param->count = count;
            wk->param->list = list;
            func_ov212_021c9460(wk->engine);
        }
    }
    if (wk->timer == SEARCH_FRAMES) {
        if (wk->searchDone == TRUE) {
            if (wk->param->count != 0) {
                GFL_SndSEPlay(SEQ_SE_SYS_84);
                func_ov210_021bed04(wk);
            } else {
                GFL_SndSEPlay(SEQ_SE_BEEP);
                func_ov210_021bed10(wk);
            }
            wk->timer = 0;
            return SEQ_SEARCH_RESULT;
        }
    } else {
        func_ov210_021bfe84(wk, wk->timer);
        wk->timer++;
    }
    return SEQ_SEARCH;
}


// pokebw2's ZknSearchSeq_SearchResult
static int func_ov210_021bd4ac(ZknSearchWork *wk) {
    if (func_ov210_021c00fc(wk) == TRUE) {
        if (wk->param->count != 0) {
            wk->param->result = ZUKAN_SEARCH_RESULT_FOUND;
            return SEQ_FINISH;
        }
        func_ov210_021bcca8(wk);
        func_ov210_021be7e4(wk);
        if (func_020355c4() == TRUE) {
            func_020260f4(wk->cursorMove, FALSE);
        } else {
            func_020260f4(wk->cursorMove, TRUE);
        }
        func_ov210_021bcf0c(wk);
        return SEQ_MAIN;
    }
    return SEQ_SEARCH_RESULT;
}

// pokebw2's ZknSearchSeq_OrderInit
static int func_ov210_021bd500(ZknSearchWork *wk) {
    switch (wk->initSeq) {
    case 0:
        wk->page = ZKNSEARCH_PAGE_ORDER;
        func_ov210_021bc8b4(TRUE);
        func_ov210_021be83c(wk);
        func_ov210_021c0234(wk);
        wk->initSeq++;
    case 1:
        if (func_ov067_021b7a64(wk->frameList) == FALSE) {
            func_ov210_021bf480(wk);
            func_ov210_021bcac8(wk);
            func_ov210_021bc5c8();
            func_ov210_021bcd30(wk);
            wk->initSeq = 0;
            return SEQ_ORDER;
        }
        break;
    }
    return SEQ_ORDER_INIT;
}

// pokebw2's ZknSearchSeq_Order
static int func_ov210_021bd568(ZknSearchWork *wk) {
    u32 ret = func_ov067_021b7ab0(wk->frameList);

    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (func_020355c4() == FALSE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be0e4(wk, ret);
        }
        break;
    case FRAMELIST_NONE:
        ret = func_ov210_021c014c(wk);
        switch (ret) {
        case ZKNSEARCH_UI_LIST_RETURN:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_ORDER_EXIT);
        case ZKNSEARCH_UI_LIST_CANCEL:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_ORDER_EXIT);
        case ZKNSEARCH_UI_LIST_RESET:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be110(wk);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be0e4(wk, ret - ZKNSEARCH_UI_LIST_ROW);
            break;
        }
        break;
    }
    return SEQ_ORDER;
}

// pokebw2's ZknSearchSeq_OrderExit
static int func_ov210_021bd640(ZknSearchWork *wk) {
    func_ov210_021bcd3c(wk);
    func_ov210_021bf38c(wk);
    func_ov210_021c0550(wk);
    return SEQ_MAIN_INIT;
}

// pokebw2's ZknSearchSeq_InitialInit
static int func_ov210_021bd658(ZknSearchWork *wk) {
    switch (wk->initSeq) {
    case 0:
        wk->page = ZKNSEARCH_PAGE_INITIAL;
        func_ov210_021bc8b4(TRUE);
        func_ov210_021be8f8(wk);
        func_ov210_021c02dc(wk);
        wk->initSeq++;
    case 1:
        if (func_ov067_021b7a64(wk->frameList) == FALSE) {
            func_ov210_021bf728(wk, FALSE);
            func_ov210_021bf4a4(wk);
            func_ov210_021bcaf8(wk);
            func_ov210_021bc5c8();
            func_ov210_021bcd30(wk);
            wk->initSeq = 0;
            return SEQ_INITIAL;
        }
        break;
    }
    return SEQ_INITIAL_INIT;
}

// pokebw2's ZknSearchSeq_Initial
static int func_ov210_021bd6c4(ZknSearchWork *wk) {
    int seq = SEQ_INITIAL;
    u32 ret = func_ov067_021b7ab0(wk->frameList);

    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (func_020355c4() == FALSE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be12c(wk, ret);
        }
        break;
    case -12:
        break;
    case -11:
    case -10:
    case -9:
    case -8:
        func_ov210_021bf728(wk, FALSE);
        break;
    case -7:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 12);
        wk->buttonId = ZKNSEARCH_OBJ_PAGE_UP;
        wk->subSeq = 0;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case -6:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 13);
        wk->buttonId = ZKNSEARCH_OBJ_PAGE_DOWN;
        wk->subSeq = 0;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case -5:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 12);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_PAGE_UP, seq);
    case -4:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 13);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_PAGE_DOWN, seq);
    case -3:
    case -2:
        func_ov210_021bf81c(wk);
        func_ov210_021bfabc(wk, 0, wk->param->criteria->initial);
        wk->buttonId = BUTTON_NONE;
        wk->subSeq = 1;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case FRAMELIST_NONE:
        ret = func_ov210_021c014c(wk);
        switch (ret) {
        case ZKNSEARCH_UI_LIST_RETURN:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_INITIAL_EXIT);
        case ZKNSEARCH_UI_LIST_CANCEL:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_INITIAL_EXIT);
        case ZKNSEARCH_UI_LIST_RESET:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be178(wk);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021bf728(wk, FALSE);
            func_ov210_021be12c(wk, ret - ZKNSEARCH_UI_LIST_ROW);
            break;
        }
        break;
    }
    func_ov210_021bf6e0(wk);
    return seq;
}

// pokebw2's ZknSearchSeq_InitialExit
static int func_ov210_021bd898(ZknSearchWork *wk) {
    func_ov210_021bcd3c(wk);
    func_ov210_021bf38c(wk);
    func_ov210_021c0550(wk);
    return SEQ_MAIN_INIT;
}

// pokebw2's ZknSearchSeq_TypeInit
static int func_ov210_021bd8b0(ZknSearchWork *wk) {
    switch (wk->initSeq) {
    case 0:
        wk->page = ZKNSEARCH_PAGE_TYPE;
        func_ov210_021bc8b4(TRUE);
        func_ov210_021be9d4(wk);
        func_ov210_021c0384(wk);
        wk->initSeq++;
    case 1:
        if (func_ov067_021b7a64(wk->frameList) == FALSE) {
            func_ov210_021bf728(wk, FALSE);
            func_ov210_021bf500(wk);
            func_ov210_021bcb28(wk);
            func_ov210_021bc5c8();
            func_ov210_021bcd30(wk);
            wk->initSeq = 0;
            return SEQ_TYPE;
        }
        break;
    }
    return SEQ_TYPE_INIT;
}

// pokebw2's ZknSearchSeq_Type
static int func_ov210_021bd920(ZknSearchWork *wk) {
    int seq = SEQ_TYPE;
    u32 ret = func_ov067_021b7ab0(wk->frameList);

    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (func_020355c4() == FALSE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be190(wk, ret);
        }
        break;
    case -12:
        break;
    case -11:
    case -10:
    case -9:
    case -8:
        func_ov210_021bf728(wk, FALSE);
        break;
    case -7:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 12);
        wk->buttonId = ZKNSEARCH_OBJ_PAGE_UP;
        wk->subSeq = 0;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case -6:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 13);
        wk->buttonId = ZKNSEARCH_OBJ_PAGE_DOWN;
        wk->subSeq = 0;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case -5:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 12);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_PAGE_UP, seq);
    case -4:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 13);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_PAGE_DOWN, seq);
    case -3:
    case -2:
        func_ov210_021bf81c(wk);
        func_ov210_021bfabc(wk, 0, func_ov210_021bcef0(wk, wk->param->criteria->type1));
        func_ov210_021bfabc(wk, 1, func_ov210_021bcef0(wk, wk->param->criteria->type2));
        wk->buttonId = BUTTON_NONE;
        wk->subSeq = 1;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case FRAMELIST_NONE:
        ret = func_ov210_021c014c(wk);
        switch (ret) {
        case ZKNSEARCH_UI_LIST_RETURN:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_TYPE_EXIT);
        case ZKNSEARCH_UI_LIST_CANCEL:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_TYPE_EXIT);
        case ZKNSEARCH_UI_LIST_RESET:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be20c(wk);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021bf728(wk, FALSE);
            func_ov210_021be190(wk, ret - ZKNSEARCH_UI_LIST_ROW);
            break;
        }
        break;
    }
    func_ov210_021bf6e0(wk);
    return seq;
}

// pokebw2's ZknSearchSeq_TypeExit
static int func_ov210_021bdb1c(ZknSearchWork *wk) {
    func_ov210_021bcd3c(wk);
    func_ov210_021bf38c(wk);
    func_ov210_021c0550(wk);
    return SEQ_MAIN_INIT;
}

// pokebw2's ZknSearchSeq_ColorInit
static int func_ov210_021bdb34(ZknSearchWork *wk) {
    switch (wk->initSeq) {
    case 0:
        wk->page = ZKNSEARCH_PAGE_COLOR;
        func_ov210_021bc8b4(TRUE);
        func_ov210_021beb0c(wk);
        func_ov210_021c0434(wk);
        wk->initSeq++;
    case 1:
        if (func_ov067_021b7a64(wk->frameList) == FALSE) {
            func_ov210_021bf728(wk, FALSE);
            func_ov210_021bf55c(wk);
            func_ov210_021bcb58(wk);
            func_ov210_021bc5c8();
            func_ov210_021bcd30(wk);
            wk->initSeq = 0;
            return SEQ_COLOR;
        }
        break;
    }
    return SEQ_COLOR_INIT;
}

// pokebw2's ZknSearchSeq_Color
static int func_ov210_021bdba4(ZknSearchWork *wk) {
    int seq = SEQ_COLOR;
    u32 ret = func_ov067_021b7ab0(wk->frameList);

    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (func_020355c4() == FALSE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be22c(wk, ret);
        }
        break;
    case -12:
        break;
    case -11:
    case -10:
    case -9:
    case -8:
        func_ov210_021bf728(wk, FALSE);
        break;
    case -7:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 12);
        wk->buttonId = ZKNSEARCH_OBJ_PAGE_UP;
        wk->subSeq = 0;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case -6:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 13);
        wk->buttonId = ZKNSEARCH_OBJ_PAGE_DOWN;
        wk->subSeq = 0;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case -5:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 12);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_PAGE_UP, seq);
    case -4:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 13);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_PAGE_DOWN, seq);
    case -3:
    case -2:
        func_ov210_021bf81c(wk);
        func_ov210_021bfabc(wk, 0, wk->param->criteria->color);
        wk->buttonId = BUTTON_NONE;
        wk->subSeq = 1;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case FRAMELIST_NONE:
        ret = func_ov210_021c014c(wk);
        switch (ret) {
        case ZKNSEARCH_UI_LIST_RETURN:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_COLOR_EXIT);
        case ZKNSEARCH_UI_LIST_CANCEL:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_COLOR_EXIT);
        case ZKNSEARCH_UI_LIST_RESET:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be278(wk);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021bf728(wk, FALSE);
            func_ov210_021be22c(wk, ret - ZKNSEARCH_UI_LIST_ROW);
            break;
        }
        break;
    }
    func_ov210_021bf6e0(wk);
    return seq;
}

// pokebw2's ZknSearchSeq_ColorExit
static int func_ov210_021bdd78(ZknSearchWork *wk) {
    func_ov210_021bcd3c(wk);
    func_ov210_021bf38c(wk);
    func_ov210_021c0550(wk);
    return SEQ_MAIN_INIT;
}

// pokebw2's ZknSearchSeq_ShapeInit
static int func_ov210_021bdd90(ZknSearchWork *wk) {
    switch (wk->initSeq) {
    case 0:
        wk->page = ZKNSEARCH_PAGE_SHAPE;
        func_ov210_021bc8b4(TRUE);
        func_ov210_021bebe8(wk);
        func_ov210_021c04cc(wk);
        wk->initSeq++;
    case 1:
        if (func_ov067_021b7a64(wk->frameList) == FALSE) {
            func_ov210_021bf728(wk, FALSE);
            func_ov210_021bf5b8(wk);
            func_ov210_021bcb88(wk);
            func_ov210_021bc5c8();
            func_ov210_021bcd30(wk);
            wk->initSeq = 0;
            return SEQ_SHAPE;
        }
        break;
    }
    return SEQ_SHAPE_INIT;
}

// pokebw2's ZknSearchSeq_Shape
static int func_ov210_021bde00(ZknSearchWork *wk) {
    int seq = SEQ_SHAPE;
    u32 ret = func_ov067_021b7ab0(wk->frameList);

    switch (ret) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (func_020355c4() == FALSE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be290(wk, ret);
        }
        break;
    case -12:
        break;
    case -11:
    case -10:
    case -9:
    case -8:
        func_ov210_021bf728(wk, FALSE);
        break;
    case -7:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 12);
        wk->buttonId = ZKNSEARCH_OBJ_PAGE_UP;
        wk->subSeq = 0;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case -6:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 13);
        wk->buttonId = ZKNSEARCH_OBJ_PAGE_DOWN;
        wk->subSeq = 0;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case -5:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_UP, 12);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_PAGE_UP, seq);
    case -4:
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_PAGE_DOWN, 13);
        return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_PAGE_DOWN, seq);
    case -3:
    case -2:
        func_ov210_021bf658(wk);
        func_ov210_021bf81c(wk);
        func_ov210_021bfb24(wk);
        wk->buttonId = BUTTON_NONE;
        wk->subSeq = 1;
        wk->nextSeq = seq;
        seq = SEQ_LIST_SCROLL;
        break;
    case FRAMELIST_NONE:
        ret = func_ov210_021c01c0(wk);
        switch (ret) {
        case ZKNSEARCH_UI_LIST_RETURN:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_SHAPE_EXIT);
        case ZKNSEARCH_UI_LIST_CANCEL:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_RETURN, 9);
            return func_ov210_021be2f4(wk, TRUE, ZKNSEARCH_OBJ_RETURN, SEQ_SHAPE_EXIT);
        case ZKNSEARCH_UI_LIST_RESET:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021be2dc(wk);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov210_021bf728(wk, FALSE);
            func_ov210_021be290(wk, ret - ZKNSEARCH_UI_LIST_ROW);
            break;
        }
        break;
    }
    func_ov210_021bf6e0(wk);
    return seq;
}

// pokebw2's ZknSearchSeq_ShapeExit
static int func_ov210_021bdfa8(ZknSearchWork *wk) {
    func_ov210_021bcd3c(wk);
    func_ov210_021bf38c(wk);
    func_ov210_021c0550(wk);
    return SEQ_MAIN_INIT;
}

// Waits for the list's jump or page scroll, and for the page button's animation
// pokebw2's ZknSearchSeq_ListScroll
static int func_ov210_021bdfc0(ZknSearchWork *wk) {
    switch (wk->subSeq) {
    case 0:
        if (func_ov067_021b7ab0(wk->frameList) == FRAMELIST_NONE) {
            wk->subSeq++;
        }
        break;
    case 1:
        if (wk->buttonId == BUTTON_NONE || func_ov210_021bede0(wk, wk->buttonId) == FALSE) {
            wk->subSeq = 0;
            func_ov210_021bf728(wk, FALSE);
            return wk->nextSeq;
        }
        break;
    }
    return SEQ_LIST_SCROLL;
}

// Flashes the cursor's position twice, or waits for a button's animation
// pokebw2's ZknSearchSeq_ButtonAnm
static int func_ov210_021be010(ZknSearchWork *wk) {
    if (wk->buttonIsActor == FALSE) {
        switch (wk->buttonSeq) {
        case 0:
        case 2:
            if (wk->buttonTimer == 0) {
                func_ov210_021bff30(wk, wk->buttonId, ZKNSEARCH_UI_PALETTE_OFF);
                wk->buttonTimer = 4;
                wk->buttonSeq++;
            } else {
                wk->buttonTimer--;
            }
            break;
        case 1:
        case 3:
            if (wk->buttonTimer == 0) {
                func_ov210_021bff30(wk, wk->buttonId, ZKNSEARCH_UI_PALETTE_PRESSED);
                wk->buttonTimer = 4;
                wk->buttonSeq++;
            } else {
                wk->buttonTimer--;
            }
            break;
        case 4:
            if (func_020355c4() == TRUE) {
                func_ov210_021bff30(wk, wk->buttonId, ZKNSEARCH_UI_PALETTE_OFF);
            } else {
                func_ov210_021bff30(wk, wk->buttonId, ZKNSEARCH_UI_PALETTE_ON);
            }
            return wk->buttonNextSeq;
        }
    } else {
        if (func_ov210_021bede0(wk, wk->buttonId) == FALSE) {
            if (wk->frameList != NULL) {
                func_ov210_021bf728(wk, FALSE);
            }
            return wk->buttonNextSeq;
        }
    }
    return SEQ_BUTTON_ANM;
}

// pokebw2's ZknSearchSeq_Finish
static int func_ov210_021be0c4(ZknSearchWork *wk) {
    func_ov210_021bff18(wk);
    func_ov210_021bce0c(wk, 8);
    func_ov210_021bc82c(wk, 0, 16);
    return SEQ_EXIT;
}

// pokebw2's ZknSearchSeq_SetOrder
static void func_ov210_021be0e4(ZknSearchWork *wk, u32 row) {
    wk->param->criteria->order = row;
    func_ov067_021b93d4(wk->frameList, row);
    func_ov210_021be8b8(wk);
    func_ov210_021bf85c(wk, row, TRUE);
}

// pokebw2's ZknSearchSeq_ResetOrder
static void func_ov210_021be110(ZknSearchWork *wk) {
    wk->param->criteria->order = 0;
    func_ov210_021be8b8(wk);
    func_ov210_021bf85c(wk, 0, TRUE);
}

// Chooses the row's initial, or none if it was the one chosen
// pokebw2's ZknSearchSeq_SetInitial
static void func_ov210_021be12c(ZknSearchWork *wk, u32 row) {
    u32 item = row + func_ov067_021b93b8(wk->frameList);

    if (wk->param->criteria->initial == item) {
        wk->param->criteria->initial = ZKNSEARCH_NONE;
        func_ov210_021bf85c(wk, row, FALSE);
    } else {
        wk->param->criteria->initial = item;
        func_ov210_021bf85c(wk, row, TRUE);
    }
    func_ov067_021b93d4(wk->frameList, row);
    func_ov210_021be978(wk);
}

// pokebw2's ZknSearchSeq_ResetInitial
static void func_ov210_021be178(ZknSearchWork *wk) {
    wk->param->criteria->initial = ZKNSEARCH_NONE;
    func_ov210_021be978(wk);
    func_ov210_021bf81c(wk);
}

// Chooses the row's type as the first or the second, dropping the first for a third, or drops it if it was chosen
// pokebw2's ZknSearchSeq_SetType
static void func_ov210_021be190(ZknSearchWork *wk, u32 row) {
    u32 type = func_ov210_021bceec(wk, row + func_ov067_021b93b8(wk->frameList));

    if (wk->param->criteria->type1 == ZKNSEARCH_NONE) {
        wk->param->criteria->type1 = type;
    } else if (wk->param->criteria->type2 == ZKNSEARCH_NONE) {
        if (wk->param->criteria->type1 == type) {
            wk->param->criteria->type1 = ZKNSEARCH_NONE;
        } else {
            wk->param->criteria->type2 = type;
        }
    } else if (wk->param->criteria->type1 == type) {
        wk->param->criteria->type1 = wk->param->criteria->type2;
        wk->param->criteria->type2 = ZKNSEARCH_NONE;
    } else if (wk->param->criteria->type2 == type) {
        wk->param->criteria->type2 = ZKNSEARCH_NONE;
    } else {
        wk->param->criteria->type1 = wk->param->criteria->type2;
        wk->param->criteria->type2 = type;
    }
    func_ov067_021b93d4(wk->frameList, row);
    func_ov210_021bea54(wk);
    func_ov210_021bf89c(wk, wk->param->criteria->type1, wk->param->criteria->type2);
}

// pokebw2's ZknSearchSeq_ResetType
static void func_ov210_021be20c(ZknSearchWork *wk) {
    wk->param->criteria->type1 = ZKNSEARCH_NONE;
    wk->param->criteria->type2 = ZKNSEARCH_NONE;
    func_ov210_021bea54(wk);
    func_ov210_021bf81c(wk);
}

// pokebw2's ZknSearchSeq_SetColor
static void func_ov210_021be22c(ZknSearchWork *wk, u32 row) {
    u32 item = row + func_ov067_021b93b8(wk->frameList);

    if (wk->param->criteria->color == item) {
        wk->param->criteria->color = ZKNSEARCH_NONE;
        func_ov210_021bf85c(wk, row, FALSE);
    } else {
        wk->param->criteria->color = item;
        func_ov210_021bf85c(wk, row, TRUE);
    }
    func_ov067_021b93d4(wk->frameList, row);
    func_ov210_021beb8c(wk);
}

// pokebw2's ZknSearchSeq_ResetColor
static void func_ov210_021be278(ZknSearchWork *wk) {
    wk->param->criteria->color = ZKNSEARCH_NONE;
    func_ov210_021beb8c(wk);
    func_ov210_021bf81c(wk);
}

// pokebw2's ZknSearchSeq_SetShape
static void func_ov210_021be290(ZknSearchWork *wk, u32 row) {
    u32 item = row + func_ov067_021b93b8(wk->frameList);

    if (wk->param->criteria->shape == item) {
        wk->param->criteria->shape = ZKNSEARCH_NONE;
        func_ov210_021bfa7c(wk, row, FALSE);
    } else {
        wk->param->criteria->shape = item;
        func_ov210_021bfa7c(wk, row, TRUE);
    }
    func_ov067_021b93d4(wk->frameList, row);
    func_ov210_021bf618(wk);
}

// pokebw2's ZknSearchSeq_ResetShape
static void func_ov210_021be2dc(ZknSearchWork *wk) {
    wk->param->criteria->shape = ZKNSEARCH_NONE;
    func_ov210_021bf618(wk);
    func_ov210_021bf81c(wk);
}

// pokebw2's ZknSearchSeq_SetButtonAnm
static int func_ov210_021be2f4(ZknSearchWork *wk, u32 isActor, u32 id, int next) {
    wk->buttonIsActor = isActor;
    wk->buttonId = id;
    wk->buttonSeq = 0;
    wk->buttonTimer = 0;
    wk->buttonNextSeq = next;
    return SEQ_BUTTON_ANM;
}

// pokebw2's ZknSearchSeq_SetPageEnd
static int func_ov210_021be314(ZknSearchWork *wk, int next) {
    wk->initSeq = 0;
    wk->nextSeq = next;
    return SEQ_PAGE_END;
}

// pokebw2's ZknSearchSeq_ToggleShortcut
static void func_ov210_021be324(ZknSearchWork *wk) {
    if (GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_POKEDEX_SEARCH) == TRUE) {
        GameData_SetKeyItemRegistration(wk->param->gameData, SHORTCUT_POKEDEX_SEARCH, FALSE);
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_SHORTCUT, 6);
    } else {
        GameData_SetKeyItemRegistration(wk->param->gameData, SHORTCUT_POKEDEX_SEARCH, TRUE);
        func_ov210_021bedb8(wk, ZKNSEARCH_OBJ_SHORTCUT, 7);
    }
}
