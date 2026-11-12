// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov255/box2_bmp.c, ported by tools/port_file.py.
#include "types.h"
#include "app/box2.h"
#include "app/box2_bmp.h"
#include "app/box2_main.h"
#include "app/box2_obj.h"
#include "app/ui/print_msg.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/text_banks.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "pml/item.h"
#include "save/box.h"
#include "system/app_menu_common.h"
#include "system/bgwinfrm.h"
#include "system/bmp_oam.h"
#include "system/bmp_winframe.h"
#include "system/printsys.h"
#include "system/wordset.h"

// The PC box's windows: the Pokémon's data on the upper screen, the buttons of the frames, the box names and counts on
// OAM, and the messages. The ROM doesn't name this file; box2_bmp.c is a guess after box2_main.c. None of these
// functions has a name yet

#define BOX2_WIN_MAX 28

// The colors of the text
#define BOX2_COLOR_NORMAL PRINT_COLOR(1, 2, 0)
#define BOX2_COLOR_MALE PRINT_COLOR(5, 6, 0)
#define BOX2_COLOR_FEMALE PRINT_COLOR(3, 4, 0)
#define BOX2_COLOR_FULL PRINT_COLOR(5, 4, 0)
#define BOX2_COLOR_BUTTON PRINT_COLOR(15, 12, 0)
#define BOX2_COLOR_MSG PRINT_COLOR(1, 2, 15)

// Makes a frame of a window's size on its BG, which shows the window

void func_ov170_021eb378(Box2SysWork *syswk, u32 index, u32 msgId, u16 frame, u16 color, u32 arrow);
void func_ov170_021eb6d8(Box2SysWork *syswk, u32 index, u32 msgId, u32 arrow) {
    func_ov170_021eb378(syswk, index, msgId, 0, BOX2_COLOR_BUTTON, arrow);
}

void func_ov170_021eb6f0(Box2SysWork *syswk, u32 index, u32 msgId) {
    func_ov170_021eb378(syswk, index, msgId, 0, BOX2_COLOR_BUTTON, FALSE);
}


static void func_ov170_021eb708(BGWinFrame *frames, u32 index, BmpWin *window) {
    func_0202ca3c(frames, index, BmpWin_GetBGIndex(window), func_02045738(window), func_02045740(window));
    func_0202cebc(frames, index, window);
}

void func_ov170_021eb744(Box2AppWork *app) {
    func_ov170_021eb708(app->bgWinFrame, 0, app->windows[14].window);
    func_ov170_021eb708(app->bgWinFrame, 1, app->windows[15].window);
    func_ov170_021eb708(app->bgWinFrame, 2, app->windows[16].window);
    func_ov170_021eb708(app->bgWinFrame, 3, app->windows[17].window);
    func_ov170_021eb708(app->bgWinFrame, 4, app->windows[18].window);
    func_ov170_021eb708(app->bgWinFrame, 5, app->windows[19].window);
}

void func_ov170_021eb79c(Box2SysWork *syswk) {
    if (syswk->param->mode == 4) {
        func_ov170_021eb6f0(syswk, 0x16, 0x47);
    } else {
        func_ov170_021eb6f0(syswk, 0x16, 0x45);
    }
    func_ov170_021eb708(syswk->app->bgWinFrame, 10, syswk->app->windows[22].window);
}



void func_ov170_021eb7c8(Box2SysWork *syswk) {
    func_ov170_021eb6f0(syswk, 0x17, 0x46);
    func_ov170_021eb708(syswk->app->bgWinFrame, 11, syswk->app->windows[23].window);
}

void func_ov170_021eb7e8(Box2SysWork *syswk, u32 box, u32 index) {
    StrBuf *str = GFL_StrBufCreate(20, HEAPID_TAIL(HEAPID_BOX2_APP));
    int x;

    loadBoxNameToStrbuf(syswk->param->boxes, box, str);
    GFL_BitmapFill(syswk->app->fontOam[index].bitmap, 0);
    x = (96 - GFL_FontGetBlockWidth(str, syswk->app->font, 0)) >> 1;
    GFL_TextRendererDrawToBitmapEx(syswk->app->fontOam[index].bitmap, x, 0, str, syswk->app->font,
                                   BOX2_COLOR_NORMAL);
    GFL_StrBufFree(str);
    func_020257d0(syswk->app->fontOam[index].oam);
}


void func_ov170_021eb7e8(Box2SysWork *syswk, u32 box, u32 index);
void func_ov170_021eb85c(Box2SysWork *syswk, u32 b) {
    u32 offset;

    offset = b + syswk->trayScroll;
    if (offset >= syswk->trayMax) {
        offset -= syswk->trayMax;
    }
    func_ov170_021eb7e8(syswk, offset, 0);
}

void func_ov170_021eb874(Box2SysWork *syswk, u32 box, u32 index) {
    StrBuf *src = GFL_MsgDataLoadStrbufNew(syswk->app->msgData, 30);
    u32 count = howManyPokesInGeneralAreInBox(syswk->param->boxes, box);

    WordSetNumber(syswk->app->wordSet, 0, count, 2, 0, TRUE);
    GFL_WordSetFormatStrbuf(syswk->app->wordSet, syswk->app->expandBuf, src);
    GFL_BitmapFill(syswk->app->fontOam[index].bitmap, 0);
    if (count == BOX2_TRAY_POKE_MAX) {
        GFL_TextRendererDrawToBitmapEx(syswk->app->fontOam[index].bitmap, 0, 0, syswk->app->expandBuf,
                                       syswk->app->smallFont, BOX2_COLOR_FULL);
    } else {
        GFL_TextRendererDrawToBitmapEx(syswk->app->fontOam[index].bitmap, 0, 0, syswk->app->expandBuf,
                                       syswk->app->smallFont, BOX2_COLOR_NORMAL);
    }
    GFL_StrBufFree(src);
    func_020257d0(syswk->app->fontOam[index].oam);
}


void func_ov170_021eb90c(Box2SysWork *syswk) {
    func_ov170_021eb6d8(syswk, 0x14, 0x54, 0);
    func_ov170_021eb6d8(syswk, 0x15, 0x55, 1);
    func_0202ce50(syswk->app->bgWinFrame, 7, 0, 9, 0xb, 3, 0xc);
    func_0202ce50(syswk->app->bgWinFrame, 7, 0, 0xc, 0xb, 3, 0xc);
}

void func_ov170_021eb95c(Box2SysWork *syswk) {
    func_0202cebc(syswk->app->bgWinFrame, 7, syswk->app->windows[20].window);
    func_0202cebc(syswk->app->bgWinFrame, 7, syswk->app->windows[21].window);
    func_ov170_021eb90c(syswk);
}

void func_ov170_021eaae4(Box2AppWork *app, u32 index);
void func_ov170_021eb988(Box2SysWork *syswk, const u16 *data, u32 count) {
    u16 x;
    u16 y;
    u32 i;

    func_0202cfc0(syswk->app->bgWinFrame, 0, &x, &y);
    for (i = 0; i < count; i++) {
        if (data[(count - 1 - i) * 2 + 1] == 0) {
            func_ov170_021eb6d8(syswk, 0x13 - i, data[(count - 1 - i) * 2], 0);
        } else {
            func_ov170_021eb6d8(syswk, 0x13 - i, data[(count - 1 - i) * 2], 1);
        }
        func_0202ce50(syswk->app->bgWinFrame, 5 - i, 0, 0, x, y, 0xc);
    }
    for (; count < 6; count++) {
        GFL_BitmapFill(BmpWin_GetBitmap(syswk->app->windows[0x13 - count].window), 0);
        func_ov170_021eaae4(syswk->app, 0x13 - count);
    }
}

void func_ov170_021eba30(Box2SysWork *syswk, BmpWin *window) {
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0xf);
    BmpWin_DrawFrame(window, 2, syswk->app->cursorChars, 10);
}

void func_ov170_021eba5c(Box2AppWork *app, u32 index) {
    func_0201f7a8(app->windows[index].window, 1);
}

void func_ov170_021eba70(Box2AppWork *app, u32 index) {
    func_0201f7a8(app->windows[index].window, 0);
}

MsgData *func_ov170_021eac98(void);
void func_ov170_021eac28(Box2AppWork *app, u32 index, MsgData *msgData, u32 msgId, int x, int y, Font *font, u16 color);
void func_ov170_021eaba0(PrintWindow *window);
void func_ov170_021ee0fc(Box2SysWork *syswk);
void func_ov170_021eba84(Box2SysWork *syswk, u32 msgId, u32 index) {
    MsgData *msgData = func_ov170_021eac98();

    func_ov170_021eba30(syswk, syswk->app->windows[index].window);
    func_ov170_021eac28(syswk->app, index, msgData, msgId, 0, 0, syswk->app->font, BOX2_COLOR_MSG);
    func_ov170_021eaba0(&syswk->app->windows[index]);
    GFL_MsgDataFree(msgData);
    func_ov170_021ee0fc(syswk);
}

void func_ov170_021ebadc(Box2SysWork *syswk, u32 index) {
    if (syswk->param->mode == BOX2_MODE_DREAM_WORLD) {
        func_ov170_021eba84(syswk, 0x34, index);
    } else {
        func_ov170_021eba84(syswk, 0xb, index);
    }
}

void func_ov170_021ebaf8(Box2SysWork *syswk, u32 itemId, u32 a) {
    loadItemNameToStrbuf(syswk->app->wordSet, 0, itemId);
    func_ov170_021eba84(syswk, 0x15, a);
}

void func_ov170_021ebb1c(Box2SysWork *syswk, u32 index) {
    func_ov170_021eba84(syswk, 0x16, index);
}

void func_ov170_021ebb28(Box2SysWork *syswk, u32 index) {
    func_ov170_021eba84(syswk, 0x2b, index);
}

void func_ov170_021ebb34(Box2SysWork *syswk, u32 item, u32 index) {
    loadItemNameToStrbuf(syswk->app->wordSet, 0, item);
    func_ov170_021eba84(syswk, 0x0e, index);
}

void func_ov170_021ebb58(Box2SysWork *syswk, u32 index) {
    func_ov170_021eba84(syswk, 0xd, index);
}

void func_ov170_021ebb64(Box2SysWork *syswk, u32 item, u32 index) {
    if (item == ITEM_NONE) {
        loadItemNameToStrbuf(syswk->app->wordSet, 0, ITEM_GRISEOUS_ORB);
        func_ov170_021eba84(syswk, 0x2c, index);
    } else if (PML_ItemIsMail(item) == TRUE) {
        func_ov170_021eba84(syswk, 0x30, index);
    } else {
        loadItemNameToStrbuf(syswk->app->wordSet, 0, item);
        func_ov170_021eba84(syswk, 0xf, index);
    }
}

void func_ov170_021ebbbc(Box2SysWork *syswk, u32 index) {
    func_ov170_021eba84(syswk, 0x20, index);
}

void func_ov170_021ebbc8(Box2SysWork *syswk, u32 type, u32 index) {
    u32 msgId;

    switch (type) {
    case 0:
        msgId = 2;
        break;
    case 1:
        msgId = 3;
        loadBoxPokemonNameToStrbuf(syswk->app->wordSet, 0, func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos));
        break;
    case 2:
        msgId = 4;
        loadBoxPokemonNameToStrbuf(syswk->app->wordSet, 0, func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos));
        break;
    case 3:
        msgId = 28;
        break;
    case 4:
        msgId = 29;
        loadBoxPokemonNameToStrbuf(syswk->app->wordSet, 0, func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos));
        break;
    case 5:
        msgId = 30;
        break;
    case 6:
        msgId = 6;
        break;
    }
    func_ov170_021eba84(syswk, msgId, index);
}

void func_ov170_021ebc28(Box2SysWork *syswk, u32 type, u32 index) {
    u32 msgId;

    switch (type) {
    case 0:
        msgId = 7;
        break;
    case 1:
        msgId = 8;
        break;
    case 2:
        msgId = 9;
        break;
    case 3:
        msgId = 10;
        break;
    }
    func_ov170_021eba84(syswk, msgId, index);
}

void func_ov170_021ebc58(Box2SysWork *syswk, u32 a, u32 type, u32 index) {
    u32 msgId;

    switch (type) {
    case 0:
        msgId = 0x10;
        break;
    case 1:
        msgId = 0;
        loadBoxPokemonNameToStrbuf(syswk->app->wordSet, 0, func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos));
        break;
    case 2:
        msgId = 5;
        break;
    case 3:
        msgId = 0x11;
        break;
    case 4:
        msgId = 0xc;
        break;
    case 5:
        msgId = 0x1b;
        break;
    }
    func_ov170_021eba84(syswk, msgId, index);
}

void func_ov170_021ebcb4(Box2SysWork *syswk, u32 type, u32 index) {
    u32 msgId;

    switch (type) {
    case 0:
        msgId = 0x1a;
        break;
    case 1:
        msgId = 0x17;
        loadItemNameToStrbuf(syswk->app->wordSet, 0, syswk->app->itemId);
        break;
    case 2:
        msgId = 0x18;
        loadItemNameToStrbuf(syswk->app->wordSet, 0, syswk->app->itemId);
        break;
    case 3:
        msgId = 0x19;
        loadItemNameToStrbuf(syswk->app->wordSet, 0, syswk->app->itemId);
        break;
    case 4:
        msgId = 0x16;
        break;
    case 5:
        msgId = 0x1f;
        break;
    case 6:
        msgId = 0xd;
        break;
    case 7:
        msgId = 0x2f;
        break;
    }
    func_ov170_021eba84(syswk, msgId, index);
}

void func_ov170_021ebd1c(Box2SysWork *syswk, u32 type, u32 index) {
    u32 msgId;

    switch (type) {
    case 2:
        msgId = 0x2d;
        break;
    case 3:
        msgId = 0x2e;
        break;
    case 4:
        msgId = 0x31;
        break;
    case 6:
        msgId = 0x33;
        break;
    }
    func_ov170_021eba84(syswk, msgId, index);
}

void func_ov170_021ebd50(Box2SysWork *syswk) {
    loadBoxPokemonNameToStrbuf(syswk->app->wordSet, 0, func_ov170_021dd2d0(syswk, syswk->tray, syswk->pos));
    func_ov170_021eba84(syswk, 0x32, 0x1b);
}

void func_ov170_021ebd78(Box2SysWork *syswk, u32 a) {
    if (a == 0) {
        func_ov170_021eba84(syswk, 0x35, 0x1b);
    } else {
        func_ov170_021eba84(syswk, 0x36, 0x1b);
    }
}
