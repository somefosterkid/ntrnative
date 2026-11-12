// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov286/b_bag_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/b_bag_main.h"
#include "battle/b_app_tool.h"
#include "battle/b_bag_anm.h"
#include "battle/b_bag_bmp.h"
#include "battle/b_bag_item.h"
#include "battle/b_bag_obj.h"
#include "battle/b_bag_ui.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_finger_cursor.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "pml/item.h"
#include "save/bag.h"
#include "system/bgwinfrm.h"
#include "system/bmp_winframe.h"
#include "system/cursor_move.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/0001.h"

// The battle bag's entry, task and states (b_bag_main.c, named by the ROM's embedded string): the bag runs one state
// function a frame, each returning the next state, until state 20 frees everything and sets the parameter's done.
// The pages are 0 the pockets, 1 a pocket's items and 2 one item.

static void func_ov197_0220c914(TCB *tcb, void *data);
static int func_ov197_0220c980(BBagWork *work);
static int func_ov197_0220caac(BBagWork *work);
static int func_ov197_0220cbb4(BBagWork *work);
static int func_ov197_0220cbe8(BBagWork *work);
static int func_ov197_0220cc18(BBagWork *work);
static int func_ov197_0220cd50(BBagWork *work);
static int func_ov197_0220cfa4(BBagWork *work);
static int func_ov197_0220cff0(BBagWork *work);
static int func_ov197_0220d024(BBagWork *work);
static int func_ov197_0220d11c(BBagWork *work);
static int func_ov197_0220d214(BBagWork *work);
static int func_ov197_0220d234(BBagWork *work);
static int func_ov197_0220d254(BBagWork *work);
static int func_ov197_0220d274(BBagWork *work);
static int func_ov197_0220d29c(BBagWork *work);
static int func_ov197_0220d2ac(BBagWork *work);
static int func_ov197_0220d328(BBagWork *work);
static int func_ov197_0220d358(BBagWork *work);
static int func_ov197_0220d370(BBagWork *work);
static int func_ov197_0220d39c(BBagWork *work);
static BOOL func_ov197_0220d3b0(TCB *tcb, BBagWork *work);
static int func_ov197_0220d464(BBagWork *work);
static void func_ov197_0220d5c0(BBagWork *work);
static void func_ov197_0220d6b8(void);
static void func_ov197_0220d6dc(BBagWork *work);
static void func_ov197_0220d798(BBagWork *work);
static void func_ov197_0220d7d0(BBagWork *work);
static void func_ov197_0220d7f0(BBagWork *work, u8 page);
static void func_ov197_0220d848(BBagWork *work, u8 page);
static BOOL func_ov197_0220d884(BBagWork *work);
static void func_ov197_0220d89c(BBagWork *work, u32 se);
static void func_ov197_0220d8b0(BBagWork *work, u32 se);

static int (*const data_ov197_0220df40[])(BBagWork *work) = {
    func_ov197_0220c980,          func_ov197_0220caac, func_ov197_0220cbb4,     func_ov197_0220cbe8,
    func_ov197_0220cc18,    func_ov197_0220cd50,     func_ov197_0220d024,       func_ov197_0220d214,
    func_ov197_0220d234,       func_ov197_0220d254,      func_ov197_0220d274, func_ov197_0220cfa4,
    func_ov197_0220cff0,  func_ov197_0220d29c, func_ov197_0220d2ac,  func_ov197_0220d328,
    func_ov197_0220d358, func_ov197_0220d464,         func_ov197_0220d370,     func_ov197_0220d39c,
};

// pokebw2's BBagMain_Start
void func_ov197_0220c834(BBagParam *param) {
    BBagWork *work = GFL_HeapAllocate(param->heapId, sizeof(BBagWork), TRUE, "b_bag_main.c", 177);
    u32 i;

    GFL_TCBMgrAddTask(func_ov094_021f86d4(), func_ov197_0220c914, work, 100);
    work->param = param;
    work->paletteFade = func_ov094_021f86e0();
    work->initialized = FALSE;
    if (work->param->mode == 1) {
        work->page = 1;
        work->seq = 1;
        work->pocket = 0;
    } else if (work->param->mode == 3) {
        work->page = 1;
        work->seq = 0;
        work->pocket = 2;
    } else {
        work->page = 0;
        work->seq = 0;
        work->pocket = 0;
    }
    if (work->param->mode != 1) {
        for (i = 0; i < 4; i++) {
            func_0200829c(work->param->bagCursor, i, &work->param->rows[i], &work->param->pages[i]);
        }
        work->lastItem = func_020082b0(work->param->bagCursor);
        work->lastPocket = func_020082b4(work->param->bagCursor);
        func_ov197_0220b9a4(work);
    } else {
        for (i = 0; i < 4; i++) {
            work->param->rows[i] = 0;
            work->param->pages[i] = 0;
        }
    }
}

// pokebw2's BBagMain_Task
static void func_ov197_0220c914(TCB *tcb, void *data) {
    BBagWork *work = data;

    if (work->param->abort == 1) {
        work->param->item = 0;
        work->param->cost = 0;
        work->seq = 20;
    }
    if (work->seq != 20) {
        work->seq = data_ov197_0220df40[work->seq](work);
    }
    if (work->seq == 20 && func_ov197_0220d3b0(tcb, work) == TRUE) {
        return;
    }
    GFL_TCBExMgrUpdate(work->tcbExMgr);
    func_ov197_0220bf40(work);
    func_0202cd94(work->buttons);
    func_ov197_0220b980(work);
}

// pokebw2's BBagMain_StateInit
static int func_ov197_0220c980(BBagWork *work) {
    G2S_BlendNone();
    GXS_SetMasterBrightness(-16);
    work->tcbExMgr = GFL_TCBExMgrCreate(work->param->heapId, work->param->heapId, 1, 4);
    work->cursor = func_ov196_0220af40(work->param->heapId);
    func_ov197_0220d5c0(work);
    func_ov197_0220d6dc(work);
    func_ov197_0220d798(work);
    if (work->param->mode == 2) {
        func_ov197_0220bb78(work);
    } else {
        func_ov197_0220ba2c(work);
    }
    func_ov197_0220b120(work);
    func_ov197_0220b1b0(work, work->page);
    func_ov197_0220b990(work);
    func_ov197_0220bbe4(work);
    func_ov197_0220bd24(work, work->page);
    func_ov197_0220c034(work);
    func_ov197_0220c4ec(work, work->page);
    if (*work->param->usingKeys == 1) {
        work->cursorVisible = TRUE;
    } else {
        work->cursorVisible = FALSE;
    }
    func_ov196_0220afdc(work->cursor, work->cursorVisible);
    if (work->page == 0) {
        func_ov197_0220d8c4(work, work->page, work->lastPocket);
    } else {
        func_ov197_0220d8c4(work, work->page, 0);
    }
    func_0203d34c(FALSE, work->param->heapId);
    func_020216d4(work->paletteFade, PALFADE_VRAM_SUB_OBJ, 0xe0, 0x20);
    func_0202173c(work->paletteFade, 0xa, 0xffff, 0, 16, 0, 0, func_ov094_021f86d4());
    if (work->param->mode == 3) {
        func_ov197_0220d7f0(work, work->page);
    }
    work->initialized = TRUE;
    return 2;
}

// pokebw2's BBagMain_StateInitLauncher
static int func_ov197_0220caac(BBagWork *work) {
    G2S_BlendNone();
    GXS_SetMasterBrightness(-16);
    work->tcbExMgr = GFL_TCBExMgrCreate(work->param->heapId, work->param->heapId, 1, 4);
    work->cursor = func_ov196_0220af40(work->param->heapId);
    func_ov197_0220d5c0(work);
    func_ov197_0220d6dc(work);
    func_ov197_0220d798(work);
    func_ov197_0220bb1c(work);
    func_ov197_0220d7f0(work, work->page);
    func_ov197_0220b120(work);
    func_ov197_0220b1b0(work, work->page);
    func_ov197_0220b990(work);
    func_ov197_0220bbe4(work);
    func_ov197_0220bd24(work, work->page);
    func_ov197_0220c034(work);
    func_ov197_0220c4ec(work, work->page);
    if (*work->param->usingKeys == 1) {
        work->cursorVisible = TRUE;
    } else {
        work->cursorVisible = FALSE;
    }
    func_ov196_0220afdc(work->cursor, work->cursorVisible);
    func_ov197_0220d8c4(work, work->page, 0);
    func_0203d34c(FALSE, work->param->heapId);
    func_020216d4(work->paletteFade, PALFADE_VRAM_SUB_OBJ, 0xe0, 0x20);
    func_0202173c(work->paletteFade, 0xa, 0xffff, 0, 16, 0, 0, func_ov094_021f86d4());
    work->initialized = TRUE;
    return 2;
}

// pokebw2's BBagMain_StateStartWipe
static int func_ov197_0220cbb4(BBagWork *work) {
    if (func_0201c6b4(work->printQueue) == TRUE) {
        GFL_WipeSet(WIPE_MODE_SUB, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, work->param->heapId);
        return 3;
    }
    return 2;
}

// pokebw2's BBagMain_StateWaitWipe
static int func_ov197_0220cbe8(BBagWork *work) {
    if (GFL_WipeIsFinished() == TRUE) {
        if (work->param->mode == 2) {
            return 0x11;
        }
        if (work->param->mode == 1) {
            return 5;
        }
        if (work->param->mode == 3) {
            return 5;
        }
        return 4;
    }
    return 3;
}

// pokebw2's BBagMain_StatePocketPage
static int func_ov197_0220cc18(BBagWork *work) {
    u32 ret;

    if (func_02021ed8(work->paletteFade) == 0) {
        if (func_ov197_0220d884(work) == TRUE) {
            return 0x12;
        }
        ret = func_02025de8(work->cursorMove);
        switch (ret) {
        case 0:
        case 1:
        case 2:
        case 3:
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->pocket = ret;
            work->nextSeq = 8;
            func_ov197_0220bf20(work, ret);
            return 0x10;
        case 4:
            if (work->lastItem == 0) {
                break;
            }
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->pocket = work->lastPocket;
            work->nextSeq = 9;
            func_ov197_0220b9d8(work);
            func_ov197_0220bf20(work, 13);
            return 0x10;
        case 5:
            func_ov197_0220d89c(work, SEQ_SE_CANCEL2);
            work->param->item = 0;
            work->param->pocket = 4;
            func_ov197_0220bf20(work, 10);
            return 0x12;
        case CURSOR_MOVE_CANCEL:
            func_ov197_0220d89c(work, SEQ_SE_CANCEL2);
            work->param->item = 0;
            work->param->pocket = 4;
            func_ov197_0220bf20(work, 10);
            return 0x12;
        case CURSOR_MOVE_MOVED:
        case CURSOR_MOVE_CURSOR_ON:
            func_ov197_0220d89c(work, SEQ_SE_SELECT1);
            break;
        case CURSOR_MOVE_NONE:
        case CURSOR_MOVE_EDGE_RIGHT:
        case CURSOR_MOVE_EDGE_LEFT:
        case CURSOR_MOVE_EDGE_DOWN:
        case CURSOR_MOVE_EDGE_UP:
            break;
        }
    }
    return 4;
}

// pokebw2's BBagMain_StateItemPage
static int func_ov197_0220cd50(BBagWork *work) {
    u32 ret;

    if (func_02021ed8(work->paletteFade) == 0) {
        if (func_ov197_0220d884(work) == TRUE) {
            return 0x12;
        }
        ret = func_02025de8(work->cursorMove);
        switch (ret) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (func_ov197_0220bb98(work, ret) == 0) {
                break;
            }
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->param->rows[work->pocket] = (u8)ret;
            work->nextSeq = 9;
            func_ov197_0220bf20(work, ret + 4);
            return 0x10;
        case 6:
            func_ov197_0220d89c(work, SEQ_SE_CANCEL2);
            func_ov197_0220bf20(work, 10);
            if (work->param->mode == 1 || work->param->mode == 3) {
                work->param->item = 0;
                work->param->pocket = 4;
                return 0x12;
            }
            work->nextSeq = 7;
            return 0x10;
        case CURSOR_MOVE_CANCEL:
            func_ov197_0220d89c(work, SEQ_SE_CANCEL2);
            func_ov197_0220bf20(work, 10);
            if (work->param->mode == 1 || work->param->mode == 3) {
                work->param->item = 0;
                work->param->pocket = 4;
                return 0x12;
            }
            work->nextSeq = 7;
            return 0x10;
        case 7:
            if (work->lastPage[work->pocket] == 0) {
                break;
            }
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->param->rows[work->pocket] = 0;
            work->nextSeq = 0xb;
            work->pageStep = -1;
            func_ov197_0220bf20(work, 11);
            return 0x10;
        case 8:
            if (work->lastPage[work->pocket] == 0) {
                break;
            }
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->param->rows[work->pocket] = 0;
            work->nextSeq = 0xb;
            work->pageStep = 1;
            func_ov197_0220bf20(work, 12);
            return 0x10;
        case CURSOR_MOVE_MOVED:
        case CURSOR_MOVE_CURSOR_ON:
            func_ov197_0220d89c(work, SEQ_SE_SELECT1);
            break;
        case CURSOR_MOVE_EDGE_LEFT:
            func_020260e0(work->cursorMove); // unused
            if (work->lastPage[work->pocket] == 0) {
                break;
            }
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->nextSeq = 0xb;
            work->pageStep = -1;
            func_ov197_0220bf20(work, 11);
            return 0x10;
        case CURSOR_MOVE_EDGE_RIGHT:
            func_020260e0(work->cursorMove); // unused
            if (work->lastPage[work->pocket] == 0) {
                break;
            }
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->nextSeq = 0xb;
            work->pageStep = 1;
            func_ov197_0220bf20(work, 12);
            return 0x10;
        case CURSOR_MOVE_NONE:
        case CURSOR_MOVE_EDGE_DOWN:
        case CURSOR_MOVE_EDGE_UP:
            break;
        }
    }
    return 5;
}

// pokebw2's BBagMain_StateStepPage
static int func_ov197_0220cfa4(BBagWork *work) {
    s8 page = work->param->pages[work->pocket];

    page += work->pageStep;
    if (page > work->lastPage[work->pocket]) {
        work->param->pages[work->pocket] = 0;
    } else if (page < 0) {
        work->param->pages[work->pocket] = work->lastPage[work->pocket];
    } else {
        work->param->pages[work->pocket] = page;
    }
    func_ov197_0220b520(work);
    func_ov197_0220b584(work);
    return 0xc;
}

// pokebw2's BBagMain_StateWaitStepPage
static int func_ov197_0220cff0(BBagWork *work) {
    if (func_0201c6b4(work->printQueue) == TRUE) {
        func_ov197_0220b990(work);
        func_ov197_0220c4ec(work, work->page);
        func_ov197_0220bd24(work, work->page);
        return 5;
    }
    return 0xc;
}

// pokebw2's BBagMain_StateUsePage
static int func_ov197_0220d024(BBagWork *work) {
    if (func_ov197_0220d884(work) == TRUE) {
        return 0x12;
    }
    switch (func_02025de8(work->cursorMove)) {
    case 0:
        func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
        work->param->item = func_ov197_0220bb98(work, work->param->rows[work->pocket]);
        work->param->pocket = work->pocket;
        if (work->param->mode == 1) {
            work->param->cost = func_ov197_0220bbd8(work->param->item);
        } else {
            work->param->cost = 0;
        }
        func_ov197_0220bf20(work, 13);
        return func_ov197_0220d11c(work);
    case 1:
        func_ov197_0220d89c(work, SEQ_SE_CANCEL2);
        work->nextSeq = 8;
        func_ov197_0220bf20(work, 10);
        return 0x10;
    case CURSOR_MOVE_CANCEL:
        func_ov197_0220d89c(work, SEQ_SE_CANCEL2);
        work->nextSeq = 8;
        func_ov197_0220bf20(work, 10);
        return 0x10;
    case CURSOR_MOVE_MOVED:
    case CURSOR_MOVE_CURSOR_ON:
        func_ov197_0220d89c(work, SEQ_SE_SELECT1);
        break;
    case CURSOR_MOVE_NONE:
    case CURSOR_MOVE_EDGE_RIGHT:
    case CURSOR_MOVE_EDGE_LEFT:
    case CURSOR_MOVE_EDGE_DOWN:
    case CURSOR_MOVE_EDGE_UP:
        break;
    }
    return 6;
}

// The refusals of USE: the Launcher's energy, why a Poké Ball can't be thrown, and an escape item outside a wild
// battle. Each prints its message and returns to the item page
// pokebw2's BBagMain_CheckUse
static int func_ov197_0220d11c(BBagWork *work) {
    BBagParam *param = work->param;

    if (param->mode == 1 && param->shooterEnergy < func_ov197_0220bbd8(param->item)) {
        GFL_MsgDataLoadStrbuf(work->msgData, Bank0001_Text_049, work->strBuf);
        func_ov197_0220b904(work);
        work->nextSeq = 0xd;
        return 0xe;
    }
    if (work->pocket == 2) {
        if (param->ballError == 1) {
            GFL_MsgDataLoadStrbuf(work->msgData, Bank0001_Text_045, work->strBuf);
            func_ov197_0220b904(work);
            work->nextSeq = 0xd;
            return 0xe;
        }
        if (param->ballError == 2) {
            GFL_MsgDataLoadStrbuf(work->msgData, Bank0001_Text_044, work->strBuf);
            func_ov197_0220b904(work);
            work->nextSeq = 0xd;
            return 0xe;
        }
        if (param->ballError == 3) {
            GFL_MsgDataLoadStrbuf(work->msgData, Bank0001_Text_051, work->strBuf);
            func_ov197_0220b904(work);
            work->nextSeq = 0xd;
            return 0xe;
        }
        if (param->ballError == 4) {
            GFL_MsgDataLoadStrbuf(work->msgData, Bank0001_Text_047, work->strBuf);
            func_ov197_0220b904(work);
            work->nextSeq = 0xd;
            return 0xe;
        }
    }
    if (param->isWild == FALSE && GetItemParam(param->item, ITEM_PARAM_BATTLE_FUNC, param->heapId) == 3) {
        GFL_MsgDataLoadStrbuf(work->msgData, Bank0001_Text_050, work->strBuf);
        func_ov197_0220b904(work);
        work->nextSeq = 0xd;
        return 0xe;
    }
    return 0x12;
}

// pokebw2's BBagMain_StateToPage0
static int func_ov197_0220d214(BBagWork *work) {
    func_ov197_0220b1b0(work, 0);
    work->page = 0;
    work->nextSeq = 4;
    return 0xa;
}

// pokebw2's BBagMain_StateToPage1
static int func_ov197_0220d234(BBagWork *work) {
    func_ov197_0220b1b0(work, 1);
    work->page = 1;
    work->nextSeq = 5;
    return 0xa;
}

// pokebw2's BBagMain_StateToPage2
static int func_ov197_0220d254(BBagWork *work) {
    func_ov197_0220b1b0(work, 2);
    work->page = 2;
    work->nextSeq = 6;
    return 0xa;
}

// pokebw2's BBagMain_StateWaitPagePrint
static int func_ov197_0220d274(BBagWork *work) {
    if (func_0201c6b4(work->printQueue) == TRUE) {
        func_ov197_0220d848(work, work->page);
        return work->nextSeq;
    }
    return 0xa;
}

// pokebw2's BBagMain_StateCloseMessage
static int func_ov197_0220d29c(BBagWork *work) {
    func_0201f7a8(work->msgWin, 0);
    return 6;
}

// pokebw2's BBagMain_StatePrintMessage
static int func_ov197_0220d2ac(BBagWork *work) {
    switch (func_0201ce30(work->printStream)) {
    case PRINT_STREAM_RUNNING:
        if (func_02035c60() == TRUE || (GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B))) {
            func_0201ce5c(work->printStream, 0);
        }
        work->streamAdvanced = FALSE;
        break;
    case PRINT_STREAM_PAUSED:
        if (work->streamAdvanced == FALSE &&
            (func_02035c60() == TRUE || (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)))) {
            func_ov197_0220d8b0(work, SEQ_SE_MESSAGE);
            func_0201ce38(work->printStream);
            work->streamAdvanced = TRUE;
        }
        break;
    case PRINT_STREAM_DONE:
        func_0201ce48(work->printStream);
        work->printStream = NULL;
        work->streamAdvanced = FALSE;
        return 0xf;
    }
    return 0xe;
}

// pokebw2's BBagMain_StateWaitMessageKey
static int func_ov197_0220d328(BBagWork *work) {
    if (func_ov197_0220d884(work) == TRUE) {
        return 0x12;
    }
    if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60() == TRUE) {
        return work->nextSeq;
    }
    return 0xf;
}

// pokebw2's BBagMain_StateWaitButtonAnm
static int func_ov197_0220d358(BBagWork *work) {
    if (work->animActive == FALSE) {
        return work->nextSeq;
    }
    return 0x10;
}

// pokebw2's BBagMain_StateStartExit
static int func_ov197_0220d370(BBagWork *work) {
    func_0202173c(work->paletteFade, 0xa, 0xffff, 0, 0, 16, 0, func_ov094_021f86d4());
    return 0x13;
}

// pokebw2's BBagMain_StateWaitFadeOut
static int func_ov197_0220d39c(BBagWork *work) {
    if (func_02021ed8(work->paletteFade) == 0) {
        return 20;
    }
    return 0x13;
}

// pokebw2's BBagMain_Exit
static BOOL func_ov197_0220d3b0(TCB *tcb, BBagWork *work) {
    if (work->initialized == TRUE) {
        if (func_0201c6b4(work->printQueue) == FALSE) {
            if (work->param->abort == 1) {
                func_0201c6ec(work->printQueue);
            } else {
                return FALSE;
            }
        }
        if (GFL_WipeIsFinished() == FALSE) {
            if (work->param->abort == 1) {
                func_020221b8();
            } else {
                return FALSE;
            }
        }
        if (work->printStream != NULL) {
            func_0201ce48(work->printStream);
        }
        func_ov197_0220bd14(work);
        func_ov197_0220c448(work);
        func_ov197_0220b1a0(work);
        func_ov197_0220d7d0(work);
        func_ov197_0220d6b8();
        GFL_TCBExMgrFree(work->tcbExMgr);
        func_ov197_0220d914(work);
        func_ov196_0220af64(work->cursor);
        if (work->cursorVisible == TRUE) {
            *work->param->usingKeys = TRUE;
        } else {
            *work->param->usingKeys = FALSE;
        }
    }
    work->param->done = TRUE;
    GFL_TCBRemove(tcb);
    GFL_HeapFree(work);
    return TRUE;
}

// pokebw2's BBagMain_StateDemo
static int func_ov197_0220d464(BBagWork *work) {
    switch (work->demoSeq) {
    case 0:
        if (func_ov094_02209c2c(work->fingerCursor) == TRUE) {
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->pocket = 2;
            work->nextSeq = 0x11;
            func_ov197_0220bf20(work, 2);
            work->demoSeq++;
            return 0x10;
        }
        break;
    case 1:
        if (func_ov094_02209c2c(work->fingerCursor) == FALSE) {
            func_ov197_0220d234(work);
            work->demoSeq++;
            work->nextSeq = 0x11;
            return 0xa;
        }
        break;
    case 2:
        if (func_ov094_02209c2c(work->fingerCursor) == TRUE) {
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->param->rows[work->pocket] = 0;
            work->nextSeq = 0x11;
            func_ov197_0220bf20(work, 4);
            work->demoSeq++;
            return 0x10;
        }
        break;
    case 3:
        if (func_ov094_02209c2c(work->fingerCursor) == FALSE) {
            func_ov197_0220d254(work);
            work->demoSeq++;
            work->nextSeq = 0x11;
            return 0xa;
        }
        break;
    case 4:
        if (func_ov094_02209c2c(work->fingerCursor) == TRUE) {
            func_ov197_0220d89c(work, SEQ_SE_DECIDE2);
            work->param->item = func_ov197_0220bb98(work, work->param->rows[work->pocket]);
            work->param->pocket = work->pocket;
            work->param->cost = 0;
            func_ov197_0220bf20(work, 13);
            work->demoSeq++;
        }
        break;
    case 5:
        if (func_ov094_02209c2c(work->fingerCursor) == FALSE) {
            return func_ov197_0220d11c(work);
        }
        break;
    }
    return 0x11;
}

// pokebw2's BBagMain_InitBG
static void func_ov197_0220d5c0(BBagWork *work) {
    {
        BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_3D };

        GFL_BGSysSetLCDConfigForEngine(&lcdConfig, BGSYS_ENGINE_SUB);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xc800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_23,
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x2000,
            0,
            BGRES_512x512,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_23,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(5);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x08000,
            0x4000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(4);
    }
    GFL_BGSysClearCharCore(5, BGSYS_TILE_SIZE_16, 0, work->param->heapId);
    GFL_BGSysClearCharCore(4, BGSYS_TILE_SIZE_16, 0, work->param->heapId);
    GFL_BGSysQueueScrLoad(5);
    GFL_BGSysQueueScrLoad(4);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
}

// pokebw2's BBagMain_ExitBG
static void func_ov197_0220d6b8(void) {
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ,
                           FALSE);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(7);
}

// pokebw2's BBagMain_LoadGraphics
static void func_ov197_0220d6dc(BBagWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(99, HEAPID_TAIL(work->param->heapId));

    GFL_BGSysLoadArcNCGRStatic(arc, 0, 6, 0, 0, TRUE, work->param->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 3, 6, 0, 0, TRUE, work->param->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, 7, 0, 0, TRUE, work->param->heapId);
    func_020216b4(work->paletteFade, arc, 1, work->param->heapId, PALFADE_BUFFER_SUB_BG, 0xe0, 0);
    GFL_ArcToolFree(arc);
    LoadSysMsgBox(4, 1, 11, 0, work->param->heapId);
    func_020216d4(work->paletteFade, PALFADE_VRAM_SUB_BG, 0xb0, 0x20);
    func_02021640(work->paletteFade, ARCID_FONT, 5, work->param->heapId, PALFADE_BUFFER_SUB_BG, 0x20, 0xc0);
}

// pokebw2's BBagMain_InitText
static void func_ov197_0220d798(BBagWork *work) {
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0001, work->param->heapId);
    work->wordSet = GFL_WordSetSystemCreateDefault(work->param->heapId);
    work->printQueue = func_0201c440(work->param->heapId);
    work->strBuf = GFL_StrBufCreate(0x200, work->param->heapId);
}

// pokebw2's BBagMain_ExitText
static void func_ov197_0220d7d0(BBagWork *work) {
    GFL_MsgDataFree(work->msgData);
    GFL_WordSetSystemFree(work->wordSet);
    func_0201c4c0(work->printQueue);
    GFL_StrBufFree(work->strBuf);
}

// pokebw2's BBagMain_ScrollPage
static void func_ov197_0220d7f0(BBagWork *work, u8 page) {
    switch (page) {
    case 0:
        GFL_BGSysMoveBGReq(6, BG_MOVE_SET_X, 0);
        GFL_BGSysMoveBGReq(6, BG_MOVE_SET_Y, 0);
        break;
    case 1:
        GFL_BGSysMoveBGReq(6, BG_MOVE_SET_X, 0x100);
        GFL_BGSysMoveBGReq(6, BG_MOVE_SET_Y, 0);
        break;
    case 2:
        GFL_BGSysMoveBGReq(6, BG_MOVE_SET_X, 0);
        GFL_BGSysMoveBGReq(6, BG_MOVE_SET_Y, 0x100);
        break;
    }
}

// pokebw2's BBagMain_ChangePage
static void func_ov197_0220d848(BBagWork *work, u8 page) {
    func_ov197_0220d7f0(work, page);
    GFL_BGSysFillScrAsync(4, 0);
    GFL_BGSysFillScrAsync(5, 0);
    func_ov197_0220b990(work);
    func_ov197_0220bd24(work, page);
    func_ov197_0220d924(work, page, 0);
    func_ov197_0220c4ec(work, page);
}

// pokebw2's BBagMain_CheckQuit
static BOOL func_ov197_0220d884(BBagWork *work) {
    if (work->param->quit == 1) {
        work->param->item = 0;
        work->param->cost = 0;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BBagMain_PlaySE
static void func_ov197_0220d89c(BBagWork *work, u32 se) {
    if (work->param->playSound == 1) {
        GFL_SndSEPlay(se);
    }
}

// pokebw2's BBagMain_PlayMessageSE
static void func_ov197_0220d8b0(BBagWork *work, u32 se) {
    if (work->param->playSound == 1) {
        GFL_SndSEPlay(se);
    }
}
