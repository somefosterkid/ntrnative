// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mb_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/mailbox_local.h"
#include "app/pokelist.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "field/mailsys.h"
#include "field/mailtool.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
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
#include "nitro/gx.h"
#include "pml/item.h"
#include "pml/mail.h"
#include "save/bag.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/bgwinfrm.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/btl_server_flow_2.h"

// The mailbox screen's set-up: the display, the BGs and their graphics, the frames, the text, the menus, the mails of
// the list and the palette changes. The ROM doesn't name this file; the name is a guess, after box2_main.c

// Black 2's, not checked against Black
typedef struct {
    u32 messageId;
    u32 type;
} MailBoxMenuItem;

// pokebw2's MailBox_VRAMConfig
const BGSysVRAMConfig data_ov136_021c78e0 = {
    GX_VRAM_BG_128_B,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_A, GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's MailBox_VBlank
void func_ov136_021c54fc(TCB *tcb, void *data) {
    GFL_BGSysUpdate();
    func_0204a648();
}

// pokebw2's MailBox_SetVRAMBanks
void func_ov136_021c5508(void) {
    GFL_BGSysSetVRAMBanks(&data_ov136_021c78e0);
}

// pokebw2's MailBox_InitBGs
void func_ov136_021c5518(MailBoxWork *work) {
    GFL_BGSysCreate(HEAPID_MAILBOX_VIEW);
    {
        BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&lcdConfig);
    }
    {
        BGSetup bg = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x18000,
                       0x4000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0 };

        GFL_BGSysCreateBG(4, &bg, BGMODE_TEXT);
        GFL_BGSysClearScr(4);
        GFL_BGSysClearCharCore(4, BGSYS_TILE_SIZE_16, 0, HEAPID_MAILBOX_VIEW);
    }
    {
        BGSetup bg = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000,
                       0x4000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, 0 };

        GFL_BGSysCreateBG(5, &bg, BGMODE_TEXT);
        GFL_BGSysClearScr(5);
        GFL_BGSysClearCharCore(5, BGSYS_TILE_SIZE_16, 0, HEAPID_MAILBOX_VIEW);
    }
    {
        BGSetup bg = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000,
                       0x4000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0 };

        GFL_BGSysCreateBG(6, &bg, BGMODE_TEXT);
        GFL_BGSysClearScr(6);
        GFL_BGSysClearCharCore(6, BGSYS_TILE_SIZE_16, 0, HEAPID_MAILBOX_VIEW);
    }
    {
        BGSetup bg = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x08000,
                       0x4000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0 };

        GFL_BGSysCreateBG(7, &bg, BGMODE_TEXT);
    }
    {
        BGSetup bg = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x18000,
                       0x4000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, 0 };

        GFL_BGSysCreateBG(0, &bg, BGMODE_TEXT);
        GFL_BGSysClearScr(0);
        GFL_BGSysClearCharCore(0, BGSYS_TILE_SIZE_16, 0, HEAPID_MAILBOX_VIEW);
    }
    {
        BGSetup bg = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000,
                       0x4000, GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, 0 };

        GFL_BGSysCreateBG(1, &bg, BGMODE_TEXT);
        GFL_BGSysClearScr(1);
        GFL_BGSysClearCharCore(1, BGSYS_TILE_SIZE_16, 0, HEAPID_MAILBOX_VIEW);
    }
    {
        BGSetup bg = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000,
                       0x4000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, 0 };

        GFL_BGSysCreateBG(2, &bg, BGMODE_TEXT);
        GFL_BGSysClearScr(2);
    }
    {
        BGSetup bg = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x00000,
                       0x4000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, 0 };

        GFL_BGSysCreateBG(3, &bg, BGMODE_TEXT);
    }
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG2, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's MailBox_FreeBGs
void func_ov136_021c5714(MailBoxWork *work) {
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysFree();
}

// pokebw2's MailBox_LoadBGGraphics
void func_ov136_021c574c(MailBoxWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_MAILBOX, HEAPID_MAILBOX_VIEW);
    NNSG2dCharacterData *character;
    void *file;
    u8 *chars;

    GFL_BGSysLoadArcNCGRStatic(arc, 42, 7, 0, 0, TRUE, HEAPID_MAILBOX_VIEW);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 41, 7, 0, 0, TRUE, HEAPID_MAILBOX_VIEW);
    GFL_G2DIOLoadArcNCLRDefault(arc, 43, PALTYPE_SUB_BG, 0, 0, HEAPID_MAILBOX_VIEW);
    GFL_BGSysLoadArcNCGRStatic(arc, 38, 3, 0, 0, TRUE, HEAPID_MAILBOX_VIEW);
    GFL_BGSysLoadArcNCGRStatic(arc, 38, 1, 0, 0, TRUE, HEAPID_MAILBOX_VIEW);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 37, 3, 0, 0, TRUE, HEAPID_MAILBOX_VIEW);
    GFL_G2DIOLoadArcNCLRDefault(arc, 39, PALTYPE_MAIN_BG, 0, 0, HEAPID_MAILBOX_VIEW);
    file = GFL_G2DIOReadBGNCGRArc(arc, 38, TRUE, &character, HEAPID_MAILBOX_VIEW);
    chars = character->pRawData;
    MIi_CpuCopyFast(chars + 0x100, work->view->tileA, 0x20);
    MIi_CpuCopyFast(chars + 0x100, work->view->tileB, 0x20);
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
}

// pokebw2's MailBox_LoadSystemPalettes
void func_ov136_021c5828(MailBoxWork *work) {
    LoadSysMsgBox(0, 0x3e2, 13, 0, HEAPID_MAILBOX_VIEW);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 7 * 0x40, 0x20, HEAPID_MAILBOX_VIEW);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 7 * 0x40 + 0x20, 0x20, HEAPID_MAILBOX_VIEW);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 7 * 0x40 + 0x20, 0x20, HEAPID_MAILBOX_VIEW);
}

// pokebw2's MailBox_CreateFrames
void func_ov136_021c587c(MailBoxWork *work) {
    u32 i;
    u16 *screen;

    work->view->frames = func_0202c9b4(2, 11, HEAPID_MAILBOX_VIEW);
    for (i = 0; i < MAILBOX_PAGE_SIZE; i++) {
        func_0202ca3c(work->view->frames, i, 2, 14, 4);
    }
    func_0202ca3c(work->view->frames, MAILBOX_FRAME_SELECTED, 1, 14, 4);
    func_0202cae4(work->view->frames, 0, ARCID_MAILBOX, 40, TRUE);
    screen = func_0202cf84(work->view->frames, 0);
    for (i = 1; i <= MAILBOX_FRAME_SELECTED; i++) {
        MI_CpuCopy8(screen, func_0202cf84(work->view->frames, i), 14 * 4 * sizeof(u16));
    }
    for (i = 0; i < MAILBOX_PAGE_SIZE; i++) {
        func_0202cb4c(work->view->frames, i, (i % 2) * 16 + 1, (i / 2) * 4);
    }
}

// pokebw2's MailBox_FreeFrames
void func_ov136_021c5918(MailBoxWork *work) {
    func_0202ca0c(work->view->frames);
}

// pokebw2's MailBox_LoadMessages
void func_ov136_021c5924(MailBoxView *view) {
    view->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, MAILBOX_MSG_FILE, HEAPID_MAILBOX_VIEW);
    view->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_MAILBOX_VIEW);
    view->strbuf = GFL_StrBufCreate(0x400, HEAPID_MAILBOX_VIEW);
}

// pokebw2's MailBox_FreeMessages
void func_ov136_021c5954(MailBoxView *view) {
    GFL_StrBufFree(view->strbuf);
    GFL_WordSetSystemFree(view->wordSet);
    GFL_MsgDataFree(view->msgData);
}

// pokebw2's MailBox_CreateYesNoMenu
void func_ov136_021c5974(MailBoxWork *work) {
    AppTaskMenuInit init;

    init.heapId = HEAPID_MAILBOX_VIEW;
    init.itemCount = 2;
    init.items = work->view->yesNoItems;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    init.x = 21;
    init.y = 8;
    init.width = 10;
    init.height = 3;
    work->view->menu = func_02027778(&init, work->view->menuRes);
}

// pokebw2's MailBox_CreateMenuResources
void func_ov136_021c59b8(MailBoxWork *work) {
    static const MailBoxMenuItem menuItems[4] = {
        { BtlServerFlow2_Text_005, 0 },
        { BtlServerFlow2_Text_006, 0 },
        { BtlServerFlow2_Text_007, 0 },
        { BtlServerFlow2_Text_008, APP_TASKMENU_ITEM_RETURN },
    };
    static const MailBoxMenuItem yesNoItems[2] = {
        { BtlServerFlow2_Text_015, 0 },
        { BtlServerFlow2_Text_016, 0 },
    };
    u16 i;

    work->view->menuRes = func_02027f60(0, 5, work->font, work->view->printQueue, HEAPID_MAILBOX_VIEW);
    for (i = 0; i < 4; i++) {
        work->view->menuItems[i].str = GFL_MsgDataLoadStrbufNew(work->view->msgData, menuItems[i].messageId);
        work->view->menuItems[i].color = PRINT_COLOR(14, 15, 3);
        work->view->menuItems[i].type = menuItems[i].type;
    }
    for (i = 0; i < 2; i++) {
        work->view->yesNoItems[i].str = GFL_MsgDataLoadStrbufNew(work->view->msgData, yesNoItems[i].messageId);
        work->view->yesNoItems[i].color = PRINT_COLOR(14, 15, 3);
        work->view->yesNoItems[i].type = yesNoItems[i].type;
    }
}

// pokebw2's MailBox_FreeMenuResources
void func_ov136_021c5aa8(MailBoxWork *work) {
    int i;

    for (i = 0; i < 4; i++) {
        GFL_StrBufFree(work->view->menuItems[i].str);
    }
    GFL_StrBufFree(work->view->yesNoItems[0].str);
    GFL_StrBufFree(work->view->yesNoItems[1].str);
    func_02027fd8(work->view->menuRes);
}

// pokebw2's MailBox_CreateMainMenu
void func_ov136_021c5aec(MailBoxWork *work) {
    AppTaskMenuInit init;

    init.heapId = HEAPID_MAILBOX_VIEW;
    init.itemCount = 4;
    init.items = work->view->menuItems;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    init.x = 20;
    init.y = 12;
    init.width = 12;
    init.height = 3;
    work->view->menu = func_02027778(&init, work->view->menuRes);
}

// pokebw2's MailBox_FreeMenu
void func_ov136_021c5b30(MailBoxWork *work) {
    func_02027858(work->view->menu);
}

// pokebw2's MailBox_LoadMails
void func_ov136_021c5b40(MailBoxWork *work) {
    u32 i;

    work->view->mailCount = 0;
    for (i = 0; i < MAIL_COUNT; i++) {
        work->view->pageMails[i] = 0xff;
        work->view->mails[i] = func_02009050(work->mails, 0, i, HEAPID_MAILBOX_VIEW);
        if (func_02008ef8(work->view->mails[i]) == TRUE) {
            work->view->pageMails[work->view->mailCount] = i;
            work->view->mailCount++;
        }
    }
    if (work->view->mailCount == 0) {
        work->view->maxPage = 0;
    } else {
        work->view->maxPage = (work->view->mailCount - 1) / MAILBOX_PAGE_SIZE;
    }
}

// pokebw2's MailBox_FreeMails
void func_ov136_021c5be0(MailBoxWork *work) {
    u32 i;

    for (i = 0; i < MAIL_COUNT; i++) {
        GFL_HeapFree(work->view->mails[i]);
    }
}

// pokebw2's MailBox_UpdateMailList
void func_ov136_021c5c00(MailBoxWork *work) {
    u16 first = MAILBOX_PAGE_SIZE * work->page;
    u8 *mails = &work->view->pageMails[first];
    u16 window = first + MAILBOX_WIN_FIRST_MAIL;
    u16 frame = 0;
    u16 i;

    for (i = 0; i < MAILBOX_PAGE_SIZE; i++) {
        if (mails[i] == 0xff) {
            func_0202cc70(work->view->frames, frame);
        } else {
            func_ov136_021c6028(work, frame, window, mails[i]);
            func_0202cc58(work->view->frames, frame);
        }
        window++;
        frame++;
    }
}

// pokebw2's MailBox_ShowSelectedFrame
void func_ov136_021c5c64(MailBoxWork *work) {
    func_ov136_021c6028(work, MAILBOX_FRAME_SELECTED, MAILBOX_WIN_SELECTED_MAIL, work->mailIndex);
    func_0202cb4c(work->view->frames, MAILBOX_FRAME_SELECTED, 4, 7);
}

// pokebw2's MailBox_UpdatePaletteFx
BOOL func_ov136_021c5c84(MailBoxWork *work) {
    MailBoxPaletteFx *fx = &work->view->paletteFx;

    switch (fx->phase) {
    case 0:
        if (!fx->onScreen) {
            func_0204b6f4(work->view->actors[fx->target], fx->firstPalette, TRUE);
        } else {
            GFL_BGSysSetScrPaletteNo(fx->target, fx->x, fx->y, fx->width, fx->height, fx->firstPalette);
            GFL_BGSysQueueScrLoad(fx->target);
        }
        fx->phase++;
        break;
    case 1:
        fx->timer++;
        if (fx->timer == 4) {
            if (!fx->onScreen) {
                func_0204b6f4(work->view->actors[fx->target], fx->secondPalette, TRUE);
            } else {
                GFL_BGSysSetScrPaletteNo(fx->target, fx->x, fx->y, fx->width, fx->height, fx->secondPalette);
                GFL_BGSysQueueScrLoad(fx->target);
            }
            fx->timer = 0;
            fx->phase++;
        }
        break;
    case 2:
        fx->timer++;
        if (fx->timer == 2) {
            return FALSE;
        }
        break;
    }
    return TRUE;
}

// pokebw2's MailBox_LoadMailGraphics
void func_ov136_021c5d68(MailBoxWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_MAILBOX, HEAPID_MAILBOX_VIEW);
    u8 design = func_02008fa4(work->view->mails[work->mailIndex]);

    GFL_BGSysLoadArcNCGRStatic(arc, design + 12, 5, 0, 0, TRUE, HEAPID_MAILBOX_VIEW);
    GFL_G2DIOLoadArcNCLRDefault(arc, design, PALTYPE_SUB_BG, 0, 0x40, HEAPID_MAILBOX_VIEW);
    loadBGScrToVramByFileNoReserveNegAlign(arc, design + 24, 5, 0, 0, TRUE, HEAPID_MAILBOX_VIEW);
    GFL_ArcToolFree(arc);
}

// pokebw2's MailBox_CreatePMSDraw
void func_ov136_021c5dd8(MailBoxWork *work) {
    work->view->pmsPrintQueue = func_0201c440(HEAPID_MAILBOX_VIEW);
    work->view->pmsClActUnit = func_0204b100(8, 2, HEAPID_MAILBOX_VIEW);
    work->view->pmsDraw = func_0202db0c(work->view->pmsClActUnit, CLACT_VRAM_SUB, work->view->pmsPrintQueue, work->font,
                                         4, 4, HEAPID_MAILBOX_VIEW);
    func_0202de5c(work->view->pmsDraw, 0);
}

// pokebw2's MailBox_FreePMSDraw
void func_ov136_021c5e30(MailBoxWork *work) {
    func_0202dc08(work->view->pmsDraw);
    func_0201c4c0(work->view->pmsPrintQueue);
    func_0204b1cc(work->view->pmsClActUnit);
}
