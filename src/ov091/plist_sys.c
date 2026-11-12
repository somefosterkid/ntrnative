// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov165/plist_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pokelist.h"
#include "battle/regulation.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "field/hidden_event.h"
#include "field/musical.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_sync.h"
#include "gfl/particle.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "system/wipe.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "pml/evolution.h"
#include "pml/item.h"
#include "pml/mail.h"
#include "pml/poke_party.h"
#include "save/bag.h"
#include "save/trainer_card.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "pml/poke_graphic.h"
#include "nitro/math.h"

// The party list's screen: its graphics, the cursor, and what each mode does with the Pokémon picked. The ROM
// doesn't name this file; it is named for the list's system, which pokelist.c's proc runs

void func_ov091_021b7b98(PokeListWork *wk);
void func_ov091_021b83d8(PokeListWork *wk);
void func_ov091_021b981c(PokeListWork *wk);
void func_ov091_021b9020(PokeListWork *wk);
void func_ov091_021b7cc4(PokeListWork *wk);
void func_ov091_021b8180(PokeListWork *wk);
void func_ov091_021b892c(PokeListWork *wk);
void func_ov091_021b8508(PokeListWork *wk);
void func_ov091_021b86d8(PokeListWork *wk);
void func_ov091_021b8758(PokeListWork *wk);
static void func_ov091_021b8a8c(PokeListWork *wk);
void func_ov091_021b907c(PokeListWork *wk);
void func_ov091_021b916c(PokeListWork *wk);
static void func_ov091_021b8418(PokeListWork *wk);
static void func_ov091_021ba288(PokeListWork *wk);
void func_ov091_021b8ac4(PokeListWork *wk);
void func_ov091_021b8db4(PokeListWork *wk);
void func_ov091_021b7624(PokeListWork *wk);
void func_ov091_021b7268(PokeListWork *wk);
void func_ov091_021b71b0(PokeListWork *wk);
void func_ov091_021b7298(PokeListWork *wk);
void func_ov091_021b71e4(PokeListWork *wk);
void func_ov091_021b7698(PokeListWork *wk);
static void func_ov091_021b6b24(PokeListWork *wk);
static void func_ov091_021b6d64(TCB *tcb, void *data);
static void func_ov091_021b6d90(PokeListWork *wk);
static void func_ov091_021b6fec(PokeListWork *wk);
static void func_ov091_021b7078(const BGSetup *setup, u8 bg, u8 mode);
void func_ov091_021b9f34(PokeListWork *wk);
void func_ov091_021b9f44(PokeListWork *wk, u32 item);
void func_ov091_021ba444(PokeListWork *wk);
static void func_ov091_021ba5e0(PokeListWork *wk, PartyPkm *pkm, u8 slot);
void func_ov091_021ba670(PokeListWork *wk, u16 item);
void func_ov091_021ba688(PokeListWork *wk, u16 item);
BOOL func_ov091_021b9aa0(PokeListWork *wk);
BOOL func_ov091_021b9ab4(PokeListWork *wk);
void func_ov091_021ba5b4(PokeListWork *wk, PartyPkm *pkm);
void func_ov091_021ba60c(PokeListWork *wk, PartyPkm *pkm);
void func_ov091_021ba098(PokeListWork *wk);
void func_ov091_021ba394(PokeListWork *wk);
void func_ov091_021ba4e4(PokeListWork *wk);
static void func_ov091_021ba624(PokeListWork *wk, PartyPkm *pkm, u16 item);
u16 func_ov091_021ba65c(PokeListWork *wk, u16 item);


// Where the cursor goes from a button when up is pressed, by the number of Pokémon in the party
// pokebw2's sUpFromButtons
static const u8 data_ov091_021beab0[6] = { 0, 1, 1, 3, 3, 5 };
// pokebw2's sUpFromCancel
static const u8 data_ov091_021beaaa[6] = { 0, 0, 2, 2, 4, 4 };



// pokebw2's data_ov091_021beac4
// pokebw2's sCameraTarget
static const VecFx32 data_ov091_021beac4 = { 0, 0, 0 };
// pokebw2's sCameraUp
static const VecFx32 data_ov091_021bead0 = { 0, FX32_ONE, 0 };
// pokebw2's sCameraPos
static const VecFx32 data_ov091_021beab8 = { 0, 0, FX32_CONST(128) };

// pokebw2's sLCDConfig
static const BGSysLCDConfig data_ov091_021beaec = { 1, 0, 0, 0 };

// pokebw2's sBG3Setup
static const BGSetup data_ov091_021bebdc = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xe, 0, 0x5000, 1, 3, 0, 0 };
// pokebw2's sBG4Setup
static const BGSetup data_ov091_021beafc = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xf, 6, 0x8000, 1, 0, 0, 0 };
// pokebw2's sBG5Setup
static const BGSetup data_ov091_021beb1c = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xe, 4, 0x8000, 1, 1, 0, 0 };
// pokebw2's sBG6Setup
static const BGSetup data_ov091_021beb3c = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xd, 2, 0x8000, 1, 2, 0, 0 };
// pokebw2's sBG7Setup
static const BGSetup data_ov091_021beb5c = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xc, 0, 0x6000, 1, 3, 0, 0 };
// pokebw2's sBG0Setup
static const BGSetup data_ov091_021beb7c = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xf, 6, 0x8000, 0, 0, 0, 0 };
// pokebw2's sBG1Setup
static const BGSetup data_ov091_021beb9c = { 0, 0, 0x1000, 0, 3, GX_BG_COLORMODE_16, 0xa, 2, 0x8000, 0, 1, 0, 0 };
// pokebw2's sBG2Setup
static const BGSetup data_ov091_021bebbc = { 0, 0, 0x1000, 0, 3, GX_BG_COLORMODE_16, 0xc, 4, 0x8000, 0, 2, 1, 0 };

// Blends the plates' palettes toward white while a plate flashes, and makes the cursor's plate glow
// pokebw2's PokeList_UpdateGlow

BOOL func_ov091_021b660c(PokeListWork *wk) {
    u8 count = PokeParty_GetPkmCount(wk->param->party);
    u8 i;

    wk->touch = func_020355c4();
    wk->menuItem = 0x17;
    wk->state = 0;
    wk->subState = 0;
    wk->cursorPos = 0;
    wk->pkm = NULL;
    wk->selectPos = 9;
    wk->selectPos2 = 9;
    wk->showShortcutButtons = FALSE;
    wk->glowPaused = FALSE;
    wk->enteredCount = 0;
    wk->flashTimer = 0;
    wk->glowAngle = 0;
    wk->unk110 = 0;
    wk->waitTimer = 0;
    wk->hpDoneFunc = NULL;
    wk->statsWindow = NULL;
    wk->pressedButton = NULL;
    wk->buttons[0] = NULL;
    wk->buttons[1] = NULL;
    wk->decided = FALSE;
    wk->demo = 0;
    wk->timeUp = FALSE;
    wk->unk278 = FALSE;
    wk->param->unk62[1] = 0;
    wk->unk28 = TRUE;
    wk->playHealSe = FALSE;
    for (i = 0; i < POKELIST_CL_RES_COUNT; i++) {
        wk->clResources[i] = CL_RES_NONE;
    }
    wk->wasMode19 = wk->param->mode == 0x19 ? TRUE : FALSE;
    if (wk->param->mode == 0x18) {
        wk->param->mode = 3;
        wk->wasMode18 = TRUE;
    } else {
        wk->wasMode18 = FALSE;
    }
    if (wk->param->mode == 0x1a) {
        wk->param->mode = 1;
        wk->wasMode1A = TRUE;
    } else {
        wk->wasMode1A = FALSE;
    }
    if (wk->param->mode == 6 && wk->param->move == 0) {
        wk->param->move = PML_ItemGetTMWazaID(wk->param->item);
    }
    func_ov091_021b6d90(wk);
    func_ov091_021b7298(wk);
    func_ov091_021b71e4(wk);
    for (i = 0; i < POKELIST_PLATE_COUNT; i++) {
        if (i < count) {
            PartyPkm *pkm = PokeParty_GetPkm(wk->param->party, i);

            wk->plates[i] = func_ov091_021ba6a0(wk, i, pkm);
        } else {
            wk->plates[i] = func_ov091_021ba81c(wk, i);
        }
        if (func_ov091_021bb43c(wk->plates[i]) <= 5) {
            wk->enteredCount++;
        }
    }
    wk->taskMenuRes = func_02027f60(0, 1, wk->font, wk->printQueue, wk->heapId);
    func_ov091_021b709c(wk);
    if (func_ov091_021b9a84(wk) == TRUE) {
        func_ov091_021bcccc(wk);
    }
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov091_021b6d64, wk, 8);
    wk->message = func_ov091_021bb594(wk);
    wk->menu = func_ov091_021bba8c(wk);
    func_ov091_021b7698(wk);
    GFL_BGSysQueueScrLoad(3);
    GFL_BGSysQueueScrLoad(2);
    GFL_BGSysQueueScrLoad(7);
    func_0203d34c(TRUE, wk->heapId);
    return TRUE;
}

BOOL func_ov091_021b6818(PokeListWork *wk) {
    u8 i;

    GFL_TCBRemove(wk->vblankTask);
    if (func_ov091_021b9a84(wk) == TRUE) {
        func_ov091_021bbe44(wk->buttons[0]);
        func_ov091_021bbe44(wk->buttons[1]);
        wk->buttons[0] = NULL;
        wk->buttons[1] = NULL;
    }
    func_ov091_021bbae0(wk, wk->menu);
    func_ov091_021bb5fc(wk, wk->message);
    if (wk->statsWindow != NULL) {
        BmpWin_Free(wk->statsWindow);
    }
    for (i = 0; i < POKELIST_PLATE_COUNT; i++) {
        func_ov091_021ba890(wk, wk->plates[i]);
    }
    func_02027fd8(wk->taskMenuRes);
    func_ov091_021b7268(wk);
    if (func_ov091_021b9a84(wk) == TRUE) {
        func_ov091_021bcd80(wk);
    }
    func_ov091_021b71b0(wk);
    func_ov091_021b7624(wk);
    func_ov091_021b6fec(wk);
    if (wk->wasMode18 == TRUE) {
        wk->param->mode = 0x18;
    }
    if (wk->wasMode19 == TRUE) {
        wk->param->mode = 0x19;
    }
    if (wk->wasMode1A == TRUE) {
        wk->param->mode = 0x1a;
    }
    func_020355d8(wk->touch);
    return TRUE;
}

BOOL func_ov091_021b68f0(PokeListWork *wk) {
    u8 i;

    switch (wk->state) {
    case 0:
        GFL_WipeSet(WIPE_MODE_SUB_FIRST, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
        wk->state = 1;
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            wk->state = wk->nextState;
        }
        break;
    case 2:
    case 3:
    case 4:
        func_ov091_021b83d8(wk);
        break;
    case 5:
        func_ov091_021b981c(wk);
        break;
    case 6:
        func_ov091_021b9020(wk);
        break;
    case 7:
        func_ov091_021b9750(wk);
        break;
    case 8:
        break;
    case 9:
        func_ov091_021b97d4(wk);
        break;
    case 10:
        func_ov091_021bb30c(wk, wk->plates[wk->cursorPos]);
        if (wk->playHealSe == TRUE) {
            wk->playHealSe = FALSE;
            GFL_SndSEPlay(SEQ_SE_RECOVERY);
        }
        wk->state = 11;
    case 11:
        if (func_ov091_021bb358(wk, wk->plates[wk->cursorPos]) == TRUE) {
            if (wk->hpDoneFunc != NULL) {
                wk->hpDoneFunc(wk);
            } else {
                wk->state = 21;
            }
        }
        break;
    case 12:
        wk->waitTimer++;
        if (wk->waitTimer > 16) {
            if (wk->wasMode1A == TRUE) {
                if (func_ov090_021b6184(wk->param, 0, 0) == TRUE) {
                    wk->state = 17;
                    func_ov091_021bbe44(wk->buttons[0]);
                    func_ov091_021bbe44(wk->buttons[1]);
                    wk->buttons[0] = NULL;
                    wk->buttons[1] = NULL;
                    func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
                    func_ov091_021bb7e8(wk, wk->message, 0xb0);
                    func_ov091_021bba68(wk, wk->message);
                }
            } else {
                wk->state = 19;
            }
        }
        break;
    case 13:
        func_ov091_021bc6cc(wk);
        break;
    case 14:
        func_ov091_021bd8b0(wk);
        break;
    case 15:
        func_ov091_021bd9c8(wk);
        break;
    case 16:
        func_ov091_021bd8e4(wk);
        break;
    case 17:
        func_ov090_021b61e8(wk->param, 2);
        wk->state = 18;
        break;
    case 18:
        if (func_ov090_021b61fc(wk->param, 2) == TRUE) {
            wk->state = 19;
        }
        break;
    case 20:
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
        wk->state = 21;
        break;
    case 19:
        if (wk->pressedButton == NULL || func_0204b98c(wk->pressedButton) == FALSE) {
            GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1, wk->heapId);
            wk->state = 21;
        }
        break;
    case 21:
        if (GFL_WipeIsFinished() == TRUE) {
            return TRUE;
        }
        break;
    }
    for (i = 0; i < POKELIST_PLATE_COUNT; i++) {
        func_ov091_021ba8cc(wk, wk->plates[i]);
    }
    func_ov091_021bb644(wk, wk->message);
    func_ov091_021b6b24(wk);
    if (func_ov091_021b9a84(wk) == TRUE) {
        func_ov091_021bcde8(wk);
        func_ov091_021bbe50(wk->buttons[0]);
        func_ov091_021bbe50(wk->buttons[1]);
    }
    func_0201c4e4(wk->printQueue);
    func_0204a600();
    return FALSE;
}



static void func_ov091_021b6b24(PokeListWork *wk) {
    MIi_CpuCopy16(wk->baseColors, wk->colors, sizeof(wk->colors));
    if (wk->flashTimer != 0) {
        u8 level;
        u8 i;
        u8 j;

        wk->flashTimer--;
        level = wk->flashTimer;
        if (level > 6) {
            level = 12 - level;
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 16; j++) {
                if (j < 2 || j > 4) {
                    u16 color = wk->baseColors[i][j];
                    u8 g = (color & 0x3e0) >> 5;
                    u8 b = (color & 0x7c00) >> 10;
                    u8 r = color & 0x1f;

                    r = r + level * 2 > 31 ? 31 : r + level * 2;
                    g = g + level * 2 > 31 ? 31 : g + level * 2;
                    b = b + level * 2 > 31 ? 31 : b + level * 2;
                    wk->colors[i][j] = GX_RGB(r, g, b);
                }
            }
        }
    }
    {
        // The glow angle step: folded, but a function static constant stays in .rodata
        static const u16 angleStep = 0x400;
        u16 glowColors[3] = { 0x7ff4, 0x7fff, 0x7be9 };
        u16 baseColors[3] = { 0x4a0a, 0x5ef7, 0x4140 };
        u8 i;
        u8 j;
        fx32 ratio;

        if (wk->glowPaused == FALSE) {
            if (wk->glowAngle + angleStep >= 0x10000) {
                wk->glowAngle = wk->glowAngle + angleStep - 0x10000;
            } else {
                wk->glowAngle = wk->glowAngle + angleStep;
            }
        } else {
            wk->glowAngle = 0xc000;
        }
        ratio = (FX_SinIdx(wk->glowAngle) + FX32_ONE) / 2;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                u16 glow = glowColors[j];
                u16 base = baseColors[j];
                u8 glowR = glow & 0x1f;
                u8 glowG = (glow & 0x3e0) >> 5;
                u8 glowB = (glow & 0x7c00) >> 10;
                u8 baseR = base & 0x1f;
                u8 baseG = (base & 0x3e0) >> 5;
                u8 baseB = (base & 0x7c00) >> 10;
                s8 dr = (baseR - glowR) * ratio >> FX32_SHIFT;
                s8 dg = (baseG - glowG) * ratio >> FX32_SHIFT;
                s8 db = (baseB - glowB) * ratio >> FX32_SHIFT;
                u8 r = MATH_CLAMP(glowR + dr, 0, 31);
                u8 g = MATH_CLAMP(glowG + dg, 0, 31);
                u8 b = MATH_CLAMP(glowB + db, 0, 31);

                wk->colors[i][2 + j] = GX_RGB(r, g, b);
            }
        }
    }
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, 0xc0, wk->colors, sizeof(wk->colors));
}

static const u16 data_ov091_021bec04[4] = { 0x4a0a, 0x5ef7, 0x4140, 0 };

// pokebw2's PokeList_VBlank
static void func_ov091_021b6d64(TCB *tcb, void *data) {
    PokeListWork *wk = data;

    if (wk->glowPaused == TRUE) {
        GX_SetVisibleWnd(GX_WNDMASK_W0);
    } else {
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
    }
    func_0204a648();
}

// pokebw2's PokeList_InitGraphics
static void func_ov091_021b6d90(PokeListWork *wk) {
    BGSysVRAMConfig vramConfig = {
        GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
        GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_NONE,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
        GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
    };
    ClActSysSetup clactSetup;

    if (func_ov091_021b9a84(wk) == TRUE) {
        vramConfig.objSub = GX_VRAM_SUB_OBJ_128_D;
        vramConfig.texture = GX_VRAM_TEX_NONE;
    } else {
        vramConfig.objSub = GX_VRAM_SUB_OBJ_16_I;
    }
    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, 0);
    func_0202223c(1, 0);
    func_02022224(0);
    func_02022224(1);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    GFL_BGSysSetVRAMBanks(&vramConfig);
    GFL_BGSysCreate(wk->heapId);
    BmpWin_InitAllocator(wk->heapId);
    GFL_BGSysSetLCDConfig(&data_ov091_021beaec);
    func_ov091_021b7078(&data_ov091_021bebdc, 3, 0);
    func_ov091_021b7078(&data_ov091_021bebbc, 2, 0);
    func_ov091_021b7078(&data_ov091_021beb9c, 1, 0);
    func_ov091_021b7078(&data_ov091_021beb5c, 7, 0);
    func_ov091_021b7078(&data_ov091_021beb3c, 6, 0);
    func_ov091_021b7078(&data_ov091_021beb1c, 5, 0);
    func_ov091_021b7078(&data_ov091_021beafc, 4, 0);
    func_ov091_021b6f0c(wk);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, GX_PLANEMASK_BG3, 16, 10);
    G2S_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 16, 10);
    GFL_BGSysMoveBG(2, BG_MOVE_SET_X, 128);
    GFL_BGSysMoveBG(1, BG_MOVE_SET_X, 128);
    clactSetup = data_020a1448;
    ClActSys_Create(&clactSetup, &vramConfig, wk->heapId);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    NNS_GfdClearVramTransferManagerTask();
}

// pokebw2's PokeList_CreateBG0
void func_ov091_021b6f0c(PokeListWork *wk) {
    GX_SetGraphicsMode(1, 0, 0);
    func_ov091_021b7078(&data_ov091_021beb7c, 0, 0);
}

// pokebw2's PokeList_Init3D
void func_ov091_021b6f28(PokeListWork *wk) {
    // Nothing reads it, but Black's .rodata has these 16 zero bytes (pokebw2's sEdgeColors)
    static const GXRgb sEdgeColors[8] = { 0 };

    GX_SetGraphicsMode(1, 0, 1);
    GFL_G3DSysCreate(FALSE, 1, FALSE, 1, 0, wk->heapId, NULL);
    G2_SetBG0Priority(0);
    G3X_EdgeMarking(TRUE);
    G3X_AntiAlias(TRUE);
    G3X_AlphaBlend(TRUE);
    wk->camera = GFL_G3DCameraCreate(2, 0, 0x50d7, 0, 0x6bca, FX32_ONE, FX32_CONST(128), 0, &data_ov091_021beab8, &data_ov091_021bead0,
                                     &data_ov091_021beac4, wk->heapId);
    GFL_G3DCameraFlush(wk->camera);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, 16,
                     10);
    func_02050b80(wk->heapId);
}

// pokebw2's PokeList_ExitGraphics
static void func_ov091_021b6fec(PokeListWork *wk) {
    NNS_GfdClearVramTransferManagerTask();
    func_0204a5ac();
    func_ov091_021b7034(wk);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's PokeList_ReleaseBG0
void func_ov091_021b7034(PokeListWork *wk) {
    GFL_BGSysReleaseBG(0);
}

// pokebw2's PokeList_Exit3D
void func_ov091_021b7040(PokeListWork *wk) {
    func_02050ed4();
    GFL_G3DCameraFree(wk->camera);
    GFL_G3DSysFree();
    GFL_BGSysReleaseBG(0);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, GX_PLANEMASK_BG3, 16, 10);
}

// pokebw2's PokeList_CreateBG
static void func_ov091_021b7078(const BGSetup *setup, u8 bg, u8 mode) {
    GFL_BGSysCreateBG(bg, setup, mode);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

void func_ov091_021b709c(PokeListWork *wk) {
    ClActorSetup cursorSetup;
    ClActorSetup buttonSetup;

    wk->actorUnit = func_0204b100(10, 8, wk->heapId);
    func_0204b270(wk->actorUnit);
    cursorSetup.x = 0;
    cursorSetup.y = 0;
    cursorSetup.sequence = 0;
    cursorSetup.priority = 10;
    cursorSetup.bgPriority = 3;
    wk->cursor = func_0204b294(wk->actorUnit, wk->clResources[CL_RES_CHAR(0)], wk->clResources[CL_RES_PLTT(0)],
                               wk->clResources[CL_RES_CELL(0)], &cursorSetup, 0, wk->heapId);
    wk->subCursor = func_0204b294(wk->actorUnit, wk->clResources[CL_RES_CHAR(0)], wk->clResources[CL_RES_PLTT(0)],
                                  wk->clResources[CL_RES_CELL(0)], &cursorSetup, 0, wk->heapId);
    func_0204b92c(wk->cursor, TRUE);
    func_0204b92c(wk->subCursor, TRUE);
    func_0204b3dc(wk->cursor, FALSE);
    func_0204b3dc(wk->subCursor, FALSE);
    buttonSetup.x = 224;
    buttonSetup.y = 168;
    buttonSetup.sequence = 1;
    buttonSetup.priority = 0;
    buttonSetup.bgPriority = 0;
    wk->exitButton = func_0204b294(wk->actorUnit, wk->clResources[CL_RES_CHAR(2)], wk->clResources[CL_RES_PLTT(1)],
                                   wk->clResources[CL_RES_CELL(2)], &buttonSetup, 0, wk->heapId);
    func_0204b92c(wk->exitButton, TRUE);
    buttonSetup.x = 196;
    buttonSetup.sequence = 0;
    wk->shortcutButton = func_0204b294(wk->actorUnit, wk->clResources[CL_RES_CHAR(2)],
                                       wk->clResources[CL_RES_PLTT(1)], wk->clResources[CL_RES_CELL(2)], &buttonSetup,
                                       0, wk->heapId);
    func_0204b92c(wk->shortcutButton, TRUE);
}


void func_ov091_021b71b0(PokeListWork *wk) {
    func_0204b3b4(wk->exitButton);
    func_0204b3b4(wk->shortcutButton);
    func_0204b3b4(wk->cursor);
    func_0204b3b4(wk->subCursor);
    func_0204b1cc(wk->actorUnit);
}

void func_ov091_021b71e4(PokeListWork *wk) {
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->smallFont = GFL_FontCreate(ARCID_FONT, 3, 0, FALSE, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, 2, TEXT_BANK_BTL_MAIN_11, wk->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1c0, 0x20, wk->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1c0, 0x20, wk->heapId);
    wk->printQueue = func_0201c450(0x1800, wk->heapId);
    func_0201dd68();
}


void func_ov091_021b7268(PokeListWork *wk) {
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->smallFont);
    GFL_FontFree(wk->font);
}

void func_ov091_021b7298(PokeListWork *wk) {
    ArcTool *arc;
    ArcTool *iconArc;
    u32 fileId;
    u32 cellFileId;

    arc = GFL_ArcSysCreateFileHandle(ARCID_POKELIST_GRA, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 6, 0, 0, 0, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0x1a0, 0x20, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 15, 3, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 20, 3, 0, 0, FALSE, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 12, 2, 0, 0, FALSE, wk->heapId);
    wk->plateScreenFile = GFL_G2DIOReadNSCRArc(arc, 18, FALSE, &wk->plateScreen, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 5, 4, 0, 0, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 14, 7, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 19, 7, 0, 0, FALSE, wk->heapId);
    wk->clResources[CL_RES_PLTT(0)] = func_0204ac18(arc, 4, 0, 0, 0, 3, wk->heapId);
    wk->clResources[CL_RES_CHAR(0)] = func_0204a6c8(arc, 11, FALSE, 0, wk->heapId);
    wk->clResources[CL_RES_CHAR(1)] = func_0204a6c8(arc, 8, FALSE, 0, wk->heapId);
    wk->clResources[CL_RES_CELL(0)] = func_0204af28(arc, 23, 26, wk->heapId);
    wk->clResources[CL_RES_CELL(1)] = func_0204af28(arc, 22, 25, wk->heapId);
    GFL_ArcToolFree(arc);

    iconArc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, wk->heapId);
    fileId = func_0201bc3c();
    wk->clResources[CL_RES_PLTT(6)] = func_0204acec(iconArc, fileId, 0, 0xc0, wk->heapId);
    cellFileId = func_0201bc40();
    wk->clResources[CL_RES_CELL(7)] = func_0204af28(iconArc, cellFileId, getOBJTileMapping_MainEng(), wk->heapId);
    GFL_ArcToolFree(iconArc);

    iconArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), wk->heapId);
    // BUG: The first handle was freed already
    GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), 0, 0x160, 0x20, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, func_02027628(), 1, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, func_02027630(), 1, 0, 0, FALSE, wk->heapId);
    GFL_BGSysSetScrPaletteNo(1, 0, 0, 0x40, 0x20, 11);
    wk->clResources[CL_RES_PLTT(5)] = func_0204ac18(iconArc, func_02027758(), 0, 0xa0, 0, 1, wk->heapId);
    wk->clResources[CL_RES_CHAR(6)] = func_0204a6c8(iconArc, func_0202775c(2), FALSE, 0, wk->heapId);
    cellFileId = func_02027760(2);
    wk->clResources[CL_RES_CELL(6)] = func_0204af28(iconArc, cellFileId, func_02027764(2), wk->heapId);
    wk->clResources[CL_RES_PLTT(1)] = func_0204ac18(iconArc, func_02027614(), 0, 0x120, 0, 3, wk->heapId);
    wk->clResources[CL_RES_CHAR(2)] = func_0204a6c8(iconArc, func_02027618(), FALSE, 0, wk->heapId);
    cellFileId = func_0202761c(2);
    wk->clResources[CL_RES_CELL(2)] = func_0204af28(iconArc, cellFileId, func_02027620(2), wk->heapId);
    wk->clResources[CL_RES_PLTT(2)] = func_0204ac18(iconArc, func_02027694(), 0, 0x60, 0, 1, wk->heapId);
    wk->clResources[CL_RES_CHAR(3)] = func_0204a6c8(iconArc, func_02027698(), FALSE, 0, wk->heapId);
    cellFileId = func_0202769c(2);
    wk->clResources[CL_RES_CELL(3)] = func_0204af28(iconArc, cellFileId, func_020276a0(2), wk->heapId);
    wk->clResources[CL_RES_PLTT(4)] = func_0204ac18(iconArc, func_020276b4(), 0, 0x80, 0, 1, wk->heapId);
    wk->clResources[CL_RES_CHAR(5)] = func_0204a6c8(iconArc, func_020276b8(), FALSE, 0, wk->heapId);
    cellFileId = func_020276bc(2);
    wk->clResources[CL_RES_CELL(5)] = func_0204af28(iconArc, cellFileId, func_020276c0(2), wk->heapId);
    GFL_ArcToolFree(iconArc);
    MIi_CpuCopy16((u16 *)HW_BG_PLTT + 6 * 16, wk->baseColors, sizeof(wk->baseColors));
}


void func_ov091_021b7624(PokeListWork *wk) {
    u8 i;

    GFL_HeapFree(wk->plateScreenFile);
    for (i = 0; i < CL_RES_PLTT_COUNT; i++) {
        if (wk->clResources[i] != CL_RES_NONE) {
            func_0204ada4(wk->clResources[i]);
        }
    }
    for (i = CL_RES_CHAR(0); i < CL_RES_CHAR(CL_RES_CHAR_COUNT); i++) {
        if (wk->clResources[i] != CL_RES_NONE) {
            func_0204a8d4(wk->clResources[i]);
        }
    }
    for (i = CL_RES_CELL(0); i < CL_RES_CELL(CL_RES_CELL_COUNT); i++) {
        if (wk->clResources[i] != CL_RES_NONE) {
            func_0204afd8(wk->clResources[i]);
        }
    }
}

void func_ov091_021b7698(PokeListWork *wk) {
    switch (wk->param->mode) {
    case 1:
        wk->buttons[0] = func_ov091_021bbdec(wk, wk->menu, 0, 22, 21, FALSE);
        func_ov091_021b7b98(wk);
        wk->nextState = 2;
        break;
    case 22:
    case 23:
        wk->buttons[0] = func_ov091_021bbdec(wk, wk->menu, 0, 12, 21, FALSE);
        wk->buttons[1] = func_ov091_021bbdec(wk, wk->menu, 1, 22, 21, TRUE);
        func_ov091_021b7b98(wk);
        wk->nextState = 2;
        break;
    case 0:
    case 6:
    case 9:
    case 14:
    case 16:
    case 18:
    case 21:
    case 25:
        func_ov091_021b7b98(wk);
        wk->nextState = 2;
        break;
    case 3:
        wk->showShortcutButtons = FALSE;
        func_ov091_021b7b98(wk);
        wk->nextState = 2;
        break;
    case 5:
        if (func_ov091_021bc1b4(wk, wk->param->item) == TRUE) {
            int slot = func_ov091_021bc1d0(wk);

            if (slot != -1) {
                wk->cursorPos = slot;
                wk->pkm = PokeParty_GetPkm(wk->param->party, slot);
                func_ov091_021bc350(wk, 0);
                func_ov091_021be2b4(wk->pkm, wk->param->item, 0, wk->param->zoneId, wk->heapId);
                func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
                wk->playHealSe = TRUE;
                wk->nextState = wk->state;
                wk->state = 0;
                func_ov091_021ba670(wk, wk->param->item);
            } else {
                func_ov091_021bc2c8(wk);
                wk->nextState = wk->state;
                wk->state = 0;
            }
        } else if (func_ov091_021ba65c(wk, wk->param->item) != 0) {
            func_ov091_021b7b98(wk);
            wk->nextState = 2;
        } else {
            wk->param->result = 10;
            func_ov091_021bb7a4(wk, wk->message);
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb9ac(wk, wk->message, 0, wk->param->item);
            func_ov091_021b9788(wk, 0xb3, TRUE, func_ov091_021b9e3c);
            func_ov091_021bb98c(wk, wk->message);
            wk->nextState = wk->state;
            wk->state = 0;
        }
        break;
    case 7:
    case 8:
        wk->cursorPos = wk->param->index;
        wk->pkm = PokeParty_GetPkm(wk->param->party, wk->param->index);
        if (wk->param->moveSlot < 4) {
            u32 move = PokeParty_GetParam(wk->pkm, PKM_PARAM_MOVE1 + wk->param->moveSlot, NULL);

            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9cc(wk, wk->message, 1, move);
            func_ov091_021b9788(wk, 0x28, TRUE, func_ov091_021ba004);
            func_ov091_021bb98c(wk, wk->message);
        } else {
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
            func_ov091_021b9788(wk, 0x23, FALSE, func_ov091_021b9f34);
            func_ov091_021bb98c(wk, wk->message);
        }
        wk->nextState = 7;
        wk->state = 0;
        wk->showShortcutButtons = FALSE;
        func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 1);
        break;
    case 10:
        wk->cursorPos = wk->param->index;
        wk->pkm = PokeParty_GetPkm(wk->param->party, wk->param->index);
        if (wk->param->item == 0) {
            wk->param->mode = 0;
            func_ov091_021b7b98(wk);
            wk->nextState = 2;
        } else {
            u32 heldItem = PokeParty_GetParam(wk->pkm, PKM_PARAM_ITEM, NULL);

            if (heldItem == 0) {
                func_ov091_021ba624(wk, wk->pkm, wk->param->item);
                func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
                func_ov091_021bb97c(wk, wk->message);
                func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
                func_ov091_021bb9ac(wk, wk->message, 1, wk->param->item);
                func_ov091_021bc24c(wk, wk->pkm, wk->param->item);
                func_ov091_021bc288(wk, wk->pkm, wk->param->item);
                if (func_ov091_021bdbf8(wk, wk->pkm) == TRUE) {
                    func_ov091_021bdc3c(wk, wk->pkm);
                    func_ov091_021b9788(wk, 0x56, TRUE, func_ov091_021ba4e4);
                    wk->demo = 1;
                } else {
                    func_ov091_021b9788(wk, 0x56, TRUE, func_ov091_021b9e20);
                }
                func_ov091_021bb98c(wk, wk->message);
                wk->param->mode = 0;
            } else {
                func_ov091_021bb97c(wk, wk->message);
                func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
                func_ov091_021bb9bc(wk, wk->message, 1, heldItem);
                func_ov091_021b9788(wk, 0x3a, FALSE, func_ov091_021ba098);
                func_ov091_021bb98c(wk, wk->message);
            }
        }
        wk->nextState = 7;
        wk->state = 0;
        wk->showShortcutButtons = TRUE;
        func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 1);
        break;
    case 11:
    case 12: {
        u32 heldItem;
        u32 msgId;

        wk->cursorPos = wk->param->index;
        wk->pkm = PokeParty_GetPkm(wk->param->party, wk->param->index);
        heldItem = PokeParty_GetParam(wk->pkm, PKM_PARAM_ITEM, NULL);
        func_ov091_021bb97c(wk, wk->message);
        if (heldItem == 0) {
            msgId = 0x56;
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9ac(wk, wk->message, 1, wk->param->item);
        } else {
            msgId = 0x40;
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9ac(wk, wk->message, 1, heldItem);
            func_ov091_021bb9ac(wk, wk->message, 2, wk->param->item);
        }
        func_ov091_021ba624(wk, wk->pkm, wk->param->item);
        func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
        func_ov091_021bc24c(wk, wk->pkm, wk->param->item);
        func_ov091_021bc288(wk, wk->pkm, wk->param->item);
        if (func_ov091_021bdc5c(wk, wk->pkm) == TRUE) {
            func_ov091_021bdca0(wk, wk->pkm);
            func_ov091_021b9788(wk, msgId, TRUE, func_ov091_021ba4e4);
            wk->demo = 2;
        } else if (wk->param->mode == 11) {
            func_ov091_021b9788(wk, msgId, TRUE, func_ov091_021b9e3c);
        } else {
            func_ov091_021b9788(wk, msgId, TRUE, func_ov091_021b9e20);
        }
        func_ov091_021bb98c(wk, wk->message);
        if (wk->param->mode == 11) {
            wk->param->result = 10;
        } else {
            wk->param->mode = 0;
        }
        wk->nextState = 7;
        wk->state = 0;
        wk->showShortcutButtons = TRUE;
        break;
    }
    }
}


void func_ov091_021b7b98(PokeListWork *wk) {
    GFL_BGSysLoadScr(0);
    switch (wk->param->mode) {
    case 0:
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
        func_ov091_021bb7e8(wk, wk->message, 9);
        wk->showShortcutButtons = TRUE;
        break;
    case 1:
        func_ov091_021bd4ec(wk);
        func_0204b3dc(wk->exitButton, FALSE);
        wk->showShortcutButtons = FALSE;
        break;
    case 22:
    case 23:
        wk->showShortcutButtons = FALSE;
        break;
    case 5:
    case 16:
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
        func_ov091_021bb7e8(wk, wk->message, 0xd);
        wk->showShortcutButtons = FALSE;
        wk->unk28 = FALSE;
        break;
    case 9:
    case 14:
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
        func_ov091_021bb7e8(wk, wk->message, 0xc);
        wk->showShortcutButtons = FALSE;
        wk->unk28 = FALSE;
        break;
    case 6:
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
        func_ov091_021bb7e8(wk, wk->message, 0xe);
        wk->showShortcutButtons = FALSE;
        wk->unk28 = FALSE;
        break;
    case 18:
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
        func_ov091_021bb7e8(wk, wk->message, 0x10);
        wk->showShortcutButtons = FALSE;
        break;
    case 3:
    case 25:
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
        func_ov091_021bb7e8(wk, wk->message, 9);
        wk->showShortcutButtons = FALSE;
        break;
    case 21:
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
        func_ov091_021bb7e8(wk, wk->message, 0xa3);
        func_0204b3dc(wk->exitButton, FALSE);
        wk->showShortcutButtons = FALSE;
        break;
    }
    func_ov091_021b8418(wk);
}

void func_ov091_021b7cc4(PokeListWork *wk) {
    if (wk->param->unk62[1] == 1) {
        wk->state = 19;
        return;
    }
    switch (wk->param->mode) {
    case 0:
    case 1:
    case 18:
    case 21:
    case 22:
    case 23:
    case 25:
        func_ov091_021b907c(wk);
        GFL_BGSysLoadScr(0);
        func_ov091_021b8180(wk);
        break;
    case 5:
        if (func_ov091_021bc17c(wk, wk->param->item) == TRUE) {
            func_ov091_021b907c(wk);
            func_ov091_021b8180(wk);
            break;
        }
        if (func_ov091_021bdd78(wk->pkm, wk->param->item, 0, wk->heapId) == TRUE) {
            int result = func_ov091_021bc350(wk, 0);

            func_ov091_021be2b4(wk->pkm, wk->param->item, 0, wk->param->zoneId, wk->heapId);
            if (result == 2) {
                func_ov091_021bb200(wk, wk->plates[wk->cursorPos], wk->pkm, 0);
            } else {
                if (result == 0x13 || result == 0xd) {
                    func_ov091_021bb200(wk, wk->plates[wk->cursorPos], wk->pkm, 0);
                } else {
                    func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
                }
                GFL_SndSEPlay(SEQ_SE_RECOVERY);
            }
            func_ov091_021ba670(wk, wk->param->item);
        } else if (wk->param->item == 466 && func_ov091_021bdcc0(wk, wk->pkm) == TRUE) {
            func_ov091_021bdd58(wk, wk->pkm);
            wk->param->result = 10;
            wk->state = 14;
            wk->demo = 3;
        } else {
            func_ov091_021bc30c(wk);
        }
        break;
    case 16:
        if (func_ov091_021b9bf4(wk, wk->pkm, wk->param->item) != FALSE) {
            func_ov091_021ba670(wk, wk->param->item);
            wk->state = 19;
            wk->param->index = wk->cursorPos;
            wk->param->result = 8;
        } else {
            func_ov091_021b9788(wk, 0x51, TRUE, func_ov091_021b9e3c);
        }
        break;
    case 9: {
        u32 heldItem = PokeParty_GetParam(wk->pkm, PKM_PARAM_ITEM, NULL);

        if (heldItem == 0) {
            if (PML_ItemIsMail(wk->param->item) == TRUE) {
                wk->state = 19;
                wk->param->index = wk->cursorPos;
                wk->param->result = 6;
                break;
            }
            wk->param->result = 10;
            func_ov091_021ba624(wk, wk->pkm, wk->param->item);
            func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9ac(wk, wk->message, 1, wk->param->item);
            func_ov091_021bc24c(wk, wk->pkm, wk->param->item);
            func_ov091_021bc288(wk, wk->pkm, wk->param->item);
            if (func_ov091_021bdbf8(wk, wk->pkm) == TRUE) {
                func_ov091_021bdc3c(wk, wk->pkm);
                func_ov091_021b9788(wk, 0x56, TRUE, func_ov091_021ba4e4);
                wk->demo = 1;
            } else {
                func_ov091_021b9788(wk, 0x56, TRUE, func_ov091_021b9e3c);
            }
            func_ov091_021bb98c(wk, wk->message);
        } else if (PML_ItemIsMail(heldItem) == TRUE) {
            wk->param->result = 10;
            func_ov091_021b9788(wk, 0x39, TRUE, func_ov091_021b9e3c);
        } else {
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9bc(wk, wk->message, 1, heldItem);
            func_ov091_021b9788(wk, 0x3a, FALSE, func_ov091_021ba098);
            func_ov091_021bb98c(wk, wk->message);
        }
        break;
    }
    case 14:
        if (PokeParty_GetParam(wk->pkm, PKM_PARAM_ITEM, NULL) == 0) {
            MailData *mail = func_02009050(wk->param->unk08, 0, wk->param->mailSlot, wk->heapId);

            PokeParty_SetParam(wk->pkm, PKM_PARAM_MAIL, (uintptr_t)mail);
            GFL_HeapFree(mail);
            PokeParty_SetParam(wk->pkm, PKM_PARAM_ITEM, wk->param->item);
            func_0200902c(wk->param->unk08, 0, wk->param->mailSlot);
            func_ov091_021b9788(wk, 0x62, TRUE, func_ov091_021b9e3c);
        } else {
            func_ov091_021b9788(wk, 0x63, TRUE, func_ov091_021b9e3c);
        }
        break;
    case 6:
        switch (func_ov091_021b9ac8(wk, wk->pkm, wk->param->index)) {
        case 0:
            wk->param->result = 10;
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
            func_ov091_021b9788(wk, 0x29, TRUE, func_ov091_021b9e3c);
            func_ov091_021bb98c(wk, wk->message);
            func_ov091_021ba5b4(wk, wk->pkm);
            func_ov091_021ba60c(wk, wk->pkm);
            break;
        case 1:
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
            func_ov091_021b9788(wk, 0x20, FALSE, func_ov091_021b9eb0);
            func_ov091_021bb98c(wk, wk->message);
            break;
        case 2:
            wk->param->result = 10;
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
            func_ov091_021b9788(wk, 0x2a, TRUE, func_ov091_021b9e3c);
            func_ov091_021bb98c(wk, wk->message);
            break;
        case 3:
            wk->param->result = 10;
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
            func_ov091_021b9788(wk, 0x2b, TRUE, func_ov091_021b9e3c);
            func_ov091_021bb98c(wk, wk->message);
            break;
        }
        break;
    case 3:
        wk->state = 19;
        wk->param->index = wk->cursorPos;
        wk->param->result = 0;
        break;
    default:
        func_ov091_021b907c(wk);
        func_ov091_021b8180(wk);
        break;
    }
}


void func_ov091_021b8180(PokeListWork *wk) {
    u32 items[8];

    GFL_BGSysLoadScr(0);
    switch (wk->param->mode) {
    case 0:
        if (func_ov091_021bb468(wk, wk->plates[wk->cursorPos]) == TRUE) {
            items[0] = 0;
            items[1] = 3;
            items[2] = 6;
            items[3] = 0x10;
        } else {
            items[0] = 0;
            items[1] = 1;
            items[2] = 3;
            items[3] = 4;
            items[4] = 6;
            items[5] = 0x10;
            if (PML_ItemIsMail(PokeParty_GetParam(wk->pkm, PKM_PARAM_ITEM, NULL)) == TRUE) {
                items[3] = 5;
            }
        }
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
        func_ov091_021bb7e8(wk, wk->message, 0x12);
        func_ov091_021bb98c(wk, wk->message);
        break;
    case 1:
    case 22:
    case 23:
        if (func_ov091_021bb468(wk, wk->plates[wk->cursorPos]) == TRUE) {
            items[0] = 0;
            items[1] = 6;
            items[2] = 0x10;
        } else {
            items[0] = 8;
            items[1] = 0;
            items[2] = 6;
            items[3] = 0x10;
        }
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
        func_ov091_021bb7e8(wk, wk->message, 0x12);
        func_ov091_021bb98c(wk, wk->message);
        break;
    case 5:
        items[0] = 2;
        items[1] = 6;
        items[2] = 0x10;
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
        func_ov091_021bb7e8(wk, wk->message, func_ov091_021bc228(wk, wk->param->item));
        func_ov091_021bb98c(wk, wk->message);
        break;
    case 18:
        if (func_ov091_021bb468(wk, wk->plates[wk->cursorPos]) == TRUE) {
            items[0] = 0;
            items[1] = 6;
            items[2] = 0x10;
        } else {
            items[0] = 7;
            items[1] = 0;
            items[2] = 6;
            items[3] = 0x10;
        }
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
        func_ov091_021bb7e8(wk, wk->message, 0x12);
        func_ov091_021bb98c(wk, wk->message);
        break;
    case 21:
        if (func_ov091_021bb468(wk, wk->plates[wk->cursorPos]) == TRUE) {
            items[0] = 0xb;
            items[1] = 0;
            items[2] = 6;
            items[3] = 0x10;
            func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
            func_ov091_021bb7e8(wk, wk->message, 0xa2);
        } else {
            items[0] = 0;
            items[1] = 6;
            items[2] = 0x10;
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
            func_ov091_021bb7e8(wk, wk->message, 0x12);
            func_ov091_021bb98c(wk, wk->message);
        }
        break;
    case 25:
        if (func_ov112_021f6744(wk->pkm) == TRUE) {
            items[0] = 0xb;
            items[1] = 0;
            items[2] = 6;
            items[3] = 0x10;
        } else {
            items[0] = 0;
            items[1] = 6;
            items[2] = 0x10;
        }
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
        func_ov091_021bb7e8(wk, wk->message, 0x12);
        func_ov091_021bb98c(wk, wk->message);
        break;
    default:
        items[0] = 0;
        items[1] = 6;
        items[2] = 0x10;
        break;
    }
    func_ov091_021bbaf8(wk, wk->menu, items);
}




void func_ov091_021b83d8(PokeListWork *wk) {
    switch (wk->subState) {
    case 0:
        wk->subState = 1;
        wk->input = 4;
        break;
    case 1:
        func_ov091_021b8a8c(wk);
        break;
    case 2:
        if (wk->state == 3) {
            func_ov091_021b86d8(wk);
        } else if (wk->state == 4) {
            func_ov091_021b8758(wk);
        } else {
            func_ov091_021b8508(wk);
        }
        break;
    }
}

void func_ov091_021b8418(PokeListWork *wk) {
    wk->cursorPos = wk->param->index;
    wk->subState = 0;
    wk->menuItem = 0x17;
    if (func_ov091_021b9a84(wk) == TRUE) {
        func_ov091_021bbe5c(wk->buttons[0], FALSE);
        func_ov091_021bbe5c(wk->buttons[1], FALSE);
    }
    if (wk->touch == FALSE) {
        if (wk->cursorPos <= 5) {
            func_ov091_021b8f68(wk, wk->cursorPos);
            func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 1);
        } else {
            func_ov091_021bbe5c(wk->buttons[(u8)(wk->cursorPos - 6)], TRUE);
        }
    } else {
        func_0204b3dc(wk->cursor, FALSE);
        if (wk->cursorPos <= 5) {
            if (wk->state == 3) {
                func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 1);
            } else {
                func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 0);
            }
        } else {
            func_ov091_021bbe5c(wk->buttons[(u8)(wk->cursorPos - 6)], FALSE);
        }
    }
    func_0204b3dc(wk->shortcutButton, wk->showShortcutButtons);
    if (func_ov091_021b9a84(wk) == TRUE || func_ov091_021b9ab4(wk) == FALSE) {
        func_0204b3dc(wk->exitButton, FALSE);
    } else {
        func_0204b3dc(wk->exitButton, TRUE);
    }
}

void func_ov091_021b8508(PokeListWork *wk) {
    func_ov091_021bb7a4(wk, wk->message);
    if (func_ov091_021b9a84(wk) == TRUE) {
        u8 i;

        GFL_BGSysFillScrArea(0, 0, 0, 21, 32, 3, BGSYS_FILL_KEEP_PALETTE);
        for (i = 0; i < POKELIST_PLATE_COUNT; i++) {
            wk->param->picked[i] = 0;
        }
        for (i = 0; i < POKELIST_PLATE_COUNT; i++) {
            int entry = func_ov091_021bb43c(wk->plates[i]);

            if (entry <= 5) {
                wk->param->picked[entry] = i + 1;
            }
        }
    }
    switch (wk->input) {
    case 3:
        func_0204b3dc(wk->cursor, FALSE);
        wk->pkm = PokeParty_GetPkm(wk->param->party, wk->cursorPos);
        wk->param->index = wk->cursorPos;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov091_021b7cc4(wk);
        break;
    case 0:
        if (func_ov091_021b9a84(wk) == TRUE) {
            func_ov091_021b892c(wk);
            break;
        }
        wk->state = 19;
        wk->param->index = 6;
        wk->param->result = 0;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        break;
    case 1:
        if (wk->cursorPos <= 5) {
            func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 0);
            func_0204b3dc(wk->cursor, FALSE);
        }
        if (func_ov091_021b9a84(wk) == TRUE) {
            func_ov091_021bbe5c(wk->buttons[0], FALSE);
            func_ov091_021bbe68(wk->buttons[1], TRUE);
            wk->state = 12;
            wk->cursorPos = 7;
        } else {
            wk->state = 19;
            wk->pressedButton = wk->exitButton;
            func_0204b878(wk->pressedButton, 9);
        }
        wk->param->index = 7;
        wk->param->result = 0;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        break;
    case 2:
        if (wk->cursorPos <= 5) {
            func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 0);
            func_0204b3dc(wk->cursor, FALSE);
        }
        wk->state = 19;
        wk->param->index = 8;
        wk->param->result = 0;
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        wk->pressedButton = wk->shortcutButton;
        func_0204b878(wk->pressedButton, 8);
        break;
    }
}



void func_ov091_021b86d8(PokeListWork *wk) {
    func_ov091_021bb7a4(wk, wk->message);
    if (wk->input == 3 && wk->cursorPos == wk->selectPos) {
        wk->input = 1;
    }
    switch (wk->input) {
    case 3:
        func_ov091_021b9968(wk);
        break;
    case 1:
        wk->pkm = NULL;
        wk->state = 2;
        func_ov091_021b8f68(wk, wk->cursorPos);
        func_ov091_021bafa0(wk, wk->plates[wk->selectPos], 0);
        func_0204b3dc(wk->subCursor, FALSE);
        wk->selectPos = 9;
        wk->param->index = wk->cursorPos;
        func_ov091_021b7b98(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        break;
    }
}

void func_ov091_021b8758(PokeListWork *wk) {
    func_ov091_021bb7a4(wk, wk->message);
    if (wk->input == 3) {
        u32 hp;
        u32 maxHp;

        wk->pkm = PokeParty_GetPkm(wk->param->party, wk->cursorPos);
        hp = PokeParty_GetParam(wk->pkm, PKM_PARAM_HP, NULL);
        maxHp = PokeParty_GetParam(wk->pkm, PKM_PARAM_MAX_HP, NULL);
        if (wk->cursorPos == wk->selectPos2 || hp == 0 || maxHp == hp) {
            func_ov091_021b9788(wk, 0x64, TRUE, func_ov091_021b9e20);
            func_ov091_021b8f68(wk, wk->cursorPos);
            func_ov091_021bafa0(wk, wk->plates[wk->selectPos2], 0);
            func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 0);
            func_0204b3dc(wk->subCursor, FALSE);
            wk->param->index = wk->cursorPos;
        } else {
            PartyPkm *user;
            u32 userMaxHp;
            u32 userHp;
            u32 share;

            func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
            wk->pkm = PokeParty_GetPkm(wk->param->party, wk->cursorPos);
            wk->state = 10;
            wk->prevHp = func_ov091_021bb464(wk, wk->plates[wk->cursorPos]);
            wk->hpDoneFunc = func_ov091_021ba288;
            func_ov091_021b8f68(wk, wk->cursorPos);
            func_ov091_021bafa0(wk, wk->plates[wk->selectPos2], 0);
            func_0204b3dc(wk->subCursor, FALSE);
            user = PokeParty_GetPkm(wk->param->party, wk->selectPos2);
            userMaxHp = PokeParty_GetParam(user, PKM_PARAM_MAX_HP, NULL);
            userHp = PokeParty_GetParam(user, PKM_PARAM_HP, NULL);
            share = userMaxHp / 5;
            if (hp + share > maxHp) {
                PokeParty_SetParam(user, PKM_PARAM_HP, userHp - (maxHp - hp));
            } else {
                PokeParty_SetParam(user, PKM_PARAM_HP, userHp - share);
            }
            wk->param->index = wk->cursorPos;
            wk->cursorPos = wk->selectPos2;
            func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
            GFL_SndSEPlay(0x56f);
        }
    } else {
        wk->pkm = NULL;
        wk->state = 2;
        func_ov091_021b8f68(wk, wk->cursorPos);
        func_ov091_021bafa0(wk, wk->plates[wk->selectPos2], 0);
        func_0204b3dc(wk->subCursor, FALSE);
        wk->selectPos2 = 9;
        wk->param->index = wk->cursorPos;
        func_ov091_021b7b98(wk);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
    }
}


static inline void ClearPicked(u8 *picked) {
    u8 i;

    for (i = 0; i < 6; i++) {
        picked[i] = 0;
    }
}

void func_ov091_021b892c(PokeListWork *wk) {
    Regulation *regulation = wk->param->regulation;
    s8 picked[6];
    u8 *p;
    u8 i;
    u32 result;

    if (wk->cursorPos != 6) {
        if (wk->cursorPos <= 5) {
            func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 0);
            func_0204b3dc(wk->cursor, FALSE);
        }
        wk->cursorPos = 6;
    }
    wk->param->index = 6;
    p = picked;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    for (i = 0; i < POKELIST_PLATE_COUNT; i++) {
        int entry = func_ov091_021bb43c(wk->plates[i]);

        if (entry <= 5) {
            p[entry] = i + 1;
        }
    }
    result = func_02019e54(regulation, wk->param->party, picked);
    if (regulation->unk2 > wk->enteredCount) {
        func_ov091_021b9788(wk, regulation->unk2 + 0x56, TRUE, func_ov091_021b9e20);
        GFL_SndSEPlay(SEQ_SE_BEEP);
        return;
    }
    if (regulation->unk3 < wk->enteredCount) {
        func_ov091_021b9788(wk, regulation->unk3 + 0x5c, TRUE, func_ov091_021b9e20);
        GFL_SndSEPlay(SEQ_SE_BEEP);
        return;
    }
    if (result == 1) {
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb9ec(wk, wk->message, 0, regulation->unk6, 3);
        func_ov091_021b9788(wk, 0x92, TRUE, func_ov091_021b9e20);
        GFL_SndSEPlay(SEQ_SE_BEEP);
        func_ov091_021bb98c(wk, wk->message);
    } else if (result == 7) {
        func_ov091_021b9788(wk, 0x94, TRUE, func_ov091_021b9e20);
        GFL_SndSEPlay(SEQ_SE_BEEP);
    } else {
        func_ov091_021bbe5c(wk->buttons[1], FALSE);
        func_ov091_021bbe68(wk->buttons[0], TRUE);
        wk->state = 12;
        wk->param->index = 6;
        wk->param->result = 0;
        wk->decided = TRUE;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    }
}




void func_ov091_021b8a8c(PokeListWork *wk) {
    func_ov091_021b8ac4(wk);
    if (wk->input == 4) {
        func_ov091_021b8db4(wk);
    }
    if (wk->input != 4) {
        wk->subState = 2;
    }
    if (wk->param->unk62[1] == 1) {
        wk->subState = 2;
        wk->input = 1;
    }
}

void func_ov091_021b8ac4(PokeListWork *wk) {
    u32 pressed = GCTX_HIDGetPressedKeys();
    u32 typed = GCTX_HIDGetTypedKeys();
    int dir;

    if (wk->touch == TRUE && func_0204b3f4(wk->cursor) == 0) {
        if (pressed != 0 && !(pressed & (PAD_BUTTON_B | PAD_BUTTON_X))) {
            if (wk->cursorPos <= 5) {
                func_ov091_021b8f68(wk, wk->cursorPos);
                func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 1);
            } else {
                func_ov091_021bbe5c(wk->buttons[(u8)(wk->cursorPos - 6)], TRUE);
                func_0204b3dc(wk->cursor, FALSE);
            }
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            wk->touch = FALSE;
        }
    } else {
        dir = 0;
        if (pressed != 0) {
            wk->touch = FALSE;
        }
        if (typed & PAD_KEY_UP) {
            dir = -2;
        } else if (typed & PAD_KEY_DOWN) {
            dir = 2;
        } else if (typed & PAD_KEY_LEFT) {
            dir = -1;
        } else if (typed & PAD_KEY_RIGHT) {
            dir = 1;
        } else if (pressed & PAD_BUTTON_A) {
            if (wk->cursorPos == 6) {
                wk->input = 0;
            } else if (wk->cursorPos == 7) {
                wk->input = 1;
            } else if (wk->unk28 == FALSE && func_ov091_021bb468(wk, wk->plates[wk->cursorPos]) == TRUE) {
                GFL_SndSEPlay(SEQ_SE_BEEP);
            } else {
                wk->input = 3;
            }
        }
        if (dir != 0) {
            int prevPos = wk->cursorPos;
            BOOL found = FALSE;
            int last;

            if (func_ov091_021b9a84(wk) == TRUE) {
                if (func_ov091_021b9aa0(wk) == TRUE) {
                    last = 7;
                } else {
                    last = 6;
                }
            } else {
                last = 5;
            }
            if (wk->cursorPos - 6 <= 1u && (typed & (PAD_KEY_UP | PAD_KEY_DOWN))) {
                u8 count = PokeParty_GetPkmCount(wk->param->party);

                if (typed & PAD_KEY_UP) {
                    if (wk->cursorPos == 7 || func_ov091_021b9aa0(wk) == FALSE) {
                        wk->cursorPos = data_ov091_021beab0[count - 1];
                    } else {
                        wk->cursorPos = data_ov091_021beaaa[count - 1];
                    }
                } else if (wk->cursorPos == 7 || func_ov091_021b9aa0(wk) == FALSE) {
                    if (count > 1) {
                        wk->cursorPos = 1;
                    } else {
                        wk->cursorPos = 0;
                    }
                } else {
                    wk->cursorPos = 0;
                }
            } else {
                do {
                    int newPos = wk->cursorPos + dir;

                    if (newPos > last) {
                        if (last == 6 && dir > 1 && prevPos != 6) {
                            wk->cursorPos = last;
                        } else {
                            wk->cursorPos = newPos - (last + 1);
                        }
                    } else if (newPos < 0) {
                        if (last == 6 && dir < 1) {
                            wk->cursorPos = last;
                        } else {
                            wk->cursorPos = wk->cursorPos + (last + 1) + dir;
                        }
                    } else {
                        wk->cursorPos += dir;
                    }
                    if (wk->cursorPos > 5 || func_ov091_021bb3c4(wk, wk->plates[wk->cursorPos]) == TRUE) {
                        found = TRUE;
                    }
                } while (!found);
            }
            if (prevPos != wk->cursorPos) {
                if (wk->cursorPos <= 5) {
                    func_ov091_021b8f68(wk, wk->cursorPos);
                    func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 1);
                } else {
                    func_ov091_021bbe5c(wk->buttons[(u8)(wk->cursorPos - 6)], TRUE);
                    func_0204b3dc(wk->cursor, FALSE);
                }
                if (prevPos <= 5) {
                    func_ov091_021bafa0(wk, wk->plates[prevPos], 0);
                } else {
                    func_ov091_021bbe5c(wk->buttons[(u8)(prevPos - 6)], FALSE);
                }
                wk->flashTimer = 12;
            }
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        }
    }
    if (pressed & PAD_BUTTON_B) {
        if (func_ov091_021b9a84(wk) == FALSE || func_ov091_021b9aa0(wk) == TRUE) {
            if (func_ov091_021b9ab4(wk) == TRUE) {
                wk->input = 1;
                wk->pressedButton = wk->exitButton;
                func_0204b878(wk->pressedButton, 9);
                wk->touch = FALSE;
            }
        }
    } else if (pressed & PAD_BUTTON_X) {
        if (wk->showShortcutButtons == TRUE) {
            wk->input = 2;
            wk->pressedButton = wk->shortcutButton;
            func_0204b878(wk->pressedButton, 8);
            wk->touch = FALSE;
        }
    }
}


void func_ov091_021b8db4(PokeListWork *wk) {
    TouchRect plateRects[7];
    u8 slots[6];
    u8 count = 0;
    u8 i;
    s32 hit;

    for (i = 0; i < POKELIST_PLATE_COUNT; i++) {
        if (func_ov091_021bb3c4(wk, wk->plates[i]) == TRUE) {
            slots[count] = i;
            func_ov091_021bb3fc(wk, wk->plates[i], &plateRects[count]);
            count++;
        }
    }
    plateRects[count].top = TOUCH_RECT_END;
    hit = func_02035c08(plateRects);
    if (hit != TOUCH_RECT_NONE) {
        if (wk->unk28 == FALSE && hit <= 5 && func_ov091_021bb468(wk, wk->plates[hit]) == TRUE) {
            GFL_SndSEPlay(SEQ_SE_BEEP);
        } else {
            s32 prevPos = wk->cursorPos;

            wk->input = 3;
            wk->cursorPos = slots[hit];
            if (prevPos <= 5) {
                func_ov091_021bafa0(wk, wk->plates[prevPos], 0);
            } else {
                func_ov091_021bbe5c(wk->buttons[(u8)(prevPos - 6)], FALSE);
            }
            if (wk->cursorPos <= 5) {
                func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 1);
            } else {
                func_ov091_021bbe5c(wk->buttons[(u8)(wk->cursorPos - 6)], TRUE);
                func_0204b3dc(wk->cursor, FALSE);
            }
            wk->flashTimer = 12;
        }
        wk->touch = TRUE;
    }
    if (hit == TOUCH_RECT_NONE) {
        TouchRect buttonRects[4] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { TOUCH_RECT_END, 0, 0, 0 } };

        if (func_ov091_021b9a84(wk) == TRUE) {
            if (func_ov091_021b9aa0(wk) == TRUE) {
                buttonRects[1].top = 168;
                buttonRects[1].bottom = 192;
                buttonRects[1].left = 176;
                buttonRects[1].right = 255;
                buttonRects[0].top = 168;
                buttonRects[0].bottom = 192;
                buttonRects[0].left = 96;
                buttonRects[0].right = 176;
            } else {
                buttonRects[0].top = 168;
                buttonRects[0].bottom = 192;
                buttonRects[0].left = 176;
                buttonRects[0].right = 255;
            }
        } else {
            if (func_ov091_021b9ab4(wk) == TRUE) {
                buttonRects[1].top = 168;
                buttonRects[1].bottom = 192;
                buttonRects[1].left = 224;
                buttonRects[1].right = 248;
            }
            if (wk->showShortcutButtons == TRUE) {
                buttonRects[2].top = 168;
                buttonRects[2].bottom = 192;
                buttonRects[2].left = 200;
                buttonRects[2].right = 224;
            }
        }
        hit = func_02035c08(buttonRects);
        if (hit != TOUCH_RECT_NONE) {
            wk->input = hit;
            wk->touch = TRUE;
        }
    }
}


void func_ov091_021b8f68(PokeListWork *wk, s32 pos) {
    ClActorPos actorPos;

    func_ov091_021bb3d4(wk, wk->plates[pos], &actorPos);
    func_0204b404(wk->cursor, &actorPos, 0);
    func_0204b3dc(wk->cursor, TRUE);
    func_0204b84c(wk->cursor, 3);
    if (pos == 0) {
        func_0204b878(wk->cursor, 0);
    } else {
        func_0204b878(wk->cursor, 1);
    }
}

void func_ov091_021b8fc4(PokeListWork *wk, s32 pos) {
    ClActorPos actorPos;

    func_ov091_021bb3d4(wk, wk->plates[pos], &actorPos);
    func_0204b404(wk->subCursor, &actorPos, 0);
    func_0204b3dc(wk->subCursor, TRUE);
    func_0204b84c(wk->subCursor, 2);
    if (pos == 0) {
        func_0204b878(wk->subCursor, 2);
    } else {
        func_0204b878(wk->subCursor, 3);
    }
}

void func_ov091_021b9020(PokeListWork *wk) {
    switch (wk->subState) {
    case 0:
        func_ov091_021b907c(wk);
        wk->subState = 1;
        break;
    case 1: {
        u32 item;

        func_ov091_021bbbd8(wk, wk->menu);
        item = func_ov091_021bbbe4(wk, wk->menu);
        if (item != 0x17) {
            wk->menuItem = item;
            wk->subState = 2;
        }
        if (wk->param->unk62[1] == 1) {
            wk->subState = 2;
        }
        break;
    }
    case 2:
        func_ov091_021b90f8(wk);
        func_ov091_021b916c(wk);
        break;
    }
}

void func_ov091_021b907c(PokeListWork *wk) {
    wk->state = 6;
    wk->subState = 0;
    G2_SetBlendBrightness(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, 8);
    G2_SetWnd0Position(224, 168, 248, 192);
    G2_SetWnd0InsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ,
                          FALSE);
    G2_SetWndOutsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ,
                          TRUE);
    wk->glowPaused = TRUE;
    func_0204b3dc(wk->shortcutButton, FALSE);
    func_0204b3dc(wk->exitButton, FALSE);
    GFL_BGSysQueueScrLoad(0);
    func_020355d8(wk->touch);
}

void func_ov091_021b90f8(PokeListWork *wk) {
    func_ov091_021bbba0(wk, wk->menu);
    func_ov091_021bb7a4(wk, wk->message);
    if (func_ov091_021b9a84(wk) == TRUE || func_ov091_021b9ab4(wk) == FALSE) {
        func_0204b3dc(wk->exitButton, FALSE);
    } else {
        func_0204b3dc(wk->exitButton, TRUE);
    }
    if (wk->menuItem != 4 && wk->menuItem != 5) {
        G2_SetBlendAlpha(GX_PLANEMASK_BG2 | GX_PLANEMASK_OBJ, GX_PLANEMASK_BG3, 16, 10);
        wk->glowPaused = FALSE;
    }
    GFL_BGSysFillScrAsync(0, 0);
    wk->touch = func_020355c4();
}

void func_ov091_021b916c(PokeListWork *wk) {
    if (wk->param->unk62[1] == 1) {
        wk->state = 19;
        return;
    }
    switch (wk->menuItem) {
    case 6:
        func_ov091_021b8f68(wk, wk->cursorPos);
        wk->pkm = NULL;
        wk->state = 2;
        func_ov091_021b7b98(wk);
        break;
    case 3:
        func_ov091_021b8f68(wk, wk->cursorPos);
        func_ov091_021b8fc4(wk, wk->cursorPos);
        wk->selectPos = wk->cursorPos;
        wk->state = 3;
        wk->pkm = NULL;
        wk->showShortcutButtons = FALSE;
        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
        func_ov091_021bb7e8(wk, wk->message, 0xb);
        func_ov091_021b8418(wk);
        break;
    case 4: {
            u32 items[4] = { 9, 10, 6, 0x10 };

            func_ov091_021b907c(wk);
            func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
            func_ov091_021bb7e8(wk, wk->message, 0x13);
            func_ov091_021bbaf8(wk, wk->menu, items);
            GFL_BGSysLoadScr(0);
            break;
        }
    case 5: {
            u32 items[4] = { 12, 13, 6, 0x10 };

            func_ov091_021b907c(wk);
            func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_MENU);
            func_ov091_021bb7e8(wk, wk->message, 0x13);
            func_ov091_021bbaf8(wk, wk->menu, items);
            GFL_BGSysLoadScr(0);
            break;
        }
    case 0:
        wk->state = 19;
        wk->param->index = wk->cursorPos;
        wk->param->result = 1;
        break;
    case 20: {
            Regulation *regulation = wk->param->regulation;
            BOOL full = FALSE;
            int status = func_ov091_021bb4a0(wk, wk->plates[wk->cursorPos]);

            if (status == 0) {
                func_ov091_021bb440(wk, wk->plates[wk->cursorPos], wk->enteredCount);
                wk->enteredCount++;
                func_ov091_021b7b98(wk);
                if (wk->enteredCount == regulation->unk3) {
                    full = TRUE;
                }
                wk->state = 2;
            } else if (status == 2) {
                func_ov091_021b9788(wk, 0x90, TRUE, func_ov091_021b9e20);
                GFL_SndSEPlay(SEQ_SE_BEEP);
            } else if (status == 3) {
                func_ov091_021b9788(wk, 0x91, TRUE, func_ov091_021b9e20);
                GFL_SndSEPlay(SEQ_SE_BEEP);
            } else if (status == 4) {
                func_ov091_021b9788(wk, regulation->unk3 + 0x5c, TRUE, func_ov091_021b9e20);
                GFL_SndSEPlay(SEQ_SE_BEEP);
            }
            wk->pkm = NULL;
            if (full == TRUE) {
                func_ov091_021bafa0(wk, wk->plates[wk->cursorPos], 0);
                wk->cursorPos = 6;
                if (wk->touch == FALSE) {
                    func_ov091_021bbe5c(wk->buttons[(u8)(wk->cursorPos - 6)], TRUE);
                    func_0204b3dc(wk->cursor, FALSE);
                }
            } else if (wk->touch == FALSE) {
                func_ov091_021b8f68(wk, wk->cursorPos);
            }
            break;
        }
    case 21: {
            int entry = func_ov091_021bb43c(wk->plates[wk->cursorPos]);
            u8 i;

            for (i = 0; i < POKELIST_PLATE_COUNT; i++) {
                int other = func_ov091_021bb43c(wk->plates[i]);

                if (other <= 5 && other > entry) {
                    func_ov091_021bb440(wk, wk->plates[i], other - 1);
                }
            }
            func_ov091_021bb440(wk, wk->plates[wk->cursorPos], POKELIST_ENTRY_ABLE);
            wk->enteredCount--;
            if (wk->touch == FALSE) {
                func_ov091_021b8f68(wk, wk->cursorPos);
            }
            wk->pkm = NULL;
            wk->state = 2;
            func_ov091_021b7b98(wk);
            break;
        }
    case 10: {
            u32 item = PokeParty_GetParam(wk->pkm, PKM_PARAM_ITEM, NULL);

            func_ov091_021bc24c(wk, wk->pkm, 0);
            func_ov091_021bc288(wk, wk->pkm, 0);
            if (item == 0) {
                func_ov091_021bb97c(wk, wk->message);
                func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
                func_ov091_021b9788(wk, 0x3d, TRUE, func_ov091_021b9e20);
                func_ov091_021bb98c(wk, wk->message);
                break;
            }
            if (func_ov091_021ba65c(wk, item) == 999) {
                func_ov091_021b9788(wk, 0x3f, TRUE, func_ov091_021b9e20);
                break;
            }
            func_ov091_021ba624(wk, wk->pkm, 0);
            func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9ac(wk, wk->message, 1, item);
            if (func_ov091_021bdc5c(wk, wk->pkm) == TRUE) {
                func_ov091_021bdca0(wk, wk->pkm);
                func_ov091_021b9788(wk, 0x3e, TRUE, func_ov091_021ba4e4);
                wk->demo = 2;
            } else {
                func_ov091_021b9788(wk, 0x3e, TRUE, func_ov091_021b9e20);
            }
            func_ov091_021bb98c(wk, wk->message);
            break;
        }
    case 11:
        wk->state = 19;
        wk->param->index = wk->cursorPos;
        wk->param->result = 0;
        break;
    case 9:
        wk->state = 19;
        wk->param->index = wk->cursorPos;
        wk->param->result = 3;
        break;
    case 12:
        wk->state = 19;
        wk->param->index = wk->cursorPos;
        wk->param->result = 7;
        break;
    case 13:
        func_ov091_021b9788(wk, 0x17, FALSE, func_ov091_021ba394);
        break;
    case 7:
        wk->state = 19;
        wk->param->index = wk->cursorPos;
        wk->param->result = 0;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        if (wk->param->mode == 5) {
            if (func_ov091_021bdd78(wk->pkm, wk->param->item, wk->menuItem - 16, wk->heapId) == TRUE) {
                u32 move = PokeParty_GetParam(wk->pkm, PKM_PARAM_MOVE1 + wk->menuItem - 16, NULL);

                GFL_BGSysLoadScr(0);
                func_ov091_021bc350(wk, move);
                func_ov091_021be2b4(wk->pkm, wk->param->item, wk->menuItem - 16, wk->param->zoneId, wk->heapId);
                func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
                func_ov091_021ba670(wk, wk->param->item);
                GFL_SndSEPlay(SEQ_SE_RECOVERY);
            } else {
                func_ov091_021bc30c(wk);
            }
        } else {
            u32 hiden = func_ov091_021b99e4(wk->pkm, wk->menuItem - 16);

            if (hiden - 22 <= 1) {
                if (func_02013e04(wk->param->zoneId) == FALSE) {
                    func_ov091_021b9788(wk, 0x50, TRUE, func_ov091_021b9e20);
                } else {
                    u32 hp = 0;
                    u32 maxHp;

                    maxHp = PokeParty_GetParam(wk->pkm, PKM_PARAM_MAX_HP, NULL);
                    hp = PokeParty_GetParam(wk->pkm, PKM_PARAM_HP, NULL);

                    if (hp > maxHp / 5) {
                        func_ov091_021b8f68(wk, wk->cursorPos);
                        func_ov091_021b8fc4(wk, wk->cursorPos);
                        wk->selectPos2 = wk->cursorPos;
                        wk->state = 4;
                        wk->pkm = NULL;
                        wk->state = 4;
                        wk->showShortcutButtons = FALSE;
                        func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_SHORT);
                        func_ov091_021bb7e8(wk, wk->message, 0x11);
                        func_ov091_021b8418(wk);
                    } else {
                        func_ov091_021b9788(wk, 0x6b, TRUE, func_ov091_021b9e20);
                    }
                }
            } else {
                switch (CheckAllowHidenEvent(hiden - 11, &wk->param->action)) {
                case 0:
                    wk->state = 19;
                    wk->param->index = wk->cursorPos;
                    wk->param->result = hiden;
                    break;
                case 1:
                    func_ov091_021b9788(wk, 0x50, TRUE, func_ov091_021b9e20);
                    break;
                case 2:
                    func_ov091_021b9788(wk, 0x38, TRUE, func_ov091_021b9e20);
                    break;
                case 3:
                    func_ov091_021b9788(wk, 0x9f, TRUE, func_ov091_021b9e20);
                    break;
                case 4:
                    func_ov091_021b9788(wk, 0x4f, TRUE, func_ov091_021b9e20);
                    break;
                }
            }
        }
        break;
    default:
        func_ov091_021b8f68(wk, wk->cursorPos);
        wk->pkm = NULL;
        wk->state = 2;
        func_ov091_021b7b98(wk);
        break;
    }
}








void func_ov091_021b9750(PokeListWork *wk) {
    if (func_ov091_021bb8fc(wk, wk->message) == 1) {
        if (GCTX_HIDGetPressedKeys() & 1) {
            wk->touch = FALSE;
        }
        if (func_02035c60() == 1) {
            wk->touch = TRUE;
        }
        wk->msgDoneFunc(wk);
    }
}

void func_ov091_021b9788(PokeListWork *wk, u32 msgId, BOOL waitInput, void (*doneFunc)(PokeListWork *wk)) {
    wk->state = 7;
    wk->msgWaitInput = waitInput;
    wk->msgDoneFunc = doneFunc;
    func_ov091_021bb714(wk, wk->message, POKELIST_MESSAGE_WINDOW_WIDE);
    func_ov091_021bb860(wk, wk->message, msgId, waitInput);
    func_0204b3dc(wk->exitButton, FALSE);
    func_0204b3dc(wk->shortcutButton, FALSE);
}

void func_ov091_021b97d4(PokeListWork *wk) {
    u32 item;

    func_ov091_021bbbd8(wk, wk->menu);
    item = func_ov091_021bbbe4(wk, wk->menu);
    if (item != 0x17) {
        wk->menuFunc(wk, item);
        func_ov091_021bbba0(wk, wk->menu);
    }
}






void func_ov091_021b9804(PokeListWork *wk, void (*menuFunc)(PokeListWork *wk, u32 item)) {
    wk->state = 9;
    wk->menuFunc = menuFunc;
    func_ov091_021bbb40(wk, wk->menu);
}

void func_ov091_021b981c(PokeListWork *wk) {
    switch (wk->subState) {
    case 3:
        func_ov091_021bb1a0(wk, wk->plates[wk->cursorPos], wk->unk104);
        func_ov091_021bb1a0(wk, wk->plates[wk->selectPos], wk->unk104);
        wk->unk104++;
        func_ov091_021bb0c4(wk, wk->plates[wk->cursorPos], wk->unk104);
        func_ov091_021bb0c4(wk, wk->plates[wk->selectPos], wk->unk104);
        if (wk->unk104 >= 16) {
            PokeParty_SwapPkms(wk->param->party, wk->cursorPos, wk->selectPos, wk->heapId);
            func_ov091_021bb200(wk, wk->plates[wk->cursorPos], PokeParty_GetPkm(wk->param->party, wk->cursorPos),
                                wk->unk104);
            func_ov091_021bb200(wk, wk->plates[wk->selectPos], PokeParty_GetPkm(wk->param->party, wk->selectPos),
                                wk->unk104);
            wk->subState = 4;
            GFL_SndSEPlay(SEQ_SE_SYS_03);
        }
        break;
    case 4:
        func_ov091_021bb1a0(wk, wk->plates[wk->cursorPos], wk->unk104);
        func_ov091_021bb1a0(wk, wk->plates[wk->selectPos], wk->unk104);
        wk->unk104--;
        func_ov091_021bb0c4(wk, wk->plates[wk->cursorPos], wk->unk104);
        func_ov091_021bb0c4(wk, wk->plates[wk->selectPos], wk->unk104);
        if (wk->unk104 == 0) {
            func_ov091_021b999c(wk);
        }
        break;
    }
}


void func_ov091_021b9968(PokeListWork *wk) {
    wk->state = 5;
    wk->subState = 3;
    wk->unk104 = 0;
    GFL_SndSEPlay(SEQ_SE_SYS_02);
    func_0204b3dc(wk->cursor, FALSE);
    func_0204b3dc(wk->subCursor, FALSE);
}

void func_ov091_021b999c(PokeListWork *wk) {
    wk->pkm = NULL;
    wk->state = 2;
    func_ov091_021b8f68(wk, wk->cursorPos);
    func_ov091_021bafa0(wk, wk->plates[wk->selectPos], 0);
    func_0204b3dc(wk->subCursor, FALSE);
    wk->selectPos = 9;
    wk->param->index = wk->cursorPos;
    func_ov091_021b7b98(wk);
}

u32 func_ov091_021b99e4(PartyPkm *pkm, u8 slot) {
    switch (PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + slot, NULL)) {
    case MOVE_CUT:
        return 11;
    case MOVE_FLY:
        return 15;
    case MOVE_SURF:
        return 12;
    case MOVE_STRENGTH:
        return 14;
    case MOVE_WATERFALL:
        return 13;
    case MOVE_FLASH:
        return 16;
    case MOVE_TELEPORT:
        return 17;
    case MOVE_DIG:
        return 18;
    case MOVE_SWEET_SCENT:
        return 19;
    case MOVE_CHATTER:
        return 20;
    case MOVE_DIVE:
        return 21;
    case MOVE_MILK_DRINK:
        return 22;
    case MOVE_SOFTBOILED:
        return 23;
    }
    return 0;
}





// The moves outside battle, by the index of their check in CheckAllowHidenEvent
static const u16 sHidenMoves[] = {
    MOVE_CUT, MOVE_SURF,        MOVE_WATERFALL, MOVE_STRENGTH, MOVE_FLY,        MOVE_FLASH,      MOVE_TELEPORT,
    MOVE_DIG, MOVE_SWEET_SCENT, MOVE_CHATTER,   MOVE_DIVE,     MOVE_MILK_DRINK, MOVE_SOFTBOILED,
};

// pokebw2's PokeList_IsBattle
BOOL func_ov091_021b9a84(PokeListWork *wk) {
    if (wk->param->mode == 1 || wk->param->mode == 22 || wk->param->mode == 23) {
        return TRUE;
    }
    return FALSE;
}

// Whether the cancel button is shown next to the battle's decide button
BOOL func_ov091_021b9aa0(PokeListWork *wk) {
    if (wk->param->mode == 1) {
        return FALSE;
    }
    return TRUE;
}

BOOL func_ov091_021b9ab4(PokeListWork *wk) {
    if (wk->param->mode == 0x15) {
        return FALSE;
    }
    return TRUE;
}

u32 func_ov091_021b9ac8(PokeListWork *wk, PartyPkm *pkm, u8 pos) {
    u8 i;
    BOOL hasEmptySlot = FALSE;
    u32 ret;

    if (wk->wasMode18 == TRUE) {
        u32 moveA = wk->param->move;
        u32 moveB;
        u32 moveC;

        if (moveA == POKELIST_MOVE_ULTIMATE) {
            moveA = MOVE_FRENZY_PLANT;
            moveB = MOVE_BLAST_BURN;
            moveC = MOVE_HYDRO_CANNON;
        } else if (moveA == POKELIST_MOVE_PLEDGE) {
            moveA = MOVE_GRASS_PLEDGE;
            moveB = MOVE_FIRE_PLEDGE;
            moveC = MOVE_WATER_PLEDGE;
        } else {
            moveB = moveA;
            moveC = moveA;
        }
        for (i = 0; i < 4; i++) {
            u32 move = PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + i, NULL);

            if (move == moveA || move == moveB || move == moveC) {
                return 3;
            }
            if (move == 0) {
                hasEmptySlot = TRUE;
            }
        }
        if (wk->param->learnMask & (1 << pos)) {
            if (hasEmptySlot == TRUE) {
                return 0;
            }
            return 1;
        }
        return 2;
    }
    for (i = 0; i < 4; i++) {
        u32 move = PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + i, NULL);

        if (move == wk->param->move) {
            return 3;
        }
        if (move == 0) {
            hasEmptySlot = TRUE;
        }
    }
    if (wk->param->item != 0) {
        u8 tm = PML_ItemGetTMBitMask(wk->param->item);

        if (tm != 0xff && canPkmLearnTM_Wrapper(pkm, tm) == TRUE) {
            ret = 0;
            if (hasEmptySlot != TRUE) {
                ret = 1;
            }
            return ret;
        }
        return 2;
    }
    ret = 0;
    if (hasEmptySlot != TRUE) {
        ret = 1;
    }
    return ret;
}


BOOL func_ov091_021b9bf4(PokeListWork *wk, PartyPkm *pkm, u16 item) {
    return CheckEvolveSpecies(wk->param->party, pkm, 3, item, wk->param->unk5F, NULL, wk->heapId);
}


// pokebw2's PokeList_PrintString
void func_ov091_021b9c1c(PokeListWork *wk, BmpWin *window, u16 msgId, u16 x, s16 y, u16 color) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    func_0201c724(wk->printQueue, BmpWin_GetBitmap(window), x, y, str, wk->font, color);
    GFL_StrBufFree(str);
}

// pokebw2's PokeList_PrintStringSmall
void func_ov091_021b9c64(PokeListWork *wk, BmpWin *window, u32 msgId, int x, s16 y, u16 color) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    func_0201c724(wk->printQueue, BmpWin_GetBitmap(window), x, y, str, wk->smallFont, color);
    GFL_StrBufFree(str);
}

// Draws a string at once, without the print queue
// pokebw2's PokeList_DrawStringSmall
void func_ov091_021b9cb0(PokeListWork *wk, BmpWin *window, u32 msgId, int x, s16 y, u16 color) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(window), x, y, str, wk->smallFont, color);
    GFL_StrBufFree(str);
}

void func_ov091_021b9cf4(PokeListWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, s16 x, s16 y,
                                 u16 color) {
    StrBuf *buf = GFL_StrBufCreate(16, wk->heapId);
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    GFL_WordSetFormatStrbuf(wordSet, buf, str);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(window), x, y, buf, wk->font, color);
    GFL_StrBufFree(str);
    GFL_StrBufFree(buf);
}

void func_ov091_021b9d58(PokeListWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, s16 x, s16 y,
                                      u16 color) {
    StrBuf *buf = GFL_StrBufCreate(16, wk->heapId);
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    GFL_WordSetFormatStrbuf(wordSet, buf, str);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(window), x, y, buf, wk->smallFont, color);
    GFL_StrBufFree(str);
    GFL_StrBufFree(buf);
}

void func_ov091_021b9dc0(PokeListWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, s16 x, s16 y,
                                     u16 color) {
    StrBuf *buf = GFL_StrBufCreate(16, wk->heapId);
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    GFL_WordSetFormatStrbuf(wordSet, buf, str);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(window), x, y, buf, wk->smallFont, color);
    GFL_StrBufFree(str);
    GFL_StrBufFree(buf);
}

// Called once a message is read: back to picking a Pokémon
// pokebw2's PokeList_MessageDoneSelect
void func_ov091_021b9e20(PokeListWork *wk) {
    func_ov091_021bb7a4(wk, wk->message);
    wk->state = 2;
    func_ov091_021b7b98(wk);
}

// Called once a message is read: the list ends
// pokebw2's PokeList_MessageDoneExit
void func_ov091_021b9e3c(PokeListWork *wk) {
    wk->state = 19;
}

void func_ov091_021b9e44(PokeListWork *wk) {
    if (func_ov091_021ba65c(wk, wk->param->item) != 0) {
        func_ov091_021bb7a4(wk, wk->message);
        wk->state = 2;
        func_ov091_021b7b98(wk);
    } else {
        func_ov091_021bb7a4(wk, wk->message);
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb9ac(wk, wk->message, 0, wk->param->item);
        func_ov091_021b9788(wk, 0xb3, TRUE, func_ov091_021b9e3c);
        func_ov091_021bb98c(wk, wk->message);
    }
}


// Asks whether to give up learning the move
// pokebw2's PokeList_AskStopLearning
void func_ov091_021b9eb0(PokeListWork *wk) {
    func_ov091_021b9804(wk, func_ov091_021b9ec0);
}

void func_ov091_021b9ec0(PokeListWork *wk, u32 item) {
    func_ov091_021bb7a4(wk, wk->message);
    if (item == 14) {
        if (wk->param->mode == 5 || wk->param->mode == 8) {
            wk->param->result = 5;
        } else {
            wk->param->result = 4;
        }
        func_ov091_021b9788(wk, 0x27, TRUE, func_ov091_021b9e3c);
    } else {
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
        func_ov091_021b9788(wk, 0x23, FALSE, func_ov091_021b9f34);
        func_ov091_021bb98c(wk, wk->message);
    }
}


// Asks whether to forget a move for the new one
// pokebw2's PokeList_AskForgetMove
void func_ov091_021b9f34(PokeListWork *wk) {
    func_ov091_021b9804(wk, func_ov091_021b9f44);
}

void func_ov091_021b9f44(PokeListWork *wk, u32 item) {
    func_ov091_021bb7a4(wk, wk->message);
    if (item == 14) {
        wk->param->result = 10;
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
        if (wk->param->mode == 5 || wk->param->mode == 8) {
            func_ov091_021b9788(wk, 0x26, TRUE, func_ov091_021bc6c0);
        } else {
            func_ov091_021b9788(wk, 0x26, TRUE, func_ov091_021b9e3c);
        }
        func_ov091_021bb98c(wk, wk->message);
    } else {
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
        func_ov091_021b9788(wk, 0x20, FALSE, func_ov091_021b9eb0);
        func_ov091_021bb98c(wk, wk->message);
    }
}

void func_ov091_021ba004(PokeListWork *wk) {
    func_ov091_021bb7a4(wk, wk->message);
    func_ov091_021bb97c(wk, wk->message);
    func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
    func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
    if (wk->param->mode == 5 || wk->param->mode == 8) {
        func_ov091_021b9788(wk, 0x29, TRUE, func_ov091_021bc6c0);
    } else {
        func_ov091_021b9788(wk, 0x29, TRUE, func_ov091_021b9e3c);
        func_ov091_021ba60c(wk, wk->pkm);
        wk->param->result = 10;
    }
    func_ov091_021bb98c(wk, wk->message);
    func_ov091_021ba5e0(wk, wk->pkm, wk->param->moveSlot);
}



// Asks whether to swap the item held for the one given
// pokebw2's PokeList_AskSwapItem
void func_ov091_021ba098(PokeListWork *wk) {
    func_ov091_021b9804(wk, func_ov091_021ba0a8);
}

void func_ov091_021ba0a8(PokeListWork *wk, u32 item) {
    if (item == 14) {
        u32 heldItem = PokeParty_GetParam(wk->pkm, PKM_PARAM_ITEM, NULL);
        BOOL canBecomeOrigin;
        BOOL canBecomeAltered;

        if (func_ov091_021ba65c(wk, heldItem) == 999) {
            if (wk->param->mode == 10) {
                wk->param->mode = 0;
                func_ov091_021bb7a4(wk, wk->message);
                func_ov091_021b9788(wk, 0x3f, TRUE, func_ov091_021b9e20);
                return;
            }
            wk->state = 19;
            func_ov091_021bb7a4(wk, wk->message);
            func_ov091_021b9788(wk, 0x3f, TRUE, func_ov091_021b9e3c);
            return;
        }
        if (PML_ItemIsMail(wk->param->item) == TRUE) {
            wk->param->index = wk->cursorPos;
            wk->state = 19;
            wk->param->result = 6;
            return;
        }
        func_ov091_021bb7a4(wk, wk->message);
        func_ov091_021ba624(wk, wk->pkm, wk->param->item);
        func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
        func_ov091_021bc24c(wk, wk->pkm, wk->param->item);
        func_ov091_021bc288(wk, wk->pkm, wk->param->item);
        canBecomeOrigin = func_ov091_021bdbf8(wk, wk->pkm);
        canBecomeAltered = func_ov091_021bdc5c(wk, wk->pkm);
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb9ac(wk, wk->message, 1, heldItem);
        func_ov091_021bb9ac(wk, wk->message, 2, wk->param->item);
        if (wk->param->mode == 10) {
            wk->param->mode = 0;
            if (canBecomeOrigin == TRUE) {
                func_ov091_021bdc3c(wk, wk->pkm);
                func_ov091_021b9788(wk, 0x40, TRUE, func_ov091_021ba4e4);
                wk->demo = 1;
            } else if (canBecomeAltered == TRUE) {
                func_ov091_021bdca0(wk, wk->pkm);
                func_ov091_021b9788(wk, 0x40, TRUE, func_ov091_021ba4e4);
                wk->demo = 2;
            } else {
                func_ov091_021b9788(wk, 0x40, TRUE, func_ov091_021b9e20);
            }
        } else {
            wk->param->result = 10;
            if (canBecomeOrigin == TRUE) {
                func_ov091_021bdc3c(wk, wk->pkm);
                func_ov091_021b9788(wk, 0x40, TRUE, func_ov091_021ba4e4);
                wk->demo = 1;
            } else if (canBecomeAltered == TRUE) {
                func_ov091_021bdca0(wk, wk->pkm);
                func_ov091_021b9788(wk, 0x40, TRUE, func_ov091_021ba4e4);
                wk->demo = 2;
            } else {
                func_ov091_021b9788(wk, 0x40, TRUE, func_ov091_021b9e3c);
            }
        }
        func_ov091_021bb98c(wk, wk->message);
    } else if (wk->param->mode == 10) {
        wk->param->mode = 0;
        func_ov091_021bb7a4(wk, wk->message);
        wk->state = 2;
        func_ov091_021b7b98(wk);
    } else {
        wk->state = 19;
    }
}


// The user's HP has gone down: the HP it shares is given to the Pokémon picked, whose HP goes up next
// pokebw2's PokeList_ShareHpDone
static void func_ov091_021ba288(PokeListWork *wk) {
    PartyPkm *user = PokeParty_GetPkm(wk->param->party, wk->selectPos2);
    // Read but not used
    PokeParty_GetParam(user, PKM_PARAM_HP, NULL);
    u32 userMaxHp = PokeParty_GetParam(user, PKM_PARAM_MAX_HP, NULL);
    PartyPkm *target = PokeParty_GetPkm(wk->param->party, wk->param->index);
    u32 hp = PokeParty_GetParam(wk->pkm, PKM_PARAM_HP, NULL);
    u32 maxHp = PokeParty_GetParam(wk->pkm, PKM_PARAM_MAX_HP, NULL);

    wk->cursorPos = wk->param->index;
    if (hp + userMaxHp / 5 > maxHp) {
        PokeParty_SetParam(target, PKM_PARAM_HP, maxHp);
    } else {
        PokeParty_SetParam(target, PKM_PARAM_HP, hp + userMaxHp / 5);
    }
    wk->hpDoneFunc = func_ov091_021ba318;
    wk->state = 10;
    wk->playHealSe = TRUE;
}

void func_ov091_021ba318(PokeListWork *wk) {
    u16 hp = func_ov091_021bb464(wk, wk->plates[wk->param->index]);

    wk->cursorPos = wk->param->index;
    func_ov091_021bb97c(wk, wk->message);
    func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
    if (wk->prevHp != 0) {
        func_ov091_021bb9ec(wk, wk->message, 1, hp - wk->prevHp, 3);
        func_ov091_021b9788(wk, 0x2c, TRUE, func_ov091_021b9e20);
    }
    func_ov091_021bb98c(wk, wk->message);
}


// Asks whether to take the mail
// pokebw2's PokeList_AskTakeMail
void func_ov091_021ba394(PokeListWork *wk) {
    func_ov091_021b9804(wk, func_ov091_021ba3a4);
}

void func_ov091_021ba3a4(PokeListWork *wk, u32 item) {
    func_ov091_021bb7a4(wk, wk->message);
    if (item == 14) {
        s32 slot = func_02009020(wk->param->unk08, 0);

        if (slot >= 0) {
            MailData *mail = CreateMailData(wk->heapId);

            PokeParty_GetParam(wk->pkm, PKM_PARAM_MAIL, mail);
            func_0200903c(wk->param->unk08, 0, slot, mail);
            GFL_HeapFree(mail);
            PokeParty_SetParam(wk->pkm, PKM_PARAM_ITEM, 0);
            func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
            func_ov091_021b9788(wk, 0x1a, TRUE, func_ov091_021b9e20);
        } else {
            func_ov091_021b9788(wk, 0x1e, TRUE, func_ov091_021b9e20);
        }
    } else {
        func_ov091_021b9788(wk, 0x1b, FALSE, func_ov091_021ba444);
    }
}


// Asks whether to take the mail anyway, which loses its message
// pokebw2's PokeList_AskDeleteMail
void func_ov091_021ba444(PokeListWork *wk) {
    func_ov091_021b9804(wk, func_ov091_021ba454);
}

void func_ov091_021ba454(PokeListWork *wk, u32 item) {
    func_ov091_021bb7a4(wk, wk->message);
    if (item == 14) {
        u32 heldItem = PokeParty_GetParam(wk->pkm, PKM_PARAM_ITEM, NULL);

        if (func_ov091_021ba65c(wk, heldItem) == 999) {
            func_ov091_021b9788(wk, 0x3f, TRUE, func_ov091_021b9e20);
            return;
        }
        func_ov091_021ba688(wk, heldItem);
        PokeParty_SetParam(wk->pkm, PKM_PARAM_ITEM, 0);
        func_ov091_021bb2c0(wk, wk->plates[wk->cursorPos]);
        func_ov091_021b9788(wk, 0x1f, TRUE, func_ov091_021b9e20);
    } else {
        func_ov091_021bb7a4(wk, wk->message);
        wk->state = 2;
        func_ov091_021b7b98(wk);
    }
}


// Called once a message is read: the form change is shown
// pokebw2's PokeList_MessageDoneDemo
void func_ov091_021ba4e4(PokeListWork *wk) {
    func_ov091_021bb7a4(wk, wk->message);
    wk->state = 14;
}

void func_ov091_021ba4f8(PokeListWork *wk) {
    if (wk->timeUp == FALSE) {
        if (wk->state == 19 || wk->state == 21 || !wk->state || wk->state == 1) {
            return;
        }
        if (wk->state == 6) {
            func_ov091_021b90f8(wk);
        }
        wk->timeUp = TRUE;
        if (wk->unk278 == TRUE) {
            wk->state = 17;
        } else {
            wk->state = 20;
        }
        wk->param->index = 6;
        wk->param->result = 0;
        if (wk->decided == FALSE) {
            Regulation *regulation;
            u8 i;

            for (i = 0; i < 6; i++) {
                wk->param->picked[i] = 0;
            }
            regulation = wk->param->regulation;
            if (func_0201a59c(regulation, wk->param->party, wk->param->picked) == FALSE) {
                for (i = 0; i < 6; i++) {
                    wk->param->picked[i] = 0;
                }
                for (i = 0; i < regulation->unk2; i++) {
                    wk->param->picked[i] = i + 1;
                }
            }
        }
    }
}


// pokebw2's PokeList_LearnMove
void func_ov091_021ba5b4(PokeListWork *wk, PartyPkm *pkm) {
    if (wk->param->mode == 5 || wk->param->mode == 8) {
        PokeParty_LearnMove(pkm, wk->param->move);
    } else {
        func_02018238(pkm, wk->param->move);
    }
}

// pokebw2's PokeList_SetMove
static void func_ov091_021ba5e0(PokeListWork *wk, PartyPkm *pkm, u8 slot) {
    if (wk->param->mode == 5 || wk->param->mode == 8) {
        PokeParty_SetMove(pkm, wk->param->move, slot);
    } else {
        func_020182a0(pkm, wk->param->move, slot);
    }
}

// pokebw2's PokeList_RaiseFriendship
void func_ov091_021ba60c(PokeListWork *wk, PartyPkm *pkm) {
    FriendshipManagerCalc(pkm, 7, wk->param->zoneId, wk->heapId);
}

// Gives a Pokémon an item from the bag, putting the item it held back in the bag
// pokebw2's PokeList_SetHeldItem
static void func_ov091_021ba624(PokeListWork *wk, PartyPkm *pkm, u16 item) {
    u32 heldItem = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);

    if (heldItem != 0) {
        func_ov091_021ba688(wk, heldItem);
    }
    PokeParty_SetParam(pkm, PKM_PARAM_ITEM, item);
    if (item != 0) {
        func_ov091_021ba670(wk, item);
    }
}

// pokebw2's PokeList_GetBagCount
u16 func_ov091_021ba65c(PokeListWork *wk, u16 item) {
    return BagSave_GetItemCountByID(wk->param->bag, item, wk->heapId);
}

// pokebw2's PokeList_SubFromBag
void func_ov091_021ba670(PokeListWork *wk, u16 item) {
    BagSave_SubItem(wk->param->bag, item, 1, wk->heapId);
}

// pokebw2's PokeList_AddToBag
void func_ov091_021ba688(PokeListWork *wk, u16 item) {
    BagSave_AddItem(wk->param->bag, item, 1, wk->heapId);
}
