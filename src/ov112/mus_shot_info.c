// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov209/mus_shot_info.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/musical/mus_shot_info.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/musical.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "save/musical_savedata.h"
#include "save/save_control.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wipe.h"
#include "text/system/btl_pokeparam_2.h"
#include "field/mus_comm_func.h"

// Overlay 209's mus_shot_info.c: the touch screen under a musical's photo. Right after a musical it asks whether to
// keep the photo, replacing the one in the save; otherwise it waits for the return button

enum {
    MUS_SHOT_INFO_STATE_START,
    MUS_SHOT_INFO_STATE_WAIT_WIPE,
    MUS_SHOT_INFO_STATE_WAIT_MESSAGE,
    MUS_SHOT_INFO_STATE_MENU,
    // Synchronizes with the other participants before the exit
    MUS_SHOT_INFO_STATE_SYNC,
    MUS_SHOT_INFO_STATE_WAIT_RETURN,
    MUS_SHOT_INFO_STATE_RETURN,
    MUS_SHOT_INFO_STATE_END,
};

// The message of the musical's system messages that the communication waits with
#define MUS_SHOT_INFO_SYNC_ID 71

struct MusShotInfo {
    HeapID heapId;
    u32 state;
    // Frames until the background scrolls again
    u8 scrollTimer;
    // Whether it asks to keep the photo, rather than showing the return button
    BOOL askSave;
    MusicalShot *shot;
    MusicalSave *save;
    TCBExManager *tcbManager;
    BmpWin *msgWin;
    Font *font;
    PrintStream *stream;
    MsgData *msgData;
    StrBuf *msgStr;
    // Whether a message is in the print queue, to show the wait icon once it is printed
    BOOL printing;
    WaitIcon *waitIcon;
    PrintQueue *printQueue;
    AppTaskMenu *menu;
    AppTaskMenuRes *menuRes;
    // Overlay 211's communication work, or NULL
    void *comm;
    ClActUnit *clactUnit;
    u32 returnPalette;
    u32 returnChars;
    u32 returnCellAnims;
    ClActor *returnButton;
};

static void func_ov112_021f44f8(MusShotInfo *info);
static void func_ov112_021f4684(const BGSetup *setup, u8 bg, u8 mode);
static void func_ov112_021f46a8(MusShotInfo *info);
static void func_ov112_021f4748(MusShotInfo *info);
static void func_ov112_021f4794(MusShotInfo *info, u32 msgId);
static void func_ov112_021f4804(MusShotInfo *info, u32 msgId);
static void func_ov112_021f485c(MusShotInfo *info);

static const TouchRect data_ov112_021f8290[] = {
    { 168, 192, 232, 255 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

static const BGSetup data_ov112_021f82b8 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7000,
    GX_BG_CHARBASE_0x00000,
    0x4000,
    GX_BG_EXTPLTT_01,
    2,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const BGSetup data_ov112_021f8298 = {
    0,
    0,
    0x1000,
    0,
    BGRES_512x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x6000,
    GX_BG_CHARBASE_0x04000,
    0x2000,
    GX_BG_EXTPLTT_01,
    0,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

static const BGSetup data_ov112_021f82d8 = {
    0,
    0,
    0x800,
    0,
    BGRES_256x256,
    GX_BG_COLORMODE_16,
    GX_BG_SCRBASE_0x7800,
    GX_BG_CHARBASE_0x00000,
    0,
    GX_BG_EXTPLTT_01,
    3,
    GX_BG_AREAOVER_XLU,
    FALSE,
};

// Nothing in the game refers to these
const u32 data_ov112_021f82f8[2] = { 1, 1 };

// pokebw2's MusShotInfo_Create
MusShotInfo *func_ov112_021f4284(MusicalShot *shot, MusicalSave *save, BOOL askSave, void *comm, HeapID heapId) {
    MusShotInfo *info = GFL_HeapAllocate(heapId, sizeof(MusShotInfo), TRUE, "mus_shot_info.c", 141);

    info->heapId = heapId;
    info->askSave = askSave;
    info->shot = shot;
    info->save = save;
    info->comm = comm;
    info->msgStr = NULL;
    info->scrollTimer = 0;
    if (askSave == TRUE) {
        info->state = MUS_SHOT_INFO_STATE_START;
    } else {
        info->state = MUS_SHOT_INFO_STATE_WAIT_WIPE;
    }
    func_ov112_021f44f8(info);
    func_ov112_021f46a8(info);
    info->menuRes = func_02027f60(4, 12, info->font, info->printQueue, info->heapId);
    func_0203d34c(FALSE, heapId);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    return info;
}

// pokebw2's MusShotInfo_Delete
void func_ov112_021f4300(MusShotInfo *info) {
    func_02027fd8(info->menuRes);
    if (info->askSave == FALSE) {
        func_0204ada4(info->returnPalette);
        func_0204a8d4(info->returnChars);
        func_0204afd8(info->returnCellAnims);
        func_0204b3b4(info->returnButton);
    }
    func_0204b1cc(info->clactUnit);
    func_ov112_021f4748(info);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(7);
    GFL_HeapFree(info);
}

// pokebw2's MusShotInfo_Main
void func_ov112_021f4350(MusShotInfo *info) {
    u8 pos;

    switch (info->state) {
    case MUS_SHOT_INFO_STATE_START:
        info->state = MUS_SHOT_INFO_STATE_WAIT_WIPE;
        GFL_SndSEPlay(SEQ_SE_MSCL_14);
        // fallthrough
    case MUS_SHOT_INFO_STATE_WAIT_WIPE:
        if (GFL_WipeIsFinished() == TRUE) {
            if (info->askSave == TRUE) {
                func_ov112_021f4794(info, 3);
                info->state = MUS_SHOT_INFO_STATE_WAIT_MESSAGE;
            } else {
                info->state = MUS_SHOT_INFO_STATE_WAIT_RETURN;
            }
        }
        break;
    case MUS_SHOT_INFO_STATE_WAIT_MESSAGE:
        if (info->stream == NULL) {
            func_ov112_021f485c(info);
            info->state = MUS_SHOT_INFO_STATE_MENU;
        }
        break;
    case MUS_SHOT_INFO_STATE_MENU:
        func_02027974(info->menu);
        if (func_020279e8(info->menu) == TRUE) {
            pos = func_02027a04(info->menu);
            func_02027858(info->menu);
            if (pos == 0) {
                MI_CpuCopy8(info->shot, func_0200a640(info->save), sizeof(MusicalShot));
            }
            if (info->comm != NULL) {
                info->state = MUS_SHOT_INFO_STATE_SYNC;
                func_ov132_021fcdec(info->comm, MUS_SHOT_INFO_SYNC_ID);
                func_ov112_021f4804(info, 6);
            } else {
                info->state = MUS_SHOT_INFO_STATE_END;
            }
        } else if (func_0200fb24()) {
            func_02027858(info->menu);
            info->state = MUS_SHOT_INFO_STATE_END;
        }
        break;
    case MUS_SHOT_INFO_STATE_SYNC:
        if (func_ov132_021fce00(info->comm, MUS_SHOT_INFO_SYNC_ID) == TRUE) {
            info->state = MUS_SHOT_INFO_STATE_END;
        } else if (func_0200fb24()) {
            info->state = MUS_SHOT_INFO_STATE_END;
        }
        break;
    case MUS_SHOT_INFO_STATE_WAIT_RETURN:
        if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) || func_02035c08(data_ov112_021f8290) == 0) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            info->state = MUS_SHOT_INFO_STATE_RETURN;
            func_0204b878(info->returnButton, 9);
            func_0204b92c(info->returnButton, TRUE);
        }
        break;
    case MUS_SHOT_INFO_STATE_RETURN:
        if (func_0204b98c(info->returnButton) == FALSE) {
            info->state = MUS_SHOT_INFO_STATE_END;
        }
        break;
    case MUS_SHOT_INFO_STATE_END:
        break;
    }

    if (info->stream != NULL && func_0201ce30(info->stream) == PRINT_STREAM_DONE) {
        func_0201ce48(info->stream);
        info->stream = NULL;
    }
    func_0201c4e4(info->printQueue);
    if (info->printing == TRUE && func_0201c6b4(info->printQueue) == TRUE) {
        info->printing = FALSE;
        info->waitIcon = func_0202ee8c(info->tcbManager, info->msgWin, 15, 16, info->heapId);
        BmpWin_FlushChar(info->msgWin);
    }
    GFL_TCBExMgrUpdate(info->tcbManager);
    info->scrollTimer++;
    if (info->scrollTimer > 1) {
        info->scrollTimer = 0;
        GFL_BGSysMoveBGReq(7, BG_MOVE_RIGHT, 1);
        GFL_BGSysMoveBGReq(7, BG_MOVE_UP, 1);
    }
}

// pokebw2's MusShotInfo_IsFinished
BOOL func_ov112_021f44e8(MusShotInfo *info) {
    if (info->state == MUS_SHOT_INFO_STATE_END) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's MusShotInfo_InitGraphics
static void func_ov112_021f44f8(MusShotInfo *info) {
    ArcTool *arc;

    func_ov112_021f4684(&data_ov112_021f8298, 4, 0);
    func_ov112_021f4684(&data_ov112_021f82b8, 6, 0);
    func_ov112_021f4684(&data_ov112_021f82d8, 7, 0);
    info->clactUnit = func_0204b100(8, 0, info->heapId);
    func_0204b270(info->clactUnit);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);

    arc = GFL_ArcSysCreateFileHandle(ARCID_MUSICAL_SHOT, info->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 4, 4, 0, 0, info->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 7, 7, 0, 0, FALSE, info->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 12, 7, 0, 0, FALSE, info->heapId);
    GFL_ArcToolFree(arc);

    if (info->askSave == FALSE) {
        ClActorSetup setup;

        arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), info->heapId);
        GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), 4, 0x120, 0x20, info->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, func_02027628(), 6, 0, 0, FALSE, info->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, func_0202762c(), 6, 0, 0, FALSE, info->heapId);
        GFL_BGSysSetScrPaletteNo(6, 0, 21, 32, 3, 9);
        GFL_BGSysLoadScr(6);
        info->returnPalette = func_0204ac18(arc, func_02027614(), CLACT_VRAM_SUB, 0, 0, 3, info->heapId);
        info->returnChars = func_0204a6c8(arc, func_02027618(), FALSE, CLACT_VRAM_SUB, info->heapId);
        info->returnCellAnims = func_0204af28(arc, func_0202761c(0), func_02027620(0), info->heapId);
        GFL_ArcToolFree(arc);

        setup.x = 232;
        setup.y = 168;
        setup.sequence = 1;
        setup.bgPriority = 0;
        setup.priority = 0;
        info->returnButton = func_0204b294(info->clactUnit, info->returnChars, info->returnPalette,
                                           info->returnCellAnims, &setup, CLACT_SURFACE_SUB, info->heapId);
    }
}

// pokebw2's MusShotInfo_InitBG
static void func_ov112_021f4684(const BGSetup *setup, u8 bg, u8 mode) {
    GFL_BGSysCreateBG(bg, setup, mode);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

// pokebw2's MusShotInfo_InitMessage
static void func_ov112_021f46a8(MusShotInfo *info) {
    info->msgWin = BmpWin_CreateDynamic(4, 1, 1, 30, 4, 10, TRUE);
    BmpWin_FlushChar(info->msgWin);
    BmpWin_FlushMap(info->msgWin);
    GFL_BGSysLoadScr(4);
    info->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, info->heapId);
    info->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_POKEPARAM_2, info->heapId);
    LoadSysMsgBox(4, 1, 11, 0, info->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x140, 0x20, info->heapId);
    func_0201dd68();
    info->tcbManager = GFL_TCBExMgrCreate(info->heapId, info->heapId, 3, 0x100);
    info->stream = NULL;
    info->msgStr = NULL;
    info->printing = FALSE;
    info->waitIcon = NULL;
    info->printQueue = func_0201c440(info->heapId);
}

// pokebw2's MusShotInfo_ExitMessage
static void func_ov112_021f4748(MusShotInfo *info) {
    func_0201c6ec(info->printQueue);
    func_0201c4c0(info->printQueue);
    if (info->waitIcon != NULL) {
        func_0202f038(info->waitIcon);
        info->waitIcon = NULL;
    }
    if (info->stream != NULL) {
        func_0201ce48(info->stream);
    }
    if (info->msgStr != NULL) {
        GFL_StrBufFree(info->msgStr);
    }
    GFL_MsgDataFree(info->msgData);
    BmpWin_Free(info->msgWin);
    GFL_FontFree(info->font);
    GFL_TCBExMgrFree(info->tcbManager);
}

// pokebw2's MusShotInfo_PrintMessage
static void func_ov112_021f4794(MusShotInfo *info, u32 msgId) {
    if (info->stream != NULL) {
        func_0201ce48(info->stream);
        info->stream = NULL;
    }
    if (info->msgStr != NULL) {
        GFL_StrBufFree(info->msgStr);
        info->msgStr = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(info->msgWin), 15);
    info->msgStr = GFL_MsgDataLoadStrbufNew(info->msgData, msgId);
    info->stream = func_0201cce4(info->msgWin, 0, 0, info->msgStr, info->font, func_02012ffc(), info->tcbManager, 2,
                                 info->heapId, 0);
    BmpWin_DrawFrame(info->msgWin, TRUE, 1, 11);
}

// pokebw2's MusShotInfo_PrintMessageNow
static void func_ov112_021f4804(MusShotInfo *info, u32 msgId) {
    if (info->msgStr != NULL) {
        GFL_StrBufFree(info->msgStr);
        info->msgStr = NULL;
    }
    GFL_BitmapFill(BmpWin_GetBitmap(info->msgWin), 15);
    info->msgStr = GFL_MsgDataLoadStrbufNew(info->msgData, msgId);
    func_0201c6fc(info->printQueue, BmpWin_GetBitmap(info->msgWin), 0, 0, info->msgStr, info->font);
    info->printing = TRUE;
    BmpWin_DrawFrame(info->msgWin, TRUE, 1, 11);
}

// pokebw2's MusShotInfo_CreateMenu
static void func_ov112_021f485c(MusShotInfo *info) {
    AppTaskMenuItem items[2];
    AppTaskMenuInit init;

    items[0].str = GFL_MsgDataLoadStrbufNew(info->msgData, BtlPokeparam2_Text_004);
    items[1].str = GFL_MsgDataLoadStrbufNew(info->msgData, BtlPokeparam2_Text_005);
    items[0].color = PRINT_COLOR(14, 15, 0);
    items[1].color = PRINT_COLOR(14, 15, 0);
    items[0].type = 0;
    items[1].type = 0;
    init.heapId = info->heapId;
    init.itemCount = 2;
    init.items = items;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    init.x = 24;
    init.y = 6;
    init.width = 8;
    init.height = 3;
    info->menu = func_02027778(&init, info->menuRes);
    GFL_StrBufFree(items[0].str);
    GFL_StrBufFree(items[1].str);
}
