// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov172/pdc_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Entree Forest capture's scene (overlay 172): the battle view's graphics, the Pokémon of the dream appearing, the
// choice between the bag and leaving, and the throw of a Poké Ball. Its seq is the step of the current proc. The name is
// the ROM's string, from GFL_HeapAllocate's assert

#include "types.h"
#include "app/pdc_main.h"
#include "app/pdc_msg.h"
#include "battle/b_bag_main.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_input.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/fade.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/trainer_data_savedata.h"
#include "system/gf_font.h"

// The effects of the battle view that the scene plays: the Pokémon appearing, and its entrance
#define PDC_EFFECT_APPEAR 0x231
#define PDC_EFFECT_APPEAR_END 0x23e

// The proc's return when the player leaves
#define PDC_RESULT_CAUGHT 1
#define PDC_RESULT_LEFT 2

typedef u32 (*PdcMainProc)(PdcMain *work);

struct PdcMain {
    PdcWork *pdc;
    PartyPkm *pkm;
    Font *font;
    Font *font2;
    BtlvInput *input;
    PdcMsg *msg;
    TCBExManager *tcbManager;
    BBagParam bag;
    u32 seq;
    s32 timer;
    PdcMainProc proc;
    HeapID heapId;
    void *bagCursor;
    u8 keyCursorFlag;
};

// The steps of the bag proc
enum {
    PDC_BAG_START,
    PDC_BAG_WAIT_FADE_OUT,
    PDC_BAG_WAIT_BAG,
    PDC_BAG_WAIT_FADE_IN_NO_BALL,
    PDC_BAG_WAIT_FADE_IN_BALL,
    PDC_BAG_WAIT_CALL,
    PDC_BAG_WAIT_THROW,
    PDC_BAG_WAIT_JOINED,
    PDC_BAG_WAIT_JINGLE,
    PDC_BAG_WAIT_JINGLE_END,
    PDC_BAG_WAIT_EFFECT_END,
    PDC_BAG_WAIT_KEY,
    PDC_BAG_WAIT_FADE_OUT_END,
};

static void func_ov098_021b6364(PdcMain *work, PdcMainProc proc);
static void func_ov098_021b636c(HeapID heapId);
static void func_ov098_021b6438(void);
static u32 func_ov098_021b644c(PdcMain *work);
static u32 func_ov098_021b6504(PdcMain *work);
static u32 func_ov098_021b6578(PdcMain *work);
u32 func_ov098_021b6750(PdcMain *work);
static void func_ov098_021b67a4(u32 slowness);

// pokebw2's sButtonFlags
static const s32 data_ov098_021b6d88[2] = { 0, 0 };
// pokebw2's sButtonFlashRows
static const s32 data_ov098_021b6d90[2] = { 4, 16 };
// pokebw2's sButtonRects
static const TouchRect data_ov098_021b6da4[3] = {
    { 144, 192, 0, 80 },
    { 152, 192, 88, 168 },
    { TOUCH_RECT_END, 0, 0, 0 },
};
// pokebw2's sButtonSet
static const BtlvInputButtonSet data_ov098_021b6d98 = { data_ov098_021b6da4, data_ov098_021b6d88, data_ov098_021b6d90 };

// pokebw2's sKeyStops
static const BtlvInputKeyStop data_ov098_021b6dc0[2] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 1, 0, KEY_CURSOR_NONE },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 0, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE },
};

// pokebw2's sClActSysSetup
static const ClActSysSetup data_ov098_021b6dd8 = {
    0, 0, 0, 512, 4, 124, 4, 124, 0, 56, 48, 48, 48, 16, 16,
};

// pokebw2's PdcMain_Create
PdcMain *func_ov098_021b61e8(PdcWork *pdc, HeapID heapId) {
    PdcMain *work = GFL_HeapAllocate(heapId, sizeof(PdcMain), TRUE, "pdc_main.c", 94);
    BtlvEffectSetup *setup;
    GameData *gameData;

    work->heapId = heapId;
    work->pdc = pdc;
    func_ov098_021b636c(work->heapId);
    work->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, work->heapId);
    work->font2 = GFL_FontCreate(ARCID_FONT, 2, 0, FALSE, work->heapId);
    work->seq = 0;
    u16 trainerTypes[4] = { 0, 0, 0xffff, 0xffff };
    setup = BtlvEffect_Create(0, 0, (const BtlFieldEnv *)func_ov098_021b6144(pdc), FALSE, trainerTypes, NULL, NULL,
                              work->heapId);
    func_ov094_021f7544(setup, work->font2, work->heapId);
    GFL_HeapFree(setup);
    gameData = func_ov098_021b613c(pdc);
    work->input = func_ov094_02202058(gameData, 0, func_ov094_021f86e0(), work->font, &work->keyCursorFlag,
                                         work->heapId);
    work->tcbManager = GFL_TCBExMgrCreate(heapId, heapId, 0x40, 0x80);
    work->pkm = func_ov098_021b6140(pdc);
    work->msg = func_ov098_021b67d4(work->font, &func_ov098_021b6150(pdc)->config, work->tcbManager, work->heapId);
    work->bagCursor = func_02008254(work->heapId);
    func_ov098_021b6364(work, func_ov098_021b644c);
    return work;
}

// pokebw2's PdcMain_Update
u32 func_ov098_021b6308(PdcMain *work) {
    GFL_TCBExMgrUpdate(work->tcbManager);
    func_ov094_021f78e4();
    func_ov094_02202228(work->input);
    if (work->proc != NULL) {
        return work->proc(work);
    }
    return 0;
}

// pokebw2's PdcMain_Free
void func_ov098_021b632c(PdcMain *work) {
    GFL_TCBExMgrFree(work->tcbManager);
    func_ov098_021b68dc(work->msg);
    func_ov094_021f7808();
    func_ov094_022021f4(work->input);
    GFL_FontFree(work->font);
    GFL_FontFree(work->font2);
    func_ov098_021b6438();
    GFL_HeapFree(work->bagCursor);
    GFL_HeapFree(work);
}

// pokebw2's PdcMain_SetProc
static void func_ov098_021b6364(PdcMain *work, PdcMainProc proc) {
    work->seq = 0;
    work->proc = proc;
}

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov098_021b6db0 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_3, GX_BG0_AS_3D };
// pokebw2's sVRAMConfig
static const BGSysVRAMConfig data_ov098_021b6df4 = {
    GX_VRAM_BG_128_D,  GX_VRAM_BGEXTPLTT_NONE, GX_VRAM_SUB_BG_128_C,  GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I, GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_AB, GX_VRAM_TEXPLTT_01_FG,  GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's PdcMain_InitGraphics
static void func_ov098_021b636c(HeapID heapId) {
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    func_0201dd60();
    GFL_BGSysSetVRAMBanks(&data_ov098_021b6df4);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysSetLCDConfig(&data_ov098_021b6db0);
    GFL_G3DSysCreate(FALSE, 1, FALSE, 1, 0, heapId, NULL);
    GFL_G3DSysSetSwapBufferParams(1, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_AntiAlias(TRUE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G2_SetBG0Priority(1);
    ClActSys_Create(&data_ov098_021b6dd8, &data_ov098_021b6df4, heapId);
    GFL_BGSysSet3DBGPriority(1);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
}

// pokebw2's PdcMain_ExitGraphics
static void func_ov098_021b6438(void) {
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    func_0204a5ac();
    GFL_G3DSysFree();
}

// The Pokémon appears and the messages introduce it
// pokebw2's PdcMain_ProcIntro
static u32 func_ov098_021b644c(PdcMain *work) {
    switch (work->seq) {
    case 0:
        func_ov098_021b6b5c(work->msg);
        work->seq++;
        break;
    case 1:
        if (func_ov098_021b6b68(work->msg)) {
            func_ov094_021f7ee8(work->pkm, 1);
            func_ov094_021f7998(PDC_EFFECT_APPEAR);
            func_ov098_021b67a4(2);
            work->seq++;
        }
        break;
    case 2:
        if (!func_ov094_021f7eb4()) {
            func_ov098_021b6910(work->msg, work->pdc);
            work->seq++;
        }
        break;
    case 3:
        if (func_ov098_021b6a3c(work->msg)) {
            func_ov094_021f8090(func_ov098_021b6154(work->pdc), func_ov098_021b6140(work->pdc), 1);
            func_ov098_021b6b5c(work->msg);
            work->seq++;
        }
        break;
    case 4:
        if (func_ov098_021b6b68(work->msg)) {
            func_ov094_021f7998(PDC_EFFECT_APPEAR_END);
            work->seq++;
        }
        break;
    case 5:
        if (!func_ov094_021f7eb4()) {
            func_ov098_021b6364(work, func_ov098_021b6504);
        }
        break;
    }
    return 0;
}

static u32 func_ov098_021b6504(PdcMain *work) {
    switch (work->seq) {
    case 0:
        func_ov098_021b694c(work->msg, work->pdc);
        work->seq++;
        break;
    case 1:
        if (func_ov098_021b6a3c(work->msg)) {
            func_ov094_0220270c(work->input, 7, NULL);
            work->seq++;
        }
        break;
    case 2: {
        s32 button = func_ov094_02202dc4(work->input, &data_ov098_021b6d98, data_ov098_021b6dc0);

        if (button != -1) {
            if (button != 0) {
                func_ov098_021b6364(work, func_ov098_021b6750);
            } else {
                func_ov098_021b6364(work, func_ov098_021b6578);
            }
        }
        break;
    }
    }
    return 0;
}

static u32 func_ov098_021b6578(PdcMain *work) {
    switch (work->seq) {
    case PDC_BAG_START:
        work->bag.bag = func_ov098_021b614c(work->pdc);
        work->bag.bagCursor = work->bagCursor;
        work->bag.mode = 3;
        work->bag.font = work->font;
        work->bag.item = 0;
        work->bag.quit = FALSE;
        work->bag.heapId = work->heapId;
        work->bag.shooterEnergy = 0;
        work->bag.shooterSpent = 0;
        work->bag.done = FALSE;
        work->bag.playSound = TRUE;
        work->bag.usingKeys = &work->keyCursorFlag;
        func_ov094_022025d8(work->input, 1);
        GFL_OvlLoad(OVERLAY_ID(196));
        work->seq++;
        break;
    case PDC_BAG_WAIT_FADE_OUT:
        if (!func_ov094_022026fc(work->input)) {
            GFL_OvlLoad(OVERLAY_ID(197));
            func_ov197_0220c834(&work->bag);
            work->seq++;
        }
        break;
    case PDC_BAG_WAIT_BAG:
        if (work->bag.done) {
            GFL_OvlUnload(OVERLAY_ID(196));
            GFL_OvlUnload(OVERLAY_ID(197));
            func_ov094_0220266c(work->input);
            if (work->bag.item == 0) {
                work->seq = PDC_BAG_WAIT_FADE_IN_NO_BALL;
            } else {
                BagSave_SubItem(func_ov098_021b614c(work->pdc), work->bag.item, 1, work->heapId);
                work->seq = PDC_BAG_WAIT_FADE_IN_BALL;
            }
        }
        break;
    case PDC_BAG_WAIT_FADE_IN_NO_BALL:
        if (!func_ov094_022026fc(work->input)) {
            func_ov098_021b6364(work, func_ov098_021b6504);
        }
        break;
    case PDC_BAG_WAIT_FADE_IN_BALL:
        if (!func_ov094_022026fc(work->input)) {
            func_ov098_021b6988(work->msg, work->pdc);
            work->seq++;
        }
        break;
    case PDC_BAG_WAIT_CALL:
        if (func_ov098_021b6a3c(work->msg)) {
            func_ov094_021f7cfc(1, work->bag.item, 4, 1, 0);
            work->seq++;
        }
        break;
    case PDC_BAG_WAIT_THROW:
        if (!func_ov094_021f7eb4()) {
            func_ov098_021b69c4(work->msg, work->pdc);
            work->seq++;
        }
        break;
    case PDC_BAG_WAIT_JOINED:
        if (func_ov098_021b6b74(work->msg)) {
            GFL_SndBGMPlay(SEQ_ME_POKEGET, 0xffff);
        }
        if (func_ov098_021b6a3c(work->msg)) {
            work->seq++;
        }
        break;
    case PDC_BAG_WAIT_JINGLE:
        if (!GFL_SndBGMIsPlaying()) {
            func_ov094_021f7c0c();
            GFL_SndBGMPlay(SEQ_BGM_WIN1, 0xffff);
            work->seq++;
        }
        break;
    case PDC_BAG_WAIT_JINGLE_END:
        if (!func_ov094_021f7eb4()) {
            work->timer = 80;
            work->seq++;
        }
        break;
    case PDC_BAG_WAIT_EFFECT_END:
        if ((GCTX_HIDGetPressedKeys() & 3) || func_02035c60() || --work->timer == 0) {
            GFL_FadeSet(3, 0, 16, 2);
            work->seq++;
        }
        break;
    case PDC_BAG_WAIT_KEY:
        if (!GFL_FadeIsRunning()) {
            u16 ball = PML_ItemGetMonsBallID(work->bag.item);

            PokeParty_SetParam(func_ov098_021b6140(work->pdc), 0x98, ball);
            return PDC_RESULT_CAUGHT;
        }
        break;
    }
    return 0;
}



// The player leaves
// pokebw2's PdcMain_ProcLeave
u32 func_ov098_021b6750(PdcMain *work) {
    switch (work->seq) {
    case 0:
        func_ov098_021b6a00(work->msg, work->pdc);
        work->seq++;
        break;
    case 1:
        if (func_ov098_021b6a3c(work->msg)) {
            GFL_FadeSet(3, 0, 16, 2);
            GFL_SndBGMFadeOut(8);
            work->seq++;
        }
        break;
    case 2:
        if (!GFL_FadeIsRunning()) {
            return PDC_RESULT_LEFT;
        }
        break;
    }
    return 0;
}

// Fades in from black, or from white if the screens were white
// pokebw2's PdcMain_FadeIn
static void func_ov098_021b67a4(u32 slowness) {
    if (GXx_GetMasterBrightness_(REG_MASTER_BRIGHT_ADDR) <= 0) {
        GFL_FadeSet(3, 16, 0, slowness);
    } else {
        GFL_FadeSet(12, 16, 0, slowness);
    }
}
