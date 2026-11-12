// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov297/zukan_toroku.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_info.h"
#include "app/zukan_nickname.h"
#include "app/zukan_toroku.h"
#include "app/zukan_toroku_graphic.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"

// The Pokédex registration: the Pokémon's entry, and then the nickname question. The name is a guess from the
// parameter's name in the returns that queue it ("toroku" is registration), and the file's strings

// The BGs of the cover that the entry gives way to, and the files that it loads from the Pokédex graphics
#define BG_COVER 1
#define FILE_COVER_PALETTE 7
#define FILE_COVER_CHARS 0x11
#define FILE_COVER_SCREEN 0x27
#define COVER_CHARS_SIZE 0x800
#define COVER_SCREEN_SIZE 0x600
// The BGs of the entry, hidden once the cover is shown
#define BG_ENTRY_TOP 0
#define BG_ENTRY_BOTTOM 2

// The BGs that the entry's fade blends
#define BLEND_BGS (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3)

// The two parts of the registration
enum {
    PHASE_ENTRY,
    PHASE_NICKNAME,
};

// The steps of the entry
enum {
    ENTRY_WAIT_FADE_IN,
    ENTRY_WAIT_KEY,
    ENTRY_WAIT_SLIDE,
    ENTRY_FADE_OUT,
    ENTRY_FADE_IN,
    ENTRY_WAIT_DONE,
};

// The steps of the nickname question
enum {
    NICKNAME_WAIT_FADE_IN,
    NICKNAME_START,
    NICKNAME_WAIT_ANSWER,
    NICKNAME_WAIT_FADE_OUT,
    NICKNAME_DONE,
};

typedef struct {
    u16 heapId;
    ZukanTorokuGraphic *graphic;
    Font *font;
    PrintQueue *printQueue;
    u32 phase;
    u32 step;
    // The blend of the entry's fade, in sixteenths, and the frames until it changes
    u32 blend;
    u32 blendTimer;
    ZukanInfo *info;
    ZukanNickname *nickname;
    TCB *vblankTask;
    BOOL coverLoaded;
    // The characters of the cover, as GFL_BGSysLoadArcNCGRDynamic returns them
    u32 coverChars;
} ZukanTorokuWork;

BOOL func_ov207_0220af40(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov207_0220b0a4(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov207_0220b048(GameProc *proc, u32 *state, void *param, void *work);
void func_ov207_0220b260(ZukanTorokuWork *wk);
void func_ov207_0220b2e0(ZukanTorokuWork *wk);
void func_ov207_0220b2f8(TCB *tcb, void *data);

// pokebw2's ZUKAN_TOROKU_PROC_FUNCTIONS
// pokebw2's ZukanToroku_Exit
// pokebw2's ZukanToroku_Init
// pokebw2's ZukanToroku_Main
const GameProcFunctions data_ov207_0220bab0 = { func_ov207_0220af40, func_ov207_0220b0a4, func_ov207_0220b048 };

// Loads the cover that follows the entry
// pokebw2's ZukanToroku_LoadCover


BOOL func_ov207_0220af40(GameProc *proc, u32 *state, void *param, void *work) {
    ZukanTorokuParam *prm = param;
    ZukanTorokuWork *wk;
    BOOL cryOnly;

    GFL_OvlLoad(OVERLAY_ID(67));
    GFL_OvlLoad(OVERLAY_ID(206));
    GFL_HeapCreateChild(HEAPID_USER, 0x6a, 0x80000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(ZukanTorokuWork), 0x6a);
    MI_CpuFill8(wk, 0, sizeof(ZukanTorokuWork));
    wk->heapId = 0x6a;
    wk->graphic = func_ov207_0220b318(1, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    if (prm->mode == 0) {
        wk->phase = PHASE_ENTRY;
        wk->step = ENTRY_WAIT_FADE_IN;
    } else {
        wk->phase = PHASE_NICKNAME;
        wk->step = ENTRY_WAIT_FADE_IN;
    }
    cryOnly = FALSE;
    if (prm->mode != 0) {
        cryOnly = TRUE;
    }
    wk->info = func_ov206_021b9de0(wk->heapId, prm->pkm, prm->nationalDex, TRUE, cryOnly, 0, 0,
                                   func_ov207_0220b428(wk->graphic), wk->font, wk->printQueue, NULL, NULL, NULL);
    wk->nickname = func_ov207_0220b55c(wk->heapId, prm->pkm, prm->boxMessage, prm->boxes, prm->box,
                                       func_ov207_0220b428(wk->graphic), wk->font, wk->printQueue, prm->gameData);
    func_0203d34c(FALSE, wk->heapId);
    if (wk->phase == PHASE_NICKNAME) {
        func_ov207_0220b260(wk);
    }
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov207_0220b2f8, wk, 1);
    GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 16, 0, 2);
    return TRUE;
}

BOOL func_ov207_0220b048(GameProc *proc, u32 *state, void *param, void *work) {
    ZukanTorokuWork *wk = work;

    GFL_TCBRemove(wk->vblankTask);
    func_ov207_0220b2e0(wk);
    func_ov207_0220b6e4(wk->nickname);
    func_ov206_021ba2cc(wk->info);
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_FontFree(wk->font);
    func_ov207_0220b3b8(wk->graphic);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(0x6a);
    GFL_OvlUnload(OVERLAY_ID(206));
    GFL_OvlUnload(OVERLAY_ID(67));
    return TRUE;
}

BOOL func_ov207_0220b0a4(GameProc *proc, u32 *state, void *param, void *work) {
    ZukanTorokuWork *wk = work;
    ZukanTorokuParam *prm = param;
    BOOL done = FALSE;
    BOOL pressed;
    BOOL touched;
    BOOL nicknameDone;

    func_ov206_021ba374(wk->info);
    nicknameDone = func_ov207_0220b72c(wk->nickname);
    switch (wk->phase) {
    case PHASE_ENTRY:
        switch (wk->step) {
        case ENTRY_WAIT_FADE_IN:
            if (!GFL_FadeIsRunning()) {
                func_ov206_021ba544(wk->info);
                wk->step = ENTRY_WAIT_KEY;
            }
            break;
        case ENTRY_WAIT_KEY:
            pressed = FALSE;
            touched = func_02035c60();
            if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
                func_020355d8(FALSE);
                pressed = TRUE;
            }
            if (touched) {
                func_020355d8(TRUE);
                pressed = TRUE;
            }
            if (pressed) {
                func_ov206_021ba514(wk->info);
                wk->step = ENTRY_WAIT_SLIDE;
            }
            break;
        case ENTRY_WAIT_SLIDE:
            if (func_ov206_021ba524(wk->info) || func_ov206_021ba534(wk->info)) {
                wk->blendTimer = 0;
                wk->blend = 16;
                wk->step = ENTRY_FADE_OUT;
            }
            break;
        case ENTRY_FADE_OUT:
            wk->blendTimer++;
            if (wk->blendTimer == 4) {
                wk->blendTimer = 0;
                wk->blend--;
            }
            G2_SetBlendBrightnessExt(BLEND_BGS, BLEND_BGS, wk->blend, 0, wk->blend - 16);
            if (wk->blend == 0) {
                func_ov206_021ba4cc(wk->info);
                func_ov207_0220b260(wk);
                wk->step = ENTRY_FADE_IN;
            }
            break;
        case ENTRY_FADE_IN:
            wk->blendTimer++;
            if (wk->blendTimer == 4) {
                wk->blendTimer = 0;
                wk->blend++;
            }
            G2_SetBlendBrightnessExt(BLEND_BGS, BLEND_BGS, wk->blend, 0, wk->blend - 16);
            if (wk->blend == 16) {
                wk->step = ENTRY_WAIT_DONE;
            }
            break;
        case ENTRY_WAIT_DONE:
            if (func_ov206_021ba534(wk->info)) {
                wk->phase = PHASE_NICKNAME;
                wk->step = NICKNAME_START;
            }
            break;
        }
        break;
    case PHASE_NICKNAME:
        switch (wk->step) {
        case NICKNAME_WAIT_FADE_IN:
            if (!GFL_FadeIsRunning()) {
                func_ov206_021ba51c(wk->info);
                wk->step = NICKNAME_START;
            }
            break;
        case NICKNAME_START:
            func_ov207_0220b908(wk->nickname);
            wk->step = NICKNAME_WAIT_ANSWER;
            break;
        case NICKNAME_WAIT_ANSWER:
            if (nicknameDone == TRUE) {
                GFL_FadeSet(FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK, 0, 16, 2);
                wk->step = NICKNAME_WAIT_FADE_OUT;
            }
            break;
        case NICKNAME_WAIT_FADE_OUT:
            if (!GFL_FadeIsRunning()) {
                switch (func_ov207_0220b940(wk->nickname)) {
                case ZUKAN_NICKNAME_NO:
                    prm->nickname = FALSE;
                    break;
                case ZUKAN_NICKNAME_YES:
                    prm->nickname = TRUE;
                    break;
                }
                wk->step = NICKNAME_DONE;
                done = TRUE;
            }
            break;
        }
        break;
    }
    func_0201c4e4(wk->printQueue);
    func_ov207_0220b40c(wk->graphic);
    func_ov207_0220b420(wk->graphic);
    func_ov207_0220b424(wk->graphic);
    return done;
}


void func_ov207_0220b260(ZukanTorokuWork *wk) {
    ArcTool *arc;

    wk->coverLoaded = TRUE;
    GFL_BGSysClearScr(3);
    GFL_BGSysSetBGPriority(1, 3);
    GFL_BGSysSetBGPriority(3, 0);
    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, FILE_COVER_PALETTE, 0, 0, 0x20, wk->heapId);
    wk->coverChars = GFL_BGSysLoadArcNCGRDynamic(arc, FILE_COVER_CHARS, BG_COVER, COVER_CHARS_SIZE, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, FILE_COVER_SCREEN, BG_COVER, (u16)wk->coverChars, COVER_SCREEN_SIZE,
                                           FALSE, wk->heapId);
    GFL_ArcToolFree(arc);
    GFL_BGSysLoadScr(BG_COVER);
}

// pokebw2's ZukanToroku_FreeCover
void func_ov207_0220b2e0(ZukanTorokuWork *wk) {
    GFL_BGSysFreeCharMemory(BG_COVER, CHAR_POS(wk->coverChars), CHAR_SIZE(wk->coverChars));
}

// Hides the entry's BGs once the cover is loaded
// pokebw2's ZukanToroku_VBlank
void func_ov207_0220b2f8(TCB *tcb, void *data) {
    ZukanTorokuWork *wk = data;

    if (wk->coverLoaded) {
        GFL_BGSysSetBGEnabled(BG_ENTRY_TOP, FALSE);
        GFL_BGSysSetBGEnabled(BG_ENTRY_BOTTOM, FALSE);
        wk->coverLoaded = FALSE;
    }
}
