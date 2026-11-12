// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov256/dpc_bmp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/dpc_bmp.h"
#include "app/dpc_main.h"
#include "app/ui/print_msg.h"
#include "constants/species.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "system/bmp_oam.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/btl_pokeparam_3.h"
#include "app/dendou_pc.h"

// The Hall of Fame PC's text: the page's title, the page number and the chosen Pokémon's data. The ROM doesn't name
// this file; the name is a guess, after box2_bmp.c

#define DPCBMP_COLOR_BLACK PRINT_COLOR(1, 2, 0)
#define DPCBMP_COLOR_BLUE PRINT_COLOR(3, 4, 0)
#define DPCBMP_COLOR_RED PRINT_COLOR(5, 6, 0)
#define DPCBMP_COLOR_PAGE PRINT_COLOR(6, 8, 0)

static void func_ov171_021dd94c(PrintWindow *win);

// pokebw2's DpcBmp_Init
void func_ov171_021dd874(DendouPCWork *wk) {
    BmpWin_InitAllocator(HEAPID_DENDOU_PC);
    wk->win[DPCBMP_WIN_TITLE].window = BmpWin_CreateDynamic(3, 1, 0, 30, 2, 3, TRUE);
    wk->win[DPCBMP_WIN_INFO].window = BmpWin_CreateDynamic(5, 4, 7, 24, 15, 1, TRUE);
}

// pokebw2's DpcBmp_Exit
void func_ov171_021dd8bc(DendouPCWork *wk) {
    BmpWin_Free(wk->win[DPCBMP_WIN_TITLE].window);
    BmpWin_Free(wk->win[DPCBMP_WIN_INFO].window);
    BmpWin_FreeAllocator();
}

// pokebw2's DpcBmp_PrintUpdate
void func_ov171_021dd8dc(DendouPCWork *wk) {
    u32 i;

    func_0201c4e4(wk->printQueue);
    for (i = 0; i < DPCBMP_WIN_MAX; i++) {
        PrintWindow_Flush(&wk->win[i], wk->printQueue);
    }
}

// pokebw2's DpcBmp_PutWindow
static void func_ov171_021dd94c(PrintWindow *win) {
    BmpWin_FlushMap(win->window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win->window));
}

// pokebw2's DpcBmp_PutTitle
void func_ov171_021dd964(DendouPCWork *wk) {
    DendouPCTeam *team = &wk->teams[wk->page];

    GFL_BitmapFill(BmpWin_GetBitmap(wk->win[DPCBMP_WIN_TITLE].window), 0);

    if (wk->param->mode == DENDOU_PC_MODE_FIRST_CLEAR) {
        StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_000);

        func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_TITLE], wk->printQueue, 0, 0, str, wk->font, DPCBMP_COLOR_BLACK,
                        PRINT_ALIGN_LEFT);
        GFL_StrBufFree(str);
    } else {
        StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_001);

        WordSetNumber(wk->wordSet, 0, team->number, 4, 0, TRUE);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
        func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_TITLE], wk->printQueue, 0, 0, wk->expandBuf, wk->font,
                        DPCBMP_COLOR_BLACK, PRINT_ALIGN_LEFT);
        GFL_StrBufFree(str);
    }

    {
        StrBuf *str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_20);

        WordSetNumber(wk->wordSet, 0, team->date.year, 2, 2, TRUE);
        WordSetNumber(wk->wordSet, 1, team->date.month, 2, 2, TRUE);
        WordSetNumber(wk->wordSet, 2, team->date.day, 2, 2, TRUE);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
        func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_TITLE], wk->printQueue,
                        BmpWin_GetWidth1(wk->win[DPCBMP_WIN_TITLE].window) * 8, 0, wk->expandBuf, wk->font,
                        DPCBMP_COLOR_RED, PRINT_ALIGN_RIGHT);
        GFL_StrBufFree(str);
    }

    func_ov171_021dd94c(&wk->win[DPCBMP_WIN_TITLE]);
}

// pokebw2's DpcBmp_PutPage
void func_ov171_021ddaf0(DendouPCWork *wk) {
    StrBuf *str;
    u8 slashWidth;
    u8 x;

    if (wk->pageMax == 1) {
        return;
    }

    GFL_BitmapFill(wk->pageBitmap, 0);

    // The slash in the middle, the page before it and the number of pages after it
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_003);
    slashWidth = GFL_FontGetBlockWidth(str, wk->font, 0);
    x = (40 - slashWidth) / 2;
    GFL_TextRendererDrawToBitmapEx(wk->pageBitmap, x, 4, str, wk->font, DPCBMP_COLOR_PAGE);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_004);
    WordSetNumber(wk->wordSet, 0, wk->page + 1, 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
    GFL_TextRendererDrawToBitmapEx(wk->pageBitmap, x - GFL_FontGetBlockWidth(wk->expandBuf, wk->font, 0), 4,
                                   wk->expandBuf, wk->font, DPCBMP_COLOR_PAGE);
    WordSetNumber(wk->wordSet, 0, wk->pageMax, 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
    GFL_TextRendererDrawToBitmapEx(wk->pageBitmap, x + slashWidth, 4, wk->expandBuf, wk->font, DPCBMP_COLOR_PAGE);
    GFL_StrBufFree(str);

    func_020257d0(wk->pageActor);
}

// pokebw2's DpcBmp_PutPokeInfo
void func_ov171_021ddc28(DendouPCWork *wk) {
    HallOfFamePokemon *poke = &wk->teams[wk->page].pokemon[wk->pokePos];
    StrBuf *str;
    u32 i;
    u32 width;

    GFL_BitmapFill(BmpWin_GetBitmap(wk->win[DPCBMP_WIN_INFO].window), 0);

    // The species and its sex, which the names of both Nidoran have
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_005);
    WordSet_LoadSpeciesName(wk->wordSet, 0, poke->species);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
    func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_INFO], wk->printQueue, 0, 0, wk->expandBuf, wk->font, DPCBMP_COLOR_BLACK,
                    PRINT_ALIGN_LEFT);
    GFL_StrBufFree(str);

    if (poke->sex == 0) {
        if (poke->species != SPECIES_NIDORAN_M) {
            str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_006);
            func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_INFO], wk->printQueue, 64, 0, str, wk->font, DPCBMP_COLOR_BLUE,
                            PRINT_ALIGN_LEFT);
            GFL_StrBufFree(str);
        }
    } else if (poke->sex == 1) {
        if (poke->species != SPECIES_NIDORAN_F) {
            str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_007);
            func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_INFO], wk->printQueue, 64, 0, str, wk->font, DPCBMP_COLOR_RED,
                            PRINT_ALIGN_LEFT);
            GFL_StrBufFree(str);
        }
    }

    // The level
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_011);
    width = GFL_FontGetBlockWidth(str, wk->smallFont, 0);
    func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_INFO], wk->printQueue, 80, 4, str, wk->smallFont, DPCBMP_COLOR_BLACK,
                    PRINT_ALIGN_LEFT);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_008);
    WordSetNumber(wk->wordSet, 0, poke->level, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
    func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_INFO], wk->printQueue, 80 + width, 0, wk->expandBuf, wk->font,
                    DPCBMP_COLOR_BLACK, PRINT_ALIGN_LEFT);
    GFL_StrBufFree(str);

    // The nickname and the trainer's name
    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_009);
    func_0201ee0c(wk->wordSet, 0, poke->nickname, 0, 1, 2);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
    func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_INFO], wk->printQueue, 0, 24, wk->expandBuf, wk->font, DPCBMP_COLOR_BLACK,
                    PRINT_ALIGN_LEFT);
    GFL_StrBufFree(str);

    str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_010);
    func_0201ee0c(wk->wordSet, 0, poke->trainerName, 0, 1, 2);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
    func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_INFO], wk->printQueue, 0, 48, wk->expandBuf, wk->font, DPCBMP_COLOR_BLACK,
                    PRINT_ALIGN_LEFT);
    GFL_StrBufFree(str);

    // The moves, in two columns
    for (i = 0; i < 4; i++) {
        str = GFL_MsgDataLoadStrbufNew(wk->msgData, BtlPokeparam3_Text_012 + i);
        loadMoveNameToStrbuf(wk->wordSet, 0, poke->moves[i]);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->expandBuf, str);
        func_ov067_021b6ae4(&wk->win[DPCBMP_WIN_INFO], wk->printQueue, (i & 1) * 96, 88 + (i >> 1) * 16, wk->expandBuf,
                        wk->font, DPCBMP_COLOR_BLACK, PRINT_ALIGN_LEFT);
        GFL_StrBufFree(str);
    }

    func_ov171_021dd94c(&wk->win[DPCBMP_WIN_INFO]);
}

// pokebw2's DpcBmp_ClearPokeInfo
void func_ov171_021ddfbc(DendouPCWork *wk) {
    func_020456f8(wk->win[DPCBMP_WIN_INFO].window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(wk->win[DPCBMP_WIN_INFO].window));
    GFL_BitmapFill(BmpWin_GetBitmap(wk->win[DPCBMP_WIN_INFO].window), 0);
    BmpWin_FlushChar(wk->win[DPCBMP_WIN_INFO].window);
}
