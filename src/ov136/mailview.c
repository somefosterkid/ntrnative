// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov215/mailview.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "field/mailview.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/pms_draw.h"
#include "system/printsys.h"
#include "text/system/btl_server_flow_3.h"

// The mail screen's graphics (overlay 215's mailview.c): its BGs, palettes, windows, buttons and the mail's text,
// loaded a step at a time by func_ov136_021c4bfc. The names are ours

// The mail graphics' archive, which no header names yet. A design's files are its palette, its characters
// MAIL_FILE_CHAR_BASE files on, and its screen MAIL_FILE_SCREEN_BASE files on
#define ARCID_MAIL 0x9b
#define MAIL_FILE_CHAR_BASE 12
#define MAIL_FILE_SCREEN_BASE 24
// The screen of the buttons, with the write phase's frame
#define MAIL_FILE_WRITE_SCREEN 36

// The BGs of the screen: the mail's, the sentences, the text and the buttons' frame, and the sub screen's
#define MAIL_BG_COUNT 5

// The steps of the loading
enum {
    MAIL_LOAD_MESSAGES,
    MAIL_LOAD_GRAPHICS,
    MAIL_LOAD_WINDOWS,
    MAIL_LOAD_OBJECTS,
};

// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov136_021c77c4 = {
    GX_VRAM_BG_128_A, GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE, GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

static void func_ov136_021c4c9c(MailSysWork *work);
static void func_ov136_021c4d54(MailSysWork *work);
static void func_ov136_021c4da8(MailSysWork *work);
static void func_ov136_021c4dd4(MailSysWork *work);
static void func_ov136_021c4de4(MailSysWork *work);
static void func_ov136_021c4ec4(MailSysWork *work);
static void func_ov136_021c4eec(MailSysWork *work);
static void func_ov136_021c5114(MailSysWork *work);
static void func_ov136_021c515c(MailSysWork *work);
static void func_ov136_021c5268(MailSysWork *work);
static void func_ov136_021c52f8(MailSysWork *work);
static void func_ov136_021c5360(MailSysWork *work);
static void func_ov136_021c5420(MailSysWork *work);
static void func_ov136_021c5470(MailSysWork *work);

// pokebw2's MailView_Load
BOOL func_ov136_021c4bfc(MailSysWork *work) {
    switch (work->subSeq) {
    case MAIL_LOAD_MESSAGES:
        work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_SERVER_FLOW_3, work->heapId);
        work->msgBuf = GFL_StrBufCreate(128, work->heapId);
        func_ov136_021c4de4(work);
        break;
    case MAIL_LOAD_GRAPHICS:
        func_ov136_021c4eec(work);
        break;
    case MAIL_LOAD_WINDOWS:
        func_ov136_021c515c(work);
        break;
    case MAIL_LOAD_OBJECTS:
        func_ov136_021c5420(work);
        work->menuRes = func_02027f60(0, 8, work->font, work->printQueue, work->heapId);
        work->subSeq = 0;
        func_ov136_021c4d54(work);
        func_ov136_021c5360(work);
        func_ov136_021c5268(work);
        func_ov136_021c4c9c(work);
        return TRUE;
    }
    work->subSeq++;
    return FALSE;
}

// pokebw2's MailView_InitFade
static void func_ov136_021c4c9c(MailSysWork *work) {
    func_02021fd8(work->palFade, 0, 0, 0x70, 16, 0);
    func_02021ee4(work->palFade, TRUE);
    func_02021d50(work->palFade);
    work->blinkLevel = 0;
    work->blinkFalling = 0;
    work->tcbBuffer = GFL_HeapAllocate(work->heapId, GFL_TCBMgrCalcAllocSize(3), FALSE, "mailview.c", 1267);
    work->tcbMgr = GFL_TCBMgrCreate(3, work->tcbBuffer);
    work->blinkTask = GFL_TCBMgrAddTask(work->tcbMgr, func_ov136_021c4b88, work, 0);
}

// pokebw2's MailView_Unload
BOOL func_ov136_021c4d14(MailSysWork *work) {
    func_ov136_021c4da8(work);
    func_ov136_021c5470(work);
    if (work->unk28) {
        GFL_MsgDataFree(work->msgData);
    }
    GFL_StrBufFree(work->msgBuf);
    func_02027fd8(work->menuRes);
    func_ov136_021c52f8(work);
    func_ov136_021c5114(work);
    func_ov136_021c4ec4(work);
    return TRUE;
}

// pokebw2's MailView_CreatePMSDraw
static void func_ov136_021c4d54(MailSysWork *work) {
    work->pmsQueue = func_0201c440(work->heapId);
    work->pmsClUnit = func_0204b100(8, 1, work->heapId);
    work->pmsDraw = func_0202db0c(work->pmsClUnit, 0, work->pmsQueue, work->font, 0, 4, work->heapId);
    func_0202de5c(work->pmsDraw, 0);
}

// pokebw2's MailView_DeletePMSDraw
static void func_ov136_021c4da8(MailSysWork *work) {
    func_0202dc08(work->pmsDraw);
    func_0201c4c0(work->pmsQueue);
    work->pmsQueue = NULL;
    func_0204b1cc(work->pmsClUnit);
}

// pokebw2's MailView_SetVRAMBanks
static void func_ov136_021c4dd4(MailSysWork *work) {
    GFL_BGSysSetVRAMBanks(&data_ov136_021c77c4);
}

typedef struct {
    BGSetup setups[MAIL_BG_COUNT];
} MailBGSetups;

// pokebw2's lcdConfig and bgSetups. One object: separate ones (statics or locals) are sorted by size, the 16-byte
// config before the 48-byte VRAM config, while Black has it after
static const struct {
    BGSysLCDConfig lcdConfig;
    MailBGSetups bgSetups;
} data_ov136_021c77f4 = { { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D }, { {
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x08000, 0x4000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x10000, 0x8000,
      GX_BG_EXTPLTT_01, 1, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000, 0x4000,
      GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe000, GX_BG_CHARBASE_0x00000, 0x4000,
      GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
    { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000, 0x4000,
      GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE },
} } };

// pokebw2's MailView_InitBGs
static void func_ov136_021c4de4(MailSysWork *work) {
    func_ov136_021c4dd4(work);
    BGSysLCDConfig lcdConfig = data_ov136_021c77f4.lcdConfig;
    GFL_BGSysSetLCDConfig(&lcdConfig);
    GFL_BGSysSetDisplayLayout(0);
    MailBGSetups bgSetups = data_ov136_021c77f4.bgSetups;

    GFL_BGSysCreateBG(0, &bgSetups.setups[0], BGMODE_TEXT);
    GFL_BGSysCreateBG(1, &bgSetups.setups[1], BGMODE_TEXT);
    GFL_BGSysCreateBG(2, &bgSetups.setups[2], BGMODE_TEXT);
    GFL_BGSysCreateBG(3, &bgSetups.setups[3], BGMODE_TEXT);
    GFL_BGSysCreateBG(4, &bgSetups.setups[4], BGMODE_TEXT);
    GFL_BGSysClearScr(0);
    GFL_BGSysClearScr(1);
    GFL_BGSysClearScr(2);
    GFL_BGSysClearScr(3);
    GFL_BGSysClearScr(4);
    GFL_BGSysClearCharCore(0, 0x20, 0, work->heapId);
    GFL_BGSysClearCharCore(1, 0x20, 0, work->heapId);
    GFL_BGSysClearCharCore(2, 0x20, 0, work->heapId);
    GFL_BGSysClearCharCore(3, 0x20, 0, work->heapId);
    GFL_BGSysClearCharCore(4, 0x20, 0, work->heapId);
}

// pokebw2's MailView_ReleaseBGs
static void func_ov136_021c4ec4(MailSysWork *work) {
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysSetDisplayLayout(1);
}

// pokebw2's MailView_LoadGraphics
static void func_ov136_021c4eec(MailSysWork *work) {
    int design = work->edit->design;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_MAIL, work->heapId);

    LoadSysMsgBox(0, 1, 4, 0, work->heapId);
    LoadSysMsgBox(0, 13, 6, 0, work->heapId);
    u32 charFile = design + MAIL_FILE_CHAR_BASE;
    GFL_BGSysLoadArcNCGRStatic(arc, charFile, 3, 0, 0, TRUE, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, charFile, 4, 0, 0, TRUE, work->heapId);
    u32 screenFile = design + MAIL_FILE_SCREEN_BASE;
    loadBGScrToVramByFileNoReserveNegAlign(arc, screenFile, 3, 0, 0x800, TRUE, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, screenFile, 4, 0, 0x800, TRUE, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, design, PALTYPE_SUB_BG, 0, 0, work->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0xa0, 0x20, work->heapId);
    work->palFade = func_02021518(work->heapId);
    func_0202155c(work->palFade, 0, 0xe0, work->heapId);
    func_020216b4(work->palFade, arc, design, work->heapId, 0, 0x60, 0);
    func_02021640(work->palFade, ARCID_FONT, 7, work->heapId, 0, 0x20, 0x30);
    func_02021640(work->palFade, ARCID_FONT, 5, work->heapId, 0, 0x20, 0x50);
    func_02021640(work->palFade, ARCID_WINFRAME, GetSysMsgBoxPaletteDatID(0), work->heapId, 0, 0x20, 0x40);
    func_02021640(work->palFade, ARCID_WINFRAME, GetSysMsgBoxPaletteDatID(1), work->heapId, 0, 0x20, 0x60);
    func_0201dd68();
    GFL_ArcToolFree(arc);
    GFL_BGSysFillScrArea(4, 0x2001, 0, 0, 32, 32, 0x11);
    GFL_BGSysQueueScrLoad(4);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG1, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG3, TRUE);
    if (work->phase != MAIL_PHASE_VIEW) {
        arc = GFL_ArcSysCreateFileHandle(ARCID_MAIL, work->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, MAIL_FILE_WRITE_SCREEN, 2, 0, 0x800, TRUE, work->heapId);
        GFL_ArcToolFree(arc);
        work->onKeyMode = func_ov136_021c4534;
        work->onTouchMode = func_ov136_021c4528;
        GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG2, TRUE);
    }
}

// pokebw2's MailView_ExitFade
static void func_ov136_021c5114(MailSysWork *work) {
    func_020215a0(work->palFade, 0);
    func_02021540(work->palFade);
    work->palFade = NULL;
    GFL_TCBRemove(work->blinkTask);
    func_02030da4(work->tcbMgr);
    GFL_HeapFree(work->tcbBuffer);
    G2_SetBlendAlpha(0, 0, 31, 0);
}

// pokebw2's MailView_CreateWindows
static void func_ov136_021c515c(MailSysWork *work) {
    int i;

    BmpWin_InitAllocator(work->heapId);
    work->windows[0] = BmpWin_CreateDynamic(1, 0, 0, 1, 1, 5, 0);
    work->windows[1] = BmpWin_CreateDynamic(1, 3, 3, 26, 4, 1, 0);
    work->windows[2] = BmpWin_CreateDynamic(1, 3, 8, 26, 4, 1, 0);
    work->windows[3] = BmpWin_CreateDynamic(1, 3, 13, 26, 4, 1, 0);
    work->windows[4] = BmpWin_CreateDynamic(1, 8, 19, 11, 2, 1, 0);
    work->windows[5] = BmpWin_CreateDynamic(1, 21, 19, 8, 2, 1, 0);
    work->windows[6] = BmpWin_CreateDynamic(0, 2, 1, 28, 4, 5, 1);
    for (i = 0; i < 6; i++) {
        work->entries[i].win = work->windows[i];
        work->entries[i].drawn = FALSE;
    }
    work->printQueue = func_0201c440(work->heapId);
    for (i = 0; i < 7; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[i]), 0);
        if (i < 5) {
            BmpWin_TransferNow(work->windows[i]);
        }
    }
}

// pokebw2's MailView_CreateButtons
static void func_ov136_021c5268(MailSysWork *work) {
    if (work->phase == MAIL_PHASE_WRITE) {
        work->buttonItems[0].str = GFL_MsgDataLoadStrbufNew(work->msgData, BtlServerFlow3_Text_000);
        work->buttonItems[0].color = PRINT_COLOR(14, 15, 3);
        work->buttonItems[0].type = 0;
        work->buttonItems[1].str = GFL_MsgDataLoadStrbufNew(work->msgData, BtlServerFlow3_Text_001);
        work->buttonItems[1].color = PRINT_COLOR(14, 15, 3);
        work->buttonItems[1].type = APP_TASKMENU_ITEM_RETURN;
        work->confirmButton = func_02027fec(work->menuRes, &work->buttonItems[0], 21, 18, 10, work->heapId);
        work->cancelButton = func_02027fec(work->menuRes, &work->buttonItems[1], 21, 21, 10, work->heapId);
    }
}

static void func_ov136_021c52f8(MailSysWork *work) {
    BmpWin *window;
    int i;

    if (work->phase == MAIL_PHASE_WRITE) {
        func_02028148(work->confirmButton);
        func_02028148(work->cancelButton);
        GFL_StrBufFree(work->buttonItems[0].str);
        GFL_StrBufFree(work->buttonItems[1].str);
    }
    func_0201c4c0(work->printQueue);
    work->printQueue = NULL;
    for (i = 0; i < 7; i++) {
        window = work->windows[i];
        func_020456f8(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
        BmpWin_Free(work->windows[i]);
    }
    BmpWin_FreeAllocator();
}


// pokebw2's MailView_PrintMail
static void func_ov136_021c5360(MailSysWork *work) {
    int i;

    for (i = 0; i < MAIL_WORD_COUNT; i++) {
        if (func_02024450(&work->edit->words[i])) {
            func_0202dc48(work->pmsDraw, work->windows[i + 1], &work->edit->words[i], i);
        }
    }
    if (work->edit->word != 0xffff) {
        work->pmsWord.type = 6;
        work->pmsWord.id = 0;
        work->pmsWord.words[0] = work->edit->word;
        func_0202dc48(work->pmsDraw, work->windows[4], &work->pmsWord, 3);
    }
    if (work->phase == MAIL_PHASE_VIEW) {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->entries[5].win), 0, 2, work->edit->name, work->font,
                      PRINT_COLOR(1, 2, 0));
        work->entries[5].drawn = TRUE;
        BmpWin_TransferNow(work->windows[5]);
    }
}

// pokebw2's MailView_CreateClAct
static void func_ov136_021c5420(MailSysWork *work) {
    ClActSys_Create(&data_020a1448, &data_ov136_021c77c4, work->heapId);
    work->clUnit = func_0204b100(4, 0, work->heapId);
    func_0204b270(work->clUnit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    func_0203d34c(FALSE, work->heapId);
}

// The counterpart of func_ov136_021c5420, which the ROM places after it (0x021a8d58). Whether it belongs to this file
// or starts mb_seq.c is a guess: it frees what func_ov136_021c5420 made
// pokebw2's MailView_DeleteClAct
static void func_ov136_021c5470(MailSysWork *work) {
    func_0204b1cc(work->clUnit);
    func_0204a5ac();
}
