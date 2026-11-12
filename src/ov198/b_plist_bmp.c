// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov287/b_plist_bmp.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/b_app_tool.h"
#include "battle/b_plist_bmp.h"
#include "battle/b_plist_main.h"
#include "constants/arc.h"
#include "constants/moves.h"
#include "constants/text_banks.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "pml/item.h"
#include "pml/waza.h"
#include "system/app_menu_common.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/hp_gauge.h"
#include "system/printsys.h"
#include "system/str_tool.h"
#include "system/text_speed.h"
#include "system/wordset.h"
#include "field/beacon_view.h"

// The battle party list's windows and text (overlay 287). The ROM doesn't name this file: b_plist_bmp.c is a guess
// after the ROM's b_plist_main.c and b_plist_anm.c and the b_plist_bmp.c of Diamond and Pearl. None of these functions
// has a name yet.

#define BPLIST_COLOR_WHITE PRINT_COLOR(15, 14, 0)
#define BPLIST_COLOR_BLACK PRINT_COLOR(1, 2, 0)
#define BPLIST_COLOR_STAT PRINT_COLOR(8, 9, 0)
#define BPLIST_COLOR_MALE PRINT_COLOR(10, 11, 0)
#define BPLIST_COLOR_FEMALE PRINT_COLOR(12, 13, 0)

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
} BPlistWinData;


void func_ov198_0220bd34(BPlistWork *work);
static void func_ov198_0220bd84(BPlistWork *work);
void func_ov198_0220bda0(BPlistWork *work);
void func_ov198_0220beb8(BPlistWork *work, u32 winIdx, u16 pos, u8 x, u8 y);
static void func_ov198_0220c0d0(BPlistWork *work, u32 winIdx, u8 pos, u8 x, u8 y);
void func_ov198_0220c16c(BPlistWork *work, u32 winIdx, u32 pos, u8 x, u8 y);
void func_ov198_0220c2f8(BPlistWork *work, u32 winIdx, u32 pos, u8 x, u8 y);
void func_ov198_0220c398(BPlistWork *work, u32 winIdx, u8 pos);
void func_ov198_0220c42c(BPlistWork *work, u32 winIdx, u8 pos);
void func_ov198_0220c4dc(BPlistWork *work, u32 move, u32 winIdx, u32 msgId, u16 x, s16 y, u32 color);
void func_ov198_0220c5ac(BPlistWork *work, u32 winIdx, u8 x, u8 y);
static void func_ov198_0220c60c(BPlistWork *work, u32 msgId);
void func_ov198_0220c694(BPlistWork *work, u32 winIdx, u32 msgId);
void func_ov198_0220c714(BPlistWork *work, u8 pos);
void func_ov198_0220c8f0(BPlistWork *work, u8 pos);
void func_ov198_0220c9f0(BPlistWork *work, u8 pos);
void func_ov198_0220caf0(BPlistWork *work, u8 pos);
void func_ov198_0220cbf0(BPlistWork *work, u8 pos);
void func_ov198_0220ccf0(BPlistWork *work, u8 pos);
void func_ov198_0220cdf0(BPlistWork *work, u8 pos);
void func_ov198_0220cfc4(BPlistWork *work, u8 pos);
void func_ov198_0220d034(BPlistWork *work, u32 winIdx);
void func_ov198_0220d090(BPlistWork *work, u32 winIdx, u32 value);
void func_ov198_0220d1bc(BPlistWork *work, u32 winIdx);
void func_ov198_0220d218(BPlistWork *work, u32 winIdx, u32 value);
void func_ov198_0220d348(BPlistWork *work, u32 winIdx, u32 msgId);
void func_ov198_0220d3b4(BPlistWork *work, u32 winIdx);
void func_ov198_0220d418(BPlistWork *work, u32 winIdx, u32 category);
void func_ov198_0220d494(BPlistWork *work, u32 winIdx, u32 value1, u32 value2);
void func_ov198_0220d628(BPlistWork *work, u32 winIdx);
void func_ov198_0220d6dc(BPlistWork *work, BPlistMove *move, u32 winIdx);
void func_ov198_0220d920(BPlistWork *work, u32 pos);
void func_ov198_0220d988(BPlistWork *work, u8 pos);
void func_ov198_0220da6c(BPlistWork *work, u8 pos);
static void func_ov198_0220e168(BPlistWork *work);
void func_ov198_0220d884(BPlistWork *work);
void func_ov198_0220d9b4(BPlistWork *work);
void func_ov198_0220dc04(BPlistWork *work);
void func_ov198_0220dca8(BPlistWork *work);
void func_ov198_0220dd78(BPlistWork *work);
void func_ov198_0220de4c(BPlistWork *work);
void func_ov198_0220df20(BPlistWork *work);
void func_ov198_0220e088(BPlistWork *work);

static const u8 data_ov198_02211af4[] = { 0, 1, 2, 3, 0xff };
static const u8 data_ov198_02211af9[] = { 0, 1, 2, 3, 4, 0xff };
// Declared in this order for MWCC's sort to lay the two out in the ROM's order
static const u8 data_ov198_02211b06[] = { 0, 1, 2, 3, 4, 5, 0xff };
static const u8 data_ov198_02211aff[] = { 0, 1, 2, 3, 4, 5, 0xff };
static const u8 data_ov198_02211b0d[] = { 0, 1, 2, 3, 4, 5, 0xff };

static const BPlistWinData data_ov198_02211b14[2] = {
#include "asset/b_plist_bmp_data_ov198_02211b14.inc"
};

static const u8 data_ov198_02211b20[] = {
#include "asset/b_plist_bmp_data_ov198_02211b20.inc"
};
static const u8 data_ov198_02211b2c[] = {
#include "asset/b_plist_bmp_data_ov198_02211b2c.inc"
};
static const u32 data_ov198_02211b3c[] = { 0x47, 0x4a, 0x4d, 0x50, 0x53 };
static const u8 data_ov198_02211b50[] = {
#include "asset/b_plist_bmp_data_ov198_02211b50.inc"
};
static const u32 data_ov198_02211b68[] = { 0, 1, 2, 3, 4, 5 };

static const BPlistWinData data_ov198_02211b80[4] = {
#include "asset/b_plist_bmp_data_ov198_02211b80.inc"
};

static const BPlistWinData data_ov198_02211b98[5] = {
#include "asset/b_plist_bmp_data_ov198_02211b98.inc"
};

static const BPlistWinData data_ov198_02211bb6[6] = {
#include "asset/b_plist_bmp_data_ov198_02211bb6.inc"
};

static const BPlistWinData data_ov198_02211bda[6] = {
#include "asset/b_plist_bmp_data_ov198_02211bda.inc"
};

static const BPlistWinData data_ov198_02211bfe[6] = {
#include "asset/b_plist_bmp_data_ov198_02211bfe.inc"
};

static const BPlistWinData data_ov198_02211c24[11] = {
#include "asset/b_plist_bmp_data_ov198_02211c24.inc"
};

static const BPlistWinData data_ov198_02211c68[12] = {
#include "asset/b_plist_bmp_data_ov198_02211c68.inc"
};

static const BPlistWinData data_ov198_02211cb0[22] = {
#include "asset/b_plist_bmp_data_ov198_02211cb0.inc"
};

// pokebw2's BPlistBmp_DrawPage



void func_ov198_0220bc24(BPlistWork *work) {
    const BPlistWinData *data = data_ov198_02211b14;
    u32 i;

    for (i = 0; i < 2; i++) {
        work->msgWins[i].window = BmpWin_CreateDynamic(data->bg, data->x, data->y, data->width, data->height,
                                                       data->palette, TRUE);
        data++;
    }
    func_ov198_0220bc70(work, work->unk_2180);
}

void func_ov198_0220bc70(BPlistWork *work, u8 page) {
    const BPlistWinData *data;
    u32 i;

    func_ov198_0220bd84(work);
    func_ov198_0220bd34(work);
    switch (page) {
    case 0:
    case 8:
        data = data_ov198_02211bda;
        work->windowCount = 6;
        break;
    case 1:
        data = data_ov198_02211b80;
        work->windowCount = 4;
        break;
    case 2:
        data = data_ov198_02211cb0;
        work->windowCount = 22;
        break;
    case 3:
        data = data_ov198_02211bfe;
        work->windowCount = 6;
        break;
    case 4:
        data = data_ov198_02211c24;
        work->windowCount = 11;
        break;
    case 5:
        data = data_ov198_02211b98;
        work->windowCount = 5;
        break;
    case 6:
        data = data_ov198_02211bb6;
        work->windowCount = 6;
        break;
    case 7:
        data = data_ov198_02211c68;
        work->windowCount = 12;
        break;
    }

    for (i = 0; i < work->windowCount; i++) {
        work->windows[i].window = BmpWin_CreateDynamic(data->bg, data->x, data->y, data->width, data->height,
                                                       data->palette, TRUE);
        data++;
    }
}


void func_ov198_0220bd34(BPlistWork *work) {
    if (work->dummyWinState == 1) {
        work->dummyWin = BmpWin_CreateDynamic(5, 0, 0, 32, 16, 0, TRUE);
    }
    work->dummyWinState ^= 1;
}

static void func_ov198_0220bd84(BPlistWork *work) {
    if (work->dummyWin != NULL) {
        BmpWin_Free(work->dummyWin);
        work->dummyWin = NULL;
    }
}

void func_ov198_0220bda0(BPlistWork *work) {
    u32 i;

    for (i = 0; i < work->windowCount; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[i].window), 0);
    }
}


void func_ov198_0220bdd4(BPlistWork *work) {
    u32 i;

    for (i = 0; i < work->windowCount; i++) {
        BmpWin_Free(work->windows[i].window);
    }
}


void func_ov198_0220be04(BPlistWork *work) {
    u32 i;

    func_ov198_0220bdd4(work);
    for (i = 0; i < 2; i++) {
        BmpWin_Free(work->msgWins[i].window);
    }
    func_ov198_0220bd84(work);
}


void func_ov198_0220be2c(BPlistWork *work, u8 page) {
    switch (page) {
    case 0:
        func_ov198_0220d884(work);
        if (work->param->unk1F == 3) {
            func_ov198_0220c60c(work, 7);
        } else if (work->param->unk1F == 2) {
            func_ov198_0220c60c(work, 9);
        } else {
            func_ov198_0220c60c(work, 6);
        }
        break;
    case 8:
        func_ov198_0220d884(work);
        func_ov198_0220c60c(work, 10);
        break;
    case 1:
        func_ov198_0220d9b4(work);
        break;
    case 2:
        func_ov198_0220dca8(work);
        break;
    case 3:
        func_ov198_0220dc04(work);
        break;
    case 4:
        func_ov198_0220dd78(work);
        break;
    case 5:
        func_ov198_0220e088(work);
        break;
    case 6:
        func_ov198_0220de4c(work);
        break;
    case 7:
        func_ov198_0220df20(work);
        break;
    }
}

void func_ov198_0220beb8(BPlistWork *work, u32 winIdx, u16 pos, u8 x, u8 y) {
    BmpWin *window = work->windows[winIdx].window;
    BPlistPokemon *pokemon;
    StrBuf *str;
    StrBuf *tmpl;
    u16 sexX;
    u8 palette;

    palette = func_02045778(window);
    pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    str = GFL_StrBufCreate(12, work->param->heapId);
    tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, data_ov198_02211b68[pos]);
    loadPokemonNicknameToStrbuf(work->wordSet, 0, pokemon->pkm);
    GFL_WordSetFormatStrbuf(work->wordSet, str, tmpl);
    if (palette == 9) {
        PrintWindow_PrintColor(&work->windows[winIdx], work->printQueue, x, y, str, work->param->font,
                               BPLIST_COLOR_WHITE);
    } else {
        PrintWindow_PrintColor(&work->windows[winIdx], work->printQueue, x, y, str, work->param->font,
                               BPLIST_COLOR_WHITE);
    }
    GFL_StrBufFree(tmpl);
    GFL_StrBufFree(str);

    if (pokemon->hideSex || pokemon->isEgg) {
        return;
    }
    if (pokemon->sex == 0) {
        tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, 22);
        sexX = func_02045738(window) * 8 - GFL_FontGetBlockWidth(tmpl, work->param->font, 0);
        if (palette == 9) {
            PrintWindow_PrintColor(&work->windows[winIdx], work->printQueue, sexX, y, tmpl, work->param->font,
                                   BPLIST_COLOR_MALE);
        } else {
            PrintWindow_PrintColor(&work->windows[winIdx], work->printQueue, sexX, y, tmpl, work->param->font,
                                   BPLIST_COLOR_MALE);
        }
        GFL_StrBufFree(tmpl);
    } else if (pokemon->sex == 1) {
        tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, 23);
        sexX = func_02045738(window) * 8 - GFL_FontGetBlockWidth(tmpl, work->param->font, 0);
        if (palette == 9) {
            PrintWindow_PrintColor(&work->windows[winIdx], work->printQueue, sexX, y, tmpl, work->param->font,
                                   BPLIST_COLOR_FEMALE);
        } else {
            PrintWindow_PrintColor(&work->windows[winIdx], work->printQueue, sexX, y, tmpl, work->param->font,
                                   BPLIST_COLOR_FEMALE);
        }
        GFL_StrBufFree(tmpl);
    }
}


static void func_ov198_0220c0d0(BPlistWork *work, u32 winIdx, u8 pos, u8 x, u8 y) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, 11);
    PrintQueue *queue;

    WordSetNumber(work->wordSet, 0, pokemon->level, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, tmpl);
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), x, y, work->strBuf, work->smallFont,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(tmpl);
}

void func_ov198_0220c16c(BPlistWork *work, u32 winIdx, u32 pos, u8 x, u8 y) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, 12);
    u8 width = GFL_FontGetBlockWidth(str, work->smallFont, 0);

    x = x - width / 2;
    PrintWindow_Print(&work->windows[winIdx], work->printQueue, x, y, str, work->smallFont, BPLIST_COLOR_WHITE);
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(work->msgData, 13);
    WordSetNumber(work->wordSet, 0, pokemon->hp, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
    PrintWindow_PrintColor(&work->windows[winIdx], work->printQueue, x - GFL_FontGetBlockWidth(work->strBuf, work->smallFont, 0), y, work->strBuf, work->smallFont, BPLIST_COLOR_WHITE);
    GFL_StrBufFree(str);
    str = GFL_MsgDataLoadStrbufNew(work->msgData, 13);
    WordSetNumber(work->wordSet, 0, pokemon->maxHp, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
    PrintWindow_PrintColor(&work->windows[winIdx], work->printQueue, x + width, y, work->strBuf, work->smallFont, BPLIST_COLOR_WHITE);
    GFL_StrBufFree(str);
}


void func_ov198_0220c2f8(BPlistWork *work, u32 winIdx, u32 pos, u8 x, u8 y) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    u8 color = 1;
    u8 fill = func_0202d01c(pokemon->hp, pokemon->maxHp, 48);

    switch (func_0202d034(pokemon->hp, pokemon->maxHp)) {
    case 3:
        return;
    case 0:
        color = 1;
        break;
    case 1:
        color = 3;
        break;
    case 2:
        color = 5;
        break;
    }
    GFL_BitmapFillArea(BmpWin_GetBitmap(work->windows[winIdx].window), x, y + 3, fill, 1, color);
    GFL_BitmapFillArea(BmpWin_GetBitmap(work->windows[winIdx].window), x, y + 4, fill, 1, color + 1);
}


void func_ov198_0220c398(BPlistWork *work, u32 winIdx, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str = GFL_StrBufCreate(16, work->param->heapId);
    StrBuf *tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, 14);
    PrintQueue *queue;

    loadAbilityNameToStrbuf(work->wordSet, 0, pokemon->ability);
    GFL_WordSetFormatStrbuf(work->wordSet, str, tmpl);
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 0, 0, str, work->param->font,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(tmpl);
    GFL_StrBufFree(str);
}

void func_ov198_0220c42c(BPlistWork *work, u32 winIdx, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str;
    PrintQueue *queue;

    if (pokemon->item == 0) {
        str = GFL_MsgDataLoadStrbufNew(work->msgData, 30);
    } else {
        StrBuf *tmpl;

        str = GFL_StrBufCreate(18, work->param->heapId);
        tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, 15);
        loadItemNameToStrbuf(work->wordSet, 0, pokemon->item);
        GFL_WordSetFormatStrbuf(work->wordSet, str, tmpl);
        GFL_StrBufFree(tmpl);
    }
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 0, 0, str, work->param->font,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
}

void func_ov198_0220c4dc(BPlistWork *work, u32 move, u32 winIdx, u32 msgId, u16 x, s16 y, u32 color) {
    BmpWin *win = work->windows[winIdx].window;
    StrBuf *str = GFL_StrBufCreate(16, work->param->heapId);
    StrBuf *tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, msgId);
    PrintQueue *queue;

    loadMoveNameToStrbuf(work->wordSet, 0, move);
    GFL_WordSetFormatStrbuf(work->wordSet, str, tmpl);
    if (x == 0xFFFF) {
        x = (func_02045738(win) * 8 - GFL_FontGetBlockWidth(str, work->param->font, 0)) / 2;
    }
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), x, y, str, work->param->font, color);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(tmpl);
    GFL_StrBufFree(str);
}





void func_ov198_0220c5ac(BPlistWork *work, u32 winIdx, u8 x, u8 y) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, 20);
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), x, y, str, work->param->font,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
}

static void func_ov198_0220c60c(BPlistWork *work, u32 msgId) {
    StrBuf *str;
    PrintQueue *queue;

    GFL_BitmapFill(BmpWin_GetBitmap(work->msgWins[0].window), 15);
    str = GFL_MsgDataLoadStrbufNew(work->msgData, msgId);
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->msgWins[0].window), 0, 0, str, work->param->font, BPLIST_COLOR_BLACK);
    work->msgWins[0].flushPending = TRUE;
    GFL_StrBufFree(str);
    work->infoFrameState = 1;
}



// pokebw2's BPlistBmp_PrintInfo9
void func_ov198_0220c67c(BPlistWork *work) {
    func_ov198_0220c60c(work, 9);
}

// pokebw2's BPlistBmp_PrintInfo10
void func_ov198_0220c688(BPlistWork *work) {
    func_ov198_0220c60c(work, 10);
}

void func_ov198_0220c694(BPlistWork *work, u32 winIdx, u32 msgId) {
    BmpWin *win = work->windows[winIdx].window;
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, msgId);
    u32 width = GFL_FontGetBlockWidth(str, work->param->font, 0);
    Font *font = work->param->font;
    PrintQueue *queue;
    s16 x;

    x = (func_02045738(win) * 8 - width) / 2;
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), x, 5, str, font, BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
}

void func_ov198_0220c714(BPlistWork *work, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str1;
    StrBuf *str2;
    u16 x;

    str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 33);
    PrintWindow_PrintColor(&work->windows[19], work->printQueue, 0, 0, str1, work->param->font, BPLIST_COLOR_WHITE);
    GFL_StrBufFree(str1);
    str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 34);
    str2 = GFL_StrBufCreate(8, work->param->heapId);
    WordSetNumber(work->wordSet, 0, pokemon->level, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, str2, str1);
    PrintWindow_PrintColor(&work->windows[11], work->printQueue, 0, 0, str2, work->param->font, BPLIST_COLOR_WHITE);
    GFL_StrBufFree(str1);
    GFL_StrBufFree(str2);
    str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 35);
    PrintWindow_PrintColor(&work->windows[20], work->printQueue, 0, 0, str1, work->param->font, BPLIST_COLOR_WHITE);
    GFL_StrBufFree(str1);
    str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 36);
    str2 = GFL_StrBufCreate(14, work->param->heapId);
    if (pokemon->level < 100) {
        WordSetNumber(work->wordSet, 0, pokemon->nextLevelExp - pokemon->exp, 6, 1, TRUE);
    } else {
        WordSetNumber(work->wordSet, 0, 0, 6, 1, TRUE);
    }
    GFL_WordSetFormatStrbuf(work->wordSet, str2, str1);
    x = func_02045738(work->windows[12].window) * 8 - GFL_FontGetBlockWidth(str2, work->param->font, 0);
    PrintWindow_PrintColor(&work->windows[12], work->printQueue, x, 0, str2, work->param->font, BPLIST_COLOR_STAT);
    GFL_StrBufFree(str1);
    GFL_StrBufFree(str2);
}


void func_ov198_0220c8f0(BPlistWork *work, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 42);
    StrBuf *str2;
    StrBuf *str3;
    u8 width;
    u8 x;
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[14].window), 0, 0, str1, work->param->font, BPLIST_COLOR_WHITE);
    work->windows[14].flushPending = TRUE;
    GFL_StrBufFree(str1);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 43);
    str3 = GFL_StrBufCreate(8, work->param->heapId);
    WordSetNumber(work->wordSet, 0, pokemon->attack, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, str3, str2);
    width = GFL_FontGetBlockWidth(str3, work->param->font, 0);
    x = func_02045738(work->windows[5].window) * 8 - width;
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[5].window), x, 0, str3, work->param->font, BPLIST_COLOR_STAT);
    work->windows[5].flushPending = TRUE;
    GFL_StrBufFree(str2);
    GFL_StrBufFree(str3);
}

void func_ov198_0220c9f0(BPlistWork *work, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 44);
    StrBuf *str2;
    StrBuf *str3;
    u8 width;
    u8 x;
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[15].window), 0, 0, str1, work->param->font, BPLIST_COLOR_WHITE);
    work->windows[15].flushPending = TRUE;
    GFL_StrBufFree(str1);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 45);
    str3 = GFL_StrBufCreate(8, work->param->heapId);
    WordSetNumber(work->wordSet, 0, pokemon->defense, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, str3, str2);
    width = GFL_FontGetBlockWidth(str3, work->param->font, 0);
    x = func_02045738(work->windows[6].window) * 8 - width;
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[6].window), x, 0, str3, work->param->font, BPLIST_COLOR_STAT);
    work->windows[6].flushPending = TRUE;
    GFL_StrBufFree(str2);
    GFL_StrBufFree(str3);
}

void func_ov198_0220caf0(BPlistWork *work, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 50);
    StrBuf *str2;
    StrBuf *str3;
    u8 width;
    u8 x;
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[16].window), 0, 0, str1, work->param->font, BPLIST_COLOR_WHITE);
    work->windows[16].flushPending = TRUE;
    GFL_StrBufFree(str1);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 51);
    str3 = GFL_StrBufCreate(8, work->param->heapId);
    WordSetNumber(work->wordSet, 0, pokemon->speed, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, str3, str2);
    width = GFL_FontGetBlockWidth(str3, work->param->font, 0);
    x = func_02045738(work->windows[7].window) * 8 - width;
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[7].window), x, 0, str3, work->param->font, BPLIST_COLOR_STAT);
    work->windows[7].flushPending = TRUE;
    GFL_StrBufFree(str2);
    GFL_StrBufFree(str3);
}

void func_ov198_0220cbf0(BPlistWork *work, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 46);
    StrBuf *str2;
    StrBuf *str3;
    u8 width;
    u8 x;
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[17].window), 0, 0, str1, work->param->font, BPLIST_COLOR_WHITE);
    work->windows[17].flushPending = TRUE;
    GFL_StrBufFree(str1);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 47);
    str3 = GFL_StrBufCreate(8, work->param->heapId);
    WordSetNumber(work->wordSet, 0, pokemon->spAttack, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, str3, str2);
    width = GFL_FontGetBlockWidth(str3, work->param->font, 0);
    x = func_02045738(work->windows[8].window) * 8 - width;
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[8].window), x, 0, str3, work->param->font, BPLIST_COLOR_STAT);
    work->windows[8].flushPending = TRUE;
    GFL_StrBufFree(str2);
    GFL_StrBufFree(str3);
}

void func_ov198_0220ccf0(BPlistWork *work, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 48);
    StrBuf *str2;
    StrBuf *str3;
    u8 width;
    u8 x;
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[18].window), 0, 0, str1, work->param->font, BPLIST_COLOR_WHITE);
    work->windows[18].flushPending = TRUE;
    GFL_StrBufFree(str1);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 49);
    str3 = GFL_StrBufCreate(8, work->param->heapId);
    WordSetNumber(work->wordSet, 0, pokemon->spDefense, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, str3, str2);
    width = GFL_FontGetBlockWidth(str3, work->param->font, 0);
    x = func_02045738(work->windows[9].window) * 8 - width;
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[9].window), x, 0, str3, work->param->font, BPLIST_COLOR_STAT);
    work->windows[9].flushPending = TRUE;
    GFL_StrBufFree(str2);
    GFL_StrBufFree(str3);
}

void func_ov198_0220cdf0(BPlistWork *work, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 38);
    StrBuf *str2;
    StrBuf *str3;
    StrBuf *str4;
    u32 width1;
    u32 width2;
    u16 x;

    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[13].window), 0, 0, str1, work->param->font, BPLIST_COLOR_WHITE);
    }
    work->windows[13].flushPending = TRUE;
    GFL_StrBufFree(str1);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 41);
    width1 = GFL_FontGetBlockWidth(str2, work->param->font, 0);
    x = (func_02045738(work->windows[4].window) * 8 - width1) / 2;
    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[4].window), x, 0, str2, work->param->font, BPLIST_COLOR_STAT);
    }
    work->windows[4].flushPending = TRUE;
    GFL_StrBufFree(str2);
    str3 = GFL_MsgDataLoadStrbufNew(work->msgData, 39);
    str4 = GFL_StrBufCreate(8, work->param->heapId);
    WordSetNumber(work->wordSet, 0, pokemon->hp, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, str4, str3);
    width2 = GFL_FontGetBlockWidth(str4, work->param->font, 0);
    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[4].window), (s16)(x - width2), 0, str4, work->param->font,
                      BPLIST_COLOR_STAT);
    }
    work->windows[4].flushPending = TRUE;
    GFL_StrBufFree(str3);
    GFL_StrBufFree(str4);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 40);
    str3 = GFL_StrBufCreate(8, work->param->heapId);
    WordSetNumber(work->wordSet, 0, pokemon->maxHp, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, str3, str2);
    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[4].window), (s16)(x + width1), 0, str3, work->param->font,
                      BPLIST_COLOR_STAT);
    }
    work->windows[4].flushPending = TRUE;
    GFL_StrBufFree(str2);
    GFL_StrBufFree(str3);
}







void func_ov198_0220cfc4(BPlistWork *work, u8 pos) {
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_DESCRIPTIONS, work->param->heapId);
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgData, pokemon->ability);
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[2].window), 0, 0, str, work->param->font, BPLIST_COLOR_STAT);
    work->windows[2].flushPending = TRUE;
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
}



void func_ov198_0220d034(BPlistWork *work, u32 winIdx) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, 61);
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 0, 0, str, work->param->font,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
}

void func_ov198_0220d090(BPlistWork *work, u32 winIdx, u32 value) {
    BmpWin *win = work->windows[winIdx].window;
    StrBuf *str;
    StrBuf *tmpl;
    u16 width;
    u16 x;

    if (value == 0) {
        str = GFL_MsgDataLoadStrbufNew(work->msgData, 60);
        width = GFL_FontGetBlockWidth(str, work->param->font, 0);
        x = func_02045738(win) * 8 - width;
        {
            PrintQueue *queue = work->printQueue;

            func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)x, 0, str,
                          work->param->font, BPLIST_COLOR_STAT);
        }
        work->windows[winIdx].flushPending = TRUE;
        GFL_StrBufFree(str);
    } else {
        tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, 62);
        str = GFL_StrBufCreate(8, work->param->heapId);
        WordSetNumber(work->wordSet, 0, value, 3, 0, TRUE);
        GFL_WordSetFormatStrbuf(work->wordSet, str, tmpl);
        width = GFL_FontGetBlockWidth(str, work->param->font, 0);
        x = func_02045738(win) * 8 - width;
        {
            PrintQueue *queue = work->printQueue;

            func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)x, 0, str,
                          work->param->font, BPLIST_COLOR_STAT);
        }
        work->windows[winIdx].flushPending = TRUE;
        GFL_StrBufFree(tmpl);
        GFL_StrBufFree(str);
    }
}


void func_ov198_0220d1bc(BPlistWork *work, u32 winIdx) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, 58);
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 0, 0, str, work->param->font,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
}

void func_ov198_0220d218(BPlistWork *work, u32 winIdx, u32 value) {
    BmpWin *win = work->windows[winIdx].window;
    StrBuf *str;
    StrBuf *tmpl;
    u16 width;
    u16 x;

    if (value <= 1) {
        str = GFL_MsgDataLoadStrbufNew(work->msgData, 60);
        width = GFL_FontGetBlockWidth(str, work->param->font, 0);
        x = func_02045738(win) * 8 - width;
        {
            PrintQueue *queue = work->printQueue;

            func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)x, 0, str,
                          work->param->font, BPLIST_COLOR_STAT);
        }
        work->windows[winIdx].flushPending = TRUE;
        GFL_StrBufFree(str);
    } else {
        tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, 59);
        str = GFL_StrBufCreate(8, work->param->heapId);
        WordSetNumber(work->wordSet, 0, value, 3, 0, TRUE);
        GFL_WordSetFormatStrbuf(work->wordSet, str, tmpl);
        width = GFL_FontGetBlockWidth(str, work->param->font, 0);
        x = func_02045738(win) * 8 - width;
        {
            PrintQueue *queue = work->printQueue;

            func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)x, 0, str,
                          work->param->font, BPLIST_COLOR_STAT);
        }
        work->windows[winIdx].flushPending = TRUE;
        GFL_StrBufFree(tmpl);
        GFL_StrBufFree(str);
    }
}


void func_ov198_0220d348(BPlistWork *work, u32 winIdx, u32 msgId) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_13, work->param->heapId);
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    PrintQueue *queue = work->printQueue;

    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 0, 0, str, work->param->font,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
}

void func_ov198_0220d3b4(BPlistWork *work, u32 winIdx) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, 63);
    PrintQueue *queue;

    GFL_FontGetBlockWidth(str, work->param->font, 0);
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 0, 0, str, work->param->font,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
}

void func_ov198_0220d418(BPlistWork *work, u32 winIdx, u32 category) {
    StrBuf *str;
    PrintQueue *queue;

    switch (category) {
    case 0:
        str = GFL_MsgDataLoadStrbufNew(work->msgData, 65);
        break;
    case 1:
        str = GFL_MsgDataLoadStrbufNew(work->msgData, 64);
        break;
    case 2:
        str = GFL_MsgDataLoadStrbufNew(work->msgData, 66);
        break;
    }
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 0, 0, str, work->param->font,
                  BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
}

void func_ov198_0220d494(BPlistWork *work, u32 winIdx, u32 value1, u32 value2) {
    BmpWin *win = work->windows[winIdx].window;
    StrBuf *str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 56);
    u32 width1 = GFL_FontGetBlockWidth(str1, work->param->font, 0);
    u32 x = (func_02045738(win) * 8 - width1) / 2;
    StrBuf *str2;
    StrBuf *buf2;
    StrBuf *str3;
    StrBuf *buf3;
    u32 width2;

    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)x, 0, str1, work->param->font,
                      BPLIST_COLOR_WHITE);
    }
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str1);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 54);
    buf2 = GFL_StrBufCreate(6, work->param->heapId);
    WordSetNumber(work->wordSet, 0, value1, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, buf2, str2);
    width2 = GFL_FontGetBlockWidth(buf2, work->param->font, 0);
    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)(x - width2), 0, buf2,
                      work->param->font, BPLIST_COLOR_WHITE);
    }
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str2);
    GFL_StrBufFree(buf2);
    str3 = GFL_MsgDataLoadStrbufNew(work->msgData, 55);
    buf3 = GFL_StrBufCreate(6, work->param->heapId);
    WordSetNumber(work->wordSet, 0, value2, 3, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, buf3, str3);
    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)(x + width1), 0, buf3,
                      work->param->font, BPLIST_COLOR_WHITE);
    }
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str3);
    GFL_StrBufFree(buf3);
}


void func_ov198_0220d628(BPlistWork *work, u32 winIdx) {
    StrBuf *str;
    u32 width;
    PrintQueue *queue;

    if (work->param->slot == 4) {
        str = GFL_MsgDataLoadStrbufNew(work->msgData, 69);
    } else {
        str = GFL_MsgDataLoadStrbufNew(work->msgData, 68);
    }
    width = GFL_FontGetBlockWidth(str, work->param->font, 0);
    queue = work->printQueue;
    func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)((96 - width) / 2), 5, str,
                  work->param->font, BPLIST_COLOR_WHITE);
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str);
}







void func_ov198_0220d6ac(BPlistWork *work) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(work->msgData, 70);

    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
    GFL_StrBufFree(str);
    func_ov198_0220e13c(work);
}

void func_ov198_0220d6dc(BPlistWork *work, BPlistMove *move, u32 winIdx) {
    StrBuf *buf = GFL_StrBufCreate(6, work->param->heapId);
    StrBuf *str1 = GFL_MsgDataLoadStrbufNew(work->msgData, 53);
    StrBuf *str2;
    StrBuf *str3;
    StrBuf *str4;
    u32 width1;
    u32 width2;

    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 40, 24, str1, work->param->font,
                      BPLIST_COLOR_WHITE);
    }
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str1);
    str2 = GFL_MsgDataLoadStrbufNew(work->msgData, 56);
    width1 = GFL_FontGetBlockWidth(str2, work->param->font, 0);
    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), 80, 24, str2, work->param->font,
                      BPLIST_COLOR_WHITE);
    }
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str2);
    str3 = GFL_MsgDataLoadStrbufNew(work->msgData, 55);
    WordSetNumber(work->wordSet, 0, move->maxPp, 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, buf, str3);
    width1 += 80;
    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)width1, 24, buf,
                      work->param->font, BPLIST_COLOR_WHITE);
    }
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str3);
    str4 = GFL_MsgDataLoadStrbufNew(work->msgData, 54);
    WordSetNumber(work->wordSet, 0, move->pp, 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(work->wordSet, buf, str4);
    width2 = GFL_FontGetBlockWidth(buf, work->param->font, 0);
    {
        PrintQueue *queue = work->printQueue;

        func_0201c724(queue, BmpWin_GetBitmap(work->windows[winIdx].window), (s16)(80 - width2), 24, buf,
                      work->param->font, BPLIST_COLOR_WHITE);
    }
    work->windows[winIdx].flushPending = TRUE;
    GFL_StrBufFree(str4);
    GFL_StrBufFree(buf);
}

void func_ov198_0220d884(BPlistWork *work) {
    s16 i;

    work->flushList = data_ov198_02211aff;
    for (i = 0; i < 6; i++) {
        u32 idx;

        GFL_BitmapFill(BmpWin_GetBitmap(work->windows[i].window), 0);
        idx = func_ov198_022110d4(work, i);
        if (work->pokemon[idx].species == 0) {
            work->windows[i].flushPending = TRUE;
            continue;
        }
        func_ov198_0220beb8(work, i, (u16)i, 32, 7);
        if (!work->pokemon[idx].isEgg) {
            func_ov198_0220d920(work, (u8)i);
        }
        if (func_020276c4(work->pokemon[idx].pkm) == APP_STATUS_ICON_NONE) {
            func_ov198_0220d988(work, (u8)i);
        }
    }
}



void func_ov198_0220d920(BPlistWork *work, u32 pos) {
    GFL_BitmapFillArea(BmpWin_GetBitmap(work->windows[pos].window), 92, 32, 24, 8, 0);
    GFL_BitmapFillArea(BmpWin_GetBitmap(work->windows[pos].window), 64, 24, 64, 8, 0);
    func_ov198_0220c16c(work, pos, pos, 92, 32);
    func_ov198_0220c2f8(work, pos, pos, 64, 24);
}



// pokebw2's BPlistBmp_PrintPlateLevel
void func_ov198_0220d988(BPlistWork *work, u8 pos) {
    if (!work->pokemon[func_ov198_022110d4(work, pos)].isEgg) {
        func_ov198_0220c0d0(work, pos, pos, 8, 32);
    }
}

void func_ov198_0220d9b4(BPlistWork *work) {
    int result;
    u32 msgId;

    work->flushList = data_ov198_02211af4;
    func_ov198_0220bda0(work);
    func_ov198_0220da6c(work, work->param->partyIndex);
    result = func_ov198_02210da8(work);
    if (result == 2) {
        msgId = 26;
    } else if (result == 3) {
        msgId = 27;
    } else if (result == 4) {
        msgId = 28;
    } else if (result == 1) {
        msgId = 29;
    } else {
        msgId = 21;
    }
    func_ov198_0220c694(work, 1, msgId);
    if (!work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].isEgg) {
        func_ov198_0220c694(work, 2, 24);
        func_ov198_0220c694(work, 3, 25);
    } else {
        BmpWin_Transfer(work->windows[2].window);
        BmpWin_Transfer(work->windows[3].window);
    }
}

void func_ov198_0220da6c(BPlistWork *work, u8 pos) {
    BmpWin *win = work->windows[0].window;
    BPlistPokemon *pokemon = &work->pokemon[func_ov198_022110d4(work, pos)];
    StrBuf *str = GFL_StrBufCreate(12, work->param->heapId);
    StrBuf *tmpl = GFL_MsgDataLoadStrbufNew(work->msgData, data_ov198_02211b68[pos]);
    StrBuf *sexStr = NULL;
    u8 nameWidth;
    u8 sexWidth;
    u8 gap;
    u8 x;

    loadPokemonNicknameToStrbuf(work->wordSet, 0, pokemon->pkm);
    GFL_WordSetFormatStrbuf(work->wordSet, str, tmpl);
    GFL_StrBufFree(tmpl);

    if (!pokemon->hideSex && !pokemon->isEgg) {
        if (pokemon->sex == 0) {
            sexStr = GFL_MsgDataLoadStrbufNew(work->msgData, 22);
        } else if (pokemon->sex == 1) {
            sexStr = GFL_MsgDataLoadStrbufNew(work->msgData, 23);
        }
    }

    nameWidth = GFL_FontGetBlockWidth(str, work->param->font, 0);
    sexWidth = 0;
    if (sexStr == NULL) {
        gap = 0;
    } else {
        sexWidth = GFL_FontGetBlockWidth(sexStr, work->param->font, 0);
        gap = 8;
    }
    x = (func_02045738(win) * 8 - nameWidth - sexWidth - gap) / 2;
    PrintWindow_PrintColor(&work->windows[0], work->printQueue, x, 7, str, work->param->font, BPLIST_COLOR_WHITE);
    GFL_StrBufFree(str);

    if (sexStr != NULL) {
        if (pokemon->sex == 0) {
            PrintWindow_PrintColor(&work->windows[0], work->printQueue, x + nameWidth + gap, 8, sexStr,
                                   work->param->font, BPLIST_COLOR_MALE);
        } else {
            PrintWindow_PrintColor(&work->windows[0], work->printQueue, x + nameWidth + gap, 8, sexStr,
                                   work->param->font, BPLIST_COLOR_FEMALE);
        }
        GFL_StrBufFree(sexStr);
    }
}

void func_ov198_0220dc04(BPlistWork *work) {
    u32 i;

    work->flushList = data_ov198_02211b06;
    func_ov198_0220bda0(work);
    func_ov198_0220beb8(work, 0, work->param->partyIndex, 0, 0);
    for (i = 0; i < 4; i++) {
        BPlistMove *move = &work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[i];

        if (move->move == 0) {
            work->windows[i + 1].flushPending = TRUE;
        } else {
            func_ov198_0220c4dc(work, move->move, i + 1, data_ov198_02211b3c[i], 0xffff, 7, BPLIST_COLOR_WHITE);
            func_ov198_0220d6dc(work, move, i + 1);
        }
    }
    func_ov198_0220c694(work, 5, 0x18);
}

void func_ov198_0220dca8(BPlistWork *work) {
    work->flushList = data_ov198_02211b50;
    func_ov198_0220bda0(work);
    func_ov198_0220beb8(work, 0, work->param->partyIndex, 0, 0);
    func_ov198_0220cdf0(work, work->param->partyIndex);
    func_ov198_0220c2f8(work, 10, work->param->partyIndex, 0, 0);
    BmpWin_FlushChar(work->windows[10].window);
    func_ov198_0220c714(work, work->param->partyIndex);
    func_ov198_0220c8f0(work, work->param->partyIndex);
    func_ov198_0220c9f0(work, work->param->partyIndex);
    func_ov198_0220caf0(work, work->param->partyIndex);
    func_ov198_0220cbf0(work, work->param->partyIndex);
    func_ov198_0220ccf0(work, work->param->partyIndex);
    func_ov198_0220c398(work, 1, work->param->partyIndex);
    func_ov198_0220c42c(work, 3, work->param->partyIndex);
    func_ov198_0220cfc4(work, work->param->partyIndex);
    func_ov198_0220c694(work, 21, 25);
}

void func_ov198_0220dd78(BPlistWork *work) {
    BPlistMove *move;

    work->flushList = data_ov198_02211b20;
    func_ov198_0220bda0(work);
    move = &work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[work->param->slot];
    func_ov198_0220beb8(work, 6, work->param->partyIndex, 0, 0);
    func_ov198_0220c5ac(work, 7, 0, 0);
    func_ov198_0220c4dc(work, move->move, 0, data_ov198_02211b3c[work->param->slot], 0, 0, BPLIST_COLOR_WHITE);
    func_ov198_0220d034(work, 8);
    func_ov198_0220d090(work, 2, move->accuracy);
    func_ov198_0220d1bc(work, 9);
    func_ov198_0220d218(work, 3, move->power);
    func_ov198_0220d348(work, 4, move->move);
    func_ov198_0220d3b4(work, 10);
    func_ov198_0220d418(work, 5, move->category);
    func_ov198_0220d494(work, 1, move->pp, move->maxPp);
}

void func_ov198_0220de4c(BPlistWork *work) {
    BPlistMove newMove;
    u32 i;

    work->flushList = data_ov198_02211b0d;
    func_ov198_0220bda0(work);
    func_ov198_0220beb8(work, 0, work->param->partyIndex, 0, 0);
    for (i = 0; i < 4; i++) {
        BPlistMove *move = &work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[i];

        if (move->move == 0) {
            work->windows[i + 1].flushPending = TRUE;
        } else {
            func_ov198_0220c4dc(work, move->move, i + 1, data_ov198_02211b3c[i], 0xffff, 7, BPLIST_COLOR_WHITE);
            func_ov198_0220d6dc(work, move, i + 1);
        }
    }
    func_ov198_0220c4dc(work, work->param->move, 5, 0x53, 0xffff, 7, BPLIST_COLOR_WHITE);
    newMove.pp = PML_MoveGetParam(work->param->move, MOVE_PARAM_PP);
    newMove.maxPp = newMove.pp;
    func_ov198_0220d6dc(work, &newMove, 5);
}

void func_ov198_0220df20(BPlistWork *work) {
    BPlistMove *move;
    u8 slot;

    work->flushList = data_ov198_02211b2c;
    func_ov198_0220bda0(work);
    func_ov198_0220beb8(work, 0, work->param->partyIndex, 0, 0);
    func_ov198_0220c5ac(work, 2, 0, 0);
    func_ov198_0220d034(work, 4);
    func_ov198_0220d1bc(work, 5);
    func_ov198_0220d3b4(work, 9);
    slot = work->param->slot;
    if (slot < 4) {
        move = &work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[slot];
        func_ov198_0220c4dc(work, move->move, 1, data_ov198_02211b3c[slot], 0, 0, BPLIST_COLOR_WHITE);
        func_ov198_0220d090(work, 6, move->accuracy);
        func_ov198_0220d218(work, 7, move->power);
        func_ov198_0220d348(work, 8, move->move);
        func_ov198_0220d418(work, 10, move->category);
        func_ov198_0220d494(work, 3, move->pp, move->maxPp);
    } else {
        u32 pp = PML_MoveGetParam(work->param->move, MOVE_PARAM_PP);

        func_ov198_0220c4dc(work, work->param->move, 1, 0x53, 0, 0, BPLIST_COLOR_WHITE);
        func_ov198_0220d348(work, 8, work->param->move);
        if (PML_MoveIsAlwaysHit(work->param->move) == TRUE) {
            func_ov198_0220d090(work, 6, 0);
        } else {
            func_ov198_0220d090(work, 6, PML_MoveGetParam(work->param->move, MOVE_PARAM_ACCURACY));
        }
        func_ov198_0220d218(work, 7, PML_MoveGetParam(work->param->move, MOVE_PARAM_POWER));
        func_ov198_0220d418(work, 10, PML_MoveGetParam(work->param->move, MOVE_PARAM_CATEGORY));
        func_ov198_0220d494(work, 3, pp, pp);
    }
    func_ov198_0220d628(work, 11);
}

void func_ov198_0220e088(BPlistWork *work) {
    u32 i;

    work->flushList = data_ov198_02211af9;
    func_ov198_0220bda0(work);
    func_ov198_0220beb8(work, 0, work->param->partyIndex, 0, 0);
    for (i = 0; i < 4; i++) {
        BPlistMove *move = &work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)].moves[i];

        if (move->move == 0) {
            work->windows[i + 1].flushPending = TRUE;
        } else {
            func_ov198_0220c4dc(work, move->move, i + 1, data_ov198_02211b3c[i], 0xffff, 7, BPLIST_COLOR_WHITE);
            func_ov198_0220d6dc(work, move, i + 1);
        }
    }
    if (GetItemParam(work->param->item, ITEM_PARAM_PP_RESTORE_ALL, work->param->heapId) == 0) {
        func_ov198_0220c60c(work, 0x68);
    }
}









void func_ov198_0220e13c(BPlistWork *work) {
    BmpWin_DrawFrame(work->msgWins[1].window, 2, 1, 14);
    GFL_BitmapFill(BmpWin_GetBitmap(work->msgWins[1].window), 15);
    func_ov198_0220e168(work);
}

static void func_ov198_0220e168(BPlistWork *work) {
    BmpWin *window;

    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    work->printStream = func_0201cce4(work->msgWins[1].window, 0, 0, work->strBuf, work->param->font, func_02012ffc(),
                                      work->tcbExMgr, 10, work->param->heapId, 15);
    window = work->msgWins[1].window;
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

void func_ov198_0220e1d4(BPlistWork *work) {
    StrBuf *str;
    BPlistPokemon *pokemon;

    pokemon = &work->pokemon[func_ov198_022110d4(work, work->param->partyIndex)];
    str = GFL_MsgDataLoadStrbufNew(work->msgData, 105);
    loadPokemonNicknameToStrbuf(work->wordSet, 0, pokemon->pkm);
    loadMoveNameToStrbuf(work->wordSet, 1, MOVE_EMBARGO);
    GFL_WordSetFormatStrbuf(work->wordSet, work->strBuf, str);
    GFL_StrBufFree(str);
}

void func_ov198_0220e22c(BPlistWork *work) {
    func_ov196_0220b0b0(work->msgWins, work->printQueue, 2);
    func_ov196_0220b0b0(work->windows, work->printQueue, work->windowCount);
}





void func_ov198_0220e25c(BPlistWork *work) {
    func_ov196_0220b094(work->windows, work->flushList);
    func_ov198_0220e27c(work);
}

void func_ov198_0220e27c(BPlistWork *work) {
    if (work->infoFrameState == 1) {
        BmpWin_DrawFrame(work->msgWins[0].window, 2, 1, 14);
        func_ov196_0220b07c(&work->msgWins[0]);
        work->infoFrameState = 0;
    }
}



