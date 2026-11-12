// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/bg_font.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/bg_font.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "system/printsys.h"

struct BGFont {
    BGFontSetup setup;
    Font *font;
    MsgData *msgData;
    BmpWin *window;
    BOOL visible;
};

static int func_ov217_021c1090(BGFont *bgFont, const StrBuf *str);

// pokebw2's BGFont_Create
BGFont *func_ov217_021c0ef4(const BGFontSetup *setup, Font *font, MsgData *msgData, HeapID heapId) {
    BGFont *bgFont = GFL_HeapAllocate(heapId, sizeof(BGFont), FALSE, "bg_font.c", 55);

    bgFont->setup = *setup;
    bgFont->font = font;
    bgFont->msgData = msgData;
    bgFont->window = BmpWin_CreateDynamic(setup->window.bg, setup->window.x, setup->window.y, setup->window.width,
                                          setup->window.height, setup->window.palette, FALSE);
    bgFont->visible = TRUE;
    return bgFont;
}

// pokebw2's BGFont_Delete
void func_ov217_021c0f48(BGFont *bgFont) {
    BmpWin_Free(bgFont->window);
    GFL_HeapFree(bgFont);
}

// pokebw2's BGFont_PrintMsg
void func_ov217_021c0f5c(BGFont *bgFont, u32 strId) {
    StrBuf *str = GFL_MsgDataLoadStrbufNew(bgFont->msgData, strId);

    func_ov217_021c0f78(bgFont, str);
    GFL_HeapFree(str);
}

// pokebw2's BGFont_PrintStr
void func_ov217_021c0f78(BGFont *bgFont, const StrBuf *str) {
    u16 color = ((bgFont->setup.window.letterColor & 0x1f) << 10) | ((bgFont->setup.window.shadowColor & 0x1f) << 5) |
                (bgFont->setup.window.backColor & 0x1f);
    GFLBitmap *bitmap = BmpWin_GetBitmap(bgFont->window);
    int x;
    int y;

    GFL_BitmapFill(bitmap, bgFont->setup.window.backColor);
    if (bgFont->setup.centered) {
        x = bgFont->setup.window.textX + func_ov217_021c1090(bgFont, str);
        y = bgFont->setup.window.textY;
    } else {
        x = bgFont->setup.window.textX;
        y = bgFont->setup.window.textY;
    }
    GFL_TextRendererDrawToBitmapEx(bitmap, x, y, str, bgFont->font, color);
    if (bgFont->visible) {
        BmpWin_Transfer(bgFont->window);
    }
}

// pokebw2's BGFont_SetVisible
void func_ov217_021c0ff8(BGFont *bgFont, BOOL visible) {
    if (!bgFont->visible && visible) {
        BmpWin_Transfer(bgFont->window);
    } else if (bgFont->visible && !visible) {
        BmpWin *window = bgFont->window;

        func_020456f8(window);
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
    }
    bgFont->visible = visible;
}

// pokebw2's BGFont_SetPalette
void func_ov217_021c1038(BGFont *bgFont, u8 palette) {
    func_02045798(bgFont->window, palette);
    GFL_BGSysSetScrPaletteNo(BmpWin_GetBGIndex(bgFont->window), BmpWin_GetPosX(bgFont->window),
                             BmpWin_GetPosY(bgFont->window), BmpWin_GetWidth1(bgFont->window),
                             BmpWin_GetHeight2(bgFont->window), palette);
    if (bgFont->visible) {
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(bgFont->window));
    }
}

// pokebw2's BGFont_GetCenteredX
static int func_ov217_021c1090(BGFont *bgFont, const StrBuf *str) {
    int width = func_02045738(bgFont->window) * 8;
    int x = (width - (int)GFL_FontGetBlockWidth(str, bgFont->font, 0)) * 0.5f;

    if (x < 0) {
        x = 0;
    }
    return x;
}
