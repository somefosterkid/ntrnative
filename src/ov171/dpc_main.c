// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov256/dpc_main.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/dpc_main.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "save/hall_of_fame.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "app/dendou_pc.h"

// The Hall of Fame PC's set-up: the display, the palette fade of the Pokémon, the text and the teams. The ROM doesn't
// name this file; the name is a guess, after box2_main.c

static void func_ov171_021dc8e4(TCB *tcb, void *data);

// pokebw2's sDpcMainVramBanks
static const BGSysVRAMConfig data_ov171_021deb20 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// pokebw2's DpcMain_InitVBlank
void func_ov171_021dc8b8(DendouPCWork *wk) {
    wk->vblankTask = GFL_VBlankTCBAdd(func_ov171_021dc8e4, wk, 0);
}

// pokebw2's DpcMain_ExitVBlank
void func_ov171_021dc8d4(DendouPCWork *wk) {
    GFL_TCBRemove(wk->vblankTask);
}

// pokebw2's DpcMain_VBlank
static void func_ov171_021dc8e4(TCB *tcb, void *data) {
    DendouPCWork *wk = data;

    GFL_BGSysUpdate();
    func_0204a648();
    func_02021d50(wk->palFade);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

// pokebw2's DpcMain_InitVram
void func_ov171_021dc910(void) {
    GFL_BGSysInitVRAM(GX_VRAM_LCDC_NONE);
    GFL_BGSysSetVRAMBanks(&data_ov171_021deb20);
}

// pokebw2's DpcMain_GetVramBanks
const BGSysVRAMConfig *func_ov171_021dc924(void) {
    return &data_ov171_021deb20;
}

// pokebw2's DpcMain_InitBg
void func_ov171_021dc92c(void) {
    GFL_BGSysCreate(HEAPID_DENDOU_PC);

    {
        BGSysLCDConfig config = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };
        GFL_BGSysSetLCDConfig(&config);
    }

    // The touch screen, which the main engine drives
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
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x1000,
            0,
            BGRES_512x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
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
            GX_BG_SCRBASE_0xd800,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
    }

    // The top screen
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
        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
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
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };
        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
    }

    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, TRUE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1, TRUE);
}

// pokebw2's DpcMain_ExitBg
void func_ov171_021dca40(void) {
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    GFL_BGSysFree();
}

// pokebw2's DpcMain_LoadBgGraphic
void func_ov171_021dca7c(void) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_DENDOU_PC, HEAPID_TAIL(HEAPID_DENDOU_PC));

    GFL_G2DIOLoadArcNCLRDefault(arc, 1, PALTYPE_MAIN_BG, 0, 0x80, HEAPID_DENDOU_PC);
    GFL_G2DIOLoadArcNCLRDefault(arc, 6, PALTYPE_SUB_BG, 0, 0x40, HEAPID_DENDOU_PC);
    GFL_BGSysLoadArcNCGRStatic(arc, 0, 0, 0, 0, TRUE, HEAPID_DENDOU_PC);
    GFL_BGSysLoadArcNCGRStatic(arc, 5, 4, 0, 0, TRUE, HEAPID_DENDOU_PC);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 2, 0, 0, 0, TRUE, HEAPID_DENDOU_PC);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 3, 1, 0, 0, TRUE, HEAPID_DENDOU_PC);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 4, 2, 0, 0, TRUE, HEAPID_DENDOU_PC);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 7, 4, 0, 0, TRUE, HEAPID_DENDOU_PC);
    GFL_ArcToolFree(arc);

    // The title's window takes the bottom rows of the background's screen
    GFL_BGSysLoadScrAreaAll(3, (u16 *)GFL_BGSysIsScrHeapExists(0) + 21 * 32, 0, 21, 32, 3);

    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_MAIN_BG, 15 * 0x20, 0x20, HEAPID_DENDOU_PC);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, PALTYPE_SUB_BG, 15 * 0x20, 0x20, HEAPID_DENDOU_PC);
}

// pokebw2's DpcMain_InitPaletteFade
void func_ov171_021dcb70(DendouPCWork *wk) {
    wk->palFade = func_02021518(HEAPID_DENDOU_PC);
    func_0202155c(wk->palFade, PALFADE_BUFFER_MAIN_OBJ, 0x200, HEAPID_DENDOU_PC);
}

// pokebw2's DpcMain_ExitPaletteFade
void func_ov171_021dcb8c(DendouPCWork *wk) {
    func_020215a0(wk->palFade, PALFADE_BUFFER_MAIN_OBJ);
    func_02021540(wk->palFade);
}

// pokebw2's DpcMain_BlendPokePalettes
void func_ov171_021dcba4(DendouPCWork *wk) {
    u32 i;

    // Both sets of actors, of which one is shown
    for (i = 0; i < HALL_OF_FAME_TEAM_SIZE; i++) {
        func_02022004(wk->palFade, PALFADE_BUFFER_MAIN_OBJ,
                                  (1 << i) | (1 << (i + HALL_OF_FAME_TEAM_SIZE)), wk->fadeNow[i], 0);
    }
    func_02021ee4(wk->palFade, TRUE);
}

// pokebw2's DpcMain_IsPaletteFading
BOOL func_ov171_021dcbe8(DendouPCWork *wk) {
    if (func_02021ed8(wk->palFade) != 0) {
        return TRUE;
    }
    func_02021ee4(wk->palFade, FALSE);
    return FALSE;
}

// pokebw2's DpcMain_SetBlend
void func_ov171_021dcc0c(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_OBJ,
                     16, 8);
}

// pokebw2's DpcMain_InitMsg
void func_ov171_021dcc24(DendouPCWork *wk) {
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_POKEPARAM_3, HEAPID_DENDOU_PC);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_DENDOU_PC);
    wk->smallFont = GFL_FontCreate(ARCID_FONT, 3, 0, FALSE, HEAPID_DENDOU_PC);
    wk->wordSet = GFL_WordSetSystemCreateDefault(HEAPID_DENDOU_PC);
    wk->printQueue = func_0201c440(HEAPID_DENDOU_PC);
    wk->expandBuf = GFL_StrBufCreate(0x400, HEAPID_DENDOU_PC);
}

// pokebw2's DpcMain_ExitMsg
void func_ov171_021dcc88(DendouPCWork *wk) {
    GFL_StrBufFree(wk->expandBuf);
    func_0201c4c0(wk->printQueue);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_FontFree(wk->smallFont);
    GFL_FontFree(wk->font);
    GFL_MsgDataFree(wk->msgData);
}

// The buffers that the save's functions copy the Pokémon's names into
static inline void DpcMain_CreateTeamStrings(DendouPCWork *wk) {
    u16 i, j;

    for (i = 0; i < HALL_OF_FAME_TEAMS; i++) {
        for (j = 0; j < HALL_OF_FAME_TEAM_SIZE; j++) {
            wk->teams[i].pokemon[j].nickname = GFL_StrBufCreate(0x20, HEAPID_DENDOU_PC);
            wk->teams[i].pokemon[j].trainerName = GFL_StrBufCreate(0x20, HEAPID_DENDOU_PC);
        }
    }
}

// pokebw2's DpcMain_LoadTeams
void func_ov171_021dccc8(DendouPCWork *wk) {
    u16 i, j;

    DpcMain_CreateTeamStrings(wk);

    if (wk->param->mode == DENDOU_PC_MODE_FIRST_CLEAR) {
        HallOfFameTeam *team = func_02012f68(wk->param->gameData);

        wk->teams[0].count = func_0200ec14(team);
        func_0200ec98(team, &wk->teams[0].date);
        for (i = 0; i < wk->teams[0].count; i++) {
            func_0200ec2c(team, i, &wk->teams[0].pokemon[i]);
        }
        wk->pageMax = 1;
    } else {
        SaveControl *save = GameData_GetSaveControl(wk->param->gameData);

        if (func_0200725c(save, SAVE_EXTRA_ID_HALL_OF_FAME, HEAPID_DENDOU_PC) == 1) {
            HallOfFameSave *hallOfFame = getAddressOfExtraSaveBlk(save, SAVE_EXTRA_ID_HALL_OF_FAME, 0);

            if (hallOfFame != NULL) {
                for (i = 0; i < HALL_OF_FAME_TEAMS; i++) {
                    wk->teams[i].count = func_0200ecd0(hallOfFame, i);
                    wk->teams[i].number = func_0200ecc4(hallOfFame, i);
                    func_0200ed14(hallOfFame, i, &wk->teams[i].date);
                    if (wk->teams[i].count != 0) {
                        for (j = 0; j < wk->teams[i].count; j++) {
                            func_0200ecf0(hallOfFame, i, j, &wk->teams[i].pokemon[j]);
                        }
                        wk->pageMax++;
                    }
                }
            }
        }
        freeIntermediateSaveExtraBlksAfterLoad(save, SAVE_EXTRA_ID_HALL_OF_FAME);
    }
}

// pokebw2's DpcMain_FreeTeams
void func_ov171_021dce28(DendouPCWork *wk) {
    u16 i, j;

    for (i = 0; i < HALL_OF_FAME_TEAMS; i++) {
        for (j = 0; j < HALL_OF_FAME_TEAM_SIZE; j++) {
            GFL_StrBufFree(wk->teams[i].pokemon[j].nickname);
            GFL_StrBufFree(wk->teams[i].pokemon[j].trainerName);
        }
    }
}
