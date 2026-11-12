// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mb_seq.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mailbox_local.h"
#include "app/pokelist.h"
#include "constants/sound.h"
#include "field/mailsys.h"
#include "field/mailtool.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "pml/item.h"
#include "pml/mail.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/bgwinfrm.h"
#include "system/bmp_winframe.h"
#include "system/cursor_move.h"
#include "system/game_data.h"
#include "system/pms_data.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "app/mailbox.h"
#include "gfl/graphics.h"
#include "gfl/overlay.h"

// The mailbox screen's state machine, one function per state, with the party list and the mail writing screen run as
// child processes

// What a yes/no question does for each answer
// Black 2's, not checked against Black
typedef struct {
    MailBoxStateFunc yes;
    MailBoxStateFunc no;
} MailBoxYesNo;

// Black 2's, not checked against Black
typedef struct {
    MailBoxStateFunc start;
    MailBoxStateFunc end;
    u32 nextState;
} MailBoxChildProc;

static u32 func_ov136_021c6984(MailBoxWork *work);
static u32 func_ov136_021c6a5c(MailBoxWork *work);
static u32 func_ov136_021c6ad4(MailBoxWork *work);
static u32 func_ov136_021c6af0(MailBoxWork *work);
static u32 func_ov136_021c6b4c(MailBoxWork *work);
static u32 func_ov136_021c6bc4(MailBoxWork *work);
static u32 func_ov136_021c6bd8(MailBoxWork *work);
static u32 func_ov136_021c6bf0(MailBoxWork *work);
static u32 func_ov136_021c6c28(MailBoxWork *work);
static u32 func_ov136_021c6c34(MailBoxWork *work);
static u32 func_ov136_021c6e18(MailBoxWork *work);
static u32 func_ov136_021c6e28(MailBoxWork *work);
static u32 func_ov136_021c6e64(MailBoxWork *work);
static u32 func_ov136_021c6e98(MailBoxWork *work);
static u32 func_ov136_021c6ea4(MailBoxWork *work);
static u32 func_ov136_021c6fa8(MailBoxWork *work);
static u32 func_ov136_021c6fd4(MailBoxWork *work);
static u32 func_ov136_021c7018(MailBoxWork *work);
static u32 func_ov136_021c7024(MailBoxWork *work);
static u32 func_ov136_021c703c(MailBoxWork *work);
static u32 func_ov136_021c7048(MailBoxWork *work);
static u32 func_ov136_021c7064(MailBoxWork *work);
static u32 func_ov136_021c7084(MailBoxWork *work);
static u32 func_ov136_021c7094(MailBoxWork *work);
static u32 func_ov136_021c70c0(MailBoxWork *work);
static u32 func_ov136_021c70ec(MailBoxWork *work);
static u32 func_ov136_021c7110(MailBoxWork *work);
static u32 func_ov136_021c714c(MailBoxWork *work, u32 nextState);
static u32 func_ov136_021c7178(MailBoxWork *work, u32 nextState);
static u32 func_ov136_021c71a8(MailBoxWork *work, u8 waitForInput, u32 nextState);
static u32 func_ov136_021c71bc(MailBoxWork *work, u32 index);
static u32 func_ov136_021c71d0(MailBoxWork *work, u32 frame, u32 nextState);
static u32 func_ov136_021c7250(MailBoxWork *work, u32 direction, u32 nextState);
static void func_ov136_021c7298(MailBoxView *view, BOOL dimmed);
static void func_ov136_021c730c(MailBoxView *view);
static void func_ov136_021c7350(MailBoxWork *work);
static void func_ov136_021c7360(MailBoxWork *work);
static void func_ov136_021c73c8(MailBoxWork *work);
static void func_ov136_021c7420(MailBoxWork *work);
void func_ov136_021c7430(MailBoxWork *work, u32 mode, u16 item, u16 mailIndex);
static u32 func_ov136_021c7494(MailBoxWork *work);
static u32 func_ov136_021c74bc(MailBoxWork *work);
static u32 func_ov136_021c74c8(MailBoxWork *work);
static u32 func_ov136_021c74f8(MailBoxWork *work);
static u32 func_ov136_021c7504(MailBoxWork *work);
static u32 func_ov136_021c7540(MailBoxWork *work);
static u32 func_ov136_021c758c(MailBoxWork *work);
static u32 func_ov136_021c7600(MailBoxWork *work);
static u32 func_ov136_021c7620(MailBoxWork *work);

// pokebw2's sMailSysProc
// pokebw2's MailSys_ProcMain
static const GameProcFunctions data_ov136_021c7d5c = { func_ov136_021c3e3c, func_ov136_021c3e9c, func_ov136_021c3f98 };

// pokebw2's sYesNoFuncs
static const MailBoxYesNo data_ov136_021c7d68[2] = {
    { func_ov136_021c758c, func_ov136_021c7110 },
    { func_ov136_021c7600, func_ov136_021c7620 },
};

// pokebw2's sChildProcs
static const MailBoxChildProc data_ov136_021c7d78[MAILBOX_CHILD_COUNT] = {
    { func_ov136_021c7494, func_ov136_021c74bc, MAILBOX_STATE_INIT },
    { func_ov136_021c7494, func_ov136_021c74c8, MAILBOX_STATE_PARTY_RETURN },
    { func_ov136_021c7504, func_ov136_021c7540, MAILBOX_STATE_MAIL_WRITTEN },
    { func_ov136_021c7494, func_ov136_021c74f8, MAILBOX_STATE_INIT },
};

// pokebw2's sStateFuncs
static const MailBoxStateFunc data_ov136_021c7da8[] = {
    func_ov136_021c6984,       func_ov136_021c6a5c,         func_ov136_021c6ad4,    func_ov136_021c6af0,
    func_ov136_021c6b4c,  func_ov136_021c6bc4, func_ov136_021c6bd8, func_ov136_021c6bf0,
    func_ov136_021c6c28,     func_ov136_021c6c34,   func_ov136_021c6e18,     func_ov136_021c6e28,
    func_ov136_021c6e64,   func_ov136_021c6e98,   func_ov136_021c6ea4,    func_ov136_021c6fa8,
    func_ov136_021c6fd4,  func_ov136_021c7018,     func_ov136_021c7024, func_ov136_021c703c,
    func_ov136_021c7048, func_ov136_021c7064,  func_ov136_021c7084,  func_ov136_021c7094,
    func_ov136_021c70c0, func_ov136_021c70ec,  func_ov136_021c7110,
};

// pokebw2's MailBox_Update
BOOL func_ov136_021c68a0(MailBoxWork *work, u32 *state) {
    *state = data_ov136_021c7da8[*state](work);
    if (*state == MAILBOX_STATE_DONE) {
        return FALSE;
    }
    if (work->view != NULL) {
        func_0202cd94(work->view->frames);
        func_ov136_021c6594(work->view);
        if (work->view->printQueue != NULL) {
            int i;

            func_0201c4e4(work->view->printQueue);
            for (i = 0; i < MAILBOX_WIN_SENTENCE_1; i++) {
                PrintQueue *queue = work->view->printQueue;
                MailBoxPrintState *printState = &work->view->printStates[i];

                if (printState->printing) {
                    if (func_0201c6c4(queue, BmpWin_GetBitmap(printState->window)) == FALSE) {
                        BmpWin_FlushChar(printState->window);
                        printState->printing = FALSE;
                    }
                }
            }
        }
        if (work->view->tcbManager != NULL) {
            GFL_TCBExMgrUpdate(work->view->tcbManager);
        }
        if (work->view->pmsPrintQueue != NULL) {
            int i;

            func_0202dbc4(work->view->pmsDraw);
            func_0201c4e4(work->view->pmsPrintQueue);
            for (i = MAILBOX_WIN_SENTENCE_1; i < MAILBOX_WINDOW_COUNT; i++) {
                PrintQueue *queue = work->view->printQueue;
                MailBoxPrintState *printState = &work->view->printStates[i];

                if (printState->printing) {
                    if (func_0201c6c4(queue, BmpWin_GetBitmap(printState->window)) == FALSE) {
                        BmpWin_FlushChar(printState->window);
                        printState->printing = FALSE;
                    }
                }
            }
        }
    }
    return TRUE;
}

// pokebw2's MailBox_StateInit
static u32 func_ov136_021c6984(MailBoxWork *work) {
    if (work->vblankTask != NULL) {
        GFL_TCBRemove(work->vblankTask);
        work->vblankTask = NULL;
    }
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetDisplayLayout(0);
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_MAILBOX_VIEW, 0x60000);
    work->view = GFL_HeapAllocate(HEAPID_MAILBOX_VIEW, sizeof(MailBoxView), FALSE, "mb_seq.c", 268);
    MIi_CpuClearFast(0, work->view, sizeof(MailBoxView));
    func_ov136_021c5b40(work);
    func_ov136_021c5508();
    func_ov136_021c5518(work);
    func_ov136_021c574c(work);
    func_ov136_021c5828(work);
    func_ov136_021c5e58(work);
    func_ov136_021c587c(work);
    func_ov136_021c64a8(work);
    func_ov136_021c5dd8(work);
    func_ov136_021c7658(work);
    func_ov136_021c5924(work->view);
    func_ov136_021c6170(work);
    func_ov136_021c5c00(work);
    func_ov136_021c59b8(work);
    work->vblankTask = GFL_VBlankTCBAdd(func_ov136_021c54fc, work, 1);
    func_0203d34c(FALSE, HEAPID_MAILBOX_VIEW);
    return work->nextState;
}

// pokebw2's MailBox_StateExit
static u32 func_ov136_021c6a5c(MailBoxWork *work) {
    if (work->vblankTask != NULL) {
        GFL_TCBRemove(work->vblankTask);
        work->vblankTask = NULL;
    }
    func_ov136_021c5aa8(work);
    func_ov136_021c5954(work->view);
    func_ov136_021c76a8(work);
    func_ov136_021c5e30(work);
    func_ov136_021c657c(work);
    func_ov136_021c5918(work);
    func_ov136_021c5f44(work);
    func_ov136_021c5714(work);
    func_ov136_021c5be0(work);
    G2_BlendNone();
    G2S_BlendNone();
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GFL_HeapFree(work->view);
    work->view = NULL;
    GFL_HeapDelete(HEAPID_MAILBOX_VIEW);
    return work->nextState;
}

// pokebw2's MailBox_StateWaitWipe
static u32 func_ov136_021c6ad4(MailBoxWork *work) {
    if (GFL_WipeIsFinished() == TRUE) {
        return work->view->wipeNextState;
    }
    return MAILBOX_STATE_WAIT_WIPE;
}

// pokebw2's MailBox_StateWaitMessage
static u32 func_ov136_021c6af0(MailBoxWork *work) {
    if (func_0201ce30(work->view->stream) == PRINT_STREAM_DONE) {
        if (!work->view->waitForInput) {
            func_0201ce48(work->view->stream);
            return work->nextState;
        }
        if (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_0201ce48(work->view->stream);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return work->nextState;
        }
    }
    return MAILBOX_STATE_WAIT_MESSAGE;
}

// pokebw2's MailBox_StateWaitYesNo
static u32 func_ov136_021c6b4c(MailBoxWork *work) {
    func_02027974(work->view->menu);
    if (!func_020279e8(work->view->menu)) {
        return MAILBOX_STATE_WAIT_YES_NO;
    }
    switch (func_02027a04(work->view->menu)) {
    case 0:
        func_02027858(work->view->menu);
        return data_ov136_021c7d68[work->view->yesNoIndex].yes(work);
    case 1:
        func_02027858(work->view->menu);
        return data_ov136_021c7d68[work->view->yesNoIndex].no(work);
    default:
        return MAILBOX_STATE_WAIT_YES_NO;
    }
}

// pokebw2's MailBox_StateWaitPaletteFx
static u32 func_ov136_021c6bc4(MailBoxWork *work) {
    if (func_ov136_021c5c84(work) == FALSE) {
        return work->nextState;
    }
    return MAILBOX_STATE_WAIT_PALETTE_FX;
}

// pokebw2's MailBox_StateStartChild
static u32 func_ov136_021c6bd8(MailBoxWork *work) {
    data_ov136_021c7d78[work->childProc].start(work);
    return MAILBOX_STATE_WAIT_CHILD;
}

// pokebw2's MailBox_StateWaitChild
static u32 func_ov136_021c6bf0(MailBoxWork *work) {
    if (GFL_ProcMgrUpdate(work->procManager) == TRUE) {
        return MAILBOX_STATE_WAIT_CHILD;
    }
    work->nextState = data_ov136_021c7d78[work->childProc].end(work);
    return data_ov136_021c7d78[work->childProc].nextState;
}

// pokebw2's MailBox_StateWipeIn
static u32 func_ov136_021c6c28(MailBoxWork *work) {
    return func_ov136_021c714c(work, MAILBOX_STATE_SELECT_MAIL);
}

// pokebw2's MailBox_StateSelectMail
static u32 func_ov136_021c6c34(MailBoxWork *work) {
    u32 result;

    if (func_0201c6b4(work->view->printQueue) == FALSE) {
        return MAILBOX_STATE_SELECT_MAIL;
    }
    switch (func_ov136_021c7648(work)) {
    case 0:
        if (work->page != 0) {
            work->page--;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return func_ov136_021c7250(work, 0, MAILBOX_STATE_CHANGE_PAGE);
        }
        return MAILBOX_STATE_SELECT_MAIL;
    case 1:
        if (work->page != work->view->maxPage) {
            work->page++;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return func_ov136_021c7250(work, 1, MAILBOX_STATE_CHANGE_PAGE);
        }
        return MAILBOX_STATE_SELECT_MAIL;
    case 2:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov136_021c66a0(work->view, MAILBOX_ACTOR_RETURN_BUTTON, 9);
        work->nextState = MAILBOX_STATE_WIPE_OUT;
        return MAILBOX_STATE_PRESS_RETURN;
    case 3:
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        func_ov136_021c66a0(work->view, MAILBOX_ACTOR_CLOSE_BUTTON, 8);
        work->nextState = MAILBOX_STATE_WIPE_OUT;
        return MAILBOX_STATE_PRESS_CLOSE;
    }
    result = func_02025de8(work->view->cursor);
    switch (result) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        if (*(work->view->pageMails + result + MAILBOX_PAGE_SIZE * work->page) == 0xff) {
            break;
        }
        work->cursor = result;
        work->mailIndex = *(work->view->pageMails + result + MAILBOX_PAGE_SIZE * work->page);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return func_ov136_021c71d0(work, result, MAILBOX_STATE_OPEN_MENU);
    case CURSOR_MOVE_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov136_021c66a0(work->view, MAILBOX_ACTOR_RETURN_BUTTON, 9);
        work->nextState = MAILBOX_STATE_WIPE_OUT;
        return MAILBOX_STATE_PRESS_RETURN;
    case CURSOR_MOVE_EDGE_RIGHT:
        func_020260e0(work->view->cursor);
        if (work->page != work->view->maxPage) {
            work->page++;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return func_ov136_021c7250(work, 1, MAILBOX_STATE_CHANGE_PAGE);
        }
        break;
    case CURSOR_MOVE_EDGE_LEFT:
        func_020260e0(work->view->cursor);
        if (work->page != 0) {
            work->page--;
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return func_ov136_021c7250(work, 0, MAILBOX_STATE_CHANGE_PAGE);
        }
        break;
    case CURSOR_MOVE_MOVED:
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_CURSOR_ON:
        break;
    default:
        if (func_ov136_021c7738()) {
            GFL_SndSEPlay(SEQ_SE_CLOSE1);
            func_ov136_021c66a0(work->view, MAILBOX_ACTOR_CLOSE_BUTTON, 8);
            work->nextState = MAILBOX_STATE_WIPE_OUT;
            return MAILBOX_STATE_PRESS_CLOSE;
        }
        break;
    }
    return MAILBOX_STATE_SELECT_MAIL;
}

// pokebw2's MailBox_StateWipeOut
static u32 func_ov136_021c6e18(MailBoxWork *work) {
    work->nextState = MAILBOX_STATE_DONE;
    return func_ov136_021c7178(work, MAILBOX_STATE_EXIT);
}

// pokebw2's MailBox_StateChangePage
static u32 func_ov136_021c6e28(MailBoxWork *work) {
    if (work->page == 0) {
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_PREV_PAGE, FALSE);
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_NEXT_PAGE, TRUE);
    } else {
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_PREV_PAGE, TRUE);
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_NEXT_PAGE, FALSE);
    }
    func_ov136_021c5c00(work);
    return MAILBOX_STATE_SELECT_MAIL;
}

// pokebw2's MailBox_StateOpenMenu
static u32 func_ov136_021c6e64(MailBoxWork *work) {
    func_ov136_021c66b8(work->view, MAILBOX_ACTOR_CURSOR, FALSE);
    func_ov136_021c7298(work->view, TRUE);
    func_ov136_021c5c64(work);
    func_ov136_021c62bc(work);
    func_ov136_021c6284(work);
    return func_ov136_021c71a8(work, FALSE, MAILBOX_STATE_CREATE_MENU);
}

// pokebw2's MailBox_StateCreateMenu
static u32 func_ov136_021c6e98(MailBoxWork *work) {
    func_ov136_021c5aec(work);
    return MAILBOX_STATE_WAIT_MENU;
}

// pokebw2's MailBox_StateWaitMenu
static u32 func_ov136_021c6ea4(MailBoxWork *work) {
    func_02027974(work->view->menu);
    if (!func_020279e8(work->view->menu)) {
        return MAILBOX_STATE_WAIT_MENU;
    }
    switch (func_02027a04(work->view->menu) + 2) {
    case 2:
        func_ov136_021c5b30(work);
        func_ov136_021c62f4(work);
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_CURSOR, FALSE);
        func_ov136_021c61cc(work->view);
        func_ov136_021c6884(work->view);
        func_ov136_021c7350(work);
        func_ov136_021c66a0(work->view, MAILBOX_ACTOR_CLOSE_BUTTON, 14);
        func_ov136_021c730c(work->view);
        return MAILBOX_STATE_READ_MAIL;
    case 3:
        func_ov136_021c5b30(work);
        func_ov136_021c630c(work, MAILBOX_MESSAGE_ERASE);
        return func_ov136_021c71a8(work, FALSE, MAILBOX_STATE_ASK_ERASE);
    case 4:
        func_ov136_021c5b30(work);
        func_ov136_021c7430(work, 14, PML_ItemGetMailItemID(func_02008fa4(work->view->mails[work->mailIndex])),
                                 work->mailIndex);
        work->childProc = MAILBOX_CHILD_PARTY_GIVE;
        work->nextState = MAILBOX_STATE_START_CHILD;
        return func_ov136_021c7178(work, 1);
    case 0:
    case 5:
        func_ov136_021c5b30(work);
        func_ov136_021c62f4(work);
        func_0202cc70(work->view->frames, MAILBOX_FRAME_SELECTED);
        func_ov136_021c7298(work->view, FALSE);
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_CURSOR, TRUE);
        return MAILBOX_STATE_SELECT_MAIL;
    default:
        return MAILBOX_STATE_WAIT_MENU;
    }
}

// pokebw2's MailBox_StateReadMail
static u32 func_ov136_021c6fa8(MailBoxWork *work) {
    if (func_ov136_021c770c() == TRUE) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_ov136_021c66a0(work->view, MAILBOX_ACTOR_RETURN_BUTTON, 9);
        work->nextState = MAILBOX_STATE_CLOSE_MAIL;
        return MAILBOX_STATE_PRESS_RETURN;
    }
    return MAILBOX_STATE_READ_MAIL;
}

// pokebw2's MailBox_StateCloseMail
static u32 func_ov136_021c6fd4(MailBoxWork *work) {
    func_ov136_021c7360(work);
    func_ov136_021c76d0(work, func_020260e0(work->view->cursor));
    func_ov136_021c61d0(work->view);
    func_0202cc70(work->view->frames, MAILBOX_FRAME_SELECTED);
    func_ov136_021c66b8(work->view, MAILBOX_ACTOR_CURSOR, TRUE);
    func_ov136_021c7298(work->view, FALSE);
    return MAILBOX_STATE_SELECT_MAIL;
}

// pokebw2's MailBox_StateAskErase
static u32 func_ov136_021c7018(MailBoxWork *work) {
    return func_ov136_021c71bc(work, 0);
}

// pokebw2's MailBox_StateEraseNotice
static u32 func_ov136_021c7024(MailBoxWork *work) {
    func_ov136_021c630c(work, MAILBOX_MESSAGE_ASK_HOLD);
    return func_ov136_021c71a8(work, FALSE, MAILBOX_STATE_ASK_GIVE);
}

// pokebw2's MailBox_StateAskGive
static u32 func_ov136_021c703c(MailBoxWork *work) {
    return func_ov136_021c71bc(work, 1);
}

// pokebw2's MailBox_StatePartyReturn
static u32 func_ov136_021c7048(MailBoxWork *work) {
    if (work->partyResult != 6) {
        work->nextState = MAILBOX_STATE_GIVE_FADE_IN;
        return MAILBOX_STATE_INIT;
    }
    work->childProc = MAILBOX_CHILD_MAIL_SYS;
    return MAILBOX_STATE_START_CHILD;
}

// pokebw2's MailBox_StateGiveFadeIn
static u32 func_ov136_021c7064(MailBoxWork *work) {
    func_ov136_021c66b8(work->view, MAILBOX_ACTOR_CURSOR, FALSE);
    func_ov136_021c7298(work->view, TRUE);
    return func_ov136_021c714c(work, MAILBOX_STATE_GIVE_RESULT);
}

// pokebw2's MailBox_StateGiveResult
static u32 func_ov136_021c7084(MailBoxWork *work) {
    func_ov136_021c62bc(work);
    return func_ov136_021c7620(work);
}

// pokebw2's MailBox_StateMailWritten
static u32 func_ov136_021c7094(MailBoxWork *work) {
    if (!work->mailGiven) {
        work->nextState = MAILBOX_STATE_GIVE_FADE_IN;
        return MAILBOX_STATE_INIT;
    }
    func_ov136_021c7430(work, 11, work->item, work->partyPos);
    work->childProc = MAILBOX_CHILD_PARTY_PICK;
    return MAILBOX_STATE_START_CHILD;
}

// pokebw2's MailBox_StatePressReturn
static u32 func_ov136_021c70c0(MailBoxWork *work) {
    if (!func_0204b98c(work->view->actors[MAILBOX_ACTOR_RETURN_BUTTON])) {
        work->param->result = 0;
        func_ov136_021c66a0(work->view, MAILBOX_ACTOR_CLOSE_BUTTON, 0);
        return work->nextState;
    }
    return MAILBOX_STATE_PRESS_RETURN;
}

// pokebw2's MailBox_StatePressClose
static u32 func_ov136_021c70ec(MailBoxWork *work) {
    if (!func_0204b98c(work->view->actors[MAILBOX_ACTOR_CLOSE_BUTTON])) {
        work->param->result = 1;
        return work->nextState;
    }
    return MAILBOX_STATE_PRESS_CLOSE;
}

// pokebw2's MailBox_StateBackToList
static u32 func_ov136_021c7110(MailBoxWork *work) {
    func_ov136_021c62f4(work);
    func_0202cc70(work->view->frames, MAILBOX_FRAME_SELECTED);
    func_ov136_021c76d0(work, func_020260e0(work->view->cursor));
    func_ov136_021c66b8(work->view, MAILBOX_ACTOR_CURSOR, TRUE);
    func_ov136_021c7298(work->view, FALSE);
    return MAILBOX_STATE_SELECT_MAIL;
}

// pokebw2's MailBox_StartWipeIn
static u32 func_ov136_021c714c(MailBoxWork *work, u32 nextState) {
    GFL_WipeSet(0, 1, 1, 0, 6, 1, HEAPID_MAILBOX_VIEW);
    work->view->wipeNextState = nextState;
    return MAILBOX_STATE_WAIT_WIPE;
}

// pokebw2's MailBox_StartWipeOut
static u32 func_ov136_021c7178(MailBoxWork *work, u32 nextState) {
    GFL_WipeSet(0, 0, 0, 0, 6, 1, HEAPID_MAILBOX_VIEW);
    work->view->wipeNextState = nextState;
    return MAILBOX_STATE_WAIT_WIPE;
}

// pokebw2's MailBox_StartMessage
static u32 func_ov136_021c71a8(MailBoxWork *work, u8 waitForInput, u32 nextState) {
    work->view->waitForInput = waitForInput;
    work->nextState = nextState;
    return MAILBOX_STATE_WAIT_MESSAGE;
}

// pokebw2's MailBox_StartYesNo
static u32 func_ov136_021c71bc(MailBoxWork *work, u32 index) {
    work->view->yesNoIndex = index;
    func_ov136_021c5974(work);
    return MAILBOX_STATE_WAIT_YES_NO;
}

// pokebw2's MailBox_StartFrameFx
static u32 func_ov136_021c71d0(MailBoxWork *work, u32 frame, u32 nextState) {
    MailBoxPaletteFx *fx;
    s8 x;
    s8 y;
    u16 width;
    u16 height;

    func_0202cf98(work->view->frames, frame, &x, &y);
    func_0202cfc0(work->view->frames, frame, &width, &height);
    fx = &work->view->paletteFx;
    fx->onScreen = TRUE;
    fx->target = 2;
    fx->firstPalette = 2;
    fx->secondPalette = 1;
    fx->phase = 0;
    fx->timer = 0;
    fx->x = x;
    fx->y = y;
    fx->width = width;
    fx->height = height;
    work->nextState = nextState;
    return MAILBOX_STATE_WAIT_PALETTE_FX;
}

// pokebw2's MailBox_StartPageFx
static u32 func_ov136_021c7250(MailBoxWork *work, u32 direction, u32 nextState) {
    MailBoxPaletteFx *fx = &work->view->paletteFx;

    fx->onScreen = FALSE;
    fx->target = direction;
    fx->firstPalette = 1;
    fx->secondPalette = 0;
    fx->phase = 0;
    fx->timer = 0;
    work->nextState = nextState;
    return MAILBOX_STATE_WAIT_PALETTE_FX;
}

// pokebw2's MailBox_SetDimmed
static void func_ov136_021c7298(MailBoxView *view, BOOL dimmed) {
    if (dimmed == TRUE) {
        func_ov136_021c670c(view, 0, TRUE);
        func_ov136_021c670c(view, 1, TRUE);
        func_ov136_021c670c(view, 3, TRUE);
        func_ov136_021c670c(view, 7, TRUE);
        G2_SetBlendAlpha(4, 8, 4, 12);
    } else {
        func_ov136_021c670c(view, 0, FALSE);
        func_ov136_021c670c(view, 1, FALSE);
        func_ov136_021c670c(view, 3, FALSE);
        func_ov136_021c670c(view, 7, FALSE);
        G2_BlendNone();
    }
}

// pokebw2's MailBox_SetDimmedForReading
static void func_ov136_021c730c(MailBoxView *view) {
    func_ov136_021c670c(view, 0, TRUE);
    func_ov136_021c670c(view, 1, TRUE);
    func_ov136_021c670c(view, 3, FALSE);
    func_ov136_021c670c(view, 7, FALSE);
    G2_SetBlendAlpha(4, 8, 4, 12);
}

// pokebw2's MailBox_ShowMail
static void func_ov136_021c7350(MailBoxWork *work) {
    func_ov136_021c5d68(work);
    func_ov136_021c6374(work);
}

// pokebw2's MailBox_ClearMail
static void func_ov136_021c7360(MailBoxWork *work) {
    int i;

    for (i = 0; i < 3; i++) {
        if (func_02024450(func_02008fc8(work->view->mails[work->mailIndex], i))) {
            func_0202dc9c(work->view->pmsDraw, i, FALSE);
        }
    }
    if (work->view->stampSentence.words[0] != 0) {
        func_0202dc9c(work->view->pmsDraw, i, FALSE);
    }
    GFL_BGSysClearScr(4);
    GFL_BGSysClearScr(5);
}

// pokebw2's MailBox_TakeMailItem
static void func_ov136_021c73c8(MailBoxWork *work) {
    BagSave *bag = GameData_GetBag(work->param->gameData);

    work->item = PML_ItemGetMailItemID(func_02008fa4(work->view->mails[work->mailIndex]));
    if (BagSave_AddItem(bag, work->item, 1, HEAPID_MAILBOX_VIEW) == TRUE) {
        work->bagFull = FALSE;
    } else {
        work->bagFull = TRUE;
    }
}

// The parameter the mailbox's child process gets (work->childParam): laid out from the assembly, the fields it names
// by offset are not checked against Black
typedef struct {
    PokeParty *party;
    BagSave *bag;
    MailData *mails;
    u32 unk_0C[3];
    PokeDexSave *pokedex;
    u32 unk_1C[6];
    u32 mode;
    u32 unk_38;
    u8 unk_3C[8];
    u16 item;
    u8 unk_46[0x16];
    u16 mailIndex;
    u8 unk_5E[0x3A];
} MailBoxChildParam;

// pokebw2's MailBox_EraseMail
static void func_ov136_021c7420(MailBoxWork *work) {
    func_0200902c(work->mails, 0, work->mailIndex);
}

void func_ov136_021c7430(MailBoxWork *work, u32 mode, u16 item, u16 mailIndex) {
    MailBoxChildParam *param = GFL_HeapAllocate(HEAPID_MAILBOX_PROC, sizeof(MailBoxChildParam), TRUE, "mb_seq.c", 1321);

    param->party = GameData_GetParty(work->param->gameData);
    param->bag = GameData_GetBag(work->param->gameData);
    param->mails = work->mails;
    param->unk_38 = 0;
    param->mode = mode;
    param->item = item;
    param->mailIndex = mailIndex;
    param->pokedex = GameData_GetPokedex(work->param->gameData);
    work->childParam = param;
}


// pokebw2's MailBox_StartPartyList
static u32 func_ov136_021c7494(MailBoxWork *work) {
    PokeListParam *param = work->childParam;

    param->index = work->partyPos;
    QueueGameProc(work->procManager, OVERLAY_ID(91), &data_ov091_021bed00, work->childParam);
    return 0;
}

// pokebw2's MailBox_EndPartyListCancel
static u32 func_ov136_021c74bc(MailBoxWork *work) {
    GFL_HeapFree(work->childParam);
    return MAILBOX_STATE_WIPE_IN;
}

// pokebw2's MailBox_EndPartyListPick
static u32 func_ov136_021c74c8(MailBoxWork *work) {
    PokeListParam *param = work->childParam;

    work->partyPos = param->index;
    work->partyResult = param->result;
    GFL_HeapFree(param);
    return 0;
}

// pokebw2's MailBox_EndPartyListCancelAsk
static u32 func_ov136_021c74f8(MailBoxWork *work) {
    GFL_HeapFree(work->childParam);
    return MAILBOX_STATE_WIPE_IN;
}

// pokebw2's MailBox_StartMailSys
static u32 func_ov136_021c7504(MailBoxWork *work) {
    work->childParam = func_ov136_021c3cc0(work->param->gameData, 2, work->partyPos, PML_ItemGetMailID(work->item),
                                           HEAPID_MAILBOX_PROC);
    QueueGameProc(work->procManager, OVERLAY_NONE, &data_ov136_021c7d5c, work->childParam);
    return 0;
}

// pokebw2's MailBox_EndMailSys
static u32 func_ov136_021c7540(MailBoxWork *work) {
    work->mailGiven = func_ov136_021c3e00(work->childParam);
    if (work->mailGiven == TRUE) {
        func_ov136_021c3e04(work->childParam,
                            PokeParty_GetPkm(GameData_GetParty(work->param->gameData), work->partyPos));
        BagSave_SubItem(GameData_GetBag(work->param->gameData), work->item, 1, HEAPID_MAILBOX_PROC);
    }
    func_ov136_021c3e24(work->childParam);
    return 0;
}

// pokebw2's MailBox_EraseYes
static u32 func_ov136_021c758c(MailBoxWork *work) {
    func_ov136_021c73c8(work);
    func_ov136_021c7420(work);
    func_ov136_021c5be0(work);
    func_ov136_021c5b40(work);
    if (work->view->mailCount <= MAILBOX_PAGE_SIZE) {
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_PREV_PAGE, FALSE);
        func_ov136_021c66b8(work->view, MAILBOX_ACTOR_NEXT_PAGE, FALSE);
    }
    if (work->page > work->view->maxPage) {
        work->page = work->view->maxPage;
        work->cursor = 0;
    }
    func_ov136_021c5c00(work);
    func_0202cc70(work->view->frames, MAILBOX_FRAME_SELECTED);
    func_ov136_021c630c(work, MAILBOX_MESSAGE_ERASED);
    return func_ov136_021c71a8(work, TRUE, MAILBOX_STATE_ERASE_NOTICE);
}

// pokebw2's MailBox_GiveYes
static u32 func_ov136_021c7600(MailBoxWork *work) {
    func_ov136_021c7430(work, 9, work->item, 0);
    work->childProc = MAILBOX_CHILD_PARTY_ASK;
    work->nextState = MAILBOX_STATE_START_CHILD;
    return func_ov136_021c7178(work, 1);
}

// pokebw2's MailBox_GiveNo
static u32 func_ov136_021c7620(MailBoxWork *work) {
    if (!work->bagFull) {
        func_ov136_021c630c(work, MAILBOX_MESSAGE_HOLD_DONE);
    } else {
        func_ov136_021c630c(work, MAILBOX_MESSAGE_BAG_FULL);
    }
    return func_ov136_021c71a8(work, TRUE, MAILBOX_STATE_BACK_TO_LIST);
}
