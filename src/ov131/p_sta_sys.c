// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov207/p_sta_sys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/net.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "p_status_local.h"
#include "system/app_menu_common.h"
#include "system/game_data.h"
#include "system/wipe.h"
#include "app/p_status.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/msg.h"
#include "gfl/tcb.h"
#include "pml/poke_graphic.h"
#include "system/gf_font.h"
#include "system/mcss.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "nnsys/gfd.h"

// The summary screen's core: setting up and tearing down the screens, the main loop, the buttons of the bottom
// screen, changing the page or the Pokémon behind a mosaic, and printing for the pages. The file's name is a guess

// The shortcut of the info page; the skill and ribbon pages' follow it
#ifdef BLACK2
#define SHORTCUT_PSTATUS_INFO 9
#else
#define SHORTCUT_PSTATUS_INFO 7
#endif

// A 16-color palette's size, and a color of a palette in the OBJ and sub-screen BG palette memory
#define PLTT_SIZE (16 * sizeof(GXRgb))
#define OBJ_PLTT_COLOR(palette, color) ((GXRgb *)HW_OBJ_PLTT + (palette) * 16 + (color))
#define DB_BG_PLTT_COLOR(palette, color) ((GXRgb *)HW_DB_BG_PLTT + (palette) * 16 + (color))

// The BG palettes of each screen, in the version's colors
#ifdef BLACK2
#define NCLR_MAIN_BG 1
#define NCLR_SUB_BG 3
#else
#define NCLR_MAIN_BG 0
#define NCLR_SUB_BG 1
#endif

void func_ov131_021d46e4(TCB *tcb, void *data);
void func_ov131_021d46ec(PStatusWork *wk);
void func_ov131_021d48e4(PStatusWork *wk);
void func_ov131_021d4940(const BGSetup *setup, u8 bg, u8 mode);
void func_ov131_021d4964(PStatusWork *wk);
void func_ov131_021d4f24(PStatusWork *wk);
void func_ov131_021d4fbc(PStatusWork *wk);
void func_ov131_021d5180(PStatusWork *wk);
void func_ov131_021d51dc(PStatusWork *wk);
void func_ov131_021d5258(PStatusWork *wk);
void func_ov131_021d5280(PStatusWork *wk);
void func_ov131_021d52a8(PStatusWork *wk);
BOOL func_ov131_021d52d4(PStatusWork *wk);
void func_ov131_021d54a8(PStatusWork *wk);
void func_ov131_021d5650(PStatusWork *wk);
BOOL func_ov131_021d5b58(PStatusWork *wk, u8 dir);
BOOL func_ov131_021d5bac(PStatusWork *wk, u8 dir, u16 *index);
void func_ov131_021d5c4c(PStatusWork *wk);
void func_ov131_021d5c98(PStatusWork *wk);
static void func_ov131_021d5dc4(PStatusWork *wk);
BOOL func_ov131_021d62e4(PStatusWork *wk);
BOOL func_ov131_021d62f4(PStatusWork *wk, BoxPkm *pkm);

// pokebw2's sPStatusVRAMConfig
static const BGSysVRAMConfig data_ov131_021dc0e4 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_0_D,   GX_VRAM_TEXPLTT_0_F,     GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K,
};

// pokebw2's sPStatusLCDConfig
static const BGSysLCDConfig data_ov131_021dbff4 = { 1, 0, 0, 1 };

// pokebw2's sPStatusBG1Setup
static const BGSetup data_ov131_021dc0c4 = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xc, 2, 0x8000, 0, 1, 0, 0 };
// pokebw2's sPStatusBG2Setup
static const BGSetup data_ov131_021dc084 = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xd, 4, 0x8000, 0, 2, 1, 0 };
// pokebw2's sPStatusBG3Setup
static const BGSetup data_ov131_021dc004 = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xe, 4, 0, 1, 3, 0, 0 };
// pokebw2's sPStatusBG4Setup
static const BGSetup data_ov131_021dc024 = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xc, 0, 0x6000, 1, 0, 0, 0 };
// pokebw2's sPStatusBG5Setup
static const BGSetup data_ov131_021dc044 = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xd, 4, 0x8000, 1, 1, 0, 0 };
// pokebw2's sPStatusBG6Setup
static const BGSetup data_ov131_021dc064 = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xe, 2, 0, 1, 2, 0, 0 };
// pokebw2's sPStatusBG7Setup
static const BGSetup data_ov131_021dc0a4 = { 0, 0, 0x800, 0, 1, GX_BG_COLORMODE_16, 0xf, 2, 0, 1, 3, 0, 0 };

// pokebw2's sPStatusCameraPos
static const VecFx32 data_ov131_021dbfc0 = { FX32_CONST(-41), 0, FX32_CONST(101) };
// pokebw2's sPStatusCameraTarget
static const VecFx32 data_ov131_021dbfcc = { 0, 0, -FX32_ONE };
// pokebw2's sPStatusCameraUp
static const VecFx32 data_ov131_021dbfd8 = { 0, FX32_ONE, 0 };

// pokebw2's sPStatusEdgeColors
static const GXRgb data_ov131_021dbfe4[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// pokebw2's PStatus_VBlank


BOOL func_ov131_021d4240(PStatusWork *wk) {
    u8 i;

    wk->partyIndex = wk->param->partyIndex;
    wk->shownPartyIndex = 0xff;
    wk->unk1B = 0;
    wk->unk20 = OS_GetVBlankCount();
    wk->unk5C = 4;
    wk->unk44 = 1;
    wk->unk48 = 0;
    wk->unk4C = 0;
    wk->unk54 = 0;
    wk->unk0C = 1;
    wk->unk25C = 0;
    wk->unk260 = 0;
    wk->unk50 = 0;
    wk->unk1A0 = NULL;
    wk->unk10 = 0;
    wk->unk14 = 0;
    wk->unk1C = func_020355c4();
    if (wk->param->mode == PSTATUS_MODE_FORGET_HM) {
        wk->param->mode = PSTATUS_MODE_FORGET_MOVE;
        wk->unk14 = 1;
    }
    if (wk->param->dataType == PSTATUS_DATA_BOX) {
        wk->unk258 = boxPkmRegenToPartyPkm(func_ov131_021d6064(wk), wk->heapId);
    } else {
        wk->unk258 = NULL;
    }
    func_ov131_021d46ec(wk);
    wk->unk88 = func_ov131_021d65d8(wk);
    wk->unk8C = func_ov131_021d7b00(wk);
    wk->unk90 = func_ov131_021d96b0(wk);
    wk->unk94 = func_ov131_021d87e0(wk);
    func_ov131_021d4964(wk);
    func_ov131_021d51dc(wk);
    func_ov131_021d4fbc(wk);
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov131_021d46e4, wk, 8);
    wk->unk1A4 = 0;
    // Colors 6-11 of OBJ palette 3, palettes 11-12 and palette 13 twice
    MIi_CpuCopy16(OBJ_PLTT_COLOR(3, 6), wk->unk1A6, 6 * sizeof(GXRgb));
    MIi_CpuCopy16(OBJ_PLTT_COLOR(11, 0), wk->unk1B6, 2 * PLTT_SIZE);
    MIi_CpuCopy16(OBJ_PLTT_COLOR(13, 0), wk->unk216, PLTT_SIZE);
    MIi_CpuCopy16(OBJ_PLTT_COLOR(13, 0), wk->unk236, PLTT_SIZE);
    func_ov131_021d5c4c(wk);
    if (wk->param->mode == PSTATUS_MODE_FORGET_MOVE) {
        wk->unk58 = PSTATUS_PAGE_FORGET;
        func_ov131_021d697c(wk, wk->unk88);
        func_ov131_021da6a4(wk, wk->unk90);
        func_ov131_021d911c(wk, wk->unk94);
    } else {
        wk->unk58 = wk->param->page;
        if (wk->unk58 < 3) {
            for (i = 0; i < 3; i++) {
                wk->unk60[i] = GameData_IsShortcutRegistered(wk->param->gameData, SHORTCUT_PSTATUS_INFO + i);
            }
        }
        func_ov131_021d697c(wk, wk->unk88);
        func_ov131_021d911c(wk, wk->unk94);
        switch (wk->unk58) {
        case 0:
            func_ov131_021d7c38(wk, wk->unk8C);
            break;
        case 1:
            func_ov131_021d9c3c(wk, wk->unk90);
            break;
        case 2:
            func_ov131_021d8f50(wk, wk->unk94);
            break;
        default:
            wk->unk58 = 0;
            func_ov131_021d7c38(wk, wk->unk8C);
            break;
        }
    }
    func_0203d34c(TRUE, wk->heapId);
    return TRUE;
}

BOOL func_ov131_021d4418(PStatusWork *wk) {
    u8 i;

    GFL_TCBRemove(wk->vblankTcb);
    wk->param->partyIndex = wk->partyIndex;
    func_ov131_021d5180(wk);
    func_ov131_021d5258(wk);
    func_ov131_021d4f24(wk);
    func_ov131_021d91ec(wk, wk->unk94);
    func_ov131_021d881c(wk, wk->unk94);
    func_ov131_021d9734(wk, wk->unk90);
    func_ov131_021d7b34(wk, wk->unk8C);
    func_ov131_021d6608(wk, wk->unk88);
    func_ov131_021d48e4(wk);
    if (wk->unk58 < 3) {
        for (i = 0; i < 3; i++) {
            GameData_SetKeyItemRegistration(wk->param->gameData, i + 7, wk->unk60[i]);
        }
    }
    if (wk->unk14 == 1) {
        wk->param->mode = PSTATUS_MODE_FORGET_HM;
        wk->unk14 = 0;
    }
    func_020355d8(wk->unk1C);
    if (wk->unk258 != NULL) {
        GFL_HeapFree(wk->unk258);
    }
    return TRUE;
}

int func_ov131_021d44c8(PStatusWork *wk) {
    u32 vblankCount;

    wk->unk30 = wk->unk28;
    wk->unk34 = wk->unk2C;
    func_02035cb8(&wk->unk28, &wk->unk2C);
    switch (wk->unk50) {
    case 0:
        if (wk->unk0C == 1) {
            func_ov131_021d5dc4(wk);
        } else {
            GFL_WipeSet(0, 1, 1, 0, 6, 1, wk->heapId);
            wk->unk50 = 1;
        }
        break;
    case 1:
        if (GFL_WipeIsFinished() == TRUE) {
            wk->unk50 = 4;
        }
        break;
    case 2:
        if (wk->unk1A0 == NULL || func_0204b98c(wk->unk1A0) == FALSE || wk->param->forceExit == TRUE) {
            GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
            wk->unk50 = 3;
        }
        break;
    case 3:
        if (GFL_WipeIsFinished() == TRUE) {
            return wk->unk54;
        }
        break;
    case 4:
        func_ov131_021d5280(wk);
        if (wk->unk0C == 1) {
            func_ov131_021d5dc4(wk);
            break;
        }
        func_ov131_021d6614(wk, wk->unk88);
        if (wk->param->forceExit == TRUE) {
            wk->unk54 = 1;
            wk->unk50 = 2;
        }
        func_ov131_021d52a8(wk);
        if (wk->unk0C == 0) {
            switch (wk->unk5C) {
            case PSTATUS_PAGE_INFO:
                func_ov131_021d7b48(wk, wk->unk8C);
                break;
            case PSTATUS_PAGE_SKILL:
            case PSTATUS_PAGE_FORGET:
                func_ov131_021d9740(wk, wk->unk90);
                break;
            case PSTATUS_PAGE_RIBBON:
                func_ov131_021d8830(wk, wk->unk94);
                break;
            }
        }
        break;
    }
    func_0201c4e4(wk->printQueue);
    func_0204a600();
    func_02014c08(wk->mcssSys);
    GFL_G3DSysReset();
    GFL_G3DSysMtxViewFlush();
    func_02014d2c(wk->mcssSys);
    GFL_G3DSysReqSwapBuffers();
    vblankCount = OS_GetVBlankCount();
    wk->unk1B += (u8)(vblankCount - wk->unk20);
    wk->unk20 = vblankCount;
    while (wk->unk1B >= 4) {
        GFL_BGSysMoveBGReq(3, BG_MOVE_RIGHT, 1);
        GFL_BGSysMoveBGReq(3, BG_MOVE_DOWN, 1);
        GFL_BGSysMoveBGReq(7, BG_MOVE_RIGHT, 1);
        GFL_BGSysMoveBGReq(7, BG_MOVE_DOWN, 1);
        wk->unk1B -= 4;
    }
    if (wk->unk48 == 1) {
        if (func_0204b98c(wk->unk178[4]) == FALSE) {
            if (func_ov131_021d5bac(wk, PSTA_DIR_UP, NULL) == TRUE) {
                func_0204b878(wk->unk178[4], 3);
            } else {
                func_0204b878(wk->unk178[4], 17);
            }
            wk->unk48 = 0;
        }
    }
    if (wk->unk4C == 1) {
        if (func_0204b98c(wk->unk178[5]) == FALSE) {
            if (func_ov131_021d5bac(wk, PSTA_DIR_DOWN, NULL) == TRUE) {
                func_0204b878(wk->unk178[5], 2);
            } else {
                func_0204b878(wk->unk178[5], 16);
            }
            wk->unk4C = 0;
        }
    }
    func_ov131_021d5650(wk);
    return 0;
}


void func_ov131_021d46e4(TCB *tcb, void *data) {
    func_0204a648();
}

// pokebw2's PStatus_InitGraphics
void func_ov131_021d46ec(PStatusWork *wk) {
    ClActSysSetup clactSetup;

    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, 0);
    func_0202223c(1, 0);
    func_02022224(0);
    func_02022224(1);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    GFL_BGSysSetVRAMBanks(&data_ov131_021dc0e4);
    GFL_BGSysCreate(wk->heapId);
    BmpWin_InitAllocator(wk->heapId);
    GFL_BGSysSetLCDConfig(&data_ov131_021dbff4);
    func_ov131_021d4940(&data_ov131_021dc0c4, 1, 0);
    func_ov131_021d4940(&data_ov131_021dc084, 2, 0);
    func_ov131_021d4940(&data_ov131_021dc004, 3, 0);
    GFL_BGSysSetBGEnabled(0, TRUE);
    func_ov131_021d4940(&data_ov131_021dc024, 4, 0);
    func_ov131_021d4940(&data_ov131_021dc044, 5, 0);
    func_ov131_021d4940(&data_ov131_021dc064, 6, 0);
    func_ov131_021d4940(&data_ov131_021dc0a4, 7, 0);
    G2_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ, 13,
                     16);
    G2S_SetBlendAlpha(GX_PLANEMASK_BG2, GX_PLANEMASK_BG3, 13, 16);
    clactSetup = data_020a1448;
    clactSetup.charCount = 110;
    ClActSys_Create(&clactSetup, &data_ov131_021dc0e4, wk->heapId);
    wk->actorUnit = func_0204b100(96, 0, wk->heapId);
    func_0204b270(wk->actorUnit);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    GFL_G3DSysCreate(FALSE, 2, FALSE, 1, 0, wk->heapId, NULL);
    wk->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, FX32_CONST(192), 0, 0, FX32_CONST(256), FX32_CONST(128),
                                     FX32_CONST(512), 0, &data_ov131_021dbfc0, &data_ov131_021dbfd8, &data_ov131_021dbfcc,
                                     wk->heapId);
    GFL_G3DCameraFlush(wk->camera);
    G3X_SetEdgeColorTable(data_ov131_021dbfe4);
    G3X_EdgeMarking(FALSE);
    GFL_G3DSysSetSwapBufferParams(0, 0);
    GFL_BGSysSet3DBGPriority(0);
    wk->mcssSys = func_02014b08(2, wk->heapId);
    func_02016020(wk->mcssSys, 0);
    func_020160c0(wk->mcssSys, 0x10000);
}

// pokebw2's PStatus_ExitGraphics
void func_ov131_021d48e4(PStatusWork *wk) {
    NNS_GfdClearVramTransferManagerTask();
    func_02014ba8(wk->mcssSys);
    GFL_G3DCameraFree(wk->camera);
    GFL_G3DSysFree();
    func_0204b1cc(wk->actorUnit);
    func_0204a5ac();
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

// pokebw2's PStatus_CreateBG
void func_ov131_021d4940(const BGSetup *setup, u8 bg, u8 mode) {
    GFL_BGSysCreateBG(bg, setup, mode);
    GFL_BGSysSetBGEnabled(bg, TRUE);
    GFL_BGSysClearBG(bg);
    GFL_BGSysLoadScr(bg);
}

void func_ov131_021d4964(PStatusWork *wk) {
    u8 i;
    ArcTool *uiArc;
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_P_STATUS, wk->heapId);

    GFL_G2DIOLoadArcNCLRDefault(arc, NCLR_MAIN_BG, 0, 0, 0, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 8, 2, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 70, 3, 0, 0, FALSE, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, NCLR_SUB_BG, 4, 0, 0, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 10, 6, 0, 0, FALSE, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 9, 5, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 71, 7, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 65, 5, 0, 0, FALSE, wk->heapId);
    wk->unk98[0] = func_0204ac18(arc, 3, 0, 0, 0, 4, wk->heapId);
    wk->unk98[6] = func_0204ac18(arc, 5, 0, 0x160, 0, 2, wk->heapId);
    wk->unk98[7] = func_0204ac18(arc, 6, 1, 0, 0, 5, wk->heapId);
    wk->unk98[8] = func_0204ac18(arc, 4, 0, 0x1a0, 0, 1, wk->heapId);
    wk->unk98[11] = func_0204ac18(arc, 2, 0, 0x1c0, 0, 1, wk->heapId);
    wk->unk98[12] = func_0204a6c8(arc, 11, 0, 0, wk->heapId);
    wk->unk98[20] = func_0204a6c8(arc, 15, 0, 0, wk->heapId);
    wk->unk98[21] = func_0204a6c8(arc, 14, 0, 0, wk->heapId);
    wk->unk98[24] = func_0204a6c8(arc, 13, 0, 0, wk->heapId);
    wk->unk98[25] = func_0204af28(arc, 0x4e, 0x81, wk->heapId);
    wk->unk98[32] = func_0204af28(arc, 0x51, 0x84, wk->heapId);
    wk->unk98[33] = func_0204af28(arc, 0x50, 0x83, wk->heapId);
    wk->unk98[36] = func_0204af28(arc, 0x52, 0x85, wk->heapId);
    wk->unk98[37] = func_0204af28(arc, 0x4f, 0x82, wk->heapId);

    uiArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(uiArc, func_02027624(), 0, 0x120, 0x20, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(uiArc, func_02027628(), 1, 0, 0, FALSE, wk->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(uiArc, func_0202762c(), 1, 0, 0, FALSE, wk->heapId);
    GFL_BGSysSetScrPaletteNo(1, 0, 21, 32, 3, 9);
    GFL_G2DIOLoadArcNCLRDefault(uiArc, 0, 4, 0x1a0, 0x20, wk->heapId);
    wk->unk98[5] = func_0204ac18(uiArc, 1, 1, 0x100, 0, 1, wk->heapId);
    wk->unk98[19] = func_0204a6c8(uiArc, 2, 0, 1, wk->heapId);
    wk->unk98[31] = func_0204af28(uiArc, 5, 8, wk->heapId);
    wk->unk98[3] = func_0204ac18(uiArc, func_020275e8(), 0, 0x100, 0, 3, wk->heapId);
    wk->unk98[4] = func_0204ac18(uiArc, func_020275e8(), 1, 0xa0, 0, 3, wk->heapId);
    wk->unk98[29] = func_0204af28(uiArc, func_020275fc(2), func_02027600(2), wk->heapId);
    wk->unk98[30] = func_0204af28(uiArc, func_020275fc(0), func_02027600(0), wk->heapId);
    for (i = 0; i < 17; i++) {
        wk->typeIconChars[i] = func_0204a6c8(uiArc, func_020275f8(i), 0, 0, wk->heapId);
    }
    wk->unk98[16] = func_0204a6c8(uiArc, 0x38, 0, 1, wk->heapId);
    wk->unk98[17] = func_0204a6c8(uiArc, 0x39, 0, 1, wk->heapId);
    wk->unk98[18] = func_0204a6c8(uiArc, 0x3a, 0, 1, wk->heapId);
    wk->unk98[13] = func_0204a6c8(uiArc, func_02027618(), 0, 0, wk->heapId);
    wk->unk98[26] = func_0204af28(uiArc, func_0202761c(2), func_02027620(2), wk->heapId);
    wk->unk98[9] = func_0204ac18(uiArc, func_02027768(), 0, 0xa0, 0, 1, wk->heapId);
    wk->unk98[22] = func_0204a6c8(uiArc, func_0202776c(2), 0, 0, wk->heapId);
    wk->unk98[34] = func_0204af28(uiArc, func_02027770(2), func_02027774(2), wk->heapId);
    wk->unk98[2] = func_0204ac18(uiArc, func_02027748(), 0, 0xc0, 0, 1, wk->heapId);
    wk->unk98[15] = func_0204a6c8(uiArc, func_0202774c(2), 0, 0, wk->heapId);
    wk->unk98[28] = func_0204af28(uiArc, func_02027750(2), func_02027754(2), wk->heapId);
    wk->unk98[10] = func_0204ac18(uiArc, func_020276b4(), 0, 0x80, 0, 1, wk->heapId);
    wk->unk98[23] = func_0204a6c8(uiArc, func_020276b8(), 0, 0, wk->heapId);
    wk->unk98[35] = func_0204af28(uiArc, func_020276bc(2), func_020276c0(2), wk->heapId);
    wk->unk98[1] = func_0204ac18(uiArc, func_02027720(1), 0, 0xe0, 0, 1, wk->heapId);
    wk->unk98[14] = func_0204a6c8(uiArc, func_0202772c(1), 0, 0, wk->heapId);
    wk->unk98[27] = func_0204af28(uiArc, func_02027738(1, 2), func_02027740(1, 2), wk->heapId);
    GFL_ArcToolFree(uiArc);

    func_ov131_021d67ec(wk, wk->unk88, arc);
    func_ov131_021d7b4c(wk, wk->unk8C, arc);
    func_ov131_021d9980(wk, wk->unk90, arc);
    func_ov131_021d8884(wk, wk->unk94, arc);
    GFL_ArcToolFree(arc);
}


void func_ov131_021d4f24(PStatusWork *wk) {
    u8 i;

    func_ov131_021d8904(wk, wk->unk94);
    func_ov131_021d9a04(wk, wk->unk90);
    func_ov131_021d7bf0(wk, wk->unk8C);
    func_ov131_021d6808(wk, wk->unk88);
    for (i = 0; i <= 11; i++) {
        func_0204ada4(wk->unk98[i]);
    }
    for (i = 12; i <= 24; i++) {
        func_0204a8d4(wk->unk98[i]);
    }
    for (i = 25; i <= 37; i++) {
        func_0204afd8(wk->unk98[i]);
    }
    for (i = 0; i < 17; i++) {
        func_0204a8d4(wk->typeIconChars[i]);
    }
}

void func_ov131_021d4fbc(PStatusWork *wk) {
    u8 sequences[8] = { 0, 1, 2, 6, 3, 2, 0, 1 };
    u8 xs[8] = { 0, 40, 80, 124, 144, 168, 200, 232 };
    ClActorSetup setup;
    u8 i;
    u32 chars;
    u32 cellAnims;

    setup.priority = 10;
    setup.bgPriority = 0;
    for (i = 0; i < 8; i++) {
        if (i <= 2) {
            chars = wk->unk98[12];
            cellAnims = wk->unk98[25];
        } else {
            chars = wk->unk98[13];
            cellAnims = wk->unk98[26];
        }
        setup.x = xs[i];
        setup.y = i == 3 ? 172 : 168;
        setup.sequence = sequences[i];
        wk->unk178[i] = func_0204b294(wk->actorUnit, chars, wk->unk98[0], cellAnims, &setup, 0, wk->heapId);
        func_0204b92c(wk->unk178[i], TRUE);
        func_0204b3dc(wk->unk178[i], TRUE);
    }
    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    wk->typeIcons[0] = func_0204b294(wk->actorUnit, wk->typeIconChars[0], wk->unk98[3], wk->unk98[29], &setup, 0, wk->heapId);
    wk->typeIcons[1] = func_0204b294(wk->actorUnit, wk->typeIconChars[0], wk->unk98[3], wk->unk98[29], &setup, 0, wk->heapId);
    for (i = 0; i < 2; i++) {
        func_0204b3dc(wk->typeIcons[i], FALSE);
    }
    if (wk->param->mode == PSTATUS_MODE_FORGET_MOVE) {
        for (i = 0; i < 8; i++) {
            if (i != 7) {
                func_0204b3dc(wk->unk178[i], FALSE);
            }
        }
    }
    if (func_ov131_021d62e4(wk) == FALSE) {
        func_0204b3dc(wk->unk178[6], FALSE);
        func_0204b3dc(wk->unk178[3], FALSE);
    }
    func_ov131_021d6810(wk, wk->unk88);
    func_ov131_021d8938(wk, wk->unk94);
    func_ov131_021d9a4c(wk, wk->unk90);
}



void func_ov131_021d5180(PStatusWork *wk) {
    u8 i;

    func_ov131_021d89fc(wk, wk->unk94);
    func_ov131_021d9bd4(wk, wk->unk90);
    func_ov131_021d694c(wk, wk->unk88);
    for (i = 0; i < 8; i++) {
        func_0204b3b4(wk->unk178[i]);
    }
    for (i = 0; i < 2; i++) {
        func_0204b3b4(wk->typeIcons[i]);
    }
}


// pokebw2's PStatus_InitText
void func_ov131_021d51dc(PStatusWork *wk) {
    GXRgb *pltt;

    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0179, wk->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1c0, 0x20, wk->heapId);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1c0, 0x20, wk->heapId);
    pltt = DB_BG_PLTT_COLOR(14, 0);
    pltt[9] = GX_RGB(25, 18, 19);
    pltt[10] = GX_RGB(18, 18, 26);
    wk->printQueue = func_0201c450(0x1000, wk->heapId);
}

// pokebw2's PStatus_ExitText
void func_ov131_021d5258(PStatusWork *wk) {
    func_0201c6ec(wk->printQueue);
    func_0201c4c0(wk->printQueue);
    GFL_MsgDataFree(wk->msgData);
    GFL_FontFree(wk->font);
}

void func_ov131_021d5280(PStatusWork *wk) {
    TouchRect rects[] = {
        { 168, 192, 0, 40 }, { 168, 192, 40, 80 }, { 168, 192, 80, 120 },
        { 168, 192, 120, 144 }, { 168, 192, 144, 168 }, { 168, 192, 168, 192 },
        { 168, 192, 200, 224 }, { 168, 192, 232, 0 }, { TOUCH_RECT_END, 0, 0, 0 },
    };

    wk->touchHit = func_02035c08(rects);
}

void func_ov131_021d52a8(PStatusWork *wk) {
    if (wk->unk44 == 1 && func_0201c6b4(wk->printQueue) == TRUE && func_ov131_021d52d4(wk) == FALSE) {
        func_ov131_021d54a8(wk);
    }
}

BOOL func_ov131_021d52d4(PStatusWork *wk) {
    int lastPage;

    if (GCTX_HIDGetTypedKeys() & PAD_KEY_DOWN) {
        if (func_ov131_021d5b58(wk, PSTA_DIR_DOWN) == TRUE) {
            func_ov131_021d5c98(wk);
            func_0204b878(wk->unk178[4], 3);
            func_0204b878(wk->unk178[5], 10);
            wk->unk48 = FALSE;
            wk->unk4C = TRUE;
            GFL_SndSEPlay(SEQ_SE_SYS_37);
            wk->unk1C = FALSE;
            return TRUE;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_UP) {
        if (func_ov131_021d5b58(wk, PSTA_DIR_UP) == TRUE) {
            func_ov131_021d5c98(wk);
            func_0204b878(wk->unk178[4], 11);
            func_0204b878(wk->unk178[5], 2);
            wk->unk48 = TRUE;
            wk->unk4C = FALSE;
            GFL_SndSEPlay(SEQ_SE_SYS_37);
            wk->unk1C = FALSE;
            return TRUE;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_RIGHT) {
        lastPage = PSTATUS_PAGE_RIBBON;
        if (wk->unk40 == 0) {
            lastPage = PSTATUS_PAGE_SKILL;
        }
        if (wk->unk58 < lastPage && wk->isEgg == FALSE) {
            wk->unk58++;
            func_ov131_021d5c98(wk);
            GFL_SndSEPlay(SEQ_SE_SYS_38);
            wk->unk1C = FALSE;
            return TRUE;
        }
    } else if (GCTX_HIDGetTypedKeys() & PAD_KEY_LEFT) {
        if (wk->unk58 > PSTATUS_PAGE_INFO && wk->isEgg == FALSE) {
            wk->unk58--;
            func_ov131_021d5c98(wk);
            GFL_SndSEPlay(SEQ_SE_SYS_38);
            wk->unk1C = FALSE;
            return TRUE;
        }
    } else if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        wk->unk54 = PSTATUS_RESULT_BACK;
        wk->param->result = PSTATUS_RESULT_BACK;
        wk->unk50 = 2;
        func_0204b878(wk->unk178[7], 9);
        wk->unk1A0 = wk->unk178[7];
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        wk->unk1C = FALSE;
        return TRUE;
    } else if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_X) && func_ov131_021d62e4(wk) == TRUE) {
        wk->unk54 = PSTATUS_RESULT_CLOSE;
        wk->param->result = PSTATUS_RESULT_CLOSE;
        wk->unk50 = 2;
        func_0204b878(wk->unk178[6], 8);
        wk->unk1A0 = wk->unk178[6];
        GFL_SndSEPlay(SEQ_SE_CLOSE1);
        wk->unk1C = FALSE;
        return TRUE;
    } else if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_Y) && func_ov131_021d62e4(wk) == TRUE) {
        if (wk->unk58 < 3) {
            if (wk->unk60[wk->unk58] == 1) {
                wk->unk60[wk->unk58] = 0;
                func_0204b878(wk->unk178[3], 6);
            } else {
                wk->unk60[wk->unk58] = 1;
                func_0204b878(wk->unk178[3], 7);
            }
            GFL_SndSEPlay(SEQ_SE_SYS_07);
            wk->unk1C = FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

void func_ov131_021d54a8(PStatusWork *wk) {
    switch (wk->touchHit) {
    case 0:
        if (wk->unk58 != 0) {
            wk->unk58 = 0;
            func_ov131_021d5c98(wk);
            GFL_SndSEPlay(SEQ_SE_SYS_38);
            wk->unk1C = TRUE;
        }
        break;
    case 1:
        if (wk->unk58 != 1 && wk->isEgg == FALSE) {
            wk->unk58 = 1;
            func_ov131_021d5c98(wk);
            GFL_SndSEPlay(SEQ_SE_SYS_38);
            wk->unk1C = TRUE;
        }
        break;
    case 2:
        if (wk->unk58 != 2 && wk->isEgg == FALSE && wk->unk40 == 1) {
            wk->unk58 = 2;
            func_ov131_021d5c98(wk);
            GFL_SndSEPlay(SEQ_SE_SYS_38);
            wk->unk1C = TRUE;
        }
        break;
    case 3:
        if (wk->unk58 < 3) {
            if (func_ov131_021d62e4(wk) == TRUE) {
                if (wk->unk60[wk->unk58] == 1) {
                    wk->unk60[wk->unk58] = 0;
                    func_0204b878(wk->unk178[3], 6);
                } else {
                    wk->unk60[wk->unk58] = 1;
                    func_0204b878(wk->unk178[3], 7);
                }
                GFL_SndSEPlay(SEQ_SE_SYS_07);
                wk->unk1C = TRUE;
            }
        }
        break;
    case 4:
        if (func_ov131_021d5b58(wk, PSTA_DIR_UP) == TRUE) {
            func_ov131_021d5c98(wk);
            func_0204b878(wk->unk178[4], 11);
            func_0204b878(wk->unk178[5], 2);
            wk->unk48 = TRUE;
            wk->unk4C = PSTA_DIR_UP;
            GFL_SndSEPlay(SEQ_SE_SYS_37);
            wk->unk1C = TRUE;
        }
        break;
    case 5:
        if (func_ov131_021d5b58(wk, PSTA_DIR_DOWN) == TRUE) {
            func_ov131_021d5c98(wk);
            func_0204b878(wk->unk178[4], 3);
            func_0204b878(wk->unk178[5], 10);
            wk->unk48 = FALSE;
            wk->unk4C = PSTA_DIR_DOWN;
            GFL_SndSEPlay(SEQ_SE_SYS_37);
            wk->unk1C = TRUE;
        }
        break;
    case 6:
        if (func_ov131_021d62e4(wk) == TRUE) {
            wk->unk54 = PSTATUS_RESULT_CLOSE;
            wk->param->result = PSTATUS_RESULT_CLOSE;
            wk->unk50 = 2;
            func_0204b878(wk->unk178[6], 8);
            wk->unk1A0 = wk->unk178[6];
            GFL_SndSEPlay(SEQ_SE_CLOSE1);
            wk->unk1C = TRUE;
        }
        break;
    case 7:
        wk->unk54 = PSTATUS_RESULT_BACK;
        wk->param->result = PSTATUS_RESULT_BACK;
        wk->unk50 = 2;
        func_0204b878(wk->unk178[7], 9);
        wk->unk1A0 = wk->unk178[7];
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        wk->unk1C = TRUE;
        break;
    }
}

void func_ov131_021d5650(PStatusWork *wk) {
    f32 ratio;
    u8 i;

    if (wk->unk1A4 + 0x400 >= 0x10000) {
        wk->unk1A4 = wk->unk1A4 + 0x400 - 0x10000;
    } else {
        wk->unk1A4 += 0x400;
    }
    {
        u16 startColors[4] = { GX_RGB(20, 20, 22), GX_RGB(16, 16, 18), GX_RGB(12, 12, 14), GX_RGB(13, 13, 14) };
        u16 endColors[4] = { GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), GX_RGB(18, 18, 23), GX_RGB(17, 17, 20) };

        ratio = (f32)((FX_SinIdx(wk->unk1A4) + FX32_ONE) / 2) / 4096.0f;
        for (i = 0; i < 4; i++) {
            s8 r;
            s8 g;
            s8 b;

            r = startColors[i] & GX_RGB_R_MASK;
            g = (startColors[i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
            b = (startColors[i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
            r = r + ratio * ((s8)(endColors[i] & GX_RGB_R_MASK) - r);
            g = g + ratio * ((s8)((endColors[i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT) - g);
            b = b + ratio * ((s8)((endColors[i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT) - b);
            if (r < 0) {
                r = 0;
            }
            if (g < 0) {
                g = 0;
            }
            if (b < 0) {
                b = 0;
            }
            if (r > 31) {
                r = 31;
            }
            if (g > 31) {
                g = 31;
            }
            if (b > 31) {
                b = 31;
            }
            wk->unk1AE[i] = GX_RGB(r, g, b);
        }
        NNS_GfdRegisterNewVramTransferTask(14, 0x6c, wk->unk1AE, sizeof(wk->unk1AE));
    }
    for (i = 0; i < 16; i++) {
        s8 r;
        s8 g;
        s8 b;

        r = wk->unk1B6[i] & GX_RGB_R_MASK;
        g = (wk->unk1B6[i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
        b = (wk->unk1B6[i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
        r = r + ratio * ((s8)(wk->unk1B6[16 + i] & GX_RGB_R_MASK) - r);
        g = g + ratio * ((s8)((wk->unk1B6[16 + i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT) - g);
        b = b + ratio * ((s8)((wk->unk1B6[16 + i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT) - b);
        if (r < 0) {
            r = 0;
        }
        if (g < 0) {
            g = 0;
        }
        if (b < 0) {
            b = 0;
        }
        if (r > 31) {
            r = 31;
        }
        if (g > 31) {
            g = 31;
        }
        if (b > 31) {
            b = 31;
        }
        wk->unk1F6[i] = GX_RGB(r, g, b);
    }
    NNS_GfdRegisterNewVramTransferTask(14, 0x180, wk->unk1F6, sizeof(wk->unk1F6));
    for (i = 12; i <= 14; i++) {
        s8 r;
        s8 g;
        s8 b;
        s8 endR;
        s8 endG;
        s8 endB;

        r = wk->unk236[i] & GX_RGB_R_MASK;
        g = (wk->unk236[i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
        b = (wk->unk236[i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
        endR = (wk->unk236[i] & GX_RGB_R_MASK) + 10;
        endG = ((wk->unk236[i] & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT) + 10;
        endB = ((wk->unk236[i] & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT) + 10;
        if (endR > 31) {
            endR = 31;
        }
        if (endG > 31) {
            endG = 31;
        }
        if (endB > 31) {
            endB = 31;
        }
        r = r + ratio * (endR - r);
        g = g + ratio * (endG - g);
        b = b + ratio * (endB - b);
        if (r < 0) {
            r = 0;
        }
        if (g < 0) {
            g = 0;
        }
        if (b < 0) {
            b = 0;
        }
        if (r > 31) {
            r = 31;
        }
        if (g > 31) {
            g = 31;
        }
        if (b > 31) {
            b = 31;
        }
        wk->unk216[i] = GX_RGB(r, g, b);
    }
    NNS_GfdRegisterNewVramTransferTask(14, 0x1a0, wk->unk216, sizeof(wk->unk216));
}




void func_ov131_021d5a9c(PStatusWork *wk, u32 mode) {
    wk->unk44 = mode;
    if (mode == 0) {
        func_0204b3dc(wk->unk178[0], FALSE);
        func_0204b3dc(wk->unk178[1], FALSE);
        func_0204b3dc(wk->unk178[2], FALSE);
        func_0204b3dc(wk->unk178[3], FALSE);
        func_0204b3dc(wk->unk178[4], FALSE);
        func_0204b3dc(wk->unk178[5], FALSE);
        func_0204b3dc(wk->unk178[6], FALSE);
        return;
    }
    func_0204b3dc(wk->unk178[0], TRUE);
    func_0204b3dc(wk->unk178[1], TRUE);
    if (wk->unk40 == 1) {
        func_0204b3dc(wk->unk178[2], TRUE);
    }
    func_0204b3dc(wk->unk178[4], TRUE);
    func_0204b3dc(wk->unk178[5], TRUE);
    if (func_ov131_021d62e4(wk) == TRUE) {
        func_0204b3dc(wk->unk178[3], TRUE);
        func_0204b3dc(wk->unk178[6], TRUE);
    }
}


BOOL func_ov131_021d5b58(PStatusWork *wk, u8 dir) {
    u16 index;
    BOOL found = func_ov131_021d5bac(wk, dir, &index);

    if (found == TRUE) {
        wk->partyIndex = index;
        if (wk->param->dataType == PSTATUS_DATA_BOX) {
            if (wk->unk258 != NULL) {
                PokeParty_ClearPkm(wk->unk258);
                GFL_HeapFree(wk->unk258);
            }
            wk->unk258 = boxPkmRegenToPartyPkm(func_ov131_021d6064(wk), wk->heapId);
        }
        func_ov131_021d5c4c(wk);
    }
    return found;
}


BOOL func_ov131_021d5bac(PStatusWork *wk, u8 dir, u16 *index) {
    BOOL done = FALSE;
    BOOL found = FALSE;
    u16 start = wk->partyIndex;
    BoxPkm *pkm;

    while (done == FALSE) {
        if (dir == PSTA_DIR_DOWN && wk->partyIndex >= wk->param->partyCount - 1) {
            done = TRUE;
        } else if (dir == PSTA_DIR_UP && wk->partyIndex == 0) {
            done = TRUE;
        } else {
            if (dir == PSTA_DIR_DOWN) {
                wk->partyIndex++;
            } else {
                wk->partyIndex--;
            }
            pkm = func_ov131_021d6064(wk);
            if (PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL) != 0) {
                if (PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL) != TRUE || wk->unk58 == PSTATUS_PAGE_INFO) {
                    if (wk->unk58 != PSTATUS_PAGE_RIBBON || func_ov131_021d62f4(wk, pkm) != FALSE) {
                        done = TRUE;
                        found = TRUE;
                    }
                }
            }
        }
    }
    if (index != NULL) {
        *index = wk->partyIndex;
    }
    wk->partyIndex = start;
    return found;
}

void func_ov131_021d5c4c(PStatusWork *wk) {
    PartyPkm *pkm = func_ov131_021d60a4(wk);
    BoxPkm *boxPkm = func_ov131_021d6064(wk);

    func_ov131_021d60d4(wk, TRUE);
    wk->isEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    wk->happiness = PokeParty_GetParam(pkm, PKM_PARAM_HAPPINESS, NULL);
    wk->unk40 = func_ov131_021d62f4(wk, boxPkm);
    func_ov131_021d60d4(wk, FALSE);
    wk->unk10 = TRUE;
}

void func_ov131_021d5c98(PStatusWork *wk) {
    func_ov131_021d60d4(wk, TRUE);
    if (wk->shownPartyIndex != wk->partyIndex) {
        if (wk->shownPartyIndex != 0xff) {
            func_ov131_021d6cdc(wk, wk->unk88);
            func_ov131_021d91ec(wk, wk->unk94);
        }
        func_ov131_021d697c(wk, wk->unk88);
        func_ov131_021d911c(wk, wk->unk94);
    }
    switch (wk->unk5C) {
    case 0:
        func_ov131_021d7e84(wk, wk->unk8C);
        break;
    case 1:
        func_ov131_021d9d44(wk, wk->unk90);
        break;
    case 2:
        func_ov131_021d909c(wk, wk->unk94);
        break;
    case 3:
        func_ov131_021da84c(wk, wk->unk90);
        break;
    }
    switch (wk->unk58) {
    case 0:
        func_ov131_021d7c38(wk, wk->unk8C);
        break;
    case 1:
        func_ov131_021d9c3c(wk, wk->unk90);
        break;
    case 2:
        func_ov131_021d8f50(wk, wk->unk94);
        break;
    case 3:
        func_ov131_021da6a4(wk, wk->unk90);
        break;
    }
    if (wk->unk5C != wk->unk58) {
        wk->unk25C = 1;
        wk->unk260 = 0;
        G2_BG2Mosaic(TRUE);
        G2S_BG1Mosaic(TRUE);
        G2S_BG2Mosaic(TRUE);
    }
    func_ov131_021d60d4(wk, FALSE);
    wk->unk0C = TRUE;
}

static void func_ov131_021d5dc4(PStatusWork *wk) {
    PartyPkm *pkm;
    u32 species;
    u32 form;
    PokeVoiceChatterInfo chatter;

    if (wk->unk25C == 1) {
        wk->unk260++;
        G2_SetBGMosaicSize(wk->unk260, wk->unk260 * 2);
        G2S_SetBGMosaicSize(wk->unk260, wk->unk260 * 2);
        if (wk->unk260 >= 3) {
            wk->unk25C = 2;
        }
    }
    if (wk->unk25C == 3) {
        wk->unk260--;
        G2_SetBGMosaicSize(wk->unk260, wk->unk260 * 2);
        G2S_SetBGMosaicSize(wk->unk260, wk->unk260 * 2);
        if (wk->unk260 == 0) {
            wk->unk0C = 0;
            wk->unk25C = 0;
            return;
        }
    }
    if (func_0201c6b4(wk->printQueue) != TRUE || (wk->unk25C != 2 && wk->unk25C != 0)) {
        return;
    }
    if (wk->shownPartyIndex != wk->partyIndex) {
        if (wk->shownPartyIndex != 0xff) {
            func_ov131_021d6d04(wk, wk->unk88);
        }
        func_ov131_021d6a38(wk, wk->unk88);
        wk->shownPartyIndex = wk->partyIndex;
    }
    switch (wk->unk5C) {
    case PSTATUS_PAGE_INFO:
        func_ov131_021d7eac(wk, wk->unk8C);
        break;
    case PSTATUS_PAGE_SKILL:
        func_ov131_021d9d94(wk, wk->unk90);
        break;
    case PSTATUS_PAGE_RIBBON:
        func_ov131_021d90c0(wk, wk->unk94);
        break;
    case PSTATUS_PAGE_FORGET:
        func_ov131_021da8bc(wk, wk->unk90);
        break;
    }
    switch (wk->unk58) {
    case PSTATUS_PAGE_INFO:
        func_ov131_021d7cac(wk, wk->unk8C);
        break;
    case PSTATUS_PAGE_SKILL:
        func_ov131_021d9cd0(wk, wk->unk90);
        break;
    case PSTATUS_PAGE_RIBBON:
        func_ov131_021d8fac(wk, wk->unk94);
        break;
    case PSTATUS_PAGE_FORGET:
        func_ov131_021da7a4(wk, wk->unk90);
        break;
    }
    if (wk->isEgg == FALSE && wk->param->mode != PSTATUS_MODE_FORGET_MOVE) {
        func_0204b3dc(wk->unk178[1], TRUE);
        if (wk->unk40 == 0) {
            func_0204b3dc(wk->unk178[2], FALSE);
        } else {
            func_0204b3dc(wk->unk178[2], TRUE);
        }
    } else {
        func_0204b3dc(wk->unk178[1], FALSE);
        func_0204b3dc(wk->unk178[2], FALSE);
    }
    if (wk->unk48 == 0) {
        if (func_ov131_021d5bac(wk, PSTA_DIR_UP, NULL) == TRUE) {
            func_0204b878(wk->unk178[4], 3);
        } else {
            func_0204b878(wk->unk178[4], 17);
        }
    }
    if (wk->unk4C == 0) {
        if (func_ov131_021d5bac(wk, PSTA_DIR_DOWN, NULL) == TRUE) {
            func_0204b878(wk->unk178[5], 2);
        } else {
            func_0204b878(wk->unk178[5], 16);
        }
    }
    if (wk->unk58 < PSTATUS_PAGE_FORGET && func_ov131_021d62e4(wk) == TRUE) {
        if (wk->unk60[wk->unk58] == TRUE) {
            func_0204b878(wk->unk178[3], 7);
        } else {
            func_0204b878(wk->unk178[3], 6);
        }
    }
    if (wk->unk10 == 1) {
        wk->unk10 = 0;
        PokeVoice_Release(0);
        if (wk->isEgg == FALSE) {
            pkm = func_ov131_021d60a4(wk);
            species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
            form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
            PokeVoice_CreateChatterInfo(&chatter);
            PokeVoice_Play(species, form, 64, 0, 0, 0, 0, &chatter);
        }
    }
    if (wk->unk25C == 2) {
        wk->unk25C = 3;
        wk->unk5C = wk->unk58;
        return;
    }
    wk->unk0C = 0;
    wk->unk5C = wk->unk58;
}







// pokebw2's PStatus_GetBoxPkm
BoxPkm *func_ov131_021d6064(PStatusWork *wk) {
    switch (wk->param->dataType) {
    case PSTATUS_DATA_PARTY_PKM:
        return func_020185f0(wk->param->party);
    case PSTATUS_DATA_PARTY:
        return func_020185f0(PokeParty_GetPkm(wk->param->party, wk->partyIndex));
    case PSTATUS_DATA_BOX:
        return (BoxPkm *)((u8 *)wk->param->party + PML_GetPkmRawSize() * wk->partyIndex);
    }
    return NULL;
}

PartyPkm *func_ov131_021d60a4(PStatusWork *wk) {
    switch (wk->param->dataType) {
    case PSTATUS_DATA_PARTY_PKM:
        return wk->param->party;
    case PSTATUS_DATA_PARTY:
        return PokeParty_GetPkm(wk->param->party, wk->partyIndex);
    case PSTATUS_DATA_BOX:
        return wk->unk258;
    }
    return NULL;
}

void func_ov131_021d60d4(PStatusWork *wk, BOOL decrypted) {
    PartyPkm *pkm;
    BoxPkm *boxPkm;

    switch (wk->param->dataType) {
    case PSTATUS_DATA_PARTY_PKM:
        pkm = wk->param->party;
        if (decrypted == TRUE) {
            PokeParty_DecryptPkm(pkm);
        } else {
            PokeParty_EncryptPkm(pkm, TRUE);
        }
        break;
    case PSTATUS_DATA_PARTY:
        pkm = PokeParty_GetPkm(wk->param->party, wk->partyIndex);
        if (decrypted == TRUE) {
            PokeParty_DecryptPkm(pkm);
        } else {
            PokeParty_EncryptPkm(pkm, TRUE);
        }
        break;
    case PSTATUS_DATA_BOX:
        boxPkm = func_ov131_021d6064(wk);
        if (decrypted == TRUE) {
            PokeParty_DecryptPkm(wk->unk258);
            PML_PkmDecrypt(boxPkm);
        } else {
            PokeParty_EncryptPkm(wk->unk258, TRUE);
            PML_PkmReEncrypt(boxPkm, TRUE);
        }
        break;
    }
}



// pokebw2's PStatus_Print
void func_ov131_021d614c(PStatusWork *wk, GFLBitmap *bitmap, u32 msgId, u16 x, u16 y, u16 color) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    func_0201c724(wk->printQueue, bitmap, x, y, str, wk->font, color);
    GFL_StrBufFree(str);
}

// pokebw2's PStatus_PrintToWindow
void func_ov131_021d618c(PStatusWork *wk, BmpWin *window, u32 msgId, u16 x, u16 y, u16 color) {
    func_ov131_021d614c(wk, BmpWin_GetBitmap(window), msgId, x, y, color);
}

// pokebw2's PStatus_PrintFormatted
void func_ov131_021d61b8(PStatusWork *wk, GFLBitmap *bitmap, WordSet *wordSet, u32 msgId, u16 x, u16 y, u16 color) {
    StrBuf *str = GFL_StrBufCreate(16, wk->heapId);
    StrBuf *format = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);

    GFL_WordSetFormatStrbuf(wordSet, str, format);
    func_0201c724(wk->printQueue, bitmap, x, y, str, wk->font, color);
    GFL_StrBufFree(format);
    GFL_StrBufFree(str);
}

void func_ov131_021d6214(PStatusWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, u16 x, u16 y,
                                    u16 color) {
    func_ov131_021d61b8(wk, BmpWin_GetBitmap(window), wordSet, msgId, x, y, color);
}

void func_ov131_021d6244(PStatusWork *wk, GFLBitmap *bitmap, WordSet *wordSet, u32 msgId, u16 x, u16 y,
                                 u16 color) {
    StrBuf *str = GFL_StrBufCreate(16, wk->heapId);
    StrBuf *format = GFL_MsgDataLoadStrbufNew(wk->msgData, msgId);
    u32 width;

    GFL_WordSetFormatStrbuf(wordSet, str, format);
    width = GFL_FontGetBlockWidth(str, wk->font, 0);
    func_0201c724(wk->printQueue, bitmap, x - width, y, str, wk->font, color);
    GFL_StrBufFree(format);
    GFL_StrBufFree(str);
}

void func_ov131_021d62b4(PStatusWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, u16 x, u16 y,
                                         u16 color) {
    func_ov131_021d6244(wk, BmpWin_GetBitmap(window), wordSet, msgId, x, y, color);
}

// pokebw2's PStatus_IsFromFieldMenu
BOOL func_ov131_021d62e4(PStatusWork *wk) {
    if (wk->param->fromFieldMenu == TRUE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's PStatus_HasRibbon
BOOL func_ov131_021d62f4(PStatusWork *wk, BoxPkm *pkm) {
    u8 i;

    for (i = 0; i < RIBBON_COUNT; i++) {
        if (PML_PkmGetParam(pkm, func_ov131_021dbf2c(i, RIBBON_DATA_PARAM), NULL) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}
