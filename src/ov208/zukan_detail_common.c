// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_common.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "save/pokedex.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/palanm.h"

// What the Pokédex detail screen's pages share: the screen's systems and the list of Pokémon it pages through, the
// blending and palette fades of the pages, and the backgrounds that the pages load

static void func_ov208_021bc520(u32 screen, ZukanDetailBlend *blend);
static void func_ov208_021bc7b4(ZukanDetailPalFade *fade, s8 wait, u8 start, u8 end);

// The pages' backgrounds, in the Pokédex's graphics archive, for the main and sub screens
typedef struct {
    u32 nclr;
    u32 ncgr;
    u32 nscr;
} BackgroundFiles;

// The palettes of the backgrounds' files that their bottom three rows and the rest of them take, by screen and page
// pokebw2's sBottomPalettes
static const u8 data_ov208_021c80e8[2][4] = {
    { 0, 3, 2, 4 },
    { 1, 1, 1, 1 },
};

// pokebw2's sTopPalettes
static const u8 data_ov208_021c80e0[2][4] = {
    { 0, 3, 2, 4 },
    { 0, 3, 2, 4 },
};

// pokebw2's sBackgroundFiles
static const BackgroundFiles data_ov208_021c80f0[2] = {
    { 1, 11, 41 },
    { 2, 12, 36 },
};

ZukanDetailCommon *func_ov208_021bc35c(HeapID heapId, GameData *gameData, ZukanDetailGraphic *graphic, Font *font,
                                            PrintQueue *printQueue, ZukanDetailTouchbar *touchbar,
                                            ZukanDetailHeadbar *headbar, const u16 *list, u16 count, u16 *index) {
    ZukanDetailCommon *common = GFL_HeapAllocate(heapId, sizeof(ZukanDetailCommon), TRUE, "zukan_detail_common.c", 96);
    common->gameData = gameData;
    common->graphic = graphic;
    common->font = font;
    common->printQueue = printQueue;
    common->touchbar = touchbar;
    common->headbar = headbar;
    common->list = list;
    common->count = count;
    common->index = index;
    return common;
}

// pokebw2's ZukanDetailCommon_Free
void func_ov208_021bc39c(ZukanDetailCommon *common) {
    GFL_HeapFree(common);
}

// pokebw2's ZukanDetailCommon_GetGameData
GameData *func_ov208_021bc3a4(ZukanDetailCommon *common) {
    return common->gameData;
}

// pokebw2's ZukanDetailCommon_GetGraphic
ZukanDetailGraphic *func_ov208_021bc3a8(ZukanDetailCommon *common) {
    return common->graphic;
}

// pokebw2's ZukanDetailCommon_GetFont
Font *func_ov208_021bc3ac(ZukanDetailCommon *common) {
    return common->font;
}

// pokebw2's ZukanDetailCommon_GetPrintQueue
PrintQueue *func_ov208_021bc3b0(ZukanDetailCommon *common) {
    return common->printQueue;
}

// pokebw2's ZukanDetailCommon_GetTouchbar
ZukanDetailTouchbar *func_ov208_021bc3b4(ZukanDetailCommon *common) {
    return common->touchbar;
}

// pokebw2's ZukanDetailCommon_GetHeadbar
ZukanDetailHeadbar *func_ov208_021bc3b8(ZukanDetailCommon *common) {
    return common->headbar;
}

// pokebw2's ZukanDetailCommon_GetCount
u16 func_ov208_021bc3bc(ZukanDetailCommon *common) {
    return common->count;
}

// pokebw2's ZukanDetailCommon_GetSpecies
u16 func_ov208_021bc3c0(ZukanDetailCommon *common) {
    return common->list[*common->index];
}

// Moves to the next Pokémon of the list that has been seen, wrapping around
// pokebw2's ZukanDetailCommon_GoNext
void func_ov208_021bc3cc(ZukanDetailCommon *common) {
    u16 start = *common->index;
    PokeDexSave *pokedex = GameData_GetPokedex(common->gameData);

    do {
        (*common->index)++;
        if (*common->index >= common->count) {
            *common->index = 0;
        }
    } while (*common->index != start && !PokeDex_IsSeen(pokedex, common->list[*common->index]));
}

// pokebw2's ZukanDetailCommon_GoPrev
void func_ov208_021bc40c(ZukanDetailCommon *common) {
    u16 start = *common->index;
    PokeDexSave *pokedex = GameData_GetPokedex(common->gameData);

    do {
        if (*common->index == 0) {
            *common->index = common->count - 1;
        } else {
            (*common->index)--;
        }
    } while (*common->index != start && !PokeDex_IsSeen(pokedex, common->list[*common->index]));
}

// pokebw2's ZukanDetailBlend_Create
ZukanDetailBlend *func_ov208_021bc448(HeapID heapId) {
    return GFL_HeapAllocate(heapId, sizeof(ZukanDetailBlend), TRUE, "zukan_detail_common.c", 275);
}

// pokebw2's ZukanDetailBlend_Free
void func_ov208_021bc464(ZukanDetailBlend *blend) {
    GFL_HeapFree(blend);
}

// pokebw2's ZukanDetailBlend_Update
void func_ov208_021bc46c(ZukanDetailBlend *main, ZukanDetailBlend *sub) {
    ZukanDetailBlend *blends[2];
    u8 screen;

    blends[0] = main;
    blends[1] = sub;
    for (screen = 0; screen < 2; screen++) {
        ZukanDetailBlend *blend = blends[screen];
        if (blend->step != 0) {
            if (blend->wait == 0) {
                blend->wait = 0;
                blend->ev += blend->step;
                if (blend->step > 0) {
                    if (blend->ev >= 16) {
                        blend->ev = 16;
                        blend->step = 0;
                    }
                } else {
                    if (blend->ev <= 0) {
                        blend->ev = 0;
                        blend->step = 0;
                    }
                }
                func_ov208_021bc520(screen, blend);
            } else {
                blend->wait--;
            }
        }
    }
}

// pokebw2's ZukanDetailBlend_IsActive
BOOL func_ov208_021bc4c8(ZukanDetailBlend *blend) {
    if (blend->step != 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ZukanDetailBlend_StartIn
void func_ov208_021bc4d8(ZukanDetailBlend *blend) {
    blend->ev = 0;
    blend->step = 2;
    blend->wait = 0;
}

// pokebw2's ZukanDetailBlend_StartOut
void func_ov208_021bc4e4(ZukanDetailBlend *blend) {
    blend->ev = 16;
    blend->step = -2;
    blend->wait = 0;
}

// pokebw2's ZukanDetailBlend_SetIn
void func_ov208_021bc4f4(u32 screen, ZukanDetailBlend *blend) {
    blend->ev = 16;
    blend->step = 0;
    blend->wait = 0;
    func_ov208_021bc520(screen, blend);
}

// pokebw2's ZukanDetailBlend_SetOut
void func_ov208_021bc508(u32 screen, ZukanDetailBlend *blend) {
    blend->ev = 0;
    blend->step = 0;
    blend->wait = 0;
    func_ov208_021bc520(screen, blend);
}

// pokebw2's ZukanDetailBlend_InitPlanes
void func_ov208_021bc518(ZukanDetailBlend *blend) {
    blend->plane1 = GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD;
    blend->plane2 = GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD;
}

// pokebw2's ZukanDetailBlend_Apply
static void func_ov208_021bc520(u32 screen, ZukanDetailBlend *blend) {
    if (screen == 0) {
        G2_SetBlendBrightnessExt(blend->plane1, blend->plane2, blend->ev, 0, blend->ev - 16);
    } else {
        G2S_SetBlendBrightnessExt(blend->plane1, blend->plane2, blend->ev, 0, blend->ev - 16);
    }
}

// pokebw2's ZukanDetailPalFade_CreateEx
ZukanDetailPalFade *func_ov208_021bc560(HeapID heapId, u16 buffers) {
    ZukanDetailPalFade *fade = GFL_HeapAllocate(heapId, sizeof(ZukanDetailPalFade), TRUE, "zukan_detail_common.c", 485);
    fade->tcbBuffer = GFL_HeapAllocate(heapId, GFL_TCBMgrCalcAllocSize(2), TRUE, "zukan_detail_common.c", 488);
    fade->tcbMgr = GFL_TCBMgrCreate(2, fade->tcbBuffer);
    fade->palette = func_02021518(heapId);
    func_02021ee4(fade->palette, 1);
    fade->buffers = buffers;
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_MAIN_BG) {
        func_0202155c(fade->palette, 0, 0x1a0, heapId);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_MAIN_OBJ) {
        func_0202155c(fade->palette, 2, 0x100, heapId);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_SUB_BG) {
        func_0202155c(fade->palette, 1, 0x1a0, heapId);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_SUB_OBJ) {
        func_0202155c(fade->palette, 3, 0x1c0, heapId);
    }
    fade->state = ZUKAN_DETAIL_PALFADE_SHOWN;
    return fade;
}

// pokebw2's ZukanDetailPalFade_Create
ZukanDetailPalFade *func_ov208_021bc610(HeapID heapId) {
    return func_ov208_021bc560(heapId, ZUKAN_DETAIL_PALFADE_MAIN_BG | ZUKAN_DETAIL_PALFADE_SUB_BG |
                                                   ZUKAN_DETAIL_PALFADE_MAIN_OBJ | ZUKAN_DETAIL_PALFADE_SUB_OBJ);
}

// pokebw2's ZukanDetailPalFade_Free
void func_ov208_021bc61c(ZukanDetailPalFade *fade) {
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_SUB_OBJ) {
        func_020215a0(fade->palette, 3);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_SUB_BG) {
        func_020215a0(fade->palette, 1);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_MAIN_OBJ) {
        func_020215a0(fade->palette, 2);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_MAIN_BG) {
        func_020215a0(fade->palette, 0);
    }
    func_02021540(fade->palette);
    func_02030da4(fade->tcbMgr);
    GFL_HeapFree(fade->tcbBuffer);
    GFL_HeapFree(fade);
}

// pokebw2's ZukanDetailPalFade_Update
void func_ov208_021bc678(ZukanDetailPalFade *fade) {
    GFL_TCBMgrUpdate(fade->tcbMgr);
    switch (fade->state) {
    case ZUKAN_DETAIL_PALFADE_SHOWN:
    case ZUKAN_DETAIL_PALFADE_HIDDEN:
        break;
    case ZUKAN_DETAIL_PALFADE_FADING_IN:
        if (!func_02021ed8(fade->palette)) {
            fade->state = ZUKAN_DETAIL_PALFADE_SHOWN;
        }
        break;
    case ZUKAN_DETAIL_PALFADE_FADING_OUT:
        if (!func_02021ed8(fade->palette)) {
            fade->state = ZUKAN_DETAIL_PALFADE_HIDDEN;
        }
        break;
    }
}

// pokebw2's ZukanDetailPalFade_VBlank
void func_ov208_021bc6bc(ZukanDetailPalFade *fade) {
    func_02021d50(fade->palette);
}

// pokebw2's ZukanDetailPalFade_IsFading
BOOL func_ov208_021bc6c8(ZukanDetailPalFade *fade) {
    if (fade->state == ZUKAN_DETAIL_PALFADE_FADING_IN || fade->state == ZUKAN_DETAIL_PALFADE_FADING_OUT) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's ZukanDetailPalFade_StartIn
void func_ov208_021bc6d8(ZukanDetailPalFade *fade) {
    if (!func_ov208_021bc6c8(fade)) {
        func_ov208_021bc7b4(fade, -1, 16, 0);
        fade->state = ZUKAN_DETAIL_PALFADE_FADING_IN;
    }
}

// pokebw2's ZukanDetailPalFade_StartOut
void func_ov208_021bc6f8(ZukanDetailPalFade *fade) {
    if (!func_ov208_021bc6c8(fade)) {
        func_ov208_021bc7b4(fade, -1, 0, 16);
        fade->state = ZUKAN_DETAIL_PALFADE_FADING_OUT;
    }
}

// pokebw2's ZukanDetailPalFade_SetHidden
void func_ov208_021bc718(ZukanDetailPalFade *fade) {
    if (!func_ov208_021bc6c8(fade)) {
        func_ov208_021bc7b4(fade, 0, 16, 16);
        fade->state = ZUKAN_DETAIL_PALFADE_HIDDEN;
    }
}

void func_ov208_021bc738(ZukanDetailPalFade *fade, ArcTool *arc, u32 fileId, HeapID heapId, u32 buffer,
                                    u32 size, u16 offset, u16 srcOffset) {
    func_02021660(fade->palette, arc, fileId, heapId, buffer, size, offset, srcOffset);
}

// pokebw2's ZukanDetailPalFade_ReadPalettes
void func_ov208_021bc758(ZukanDetailPalFade *fade) {
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_MAIN_BG) {
        func_020216d4(fade->palette, 0, 0, 0x1a0);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_MAIN_OBJ) {
        func_020216d4(fade->palette, 2, 0, 0x100);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_SUB_BG) {
        func_020216d4(fade->palette, 1, 0, 0x1a0);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_SUB_OBJ) {
        func_020216d4(fade->palette, 3, 0, 0x1c0);
    }
}

// pokebw2's ZukanDetailPalFade_Start
static void func_ov208_021bc7b4(ZukanDetailPalFade *fade, s8 wait, u8 start, u8 end) {
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_MAIN_BG) {
        func_0202173c(fade->palette, ZUKAN_DETAIL_PALFADE_MAIN_BG, 0x1fff, wait, start, end, 0, fade->tcbMgr);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_MAIN_OBJ) {
        func_0202173c(fade->palette, ZUKAN_DETAIL_PALFADE_MAIN_OBJ, 0xff, wait, start, end, 0, fade->tcbMgr);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_SUB_BG) {
        func_0202173c(fade->palette, ZUKAN_DETAIL_PALFADE_SUB_BG, 0x1fff, wait, start, end, 0, fade->tcbMgr);
    }
    if (fade->buffers & ZUKAN_DETAIL_PALFADE_SUB_OBJ) {
        func_0202173c(fade->palette, ZUKAN_DETAIL_PALFADE_SUB_OBJ, 0x3fff, wait, start, end, 0, fade->tcbMgr);
    }
}

// Loads a page's background on a BG, with its bottom rows' colors in bottomPalette and the rest's in topPalette
// pokebw2's ZukanDetailBackground_Create
ZukanDetailBackground *func_ov208_021bc844(HeapID heapId, u32 page, u8 bg, u8 bottomPalette, u8 topPalette) {
    ZukanDetailBackground *background =
        GFL_HeapAllocate(heapId, sizeof(ZukanDetailBackground), TRUE, "zukan_detail_common.c", 803);
    ArcTool *arc;
    u32 screen = 1;
    u32 palType;
    u32 nscr;

    if (bg < 4) {
        screen = 0;
        palType = PALTYPE_MAIN_BG;
    } else {
        palType = PALTYPE_SUB_BG;
    }
    arc = GFL_ArcSysCreateFileHandle(ARCID_ZUKAN_GRA, heapId);
    GFL_G2DIOLoadArcNCLR(arc, data_ov208_021c80f0[screen].nclr, palType, data_ov208_021c80e8[screen][page] * 32,
                         bottomPalette * 32, 32, heapId);
    GFL_G2DIOLoadArcNCLR(arc, data_ov208_021c80f0[screen].nclr, palType, data_ov208_021c80e0[screen][page] * 32, topPalette * 32,
                         32, heapId);
    background->chars = GFL_BGSysLoadArcNCGRDynamic(arc, data_ov208_021c80f0[screen].ncgr, bg, 0, FALSE, heapId);
    nscr = data_ov208_021c80f0[screen].nscr;
    if (page == 3 && screen == 0) {
        nscr = 32;
    }
    GFL_G2DIOLoadNSCRSync(arc, nscr, bg, 0, CHAR_POS(background->chars), 0, FALSE, heapId);
    GFL_BGSysSetScrPaletteNo(bg, 0, 21, 32, 3, bottomPalette);
    GFL_BGSysSetScrPaletteNo(bg, 0, 0, 32, 21, topPalette);
    GFL_ArcToolFree(arc);
    GFL_BGSysQueueScrLoad(bg);
    background->bg = bg;
    background->wait = 4;
    return background;
}

// pokebw2's ZukanDetailBackground_Free
void func_ov208_021bc97c(ZukanDetailBackground *background) {
    GFL_BGSysFreeCharMemory(background->bg, CHAR_POS(background->chars), CHAR_SIZE(background->chars));
    GFL_HeapFree(background);
}

// Scrolls the background by a pixel every few frames
// pokebw2's ZukanDetailBackground_Update
void func_ov208_021bc99c(ZukanDetailBackground *background) {
    if (background->wait == 0) {
        GFL_BGSysMoveBGReq(background->bg, BG_MOVE_RIGHT, 1);
        background->wait = 4;
    } else {
        background->wait--;
    }
}

// Loads a BG's palette, characters and screen from an archive. The characters are only loaded, and returned, when
// loaded is FALSE; otherwise chars gives them
u32 func_ov208_021bc9bc(BOOL loaded, HeapID heapId, u8 bg, u32 palettes, u8 palette, u8 srcPalette, u32 arcId, u32 nclr,
                       u32 ncgr, u32 nscr, u32 chars) {
    u32 palType = PALTYPE_MAIN_BG;
    ArcTool *arc;

    if (bg >= 4) {
        palType = PALTYPE_SUB_BG;
    }
    arc = GFL_ArcSysCreateFileHandle(arcId, heapId);
    if (!loaded) {
        GFL_G2DIOLoadArcNCLR(arc, nclr, palType, srcPalette * 32, palette * 32, palettes * 32, heapId);
    }
    if (!loaded) {
        chars = GFL_BGSysLoadArcNCGRDynamic(arc, ncgr, bg, 0, FALSE, heapId);
    }
    GFL_G2DIOLoadNSCRSync(arc, nscr, bg, 0, CHAR_POS(chars), 0, FALSE, heapId);
    GFL_BGSysSetScrPaletteNo(bg, 0, 0, 32, 24, palette);
    GFL_ArcToolFree(arc);
    GFL_BGSysQueueScrLoad(bg);
    return chars;
}

// pokebw2's ZukanDetail_FreeBG
void func_ov208_021bca54(u32 bg, u32 chars) {
    GFL_BGSysFreeCharMemory(bg, CHAR_POS(chars), CHAR_SIZE(chars));
}
