// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov142/itemmenu_disp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/itemmenu_disp.h"
#include "app/itemmenu.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/sound.h"
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
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "pml/item.h"
#include "pml/waza.h"
#include "save/bag.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/app_keycursor.h"
#include "system/app_menu_common.h"
#include "system/blink_palanm.h"
#include "system/scroll_bar.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "field/beacon_view.h"
#include "gfl/str.h"

// The bag's screens: BGs, windows, OBJ, the drawing of the item list and of the item under the cursor, and the menus'
// windows. The ROM doesn't name this file; it is named after itemmenu.c for what it holds

// The colors of the text
#define ITEMMENU_TEXT_COLOR PRINT_COLOR(15, 14, 0)
#define ITEMMENU_TITLE_COLOR PRINT_COLOR(15, 2, 0)
#define ITEMMENU_MENU_COLOR PRINT_COLOR(14, 15, 0)

void func_ov070_021b8f98(void);
void func_ov070_021b9178(ItemMenuWork *work, ArcTool *arc);
void func_ov070_021b91fc(ItemMenuWork *work, ArcTool *arc);
void func_ov070_021b9ba0(ItemMenuWork *work);
void func_ov070_021b9de8(ItemMenuWork *work);
void func_ov070_021bb6e0(ItemMenuWork *work, PrintWindow *window, StrBuf *strbuf, u16 x, s16 y, u16 color);
void func_ov070_021bb724(PrintWindow *window);
void func_ov070_021bafc4(ItemMenuWork *work);
s32 func_ov070_021b61b8(ItemMenuWork *work);
BagItem *func_ov070_021b616c(ItemMenuWork *work, s32 index);
void func_ov070_021b9e14(ItemMenuWork *work, u32 item);
BOOL func_ov070_021ba1d0(u16 item);

// Each data table is declared just before the function that first uses it, which gives the original's .rodata order

void func_ov070_021b61f4(ItemMenuWork *work, u32 index, u32 item);
s32 func_ov070_021b61c8(ItemMenuWork *work);
void func_ov070_021baf38(ItemMenuWork *work, u32 pocket);
void func_ov070_021bb580(ItemMenuWork *work, BOOL keys);
void func_ov070_021bb3f4(ItemMenuWork *work, BOOL active);
void func_ov070_021bb5a4(ItemMenuWork *work);
void func_ov070_021bb398(ItemMenuWork *work, u32 palette);
void func_ov070_021ba6a4(ItemMenuWork *work);
void func_ov070_021bab04(ItemMenuWork *work, u32 move);
void func_ov070_021ba6f4(ItemMenuWork *work);

// pokebw2's ItemMenuDisp_InitBGs
void func_ov070_021b8f98(void) {
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
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
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(0);
        GFL_BGSysLoadScr(0);
        GFL_BGSysSetBGEnabled(0, FALSE);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(1);
        GFL_BGSysLoadScr(1);
        GFL_BGSysSetBGEnabled(1, FALSE);
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
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(2);
        GFL_BGSysLoadScr(2);
        GFL_BGSysSetBGEnabled(2, FALSE);
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
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(3, 0, 1, 0);
        GFL_BGSysClearBG(3);
        GFL_BGSysLoadScr(3);
        GFL_BGSysSetBGEnabled(3, FALSE);
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
            3,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(4, FALSE);
        GFL_BGSysLoadScr(4);
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
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(5, FALSE);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            GX_BG_AREAOVER_XLU,
            FALSE,
        };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysSetBGEnabled(6, FALSE);
        GFL_BGSysFillChar(6, 0, 1, 0);
        GFL_BGSysClearScr(6);
    }
}

// pokebw2's ItemMenuDisp_ShowBGs
void func_ov070_021b913c(void) {
    GFL_BGSysSetBGEnabled(0, TRUE);
    GFL_BGSysSetBGEnabled(1, TRUE);
    GFL_BGSysSetBGEnabled(2, TRUE);
    GFL_BGSysSetBGEnabled(3, TRUE);
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysSetBGEnabled(5, TRUE);
    GFL_BGSysSetBGEnabled(6, TRUE);
}

// Loads the bag's picture for the player's gender
// pokebw2's ItemMenuDisp_LoadBagPicture
void func_ov070_021b9178(ItemMenuWork *work, ArcTool *arc) {
    u32 gender;
    u32 palette;
    u32 chars;
    u32 screen;

    gender = getTrainerGender(work->playerInfo);
    if (gender == 0) {
        palette = 4;
        chars = 3;
        screen = 5;
    } else if (gender == 1) {
        palette = 1;
        chars = 0;
        screen = 2;
    }
    GFL_G2DIOLoadArcNCLRDefault(arc, palette, 0, 0, 0x20, work->heapId);
    work->bg0Chars = GFL_BGSysLoadArcNCGRDynamic(arc, chars, 0, 0, 0, work->heapId);
    GFL_G2DIOLoadNSCRSync(arc, screen, 0, 0, (u16)work->bg0Chars, 0, 0, work->heapId);
}

// Loads the pocket tabs' OBJ for the player's gender
// pokebw2's ItemMenuDisp_LoadPocketTabRes
void func_ov070_021b91fc(ItemMenuWork *work, ArcTool *arc) {
    u32 gender;
    u32 palette;
    u32 chars;
    u32 cells;
    u32 anims;

    gender = getTrainerGender(work->playerInfo);
    if (gender == 0) {
        palette = 16;
        chars = 15;
        cells = 14;
        anims = 13;
    } else if (gender == 1) {
        palette = 12;
        chars = 11;
        cells = 10;
        anims = 9;
    }
    work->pocketTabPlt = func_0204ac18(arc, palette, 0, 0xc0, 0, 4, work->heapId);
    work->pocketTabChr = func_0204a6c8(arc, chars, 0, 0, work->heapId);
    work->pocketTabCel = func_0204af28(arc, cells, anims, work->heapId);
}

static const ClActSysSetup sItemMenuClActSetup = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 100, 100, 100, 100, 16, 16 };

static BGSysVRAMConfig sItemMenuVRAMConfig = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

void func_ov070_021b9280(ItemMenuWork *work) {
    ClActorSetup categorySetup;
    ClActorSetup typeSetup;
    ClActorSetup sortSetup;
    ArcTool *arc;
    u32 i;

    G2_BlendNone();
    GFL_BGSysCreate(work->heapId);
    BmpWin_InitAllocator(work->heapId);
    func_0201dd60();
    GFL_BGSysSetVRAMBanks(&sItemMenuVRAMConfig);
    ClActSys_Create(&sItemMenuClActSetup, &sItemMenuVRAMConfig, work->heapId);
    {
        BGSysLCDConfig lcd = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_0, GX_BG0_AS_2D };

        GFL_BGSysSetLCDConfig(&lcd);
    }
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    func_ov070_021b8f98();
    func_0201dd68();
    work->cursorImageChars = LoadCursorImageEndOfHeap(3, 13, 0, work->heapId);

    arc = GFL_ArcSysCreateFileHandle(88, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 8, PALTYPE_SUB_BG, 0, 0, work->heapId);
    if (getTrainerGender(work->playerInfo) == 0) {
        GFL_G2DIOLoadArcNCLR(arc, 8, PALTYPE_SUB_BG, 0x20, 0, 0x20, work->heapId);
    }
    work->bg4Chars = GFL_BGSysLoadArcNCGRDynamic(arc, 6, 4, 0, FALSE, work->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 7, 4, 0, CHAR_POS(work->bg4Chars), 0, FALSE, work->heapId);
    func_ov070_021b9178(work, arc);
    work->bg5Chars = GFL_BGSysLoadArcNCGRDynamic(arc, 17, 5, 0, FALSE, work->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 23, 5, 0, CHAR_POS(work->bg5Chars), 0, FALSE, work->heapId);
    func_ov070_021b91fc(work, arc);
    work->bg3Chars = GFL_BGSysLoadArcNCGRDynamic(arc, 33, 3, 0, FALSE, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 34, PALTYPE_MAIN_BG, 0x60, 0x20, work->heapId);
    GFL_ArcToolFree(arc);

    GFL_BGSysLoadNCLRDefault(23, 5, PALTYPE_MAIN_BG, 0x180, 0x20, work->heapId);
    work->actorUnit = func_0204b100(52, 0, work->heapId);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    GFL_BGSysSetBGEnabledA(GX_PLANEMASK_OBJ, TRUE);

    arc = GFL_ArcSysCreateFileHandle(83, work->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 27, PALTYPE_MAIN_BG, 0x100, 0x20, work->heapId);
    work->bg1Chars = GFL_BGSysLoadArcNCGRDynamic(arc, 28, 1, 0, FALSE, work->heapId);
    GFL_G2DIOLoadNSCRSync(arc, 29, 1, 0, CHAR_POS(work->bg1Chars) + 0x8000, 0, FALSE, work->heapId);
    GFL_BGSysLoadScr(1);
    work->buttonPlt = func_0204ac18(arc, 19, 0, 0x140, 0, 3, work->heapId);
    work->buttonChr = func_0204a6c8(arc, 20, FALSE, 0, work->heapId);
    work->buttonCel = func_0204af28(arc, 23, 26, work->heapId);
    GFL_ArcToolFree(arc);
    func_ov070_021ba6f4(work);

    arc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), work->heapId);
    work->typeIconPlt = func_0204ac18(arc, func_020275e8(), 1, 0x80, 0, 3, work->heapId);
    work->typeIconCel = func_0204af28(arc, func_020275fc(2), func_02027600(2), work->heapId);
    for (i = 0; i < NELEMS(work->categoryIconChr); i++) {
        work->categoryIconChr[i] = func_0204a6c8(arc, func_02027610(i), FALSE, 1, work->heapId);
    }
    for (i = 0; i < NELEMS(work->typeIconChr); i++) {
        work->typeIconChr[i] = func_0204a6c8(arc, func_020275f8(i), FALSE, 1, work->heapId);
    }
    GFL_ArcToolFree(arc);

    categorySetup.x = 80;
    categorySetup.y = 184;
    categorySetup.priority = 1;
    categorySetup.bgPriority = 1;
    categorySetup.sequence = 0;
    for (i = 0; i < NELEMS(work->categoryIcons); i++) {
        work->categoryIcons[i] = func_0204b294(work->actorUnit, work->categoryIconChr[i], work->typeIconPlt,
                                               work->typeIconCel, &categorySetup, 1, work->heapId);
        func_0204b6f4(work->categoryIcons[i], func_02027604(i), 1);
        func_0204b3dc(work->categoryIcons[i], FALSE);
    }
    typeSetup.x = 82;
    typeSetup.y = 165;
    typeSetup.priority = 1;
    typeSetup.bgPriority = 1;
    typeSetup.sequence = 0;
    for (i = 0; i < NELEMS(work->typeIcons); i++) {
        work->typeIcons[i] = func_0204b294(work->actorUnit, work->typeIconChr[i], work->typeIconPlt, work->typeIconCel,
                                           &typeSetup, 1, work->heapId);
        func_0204b6f4(work->typeIcons[i], func_020275ec(i), 1);
        func_0204b3dc(work->typeIcons[i], FALSE);
    }

    arc = GFL_ArcSysCreateFileHandle(88, work->heapId);
    work->sortButtonPlt = func_0204ac18(arc, 39, 0, 0x80, 0, 2, work->heapId);
    work->sortButtonChr = func_0204a6c8(arc, 38, FALSE, 0, work->heapId);
    work->sortButtonCel = func_0204af28(arc, 37, 36, work->heapId);
    GFL_ArcToolFree(arc);
    sortSetup.priority = 10;
    sortSetup.bgPriority = 1;
    sortSetup.x = 146;
    sortSetup.y = 176;
    sortSetup.sequence = 0;
    work->sortButton = func_0204b294(work->actorUnit, work->sortButtonChr, work->sortButtonPlt, work->sortButtonCel,
                                     &sortSetup, 0, work->heapId);
    func_0204b92c(work->sortButton, TRUE);
    func_0204b3dc(work->sortButton, TRUE);
    func_ov070_021bb5a4(work);
}




// pokebw2's ItemMenuDisp_FreeBGChars
void func_ov070_021b9758(ItemMenuWork *work) {
    GFL_BGSysFreeCharMemory(4, (u16)work->bg4Chars, (u16)(work->bg4Chars >> 16));
    GFL_BGSysFreeCharMemory(5, (u16)work->bg5Chars, (u16)(work->bg5Chars >> 16));
    GFL_BGSysFreeCharMemory(3, (u16)work->bg3Chars, (u16)(work->bg3Chars >> 16));
    GFL_BGSysFreeCharMemory(0, (u16)work->bg0Chars, (u16)(work->bg0Chars >> 16));
    GFL_BGSysFreeCharMemory(1, (u16)work->bg1Chars, (u16)(work->bg1Chars >> 16));
}

// Shows or hides the TM's move data on the lower screen
// pokebw2's ItemMenuDisp_ShowTMInfoBGs
void func_ov070_021b97cc(ItemMenuWork *work, BOOL show) {
    GFL_BGSysSetBGEnabled(5, show);
    GFL_BGSysSetBGEnabled(6, show);
}

void func_ov070_021b97e8(ItemMenuWork *work, BOOL keys) {
    BagItem *slot;
    u16 move;

    if (keys == TRUE) {
        work->tmInfoRequest = 1;
    } else {
        work->tmInfoRequest = 2;
    }
    if (work->selIcon != NULL) {
        func_0204b3dc(work->selIcon, keys);
    }
    slot = func_ov070_021b616c(work, func_ov070_021b61b8(work));
    move = PML_ItemGetTMWazaID(slot->item);
    if (move != 0) {
        func_0204b3dc(work->categoryIcons[PML_MoveGetCategory(move)], keys);
        func_0204b3dc(work->typeIcons[PML_MoveGetType(move)], keys);
    }
}

void func_ov070_021b9850(ItemMenuWork *work) {
    BagItem *slot;
    u32 i;
    u16 move;
    u32 hm;

    for (i = 0; i < NELEMS(work->categoryIcons); i++) {
        func_0204b3dc(work->categoryIcons[i], FALSE);
    }
    for (i = 0; i < NELEMS(work->typeIcons); i++) {
        func_0204b3dc(work->typeIcons[i], FALSE);
    }
    slot = func_ov070_021b616c(work, func_ov070_021b61b8(work));
    if (slot == NULL || slot->item == ITEM_NONE) {
        func_ov070_021b97e8(work, FALSE);
        return;
    }
    func_ov070_021b97e8(work, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(work->itemNameWindow.window), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(work->countWindow.window), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(work->descWindow.window), 0);
    move = PML_ItemGetTMWazaID(slot->item);
    if (move == 0) {
        GFL_MsgDataLoadStrbuf(work->msgData, 134, work->strbuf);
        func_ov070_021b61f4(work, 0, slot->item);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021bb6e0(work, &work->itemNameWindow, work->expandBuf, 0, 4, ITEMMENU_TEXT_COLOR);
        if (work->pocket != BAG_POCKET_KEY_ITEMS) {
            GFL_MsgDataLoadStrbuf(work->msgData, 131, work->strbuf);
            WordSetNumber(work->wordSet, 0, slot->count, 3, 0, TRUE);
            GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
            func_ov070_021bb6e0(work, &work->countWindow, work->expandBuf, 0, 4, ITEMMENU_TEXT_COLOR);
        } else {
            BmpWin_FlushChar(work->countWindow.window);
        }
        func_020456f8(work->tmInfoWindow.window);
    } else {
        hm = PML_ItemGetHMID(slot->item);
        if (hm == 0xff) {
            GFL_MsgDataLoadStrbuf(work->msgData, 69, work->strbuf);
            WordSetNumber(work->wordSet, 0, PML_ItemGetTMBitMask(slot->item) + 1, 2, 2, TRUE);
        } else {
            GFL_MsgDataLoadStrbuf(work->msgData, 71, work->strbuf);
            WordSetNumber(work->wordSet, 0, hm + 1, 2, 2, TRUE);
        }
        loadMoveNameToStrbuf(work->wordSet, 1, move);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021bb6e0(work, &work->itemNameWindow, work->expandBuf, 0, 4, ITEMMENU_TEXT_COLOR);
        func_ov070_021bab04(work, move);
        BmpWin_FlushChar(work->countWindow.window);
    }
    setItemDescriptionTextToStrbuf(work->strbuf, slot->item, work->heapId);
    func_ov070_021bb6e0(work, &work->descWindow, work->strbuf, 0, 4, ITEMMENU_TEXT_COLOR);
    func_ov070_021bb724(&work->itemNameWindow);
    func_ov070_021bb724(&work->countWindow);
    func_ov070_021bb724(&work->descWindow);
    func_ov070_021b9e14(work, slot->item);
}



// Hides the item's data on the lower screen
// pokebw2's ItemMenuDisp_HideItemInfo
void func_ov070_021b9aac(ItemMenuWork *work) {
    u32 i;

    func_ov070_021b97e8(work, FALSE);
    func_020456f8(work->tmInfoWindow.window);
    for (i = 0; i < NELEMS(work->categoryIcons); i++) {
        func_0204b3dc(work->categoryIcons[i], FALSE);
    }
    for (i = 0; i < NELEMS(work->typeIcons); i++) {
        func_0204b3dc(work->typeIcons[i], FALSE);
    }
}

// pokebw2's ItemMenuDisp_Exit
void func_ov070_021b9af8(ItemMenuWork *work) {
    s32 i;

    BmpWin_Free(work->itemNameWindow.window);
    BmpWin_Free(work->descWindow.window);
    BmpWin_Free(work->countWindow.window);
    BmpWin_Free(work->tmInfoWindow.window);
    BmpWin_Free(work->quantityWindow.window);
    BmpWin_Free(work->priceWindow.window);
    BmpWin_Free(work->menuTitleWindow.window);
    func_0204b3b4(work->scrollBar);
    if (work->selIcon != NULL) {
        func_0204b3b4(work->selIcon);
        func_0204a8d4(work->selIconChr);
        func_0204ada4(work->selIconPlt);
    }
    func_0204afd8(work->selIconCel);
    for (i = 0; i < ITEMMENU_ROW_OBJS; i++) {
        GFL_BitmapFree(work->rowNameBitmaps[i]);
    }
    func_0204b1cc(work->actorUnit);
    func_0204a5ac();
}

void func_ov070_021b9ba0(ItemMenuWork *work) {
    GFL_MsgDataLoadStrbuf(work->msgData, 0x87, work->strbuf);
    func_ov070_021bb6e0(work, &work->menuTitleWindow, work->strbuf, 0, 4, 0x3dc0);
}


// Creates the windows
// pokebw2's ItemMenuDisp_CreateWindows
void func_ov070_021b9bdc(ItemMenuWork *work) {
    ArcTool *arc;

    work->menuTitleWindow.window = BmpWin_CreateDynamic(3, 18, 9, 12, 3, 3, TRUE);
    func_ov070_021b9ba0(work);
    func_020456f8(work->menuTitleWindow.window);
    work->quantityWindow.window = BmpWin_CreateDynamic(3, 17, 13, 11, 2, 3, TRUE);
    work->priceWindow.window = BmpWin_CreateDynamic(3, 17, 15, 11, 2, 3, TRUE);
    work->tmInfoWindow.window = BmpWin_CreateDynamic(6, 0, 19, 32, 5, 0, TRUE);
    work->itemNameWindow.window = BmpWin_CreateDynamic(6, 7, 5, 18, 3, 0, TRUE);
    work->countWindow.window = BmpWin_CreateDynamic(6, 20, 8, 6, 3, 0, TRUE);
    work->descWindow.window = BmpWin_CreateDynamic(6, 2, 12, 29, 7, 0, TRUE);
    BmpWin_FlushMap(work->itemNameWindow.window);
    BmpWin_FlushMap(work->countWindow.window);
    BmpWin_FlushMap(work->descWindow.window);
    BmpWin_FlushMap(work->tmInfoWindow.window);

    arc = GFL_ArcSysCreateFileHandle(ARCID_ITEMGRA, work->heapId);
    work->selIconCel = func_0204af28(arc, 1, 0, work->heapId);
    GFL_ArcToolFree(arc);

    GFL_MsgDataLoadStrbuf(work->msgData, 102, work->strbuf);
    func_ov070_021bb6e0(work, &work->tmInfoWindow, work->strbuf, 8, 4, ITEMMENU_TEXT_COLOR);
    GFL_MsgDataLoadStrbuf(work->msgData, 93, work->strbuf);
    func_ov070_021bb6e0(work, &work->tmInfoWindow, work->strbuf, 8, 24, ITEMMENU_TEXT_COLOR);
    GFL_MsgDataLoadStrbuf(work->msgData, 91, work->strbuf);
    func_ov070_021bb6e0(work, &work->tmInfoWindow, work->strbuf, 112, 4, ITEMMENU_TEXT_COLOR);
    GFL_MsgDataLoadStrbuf(work->msgData, 92, work->strbuf);
    func_ov070_021bb6e0(work, &work->tmInfoWindow, work->strbuf, 112, 24, ITEMMENU_TEXT_COLOR);
    GFL_MsgDataLoadStrbuf(work->msgData, 90, work->strbuf);
    func_ov070_021bb6e0(work, &work->tmInfoWindow, work->strbuf, 208, 4, ITEMMENU_TEXT_COLOR);
    if (func_020355c4() == FALSE) {
        func_ov070_021b9850(work);
    }
}

// pokebw2's ItemMenuDisp_FreeSelIcon
void func_ov070_021b9de8(ItemMenuWork *work) {
    if (work->selIcon != NULL) {
        func_0204b3b4(work->selIcon);
        func_0204a8d4(work->selIconChr);
        func_0204ada4(work->selIconPlt);
        work->selIcon = NULL;
    }
}

void func_ov070_021b9e14(ItemMenuWork *work, u32 item) {
    ArcTool *arc;
    ClActorSetup setup;

    if (work->selIcon != NULL) {
        func_ov070_021b9de8(work);
    }
    arc = GFL_ArcSysCreateFileHandle(0x19, work->heapId);
    work->selIconPlt = func_0204ac18(arc, GetItemGraphicsDatID(item, 2), 1, 0, 0, 1, work->heapId);
    work->selIconChr = func_0204a6c8(arc, GetItemGraphicsDatID(item, 1), 0, 1, work->heapId);
    GFL_ArcToolFree(arc);
    setup.x = 0x84;
    setup.y = 0x50;
    setup.sequence = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    work->selIcon = func_0204b294(work->actorUnit, work->selIconChr, work->selIconPlt, work->selIconCel, &setup, 1, work->heapId);
    func_0204b92c(work->selIcon, 1);
    func_0204b3dc(work->selIcon, 1);
}


// pokebw2's ItemMenuDisp_Update
void func_ov070_021b9ed8(ItemMenuWork *work) {
    func_0204a600();
    if (work->taskMenu != NULL) {
        func_02027974(work->taskMenu);
    }
}

// Loads the list's OBJ
// pokebw2's ItemMenuDisp_LoadListRes
void func_ov070_021b9ef4(ItemMenuWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(88, work->heapId);
    s32 i;

    work->rowMarkPlt = func_0204ac18(arc, 32, 0, 0x1a0, 0, 1, work->heapId);
    work->rowMarkChr = func_0204a6c8(arc, 31, FALSE, 0, work->heapId);
    work->rowMarkCel = func_0204af28(arc, 30, 29, work->heapId);
    work->listCursorPlt = func_0204ac18(arc, 28, 0, 0x60, 0, 1, work->heapId);
    work->listCursorChr = func_0204a6c8(arc, 27, FALSE, 0, work->heapId);
    work->listCursorCel = func_0204af28(arc, 26, 25, work->heapId);
    for (i = 0; i < ITEMMENU_ROW_OBJS; i++) {
        work->rowNameBitmaps[i] = GFL_BitmapCreate(12, 2, 32, work->heapId);
        work->rowNameChr[i] = func_0204a6c8(arc, 20, FALSE, 0, work->heapId);
    }
    getTrainerGender(work->playerInfo);
    work->rowNamePlt = func_0204ac18(arc, 21, 0, 0, 0, 3, work->heapId);
    work->rowNameCel = func_0204af28(arc, 19, 18, work->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov070_021ba028(ItemMenuWork *work) {
    ClActorSetup scrollSetup;
    ClActorSetup cursorSetup;
    ClActorSetup rowSetup;
    s32 i;

    scrollSetup.x = 248;
    scrollSetup.y = 26;
    scrollSetup.priority = 1;
    scrollSetup.bgPriority = 2;
    scrollSetup.sequence = 0;
    work->scrollBar = func_0204b294(work->actorUnit, work->listCursorChr, work->listCursorPlt, work->listCursorCel, &scrollSetup, 0, work->heapId);
    func_0204b3dc(work->scrollBar, TRUE);
    func_0204b92c(work->scrollBar, TRUE);
    func_ov070_021ba6a4(work);

    cursorSetup.x = 140;
    cursorSetup.y = 48;
    cursorSetup.priority = 1;
    cursorSetup.bgPriority = 2;
    cursorSetup.sequence = 1;
    work->listCursor = func_0204b294(work->actorUnit, work->listCursorChr, work->listCursorPlt, work->listCursorCel, &cursorSetup, 0, work->heapId);
    func_0204b3dc(work->listCursor, TRUE);
    func_0204b92c(work->listCursor, TRUE);

    for (i = 0; i < ITEMMENU_ROW_OBJS; i++) {
        rowSetup.x = 140;
        rowSetup.y = i * 24;
        rowSetup.priority = 10;
        rowSetup.bgPriority = 3;
        rowSetup.sequence = 0;
        work->rowNames[i] = func_0204b294(work->actorUnit, work->rowNameChr[i], work->rowNamePlt, work->rowNameCel, &rowSetup, 0, work->heapId);
        rowSetup.x -= 24;
        rowSetup.sequence = 1;
        work->rowMarks[i] = func_0204b294(work->actorUnit, work->rowMarkChr, work->rowMarkPlt, work->rowMarkCel, &rowSetup, 0, work->heapId);
        func_0204b3dc(work->rowNames[i], FALSE);
        func_0204b3dc(work->rowMarks[i], FALSE);
    }
}


// Whether a TM or HM teaches a move that is used in the field
// pokebw2's ItemMenuDisp_IsFieldMoveTM
BOOL func_ov070_021ba1d0(u16 item) {
    if (PML_ItemIsTMHM(item) == TRUE) {
        switch (PML_ItemGetTMWazaID(item)) {
        case 0xf:
        case 0x13:
        case 0x39:
        case 0x46:
        case 0x7f:
        case 0x123:
            return TRUE;
        }
    }
    return FALSE;
}

static const u8 data_ov070_021bba79[] = { TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE };
void func_ov070_021ba218(ItemMenuWork *work) {
    s32 i;
    ArcTool *arc;
    BagItem *slot;
    void *data;
    s32 kind;
    u16 color;
    u16 move;

    if (func_ov070_021b61c8(work) == 0) {
        func_ov070_021bb724(&work->menuTitleWindow);
    } else {
        BmpWin *window = work->menuTitleWindow.window;

        func_020456f8(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    }
    if (work->drawnScroll == work->scroll) {
        return;
    }
    arc = PML_ItemArcHandleCreate(work->heapId);
    for (i = 0; i < ITEMMENU_ROW_OBJS; i++) {
        work->rowMarkTypes[i] = 0;
        if (work->scroll + i < 0) {
            continue;
        }
        slot = func_ov070_021b616c(work, work->scroll + i);
        if (slot == NULL || slot->item == ITEM_NONE) {
            continue;
        }
        data = PML_ItemArcHandleReadFile(arc, slot->item, work->heapId);
        kind = PML_ItemGetParam(data, ITEM_PARAM_KIND);
        if (data_ov070_021bba79[i] == FALSE) {
            if (func_ov070_021ba1d0(slot->item) == TRUE) {
                color = PRINT_COLOR(6, 5, 13);
            } else {
                color = PRINT_COLOR(2, 1, 13);
            }
        } else if (func_ov070_021ba1d0(slot->item) == TRUE) {
            color = PRINT_COLOR(8, 7, 13);
        } else {
            color = PRINT_COLOR(4, 3, 13);
        }
        GFL_BitmapFill(work->rowNameBitmaps[i], 13);
        GFL_MsgDataLoadStrbuf(work->msgData, 130, work->strbuf);
        move = PML_ItemGetTMWazaID(slot->item);
        if (move == 0) {
            func_ov070_021b61f4(work, 0, slot->item);
        } else {
            loadMoveNameToStrbuf(work->wordSet, 0, move);
        }
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        GFL_TextRendererDrawToBitmapEx(work->rowNameBitmaps[i], 0, 0, work->expandBuf, work->font, color);
        if (kind == 4) {
            work->rowMarkTypes[i] = 4;
        } else if (kind == 5) {
            work->rowMarkTypes[i] = 5;
        } else if (kind == 1) {
            work->rowMarkTypes[i] = 6;
        } else if (PML_ItemGetHMID(slot->item) != 0xff) {
            work->rowMarkTypes[i] = 8;
        } else if (PML_ItemGetTMBitMask(slot->item) != 0xff) {
            work->rowMarkTypes[i] = 7;
        } else if (PML_ItemGetParam(data, ITEM_PARAM_REGISTRABLE) == 0) {
            work->rowMarkTypes[i] = 3;
        } else if (func_ov070_021b83a0(work, slot->item) == FALSE) {
            work->rowMarkTypes[i] = 2;
        } else {
            work->rowMarkTypes[i] = 1;
        }
        GFL_HeapFree(data);
    }
    GFL_ArcToolFree(arc);
    work->iconsDirty = TRUE;
}


void func_ov070_021ba41c(ItemMenuWork *work) {
    ClActorPos pos;

    func_ov070_021bb398(work, 0);
    func_0204b454(work->listCursor, &pos, 0xffff);
    pos.y = (work->cursorRow + 1) * 24;
    func_0204b404(work->listCursor, &pos, 0xffff);
    if (func_020355c4() == FALSE) {
        func_ov070_021bb398(work, 1);
    }
    func_0202eb04(work->paletteAnim);
    func_0204b3dc(work->scrollBar, func_ov070_021b61c8(work) >= ITEMMENU_LIST_ROWS + 1);
    func_0204b908(work->listCursor, 0);
    func_0204b908(work->scrollBar, 0);
}

void func_ov070_021ba4a4(ItemMenuWork *work) {
    u8 *pixels;
    u32 chars;
    s32 i;

    for (i = 0; i < ITEMMENU_ROW_OBJS; i++) {
        chars = func_0204abbc(work->rowNameChr[i], FALSE);
        pixels = GFL_BitmapGetPixelData(work->rowNameBitmaps[i]);
        DC_FlushRange(pixels, GFL_BitmapCalcPixelDataSize(work->rowNameBitmaps[i]));
        GX_LoadOBJ(pixels + 0x100, chars + 0x80, 0x80);
        GX_LoadOBJ(pixels + 0x280, chars + 0x100, 0x80);
        GX_LoadOBJ(pixels, chars + 0x300, 0x100);
        GX_LoadOBJ(pixels + 0x180, chars + 0x400, 0x100);
        if (work->rowMarkTypes[i] != 0) {
            if (work->pocket == 4) {
                if (func_ov070_021b811c(work) == TRUE && work->rowMarkTypes[i] != 3) {
                    func_0204b878(work->rowMarks[i], (u16)(work->rowMarkTypes[i] - 1));
                    func_0204b3dc(work->rowMarks[i], TRUE);
                } else {
                    func_0204b3dc(work->rowMarks[i], FALSE);
                }
            } else if (work->pocket == 0 && work->rowMarkTypes[i] >= 4) {
                func_0204b878(work->rowMarks[i], (u16)(work->rowMarkTypes[i] - 1));
                func_0204b3dc(work->rowMarks[i], TRUE);
            } else if (work->pocket == 2 && work->rowMarkTypes[i] >= 7) {
                func_0204b878(work->rowMarks[i], (u16)(work->rowMarkTypes[i] - 1));
                func_0204b3dc(work->rowMarks[i], TRUE);
            } else {
                func_0204b3dc(work->rowMarks[i], FALSE);
            }
            func_0204b3dc(work->rowNames[i], TRUE);
        } else {
            func_0204b3dc(work->rowMarks[i], FALSE);
            func_0204b3dc(work->rowNames[i], FALSE);
        }
    }
}



void func_ov070_021ba654(ItemMenuWork *work) {
    u32 x;
    u32 y;
    ClActorPos pos;

    if (func_02035cb8(&x, &y) == TRUE) {
        if (y < 26) {
            y = 26;
        } else if (y > 142) {
            y = 142;
        }
        func_0204b454(work->scrollBar, &pos, 0xffff);
        pos.y = y;
        func_0204b404(work->scrollBar, &pos, 0xffff);
    }
}

void func_ov070_021ba6a4(ItemMenuWork *work) {
    s32 count = func_ov070_021b61c8(work);
    ClActorPos pos;

    if (count >= ITEMMENU_LIST_ROWS + 1) {
        func_0204b454(work->scrollBar, &pos, 0xffff);
        pos.y = func_0202ee0c(count - ITEMMENU_LIST_ROWS, work->scroll + 1, 26, 142, 0);
        func_0204b404(work->scrollBar, &pos, 0xffff);
    }
}

void func_ov070_021ba6f4(ItemMenuWork *work) {
    ClActorSetup setup;
    u8 i;

    setup.priority = 10;
    setup.bgPriority = 1;
    {
        u8 sequences[] = { 4, 5, 6, 0, 1 };
        u8 xs[] = { 0, 120, 173, 192, 224 };

        for (i = 0; i < 5; i++) {
            setup.x = xs[i];
            setup.y = 168;
            setup.sequence = sequences[i];
            if (i == 2) {
                setup.y += 4;
            }
            work->buttons[i] = func_0204b294(work->actorUnit, work->buttonChr, work->buttonPlt, work->buttonCel, &setup, 0, work->heapId);
            func_0204b3dc(work->buttons[i], TRUE);
        }
    }
    func_0204b92c(work->buttons[0], TRUE);
    func_0204b92c(work->buttons[1], TRUE);
    if (func_ov070_021b811c(work) == FALSE) {
        func_0204b3dc(work->buttons[2], FALSE);
    }
    if (work->mode == 2) {
        func_0204b3dc(work->buttons[3], FALSE);
    }
    {
        u8 sequences[] = { 3, 2 };
        u8 ys[] = { 97, 119 };
        u8 button;

        for (i = 0; i < 2; i++) {
            setup.x = 232;
            setup.y = ys[i];
            setup.sequence = sequences[i];
            setup.bgPriority = 0;
            button = i + 5;
            work->buttons[button] = func_0204b294(work->actorUnit, work->buttonChr, work->buttonPlt, work->buttonCel, &setup, 0, work->heapId);
            func_0204b92c(work->buttons[button], TRUE);
            func_0204b3dc(work->buttons[button], FALSE);
        }
    }
}


static const u8 data_ov070_021bba74[] = { 14, 12, 10, 12, 16 };
static const u8 data_ov070_021bba81[][2] = { { 9, 4 }, { 8, 3 }, { 6, 1 }, { 7, 2 }, { 5, 0 } };
static ClActorPos data_ov070_021bbbcc[] = { { 0, 80 }, { 0, 80 }, { 0, 80 }, { 0, 80 }, { 0, 80 } };

static const char data_ov070_021bba64[] = "aw";

static const u8 data_ov070_021bba68[4] = { 3, 2, 0, 0 };

static const u8 data_ov070_021bba6c[8] = { 0, 120, 173, 192, 224, 0, 0, 0 };

static const u8 data_ov070_021bbaa8[8] = { 4, 5, 6, 0, 1, 0, 0, 0 };

void func_ov070_021ba8c0(ItemMenuWork *work) {
    ClActorSetup setup;
    u32 i;

    for (i = 0; i < NELEMS(work->pocketTabs); i++) {
        setup.priority = data_ov070_021bba74[i];
        setup.bgPriority = 1;
        setup.x = data_ov070_021bbbcc[i].x - 16;
        setup.y = data_ov070_021bbbcc[i].y;
        setup.sequence = data_ov070_021bba81[i][1];
        work->pocketTabs[i] = func_0204b294(work->actorUnit, work->pocketTabChr, work->pocketTabPlt, work->pocketTabCel, &setup, 0, work->heapId);
        func_0204b92c(work->pocketTabs[i], TRUE);
    }
}




void func_ov070_021ba96c(ItemMenuWork *work, u32 pocket) {
    u32 i;

    for (i = 0; i < NELEMS(work->pocketTabs); i++) {
        if (i == pocket) {
            func_0204b878(work->pocketTabs[i], data_ov070_021bba81[i][0]);
        } else {
            func_0204b878(work->pocketTabs[i], data_ov070_021bba81[i][1]);
        }
    }
}


void func_ov070_021ba9ac(ItemMenuWork *work) {
    func_0201f7a8(work->msgWindow.window, TRUE);
}

void func_ov070_021ba9c0(ItemMenuWork *work, u32 *msgIds, s32 count) {
    AppTaskMenuInit init;
    s32 i;

    init.heapId = work->heapId;
    init.itemCount = count;
    init.items = work->menuItems;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 24;
    init.width = 13;
    init.height = 3;
    for (i = 0; i < count; i++) {
        work->menuItems[i].str = GFL_StrBufCreate(100, work->heapId);
        GFL_MsgDataLoadStrbuf(work->msgData, msgIds[i], work->menuItems[i].str);
        work->menuItems[i].color = ITEMMENU_MENU_COLOR;
        work->menuItems[i].type = 0;
    }
    work->menuItems[i - 1].type = APP_TASKMENU_ITEM_RETURN;
    work->taskMenu = func_02027778(&init, work->taskMenuRes);
    for (i = 0; i < count; i++) {
        GFL_StrBufFree(work->menuItems[i].str);
    }
    func_ov070_021bb3f4(work, FALSE);
}


void func_ov070_021baaa4(ItemMenuWork *work, u32 pocket) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(88, work->heapId);

    if (pocket != BAG_POCKET_TMS_HMS) {
        GFL_G2DIOLoadNSCRAsync(arc, 23, 5, 0, CHAR_POS(work->bg5Chars), 0, FALSE, work->heapId);
    } else {
        GFL_G2DIOLoadNSCRAsync(arc, 24, 5, 0, CHAR_POS(work->bg5Chars), 0, FALSE, work->heapId);
    }
    GFL_ArcToolFree(arc);
}

void func_ov070_021bab04(ItemMenuWork *work, u32 move) {
    PrintWindow *window = &work->tmInfoWindow;
    s32 pp;
    s32 power;
    s32 accuracy;

    func_0204b3dc(work->categoryIcons[PML_MoveGetCategory(move)], TRUE);
    func_0204b3dc(work->typeIcons[PML_MoveGetType(move)], TRUE);
    pp = PML_MoveGetMaxPP(move, 0);
    power = PML_MoveGetParam(move, MOVE_PARAM_POWER);
    accuracy = PML_MoveGetParam(move, MOVE_PARAM_ACCURACY);
    GFL_BitmapFillArea(BmpWin_GetBitmap(window->window), 176, 0, 32, 40, 0);
    GFL_BitmapFillArea(BmpWin_GetBitmap(window->window), 232, 0, 24, 24, 0);
    if (power <= 1) {
        GFL_MsgDataLoadStrbuf(work->msgData, 23, work->strbuf);
        func_ov070_021bb6e0(work, window, work->strbuf, 176, 4, ITEMMENU_TEXT_COLOR);
    } else {
        GFL_MsgDataLoadStrbuf(work->msgData, 95, work->strbuf);
        WordSetNumber(work->wordSet, 0, power, 3, 0, TRUE);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021bb6e0(work, window, work->expandBuf, 176, 4, ITEMMENU_TEXT_COLOR);
    }
    if (accuracy == 0 || PML_MoveIsAlwaysHit(move) == TRUE) {
        GFL_MsgDataLoadStrbuf(work->msgData, 23, work->strbuf);
        func_ov070_021bb6e0(work, window, work->strbuf, 176, 24, ITEMMENU_TEXT_COLOR);
    } else {
        GFL_MsgDataLoadStrbuf(work->msgData, 95, work->strbuf);
        WordSetNumber(work->wordSet, 0, accuracy, 3, 0, TRUE);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021bb6e0(work, window, work->expandBuf, 176, 24, ITEMMENU_TEXT_COLOR);
    }
    GFL_MsgDataLoadStrbuf(work->msgData, 94, work->strbuf);
    WordSetNumber(work->wordSet, 0, pp, 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021bb6e0(work, window, work->expandBuf, 232, 4, ITEMMENU_TEXT_COLOR);
    func_ov070_021bb724(window);
}




void func_ov070_021bacf0(ItemMenuWork *work, s32 mode) {
    PrintWindow *msgWindow;
    BmpWin **window;
    if (work->msgWindow.window == NULL) {
        work->msgWindow.window = BmpWin_CreateDynamic(3U, 1U, 1U, 0x1EU, 4U, 0xCU, 1U);
    }
    window = &work->msgWindow.window;
    GFL_BitmapFill(BmpWin_GetBitmap(*window), 0xFU);
    msgWindow = &work->msgWindow;
    BmpWin_FlushChar(*window);
    if (mode == 1) {
        work->printStream = func_0201cce4(*window, 0, 0, work->expandBuf, work->font, func_02012ffc(),
                work->tcbManager, 2U, (u16)((s32)work->heapId), 0xFU);
    } else {
        func_ov070_021bb6e0(work, &work->msgWindow, work->expandBuf, 0U, 0, 0x44FU);
    }
    BmpWin_DrawFrame(*window, 1U, (u16)work->cursorImageChars, 0xDU);
    func_ov070_021bb724(msgWindow);
}

void func_ov070_021badac(ItemMenuWork *work) {
    func_ov070_021bacf0(work, 0);
}

void func_ov070_021badb8(ItemMenuWork *work, u32 item) {
    GFL_MsgDataLoadStrbuf(work->msgData, 43, work->strbuf);
    func_ov070_021b61f4(work, 0, item);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
}

BOOL func_ov070_021badf0(ItemMenuWork *work) {
    if (work->printStream != NULL) {
        u32 state = func_0201ce30(work->printStream);

        func_020286c4(work->keyCursor, work->printStream, work->msgWindow.window);
        switch (state) {
        case PRINT_STREAM_DONE:
            func_0201ce48(work->printStream);
            work->printStream = NULL;
            work->streamResumed = FALSE;
            break;
        case PRINT_STREAM_PAUSED:
            if (work->streamResumed == FALSE &&
                ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c60())) {
                GFL_SndSEPlay(SEQ_SE_MESSAGE);
                func_0201ce38(work->printStream);
                work->streamResumed = TRUE;
            }
            break;
        case PRINT_STREAM_RUNNING:
            if ((GCTX_HIDGetHeldKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || func_02035c38()) {
                func_0201ce5c(work->printStream, 0);
            }
            work->streamResumed = FALSE;
            break;
        }
        return FALSE;
    }
    return TRUE;
}


void func_ov070_021bae98(ItemMenuWork *work) {
    work->pocketNameWindow.window = BmpWin_CreateDynamic(2, 3, 21, 12, 3, 12, TRUE);
    work->moneyWindow.window = BmpWin_CreateDynamic(2, 1, 21, 8, 3, 12, TRUE);
    work->moneyValueWindow.window = BmpWin_CreateDynamic(2, 9, 21, 9, 3, 12, TRUE);
    func_ov070_021baf38(work, work->pocket);
    func_ov070_021ba96c(work, work->pocket);
}



void func_ov070_021baf10(ItemMenuWork *work) {
    BmpWin_Free(work->pocketNameWindow.window);
    BmpWin_Free(work->moneyValueWindow.window);
    BmpWin_Free(work->moneyWindow.window);
}

void func_ov070_021baf38(ItemMenuWork *work, u32 pocket) {
    u32 x;

    GFL_BitmapFill(BmpWin_GetBitmap(work->pocketNameWindow.window), 0);
    GFL_MsgDataLoadStrbuf(work->msgData, 139, work->strbuf);
    loadBagPocketNameToStrbuf(work->wordSet, 0, pocket);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    x = (96 - GFL_FontGetBlockWidth(work->expandBuf, work->font, 0)) / 2;
    func_ov070_021bb6e0(work, &work->pocketNameWindow, work->expandBuf, x, 4, ITEMMENU_TITLE_COLOR);
    func_ov070_021bb724(&work->pocketNameWindow);
}

void func_ov070_021bafc4(ItemMenuWork *work) {
    GFL_BitmapFill(BmpWin_GetBitmap(work->moneyValueWindow.window), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(work->moneyWindow.window), 0);
    GFL_MsgDataLoadStrbuf(work->msgData, 81, work->strbuf);
    func_ov070_021bb6e0(work, &work->moneyWindow, work->strbuf, 0, 4, ITEMMENU_TITLE_COLOR);
    func_ov070_021bb724(&work->moneyWindow);
    GFL_MsgDataLoadStrbuf(work->msgData, 82, work->strbuf);
    WordSetNumber(work->wordSet, 0, getCash(getTrainerCardDataBlkAddress(work->gameData)), 7, 1, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021bb6e0(work, &work->moneyValueWindow, work->expandBuf, 0, 4, ITEMMENU_TITLE_COLOR);
    func_ov070_021bb724(&work->moneyValueWindow);
}




void func_ov070_021bb08c(ItemMenuWork *work) {
    func_0204b3dc(work->buttons[0], FALSE);
    func_0204b3dc(work->buttons[1], FALSE);
    func_020456f8(work->pocketNameWindow.window);
    func_ov070_021bafc4(work);
}

void func_ov070_021bb0bc(ItemMenuWork *work) {
    func_0204b3dc(work->buttons[0], TRUE);
    func_0204b3dc(work->buttons[1], TRUE);
    func_020456f8(work->moneyValueWindow.window);
    func_020456f8(work->moneyWindow.window);
    func_ov070_021baf38(work, work->pocket);
}

void func_ov070_021bb0f8(ItemMenuWork *work) {
    AppTaskMenuInit init;

    init.heapId = work->heapId;
    init.itemCount = 2;
    init.items = work->menuItems;
    init.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    init.x = 32;
    init.y = 12;
    init.width = 8;
    init.height = 3;
    work->menuItems[0].str = GFL_StrBufCreate(100, work->heapId);
    GFL_MsgDataLoadStrbuf(work->msgData, 132, work->menuItems[0].str);
    work->menuItems[0].color = ITEMMENU_MENU_COLOR;
    work->menuItems[0].type = 0;
    work->menuItems[1].str = GFL_StrBufCreate(100, work->heapId);
    GFL_MsgDataLoadStrbuf(work->msgData, 133, work->menuItems[1].str);
    work->menuItems[1].color = ITEMMENU_MENU_COLOR;
    work->menuItems[1].type = 0;
    work->taskMenu = func_02027778(&init, work->taskMenuRes);
    GFL_StrBufFree(work->menuItems[0].str);
    GFL_StrBufFree(work->menuItems[1].str);
    func_ov070_021bb580(work, FALSE);
    func_ov070_021bb3f4(work, FALSE);
}




void func_ov070_021bb1ac(ItemMenuWork *work) {
    func_02027858(work->taskMenu);
    work->taskMenu = NULL;
}

void func_ov070_021bb1c4(ItemMenuWork *work) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(88, work->heapId);
    void *file = GFL_ArcToolReadHeapNewLZ(arc, 35, FALSE, work->heapId);
    NNSG2dScreenData *screen;
    s32 i = 0;

    if (NNS_G2dGetUnpackedScreenData(file, &screen)) {
        u16 *tiles = (u16 *)screen->rawData;
        s32 count = (screen->screenWidth / 8) * (screen->screenHeight / 8);

        for (; i < count; i++) {
            tiles[i] += (u16)work->bg3Chars | (3 << 12);
        }
        GFL_BGSysLoadScrAreaAll(3, screen->rawData, 16, 12, screen->screenWidth / 8, screen->screenHeight / 8);
        GFL_BGSysQueueScrLoad(3);
    }
    GFL_HeapFree(file);
    GFL_ArcToolFree(arc);
}

void func_ov070_021bb260(ItemMenuWork *work, s32 quantity) {
    GFL_BitmapFill(BmpWin_GetBitmap(work->quantityWindow.window), 5);
    GFL_MsgDataLoadStrbuf(work->msgData, 131, work->strbuf);
    WordSetNumber(work->wordSet, 0, quantity, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
    func_ov070_021bb6e0(work, &work->quantityWindow, work->expandBuf, 0, 0, PRINT_COLOR(15, 14, 5));
    func_ov070_021bb724(&work->quantityWindow);
    if (work->quantityMode == 2) {
        PrintWindow *window = &work->priceWindow;
        s32 price = func_ov070_021b619c(work->item, work->quantity, work->heapId);
        GFL_BitmapFill(BmpWin_GetBitmap(window->window), 5);
        GFL_MsgDataLoadStrbuf(work->msgData, 84, work->strbuf);
        WordSetNumber(work->wordSet, 0, price, 7, 0, TRUE);
        GFL_WordSetFormatStrbuf(work->wordSet, work->expandBuf, work->strbuf);
        func_ov070_021bb6e0(work, window, work->expandBuf, 0, 0, PRINT_COLOR(15, 14, 5));
        func_ov070_021bb724(window);
    }
}




u32 func_ov070_021bb368(u32 index) {
    switch (index) {
    case 0:
        return 10;
    case 3:
        return 13;
    case 1:
        return 11;
    case 2:
        return 12;
    case 4:
        return 14;
    }
    return index;
}

void func_ov070_021bb398(ItemMenuWork *work, u32 palette) {
    ClActorPos cursorPos;
    ClActorPos rowPos;
    u32 i;

    func_0204b454(work->listCursor, &cursorPos, 0);
    for (i = 0; i < ITEMMENU_ROW_OBJS; i++) {
        func_0204b454(work->rowNames[i], &rowPos, 0);
        if (rowPos.y == cursorPos.y) {
            break;
        }
    }
    func_0204b6f4(work->rowNames[i], (u8)palette, 0);
}

void func_ov070_021bb3f4(ItemMenuWork *work, BOOL active) {
    BOOL registered;

    if (work->buttonsActive == active) {
        return;
    }
    work->buttonsActive = active;
    registered = GameData_IsShortcutRegistered(work->gameData, func_ov070_021bb368(work->pocket));
    if (active == TRUE) {
        func_0202173c(work->paletteFade, 4, 0x23cf, 0, 0, 0, 0, GFL_VBlankGetTCBMgr());
        func_0202173c(work->paletteFade, 1, 1, 0, 0, 0, 0, GFL_VBlankGetTCBMgr());
        G2_SetBlendBrightness(6, 0);
        func_ov070_021bb5a4(work);
        func_0204b878(work->buttons[0], 4);
        func_0204b878(work->buttons[1], 5);
        func_0204b878(work->buttons[3], 0);
        if (registered == TRUE) {
            func_0204b878(work->buttons[2], 7);
        } else {
            func_0204b878(work->buttons[2], 6);
        }
        func_ov070_021bb580(work, TRUE);
    } else {
        func_020216d4(work->paletteFade, 2, 0, 0x200);
        func_0202173c(work->paletteFade, 4, 0x23cf, 0, 8, 8, 0, GFL_VBlankGetTCBMgr());
        func_0202173c(work->paletteFade, 1, 1, 0, 8, 8, 0, GFL_VBlankGetTCBMgr());
        if (work->mode == 4 && func_0204b3f4(work->buttons[0]) == FALSE) {
            G2_SetBlendBrightness(2, -8);
        } else {
            G2_SetBlendBrightness(6, -8);
        }
        func_0204b878(work->sortButton, 4);
        func_0204b878(work->buttons[0], 18);
        func_0204b878(work->buttons[1], 19);
        func_0204b878(work->buttons[3], 14);
        if (registered == TRUE) {
            func_0204b878(work->buttons[2], 22);
        } else {
            func_0204b878(work->buttons[2], 21);
        }
    }
}






void func_ov070_021bb580(ItemMenuWork *work, BOOL keys) {
    if (keys == TRUE) {
        func_0204b878(work->buttons[4], 1);
    } else {
        func_0204b878(work->buttons[4], 0xf);
    }
}

void func_ov070_021bb5a4(ItemMenuWork *work) {
    if (work->pocket == 2 || work->pocket == 3 || func_ov070_021b61c8(work) <= 1) {
        func_0204b878(work->sortButton, 4);
    } else {
        func_0204b878(work->sortButton, 0);
    }
}

void func_ov070_021bb5dc(ItemMenuWork *work, BOOL active) {
    BOOL registered = GameData_IsShortcutRegistered(work->gameData, func_ov070_021bb368(work->pocket));

    if (active == TRUE) {
        func_ov070_021bb5a4(work);
        func_0204b878(work->buttons[0], 4);
        func_0204b878(work->buttons[1], 5);
        func_0204b878(work->buttons[3], 0);
        if (registered == TRUE) {
            func_0204b878(work->buttons[2], 7);
        } else {
            func_0204b878(work->buttons[2], 6);
        }
    } else {
        func_0204b878(work->sortButton, 4);
        func_0204b878(work->buttons[0], 18);
        func_0204b878(work->buttons[1], 19);
        func_0204b878(work->buttons[3], 14);
        if (registered == TRUE) {
            func_0204b878(work->buttons[2], 22);
        } else {
            func_0204b878(work->buttons[2], 21);
        }
    }
}



BOOL func_ov070_021bb690(ItemMenuWork *work) {
    ClActorPos pos;
    u32 i;

    for (i = 0; i < NELEMS(work->pocketTabs); i++) {
        func_0204b454(work->pocketTabs[i], &pos, 0xffff);
        if (pos.x == data_ov070_021bbbcc[i].x) {
            return FALSE;
        }
        pos.x += 4;
        func_0204b404(work->pocketTabs[i], &pos, 0xffff);
    }
    return TRUE;
}


void func_ov070_021bb6e0(ItemMenuWork *work, PrintWindow *window, StrBuf *strbuf, u16 x, s16 y, u16 color) {
    PrintQueue *queue = work->printQueue;
    GFLBitmap *bitmap = BmpWin_GetBitmap(window->window);
    func_0201c724(queue, bitmap, x, y, strbuf, work->font, color);
    window->flushPending = 1;
}





// pokebw2's ItemMenuDisp_TransferScreen
void func_ov070_021bb724(PrintWindow *window) {
    BmpWin_FlushMap(window->window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window->window));
}

void func_ov070_021bb73c(ItemMenuWork *work) {
    func_0201c4e4(work->printQueue);
    PrintWindow_Flush(&work->tmInfoWindow, work->printQueue);
    PrintWindow_Flush(&work->itemNameWindow, work->printQueue);
    PrintWindow_Flush(&work->countWindow, work->printQueue);
    PrintWindow_Flush(&work->descWindow, work->printQueue);
    PrintWindow_Flush(&work->quantityWindow, work->printQueue);
    PrintWindow_Flush(&work->moneyWindow, work->printQueue);
    PrintWindow_Flush(&work->moneyValueWindow, work->printQueue);
    PrintWindow_Flush(&work->priceWindow, work->printQueue);
    PrintWindow_Flush(&work->menuTitleWindow, work->printQueue);
    PrintWindow_Flush(&work->msgWindow, work->printQueue);
    PrintWindow_Flush(&work->pocketNameWindow, work->printQueue);
}


