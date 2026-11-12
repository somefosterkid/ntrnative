// Black's ircbattlemenu.c (the name its asserts pass; pokebw2's ircbattlemenu.c): ov021 0x02196030-0x02197c4c.

#include "types.h"
#include "field/beacon_view.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bmp.h"
#include "gfl/button_man.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/time_icon.h"
#include "system/wipe.h"
#include "gfl/tcbl.h"
#include "system/game_system.h"
#include "system/game_data.h"
#include "system/palanm.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "system/game_comm.h"
#include "save/box.h"
#include "save/trainer_data_savedata.h"
#include "gfl/sound.h"
#include "save/save_control.h"
#include "gfl/proc.h"
#include "gfl/tcb.h"
#include "field/event_irc.h"
#include "gfl/net.h"

// A button's text window on BG 7, in tiles
typedef struct {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
} IrcBattleMenuWindowParam;

static IrcBattleMenuWindowParam data_ov021_021ddb84[4] = {
#include "asset/ircbattlemenu_data_ov021_021ddb84.inc"
};
static const u32 data_ov021_021d4014[4] = { 0x00000020, 0x00000021, 0x00000028, 0x0000002a };
static const u32 data_ov021_021d4024[4] = { 0x00000008, 0x00000008, 0x00000010, 0x00000008 };
static IrcBattleMenuWindowParam data_ov021_021ddaf4[2] = { { 8, 8, 24, 4 }, { 8, 14, 24, 4 } };
static const TouchRect data_ov021_021d404c[9] = {
#include "asset/ircbattlemenu_data_ov021_021d404c.inc"
};
static const TouchRect data_ov021_021d3ff4[4] = {
#include "asset/ircbattlemenu_data_ov021_021d3ff4.inc"
};
static IrcBattleMenuWindowParam data_ov021_021ddb44[4] = {
#include "asset/ircbattlemenu_data_ov021_021ddb44.inc"
};
static const TouchRect data_ov021_021d4034[6] = {
#include "asset/ircbattlemenu_data_ov021_021d4034.inc"
};
static const TouchRect data_ov021_021d4004[4] = {
#include "asset/ircbattlemenu_data_ov021_021d4004.inc"
};
static u16 data_ov021_021ddad4[16] = {
#include "asset/ircbattlemenu_data_ov021_021ddad4.inc"
};
// Black's layout (size 0x308, from its GFL_ProcInitSubsystem); the unk_ fields are the ones this file never touches.
typedef struct IrcBattleMenu IrcBattleMenu;
struct IrcBattleMenu {
    void (*seq)(IrcBattleMenu *wk);                      // 0x000: the sequence function the main proc calls
    BOOL (*buttonFunc)(u32 button, IrcBattleMenu *wk);   // 0x004: the touch handler of the buttons on screen
    u32 mode;                                            // 0x008: what the menu chose, given to EventIRCWork
    u32 useBattleBox;                                    // 0x00c: given to EventIRCWork
    u16 heapId;                                          // 0x010
    u16 unk_12;
    BmpWin *buttonWin[4];                                // 0x014
    ButtonMan *buttonMan;                                // 0x024
    MsgData *msgData;                                    // 0x028
    Font *font;                                          // 0x02c
    GameData *gameData;                                  // 0x030
    u32 unk_34;
    StrBuf *strBuf;                                      // 0x038
    u32 cursorChar;                                      // 0x03c: the frame's chars (pos | count << 16)
    u32 bgChar4;                                         // 0x040: BG 4's chars
    u32 bgChar0;                                         // 0x044: BG 0's chars
    u16 palette[8][16];                                  // 0x048: the sub screen's BG palette
    u16 paletteLight[8][16];                             // 0x148: the same, lightened
    u8 unk_248[0x20];
    TCB *vblankTcb;                                      // 0x268
    ClActUnit *clUnit;                                   // 0x26c
    u32 clRes[6];                                        // 0x270: palettes, chars, cell anims (main, sub each)
    u32 unk_288;
    ClActor *subActors[2];                               // 0x28c
    ClActor *actors[5];                                  // 0x294: the buttons
    KeyCursor *keyCursor;                                // 0x2a8
    s32 bgScroll;                                        // 0x2ac
    BmpWin *msgWin;                                      // 0x2b0
    PrintStream *printStream;                            // 0x2b4
    TCBExManager *tcbExMgr;                              // 0x2b8
    PrintQueue *printQueue;                              // 0x2bc
    AppTaskMenu *taskMenu;                               // 0x2c0
    AppTaskMenuItem taskMenuItems[2];                    // 0x2c4
    AppTaskMenuRes *taskMenuRes;                         // 0x2dc
    AppTaskMenuWin *taskMenuWin;                         // 0x2e0
    EventIRCWork *ircWork;                               // 0x2e4
    WaitIcon *waitIcon;                                  // 0x2e8
    void *objPltBuf;                                     // 0x2ec: the OBJ palettes saved for the fade
    void *bgPltBuf;                                      // 0x2f0: the BG palettes saved for the fade
    s32 buttonCount;                                     // 0x2f4
    u32 unk_2f8;                                         // 0x2f8: set when the party was saved
    u32 unk_2fc;
    u32 selected;                                        // 0x300: the button touched
    u16 unk_304;
    u8 unk_306;                                          // 0x306: set when leaving
    u8 unk_307;
};

enum {
    CLRES_PLT_MAIN,
    CLRES_PLT_SUB,
    CLRES_CHAR_MAIN,
    CLRES_CHAR_SUB,
    CLRES_CELL_MAIN,
    CLRES_CELL_SUB,
};

void func_ov021_021961a4(IrcBattleMenu *wk, u16 param_2, s32 param_3);
BOOL func_ov021_021975d0(IrcBattleMenu *wk);
void func_ov021_02196a4c(IrcBattleMenu *wk);
void func_ov021_021966f0(IrcBattleMenu *wk);
void func_ov021_02196cd8(uintptr_t param_1);
void func_ov021_02196a60(IrcBattleMenu *wk);
void func_ov021_02196e04(IrcBattleMenu *wk);
void func_ov021_02196804(IrcBattleMenu *wk);
void func_ov021_02197700(IrcBattleMenu *wk);
void func_ov021_02197644(IrcBattleMenu *wk, s32 param_2);
void func_ov021_02196c68(IrcBattleMenu *wk);
void func_ov021_02197190(IrcBattleMenu *wk);
void func_ov021_0219630c(int count, const u32 *msgIds, IrcBattleMenu *wk, IrcBattleMenuWindowParam *params, const u32 *centersX, const u32 *centersY);
BOOL func_ov021_02196bf8(s32 param_1, IrcBattleMenu *wk);
void func_ov021_02196ff0(IrcBattleMenu *wk);
void func_ov021_0219728c(IrcBattleMenu *wk);
BOOL func_ov021_02196f44(u32 param_1, IrcBattleMenu *wk);
void func_ov021_021973a0(IrcBattleMenu *wk);
void func_ov021_021978ac(IrcBattleMenu *wk);
void func_ov021_02197798(IrcBattleMenu *wk);
void func_ov021_0219707c(IrcBattleMenu *wk);
BOOL func_ov021_02196974(u32 param_1, IrcBattleMenu *wk);

void func_ov021_02196768(IrcBattleMenu *wk);
void func_ov021_021974a0(IrcBattleMenu *wk);

extern const ClActSysSetup data_020a1448;

void func_ov021_02196030(IrcBattleMenu *wk, void (*seq)(IrcBattleMenu *)) {
    wk->seq = seq;
}

// line: the caller's __LINE__ in the original, kept as it was compiled
void func_ov021_02196034(IrcBattleMenu *wk, void (*seq)(IrcBattleMenu *), u32 line) {
    func_ov021_02196030(wk, seq);
}

void func_ov021_0219603c(IrcBattleMenu *wk) {
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x512, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x00000,
                          0x8000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(0);
        GFL_BGSysLoadScr(0);
        GFL_BGSysSetBGEnabled(0, TRUE);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x00000,
                          0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(1);
        GFL_BGSysLoadScr(1);
        GFL_BGSysSetBGEnabled(1, TRUE);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf800, GX_BG_CHARBASE_0x00000,
                          0x8000, GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysLoadScr(4);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xf000, GX_BG_CHARBASE_0x08000,
                          0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(5, TRUE);
        GFL_BGSysFillChar(5, 0, 1, 0);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, 17);
        GFL_BGSysLoadScr(5);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xc800, GX_BG_CHARBASE_0x00000,
                          0x8000, GX_BG_EXTPLTT_01, 2, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysLoadScr(6);
    }
    {
        BGSetup setup = { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0xe800, GX_BG_CHARBASE_0x08000,
                          0x8000, GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(7, TRUE);
        GFL_BGSysLoadScr(7);
    }
}

void func_ov021_021961a4(IrcBattleMenu *wk, u16 param_2, s32 param_3) {
    PaletteFade *fade;
    u16 *addr;
    u32 size;

    fade = func_02021518(wk->heapId);
    func_0202155c(fade, param_2, 0x200U, wk->heapId);
    size = param_3 << 5;
    func_020216d4(fade, param_2, 0U, size);
    func_02021fd8(fade, (u32) param_2, 0U, (u16) (param_3 * 0x10), 6U, 0U);
    addr = func_02021730(fade, param_2);
    DC_FlushRange(addr, size);
    if (param_2 != 1) {
        if (param_2 == 3) {
            GXS_LoadOBJPltt(addr, 0U, size);
        }
    } else {
        GXS_LoadBGPltt(addr, 0U, size);
    }
    func_020215a0(fade, param_2);
    func_02021540(fade);
}

void func_ov021_02196228(IrcBattleMenu *wk, s32 param_2) {
    if (param_2 != 0) {
        MI_CpuCopy8((void *)HW_DB_OBJ_PLTT, wk->objPltBuf, 0x200);
        MI_CpuCopy8((void *)HW_DB_BG_PLTT, wk->bgPltBuf, 0x200);
        func_ov021_021961a4(wk, 3, 0xe);
        func_ov021_021961a4(wk, 1, 9);
        return;
    }
    GXS_LoadOBJPltt(wk->objPltBuf, 0, 0x1c0);
    GXS_LoadBGPltt(wk->bgPltBuf, 0, 0x120);
}

void func_ov021_02196290(IrcBattleMenu *wk) {
    WaitIcon *pWVar1;

    if (wk->waitIcon != (WaitIcon *)0x0) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = 0;
    }
    pWVar1 = func_0202ee8c(wk->tcbExMgr, wk->msgWin, 0xf,
                                                  0x10, wk->heapId);
    wk->waitIcon = pWVar1;
}

void func_ov021_021962c4(IrcBattleMenu *wk) {
    if (wk->waitIcon != (WaitIcon *)0x0) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = 0;
    }
    func_0201f7a8(wk->msgWin, 2);
    func_020456f8(wk->msgWin);
    GFL_BGSysQueueScrLoad(5);
}

void func_ov021_021962f4(IrcBattleMenu *wk) {
    func_ov021_021962c4(wk);
    BmpWin_Free(wk->msgWin);
    wk->msgWin = 0;
}

void func_ov021_0219630c(int count, const u32 *msgIds, IrcBattleMenu *wk, IrcBattleMenuWindowParam *params, const u32 *centersX, const u32 *centersY) {
    int i;
    int j;

    GFL_BGSysClearScr(7);
    wk->buttonCount = count;
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x100, 0x20, wk->heapId);
    for (i = 0; i < 4; i++) {
        if (wk->buttonWin[i] != NULL) {
            func_020456f8(wk->buttonWin[i]);
            func_0201f7a8(wk->buttonWin[i], 2);
            BmpWin_Free(wk->buttonWin[i]);
        }
        wk->buttonWin[i] = NULL;
    }
    for (j = 0; j < count; j++) {
        s32 x;
        s32 y;

        GFL_TextRndUpdateColorIndexLUT(0xf, 2, 0);
        wk->buttonWin[j] = BmpWin_CreateDynamic(7, params[j].x, params[j].y, params[j].width, params[j].height, 8, FALSE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->buttonWin[j]), 0);
        GFL_MsgDataLoadStrbuf(wk->msgData, msgIds[j], wk->strBuf);
        if (centersX != NULL) {
            x = centersX[j] - GFL_FontGetBlockWidth(wk->strBuf, wk->font, 0) / 2;
        } else {
            x = 0;
        }
        if (centersY != NULL) {
            y = centersY[j] - GFL_FontGetBlockHeight(wk->strBuf, wk->font) / 2;
        } else {
            y = 0;
        }
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->buttonWin[j]), x, y, wk->strBuf, wk->font);
        BmpWin_FlushChar(wk->buttonWin[j]);
        BmpWin_FlushMap(wk->buttonWin[j]);
    }
    if (wk->buttonMan != NULL) {
        func_02051c6c(wk->buttonMan);
    }
    wk->buttonMan = NULL;
    GFL_BGSysQueueScrLoad(7);
    GFL_BGSysSetBGEnabled(7, 1);
}

void func_ov021_02196454(IrcBattleMenu *wk) {
    int i;

    if (wk->buttonMan != NULL) {
        func_02051c6c(wk->buttonMan);
    }
    wk->buttonMan = NULL;
    for (i = 0; i < 4; i++) {
        if (wk->buttonWin[i] != NULL) {
            func_020456f8(wk->buttonWin[i]);
            GFL_BGSysQueueScrLoad(5);
            func_0201f7a8(wk->buttonWin[i], 2);
            BmpWin_Free(wk->buttonWin[i]);
        }
        wk->buttonWin[i] = NULL;
    }
    wk->buttonCount = 0;
}

void func_ov021_0219649c(u32 button, u32 event, void *work) {
    IrcBattleMenu *wk = work;

    if (event == BMN_EVENT_TOUCH) {
        if (wk->buttonFunc != NULL) {
            if (!wk->buttonFunc(button, wk)) {
                return;
            }
        }
    }
}

void func_ov021_021964b0(u16 (*palettes)[16], const u16 *offsets, int count) {
    int i;
    int j;

    for (i = 0; i < count; i++) {
        for (j = 0; j < 16; j++) {
            u16 color = palettes[i][j];
            u16 r = color & GX_RGB_R_MASK;
            u16 g = (color & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
            u16 b = (color & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;

            r += offsets[j];
            g += offsets[j];
            b += offsets[j];
            if (r > 31) {
                r = 31;
            }
            if (g > 31) {
                g = 31;
            }
            if (b > 31) {
                b = 31;
            }
            palettes[i][j] = r + (g << GX_RGB_G_SHIFT) + (b << GX_RGB_B_SHIFT);
        }
    }
}

void func_ov021_02196538(IrcBattleMenu *wk) {
    ArcTool *arc;
    NNSG2dPaletteData *palette;
    void *ptr;
    u32 uVar;

    wk->unk_2f8 = 0;
    wk->strBuf = GFL_StrBufCreate(300, wk->heapId);
    wk->font = GFL_FontCreate(0x17, 0, 0, 0, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(0, 2, 0x2e, wk->heapId);
    arc = GFL_ArcSysCreateFileHandle(0x1c, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, 0, 0, 0, wk->heapId);
    uVar = GFL_BGSysLoadArcNCGRDynamic(arc, 6, 0, 0, 0, wk->heapId);
    wk->bgChar0 = uVar;
    GFL_G2DIOLoadNSCRSync(arc, 0xd, 0, 0, uVar & 0xffff, 0, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    arc = GFL_ArcSysCreateFileHandle(0x1d, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 2, 4, 0, 0, wk->heapId);
    ptr = GFL_G2DIOReadNCLRArc(arc, 2, &palette, wk->heapId);
    MI_CpuCopy8(palette->pRawData, wk->palette, 0x100);
    MI_CpuCopy8(palette->pRawData, wk->paletteLight, 0x100);
    func_ov021_021964b0(wk->paletteLight, data_ov021_021ddad4, 8);
    GFL_HeapFree(ptr);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0, 0, wk->heapId);
    uVar = GFL_BGSysLoadArcNCGRDynamic(arc, 4, 4, 0, 0, wk->heapId);
    wk->bgChar4 = uVar;
    GFL_G2DIOLoadNSCRSync(arc, 9, 4, 0, uVar & 0xffff, 0, 0, wk->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 0xb, 6, 0, wk->bgChar4 & 0xffff, 0, 0, wk->heapId);
    GFL_ArcToolFree(arc);
    uVar = LoadCursorImageEndOfHeap(5, 0xc, 0, wk->heapId);
    wk->cursorChar = uVar;
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x160, 0x20, wk->heapId);
}

void func_ov021_021966b0(IrcBattleMenu *wk, u32 param_2) {
    ArcTool *arc;

    arc = GFL_ArcSysCreateFileHandle(0x1d, wk->heapId);
    GFL_G2DIOLoadNSCRSync
                        (arc, param_2, 6, 0, wk->bgChar4 & 0xffff, 0, 0, wk->heapId
                        );
    GFL_ArcToolFree(arc);
    GFL_BGSysSetBGEnabled(6, 1);
}

void func_ov021_021966f0(IrcBattleMenu *wk) {
    u32 buf[4] = { 0, 1, 0xe, 0xd };

    func_ov021_021966b0(wk, 0xb);
    func_ov021_0219707c(wk);
    func_ov021_0219630c(4, buf, wk, data_ov021_021ddb84, data_ov021_021d4014, data_ov021_021d4024);
    wk->buttonMan = func_02051b84(data_ov021_021d404c, func_ov021_0219649c, wk, wk->heapId);
    wk->buttonFunc = func_ov021_02196974;
    func_ov021_02196034(wk, func_ov021_02196a4c, 0x340);
}

void func_ov021_02196768(IrcBattleMenu *wk) {
    u32 pos;

    func_0201dd68();
    func_ov021_02196454(wk);
    GFL_BGSysFreeFilledChar(5U, 1U, 0U);
    pos = wk->cursorChar;
    GFL_BGSysFreeCharMemory(5U, (u32) (u16) pos, (u32) (u16) (pos >> 0x10));
    GFL_BGSysReleaseBG(5U);
    GFL_BGSysReleaseBG(4U);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
    GFL_StrBufFree(wk->strBuf);
    GFL_BGSysSetBGEnabled(5U, 0U);
}

void func_ov021_021967bc(IrcBattleMenu *wk) {
    GameSystem *gsys;
    GameCommSys *comm;

    if (GFL_WipeIsFinished()) {
        if (wk->mode != 0xc) {
            gsys = func_ov010_02155a90(wk->ircWork);
            comm = GSYS_GetGameCommSystem(gsys);
            if (GameCommSys_BootCheck(comm) == 0) {
                func_ov021_02196034(wk, NULL, 0x361);
            }
        } else {
            func_ov021_02196034(wk, NULL, 0x365);
        }
    }
}

void func_ov021_02196804(IrcBattleMenu *wk) {
    wk->unk_306 = 1;
    GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
    if (wk->mode != 0xc) {
        if (GameCommSys_BootCheck(GSYS_GetGameCommSystem(func_ov010_02155a90(wk->ircWork))) != 0) {
            func_02026400(GSYS_GetGameCommSystem(func_ov010_02155a90(wk->ircWork)));
        }
    }
    func_ov021_02196034(wk, func_ov021_021967bc, 0x37c);
}

void func_ov021_0219686c(IrcBattleMenu *wk) {
    TrainerDataSave *config;

    if (wk->mode == 0xc) {
        func_ov021_02196034(wk, func_ov021_02196804, 0x38c);
        return;
    }
    config = getTrainerDataBlkAddress(func_ov010_02155a94(wk->ircWork));
    if (func_02008428((void *)config) == 1) {
        func_ov021_02196034(wk, func_ov021_02197700, 0x38f);
        return;
    }
    if (wk->mode - 6U <= 2) {
        func_ov021_02196034(wk, func_ov021_02196804, 0x394);
    } else {
        func_ov021_02196034(wk, func_ov021_02196a60, 0x397);
    }
}

void func_ov021_021968d8(IrcBattleMenu *wk) {
}

void func_ov021_021968dc(IrcBattleMenu *wk) {
    ClActorCallback cb;
    s32 idx;

    idx = wk->selected;
    cb.type = 0;
    cb.param = (uintptr_t)wk;
    cb.func = (ClActorAnimCallback)func_ov021_0219686c;
    if (idx == 5) {
        idx = 0;
    } else if (idx == 6) {
        idx = 1;
    } else if (idx == 7) {
        idx = 3;
    }
    func_0204b92c(wk->actors[idx], TRUE);
    func_0204ba00(wk->actors[idx], &cb);
    func_0204b9a0(wk->actors[idx]);
    func_ov021_02196034(wk, func_ov021_021968d8, 0x3bf);
}

void func_ov021_02196944(IrcBattleMenu *wk) {
    if (func_ov021_021975d0(wk) != 0 && func_02035c60() != 0) {
        func_ov021_021962f4(wk);
        func_ov021_02196034(wk, func_ov021_02196a4c, 0x3cb);
    }
}

BOOL func_ov021_02196974(u32 param_1, IrcBattleMenu *wk) {
    u32 ret;
    PokeParty *party;

    ret = 0;
    wk->selected = param_1;
    switch (param_1) {
    case 0:
    case 5:
        GFL_SndSEPlay(0x73b);
        func_ov021_02196034(wk, func_ov021_021968dc, 0x3de);
        wk->mode = 1;
        ret = 1;
        break;
    case 1:
    case 6:
        party = GameData_GetParty(wk->gameData);
        if (!(howManyPartyPokesAreNotEggs(party) >= 2)) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 0x2c, wk->strBuf);
            func_ov021_02197644(wk, 0);
            func_ov021_02196034(wk, func_ov021_02196944, 0x3ea);
        } else {
            GFL_SndSEPlay(0x73b);
            wk->mode = 6;
            func_ov021_02196034(wk, func_ov021_021968dc, 0x3f0);
            ret = 1;
        }
        break;
    case 2:
        GFL_SndSEPlay(0x73b);
        wk->mode = 8;
        func_ov021_02196034(wk, func_ov021_021968dc, 0x3f6);
        ret = 1;
        break;
    case 3:
    case 7:
        GFL_SndSEPlay(0x73b);
        wk->mode = 7;
        func_ov021_02196034(wk, func_ov021_021968dc, 0x3fd);
        ret = 1;
        break;
    case 4:
        GFL_SndSEPlay(0x73c);
        wk->mode = 0xc;
        func_ov021_02196034(wk, func_ov021_021968dc, 0x404);
        break;
    }
    return ret;
}

void func_ov021_02196a4c(IrcBattleMenu *wk) {
    BOOL BVar1;

    BVar1 = GFL_WipeIsFinished();
    if (BVar1 != 0) {
        func_02051c88(wk->buttonMan);
    }
}

void func_ov021_02196a60(IrcBattleMenu *wk) {
    u32 buf[2] = { 0x00000003, 0x00000004 };

    func_ov021_02197190(wk);
    func_ov021_021966b0(wk, 0xc);
    func_ov021_0219630c(2, buf, wk, data_ov021_021ddaf4, NULL, NULL);
    wk->buttonMan = func_02051b84(data_ov021_021d3ff4, func_ov021_0219649c, wk, wk->heapId);
    wk->buttonFunc = (BOOL (*)(u32, IrcBattleMenu *))func_ov021_02196bf8;
    func_ov021_02196034(wk, func_ov021_02196ff0, 0x432);
}

void func_ov021_02196acc(IrcBattleMenu *wk) {
    if (wk->mode == 5) {
        if (func_0200bba8(getBattleBox(wk->ircWork->save)) != 0) {
            func_ov021_02196034(wk, func_ov021_02196e04, 0x443);
            return;
        }
        func_ov021_02196034(wk, func_ov021_02196804, 0x446);
        return;
    }
    func_ov021_02196034(wk, func_ov021_02196c68, 0x44D);
}

void func_ov021_02196b24(IrcBattleMenu *wk) {
}

void func_ov021_02196b28(IrcBattleMenu *wk) {
    ClActorCallback cb;
    s32 idx;

    idx = 0;
    if (wk->mode == 5) {
        idx = 1;
    }
    cb.type = 0;
    cb.param = (uintptr_t)wk;
    cb.func = (ClActorAnimCallback)func_ov021_02196acc;
    func_0204b92c(wk->actors[idx], TRUE);
    func_0204ba00(wk->actors[idx], &cb);
    func_0204b9a0(wk->actors[idx]);
    func_ov021_02196034(wk, func_ov021_02196b24, 0x471);
}

void func_ov021_02196b7c(IrcBattleMenu *wk) {
    func_ov021_02196034(wk, func_ov021_021966f0, 0x47a);
}

void func_ov021_02196b90(IrcBattleMenu *wk) {
}

void func_ov021_02196b94(IrcBattleMenu *wk) {
    ClActorCallback cb;

    cb.type = 0;
    cb.param = (uintptr_t)wk;
    cb.func = (ClActorAnimCallback)func_ov021_02196b7c;
    func_0204b92c(wk->actors[wk->selected], TRUE);
    func_0204ba00(wk->actors[wk->selected], &cb);
    func_0204b9a0(wk->actors[wk->selected]);
    func_ov021_02196034(wk, func_ov021_02196b90, 0x492);
}

BOOL func_ov021_02196bf8(s32 param_1, IrcBattleMenu *wk) {
    BOOL ret = TRUE;

    wk->selected = param_1;
    switch (param_1) {
    case 0:
        GFL_SndSEPlay(0x73b);
        func_ov021_02196034(wk, func_ov021_02196b28, 0x4a3);
        ret = TRUE;
        break;
    case 1:
        GFL_SndSEPlay(0x73b);
        wk->mode = 5;
        func_ov021_02196034(wk, func_ov021_02196b28, 0x4a9);
        ret = TRUE;
        break;
    case 2:
        GFL_SndSEPlay(0x73c);
        wk->mode = 0xc;
        func_ov021_02196034(wk, func_ov021_02196b94, 0x4af);
        break;
    }
    return ret;
}

void func_ov021_02196c68(IrcBattleMenu *wk) {
    u32 buf[4] = { 0x00000005, 0x00000006, 0x00000007, 0x0000001f };

    func_ov021_0219728c(wk);
    func_ov021_021966b0(wk, 0xd);
    func_ov021_0219630c(4, buf, wk, data_ov021_021ddb44, NULL, NULL);
    wk->buttonMan = func_02051b84(data_ov021_021d4034, func_ov021_0219649c, wk, wk->heapId);
    wk->buttonFunc = func_ov021_02196f44;
    func_ov021_02196034(wk, func_ov021_02196ff0, 0x4cc);
}

void func_ov021_02196cd8(uintptr_t param_1) {
    IrcBattleMenu *wk = (IrcBattleMenu *)param_1;

    if (wk->useBattleBox == 2) {
        if (wk->mode == 5) {
            wk->mode = 1;
            func_ov021_02196034(wk, func_ov021_02196a60, 0x4DD);
            return;
        }
        func_ov021_02196034(wk, func_ov021_02196c68, 0x4E1);
        return;
    }
    func_ov021_02196034(wk, func_ov021_02196804, 0x4E5);
}

void func_ov021_02196d20(IrcBattleMenu *wk) {
}

void func_ov021_02196d24(IrcBattleMenu *wk) {
    ClActorCallback cb;
    u8 tbl[3] = { 1, 0, 2 };
    s32 idx;

    idx = tbl[wk->useBattleBox];
    cb.type = 0;
    cb.param = (uintptr_t)wk;
    cb.func = (ClActorAnimCallback)func_ov021_02196cd8;
    func_0204b92c(wk->actors[idx], TRUE);
    func_0204ba00(wk->actors[idx], &cb);
    func_0204b9a0(wk->actors[idx]);
    func_ov021_02196034(wk, func_ov021_02196d20, 0x504);
}

void func_ov021_02196d88(IrcBattleMenu *wk) {
    func_02051c88(wk->buttonMan);
}

BOOL func_ov021_02196d94(s32 param_1, IrcBattleMenu *wk) {
    u32 ret;

    ret = 0;
    wk->selected = param_1;
    switch (param_1) {
    case 1:
        GFL_SndSEPlay(0x73b);
        wk->useBattleBox = 0;
        func_ov021_02196034(wk, func_ov021_02196d24, 0x524);
        ret = 1;
        break;
    case 0:
        GFL_SndSEPlay(0x73b);
        ret = 1;
        wk->useBattleBox = 1;
        func_ov021_02196034(wk, func_ov021_02196d24, 0x52a);
        break;
    case 2:
        GFL_SndSEPlay(0x73c);
        wk->useBattleBox = 2;
        func_ov021_02196034(wk, func_ov021_02196d24, 0x530);
        break;
    }
    return ret;
}

void func_ov021_02196e04(IrcBattleMenu *wk) {
    u32 buf[2] = { 0x00000028, 0x00000029 };

    func_ov021_021973a0(wk);
    func_ov021_021966b0(wk, 0xd);
    func_ov021_0219630c(2, buf, wk, data_ov021_021ddaf4, NULL, NULL);
    wk->buttonMan = func_02051b84(data_ov021_021d4004, func_ov021_0219649c, wk, wk->heapId);
    wk->buttonFunc = (BOOL (*)(u32, IrcBattleMenu *))func_ov021_02196d94;
    func_ov021_02196034(wk, func_ov021_02196d88, 0x54d);
}

void func_ov021_02196e70(IrcBattleMenu *wk) {
    BattleBoxSave *battleBox;

    if (wk->mode == 0xc) {
        func_ov021_02196034(wk, func_ov021_02196a60, 0x558);
        return;
    }
    battleBox = getBattleBox(wk->ircWork->save);
    if (func_0200bba8(battleBox) != 0) {
        func_ov021_02196034(wk, func_ov021_02196e04, 0x55d);
        return;
    }
    func_ov021_02196034(wk, func_ov021_02196804, 0x560);
}

void func_ov021_02196ec8(IrcBattleMenu *wk) {
}

void func_ov021_02196ecc(IrcBattleMenu *wk) {
    s32 idx;
    ClActorCallback cb;

    switch (wk->mode) {
    case 1:
        idx = 0;
        break;
    case 2:
        idx = 1;
        break;
    case 3:
        idx = 2;
        break;
    case 4:
        idx = 3;
        break;
    default:
        idx = 4;
        break;
    }
    cb.type = 0;
    cb.param = (uintptr_t)wk;
    cb.func = (ClActorAnimCallback)func_ov021_02196e70;
    func_0204b92c(wk->actors[idx], TRUE);
    func_0204ba00(wk->actors[idx], &cb);
    func_0204b9a0(wk->actors[idx]);
    func_ov021_02196034(wk, func_ov021_02196ec8, 0x58f);
}

BOOL func_ov021_02196f44(u32 param_1, IrcBattleMenu *wk) {
    u32 ret;

    ret = 0;
    wk->selected = param_1;
    switch (param_1) {
    case 0:
        GFL_SndSEPlay(0x73b);
        wk->mode = 1;
        func_ov021_02196034(wk, func_ov021_02196ecc, 0x5b1);
        ret = 1;
        break;
    case 1:
        GFL_SndSEPlay(0x73b);
        wk->mode = 2;
        func_ov021_02196034(wk, func_ov021_02196ecc, 0x5b7);
        ret = 1;
        break;
    case 2:
        GFL_SndSEPlay(0x73b);
        wk->mode = 3;
        func_ov021_02196034(wk, func_ov021_02196ecc, 0x5bd);
        ret = 1;
        break;
    case 3:
        GFL_SndSEPlay(0x73b);
        wk->mode = 4;
        func_ov021_02196034(wk, func_ov021_02196ecc, 0x5c3);
        ret = 1;
        break;
    case 4:
        GFL_SndSEPlay(0x73c);
        wk->mode = 0xc;
        func_ov021_02196034(wk, func_ov021_02196ecc, 0x5c9);
        break;
    }
    return ret;
}

void func_ov021_02196ff0(IrcBattleMenu *wk) {
    func_02051c88(wk->buttonMan);
}

void func_ov021_02196ffc(IrcBattleMenu *wk) {
    ArcTool *arc;
    u32 result;

    arc = GFL_ArcSysCreateFileHandle(0x1d, wk->heapId);
    result = func_0204a6c8(arc, 5, 0, 1, wk->heapId);
    wk->clRes[CLRES_CHAR_SUB] = result;
    result = func_0204ac18(arc, 3, 1, 0, 0, 10, wk->heapId);
    wk->clRes[CLRES_PLT_SUB] = result;
    result = func_0204af28(arc, 7, 0x10, wk->heapId);
    wk->clRes[CLRES_CELL_SUB] = result;
    GFL_ArcToolFree(arc);
}

void func_ov021_02197058(IrcBattleMenu *wk) {
    ClActor *actor;
    s32 i;

    i = 0;
    do {
        actor = wk->actors[i];
        if (actor != NULL) {
            func_0204b3b4(actor);
            wk->actors[i] = NULL;
        }
        i = i + 1;
    } while (i < 5);
}

void func_ov021_0219707c(IrcBattleMenu *wk) {
    u8 xs[5] = { 0x80, 0x80, 0x80, 0x80, 0xe0 };
    u8 ys[5] = { 0x60, 0x60, 0x60, 0x60, 0xb1 };
    u8 seqs[5] = { 2, 1, 3, 4, 0 };
    ClActorSetup setup;
    s32 i;

    func_ov021_02197058(wk);
    for (i = 0; i < 5; i++) {
        setup.x = xs[i];
        setup.y = ys[i];
        setup.sequence = seqs[i];
        setup.priority = 0;
        setup.bgPriority = 1;
        wk->actors[i] = func_0204b294(wk->clUnit, wk->clRes[CLRES_CHAR_SUB], wk->clRes[CLRES_PLT_SUB], wk->clRes[CLRES_CELL_SUB], &setup, 1, wk->heapId);
        func_0204b92c(wk->actors[i], FALSE);
        func_0204b3dc(wk->actors[i], TRUE);
        func_0204b688(wk->actors[i], TRUE);
    }
}

void func_ov021_02197190(IrcBattleMenu *wk) {
    u8 xs[3] = { 0x80, 0x80, 0xe0 };
    u8 ys[3] = { 0x48, 0x78, 0xb1 };
    u8 seqs[3] = { 9, 10, 0 };
    ClActorSetup setup;
    s32 i;

    func_ov021_02197058(wk);
    for (i = 0; i < 3; i++) {
        setup.x = xs[i];
        setup.y = ys[i];
        setup.sequence = seqs[i];
        setup.priority = 0;
        setup.bgPriority = 1;
        wk->actors[i] = func_0204b294(wk->clUnit, wk->clRes[CLRES_CHAR_SUB], wk->clRes[CLRES_PLT_SUB], wk->clRes[CLRES_CELL_SUB], &setup, 1, wk->heapId);
        func_0204b92c(wk->actors[i], FALSE);
        func_0204b3dc(wk->actors[i], TRUE);
        func_0204b688(wk->actors[i], TRUE);
    }
}

void func_ov021_0219728c(IrcBattleMenu *wk) {
    u8 xs[5] = { 0x80, 0x80, 0x80, 0x80, 0xe0 };
    u8 ys[5] = { 0x18, 0x40, 0x68, 0x90, 0xb1 };
    u8 seqs[5] = { 5, 6, 7, 8, 0 };
    ClActorSetup setup;
    s32 i;

    func_ov021_02197058(wk);
    for (i = 0; i < 5; i++) {
        setup.x = xs[i];
        setup.y = ys[i];
        setup.sequence = seqs[i];
        setup.priority = 0;
        setup.bgPriority = 1;
        wk->actors[i] = func_0204b294(wk->clUnit, wk->clRes[CLRES_CHAR_SUB], wk->clRes[CLRES_PLT_SUB], wk->clRes[CLRES_CELL_SUB], &setup, 1, wk->heapId);
        func_0204b92c(wk->actors[i], FALSE);
        func_0204b3dc(wk->actors[i], TRUE);
        func_0204b688(wk->actors[i], TRUE);
    }
}

void func_ov021_021973a0(IrcBattleMenu *wk) {
    u8 xs[3] = { 0x80, 0x80, 0xe0 };
    u8 ys[3] = { 0x48, 0x78, 0xb1 };
    u8 seqs[3] = { 0x11, 0x11, 0 };
    ClActorSetup setup;
    s32 i;

    func_ov021_02197058(wk);
    for (i = 0; i < 3; i++) {
        setup.x = xs[i];
        setup.y = ys[i];
        setup.sequence = seqs[i];
        setup.priority = 0;
        setup.bgPriority = 1;
        wk->actors[i] = func_0204b294(wk->clUnit, wk->clRes[CLRES_CHAR_SUB], wk->clRes[CLRES_PLT_SUB], wk->clRes[CLRES_CELL_SUB], &setup, 1, wk->heapId);
        func_0204b92c(wk->actors[i], FALSE);
        func_0204b3dc(wk->actors[i], TRUE);
        func_0204b688(wk->actors[i], TRUE);
    }
}

void func_ov021_0219749c(IrcBattleMenu *wk, u32 param_2, u32 param_3, BOOL param_4, u32 param_5) {
}

void func_ov021_021974a0(IrcBattleMenu *wk) {
    ClActor *pCVar1;
    u32 palette;
    s32 i;

    i = 0;
    do {
        pCVar1 = wk->actors[i];
        if (pCVar1 != NULL) {
            func_0204b3b4(pCVar1);
            wk->actors[i] = NULL;
        }
        i = i + 1;
    } while (i < 5);
    i = 0;
    do {
        pCVar1 = wk->subActors[i];
        if (pCVar1 != NULL) {
            func_0204b3b4(pCVar1);
            wk->subActors[i] = NULL;
        }
        i = i + 1;
    } while (i < 2);
    i = 0;
    do {
        palette = wk->clRes[i];
        if (palette != 0) {
            func_0204ada4(palette);
        }
        i = i + 1;
    } while (i < 2);
    for (; i < 4; i = i + 1) {
        palette = wk->clRes[i];
        if (palette != 0) {
            func_0204a8d4(palette);
        }
    }
    for (; i < 6; i = i + 1) {
        palette = wk->clRes[i];
        if (palette != 0) {
            func_0204afd8(palette);
        }
    }
}

void func_ov021_02197534(IrcBattleMenu *wk) {
    AppTaskMenuInit init;

    init.heapId = wk->heapId;
    init.itemCount = 2;
    init.items = wk->taskMenuItems;
    init.posType = 1;
    init.x = 0x20;
    init.y = 0xe;
    init.width = 0xd;
    init.height = 3;
    wk->taskMenuItems[0].str = GFL_StrBufCreate(100, wk->heapId);
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x1a, wk->taskMenuItems[0].str);
    wk->taskMenuItems[0].color = 0x39e3;
    wk->taskMenuItems[1].str = GFL_StrBufCreate(100, wk->heapId);
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x1b, wk->taskMenuItems[1].str);
    wk->taskMenuItems[1].color = 0x39e3;
    wk->taskMenuItems[0].type = 0;
    wk->taskMenuItems[1].type = 0;
    wk->taskMenu = func_02027778(&init, wk->taskMenuRes);
    func_02027cd8(wk->taskMenu, TRUE);
    GFL_StrBufFree(wk->taskMenuItems[0].str);
    GFL_StrBufFree(wk->taskMenuItems[1].str);
}

BOOL func_ov021_021975d0(IrcBattleMenu *wk) {
    BmpWin *window;
    PrintStream *stream;

    stream = wk->printStream;
    if (stream != NULL) {
        window = wk->msgWin;
        if (window != NULL) {
            func_020286c4(wk->keyCursor, stream, window);
        }
        switch (func_0201ce30(wk->printStream)) {
        case 2:
            func_0201ce48(wk->printStream);
            wk->printStream = NULL;
            break;
        case 1:
            if (func_02035c60() != 0) {
                GFL_SndSEPlay(0x547U);
                func_0201ce38(wk->printStream);
            }
            break;
        case 0:
            if (func_02035c38() != 0) {
                func_0201ce5c(wk->printStream, 0);
            }
            break;
        }
        return 0U;
    }
    return 1U;
}

void func_ov021_02197644(IrcBattleMenu *wk, s32 param_2) {
    BmpWin *pBVar1;
    GFLBitmap *pGVar2;
    s32 wait;
    PrintStream *pPVar3;

    if (wk->msgWin == 0) {
        pBVar1 = BmpWin_CreateDynamic(5, 1, 3, 0x1e, 4, 0xb, 1);
        wk->msgWin = pBVar1;
    }
    pBVar1 = wk->msgWin;
    pGVar2 = BmpWin_GetBitmap(pBVar1);
    GFL_BitmapFill(pGVar2, 0xf);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0xf);
    if (param_2 == 0) {
        wait = func_02012ffc();
        pPVar3 = func_0201cce4(pBVar1, 0, 0, wk->strBuf, wk->font, wait,
                                                      wk->tcbExMgr, 2, wk->heapId, 0xf);
        wk->printStream = pPVar3;
    }
    else {
        pGVar2 = BmpWin_GetBitmap(pBVar1);
        GFL_TextRendererDrawToBitmap(pGVar2, 0, 0, wk->strBuf, wk->font)
        ;
        wk->printStream = 0;
    }
    BmpWin_DrawFrame(pBVar1, 1, (u16)wk->cursorChar, 0xc);
    BmpWin_FlushChar(pBVar1);
    BmpWin_FlushMap(pBVar1);
    GFL_BGSysQueueScrLoad(5);
}

void func_ov021_02197700(IrcBattleMenu *wk) {
    func_ov021_02196228(wk, 1);
    GFL_BGSysFillScrAsync(5, 0);
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x19, wk->strBuf);
    func_ov021_02197644(wk, 0);
    func_ov021_02196034(wk, func_ov021_021978ac, 0x761);
}

void func_ov021_02197738(IrcBattleMenu *wk) {
    if (func_02012dd0(wk->gameData) == 2) {
        func_ov021_02196228(wk, 0);
        func_0201f7a8(wk->msgWin, 2);
        func_020456f8(wk->msgWin);
        GFL_BGSysQueueScrLoad(5);
        if (wk->mode == 1) {
            func_ov021_02196034(wk, func_ov021_02196a60, 0x776);
        } else {
            func_ov021_02196034(wk, func_ov021_02196804, 0x779);
        }
        wk->unk_2f8 = 1;
    }
}

void func_ov021_02197798(IrcBattleMenu *wk) {
    if (func_ov021_021975d0(wk)) {
        func_02012dac(wk->gameData);
        func_ov021_02196034(wk, func_ov021_02197738, 0x78e);
    }
}

void func_ov021_021977c0(IrcBattleMenu *wk) {
    if (func_ov021_021975d0(wk) && func_02035c60()) {
        func_ov021_02196228(wk, 0);
        GFL_BGSysClearScr(3);
        func_ov021_021962f4(wk);
        wk->mode = 0xc;
        func_ov021_02196034(wk, func_ov021_02196a4c, 0x7a2);
    }
}

void func_ov021_02197800(IrcBattleMenu *wk) {
    if (func_020279e8(wk->taskMenu)) {
        if (func_02027a04(wk->taskMenu) == 0) {
            if (func_020071f0(GameData_GetSaveControl(wk->gameData))) {
                GFL_MsgDataLoadStrbuf(wk->msgData, 0x2d, wk->strBuf);
                func_ov021_02197644(wk, 0);
                func_ov021_02196034(wk, func_ov021_021977c0, 0x7b9);
            } else {
                GFL_MsgDataLoadStrbuf(wk->msgData, 0x1c, wk->strBuf);
                func_ov021_02197644(wk, 1);
                func_ov021_02196290(wk);
                func_ov021_02196034(wk, func_ov021_02197798, 0x7bf);
            }
        } else {
            func_ov021_02196228(wk, 0);
            GFL_BGSysClearScr(3);
            func_ov021_021962f4(wk);
            wk->mode = 0xc;
            func_ov021_02196034(wk, func_ov021_02196a4c, 0x7c7);
        }
        func_02027858(wk->taskMenu);
        wk->taskMenu = 0;
    }
}

void func_ov021_021978ac(IrcBattleMenu *wk) {
    if (func_ov021_021975d0(wk)) {
        func_ov021_02197534(wk);
        func_ov021_02196034(wk, func_ov021_02197800, 0x7dc);
    }
}

void func_ov021_021978d4(void) {
    func_0204a648();
}

static BGSysVRAMConfig data_ov021_021ddb14 = {
#include "asset/ircbattlemenu_data_ov021_021ddb14.inc"
};

BOOL func_ov021_021978dc(GameProc *proc, u32 *state, void *param, void *work) {
    IrcBattleMenu *wk;
    GameSystem *gsys;

    GFL_HeapCreateChild(1, 0x1f, 0x18000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(IrcBattleMenu), 0x1f);
    MI_CpuFill8(wk, 0, sizeof(IrcBattleMenu));
    wk->heapId = 0x1f;
    GFL_BGSysCreate(wk->heapId);
    BmpWin_InitAllocator(wk->heapId);
    func_0201dd60();
    GFL_BGSysSetVRAMBanks(&data_ov021_021ddb14);
    {
        BGSysLCDConfig lcdConfig = { 1, 0, 0, 0 };

        GFL_BGSysSetLCDConfig(&lcdConfig);
    }
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    ClActSys_Create(&data_020a1448, &data_ov021_021ddb14, wk->heapId);
    wk->clUnit = func_0204b100(0x28, 0, wk->heapId);
    wk->vblankTcb = GFL_VBlankTCBAdd((void *)func_ov021_021978d4, wk, 0);
    func_ov021_0219603c(wk);
    func_ov021_02196538(wk);
    wk->objPltBuf = GFL_HeapAllocate(wk->heapId, 0x200, 0, "ircbattlemenu.c", 0x829);
    wk->bgPltBuf = GFL_HeapAllocate(wk->heapId, 0x200, 0, "ircbattlemenu.c", 0x82a);
    wk->tcbExMgr = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 2, 0);
    wk->keyCursor = func_02028590(0xf, 0, 1, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    wk->taskMenuRes = func_02027f60(5, 9, wk->font, wk->printQueue, wk->heapId);
    func_ov021_02196ffc(wk);
    func_ov021_0219749c(wk, 0x39, 0x9a, 0, 10);
    func_ov021_0219749c(wk, 0xc8, 0x26, 1, 0xb);
    GFL_WipeSet(0, 1, 1, 0, 6, 1, wk->heapId);
    func_ov021_02196034(wk, func_ov021_021966f0, 0x83b);
    wk->ircWork = param;
    gsys = func_ov010_02155a90(wk->ircWork);
    wk->gameData = GSYS_GetGameData(gsys);
    G2S_SetBlendAlpha(4, 1, 0xf, 4);
    func_0203d34c(0, wk->heapId);
    GFL_BGSysSetEnabledBGsA(0x13);
    GFL_BGSysSetEnabledBGsB(0x17);
    G2_SetBlendAlpha(0x10, 3, 9, 0xe);
    return 1;
}

BOOL func_ov021_02197aac(GameProc *proc, u32 *state, void *param, void *work) {
    IrcBattleMenu *wk = work;
    BOOL ret = TRUE;
    void (*callback)(IrcBattleMenu *) = wk->seq;
    GameSystem *gsys;
    WifiList *wifiList;

    wifiList = func_0201283c(wk->gameData);
    func_0201058c(wifiList);
    if (callback != NULL) {
        callback(wk);
        ret = FALSE;
    }
    if (wk->taskMenu != NULL) {
        func_02027974(wk->taskMenu);
    }
    if (wk->taskMenuWin != NULL) {
        func_02028178(wk->taskMenuWin);
    }
    GFL_BGSysMoveBG(4, 3, wk->bgScroll);
    wk->bgScroll--;
    GFL_TCBExMgrUpdate(wk->tcbExMgr);
    func_0201c4e4(wk->printQueue);
    func_0204a600();
    if (wk->unk_306 == 0) {
        gsys = func_ov010_02155a90(wk->ircWork);
        func_020121cc(gsys);
    }
    if (GFL_WipeIsFinished()) {
        gsys = func_ov010_02155a90(wk->ircWork);
        if (func_02012204(gsys)) {
            wk->mode = 0xc;
            ret = TRUE;
            func_0202223c(0, 0);
            func_0202223c(1, 0);
        }
    }
    return ret;
}

BOOL func_ov021_02197b5c(GameProc *proc, u32 *state, void *param, void *work) {
    IrcBattleMenu *wk = work;
    EventIRCWork *ircWork = param;
    BOOL BVar1;

    BVar1 = GFL_WipeIsFinished();
    if (BVar1 == 0) {
        return 0;
    }
    func_ov021_02196768(wk);
    func_ov010_02155a8c(ircWork, wk->mode);
    ircWork->useBattleBox = wk->useBattleBox;
    if (wk->waitIcon != (WaitIcon *)0x0) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = 0;
    }
    GFL_HeapFree(wk->objPltBuf);
    GFL_HeapFree(wk->bgPltBuf);
    func_ov021_021974a0(wk);
    if (wk->printStream != (PrintStream *)0x0) {
        func_0201ce48(wk->printStream);
    }
    func_02028604(wk->keyCursor);
    GFL_TCBExMgrFree(wk->tcbExMgr);
    GFL_BGSysReleaseBG(7);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    if (wk->msgWin != (BmpWin *)0x0) {
        BmpWin_Free(wk->msgWin);
    }
    if (wk->taskMenu != (AppTaskMenu *)0x0) {
        func_02027858(wk->taskMenu);
        wk->taskMenu = 0;
    }
    if (wk->taskMenuWin != (AppTaskMenuWin *)0x0) {
        func_02028148(wk->taskMenuWin);
    }
    func_02027fd8(wk->taskMenuRes);
    GFL_TCBRemove(wk->vblankTcb);
    func_0204b1cc(wk->clUnit);
    func_0204a5ac();
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x1f);
    return 1;
}

const GameProcFunctions data_ov021_021d3fe8 = {
    func_ov021_021978dc,
    func_ov021_02197aac,
    func_ov021_02197b5c,
};
