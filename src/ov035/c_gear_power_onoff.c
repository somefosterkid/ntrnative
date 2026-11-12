// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov088/c_gear_power_onoff.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "field/c_gear_power_onoff.h"
#include "field/subscreen.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "save/player_info.h"
#include "system/app_keycursor.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/dsi.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wipe.h"

// The touch screen that asks whether to turn the C-Gear on or off, named after the ROM's "c_gear_power_onoff.c". The
// mode picks the sequence: with the C-Gear off it offers to turn it on, with it on it offers to turn it off, and with
// wireless communication disabled in the system settings it only says so. Once the sequence ends, the field returns
// to subscreen 14. The names are ours

// The sequences, by the mode
#define MODE_POWER_ON 0
#define MODE_POWER_OFF 1
#define MODE_NO_WIRELESS 2

// The messages of system message file 25
#define MSG_ASK_POWER_ON 0
#define MSG_POWER_ON 1
#define MSG_ASK_POWER_OFF 2
#define MSG_EXPLANATION 3
#define MSG_NO_WIRELESS 4
#define MSG_YES 5
#define MSG_NO 6

// The yes/no menu's items
#define YESNO_YES 0

// The color the windows are cleared to
#define WINDOW_BG_COLOR 15

struct CGearPowerOnOff {
    u16 heapId;
    GameSystem *gsys;
    GameCommSys *comm;
    FieldSubscreen *subscreen;
    // The characters of BG 6 that the background takes, as GFL_BGSysLoadArcNCGRDynamic returns them
    u32 bgChars;
    // The window frame's characters
    u32 frameChars;
    // The question, at the top
    BmpWin *messageWindow;
    // The explanation below it
    BmpWin *explanationWindow;
    TCBExManager *tcbExMgr;
    // Never set, but freed with its string if it were
    PrintStream *printStream;
    StrBuf *printStr;
    u32 unk_2c;
    KeyCursor *keyCursor;
    Font *font;
    MsgData *msgData;
    PrintQueue *printQueue;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *yesNoMenu;
    u16 mode;
    u16 seq;
    // Whether the C-Gear is being turned off, once the answer is in
    u16 powerOff;
    u16 finished;
};

typedef BOOL (*CGearPowerOnOffSeqFunc)(CGearPowerOnOff *work);

static void func_ov035_021eff78(CGearPowerOnOff *work, HeapID heapId);
static void func_ov035_021effd4(CGearPowerOnOff *work);
static void func_ov035_021f000c(CGearPowerOnOff *work, HeapID heapId);
static void func_ov035_021f0100(CGearPowerOnOff *work);
static void func_ov035_021f0140(CGearPowerOnOff *work, HeapID heapId);
static void func_ov035_021f015c(CGearPowerOnOff *work);
static void func_ov035_021f0168(CGearPowerOnOff *work, HeapID heapId);
static void func_ov035_021f0218(CGearPowerOnOff *work);
void func_ov035_021f0254(CGearPowerOnOff *work, BmpWin *window, u32 msgId);
void func_ov035_021f0298(CGearPowerOnOff *work, BmpWin *window, u32 msgId, u32 height);
void func_ov035_021f02fc(CGearPowerOnOff *work, HeapID heapId);
void func_ov035_021f0340(CGearPowerOnOff *work);
void func_ov035_021f0354(CGearPowerOnOff *work);
u8 func_ov035_021f0364(CGearPowerOnOff *work);
BOOL func_ov035_021f0370(CGearPowerOnOff *work);
BOOL func_ov035_021f037c(CGearPowerOnOff *work);
BOOL func_ov035_021f0468(CGearPowerOnOff *work);
BOOL func_ov035_021f053c(CGearPowerOnOff *work);

// The NCLR and NCGR file IDs, by the player's gender
static const u32 data_ov035_021f05bc[3] = { 0xd, 0x6, 0xd };
static const u32 data_ov035_021f05b0[3] = { 0xe, 0x7, 0xe };

// pokebw2's sBG5Setup
static const BGSetup data_ov035_021f05c8 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, 0x6800,
    GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sBG4Setup
static const BGSetup data_ov035_021f05e8 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x00000, 0x6800,
    GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sBG6Setup
static const BGSetup data_ov035_021f0608 = {
    0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, 0x6800,
    GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE,
};

// pokebw2's sSeqFuncs
static CGearPowerOnOffSeqFunc data_ov035_021f0640[] = {
    [MODE_POWER_ON] = func_ov035_021f037c,
    [MODE_POWER_OFF] = func_ov035_021f0468,
    [MODE_NO_WIRELESS] = func_ov035_021f053c,
};

// The strings are loaded when the menu is created
// pokebw2's sYesNoItems
static AppTaskMenuItem data_ov035_021f0660[2] = {
    { NULL, 0x39e3, 0 },
    { NULL, 0x39e3, 0 },
};

// The heap is set when the menu is created
// pokebw2's sYesNoInit
static AppTaskMenuInit data_ov035_021f064c = {
    0, 2, data_ov035_021f0660, APP_TASKMENU_POS_TOP_LEFT, 21, 6, 0, 0,
};

// pokebw2's CGearPowerOnOff_Create
CGearPowerOnOff *func_ov035_021efec0(FieldSubscreen *subscreen, GameSystem *gsys, HeapID heapId) {
    CGearPowerOnOff *work = GFL_HeapAllocate(heapId, sizeof(CGearPowerOnOff), TRUE, "c_gear_power_onoff.c", 301);

    work->gsys = gsys;
    work->comm = GSYS_GetGameCommSystem(gsys);
    work->subscreen = subscreen;
    work->heapId = heapId;
    func_ov035_021eff78(work, heapId);

    if (!func_0202eb44()) {
        work->mode = MODE_NO_WIRELESS;
    } else if (func_0201214c(gsys) == TRUE) {
        work->mode = MODE_POWER_OFF;
    } else {
        work->mode = MODE_POWER_ON;
    }
    func_ov035_021eff3c(work, TRUE);
    return work;
}

// pokebw2's CGearPowerOnOff_Free
void func_ov035_021eff2c(CGearPowerOnOff *work) {
    func_ov035_021effd4(work);
    GFL_HeapFree(work);
}

// pokebw2's CGearPowerOnOff_Update
void func_ov035_021eff3c(CGearPowerOnOff *work, BOOL active) {
    if (!work->finished && data_ov035_021f0640[work->mode](work)) {
        FieldSubscreen_SaveReturnSubscreen(work->subscreen, 14);
        work->finished = TRUE;
    }
    func_0201c4e4(work->printQueue);
}

// pokebw2's CGearPowerOnOff_Init
static void func_ov035_021eff78(CGearPowerOnOff *work, HeapID heapId) {
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, heapId);
    func_0201dd68();
    work->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_S019A_CGEAR_CONTROL_WARNING, heapId);
    work->printQueue = func_0201c440(heapId);
    func_ov035_021f000c(work, heapId);
    func_ov035_021f0140(work, heapId);
    func_ov035_021f0168(work, heapId);
    func_0203d34c(FALSE, heapId);
    func_ov021_021a12d4(work->subscreen, heapId);
}

// pokebw2's CGearPowerOnOff_Exit
static void func_ov035_021effd4(CGearPowerOnOff *work) {
    func_ov021_021a0fb4(work->subscreen, work->heapId);
    func_ov035_021f0218(work);
    func_ov035_021f015c(work);
    func_ov035_021f0100(work);
    func_0201c6ec(work->printQueue);
    func_0201c4c0(work->printQueue);
    GFL_MsgDataFree(work->msgData);
    GFL_FontFree(work->font);
}

// pokebw2's CGearPowerOnOff_InitBG
static void func_ov035_021f000c(CGearPowerOnOff *work, HeapID heapId) {
    int bg;
    ArcTool *arc;
    u32 gender;
    u32 bgChars;

    for (bg = 4; bg <= 7; bg++) {
        GFL_BGSysSetBGEnabled((u8)bg, FALSE);
    }

    GFL_BGSysCreateBG(6, &data_ov035_021f0608, 0);
    GFL_BGSysClearScr(6);
    GFL_BGSysLoadScr(6);
    GFL_BGSysSetBGEnabled(6, TRUE);

    GFL_BGSysCreateBG(5, &data_ov035_021f05c8, 0);
    GFL_BGSysClearScr(5);
    GFL_BGSysLoadScr(5);
    GFL_BGSysSetBGEnabled(5, TRUE);

    GFL_BGSysCreateBG(4, &data_ov035_021f05e8, 0);
    GFL_BGSysClearScr(4);
    GFL_BGSysLoadScr(4);
    GFL_BGSysSetBGEnabled(4, TRUE);

    arc = GFL_ArcSysCreateFileHandle(ARCID_C_GEAR, heapId);
    gender = getTrainerGender(GetGameDataPlayerInfo(GSYS_GetGameData(work->gsys)));
    GFL_G2DIOLoadArcNCLRDefault(arc, data_ov035_021f05bc[gender], 4, FALSE, 1, heapId);
    bgChars = GFL_BGSysLoadArcNCGRDynamic(arc, data_ov035_021f05b0[gender], 6, FALSE, FALSE, heapId);
    work->bgChars = bgChars;
    GFL_G2DIOLoadNSCRSync(arc, 3, 6, 0, (u16)bgChars, 0, FALSE, heapId);
    GFL_ArcToolFree(arc);
}

// pokebw2's CGearPowerOnOff_ExitBG
static void func_ov035_021f0100(CGearPowerOnOff *work) {
    GFL_BGSysFreeCharMemory(6, (u16)work->bgChars, (u16)(work->bgChars >> 16));
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysSetBGEnabled(6, FALSE);
    GFL_BGSysSetBGEnabled(5, FALSE);
    GFL_BGSysSetBGEnabled(4, FALSE);
}

// pokebw2's CGearPowerOnOff_CreateMenuRes
static void func_ov035_021f0140(CGearPowerOnOff *work, HeapID heapId) {
    work->menuRes = func_02027f60(4, 3, work->font, work->printQueue, heapId);
}

// pokebw2's CGearPowerOnOff_FreeMenuRes
static void func_ov035_021f015c(CGearPowerOnOff *work) {
    func_02027fd8(work->menuRes);
}

// pokebw2's CGearPowerOnOff_CreateWindows
static void func_ov035_021f0168(CGearPowerOnOff *work, HeapID heapId) {
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 0x20, 0x20, heapId);
    work->frameChars = LoadCursorImageEndOfHeap(5, 2, 0, heapId);
    work->messageWindow = BmpWin_CreateDynamic(5, 1, 1, 30, 6, 1, TRUE);
    work->explanationWindow = BmpWin_CreateDynamic(5, 1, 13, 30, 10, 1, TRUE);

    func_02045790(work->messageWindow, 4);
    GFL_BitmapFill(BmpWin_GetBitmap(work->messageWindow), WINDOW_BG_COLOR);
    BmpWin_FlushChar(work->messageWindow);
    BmpWin_FlushMap(work->messageWindow);

    GFL_BitmapFill(BmpWin_GetBitmap(work->explanationWindow), WINDOW_BG_COLOR);
    BmpWin_FlushChar(work->explanationWindow);
    BmpWin_FlushMap(work->explanationWindow);

    work->tcbExMgr = GFL_TCBExMgrCreate(heapId, heapId, 1, 4);
    work->keyCursor = func_02028590(WINDOW_BG_COLOR, FALSE, TRUE, heapId);
}

// pokebw2's CGearPowerOnOff_FreeWindows
static void func_ov035_021f0218(CGearPowerOnOff *work) {
    if (work->printStream != NULL) {
        func_0201ce48(work->printStream);
        work->printStream = NULL;
        GFL_StrBufFree(work->printStr);
        work->printStr = NULL;
    }
    func_02028604(work->keyCursor);
    GFL_TCBExMgrFree(work->tcbExMgr);
    BmpWin_Free(work->explanationWindow);
    BmpWin_Free(work->messageWindow);
    func_0201f614(5, work->frameChars);
}

// pokebw2's CGearPowerOnOff_PrintMessage
void func_ov035_021f0254(CGearPowerOnOff *work, BmpWin *window, u32 msgId) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, msgId);

    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, str, work->font);
    func_0201f79c(window, FALSE, work->frameChars, 2);
    BmpWin_FlushChar(window);
    GFL_StrBufFree(str);
}

// Prints the message in the window at a new height, clearing its old frame first
// pokebw2's CGearPowerOnOff_PrintMessageResized
void func_ov035_021f0298(CGearPowerOnOff *work, BmpWin *window, u32 msgId, u32 height) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, msgId);

    func_02045790(window, height);
    GFL_BitmapFill(BmpWin_GetBitmap(window), WINDOW_BG_COLOR);
    func_0201f7a8(window, 2);
    BmpWin_FlushMap(window);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, str, work->font);
    func_0201f79c(window, FALSE, work->frameChars, 2);
    BmpWin_FlushChar(window);
    GFL_StrBufFree(str);
}

// pokebw2's CGearPowerOnOff_CreateYesNo
void func_ov035_021f02fc(CGearPowerOnOff *work, HeapID heapId) {
    data_ov035_021f064c.heapId = heapId;
    data_ov035_021f0660[0].str = GFL_MsgDataLoadStrbufNew(work->msgData, MSG_YES);
    data_ov035_021f0660[1].str = GFL_MsgDataLoadStrbufNew(work->msgData, MSG_NO);
    work->yesNoMenu = func_0202784c(&data_ov035_021f064c, work->menuRes);
    func_02027cd8(work->yesNoMenu, TRUE);
    GFL_StrBufFree(data_ov035_021f0660[0].str);
    GFL_StrBufFree(data_ov035_021f0660[1].str);
}

// pokebw2's CGearPowerOnOff_FreeYesNo
void func_ov035_021f0340(CGearPowerOnOff *work) {
    if (work->yesNoMenu != NULL) {
        func_02027858(work->yesNoMenu);
        work->yesNoMenu = NULL;
    }
}

// pokebw2's CGearPowerOnOff_UpdateYesNo
void func_ov035_021f0354(CGearPowerOnOff *work) {
    if (work->yesNoMenu != NULL) {
        func_02027974(work->yesNoMenu);
    }
}

// pokebw2's CGearPowerOnOff_GetYesNoPos
u8 func_ov035_021f0364(CGearPowerOnOff *work) {
    return func_02027a04(work->yesNoMenu);
}

// pokebw2's CGearPowerOnOff_IsYesNoFinished
BOOL func_ov035_021f0370(CGearPowerOnOff *work) {
    return func_020279e8(work->yesNoMenu);
}

BOOL func_ov035_021f037c(CGearPowerOnOff *work) {
    switch (work->seq) {
    case 0:
        func_ov035_021f0254(work, work->messageWindow, MSG_ASK_POWER_ON);
        func_ov035_021f0254(work, work->explanationWindow, MSG_EXPLANATION);
        func_ov035_021f02fc(work, work->heapId);
        work->seq++;
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            work->seq++;
        }
        break;
    case 2:
        if (func_ov035_021f0370(work)) {
            if (func_ov035_021f0364(work) == YESNO_YES) {
                func_02012158(work->gsys, 1);
                func_020121cc(work->gsys);
                work->seq = 3;
            } else {
                work->seq = 5;
            }
            func_ov035_021f0340(work);
        }
        break;
    case 3:
        func_ov035_021f0298(work, work->messageWindow, MSG_POWER_ON, 6);
        work->seq++;
        break;
    case 4:
        if (func_02035c60() || func_02012204(work->gsys)) {
            work->seq++;
        }
        break;
    case 5:
        if (func_02012204(work->gsys)) {
            if (GameCommSys_BootCheck(work->comm)) {
                func_02026400(work->comm);
            }
            work->seq = 6;
        } else {
            return TRUE;
        }
        break;
    case 6:
        if (!GameCommSys_BootCheck(work->comm)) {
            return TRUE;
        }
        break;
    }
    func_ov035_021f0354(work);
    return FALSE;
}

BOOL func_ov035_021f0468(CGearPowerOnOff *work) {
    switch (work->seq) {
    case 0:
        func_ov035_021f0254(work, work->messageWindow, MSG_ASK_POWER_OFF);
        func_ov035_021f0254(work, work->explanationWindow, MSG_EXPLANATION);
        func_ov035_021f02fc(work, work->heapId);
        work->seq++;
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            work->seq++;
        }
        break;
    case 2:
        if (func_ov035_021f0370(work) || func_02012204(work->gsys) == TRUE) {
            if (func_ov035_021f0364(work) == YESNO_YES || func_02012204(work->gsys) == TRUE) {
                if (GameCommSys_BootCheck(work->comm)) {
                    func_02026400(work->comm);
                }
                work->powerOff = TRUE;
            } else {
                work->powerOff = FALSE;
            }
            work->seq = 3;
            func_ov035_021f0340(work);
        }
        break;
    case 3:
        if (work->powerOff) {
            if (GameCommSys_BootCheck(work->comm)) {
                break;
            }
            func_02012158(work->gsys, 0);
        }
        work->seq++;
        break;
    case 4:
        return TRUE;
    }
    func_ov035_021f0354(work);
    return FALSE;
}

BOOL func_ov035_021f053c(CGearPowerOnOff *work) {
    switch (work->seq) {
    case 0:
        func_020456f8(work->messageWindow);
        func_020456f8(work->explanationWindow);
        func_02045788(work->explanationWindow, 6);
        BmpWin_FlushChar(work->explanationWindow);
        BmpWin_FlushMap(work->explanationWindow);
        func_ov035_021f0254(work, work->explanationWindow, MSG_NO_WIRELESS);
        work->seq++;
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            work->seq++;
        }
        break;
    case 2:
        if (func_02035c60()) {
            work->seq++;
        }
        break;
    case 3:
        return TRUE;
    }
    return FALSE;
}




