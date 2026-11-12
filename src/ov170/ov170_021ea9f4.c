// Black's code at ov170 0x021ea9f4-0x021eb6d8. No name is known: the module and address.

#include "types.h"
#include "app/box2_bmp.h"
#include "app/box2_main.h"
#include "app/box2_seq.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/msg.h"
#include "struct_decls.h"
#include "system/wordset.h"
#include "gfl/sound.h"
#include "app/box2_bgwfrm.h"
#include "gfl/arc_util.h"
#include "system/app_menu_common.h"
#include "app/ui/print_msg.h"


typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
} Box2WinSetup;

// Black's windows: BmpWin_CreateDynamic's arguments, in the order (bg, x, y, width, height, palette)
const Box2WinSetup data_ov170_021efc20[28] = {
#include "asset/ov170_021ea9f4_data_ov170_021efc20.inc"
};

void func_ov170_021eae74(Box2AppWork *app, Box2PokeInfo *info, u32 index);
void func_ov170_021eaf20(Box2AppWork *app, Box2PokeInfo *info, u32 index);
void func_ov170_021eafbc(Box2SysWork *syswk, Box2PokeInfo *info, u32 index);
void func_ov170_021eb098(Box2AppWork *app, Box2PokeInfo *info, u32 index);
void func_ov170_021eb114(Box2AppWork *app, Box2PokeInfo *info, u32 index);
void func_ov170_021eb190(Box2AppWork *app, Box2PokeInfo *info, u32 index);
void func_ov170_021eb208(Box2AppWork *app, Box2PokeInfo *info, u32 index);
void func_ov170_021eac28(Box2AppWork *app, u32 index, MsgData *msgData, u32 msgId, int x, int y, Font *font, u16 color);
void func_ov170_021eadb4(Box2AppWork *app, Box2PokeInfo *info, u32 index);


u32 func_ov170_021ea9f4(Box2SysWork *syswk, u32 seq) {
    GFL_SndSEPlay(0x54c);
    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == 1) {
        syswk->moveMode = 2;
    } else {
        syswk->moveMode = 1;
    }
    syswk->unk1C_4 = 0;
    func_ov170_021dd238(syswk);
    return func_ov170_021e8f50(syswk, seq);
}

u32 func_ov170_021eaa3c(Box2SysWork *syswk, u32 seq) {
    GFL_SndSEPlay(0x54c);
    if (func_ov170_021ef5e8(syswk->app->bgWinFrame) == 1) {
        syswk->moveMode = 2;
    } else {
        syswk->moveMode = 1;
    }
    syswk->unk1C_4 = 0;
    return func_ov170_021e8f50(syswk, seq);
}


void func_ov170_021eaa80(Box2SysWork *syswk) {
    const Box2WinSetup *setup;
    u32 i;

    BmpWin_InitAllocator(HEAPID_BOX2_APP);
    setup = data_ov170_021efc20;
    for (i = 0; i < 28; i++) {
        syswk->app->windows[i].window = BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, setup->width, setup->height, setup->palette, TRUE);
        setup++;
    }
}

void func_ov170_021eaac4(Box2SysWork *syswk) {
    u32 i;

    for (i = 0; i < 28; i++) {
        BmpWin_Free(syswk->app->windows[i].window);
    }
    BmpWin_FreeAllocator();
}

void func_ov170_021eaae4(Box2AppWork *app, u32 index) {
    app->flushChar[index >> 3] |= 1 << (index & 7);
}

void func_ov170_021eab04(Box2AppWork *app) {
    u32 i;
    u8 val;
    u8 bit;

    for (i = 0; i < 28; i++) {
        if (app->windows[i].window != NULL) {
            val = i >> 3;
            bit = 1 << (i & 7);
            if (app->flushChar[val] & bit) {
                BmpWin_FlushChar(app->windows[i].window);
                app->flushChar[val] ^= bit;
            }
        }
    }
}

void func_ov170_021eab50(Box2AppWork *app) {
    u32 i;
    GFLBitmap *bitmap;

    func_0201c4e4(app->printQueue);
    for (i = 0; i < 28; i++) {
        PrintQueue *queue = app->printQueue;
        if (app->windows[i].flushPending != 0) {
            bitmap = BmpWin_GetBitmap(app->windows[i].window);
            if (func_0201c6c4(queue, bitmap) == 0) {
                BmpWin_FlushChar(app->windows[i].window);
                app->windows[i].flushPending = 0;
            }
        }
    }
}






void func_ov170_021eaba0(PrintWindow *window) {
    u8 bg;

    BmpWin_FlushMap(window->window);
    bg = BmpWin_GetBGIndex(window->window);
    GFL_BGSysQueueScrLoad(bg);
}

void func_ov170_021eabb8(PrintWindow *window) {
    u8 bg;

    func_020456f8(window->window);
    bg = BmpWin_GetBGIndex(window->window);
    GFL_BGSysQueueScrLoad(bg);
}

void func_ov170_021eabd0(Box2AppWork *app, u32 index, MsgData *msgData, u32 msgId, int x, int y, Font *font, u16 color) {
    StrBuf *str;
    PrintQueue *queue;
    GFLBitmap *bitmap;

    str = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    queue = app->printQueue;
    bitmap = BmpWin_GetBitmap(app->windows[index].window);
    func_0201c724(queue, bitmap, x, y, str, font, color);
    app->windows[index].flushPending = TRUE;
    GFL_StrBufFree(str);
}

void func_ov170_021eac28(Box2AppWork *app, u32 index, MsgData *msgData, u32 msgId, int x, int y, Font *font, u16 color) {
    StrBuf *str;
    PrintQueue *queue;
    GFLBitmap *bitmap;

    str = GFL_MsgDataLoadStrbufNew(msgData, msgId);
    GFL_WordSetFormatStrbuf(app->wordSet, app->expandBuf, str);
    queue = app->printQueue;
    bitmap = BmpWin_GetBitmap(app->windows[index].window);
    func_0201c724(queue, bitmap, x, y, app->expandBuf, font, color);
    app->windows[index].flushPending = TRUE;
    GFL_StrBufFree(str);
}



MsgData *func_ov170_021eac98(void) {
    return GFL_MsgSysLoadData(0, 2, 10, 0x4c);
}

void func_ov170_021eaca8(Box2SysWork *syswk) {
    Box2AppWork *app;
    MsgData *msgData;

    app = syswk->app;
    GFL_BitmapFill(BmpWin_GetBitmap(app->windows[0].window), 0);
    msgData = func_ov170_021eac98();
    func_ov170_021eabd0(app, 0, msgData, syswk->param->mode + 0x25, 0, 8, app->font, 0x440);
    GFL_MsgDataFree(msgData);
    func_ov170_021eaba0(&syswk->app->windows[0]);
}


void func_ov170_021eacfc(Box2SysWork *syswk) {
    GFLBitmap *pGVar1;
    Box2AppWork *app;

    app = syswk->app;
    pGVar1 = BmpWin_GetBitmap(app->windows[10].window);
    GFL_BitmapFill(pGVar1, 0);
    pGVar1 = BmpWin_GetBitmap(app->windows[11].window);
    GFL_BitmapFill(pGVar1, 0);
    pGVar1 = BmpWin_GetBitmap(app->windows[12].window);
    GFL_BitmapFill(pGVar1, 0);
    pGVar1 = BmpWin_GetBitmap(app->windows[13].window);
    GFL_BitmapFill(pGVar1, 0);
    func_ov170_021eabd0(app, 10, app->msgData, 0x6d, 0, 0, app->font, 0x440);
    func_ov170_021eabd0(app, 0xb, app->msgData, 100, 0, 0, app->font, 0x440);
    func_ov170_021eabd0(app, 0xc, app->msgData, 0x65, 0, 0, app->font, 0x440);
    func_ov170_021eabd0(app, 0xd, app->msgData, 0x66, 0, 0, app->font, 0x440);
}

void func_ov170_021eadb4(Box2AppWork *app, Box2PokeInfo *info, u32 index) {
    GFL_BitmapFill(BmpWin_GetBitmap(app->windows[index].window), 0);
    if (info->egg == FALSE) {
        setBoxPokemonSpeciesNameToStrbuf(app->wordSet, 0, info->pkm);
        func_ov170_021eac28(app, index, app->msgData, 0, 0, 0, app->font, 0x440);
    } else {
        func_ov170_021eaae4(app, index);
        return;
    }
    func_ov170_021eaba0(&app->windows[index]);
}


void func_ov170_021eae20(Box2AppWork *app, Box2PokeInfo *info, u32 index) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(app->windows[index].window);
    GFL_BitmapFill(bitmap, 0);
    loadBoxPokemonNameToStrbuf(app->wordSet, 0, info->pkm);
    func_ov170_021eac28(app, index, app->msgData, 1, 0, 0, app->font, 0x440);
    func_ov170_021eaba0(&app->windows[index]);
}

void func_ov170_021eae74(Box2AppWork *app, Box2PokeInfo *info, u32 index) {
    u32 width;

    GFL_BitmapFill(BmpWin_GetBitmap(app->windows[index].window), 0);
    if (info->egg == FALSE) {
        func_ov170_021eac28(app, index, app->msgData, 0x6b, 0, 0, app->font, 0x440);
        width = GFL_FontGetBlockWidth(app->expandBuf, app->font, 0);
        WordSetNumber(app->wordSet, 0, info->level, 3, 0, TRUE);
        func_ov170_021eac28(app, index, app->msgData, 0x67, width, 0, app->font, 0x440);
    } else {
        func_ov170_021eaae4(app, index);
        return;
    }
    func_ov170_021eaba0(&app->windows[index]);
}

void func_ov170_021eaf20(Box2AppWork *app, Box2PokeInfo *info, u32 index) {
    GFL_BitmapFill(BmpWin_GetBitmap(app->windows[index].window), 0);
    if (info->egg == FALSE && info->sexPut == TRUE) {
        if (info->sex == 0) {
            func_ov170_021eabd0(app, index, app->msgData, 0x5f, 0, 0, app->font, 0x14c0);
        } else if (info->sex == 1) {
            func_ov170_021eabd0(app, index, app->msgData, 0x60, 0, 0, app->font, 0xc80);
        } else {
            func_ov170_021eaae4(app, index);
            return;
        }
    } else {
        func_ov170_021eaae4(app, index);
        return;
    }
    func_ov170_021eaba0(&app->windows[index]);
}

void func_ov170_021eafbc(Box2SysWork *syswk, Box2PokeInfo *info, u32 index) {
    u16 number;
    u32 width;

    GFL_BitmapFill(BmpWin_GetBitmap(syswk->app->windows[index].window), 0);
    if (info->egg == FALSE) {
        if (syswk->app->nationalDex == TRUE) {
            number = info->species;
        } else {
            number = syswk->app->regionalDex[info->species];
        }
        if (number != 999) {
            func_ov170_021eac28(syswk->app, index, syswk->app->msgData, 0x6c, 0, 0, syswk->app->font, 0x440);
            width = GFL_FontGetBlockWidth(syswk->app->expandBuf, syswk->app->font, 0);
            WordSetNumber(syswk->app->wordSet, 0, number, 3, 2, TRUE);
            func_ov170_021eac28(syswk->app, index, syswk->app->msgData, 0x68, width, 0, syswk->app->font, 0x440);
        } else {
            func_ov170_021eaae4(syswk->app, index);
            return;
        }
    } else {
        func_ov170_021eaae4(syswk->app, index);
        return;
    }
    func_ov170_021eaba0(&syswk->app->windows[index]);
}

void func_ov170_021eb098(Box2AppWork *app, Box2PokeInfo *info, u32 index) {
    GFL_BitmapFill(BmpWin_GetBitmap(app->windows[index].window), 0);
    if (info->egg == FALSE) {
        loadNatureToStrbuf(app->wordSet, 0, info->nature);
        func_ov170_021eac28(app, index, app->msgData, 0x62, 0, 0, app->font, 0x440);
    } else {
        func_ov170_021eabd0(app, index, app->msgData, 0x6a, 0, 0, app->font, 0x440);
    }
    func_ov170_021eaba0(&app->windows[index]);
}

void func_ov170_021eb114(Box2AppWork *app, Box2PokeInfo *info, u32 index) {
    GFL_BitmapFill(BmpWin_GetBitmap(app->windows[index].window), 0);
    if (info->egg == FALSE) {
        loadAbilityNameToStrbuf(app->wordSet, 0, info->ability);
        func_ov170_021eac28(app, index, app->msgData, 0x61, 0, 0, app->font, 0x440);
    } else {
        func_ov170_021eabd0(app, index, app->msgData, 0x6a, 0, 0, app->font, 0x440);
    }
    func_ov170_021eaba0(&app->windows[index]);
}






void func_ov170_021eb190(Box2AppWork *app, Box2PokeInfo *info, u32 index) {
    GFLBitmap *bitmap;
    bitmap = BmpWin_GetBitmap(app->windows[index].window);
    GFL_BitmapFill(bitmap, 0);
    if (info->item != 0) {
        loadItemNameToStrbuf(app->wordSet, 0, info->item);
        func_ov170_021eac28(app, index, app->msgData, 99, 0, 0, app->font, 0x440);
    } else {
        func_ov170_021eabd0(app, index, app->msgData, 0x69, 0, 0, app->font, 0x440);
    }
    func_ov170_021eaba0(&app->windows[index]);
}

void func_ov170_021eb208(Box2AppWork *app, Box2PokeInfo *info, u32 index) {
    MsgData *msgData;
    u32 i;

    GFL_BitmapFill(BmpWin_GetBitmap(app->windows[index].window), 0);
    msgData = GFL_MsgSysLoadData(0, 2, 0xcb, HEAPID_TAIL(HEAPID_BOX2_APP));
    if (info->egg == FALSE) {
        for (i = 0; i < 4; i++) {
            func_ov170_021eabd0(app, index, msgData, info->waza[i], 0, i << 4, app->font, 0x440);
        }
    } else {
        func_ov170_021eabd0(app, index, app->msgData, 0x6a, 0, 0, app->font, 0x440);
    }
    GFL_MsgDataFree(msgData);
    func_ov170_021eaba0(&app->windows[index]);
}


void func_ov170_021eb2a4(Box2SysWork *syswk, Box2PokeInfo *info) {
    func_ov170_021eadb4(syswk->app, info, 1);
    func_ov170_021eae20(syswk->app, info, 2);
    func_ov170_021eae74(syswk->app, info, 3);
    func_ov170_021eaf20(syswk->app, info, 4);
    func_ov170_021eafbc(syswk, info, 5);
    func_ov170_021eb098(syswk->app, info, 6);
    func_ov170_021eb114(syswk->app, info, 7);
    func_ov170_021eb190(syswk->app, info, 8);
    func_ov170_021eb208(syswk->app, info, 9);
    func_ov170_021eaba0(syswk->app->windows + 10);
    func_ov170_021eaba0(syswk->app->windows + 0xb);
    func_ov170_021eaba0(syswk->app->windows + 0xc);
    func_ov170_021eaba0(syswk->app->windows + 0xd);
}

void func_ov170_021eb324(Box2AppWork *app) {
    u32 i;

    for (i = 1; i <= 9; i++) {
        func_ov170_021eabb8(&app->windows[i]);
        GFL_BitmapFill(BmpWin_GetBitmap(app->windows[i].window), 0);
        func_ov170_021eaae4(app, i);
    }
    func_ov170_021eabb8(&app->windows[10]);
    func_ov170_021eabb8(&app->windows[11]);
    func_ov170_021eabb8(&app->windows[12]);
    func_ov170_021eabb8(&app->windows[13]);
}

void func_ov170_021eb378(Box2SysWork *syswk, u32 index, u32 msgId, u16 frame, u16 color, u32 arrow) {
    BmpWin *window = syswk->app->windows[index].window;
    GFLBitmap *bitmap = BmpWin_GetBitmap(window);
    // bg, x and y are never read, as in pokebw2's DrawButton (box2_bmp.c): the calls stay in the code
    u8 bg = BmpWin_GetBGIndex(window);
    u8 x = BmpWin_GetPosX(window);
    u8 y = BmpWin_GetPosY(window);
    u8 width = func_02045738(window);
    u8 height = func_02045740(window);
    NNSG2dCharacterData *chars;
    void *buf;
    GFLBitmap *tile;
    u8 *pixels;
    u8 i;
    StrBuf *str;

    GFL_BitmapFill(bitmap, 4);

    buf = GFL_G2DIOReadBGNCGR(0x74, 62, TRUE, &chars, HEAPID_TAIL(HEAPID_BOX2_APP));
    pixels = chars->pRawData;
    frame *= 0x20;

    tile = GFL_BitmapWrap(&pixels[frame], 8, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
    GFL_BitmapCopyArea(tile, bitmap, 0, 0, 0, 0, 8, 8, 0xffff);
    GFL_BitmapFree(tile);

    tile = GFL_BitmapWrap(&pixels[frame + 0x40], 8, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
    GFL_BitmapCopyArea(tile, bitmap, 0, 0, (width - 1) * 8, 0, 8, 8, 0xffff);
    GFL_BitmapFree(tile);

    tile = GFL_BitmapWrap(&pixels[frame + 0xc0], 8, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
    GFL_BitmapCopyArea(tile, bitmap, 0, 0, 0, (height - 1) * 8, 8, 8, 0xffff);
    GFL_BitmapFree(tile);

    tile = GFL_BitmapWrap(&pixels[frame + 0x100], 8, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
    GFL_BitmapCopyArea(tile, bitmap, 0, 0, (width - 1) * 8, (height - 1) * 8, 8, 8, 0xffff);
    GFL_BitmapFree(tile);

    tile = GFL_BitmapWrap(&pixels[frame + 0x60], 8, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
    for (i = 1; i < height - 1; i++) {
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, 0, i * 8, 8, 8, 0xffff);
    }
    GFL_BitmapFree(tile);

    tile = GFL_BitmapWrap(&pixels[frame + 0xa0], 8, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
    for (i = 1; i < height - 1; i++) {
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, (width - 1) * 8, i * 8, 8, 8, 0xffff);
    }
    GFL_BitmapFree(tile);

    tile = GFL_BitmapWrap(&pixels[frame + 0x20], 8, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
    for (i = 1; i < width - 1; i++) {
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, i * 8, 0, 8, 8, 0xffff);
    }
    GFL_BitmapFree(tile);

    tile = GFL_BitmapWrap(&pixels[frame + 0xe0], 8, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
    for (i = 1; i < width - 1; i++) {
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, i * 8, (height - 1) * 8, 8, 8, 0xffff);
    }
    GFL_BitmapFree(tile);

    GFL_HeapFree(buf);

    if (arrow == TRUE) {
        buf = GFL_G2DIOReadBGNCGR(getUINarcIdx(), 0x20, FALSE, &chars, HEAPID_TAIL(HEAPID_BOX2_APP));
        pixels = chars->pRawData;
        tile = GFL_BitmapWrap(&pixels[0x120], 0x10, 8, 0x20, HEAPID_TAIL(HEAPID_BOX2_APP));
        GFL_BitmapCopyArea(tile, bitmap, 0, 0, (width - 3) * 8, 8, 0x10, 8, 5);
        GFL_BitmapFree(tile);
        GFL_HeapFree(buf);
    }

    str = GFL_MsgDataLoadStrbufNew(syswk->app->msgData, msgId);
    func_ov067_021b6ae4(&syswk->app->windows[index], syswk->app->printQueue, width * 8 / 2, 4, str, syswk->app->font, color, 2);
    GFL_StrBufFree(str);
}


