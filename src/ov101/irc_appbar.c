// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov175/irc_appbar.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/irc_appbar.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/mi.h"
#include "system/app_taskmenu.h"
#include "text/system/btl_main_4.h"

// irc_appbar.c, a ROM name: the compatibility checker's CANCEL and BACK button. The names are descriptive guesses

#define ACTOR_COUNT 7

// The steps of the button
enum {
    STATE_WAIT_TOUCH,
    STATE_FLASH,
    STATE_PRESSED,
};

struct IrcAppBar {
    AppTaskMenuWin *win;
    AppTaskMenuRes *res;
    ClActor *actors[ACTOR_COUNT];
    u8 unk24[0xc];
    u32 palRes;
    u32 charRes;
    u32 cellRes;
    u32 bg;
    u32 unk40;
    int result;
    int unk48;
    u8 unk4C[4];
    int state;
    int label;
    u16 heapId;
    int active;
};

static void func_ov101_021b732c(IrcAppBar *bar, u32 vramType, u32 unused2, u32 unused3, u32 unused4, u8 objPalette,
                                 HeapID heapId);
static void func_ov101_021b7384(IrcAppBar *bar);
static void func_ov101_021b739c(IrcAppBar *bar, ClActUnit *unit, u32 unused2, u32 surface, u8 priority,
                                  HeapID heapId);
static void func_ov101_021b73e8(IrcAppBar *bar);

IrcAppBar *func_ov101_021b70dc(u32 unused0, ClActUnit *unit, u8 bg, u8 palette, u8 objPalette, u32 unused5, Font *font,
                            PrintQueue *queue, HeapID heapId) {
    IrcAppBar *bar = GFL_HeapAllocate(heapId, sizeof(IrcAppBar), FALSE, "irc_appbar.c", 125);
    MsgData *msgData;
    AppTaskMenuItem item;
    BOOL isSub = FALSE;
    u32 vramType;
    u32 surface;

    MI_CpuFill8(bar, 0, sizeof(IrcAppBar));
    bar->bg = bg;
    bar->result = -1;
    bar->unk48 = -1;
    bar->label = 0;
    bar->heapId = heapId;
    bar->active = TRUE;
    if (bg >= 4) {
        isSub = TRUE;
        vramType = 1;
        surface = 4;
    } else {
        vramType = 0;
        surface = 0;
    }
    func_ov101_021b732c(bar, vramType, surface, unused5, palette, objPalette, heapId);
    bar->res = func_02027f60(bg, palette, font, queue, heapId);
    func_ov101_021b739c(bar, unit, unused0, isSub, GFL_BGSysGetBGPriority(bg), heapId);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_4, heapId);
    MI_CpuFill8(&item, 0, sizeof(AppTaskMenuItem));
    item.color = 0x39e3;
    item.str = GFL_MsgDataLoadStrbufNew(msgData, BtlMain4_Text_008);
    item.type = APP_TASKMENU_ITEM_RETURN;
    bar->win = func_02027fec(bar->res, &item, 19, 21, 13, heapId);
    GFL_StrBufFree(item.str);
    GFL_MsgDataFree(msgData);
    return bar;
}

// pokebw2's IrcAppBar_Free
void func_ov101_021b71c0(IrcAppBar *bar) {
    func_02028148(bar->win);
    func_ov101_021b73e8(bar);
    func_02027fd8(bar->res);
    func_ov101_021b7384(bar);
    GFL_HeapFree(bar);
}

// pokebw2's IrcAppBar_Update
void func_ov101_021b71e4(IrcAppBar *bar) {
    if (bar->active) {
        switch (bar->state) {
        case STATE_WAIT_TOUCH:
            bar->result = -1;
            if (func_0202824c(bar->win)) {
                GFL_SndSEPlay(SEQ_SE_CANCEL1);
                func_0202822c(bar->win, TRUE);
                bar->state = STATE_FLASH;
            }
            break;
        case STATE_FLASH:
            if (func_02028230(bar->win)) {
                func_020282a4(bar->win);
                func_02028218(bar->win, FALSE);
                bar->state = STATE_PRESSED;
            }
            break;
        case STATE_PRESSED:
            bar->result = 1;
            break;
        }
        func_02028178(bar->win);
    }
}

// pokebw2's IrcAppBar_GetResult
int func_ov101_021b7250(IrcAppBar *bar) {
    return bar->result;
}

// pokebw2's IrcAppBar_IsPressed
BOOL func_ov101_021b7254(IrcAppBar *bar) {
    if (bar->state > 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcAppBar_Reset
void func_ov101_021b7264(IrcAppBar *bar) {
    func_020282a4(bar->win);
    func_02028218(bar->win, FALSE);
    bar->state = STATE_WAIT_TOUCH;
}

// pokebw2's IrcAppBar_IsPressedOrDone
BOOL func_ov101_021b727c(IrcAppBar *bar) {
    if (bar->state != 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IrcAppBar_SetVisible
void func_ov101_021b728c(IrcAppBar *bar, BOOL visible) {
    bar->active = visible;
    GFL_BGSysSetBGEnabled(bar->bg, visible);
}

// pokebw2's IrcAppBar_SetActive
void func_ov101_021b72a0(IrcAppBar *bar, BOOL active) {
    bar->active = active;
    bar->state = STATE_WAIT_TOUCH;
}

// pokebw2's IrcAppBar_SetLabel
void func_ov101_021b72a8(IrcAppBar *bar, int label) {
    if (label != bar->label) {
        MsgData *msgData;
        AppTaskMenuItem item;

        func_02028148(bar->win);
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_4, bar->heapId);
        MI_CpuFill8(&item, 0, sizeof(AppTaskMenuItem));
        item.color = 0x39e3;
        item.str = GFL_MsgDataLoadStrbufNew(msgData, BtlMain4_Text_008 + label);
        item.type = APP_TASKMENU_ITEM_RETURN;
        bar->win = func_02027fec(bar->res, &item, 19, 21, 13, bar->heapId);
        GFL_StrBufFree(item.str);
        GFL_MsgDataFree(msgData);
        bar->label = label;
        bar->state = STATE_WAIT_TOUCH;
        bar->result = -1;
    }
}

static void func_ov101_021b732c(IrcAppBar *bar, u32 vramType, u32 unused2, u32 unused3, u32 unused4, u8 objPalette,
                                 HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(94, heapId);

    bar->palRes = func_0204ac18(arc, 41, vramType, (u16)(objPalette << 5), 0, 1, heapId);
    bar->charRes = func_0204a6c8(arc, 40, FALSE, vramType, heapId);
    bar->cellRes = func_0204af28(arc, 39, 38, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's IrcAppBar_FreeObjRes
static void func_ov101_021b7384(IrcAppBar *bar) {
    func_0204afd8(bar->cellRes);
    func_0204a8d4(bar->charRes);
    func_0204ada4(bar->palRes);
}

static void func_ov101_021b739c(IrcAppBar *bar, ClActUnit *unit, u32 unused2, u32 surface, u8 priority,
                                  HeapID heapId) {
    ClActorSetup setup;

    MI_CpuFill8(&setup, 0, sizeof(ClActorSetup));
    setup.x = 128;
    setup.y = 96;
    setup.sequence = 0;
    setup.bgPriority = priority + 1;
    bar->actors[0] = func_0204b294(unit, bar->charRes, bar->palRes, bar->cellRes, &setup, surface, heapId);
}

// pokebw2's IrcAppBar_FreeActors
static void func_ov101_021b73e8(IrcAppBar *bar) {
    int i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        if (bar->actors[i] != NULL) {
            func_0204b3b4(bar->actors[i]);
        }
    }
}
