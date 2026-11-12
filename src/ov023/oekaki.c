// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov070/oekaki.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/oekaki.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "field/union_app.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/math_util.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/event_mapchange.h"

// oekaki.c, which the ROM names: the drawing board of the Union Room. The parent collects what every member draws
// in a frame and sends it back to all of them, so that each machine draws the same lines; a newcomer gets the
// picture so far in chunks. Names are ours

// The screen's graphics
#define OEKAKI_ARCID 191

#define OEKAKI_COLOR_TEXT PRINT_COLOR(1, 2, 15)
#define OEKAKI_COLOR_NAME_MINE PRINT_COLOR(3, 4, 0)

// Where the top screen's actors are. The SDK's FX32_ONE is a long, which keeps the sum it is in from folding
#define OEKAKI_SUB_SURFACE_Y (256 * 0x1000L)

// The brushes are 6 by 6 pixels, in 8 pixel wide rows
#define OEKAKI_BRUSH_SIZE 6
#define OEKAKI_BRUSH_COUNT 3
#define OEKAKI_COLOR_COUNT 8

typedef struct {
    int (*func)(OekakiWork *wk, int seq);
    // Whether the sequence still runs in OEKAKI_PROC_SEQ_END
    BOOL runsAtEnd;
} OekakiSeq;

typedef struct {
    u16 x;
    u16 y;
    u16 sequence;
} OekakiButtonData;

typedef struct {
    u32 messageId;
    u32 type;
} OekakiMenuItem;

BOOL func_ov023_0218456c(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov023_02184674(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov023_021847c8(GameProc *proc, u32 *state, void *param, void *work);
static void func_ov023_0218489c(TCB *tcb, void *data);
static void func_ov023_021848a8(u8 netId, UnionAppMember *member, void *work);
static void func_ov023_021848cc(u8 netId, UnionAppMember *member, void *work);
static void func_ov023_021848d0(void);
static void func_ov023_021848e0(void);
static void func_ov023_021849f8(OekakiWork *wk);
static void func_ov023_02184b04(OekakiWork *wk);
static void func_ov023_02184b34(void);
static void func_ov023_02184b78(OekakiWork *wk, ArcTool *arc);
static void func_ov023_02184c48(OekakiWork *wk, ArcTool *arc);
static void func_ov023_02184d30(OekakiWork *wk);
static void func_ov023_02184ecc(OekakiWork *wk, GameProc *proc);
static void func_ov023_02185004(OekakiWork *wk);
static void func_ov023_0218506c(ClActor *actor, int x, int y);
static void func_ov023_02185080(OekakiTouchInfo *info, int color, int brush);
static void func_ov023_021850a0(OekakiWork *wk);
static void func_ov023_021852b4(ClActor **buttons, int color);
static void func_ov023_021852f4(ClActor *actor, BOOL pressed);
static int func_ov023_0218530c(OekakiWork *wk, int seq);
static void func_ov023_02185354(OekakiWork *wk);
static int func_ov023_02185378(OekakiWork *wk, int seq);
static int func_ov023_021853d0(OekakiWork *wk, int seq);
static int func_ov023_021853f0(OekakiWork *wk, int seq);
static int func_ov023_02185420(OekakiWork *wk, int seq);
static int func_ov023_02185454(OekakiWork *wk, int seq);
static int func_ov023_02185584(OekakiWork *wk, int seq);
static int func_ov023_021855c8(OekakiWork *wk, int seq);
static int func_ov023_021855ec(OekakiWork *wk, int seq);
static int func_ov023_02185628(OekakiWork *wk, int seq);
static int func_ov023_02185670(OekakiWork *wk, int seq);
static int func_ov023_021856ac(OekakiWork *wk, int seq);
static int func_ov023_021856cc(OekakiWork *wk, int seq);
static int func_ov023_021856f4(OekakiWork *wk, int seq);
static int func_ov023_02185730(OekakiWork *wk, int seq);
static int func_ov023_02185764(OekakiWork *wk, int seq);
static void func_ov023_021857f4(OekakiWork *wk, int index, int netId);
static int func_ov023_02185814(OekakiWork *wk, int seq);
static int func_ov023_02185854(OekakiWork *wk, int seq);
static int func_ov023_02185880(OekakiWork *wk, int seq);
static int func_ov023_021858b8(OekakiWork *wk, int seq);
static int func_ov023_02185930(OekakiWork *wk, int seq);
static int func_ov023_02185934(OekakiWork *wk, int seq);
static int func_ov023_02185954(OekakiWork *wk, int seq);
static int func_ov023_02185974(OekakiWork *wk, int seq);
static int func_ov023_021859c4(OekakiWork *wk, int seq);
static int func_ov023_02185a1c(OekakiWork *wk, int seq);
static void func_ov023_02185bbc(BmpWin *window, const u8 *brush, int srcX, int srcY, int width, int height, int x, int y,
                             int brushWidth, int brushHeight);
static void func_ov023_02185c38(BmpWin *window, const u8 *brush, int x, int y, int *oldX, int *oldY, int penDown,
                            int oldSize);
static void func_ov023_02185da8(OekakiTouchInfo *touch, OekakiOldTouch *old);
static void func_ov023_02185de8(BmpWin *window, OekakiTouchInfo *touch, OekakiOldTouch *old, BOOL transfer);
static void func_ov023_02185ea0(OekakiWork *wk);
static void func_ov023_02185f10(u16 *angle);
static int func_ov023_02185f50(OekakiWork *wk);
static void func_ov023_02185f74(BmpWin **windows, u16 color, OekakiWork *wk);
static void func_ov023_021860d0(OekakiWork *wk);
static int func_ov023_02186164(OekakiWork *wk);
BOOL func_ov023_02186170(OekakiWork *wk);
static void func_ov023_021861c4(OekakiWork *wk, int messageId, BOOL stream);
static BOOL func_ov023_021862b0(OekakiWork *wk);
static void func_ov023_021862f4(OekakiWork *wk);
static void func_ov023_02186304(OekakiWork *wk, u8 limit);
static int func_ov023_02186324(OekakiWork *wk);
static void func_ov023_02186444(OekakiWork *wk, int seq);
static void func_ov023_0218644c(OekakiWork *wk, int procSeq);
static void func_ov023_02186480(OekakiWork *wk);
static BOOL func_ov023_021864a8(OekakiWork *wk);
static void func_ov023_021864c4(OekakiTouchInfo *info, OekakiTouchData *touchData, int color, int brush);
static int func_ov023_0218654c(OekakiWork *wk);
static u32 func_ov023_02186558(OekakiWork *wk);
static void func_ov023_02186564(void);
static AppTaskMenu *func_ov023_021865b4(OekakiWork *wk);
static int func_ov023_02186660(OekakiWork *wk);
static void func_ov023_02186694(OekakiWork *wk);
static void func_ov023_021866c4(OekakiWork *wk);
static void func_ov023_02186708(OekakiWork *wk);
static void func_ov023_021867c4(OekakiWork *wk);
static int func_ov023_02186878(OekakiQueue *queue);
static void func_ov023_02186888(OekakiQueue *queue);
static void func_ov023_021868c8(OekakiQueue *queue);
static void func_ov023_02186938(OekakiTouchInfo *touch);
static void func_ov023_02186958(OekakiQueue *queue);
static void func_ov023_021869c0(OekakiTouchInfo *touch, OekakiQueue *queue);
static void func_ov023_02186a04(OekakiTouchInfo *touch, OekakiQueue *queue);

// The sequences, by OEKAKI_SEQ_*
// pokebw2's sSeqTable
static OekakiSeq data_ov023_02186e80[OEKAKI_SEQ_COUNT] = {
    { NULL, TRUE },
    { func_ov023_02185378, FALSE },
    { func_ov023_021853d0, FALSE },
    { func_ov023_021853f0, FALSE },
    { func_ov023_0218530c, FALSE },
    { func_ov023_02185420, TRUE },
    { func_ov023_02185454, TRUE },
    { func_ov023_021855c8, TRUE },
    { func_ov023_02185584, TRUE },
    { func_ov023_021855ec, TRUE },
    { func_ov023_02185628, TRUE },
    { func_ov023_02185670, TRUE },
    { func_ov023_021856ac, TRUE },
    { func_ov023_021856cc, TRUE },
    { func_ov023_021856f4, TRUE },
    { func_ov023_02185730, TRUE },
    { func_ov023_02185764, TRUE },
    { func_ov023_02185814, TRUE },
    { func_ov023_02185854, TRUE },
    { func_ov023_02185880, TRUE },
    { func_ov023_021858b8, TRUE },
    { func_ov023_02185930, TRUE },
    { func_ov023_02185934, TRUE },
    { func_ov023_02185954, TRUE },
    { func_ov023_02185974, TRUE },
    { func_ov023_021859c4, TRUE },
    { func_ov023_02185a1c, TRUE },
};

// pokebw2's OEKAKI_PROC_FUNCTIONS
// pokebw2's Oekaki_Exit
// pokebw2's Oekaki_Main
const GameProcFunctions data_ov023_02186b30 = { func_ov023_0218456c, func_ov023_02184674, func_ov023_021847c8 };

void func_ov023_02184560(OekakiWork *wk, u8 limit) {
    func_ov022_02184148(wk->param->app, limit);
}

BOOL func_ov023_0218456c(GameProc *proc, u32 *state, void *param, void *work) {
    OekakiWork *wk;
    ArcTool *arc;

    switch (*state) {
    case 0:
        GFL_BGSysDisableAllA();
        GFL_BGSysDisableAllB();
        GFL_HeapCreateChild(HEAPID_USER, HEAPID_OEKAKI, 0x50000);
        wk = GFL_ProcInitSubsystem(proc, sizeof(OekakiWork), HEAPID_OEKAKI);
        MI_CpuFill8(wk, 0, sizeof(OekakiWork));
        GFL_BGSysCreate(HEAPID_OEKAKI);
        wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_OEKAKI);
        wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_EVENT_MAPCHANGE, HEAPID_OEKAKI);
        wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_OEKAKI);
        func_ov023_021848d0();
        func_ov023_021848e0();
        GFL_WipeSet(WIPE_MODE_BOTH, 9, 9, WIPE_COLOR_BLACK, 16, 1, HEAPID_OEKAKI);
        wk->param = param;
        arc = GFL_ArcSysCreateFileHandle(OEKAKI_ARCID, HEAPID_OEKAKI);
        func_ov023_02184b78(wk, arc);
        wk->vblankTcb = GFL_VBlankTCBAdd(func_ov023_0218489c, wk, 0);
        func_ov023_021849f8(wk);
        func_ov023_02184c48(wk, arc);
        func_ov023_02184d30(wk);
        func_ov023_02184ecc(wk, proc);
        func_ov023_02186564();
        func_ov023_021841e0(wk);
        func_ov023_021866c4(wk);
        func_0203d34c(TRUE, HEAPID_OEKAKI);
        GFL_ArcToolFree(arc);
        (*state)++;
        break;
    case 1:
        *state = 0;
        return TRUE;
    }
    return FALSE;
}


static BOOL func_ov023_02184674(GameProc *proc, u32 *state, void *param, void *work) {
    OekakiWork *wk = work;

    // The parent forgets the leaving children that are gone
    if (func_02039338() == 0 && wk->leavingBits != 0) {
        wk->leavingBits &= func_ov023_02186558(wk);
    }
    wk->procSeq = *state;
    func_ov023_0218644c(wk, *state);
    switch (*state) {
    case OEKAKI_PROC_SEQ_FADE_IN:
        if (GFL_WipeIsFinished()) {
            func_ov022_021840fc(wk->param->app, func_ov023_021848a8, func_ov023_021848cc, wk);
            if (func_02039338() == 0) {
                func_ov023_02184560(wk, 1);
            }
            *state = OEKAKI_PROC_SEQ_MAIN;
        }
        break;
    case OEKAKI_PROC_SEQ_MAIN:
        if (data_ov023_02186e80[wk->seq].func != NULL) {
            *state = data_ov023_02186e80[wk->seq].func(wk, *state);
        }
        func_ov023_02185f74(wk->nameWindows, OEKAKI_COLOR_TEXT, wk);
        if (func_02039338() == 0) {
            int next = func_ov023_02186324(wk);

            if (*state == OEKAKI_PROC_SEQ_MAIN) {
                *state = next;
            }
        }
        func_ov023_02185f10(&wk->paletteAngle);
        break;
    case OEKAKI_PROC_SEQ_END:
        if (data_ov023_02186e80[wk->seq].func != NULL && data_ov023_02186e80[wk->seq].runsAtEnd) {
            *state = data_ov023_02186e80[wk->seq].func(wk, *state);
        }
        break;
    case OEKAKI_PROC_SEQ_FADE_OUT:
        if (GFL_WipeIsFinished()) {
            func_0203a5e4(OEKAKI_NET_CMD_BASE);
            func_ov022_02184174(wk->param->app);
            *state = OEKAKI_PROC_SEQ_START_WAIT;
        }
        break;
    case OEKAKI_PROC_SEQ_START_WAIT:
        if (func_ov022_0218417c(wk->param->app)) {
            return TRUE;
        }
        break;
    }
    func_ov023_021867c4(wk);
    func_0204a600();
    GFL_TCBExMgrUpdate(wk->tcbManager);
    func_ov023_02186708(wk);
    if (GFL_WipeIsFinished() && func_0200fb24()) {
        return TRUE;
    }
    return FALSE;
}

static BOOL func_ov023_021847c8(GameProc *proc, u32 *state, void *param, void *work) {
    OekakiWork *wk = work;

    switch (*state) {
    case 0:
        GFL_TCBRemove(wk->vblankTcb);
        func_0204a8d4(wk->resources[OEKAKI_RES_SUB].chars);
        func_0204ada4(wk->resources[OEKAKI_RES_SUB].palette);
        func_0204afd8(wk->resources[OEKAKI_RES_SUB].cellAnims);
        func_0204a8d4(wk->resources[OEKAKI_RES_COMMON].chars);
        func_0204ada4(wk->resources[OEKAKI_RES_COMMON].palette);
        func_0204afd8(wk->resources[OEKAKI_RES_COMMON].cellAnims);
        func_0204a8d4(wk->resources[OEKAKI_RES_MAIN].chars);
        func_0204ada4(wk->resources[OEKAKI_RES_MAIN].palette);
        func_0204afd8(wk->resources[OEKAKI_RES_MAIN].cellAnims);
        func_0204b1cc(wk->actorUnit);
        func_0204a5ac();
        if (wk->menu != NULL) {
            func_ov023_02186694(wk);
        }
        func_ov023_02185004(wk);
        func_ov023_02184b34();
        GFL_MsgDataFree(wk->msgData);
        GFL_WordSetSystemFree(wk->wordSet);
        GFL_FontFree(wk->font);
        (*state)++;
        break;
    case 1:
        (*state)++;
        break;
    case 2:
        if (wk->waitAlone) {
            if (func_ov023_02186558(wk) == 1) {
                (*state)++;
            }
        } else {
            (*state)++;
        }
        break;
    case 3:
        func_ov023_02184b04(wk);
        GFL_ProcReleaseSubsystem(proc);
        GFL_HeapDelete(HEAPID_OEKAKI);
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Oekaki_VBlank
static void func_ov023_0218489c(TCB *tcb, void *data) {
    func_0204a648();
    GFL_BGSysUpdate();
}

// pokebw2's Oekaki_OnJoin
static void func_ov023_021848a8(u8 netId, UnionAppMember *member, void *work) {
    OekakiWork *wk = work;

    if (func_02039338() == 0) {
        func_ov023_02186860(wk, OEKAKI_NET_CMD_NEWCOMER, netId);
    }
    func_ov023_021866c4(wk);
}

// pokebw2's Oekaki_OnLeave
static void func_ov023_021848cc(u8 netId, UnionAppMember *member, void *work) {
}

// pokebw2's sOekakiVRAMConfig
static const BGSysVRAMConfig data_ov023_02186acc = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,       GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's Oekaki_SetVRAMBanks
static void func_ov023_021848d0(void) {
    GFL_BGSysSetVRAMBanks(&data_ov023_02186acc);
}

// pokebw2's Oekaki_InitBG
static void func_ov023_021848e0(void) {
    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&config);
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
            GX_BG_CHARBASE_0x10000,
            0x4000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(0);
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
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(1);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd800,
            GX_BG_CHARBASE_0x08000,
            0x4000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
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
            GX_BG_CHARBASE_0x00000,
            0x4000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysClearScr(4);
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
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
    }
    GFL_BGSysClearCharCore(0, 0x20, 0, HEAPID_OEKAKI);
    GFL_BGSysClearCharCore(4, 0x20, 0, HEAPID_OEKAKI);
    GFL_BGSysClearCharCore(1, 0x20, 0, HEAPID_OEKAKI);
    GFL_BGSysClearCharCore(5, 0x20, 0, HEAPID_OEKAKI);
}

// pokebw2's Oekaki_InitWork
static void func_ov023_021849f8(OekakiWork *wk) {
    int i;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        wk->nameBufs[i] = GFL_StrBufCreate(8, HEAPID_OEKAKI);
        wk->allTouch[i].size = 0;
        wk->oldTouch[i].size = 0;
        wk->members[i].member = NULL;
        wk->members[i].old = NULL;
    }
    wk->endBuf = GFL_StrBufCreate(10, HEAPID_OEKAKI);
    wk->msgBuf = GFL_StrBufCreate(80, HEAPID_OEKAKI);
    wk->color = 0;
    wk->brush = 1;
    wk->banFlag = OEKAKI_BAN_NONE;
    wk->menuOpen = FALSE;
    wk->shareNum = 2;
    wk->shareBit = func_ov023_02186558(wk);
    wk->unk9E74 = 0;
    wk->waitAlone = FALSE;
    wk->seq = OEKAKI_SEQ_NONE;
    wk->joining = FALSE;
    wk->joiningBits = 0;
    wk->menu = NULL;
    func_02039338();
    func_ov023_02186444(wk, OEKAKI_SEQ_MAIN);
    GFL_MsgDataLoadStrbuf(wk->msgData, EventMapchange_Text_007, wk->endBuf);
    wk->buffer = GFL_HeapAllocate(HEAPID_OEKAKI, 0x3c00, FALSE, "oekaki.c", 0x31d);
    wk->memberCount = 0;
}

// pokebw2's Oekaki_FreeWork
static void func_ov023_02184b04(OekakiWork *wk) {
    int i;

    GFL_HeapFree(wk->buffer);
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        GFL_StrBufFree(wk->nameBufs[i]);
    }
    GFL_StrBufFree(wk->msgBuf);
    GFL_StrBufFree(wk->endBuf);
}

// pokebw2's Oekaki_FreeBG
static void func_ov023_02184b34(void) {
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

// pokebw2's Oekaki_LoadGraphics
static void func_ov023_02184b78(OekakiWork *wk, ArcTool *arc) {
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, PALTYPE_MAIN_BG, 0, 12 * 0x20, HEAPID_OEKAKI);
    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_SUB_BG, 0, 12 * 0x20, HEAPID_OEKAKI);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 13 * 0x20, 0x20, HEAPID_OEKAKI);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 13 * 0x20, 0x20, HEAPID_OEKAKI);
    GFL_BGSysLoadArcNCGRStatic(arc, 2, 2, 0, 0x2000, FALSE, HEAPID_OEKAKI);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 6, 2, 0, 0x600, FALSE, HEAPID_OEKAKI);
    GFL_BGSysLoadArcNCGRStatic(arc, 4, 5, 0, 0x2000, FALSE, HEAPID_OEKAKI);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 7, 5, 0, 0x600, FALSE, HEAPID_OEKAKI);
    LoadSysMsgBox(0, 1, 10, 0, HEAPID_OEKAKI);
    LoadSysMsgBox(0, 31, 11, 0, HEAPID_OEKAKI);
}

// pokebw2's Oekaki_LoadActorResources
static void func_ov023_02184c48(OekakiWork *wk, ArcTool *arc) {
    ArcTool *commonArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), HEAPID_OEKAKI);

    ClActSys_Create(&data_020a1448, &data_ov023_02186acc, HEAPID_OEKAKI);
    wk->actorUnit = func_0204b100(53, 1, HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_MAIN].chars = func_0204a6c8(arc, 3, FALSE, 0, HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_MAIN].palette = func_0204abf0(arc, 0, 0, 0, HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_MAIN].cellAnims = func_0204af28(arc, 5, 8, HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_COMMON].chars = func_0204a6c8(commonArc, func_02027618(), FALSE, 0, HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_COMMON].palette =
        func_0204ac18(commonArc, func_02027614(), 0, 11 * 0x20, 0, 3, HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_COMMON].cellAnims =
        func_0204af28(commonArc, func_0202761c(0), func_02027620(0), HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_SUB].chars = func_0204a6c8(arc, 3, FALSE, 1, HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_SUB].palette = func_0204abf0(arc, 0, 1, 0, HEAPID_OEKAKI);
    wk->resources[OEKAKI_RES_SUB].cellAnims = func_0204af28(arc, 5, 8, HEAPID_OEKAKI);
    GFL_ArcToolFree(commonArc);
}

// Where the buttons are, and their animations
// pokebw2's sButtonData
static const OekakiButtonData data_ov023_02186bec[OEKAKI_BUTTON_COUNT] = {
#include "asset/oekaki_data_ov023_02186bec.inc"
};

// pokebw2's Oekaki_CreateActors
static void func_ov023_02184d30(OekakiWork *wk) {
    ClActorSetup setup;
    int i;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    setup.priority = 1;
    setup.bgPriority = 1;
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        setup.x = i * 40 + 24;
        setup.y = 64;
        wk->cursors[i] =
            func_0204b294(wk->actorUnit, wk->resources[OEKAKI_RES_MAIN].chars, wk->resources[OEKAKI_RES_MAIN].palette,
                          wk->resources[OEKAKI_RES_MAIN].cellAnims, &setup, 0, HEAPID_OEKAKI);
        func_0204b92c(wk->cursors[i], TRUE);
        func_0204b878(wk->cursors[i], i);
        func_0204b3dc(wk->cursors[i], FALSE);
    }
    for (i = 0; i < OEKAKI_BUTTON_COUNT; i++) {
        setup.x = data_ov023_02186bec[i].x;
        setup.y = data_ov023_02186bec[i].y;
        wk->buttons[i] =
            func_0204b294(wk->actorUnit, wk->resources[OEKAKI_RES_MAIN].chars, wk->resources[OEKAKI_RES_MAIN].palette,
                          wk->resources[OEKAKI_RES_MAIN].cellAnims, &setup, 0, HEAPID_OEKAKI);
        func_0204b92c(wk->buttons[i], TRUE);
        func_0204b878(wk->buttons[i], data_ov023_02186bec[i].sequence);
        if (i >= OEKAKI_BUTTON_END) {
            func_0204b84c(wk->buttons[i], 2);
        }
    }
    func_0204b878(wk->buttons[0], 6);
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        setup.x = 24;
        setup.y = i * 32 + 32 + OEKAKI_SUB_SURFACE_Y;
        wk->nameActors[i] =
            func_0204b294(wk->actorUnit, wk->resources[OEKAKI_RES_SUB].chars, wk->resources[OEKAKI_RES_SUB].palette,
                          wk->resources[OEKAKI_RES_SUB].cellAnims, &setup, 1, HEAPID_OEKAKI);
        func_0204b92c(wk->nameActors[i], TRUE);
        func_0204b878(wk->nameActors[i], i);
        func_0204b800(wk->nameActors[i], 1);
        func_0204b3dc(wk->nameActors[i], FALSE);
    }
    setup.x = 212;
    setup.y = 160;
    wk->endButton =
        func_0204b294(wk->actorUnit, wk->resources[OEKAKI_RES_COMMON].chars, wk->resources[OEKAKI_RES_COMMON].palette,
                      wk->resources[OEKAKI_RES_COMMON].cellAnims, &setup, 0, HEAPID_OEKAKI);
    func_0204b92c(wk->endButton, TRUE);
    func_0204b878(wk->endButton, 1);
    func_0204b3dc(wk->endButton, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's Oekaki_CreateWindows
static void func_ov023_02184ecc(OekakiWork *wk, GameProc *proc) {
    int i;

    BmpWin_InitAllocator(HEAPID_OEKAKI);
    wk->msgWindow = BmpWin_CreateDynamic(0, 2, 1, 27, 4, 13, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWindow), 15);
    PrintWindow_Init(&wk->msgPrint, wk->msgWindow);
    wk->drawWindow = BmpWin_CreateDynamic(1, 1, 2, 30, 15, 0, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->drawWindow), 2);
    wk->tcbManager = GFL_TCBExMgrCreate(HEAPID_OEKAKI, HEAPID_OEKAKI, 32, 32);
    wk->nameQueue = func_0201c440(HEAPID_OEKAKI);
    wk->printQueue = func_0201c440(HEAPID_OEKAKI);
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        wk->nameWindows[i] = BmpWin_CreateDynamic(4, 5, i * 4 + 3, 10, 2, 13, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->nameWindows[i]), 0);
        PrintWindow_Init(&wk->namePrint[i], wk->nameWindows[i]);
        BmpWin_TransferNow(wk->nameWindows[i]);
    }
    func_ov023_02185f74(wk->nameWindows, OEKAKI_COLOR_TEXT, wk);
}

// pokebw2's Oekaki_FreeWindows
static void func_ov023_02185004(OekakiWork *wk) {
    int i;

    GFL_TCBExMgrFreeTasks(wk->tcbManager);
    GFL_TCBExMgrFree(wk->tcbManager);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    func_0201c6ec(wk->nameQueue);
    func_0201c4c0(wk->nameQueue);
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        BmpWin_Free(wk->nameWindows[i]);
    }
    BmpWin_Free(wk->drawWindow);
    BmpWin_Free(wk->msgWindow);
    BmpWin_FreeAllocator();
}

// Puts a pen's tip at a point
// pokebw2's Oekaki_SetCursorPos
static void func_ov023_0218506c(ClActor *actor, int x, int y) {
    ClActorPos pos;

    pos.x = x;
    pos.y = y - 8;
    func_0204b528(actor, &pos);
}

// The touch of a frame in which the pen is up
// pokebw2's Oekaki_MakeEmptyTouch
static void func_ov023_02185080(OekakiTouchInfo *info, int color, int brush) {
    OekakiTouchData touchData;

    touchData.size = 0;
    touchData.points[0].x = 0;
    touchData.points[0].y = 0;
    func_ov023_021864c4(info, &touchData, color, brush);
}

// The picture
// pokebw2's sDrawAreaRects
static const TouchRect data_ov023_02186ab4[] = {
    { 16, 136, 8, 248 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// The buttons, in the order of OEKAKI_BUTTON_*
// pokebw2's sOekakiButtonRects
static const TouchRect data_ov023_02186afc[] = {
#include "asset/oekaki_data_ov023_02186afc.inc"
};

// Handles the buttons, and queues what the player draws this frame
// pokebw2's Oekaki_UpdateTouch
static void func_ov023_021850a0(OekakiWork *wk) {
    OekakiTouchData touchData;
    u32 cursorX;
    u32 cursorY;
    OekakiTouchInfo info;
    u32 x;
    u32 y;
    BOOL endPressed = FALSE;
    int button;
    int drawArea;

    button = func_02035c08(data_ov023_02186afc);
    if (button != TOUCH_RECT_NONE) {
        switch (button) {
        case OEKAKI_BUTTON_COLOR:
        case OEKAKI_BUTTON_COLOR + 1:
        case OEKAKI_BUTTON_COLOR + 2:
        case OEKAKI_BUTTON_COLOR + 3:
        case OEKAKI_BUTTON_COLOR + 4:
        case OEKAKI_BUTTON_COLOR + 5:
        case OEKAKI_BUTTON_COLOR + 6:
        case OEKAKI_BUTTON_COLOR + 7:
            if (wk->color != button) {
                wk->color = button;
                func_ov023_021852b4(wk->buttons, button);
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            break;
        case OEKAKI_BUTTON_END:
            if (wk->seq == OEKAKI_SEQ_MAIN) {
                if (func_02039338() == 0) {
                    // Not while a member that joined has yet to get the picture
                    if (wk->shareBit != func_ov023_02186558(wk)) {
                        GFL_SndSEPlay(SEQ_SE_SELECT4);
                    } else {
                        wk->banFlag = OEKAKI_BAN_ON;
                        func_ov023_021861c4(wk, EventMapchange_Text_001, TRUE);
                        endPressed = TRUE;
                        func_ov023_02186444(wk, OEKAKI_SEQ_END_SELECT_MSG_WAIT);
                        func_ov023_021852f4(wk->endButton, TRUE);
                        GFL_SndSEPlay(SEQ_SE_CANCEL1);
                    }
                } else {
                    // Nor while the parent has its menu open
                    if (wk->allTouch[0].banFlag == OEKAKI_BAN_ON) {
                        GFL_SndSEPlay(SEQ_SE_SELECT4);
                    } else {
                        func_ov023_021861c4(wk, EventMapchange_Text_001, TRUE);
                        endPressed = TRUE;
                        func_ov023_02186444(wk, OEKAKI_SEQ_END_SELECT_MSG_WAIT);
                        func_ov023_021852f4(wk->endButton, TRUE);
                        GFL_SndSEPlay(SEQ_SE_CANCEL1);
                    }
                }
            }
            break;
        case OEKAKI_BUTTON_BRUSH:
        case OEKAKI_BUTTON_BRUSH + 1:
        case OEKAKI_BUTTON_BRUSH + 2: {
            int i;

            for (i = 0; i < OEKAKI_BRUSH_COUNT; i++) {
                if (button - OEKAKI_BUTTON_BRUSH == i) {
                    func_0204b878(wk->buttons[OEKAKI_BUTTON_BRUSH + i], i * 2 + 30);
                } else {
                    func_0204b878(wk->buttons[OEKAKI_BUTTON_BRUSH + i], i * 2 + 29);
                }
            }
            if (wk->brush != button - OEKAKI_BUTTON_BRUSH) {
                wk->brush = button - OEKAKI_BUTTON_BRUSH;
                GFL_SndSEPlay(SEQ_SE_SELECT1);
            }
            break;
        }
        }
    }

    // The player's own pen
    drawArea = func_02035ba4(data_ov023_02186ab4);
    func_0204b3dc(wk->cursors[func_02039338()], FALSE);
    if (drawArea != TOUCH_RECT_NONE) {
        func_02035cb8(&cursorX, &cursorY);
        func_ov023_0218506c(wk->cursors[func_02039338()], cursorX, cursorY);
        func_0204b3dc(wk->cursors[func_02039338()], TRUE);
    }

    x = 0;
    y = 0;
    if (func_02035c38() == TRUE) {
        // A line from the point of the frame before
        func_02035cb8(&x, &y);
        touchData.size = 2;
        touchData.points[0].x = wk->touchData.points[1].x;
        touchData.points[0].y = wk->touchData.points[1].y;
        touchData.points[1].x = x;
        touchData.points[1].y = y;
        func_ov023_021864c4(&info, &touchData, wk->color, wk->brush);
        if (endPressed == TRUE) {
            info.size = 0;
        }
    } else {
        touchData.points[1].x = 0;
        touchData.points[1].y = 0;
        func_ov023_02185080(&info, wk->color, wk->brush);
    }
    func_ov023_02186988(&info, &wk->queue, 0);
    if (func_02039338() == 0) {
        func_ov023_021868c8(&wk->queue);
    } else {
        func_ov023_02186888(&wk->queue);
    }
    wk->touchData = touchData;
}

// Shows the picked color's button as pressed
// pokebw2's Oekaki_SetColorButtons
static void func_ov023_021852b4(ClActor **buttons, int color) {
    int i;

    for (i = 0; i < OEKAKI_COLOR_COUNT; i++) {
        if (i == color) {
            func_0204b878(buttons[i], data_ov023_02186bec[i].sequence + 1);
        } else {
            func_0204b878(buttons[i], data_ov023_02186bec[i].sequence);
        }
    }
}

// pokebw2's Oekaki_SetEndButton
static void func_ov023_021852f4(ClActor *actor, BOOL pressed) {
    if (pressed == TRUE) {
        func_0204b878(actor, 9);
    } else {
        func_0204b878(actor, 1);
    }
}

// pokebw2's Oekaki_SeqMain
static int func_ov023_0218530c(OekakiWork *wk, int seq) {
    func_ov023_021850a0(wk);
    if (func_02039338() == 0) {
        // A parent with no children has nobody to send to
        if (func_ov023_02185f50(wk) != 1) {
            func_ov023_021860d0(wk);
        }
    } else {
        func_ov023_021860d0(wk);
    }
    func_ov023_02185ea0(wk);
    func_ov023_02185de8(wk->drawWindow, wk->allTouch, wk->oldTouch, TRUE);
    return seq;
}

// Draws what has arrived while the player cannot draw
// pokebw2's Oekaki_DrawOthers
static void func_ov023_02185354(OekakiWork *wk) {
    func_ov023_02185ea0(wk);
    func_ov023_02185de8(wk->drawWindow, wk->allTouch, wk->oldTouch, FALSE);
}

// pokebw2's Oekaki_SeqNewcomer
static int func_ov023_02185378(OekakiWork *wk, int seq) {
    if (wk->newcomerId == func_02039338()) {
        func_ov023_021861c4(wk, EventMapchange_Text_013, FALSE);
    } else {
        func_ov023_021861c4(wk, EventMapchange_Text_000, FALSE);
    }
    func_ov023_02186444(wk, OEKAKI_SEQ_NEWCOMER_WAIT);
    func_ov023_02186480(wk);
    GFL_SndSEPlay(SEQ_SE_DECIDE1);
    G2_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3, -6);
    func_ov023_02185354(wk);
    return seq;
}

// Waits for OEKAKI_NET_CMD_PICTURE_END
// pokebw2's Oekaki_SeqNewcomerWait
static int func_ov023_021853d0(OekakiWork *wk, int seq) {
    int i;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        func_0204b3dc(wk->cursors[i], FALSE);
    }
    return seq;
}

// pokebw2's Oekaki_SeqNewcomerEnd
static int func_ov023_021853f0(OekakiWork *wk, int seq) {
    reg_G2_BLDCNT = 0;
    func_ov023_02186304(wk, 1);
    func_ov023_02186444(wk, OEKAKI_SEQ_MAIN);
    func_ov023_02185354(wk);
    wk->joining = FALSE;
    return seq;
}

// pokebw2's Oekaki_SeqEndSelectMsgWait
static int func_ov023_02185420(OekakiWork *wk, int seq) {
    if (func_ov023_021862b0(wk)) {
        if (wk->menu == NULL) {
            wk->menu = func_ov023_021865b4(wk);
        }
        func_ov023_02186444(wk, OEKAKI_SEQ_END_SELECT);
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqEndSelect
static int func_ov023_02185454(OekakiWork *wk, int seq) {
    wk->myTouch.size = 0;
    // The parent has its own menu open
    if (wk->allTouch[0].banFlag == OEKAKI_BAN_ON && func_02039338() != 0) {
        func_ov023_02185354(wk);
        func_020278f4(wk->menu);
        if (func_ov023_021864a8(wk)) {
            GFL_SndSEPlay(SEQ_SE_SELECT4);
        }
        return seq;
    }
    if (func_02039338() == 0 && wk->leavingBits != 0) {
        func_ov023_02185354(wk);
        func_020278f4(wk->menu);
        return seq;
    }
    switch (func_ov023_02186660(wk)) {
    case 0:
        if (func_02039338() == 0) {
            func_ov023_02186444(wk, OEKAKI_SEQ_PARENT_END_SELECT_MSG_WAIT);
            func_ov023_021861c4(wk, EventMapchange_Text_004, TRUE);
        } else {
            func_ov023_02186444(wk, OEKAKI_SEQ_END_REQUEST);
            func_0201f7a8(wk->msgWindow, 2);
        }
        BmpWin_TransferNow(wk->drawWindow);
        break;
    case 1:
        func_ov023_02186444(wk, OEKAKI_SEQ_MAIN);
        func_ov023_021852f4(wk->endButton, FALSE);
        func_0201f7a8(wk->msgWindow, 2);
        BmpWin_TransferNow(wk->drawWindow);
        if (func_02039338() == 0) {
            func_ov023_02184560(wk, 1);
            wk->banFlag = OEKAKI_BAN_NONE;
        }
        break;
    }
    func_ov023_02185354(wk);
    return seq;
}

// Asks the parent to leave
// pokebw2's Oekaki_SeqEndRequest
static int func_ov023_02185584(OekakiWork *wk, int seq) {
    OekakiSignal signal;

    MI_CpuFill8(&signal, 0, sizeof(signal));
    signal.type = OEKAKI_SIGNAL_LEAVE_REQUEST;
    signal.netId = func_02039338();
    if (func_0203d3a8(func_020399a8(), OEKAKI_NET_CMD_SIGNAL, sizeof(signal), &signal)) {
        func_ov023_02186444(wk, OEKAKI_SEQ_END_ANSWER_WAIT);
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqEndAnswerWait
static int func_ov023_021855c8(OekakiWork *wk, int seq) {
    wk->myTouch.size = 0;
    wk->signalWait = 0;
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqEndOk
static int func_ov023_021855ec(OekakiWork *wk, int seq) {
    wk->myTouch.size = 0;
    wk->signalWait++;
    if (wk->signalWait > 30) {
        func_ov023_02186444(wk, OEKAKI_SEQ_END_LEAVE);
        wk->signalWait = 0;
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqEndLeave
static int func_ov023_02185628(OekakiWork *wk, int seq) {
    OekakiSignal signal;

    MI_CpuFill8(&signal, 0, sizeof(signal));
    signal.type = OEKAKI_SIGNAL_LEAVE;
    signal.netId = func_02039338();
    if (func_0203d3a8(func_020399a8(), OEKAKI_NET_CMD_SIGNAL, sizeof(signal), &signal)) {
        func_ov023_02186444(wk, OEKAKI_SEQ_END_MSG);
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqEndRefused
static int func_ov023_02185670(OekakiWork *wk, int seq) {
    wk->myTouch.size = 0;
    wk->leaving = FALSE;
    func_ov023_02186444(wk, OEKAKI_SEQ_MAIN);
    func_ov023_021852f4(wk->endButton, FALSE);
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqEndMsg
static int func_ov023_021856ac(OekakiWork *wk, int seq) {
    func_ov023_021861c4(wk, EventMapchange_Text_006, TRUE);
    func_ov023_02186444(wk, OEKAKI_SEQ_END_MSG_WAIT);
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqEndMsgWait
static int func_ov023_021856cc(OekakiWork *wk, int seq) {
    if (func_ov023_021862b0(wk)) {
        wk->wait = 0;
        func_ov023_02186444(wk, OEKAKI_SEQ_END_WAIT);
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqEndWait
static int func_ov023_021856f4(OekakiWork *wk, int seq) {
    wk->wait++;
    if (wk->wait > 60) {
        GFL_WipeSet(WIPE_MODE_BOTH, 8, 8, WIPE_COLOR_BLACK, 16, 1, HEAPID_OEKAKI);
        seq = OEKAKI_PROC_SEQ_FADE_OUT;
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqParentEndSelectMsgWait
static int func_ov023_02185730(OekakiWork *wk, int seq) {
    if (func_ov023_021862b0(wk)) {
        if (wk->menu == NULL) {
            wk->menu = func_ov023_021865b4(wk);
        }
        func_ov023_02186444(wk, OEKAKI_SEQ_PARENT_END_SELECT);
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqParentEndSelect
static int func_ov023_02185764(OekakiWork *wk, int seq) {
    int result = func_ov023_02186660(wk);

    if (wk->shareNum != func_ov023_02186164(wk) || wk->leavingBits != 0) {
        func_ov023_02185354(wk);
        return seq;
    }
    switch (result) {
    case 0:
        func_ov023_02186444(wk, OEKAKI_SEQ_PARENT_END_SEND);
        func_ov022_02184114(wk->param->app);
        break;
    case 1:
        func_ov023_02186444(wk, OEKAKI_SEQ_MAIN);
        func_ov023_021852f4(wk->endButton, FALSE);
        func_0201f7a8(wk->msgWindow, 2);
        if (func_02039338() == 0) {
            func_ov023_02184560(wk, 1);
            wk->banFlag = OEKAKI_BAN_NONE;
        }
        break;
    }
    func_ov023_02185354(wk);
    return seq;
}

// Puts a member's name in a slot of the messages
// pokebw2's Oekaki_SetPlayerName
static void func_ov023_021857f4(OekakiWork *wk, int index, int netId) {
    UnionAppMember *member = func_ov022_02184180(wk->param->app, netId);

    if (member != NULL) {
        copyVarForText(wk->wordSet, index, &member->info);
    }
}

// pokebw2's Oekaki_SeqParentEndSend
static int func_ov023_02185814(OekakiWork *wk, int seq) {
    if (func_0203d3a8(func_0203996c(GFL_NET_NETID_SERVER), OEKAKI_NET_CMD_PARENT_END, 0, NULL)) {
        func_ov023_02186444(wk, OEKAKI_SEQ_PARENT_END_MSG);
        func_ov023_021857f4(wk, 0, 0);
        seq = OEKAKI_PROC_SEQ_END;
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqParentEndMsg
static int func_ov023_02185854(OekakiWork *wk, int seq) {
    func_ov023_021857f4(wk, 0, 0);
    func_ov023_021861c4(wk, EventMapchange_Text_003, TRUE);
    func_ov023_02186444(wk, OEKAKI_SEQ_PARENT_END_MSG_WAIT);
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqParentEndMsgWait
static int func_ov023_02185880(OekakiWork *wk, int seq) {
    if (func_ov023_021862b0(wk)) {
        func_ov023_02186444(wk, OEKAKI_SEQ_PARENT_END_SYNC_WAIT);
        func_0203a424(func_020399a8(), 200, 0x29, (u16)wk->shareBit);
    }
    func_ov023_02185354(wk);
    return seq;
}

// Waits until every member is at the same point, or gone
// pokebw2's Oekaki_SeqParentEndSyncWait
static int func_ov023_021858b8(OekakiWork *wk, int seq) {
    NetHandle *handle = func_020399a8();
    int i;
    u32 bits = 0;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        if (func_0203d1c0((u8)i) == TRUE) {
            bits |= 1 << i;
        }
    }
    if (func_02039cb4(handle, 200, 0x29) || func_ov023_0218654c(wk) == 1 || bits != wk->shareBit) {
        GFL_WipeSet(WIPE_MODE_BOTH, 8, 8, WIPE_COLOR_BLACK, 16, 1, HEAPID_OEKAKI);
        seq = OEKAKI_PROC_SEQ_FADE_OUT;
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_Seq21
static int func_ov023_02185930(OekakiWork *wk, int seq) {
    return seq;
}

// pokebw2's Oekaki_SeqNoMembersMsg
static int func_ov023_02185934(OekakiWork *wk, int seq) {
    func_ov023_021861c4(wk, EventMapchange_Text_005, TRUE);
    func_ov023_02186444(wk, OEKAKI_SEQ_NO_MEMBERS_MSG_WAIT);
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqNoMembersMsgWait
static int func_ov023_02185954(OekakiWork *wk, int seq) {
    if (func_ov023_021862b0(wk)) {
        func_ov023_02186444(wk, OEKAKI_SEQ_END_WAIT);
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqMemberLeft
static int func_ov023_02185974(OekakiWork *wk, int seq) {
    func_ov023_021861c4(wk, EventMapchange_Text_002, TRUE);
    func_ov023_02186480(wk);
    func_ov023_02186444(wk, OEKAKI_SEQ_MEMBER_LEFT_MSG_WAIT);
    GFL_SndSEPlay(SEQ_SE_DECIDE1);
    if (func_02039338() == 0) {
        func_ov023_02186304(wk, 0);
    }
    wk->leftCount = func_ov023_0218654c(wk);
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqMemberLeftMsgWait
static int func_ov023_021859c4(OekakiWork *wk, int seq) {
    if ((wk->leftCount != 0 && wk->leftCount != func_ov023_0218654c(wk)) || !(func_ov023_02186558(wk) & wk->leavingBits)) {
        wk->leftCount = 0;
    }
    if (func_ov023_021862b0(wk)) {
        func_ov023_02186444(wk, OEKAKI_SEQ_MEMBER_LEFT_WAIT);
        wk->wait = 0;
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SeqMemberLeftWait
static int func_ov023_02185a1c(OekakiWork *wk, int seq) {
    if ((wk->leftCount != 0 && wk->leftCount != func_ov023_0218654c(wk)) || !(func_ov023_02186558(wk) & wk->leavingBits)) {
        wk->leftCount = 0;
    }
    wk->wait++;
    if (wk->wait > 60 && wk->leftCount == 0) {
        func_ov023_021862f4(wk);
        func_ov023_02186444(wk, OEKAKI_SEQ_MAIN);
        if (func_02039338() == 0) {
            wk->banFlag = OEKAKI_BAN_NONE;
            func_ov023_02186304(wk, 1);
        }
        wk->shareNum = func_ov022_021841a4(wk->param->app);
        wk->shareBit = func_ov022_021841a0(wk->param->app);
    }
    func_ov023_02185354(wk);
    return seq;
}

// pokebw2's Oekaki_SetStatus
void func_ov023_02185ab0(OekakiWork *wk, int seq, int netId) {
    switch (seq) {
    case OEKAKI_SEQ_NEWCOMER:
        if (wk->leaving == TRUE) {
            return;
        }
        if (wk->seq == OEKAKI_SEQ_END_SELECT || wk->seq == OEKAKI_SEQ_PARENT_END_SELECT) {
            func_ov023_02186480(wk);
        }
        func_ov023_021852f4(wk->endButton, FALSE);
        func_ov023_021857f4(wk, 0, netId);
        wk->newcomerId = netId;
        wk->leavingBits = 0;
        break;
    case OEKAKI_SEQ_NEWCOMER_END:
        func_ov023_021862f4(wk);
        break;
    case OEKAKI_SEQ_MEMBER_LEFT:
        if (wk->leaving == TRUE) {
            return;
        }
        func_ov023_021857f4(wk, 0, netId);
        if (netId == func_02039338()) {
            return;
        }
        if (func_02039338() == 0) {
            wk->leavingBits &= 0xffff ^ netId;
        }
        if (wk->seq == OEKAKI_SEQ_END_SELECT || wk->seq == OEKAKI_SEQ_PARENT_END_SELECT) {
            func_ov023_02186480(wk);
            BmpWin_TransferNow(wk->drawWindow);
        }
        func_ov023_021852f4(wk->endButton, FALSE);
        break;
    case OEKAKI_SEQ_END_OK:
        func_ov023_02186860(wk, OEKAKI_NET_CMD_LEAVING, func_02039338());
        break;
    case OEKAKI_SEQ_END_REFUSED:
    case OEKAKI_SEQ_PARENT_END_MSG:
        break;
    default:
        return;
    }
    func_ov023_02186444(wk, seq);
}

#define C(color, pixels) ((color) * (pixels))
// The brushes, as rows of 4 bits per pixel. C is the color in each pixel that is set
// clang-format off
#define BRUSH_SMALL(color) { \
    0x00,           0x00,           0x00,           0x00, \
    0x00,           0x00,           0x00,           0x00, \
    0x00,           C(color, 0x11), 0x00,           0x00, \
    0x00,           C(color, 0x11), 0x00,           0x00, \
    0x00,           0x00,           0x00,           0x00, \
    0x00,           0x00,           0x00,           0x00, \
}
#define BRUSH_MEDIUM(color) { \
    0x00,           0x00,           0x00,           0x00, \
    0x00,           C(color, 0x11), 0x00,           0x00, \
    C(color, 0x10), C(color, 0x11), C(color, 0x01), 0x00, \
    C(color, 0x10), C(color, 0x11), C(color, 0x01), 0x00, \
    0x00,           C(color, 0x11), 0x00,           0x00, \
    0x00,           0x00,           0x00,           0x00, \
}
#define BRUSH_LARGE(color) { \
    C(color, 0x10), C(color, 0x11), C(color, 0x01), 0x00, \
    C(color, 0x11), C(color, 0x11), C(color, 0x11), 0x00, \
    C(color, 0x11), C(color, 0x11), C(color, 0x11), 0x00, \
    C(color, 0x11), C(color, 0x11), C(color, 0x11), 0x00, \
    C(color, 0x11), C(color, 0x11), C(color, 0x11), 0x00, \
    C(color, 0x10), C(color, 0x11), C(color, 0x01), 0x00, \
}
// clang-format on

// The pixels of each brush in each color
// pokebw2's sBrushPatterns
static const u8 data_ov023_02186c34[OEKAKI_BRUSH_COUNT][OEKAKI_COLOR_COUNT][OEKAKI_BRUSH_SIZE * 4] = {
    {
        BRUSH_SMALL(1),
        BRUSH_SMALL(2),
        BRUSH_SMALL(3),
        BRUSH_SMALL(4),
        BRUSH_SMALL(5),
        BRUSH_SMALL(6),
        BRUSH_SMALL(7),
        BRUSH_SMALL(8),
    },
    {
        BRUSH_MEDIUM(1),
        BRUSH_MEDIUM(2),
        BRUSH_MEDIUM(3),
        BRUSH_MEDIUM(4),
        BRUSH_MEDIUM(5),
        BRUSH_MEDIUM(6),
        BRUSH_MEDIUM(7),
        BRUSH_MEDIUM(8),
    },
    {
        BRUSH_LARGE(1),
        BRUSH_LARGE(2),
        BRUSH_LARGE(3),
        BRUSH_LARGE(4),
        BRUSH_LARGE(5),
        BRUSH_LARGE(6),
        BRUSH_LARGE(7),
        BRUSH_LARGE(8),
    },
};

// Draws a brush into the picture, clipped at the left and the top
static void func_ov023_02185bbc(BmpWin *window, const u8 *brush, int srcX, int srcY, int width, int height, int x, int y,
                             int brushWidth, int brushHeight) {
    GFLBitmap *bitmap;

    if (x < 0) {
        int clip = -x;

        if (clip > brushWidth) {
            clip = brushWidth;
        }
        srcX += clip;
        x = 0;
        width -= clip;
    }
    if (y < 0) {
        int clip = -y;

        if (clip > brushHeight) {
            clip = brushHeight;
        }
        srcY += clip;
        y = 0;
        height -= clip;
    }
    bitmap = GFL_BitmapWrap((void *)brush, 8, 8, 0x20, HEAPID_OEKAKI);
    GFL_BitmapCopyArea(bitmap, BmpWin_GetBitmap(window), srcX, srcY, x, y, width, height, 0);
    GFL_BitmapFree(bitmap);
}

// Draws a line with a brush from *oldX, *oldY to x, y, and makes that the old point
static void func_ov023_02185c38(BmpWin *window, const u8 *brush, int x, int y, int *oldX, int *oldY, int penDown,
                            int oldSize) {
    int x0 = *oldX;
    int y0 = *oldY;
    int xEnd = x;
    int dx;
    int yEnd = y;
    int dy;
    int step;
    int error;

    if (!penDown && !oldSize) {
        *oldX = x;
        *oldY = y;
        return;
    }
    dx = x - x0;
    if (dx < 0) {
        dx = -dx;
    }
    dy = y - y0;
    if (dy < 0) {
        dy = -dy;
    }
    if (dx > dy) {
        if (x0 > x) {
            if (y0 > y) {
                step = 1;
            } else {
                step = -1;
            }
            xEnd = x0;
            x0 = x;
            y0 = y;
        } else {
            if (y0 < y) {
                step = 1;
            } else {
                step = -1;
            }
        }
        func_ov023_02185bbc(window, brush, 0, 0, OEKAKI_BRUSH_SIZE, OEKAKI_BRUSH_SIZE, x0, y0, OEKAKI_BRUSH_SIZE,
                         OEKAKI_BRUSH_SIZE);
        error = dx >> 1;
        for (x0++; x0 <= xEnd; x0++) {
            error -= dy;
            if (error < 0) {
                error += dx;
                y0 += step;
            }
            func_ov023_02185bbc(window, brush, 0, 0, OEKAKI_BRUSH_SIZE, OEKAKI_BRUSH_SIZE, x0, y0, OEKAKI_BRUSH_SIZE,
                             OEKAKI_BRUSH_SIZE);
        }
    } else {
        if (y0 > y) {
            if (x0 > x) {
                step = 1;
            } else {
                step = -1;
            }
            yEnd = y0;
            y0 = y;
            x0 = x;
        } else {
            if (x0 < x) {
                step = 1;
            } else {
                step = -1;
            }
        }
        func_ov023_02185bbc(window, brush, 0, 0, OEKAKI_BRUSH_SIZE, OEKAKI_BRUSH_SIZE, x0, y0, OEKAKI_BRUSH_SIZE,
                         OEKAKI_BRUSH_SIZE);
        error = dy >> 1;
        for (y0++; y0 <= yEnd; y0++) {
            error -= dx;
            if (error < 0) {
                error += dy;
                x0 += step;
            }
            func_ov023_02185bbc(window, brush, 0, 0, OEKAKI_BRUSH_SIZE, OEKAKI_BRUSH_SIZE, x0, y0, OEKAKI_BRUSH_SIZE,
                             OEKAKI_BRUSH_SIZE);
        }
    }
    *oldX = x;
    *oldY = y;
}

// Keeps where each member's line ended
// pokebw2's Oekaki_SaveOldTouch
static void func_ov023_02185da8(OekakiTouchInfo *touch, OekakiOldTouch *old) {
    int i;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        old[i].size = touch[i].size;
        if (touch[i].size != 0) {
            old[i].x = touch[i].x[touch[i].size - 1];
            old[i].y = touch[i].y[touch[i].size - 1];
        }
    }
}

// Draws every member's line of the frame, and clears the touches
// pokebw2's Oekaki_DrawTouches
static void func_ov023_02185de8(BmpWin *window, OekakiTouchInfo *touch, OekakiOldTouch *old, BOOL transfer) {
    int i;
    BOOL drawn = FALSE;
    int x;
    int y;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        if (touch[i].size == 2) {
            x = touch[i].x[0] - 9;
            y = touch[i].y[0] - 17;
            func_ov023_02185c38(window, data_ov023_02186c34[touch[i].brush][touch[i].color], touch[i].x[1] - 9,
                            touch[i].y[1] - 17, &x, &y, TRUE, old[i].size);
            drawn = TRUE;
        }
    }
    if (drawn && transfer) {
        BmpWin_TransferNow(window);
    }
    func_ov023_02185da8(touch, old);
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        touch[i].size = 0;
    }
}

// Shows the other members' pens where they draw
// pokebw2's Oekaki_UpdateCursors
static void func_ov023_02185ea0(OekakiWork *wk) {
    OekakiTouchInfo *touch = wk->allTouch;
    int i;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        if (i != func_02039338()) {
            if (touch[i].size != 0 && func_ov022_02184180(wk->param->app, i) != NULL) {
                func_0204b3dc(wk->cursors[i], TRUE);
                func_ov023_0218506c(wk->cursors[i], touch[i].x[touch[i].size - 1], touch[i].y[touch[i].size - 1]);
            } else {
                func_0204b3dc(wk->cursors[i], FALSE);
            }
        }
    }
}

// Pulses the green of a color of the actors' palette
// pokebw2's Oekaki_UpdatePalette
static void func_ov023_02185f10(u16 *angle) {
    u16 color;

    *angle += 20;
    if (*angle > 360) {
        *angle = 0;
    }
    color = GX_RGB(29, func_0203f684(*angle) * 10 / FX16_ONE + 15, 0);
    GX_LoadOBJPltt(&color, 12 * sizeof(u16), sizeof(u16));
}

// pokebw2's Oekaki_CountMembers
static int func_ov023_02185f50(OekakiWork *wk) {
    int i;
    int count = 0;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        if (func_ov022_02184180(wk->param->app, i) != NULL) {
            count++;
        }
    }
    return count;
}

static void func_ov023_02185f74(BmpWin **windows, u16 color, OekakiWork *wk) {
    int myId = func_02039338();
    int i;

    if (!func_ov023_02186170(wk)) {
        return;
    }
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        if (wk->members[i].member == NULL) {
            func_0204b3dc(wk->nameActors[i], FALSE);
        } else {
            func_0204b3dc(wk->nameActors[i], TRUE);
        }
    }
    func_0201c6ec(wk->nameQueue);
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        GFL_BitmapFillArea(BmpWin_GetBitmap(windows[i]), 0, 0, 80, 16, 0);
    }
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        if (wk->members[i].member != NULL) {
            textCopy(wk->members[i].member->info.name, wk->nameBufs[i]);
            if (myId == i) {
                PrintWindow_PrintColor(&wk->namePrint[i], wk->nameQueue, 0, 0, wk->nameBufs[i], wk->font,
                                       OEKAKI_COLOR_NAME_MINE);
            } else {
                PrintWindow_PrintColor(&wk->namePrint[i], wk->nameQueue, 0, 0, wk->nameBufs[i], wk->font, color);
            }
        }
        BmpWin_TransferNow(windows[i]);
    }
}


// Sends the oldest frame of the queue: the parent every member's touch, a child its own
// pokebw2's Oekaki_SendTouch
static void func_ov023_021860d0(OekakiWork *wk) {
    NetHandle *handle;

    if (func_02039338() == 0) {
        handle = func_020399a8();
        if (func_0203d56c(handle)) {
            func_ov023_02186a04(wk->sendTouch, &wk->queue);
            wk->sendTouch[0].banFlag = wk->banFlag;
            if (func_0203d3a8(handle, OEKAKI_NET_CMD_ALL_TOUCH, sizeof(wk->sendTouch), wk->sendTouch) == TRUE) {
                func_ov023_02186958(&wk->queue);
            }
        }
    } else {
        handle = func_020399a8();
        func_ov023_021869c0(&wk->myTouch, &wk->queue);
        if (func_0203d56c(handle)) {
            if (func_0203d3a8(func_020399a8(), OEKAKI_NET_CMD_TOUCH, sizeof(wk->myTouch), &wk->myTouch) == TRUE) {
                func_ov023_02186958(&wk->queue);
            }
        }
    }
}

// pokebw2's Oekaki_GetMemberCount
static int func_ov023_02186164(OekakiWork *wk) {
    return func_ov022_021841a4(wk->param->app);
}

// Reads the members, and returns whether any has changed
// pokebw2's Oekaki_UpdateMembers
BOOL func_ov023_02186170(OekakiWork *wk) {
    BOOL changed = FALSE;
    int i;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        wk->members[i].old = wk->members[i].member;
        wk->members[i].member = func_ov022_02184180(wk->param->app, i);
    }
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        if (wk->members[i].old != wk->members[i].member) {
            changed = TRUE;
        }
    }
    return changed;
}

static void func_ov023_021861c4(OekakiWork *wk, int messageId, BOOL stream) {
    StrBuf *format = GFL_StrBufCreate(0x50, HEAPID_OEKAKI);

    GFL_MsgDataLoadStrbuf(wk->msgData, messageId, format);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->msgBuf, format);
    GFL_StrBufFree(format);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWindow), 15);
    BmpWin_DrawFrame(wk->msgWindow, FALSE, 1, 10);
    if (wk->stream != NULL) {
        func_0201ce48(wk->stream);
        wk->stream = NULL;
    }
    if (!stream) {
        PrintQueue *queue = wk->printQueue;
        func_0201c724(queue, BmpWin_GetBitmap(wk->msgPrint.window), 0, 0, wk->msgBuf, wk->font, OEKAKI_COLOR_TEXT);
        wk->msgPrint.flushPending = TRUE;
    } else {
        wk->stream = func_0201cce4(wk->msgWindow, 0, 0, wk->msgBuf, wk->font, func_02012ffc(), wk->tcbManager, TRUE, HEAPID_OEKAKI, 0xf0f);
    }
    BmpWin_TransferNow(wk->msgWindow);
}


// pokebw2's Oekaki_IsMessageDone
static BOOL func_ov023_021862b0(OekakiWork *wk) {
    u32 state = func_0201ce30(wk->stream);

    if (state == PRINT_STREAM_PAUSED) {
        if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) {
            func_0201ce38(wk->stream);
        }
        return FALSE;
    }
    if (state == PRINT_STREAM_DONE) {
        func_0201ce48(wk->stream);
        wk->stream = NULL;
        return TRUE;
    }
    if (wk->stream == NULL) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's Oekaki_ClearMessage
static void func_ov023_021862f4(OekakiWork *wk) {
    func_0201f7a8(wk->msgWindow, 0);
}

// The parent's: lets up to `limit` more players enter
// pokebw2's Oekaki_SetEntry
static void func_ov023_02186304(OekakiWork *wk, u8 limit) {
    if (func_02039338() == 0) {
        func_ov023_0218654c(wk);
        func_ov023_02184560(wk, limit);
    }
}

// The parent's update: follows the members that come and go, and returns the proc's next sequence
// pokebw2's Oekaki_UpdateParent
static int func_ov023_02186324(OekakiWork *wk) {
    int count = func_ov023_02186164(wk);

    switch (count) {
    case 1:
        // Everyone else has left
        if (wk->seq <= OEKAKI_SEQ_PARENT_END_SELECT) {
            if (func_ov023_02186558(wk) != 1) {
                wk->waitAlone = TRUE;
            }
            wk->nextSeq = OEKAKI_SEQ_NO_MEMBERS_MSG;
            if (wk->menuOpen) {
                func_ov023_02186480(wk);
                func_0204b878(wk->buttons[OEKAKI_BUTTON_END], 21);
            }
            return OEKAKI_PROC_SEQ_END;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (count < wk->memberCount) {
            if (wk->banFlag == OEKAKI_BAN_ON) {
                func_ov023_02186304(wk, 0);
            } else if (!wk->joining) {
                func_ov023_02186304(wk, 1);
            } else {
                func_ov023_02186304(wk, 0);
            }
        }
        break;
    case 0:
    case 5:
        break;
    }
    if (count < wk->memberCount) {
        wk->shareNum = func_ov023_0218654c(wk);
        wk->shareBit = func_ov023_02186558(wk);
        // The member that was joining is the one that left
        if (wk->joining && !(wk->shareBit & wk->joiningBits)) {
            func_ov023_02186304(wk, 1);
            wk->banFlag = OEKAKI_BAN_NONE;
            wk->joining = FALSE;
            wk->joiningBits = 0;
        }
    }
    wk->memberCount = func_ov023_02186164(wk);
    if (wk->shareNum < func_ov023_0218654c(wk)) {
        wk->banFlag = OEKAKI_BAN_ON;
        wk->joining = TRUE;
        wk->joiningBits = func_ov023_02186558(wk) ^ wk->shareBit;
    }
    return OEKAKI_PROC_SEQ_MAIN;
}

// pokebw2's Oekaki_SetNextSeq
static void func_ov023_02186444(OekakiWork *wk, int seq) {
    wk->nextSeq = seq;
}

// Changes to the next sequence, which while the proc ends has to be one that runs then
// pokebw2's Oekaki_UpdateSeq
static void func_ov023_0218644c(OekakiWork *wk, int procSeq) {
    if (wk->seq != wk->nextSeq) {
        if (procSeq != OEKAKI_PROC_SEQ_END) {
            wk->seq = wk->nextSeq;
        } else if (data_ov023_02186e80[wk->nextSeq].runsAtEnd) {
            wk->seq = wk->nextSeq;
        }
    }
}

// pokebw2's Oekaki_CloseMenu
static void func_ov023_02186480(OekakiWork *wk) {
    if (wk->menuOpen) {
        if (wk->menu != NULL) {
            func_ov023_02186694(wk);
        }
        wk->menuOpen = FALSE;
    }
}

// Where the menu comes up
// pokebw2's sMenuAreaRects
static const TouchRect data_ov023_02186aac[] = {
    { 48, 112, 200, 248 },
    { TOUCH_RECT_END, 0, 0, 0 },
};

// pokebw2's Oekaki_IsMenuAreaTouched
static BOOL func_ov023_021864a8(OekakiWork *wk) {
    if (func_02035c08(data_ov023_02186aac) != TOUCH_RECT_NONE) {
        return TRUE;
    }
    return FALSE;
}

// Makes what is sent of the touch screen's points
// pokebw2's Oekaki_MakeTouchInfo
static void func_ov023_021864c4(OekakiTouchInfo *info, OekakiTouchData *touchData, int color, int brush) {
    int last;
    int i;

    if (touchData->size != 0) {
        info->x[0] = touchData->points[0].x;
        info->y[0] = touchData->points[0].y;
        last = touchData->size - 1;
        info->x[1] = touchData->points[last].x;
        info->y[1] = touchData->points[last].y;
        // A point at the origin is no point
        for (i = 0; i < 2; i++) {
            if (info->x[i] + info->y[i] == 0) {
                touchData->size = 0;
            }
        }
    }
    if (touchData->size >= 2) {
        info->size = 2;
    } else {
        info->size = touchData->size;
    }
    info->color = color;
    info->brush = brush;
}

// pokebw2's Oekaki_CountJoined
static int func_ov023_0218654c(OekakiWork *wk) {
    return func_ov022_021841a4(wk->param->app);
}

// pokebw2's Oekaki_GetJoined
static u32 func_ov023_02186558(OekakiWork *wk) {
    return func_ov022_021841a0(wk->param->app);
}

// pokebw2's Oekaki_ShowScreens
static void func_ov023_02186564(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_BGSysSetBGEnabled(1, TRUE);
    GFL_BGSysSetBGEnabled(2, TRUE);
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysSetBGEnabled(5, TRUE);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
}

// pokebw2's sOekakiMenuItems
static const OekakiMenuItem data_ov023_02186abc[] = {
    { EventMapchange_Text_014, 0 },
    { EventMapchange_Text_015, 0 },
};

// The Yes and No of the question whether to leave
// pokebw2's Oekaki_CreateMenu
static AppTaskMenu *func_ov023_021865b4(OekakiWork *wk) {
    AppTaskMenuInit init;
    AppTaskMenu *menu;
    int i;

    wk->menuRes = func_02027f60(0, 2, wk->font, wk->printQueue, HEAPID_OEKAKI);
    for (i = 0; i < 2; i++) {
        wk->menuItems[i].str = GFL_MsgDataLoadStrbufNew(wk->msgData, data_ov023_02186abc[i].messageId);
        wk->menuItems[i].color = PRINT_COLOR(14, 15, 3);
        wk->menuItems[i].type = data_ov023_02186abc[i].type;
    }
    init.heapId = HEAPID_OEKAKI;
    init.itemCount = 2;
    init.items = wk->menuItems;
    init.posType = APP_TASKMENU_POS_TOP_LEFT;
    init.x = 21;
    init.y = 7;
    init.width = 10;
    init.height = 3;
    menu = func_02027778(&init, wk->menuRes);
    wk->menuOpen = TRUE;
    return menu;
}

// Returns the item picked, or 2 while none is
// pokebw2's Oekaki_UpdateMenu
static int func_ov023_02186660(OekakiWork *wk) {
    int result = 2;

    func_02027974(wk->menu);
    if (func_020279e8(wk->menu)) {
        if (func_02027a04(wk->menu) == 0) {
            result = 0;
        } else {
            result = 1;
        }
        func_ov023_02186694(wk);
    }
    return result;
}

// pokebw2's Oekaki_FreeMenu
static void func_ov023_02186694(OekakiWork *wk) {
    func_02027858(wk->menu);
    GFL_StrBufFree(wk->menuItems[1].str);
    GFL_StrBufFree(wk->menuItems[0].str);
    func_02027fd8(wk->menuRes);
    wk->menu = NULL;
}

// Records the other members as trainers the player has met
// pokebw2's Oekaki_RecordMetTrainers
static void func_ov023_021866c4(OekakiWork *wk) {
    RivalDataSave *rivalData = getHollow_RivalData(GameData_GetSaveControl(wk->param->gameData));
    int i;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        UnionAppMember *member = func_ov022_02184180(wk->param->app, i);

        if (member != NULL && i != func_02039338()) {
            func_0200ed54(rivalData, getIDAsUInt(&member->info));
        }
    }
}

// pokebw2's Oekaki_FlushPrint
static void func_ov023_02186708(OekakiWork *wk) {
    int i;

    func_0201c4e4(wk->nameQueue);
    func_0201c4e4(wk->printQueue);
    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        PrintWindow_Flush(&wk->namePrint[i], wk->nameQueue);
    }
    PrintWindow_Flush(&wk->msgPrint, wk->printQueue);
}

// Sends the command that waits, until it goes out
// pokebw2's Oekaki_SendRequest
static void func_ov023_021867c4(OekakiWork *wk) {
    switch (wk->sendCommand) {
    case OEKAKI_NET_CMD_NEWCOMER:
        if (func_0203d3a8(func_0203996c(GFL_NET_NETID_SERVER), OEKAKI_NET_CMD_NEWCOMER, 1, &wk->sendArg)) {
            wk->sendCommand = 0;
        }
        break;
    case OEKAKI_NET_CMD_PICTURE_END:
        if (func_0203d3a8(func_0203996c(GFL_NET_NETID_SERVER), OEKAKI_NET_CMD_PICTURE_END, 0, NULL)) {
            wk->sendCommand = 0;
        }
        break;
    case OEKAKI_NET_CMD_SIGNAL:
        if (func_0203d3a8(func_0203996c(GFL_NET_NETID_SERVER), OEKAKI_NET_CMD_SIGNAL, sizeof(OekakiSignal),
                          &wk->signal)) {
            wk->sendCommand = 0;
        }
        break;
    case OEKAKI_NET_CMD_LEAVING:
        if (func_0203d3a8(func_020399a8(), OEKAKI_NET_CMD_LEAVING, 0, NULL)) {
            wk->sendCommand = 0;
        }
        break;
    }
}

// pokebw2's Oekaki_RequestSend
void func_ov023_02186860(OekakiWork *wk, int command, int arg) {
    if (wk->sendCommand == 0) {
        wk->sendCommand = command;
        wk->sendArg = arg;
    }
}

// The frame before the one that is being filled
// pokebw2's Oekaki_QueuePrev
static int func_ov023_02186878(OekakiQueue *queue) {
    int index = queue->end - 1;

    if (index < 0) {
        index = OEKAKI_QUEUE_SIZE - 1;
    }
    return index;
}

// A child's: goes on to the next frame, unless this one and the one before are both empty
// pokebw2's Oekaki_QueueAdvanceChild
static void func_ov023_02186888(OekakiQueue *queue) {
    if (queue->fifo[queue->end][0].size != 0 || queue->fifo[func_ov023_02186878(queue)][0].size != 0) {
        queue->end++;
        if (queue->end >= OEKAKI_QUEUE_SIZE) {
            queue->end = 0;
        }
    }
}

// The parent's: the same for the touches of every member
// pokebw2's Oekaki_QueueAdvanceParent
static void func_ov023_021868c8(OekakiQueue *queue) {
    int i;
    int empty = 0;
    int emptyPrev = 0;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        if (queue->fifo[queue->end][i].size == 0) {
            empty++;
        }
    }
    if (empty == OEKAKI_MEMBER_MAX) {
        int prev = func_ov023_02186878(queue);

        for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
            if (queue->fifo[prev][i].size == 0) {
                emptyPrev++;
            }
        }
    }
    if (emptyPrev != OEKAKI_MEMBER_MAX) {
        queue->end++;
        if (queue->end >= OEKAKI_QUEUE_SIZE) {
            queue->end = 0;
        }
    }
}

// pokebw2's Oekaki_ClearTouchSizes
static void func_ov023_02186938(OekakiTouchInfo *touch) {
    int i;

    for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
        touch[i].size = 0;
    }
}

// Drops the oldest frame, once it has been sent
// pokebw2's Oekaki_QueueNext
static void func_ov023_02186958(OekakiQueue *queue) {
    if (queue->start != queue->end) {
        func_ov023_02186938(queue->fifo[queue->start]);
        queue->start++;
        if (queue->start >= OEKAKI_QUEUE_SIZE) {
            queue->start = 0;
        }
    }
}

// pokebw2's Oekaki_QueuePush
void func_ov023_02186988(OekakiTouchInfo *touch, OekakiQueue *queue, int netId) {
    queue->fifo[queue->end][netId] = *touch;
}

// A child's touch of the oldest frame, or an empty one
// pokebw2's Oekaki_QueuePop
static void func_ov023_021869c0(OekakiTouchInfo *touch, OekakiQueue *queue) {
    if (queue->start != queue->end) {
        *touch = queue->fifo[queue->start][0];
    } else {
        touch->size = 0;
    }
}

// Every member's touch of the oldest frame, or empty ones
// pokebw2's Oekaki_QueuePopAll
static void func_ov023_02186a04(OekakiTouchInfo *touch, OekakiQueue *queue) {
    int i;

    if (queue->start != queue->end) {
        for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
            touch[i] = queue->fifo[queue->start][i];
        }
    } else {
        for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
            touch[i].size = 0;
        }
    }
}
