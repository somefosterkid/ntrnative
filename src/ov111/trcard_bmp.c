// Black's code at ov111 0x021c6000-0x021c6d9c. The name is pokebw2's file between trainer_card.c and trcard_obj.c (22
// functions there, 21 here).

#include "types.h"
#include "app/trainer_card.h"
#include "field/trcard_sys.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "struct_decls.h"
#include "system/printsys.h"
#include "system/str_tool.h"
#include "system/wordset.h"
#include "system/bmp_winframe.h"
#include "gfl/arc_util.h"
#include "save/timesig_savedata.h"
#include "system/game_data.h"
#include "system/union_view.h"
#include "system/pms_draw.h"
#include "save/playtime.h"

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
} TrCardBmpWinDef;

const TrCardBmpWinDef data_ov111_021c8773[8] = {
#include "asset/trcard_bmp_data_ov111_021c8773.inc"
};

void func_ov111_021c6c38(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *strbuf);
void func_ov111_021c6b5c(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, u32 value, u32 pad, int color);
void func_ov111_021c6954(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, u32 value, u8 digits, u32 pad, int format, int color);
void func_ov111_021c68f0(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, s32 value, u8 digits, u32 pad, int index);
void func_ov111_021c6c7c(TrainerCardWork *wk, BmpWin *win, int y, const TrainerCardData *data, StrBuf *tmp, int color);
void func_ov111_021c69d4(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, u32 pad, int color);
void func_ov111_021c6a84(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, u32 pad, int color);

void func_ov111_021c6000(TrainerCardWork *wk) {
    const TrCardBmpWinDef *defs = data_ov111_021c8773;
    u8 i;

    BmpWin_InitAllocator(wk->heapId);
    for (i = 0; i < 8; i++) {
        wk->win[i] = BmpWin_CreateDynamic(defs[i].bg, defs[i].x, defs[i].y, defs[i].width, defs[i].height, defs[i].palette, 1);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->win[i]), 0);
    }
    GFL_BGSysFillChar(7, 0, 1, 0);
    LoadSysMsgBox(4, 0x31, 0xd, wk->frameType, wk->heapId);
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x1c0, 0x20, wk->heapId);
    GFL_BGSysQueueScrLoad(7);
    GFL_BGSysQueueScrLoad(4);
    wk->printQueue = func_0201c440(wk->heapId);
}

void func_ov111_021c60a0(TrainerCardWork *wk) {
    u32 i;

    func_0201c4c0(wk->printQueue);
    i = 0;
    do {
        BmpWin_Free(wk->win[i]);
        i = i + 1 & 0xffff;
    } while (i < 8);
    BmpWin_FreeAllocator();
}

void func_ov111_021c60cc(TrainerCardWork *wk, BmpWin **win, const TrainerCardData *data) {
    StrBuf *tmp;
    u8 i;

    for (i = 0; i < 5; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(win[i]), 0);
        if (i != TRCARD_WIN_POKEDEX || (i == TRCARD_WIN_POKEDEX && data->unk04_2)) {
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win[i]), 0, 0, wk->labels[i], wk->font);
        }
    }
    tmp = wk->tmpBuf;
    GFL_StrBufLoadString(wk->nameBuf, data->name);
    func_ov111_021c6c38(wk, win[0], 0x88, 8, 0, wk->nameBuf);
    if (data->unk04_2) {
        func_ov111_021c6b5c(wk, win[3], 0x88, 8, 0, tmp, data->seenCount, 1, 0);
    }
    func_ov111_021c61ec(wk, data->unk06, 0);
    func_ov111_021c628c(wk, data->unk48, 0);
    func_ov111_021c6954(wk, win[4], 0xd8, 8, 0, tmp, data->money, 7, 1, 1, 0);
    func_0202de5c(wk->pmsDraw, 0);
    func_0202dc48(wk->pmsDraw, win[5], &wk->data->greeting, 0);
    if (wk->side != 0) {
        func_0202dcb0(wk->pmsDraw, 0, 0);
    }
}


void func_ov111_021c61ec(TrainerCardWork *wk, int view, BOOL flush) {
    MsgData *msgData;
    TimeSigSave *timeSig;
    StrBuf *strbuf;
    u32 result;
    GFLBitmap *pGVar4;
    msgData = GFL_MsgSysLoadData(0, 2, 0xbf, wk->heapId);
    timeSig = getTimeSigSaveBlock(wk->param->gameData);
    if (func_020089f4(timeSig) != 0) {
        strbuf = GFL_MsgDataLoadStrbufNew(msgData, ((u32)func_02025c9c(view)));
    } else {
        strbuf = GFL_MsgDataLoadStrbufNew(msgData, 0);
    }
    result = GFL_FontGetBlockWidth(strbuf, wk->font, 0);
    pGVar4 = BmpWin_GetBitmap(wk->win[1]);
    GFL_BitmapFill(pGVar4, 0);
    pGVar4 = BmpWin_GetBitmap(wk->win[1]);
    GFL_TextRendererDrawToBitmap(pGVar4, (s16)((0x80 - result) * 0x10000 >> 0x10), 0, strbuf, wk->font);
    GFL_StrBufFree(strbuf);
    GFL_MsgDataFree(msgData);
    if (flush != 0) {
        BmpWin_FlushMap(wk->win[1]);
        BmpWin_FlushChar(wk->win[1]);
        GFL_BGSysQueueScrLoad(7);
    }
}


void func_ov111_021c628c(TrainerCardWork *wk, u32 personality, BOOL flush) {
    StrBuf *strbuf;
    u32 result;
    GFLBitmap *pGVar2;

    strbuf = GFL_MsgDataLoadStrbufNew(wk->msgData, personality + 0x1f);
    result = GFL_FontGetBlockWidth(strbuf, wk->font, 0);
    pGVar2 = BmpWin_GetBitmap(wk->win[2]);
    GFL_BitmapFill(pGVar2, 0);
    pGVar2 = BmpWin_GetBitmap(wk->win[2]);
    GFL_TextRendererDrawToBitmap(pGVar2, 0, 0, wk->labels[2], wk->font);
    pGVar2 = BmpWin_GetBitmap(wk->win[2]);
    GFL_TextRendererDrawToBitmap(pGVar2, (s16)((0x80 - result) * 0x10000 >> 0x10), 0, strbuf, wk->font);
    GFL_StrBufFree(strbuf);
    if (flush != 0) {
        BmpWin_FlushMap(wk->win[2]);
        BmpWin_FlushChar(wk->win[2]);
        GFL_BGSysQueueScrLoad(7);
    }
}

const u8 data_ov111_021c876c[7] = {1, 1, 1, 1, 1, 1, 1};
void func_ov111_021c631c(TrainerCardWork *wk, BmpWin **win) {
    int i;

    if (wk->side != 0) {
        BmpWin_FlushMap(win[6]);
        BmpWin_FlushChar(win[6]);
    } else {
        for (i = 0; i <= 5; i++) {
            if (data_ov111_021c876c[i] != 0) {
                BmpWin_FlushMap(win[i]);
                BmpWin_FlushChar(win[i]);
            }
        }
    }
    GFL_BGSysQueueScrLoad(7);
    GFL_BGSysQueueScrLoad(4);
}


void func_ov111_021c636c(TrainerCardWork *wk, BmpWin **win, TrainerCardData *data) {
    func_ov111_021c6874(wk, (s32)wk->scroll, 0, 0);
}

const u16 data_ov111_021c8768[2] = {0x0440, 0x1060};

void func_ov111_021c6380(BmpWin *win, TrainerCardWork *wk, int x, int y, StrBuf *strbuf, s32 color) {
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), x, y, strbuf, wk->font, data_ov111_021c8768[color]);
}

void func_ov111_021c63b8(TrainerCardWork *wk, BmpWin *win, u32 idx, int y, s32 color) {
    StrBuf *tmp = wk->tmpBuf;
    TrainerCardData *data = wk->data;
    u32 width;

    switch (idx) {
    case 0:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[5], color);
        func_ov111_021c68f0(wk, win, 0x60, 0, y, tmp, data->trainerId, 5, 2, color);
        return;
    case 1:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[6], color);
        func_ov111_021c6c7c(wk, win, y, wk->data, wk->numBuf, color);
        return;
    case 2:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[7], color);
        return;
    case 3:
        func_ov111_021c69d4(wk, win, 0xd8, 0, y, tmp, 0, color);
        return;
    case 4:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[8], color);
        return;
    case 5:
        if (data->clearMonth != 0) {
            func_ov111_021c6a84(wk, win, 0xd8, 0, y, tmp, 0, color);
        } else {
            width = GFL_FontGetBlockWidth(wk->labels[24], wk->font, 0);
            func_ov111_021c6380(win, wk, 0xd8 - width, y, wk->labels[24], color);
        }
        return;
    case 6:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[9], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk38, 9, 1, color);
        return;
    case 7:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[10], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk3C, 9, 1, color);
        return;
    case 8:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[11], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk40, 9, 1, color);
        return;
    case 9:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[12], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk44, 9, 1, color);
        return;
    case 10:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[13], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk4C, 9, 1, color);
        return;
    case 11:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[14], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk50, 9, 1, color);
        return;
    case 12:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[15], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk54, 9, 1, color);
        return;
    case 13:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[16], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk58, 5, 1, color);
        return;
    case 14:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[17], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk5C, 4, 1, color);
        return;
    case 15:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[18], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->passPowerCount, 2, 1, color);
        return;
    case 16:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[19], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk60, 4, 1, color);
        return;
    case 17:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[20], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->palParkHighScore, 3, 1, color);
        return;
    case 18:
        func_ov111_021c6380(win, wk, 0, y, wk->labels[21], color);
        func_ov111_021c68f0(wk, win, 0xd8, 8, y, tmp, data->unk64, 4, 1, color);
        return;
    default:
        return;
    }
}



// pokebw2's TrCardBmp_AddRow
void func_ov111_021c67e8(TrainerCardWork *wk, int row) {
    wk->rows[wk->rowCount] = row;
    wk->rowCount++;
}

void func_ov111_021c6804(TrainerCardWork *wk, TrainerCardData *data) {
    int i;

    for (i = 0; i < 13; i++) {
        func_ov111_021c67e8(wk, i);
    }
    if (data->unk68_0) {
        func_ov111_021c67e8(wk, 14);
        func_ov111_021c67e8(wk, 13);
    }
    if (data->unk68_1) {
        func_ov111_021c67e8(wk, 15);
    }
    if (data->unk68_2) {
        func_ov111_021c67e8(wk, 16);
    }
    if (data->unk68_3) {
        func_ov111_021c67e8(wk, 17);
    }
    if (data->unk68_4) {
        func_ov111_021c67e8(wk, 18);
    }
}

void func_ov111_021c6874(TrainerCardWork *wk, int scroll, BOOL flush, int color) {
    BmpWin *win = wk->win[6];
    int top;
    int y;
    int i;

    GFL_BitmapFill(BmpWin_GetBitmap(win), 0);
    y = scroll % 16;
    top = -(scroll / 16);
    for (i = 0; i < 5; i++) {
        func_ov111_021c63b8(wk, win, wk->rows[top + i], y + i * 16, color);
    }
    if (flush != 0) {
        BmpWin_FlushChar(win);
        BmpWin_FlushMap(win);
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win));
    }
}



void func_ov111_021c68f0(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, s32 value, u8 digits, u32 pad, int index) {
    u32 width;

    GFL_WordSetFormatNumber(tmp, value, digits, pad, TRUE);
    width = GFL_FontGetBlockWidth(tmp, wk->font, 0);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), x - (width + margin), y, tmp, wk->font, data_ov111_021c8768[index]);
}

void func_ov111_021c6954(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, u32 value, u8 digits, u32 pad, int format, int color) {
    u32 width;

    WordSetNumber(wk->wordSet, 0, value, digits, pad, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, tmp, wk->formats[format]);
    width = GFL_FontGetBlockWidth(tmp, wk->font, 0);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), x - (width + margin), y, tmp, wk->font, data_ov111_021c8768[color]);
}

void func_ov111_021c69d4(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, u32 pad, int color) {
    u32 width;

    WordSetNumber(wk->wordSet, 0, wk->data->startYear + 2000, 4, pad, TRUE);
    WordSetNumber(wk->wordSet, 1, wk->data->startMonth, 2, pad, TRUE);
    WordSetNumber(wk->wordSet, 2, wk->data->startDay, 2, pad, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, tmp, wk->formats[2]);
    width = GFL_FontGetBlockWidth(tmp, wk->font, 0);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), x - (width + margin), y, tmp, wk->font, data_ov111_021c8768[color]);
}

void func_ov111_021c6a84(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, u32 pad, int color) {
    u32 width;

    WordSetNumber(wk->wordSet, 0, wk->data->clearYear + 2000, 4, pad, TRUE);
    WordSetNumber(wk->wordSet, 1, wk->data->clearMonth, 2, pad, TRUE);
    WordSetNumber(wk->wordSet, 2, wk->data->clearDay, 2, pad, TRUE);
    WordSetNumber(wk->wordSet, 3, wk->data->clearHour, 2, 2, TRUE);
    WordSetNumber(wk->wordSet, 4, wk->data->clearMinute, 2, 2, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, tmp, wk->formats[3]);
    width = GFL_FontGetBlockWidth(tmp, wk->font, 0);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), x - (width + margin), y, tmp, wk->font, data_ov111_021c8768[color]);
}




void func_ov111_021c6b5c(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, u32 value, u32 pad, int color) {
    u32 width;

    WordSetNumber(wk->wordSet, 0, value, 3, pad, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, tmp, wk->formats[4]);
    width = GFL_FontGetBlockWidth(tmp, wk->font, 0);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), x - (width + margin), y, tmp, wk->font, data_ov111_021c8768[color]);
}


void func_ov111_021c6bd4(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *tmp, s32 value, u8 digits, u32 pad, int a10, int a11, int color) {
    u32 width;

    GFL_WordSetFormatNumber(tmp, value, digits, pad, TRUE);
    width = GFL_FontGetBlockWidth(tmp, wk->font, 0);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), x - (width + margin), y, tmp, wk->font, data_ov111_021c8768[color]);
}



void func_ov111_021c6c38(TrainerCardWork *wk, BmpWin *win, int x, int margin, int y, StrBuf *strbuf) {
    u32 width = GFL_FontGetBlockWidth(strbuf, wk->font, 0);

    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(win), x - (width + margin), y, strbuf, wk->font);
}

void func_ov111_021c6c7c(TrainerCardWork *wk, BmpWin *win, int y, const TrainerCardData *data, StrBuf *tmp, int color) {
    int hours;

    if (data->playTime == NULL) {
        func_ov111_021c6bd4(wk, win, 216, 0, y, tmp, data->playMinutes, 2, 2, 208, 16, color);
        func_ov111_021c6bd4(wk, win, 216, 19, y, tmp, data->playHours, 3, 1, 173, 27, color);
        GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), 198, y, wk->colonBuf, wk->font, data_ov111_021c8768[color]);
    } else {
        hours = func_0200864c(data->playTime);
        if (hours > 999) {
            hours = 999;
        }
        func_ov111_021c6bd4(wk, win, 216, 0, y, tmp, func_02008650(data->playTime), 2, 2, 208, 16, color);
        func_ov111_021c6bd4(wk, win, 216, 19, y, tmp, hours, 3, 1, 173, 27, color);
        GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(win), 198, y, wk->colonBuf, wk->font, data_ov111_021c8768[color]);
    }
}



void func_ov111_021c6d98(TrainerCardWork *wk, BmpWin *win, int a2, StrBuf *str) {
}
