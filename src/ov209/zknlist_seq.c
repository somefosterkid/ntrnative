// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov299/zknlist_seq.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/frame_list.h"
#include "app/ui/ui_scene.h"
#include "app/zknlist_bmp.h"
#include "app/zknlist_main.h"
#include "app/zknlist_obj.h"
#include "app/zknlist_seq.h"
#include "app/zknlist_ui.h"
#include "app/zukan_common.h"
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
#include "system/game_data.h"
#include "system/palanm.h"
#include "app/zukanlist.h"

// The sequences of the Pokédex's list. The ROM doesn't name this file; zknlist_seq.c is a guess after the search
// screen's zknsearch_seq.c

enum {
    SEQ_INIT,
    SEQ_EXIT,
    SEQ_WAIT_FADE,
    SEQ_START,
    SEQ_LIST,
    SEQ_LIST_SCROLL,
    SEQ_BUTTON_ANM,
    SEQ_DECIDE,
    SEQ_FINISH,
    SEQ_END,
};

// Which Pokédex counts, func_0200d1f8's values
#define DEX_REGIONAL 0
#define DEX_NATIONAL 1
#define DEX_HABITAT 2

// No button to wait for while the list jumps
#define BUTTON_NONE ZKNLIST_OBJ_COUNT

typedef int (*ZknListSeqFunc)(ZknListWork *wk);

int func_ov209_021bcc94(ZknListWork *wk);
int func_ov209_021bcd48(ZknListWork *wk);
static int func_ov209_021bcde0(ZknListWork *wk);
int func_ov209_021bcdf8(ZknListWork *wk);
static int func_ov209_021bce98(ZknListWork *wk);
int func_ov209_021bd148(ZknListWork *wk);
static int func_ov209_021bd1a0(ZknListWork *wk);
int func_ov209_021bd1c8(ZknListWork *wk);
int func_ov209_021bd268(ZknListWork *wk);
void func_ov209_021bd280(ZknListWork *wk);
int func_ov209_021bd2c4(ZknListWork *wk, u32 id, u32 anm, int next);
BOOL func_ov209_021bd2f4(ZknListWork *wk, u32 index);
void func_ov209_021bd314(ZknListWork *wk, u32 index);
int func_ov209_021bd340(ZknListWork *wk, u32 index);
void func_ov209_021bd354(ZknListWork *wk);

// pokebw2's sZknListSeqFuncs
static const ZknListSeqFunc data_ov209_021be96c[] = {
    func_ov209_021bcc94,       func_ov209_021bcd48,      func_ov209_021bcde0, func_ov209_021bcdf8,  func_ov209_021bce98,
    func_ov209_021bd148, func_ov209_021bd1a0, func_ov209_021bd1c8,   func_ov209_021bd268,
};

// pokebw2's ZknListSeq_Main
BOOL func_ov209_021bcc58(ZknListWork *wk) {
    wk->seq = data_ov209_021be96c[wk->seq](wk);
    if (wk->seq == SEQ_END) {
        return FALSE;
    }
    func_ov209_021bd838(wk);
    func_ov209_021bd3fc(wk);
    func_ov212_021c93e4(3, 7, &wk->scrollTimer);
    return TRUE;
}

int func_ov209_021bcc94(ZknListWork *wk) {
    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GFL_BGSysSetDisplayLayout(0);
    func_ov209_021bc6c4(wk);
    func_ov209_021bc370();
    func_ov209_021bc388();
    func_ov209_021bc574();
    func_ov209_021bc824(wk);
    func_ov209_021bd398(wk);
    func_ov209_021bd7f8(wk);
    func_ov209_021bc88c(wk);
    func_ov209_021bc9a8(wk);
    if (wk->param->searched == FALSE) {
        func_ov209_021bd628(wk);
    } else {
        func_ov209_021bd704(wk);
    }
    func_0203d34c(TRUE, HEAPID_ZUKAN_LIST);
    func_ov209_021bc7b8();
    func_ov209_021bc7d0();
    func_ov209_021bc8b4(wk);
    func_ov209_021bc298(wk);
    func_ov209_021bc2e4(wk);
    return SEQ_START;
}


int func_ov209_021bcd48(ZknListWork *wk) {
    if (func_ov209_021bc91c(wk) == TRUE || func_02021ed8(wk->paletteFade) != 0) {
        return SEQ_EXIT;
    }
    func_ov209_021bc2fc(wk);
    func_ov209_021bc2b0(wk);
    func_ov209_021bc810();
    func_ov209_021bcb60(wk);
    func_ov209_021bc8a4(wk);
    func_ov209_021bd824(wk);
    func_ov209_021bd3e0(wk);
    func_ov209_021bc868(wk);
    func_ov209_021bc52c();
    func_ov209_021bc704(wk);
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GFL_OvlUnload(OVERLAY_ID(67));
    return SEQ_END;
}


// pokebw2's ZknListSeq_WaitFade
static int func_ov209_021bcde0(ZknListWork *wk) {
    if (func_02021ed8(wk->paletteFade) == 0) {
        return wk->fadeNextSeq;
    }
    return SEQ_WAIT_FADE;
}

int func_ov209_021bcdf8(ZknListWork *wk) {
    switch (wk->initSeq) {
    case 0:
        if (func_ov067_021b7a64(wk->frameList) == FALSE) {
            func_ov209_021be5d4(wk, FALSE);
            func_ov209_021be588(wk);
            func_ov209_021be43c(wk);
            func_ov209_021bd280(wk);
            wk->initSeq++;
        }
        break;
    case 1:
        if (func_02021ed8(wk->paletteFade) == 0) {
            GX_SetMasterBrightness(0);
            GXS_SetMasterBrightness(0);
            func_ov209_021bc730(wk, 16, 0);
            func_ov209_021bc90c(wk, -8);
            wk->initSeq++;
        }
        break;
    case 2:
        if (func_ov209_021bc91c(wk) == FALSE && func_02021ed8(wk->paletteFade) == 0) {
            wk->initSeq = 0;
            return SEQ_LIST;
        }
        break;
    }
    return SEQ_START;
}

static int func_ov209_021bce98(ZknListWork *wk) {
    int seq = SEQ_LIST;
    u32 ret = func_ov067_021b7ab0(wk->frameList);

    switch (ret) {
    case -12:
        break;
    case -11:
    case -10:
    case -9:
    case -8:
        func_ov209_021be5d4(wk, FALSE);
        break;
    case -7:
        func_ov209_021bd890(wk, ZKNLIST_OBJ_PAGE_UP, 12);
        wk->buttonId = ZKNLIST_OBJ_PAGE_UP;
        wk->subSeq = 0;
        seq = SEQ_LIST_SCROLL;
        break;
    case -6:
        func_ov209_021bd890(wk, ZKNLIST_OBJ_PAGE_DOWN, 13);
        wk->buttonId = ZKNLIST_OBJ_PAGE_DOWN;
        wk->subSeq = 0;
        seq = SEQ_LIST_SCROLL;
        break;
    case -5:
        return func_ov209_021bd2c4(wk, ZKNLIST_OBJ_PAGE_UP, 12, SEQ_LIST);
    case -4:
        return func_ov209_021bd2c4(wk, ZKNLIST_OBJ_PAGE_DOWN, 13, SEQ_LIST);
    case -3:
    case -2:
        func_ov209_021be4a8(wk);
        wk->buttonId = BUTTON_NONE;
        wk->subSeq = 1;
        seq = SEQ_LIST_SCROLL;
        break;
    case FRAMELIST_NONE: {
        BOOL touch = func_020355c4();
        int ui = func_ov209_021be658(wk);

        switch (ui) {
        case ZKNLIST_UI_MON: {
            int index = func_ov067_021b93a4(wk->frameList);

            if (func_ov209_021bd2f4(wk, index) == TRUE) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                return func_ov209_021bd340(wk, index);
            }
            func_020355d8(touch);
            break;
        }
        case ZKNLIST_UI_ROW:
        case ZKNLIST_UI_ROW + 1:
        case ZKNLIST_UI_ROW + 2:
        case ZKNLIST_UI_ROW + 3:
        case ZKNLIST_UI_ROW + 4:
        case ZKNLIST_UI_ROW + 5:
        case ZKNLIST_UI_ROW + 6: {
            int index = func_ov067_021b93b8(wk->frameList) + ui - ZKNLIST_UI_ROW;

            if (func_ov209_021bd2f4(wk, index) == TRUE) {
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                func_ov067_021b93d4(wk->frameList, ui - ZKNLIST_UI_ROW);
                return func_ov209_021bd340(wk, index);
            }
            break;
        }
        case ZKNLIST_UI_START:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            wk->param->result = ZUKAN_LIST_RESULT_SEARCH;
            func_ov209_021bd314(wk, func_ov067_021b93a4(wk->frameList));
            return func_ov209_021bd2c4(wk, ZKNLIST_OBJ_START, 4, SEQ_FINISH);
        case ZKNLIST_UI_SELECT:
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            if (func_0200ca70(wk->param->pokedex) == DEX_NATIONAL) {
                func_0200ca5c(wk->param->pokedex, DEX_REGIONAL);
            } else {
                func_0200ca5c(wk->param->pokedex, DEX_NATIONAL);
            }
            wk->param->result = ZUKAN_LIST_RESULT_RELOAD;
            func_ov209_021bd314(wk, func_ov067_021b93a4(wk->frameList));
            return func_ov209_021bd2c4(wk, ZKNLIST_OBJ_SELECT, 5, SEQ_FINISH);
        case ZKNLIST_UI_SHORTCUT:
            GFL_SndSEPlay(SEQ_SE_SYS_07);
            func_ov209_021bd354(wk);
            break;
        case ZKNLIST_UI_EXIT:
            GFL_SndSEPlay(SEQ_SE_CLOSE1);
            wk->param->result = ZUKAN_LIST_RESULT_CLOSE;
            func_ov209_021bd314(wk, func_ov067_021b93a4(wk->frameList));
            return func_ov209_021bd2c4(wk, ZKNLIST_OBJ_EXIT, 8, SEQ_FINISH);
        case ZKNLIST_UI_RETURN:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            wk->param->result = ZUKAN_LIST_RESULT_BACK;
            func_ov209_021bd314(wk, func_ov067_021b93a4(wk->frameList));
            return func_ov209_021bd2c4(wk, ZKNLIST_OBJ_RETURN, 9, SEQ_FINISH);
        case ZKNLIST_UI_CANCEL:
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            wk->param->result = ZUKAN_LIST_RESULT_BACK;
            func_ov209_021bd314(wk, func_ov067_021b93a4(wk->frameList));
            return func_ov209_021bd2c4(wk, ZKNLIST_OBJ_RETURN, 9, SEQ_FINISH);
        }
        break;
    }
    default: {
        int index = func_ov067_021b93a4(wk->frameList);

        func_ov209_021be5d4(wk, FALSE);
        if (func_ov209_021bd2f4(wk, index) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return func_ov209_021bd340(wk, index);
        }
        break;
    }
    }
    func_ov209_021be588(wk);
    func_ov209_021be43c(wk);
    return seq;
}



int func_ov209_021bd148(ZknListWork *wk) {
    switch (wk->subSeq) {
    case 0:
        if (func_ov067_021b7ab0(wk->frameList) == FRAMELIST_NONE) {
            wk->subSeq++;
        }
        break;
    case 1:
        if (wk->buttonId == BUTTON_NONE || func_ov209_021bd8c4(wk, wk->buttonId) == FALSE) {
            wk->subSeq = 0;
            func_ov209_021be5d4(wk, FALSE);
            return SEQ_LIST;
        }
        break;
    }
    return SEQ_LIST_SCROLL;
}


// pokebw2's ZknListSeq_ButtonAnm
static int func_ov209_021bd1a0(ZknListWork *wk) {
    if (func_ov209_021bd8c4(wk, wk->buttonId) == FALSE) {
        func_ov209_021be5d4(wk, FALSE);
        return wk->buttonNextSeq;
    }
    return SEQ_BUTTON_ANM;
}

int func_ov209_021bd1c8(ZknListWork *wk) {
    switch (wk->decideSeq) {
    case 0:
    case 2:
        if (wk->decideTimer == 0) {
            func_ov067_021b9280(wk->frameList, (u16)func_ov067_021b93b0(wk->frameList), 1);
            wk->decideTimer = 4;
            wk->decideSeq++;
        } else {
            wk->decideTimer--;
        }
        break;
    case 1:
    case 3:
        if (wk->decideTimer == 0) {
            func_ov067_021b9280(wk->frameList, (u16)func_ov067_021b93b0(wk->frameList), 2);
            GFL_BGSysQueueScrLoad(2);
            wk->decideTimer = 4;
            wk->decideSeq++;
        } else {
            wk->decideTimer--;
        }
        break;
    case 4:
        if (wk->decideTimer == 0) {
            wk->decideSeq = 0;
            return SEQ_FINISH;
        } else {
            wk->decideTimer--;
        }
        break;
    }
    return SEQ_DECIDE;
}


int func_ov209_021bd268(ZknListWork *wk) {
    func_ov209_021bc730(wk, 0, 16);
    func_ov209_021bc90c(wk, 8);
    return SEQ_EXIT;
}


// Takes the colors that the fades start from, and blacks them out
// pokebw2's ZknListSeq_InitFade
void func_ov209_021bd280(ZknListWork *wk) {
    func_020216d4(wk->paletteFade, PALFADE_BUFFER_MAIN_BG, 0, 0x200);
    func_020216d4(wk->paletteFade, PALFADE_BUFFER_SUB_BG, 0, 0x200);
    func_020216d4(wk->paletteFade, PALFADE_BUFFER_MAIN_OBJ, 0, 0x200);
    func_020216d4(wk->paletteFade, PALFADE_BUFFER_SUB_OBJ, 0, 0x200);
    func_ov209_021bc730(wk, 16, 16);
}

// pokebw2's ZknListSeq_SetButtonAnm
int func_ov209_021bd2c4(ZknListWork *wk, u32 id, u32 anm, int next) {
    func_ov209_021bd890(wk, id, anm);
    wk->buttonId = id;
    wk->buttonNextSeq = next;
    return SEQ_BUTTON_ANM;
}

BOOL func_ov209_021bd2f4(ZknListWork *wk, u32 index) {
    if ((func_ov067_021b9398(wk->frameList, index) & ZKNLIST_ITEM_STATE_MASK) >> ZKNLIST_ITEM_STATE_SHIFT != 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov209_021bd314(ZknListWork *wk, u32 index) {
    if (func_ov209_021bd2f4(wk, index) == TRUE) {
        func_0200ca78(wk->param->pokedex, func_ov067_021b9398(wk->frameList, index));
    }
}



// pokebw2's ZknListSeq_SetDecide
int func_ov209_021bd340(ZknListWork *wk, u32 index) {
    func_ov209_021bd314(wk, index);
    wk->param->result = ZUKAN_LIST_RESULT_DETAIL;
    return SEQ_DECIDE;
}

// pokebw2's ZknListSeq_ToggleShortcut
void func_ov209_021bd354(ZknListWork *wk) {
    if (GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_POKEDEX_LIST) == TRUE) {
        GameData_SetKeyItemRegistration(wk->param->gameData, SHORTCUT_POKEDEX_LIST, FALSE);
        func_ov209_021bd890(wk, ZKNLIST_OBJ_SHORTCUT, 6);
    } else {
        GameData_SetKeyItemRegistration(wk->param->gameData, SHORTCUT_POKEDEX_LIST, TRUE);
        func_ov209_021bd890(wk, ZKNLIST_OBJ_SHORTCUT, 7);
    }
}
