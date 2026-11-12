// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/app_taskmenu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/sound.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nnsys/g2d.h"
#include "nnsys/gfd.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/printsys.h"

// The menus' task menus: a column of buttons, one window each, picked with the keys or the touch screen, and single
// buttons of the same look

#define DEFAULT_WIDTH 13
#define DEFAULT_HEIGHT 3

// BGs 0 to 3 are the main screen's
#define MAIN_BG_LAST 3

// The frame's characters and palettes in the menus' common archive
#define FRAME_CHAR_FILE 0x20
#define FRAME_PLTT_FILE 0x1f

// The frame's tiles: the corners and edges, then three tiles for the icon of each item type from 1
#define FRAME_TILE_SIZE 0x20
#define FRAME_TILE_TOP_LEFT 0
#define FRAME_TILE_TOP 1
#define FRAME_TILE_TOP_RIGHT 2
#define FRAME_TILE_LEFT 3
#define FRAME_TILE_CENTER 4
#define FRAME_TILE_RIGHT 5
#define FRAME_TILE_BOTTOM_LEFT 6
#define FRAME_TILE_BOTTOM 7
#define FRAME_TILE_BOTTOM_RIGHT 8
#define FRAME_TILE_ICON(type) (((type) - 1) * 3 + 9)

// The color of a button's palette that pulses
#define PULSE_COLOR_OFFSET (6 * sizeof(GXRgb))

// Where a button's text starts
#define TEXT_X 10
#define TEXT_Y 6

// Black 2's; twinned code confirms the offsets of bg, palette, character, font and queue in Black
struct AppTaskMenuRes {
    u16 bg;
    u16 palette;
    NNSG2dCharacterData *character;
    void *characterFile;
    u32 unkC;
    Font *font;
    PrintQueue *queue;
};

struct AppTaskMenu {
    u8 cursorPos;
    u8 flashCount;
    u16 pulseAngle;
    GXRgb pulseColor;
    BOOL isUpdatingText;
    BOOL isDecided;
    BOOL isLocked;
    u32 flashSpeed;
    BmpWin **windows;
    AppTaskMenuItem *items;
    AppTaskMenuInit init;
    AppTaskMenuRes *res;
};

// Black 2's; twinned code confirms the offsets of window, isFlashing, isUpdatingText, flashCount, flashSpeed,
// pulseAngle, pulseColor, res, color1 and color2 in Black
struct AppTaskMenuWin {
    BmpWin *window;
    u32 unk4;
    u32 isFlashing;
    BOOL isUpdatingText;
    u16 flashCount;
    u16 flashSpeed;
    u16 pulseAngle;
    GXRgb pulseColor;
    AppTaskMenuRes *res;
    GXRgb color1;
    GXRgb color2;
};

static void func_02027a24(AppTaskMenu *menu, AppTaskMenuRes *res);
static void func_02027b30(BmpWin *window, AppTaskMenuRes *res, BOOL active);
static void func_02027b60(u16 *angle, GXRgb *color, u8 bg, u8 palette);
static void func_02027bf4(u16 *angle, GXRgb *color, u8 bg, u8 palette, GXRgb color1, GXRgb color2);
static void func_02027d44(AppTaskMenu *menu);
void func_02027e3c(AppTaskMenu *menu);
AppTaskMenuWin *func_02028084(AppTaskMenuRes *res, const AppTaskMenuItem *item, u8 x, u8 y, u8 width, u8 height, u32 fastFlash, BOOL centered, HeapID heapId);
static void func_02028164(AppTaskMenuWin *win);
static void func_020282d8(BmpWin *window, AppTaskMenuRes *res, u32 type);

// pokebw2's AppTaskMenu_Create
AppTaskMenu *func_02027778(AppTaskMenuInit *init, AppTaskMenuRes *res) {
    AppTaskMenu *menu = GFL_HeapAllocate(init->heapId, sizeof(AppTaskMenu), FALSE, "app_taskmenu.c", 162);

    menu->init = *init;
    menu->res = res;
    menu->windows =
        GFL_HeapAllocate(menu->init.heapId, menu->init.itemCount * sizeof(BmpWin *), FALSE, "app_taskmenu.c", 168);
    menu->items = GFL_HeapAllocate(menu->init.heapId, menu->init.itemCount * sizeof(AppTaskMenuItem), FALSE,
                                   "app_taskmenu.c", 169);
    MIi_CpuCopy16(menu->init.items, menu->items, menu->init.itemCount * sizeof(AppTaskMenuItem));
    menu->isDecided = FALSE;
    menu->isLocked = FALSE;
    menu->cursorPos = 0;
    menu->flashCount = 0;
    menu->pulseAngle = 0;
    menu->flashSpeed = 1;
    menu->init.width = init->width == 0 ? DEFAULT_WIDTH : init->width;
    menu->init.height = init->height == 0 ? DEFAULT_HEIGHT : init->height;

    func_02027a24(menu, res);
    if (func_020355c4() == FALSE) {
        func_02027b30(menu->windows[menu->cursorPos], menu->res, TRUE);
    }
    menu->isUpdatingText = TRUE;
    return menu;
}

// pokebw2's AppTaskMenu_CreateFastFlash
AppTaskMenu *func_0202784c(AppTaskMenuInit *init, AppTaskMenuRes *res) {
    AppTaskMenu *menu = func_02027778(init, res);
    menu->flashSpeed = 2;
    return menu;
}

// pokebw2's AppTaskMenu_Free
void func_02027858(AppTaskMenu *menu) {
    u8 bg = BmpWin_GetBGIndex(menu->windows[0]);
    u8 i;

    for (i = 0; i < menu->init.itemCount; i++) {
        while (func_0201c6c4(menu->res->queue, BmpWin_GetBitmap(menu->windows[i]))) {
            func_0201c4e4(menu->res->queue);
        }
    }
    for (i = 0; i < menu->init.itemCount; i++) {
        func_020456f8(menu->windows[i]);
        BmpWin_Free(menu->windows[i]);
    }
    GFL_BGSysQueueScrLoad(bg);
    GFL_HeapFree(menu->windows);
    GFL_HeapFree(menu->items);
    GFL_HeapFree(menu);
}

// pokebw2's AppTaskMenu_UpdateText
void func_020278f4(AppTaskMenu *menu) {
    if (menu->isUpdatingText == TRUE) {
        BOOL printed = TRUE;
        u8 i;

        for (i = 0; i < menu->init.itemCount; i++) {
            if (func_0201c6c4(menu->res->queue, BmpWin_GetBitmap(menu->windows[i])) == TRUE) {
                printed = FALSE;
            }
        }
        if (printed == TRUE) {
            for (i = 0; i < menu->init.itemCount; i++) {
                BmpWin_Transfer(menu->windows[i]);
            }
            menu->isUpdatingText = FALSE;
        }
    }
}

// pokebw2's AppTaskMenu_Update
void func_02027974(AppTaskMenu *menu) {
    func_020278f4(menu);
    if (menu->isDecided == FALSE) {
        func_02027d44(menu);
        if (menu->isDecided == FALSE) {
            func_02027e3c(menu);
        }
    } else {
        if ((u8)(menu->flashCount / (4 / menu->flashSpeed) % 2) == 0) {
            func_02027b30(menu->windows[menu->cursorPos], menu->res, TRUE);
        } else {
            func_02027b30(menu->windows[menu->cursorPos], menu->res, FALSE);
        }
        menu->flashCount++;
    }
    func_02027b60(&menu->pulseAngle, &menu->pulseColor, menu->res->bg, menu->res->palette);
}

// pokebw2's AppTaskMenu_IsFlashFinished
BOOL func_020279e8(AppTaskMenu *menu) {
    if (menu->flashCount >= 16 / menu->flashSpeed) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's AppTaskMenu_GetCursorPos
u8 func_02027a04(AppTaskMenu *menu) {
    return menu->cursorPos;
}

// pokebw2's AppTaskMenu_SetCursorActive
void func_02027a08(AppTaskMenu *menu, BOOL active) {
    func_02027b30(menu->windows[menu->cursorPos], menu->res, active);
}

// pokebw2's AppTaskMenu_IsDecided
BOOL func_02027a20(AppTaskMenu *menu) {
    return menu->isDecided;
}

// pokebw2's AppTaskMenu_CreateWindows
static void func_02027a24(AppTaskMenu *menu, AppTaskMenuRes *res) {
    u8 i;
    u8 x;
    u8 y;

    if (menu->init.posType == APP_TASKMENU_POS_TOP_LEFT) {
        x = menu->init.x;
        y = menu->init.y;
    } else {
        x = menu->init.x - menu->init.width;
        y = menu->init.y - menu->init.height * menu->init.itemCount;
    }

    for (i = 0; i < menu->init.itemCount; i++) {
        menu->windows[i] = BmpWin_CreateDynamic(menu->res->bg, x, y + i * menu->init.height, menu->init.width,
                                                menu->init.height, menu->res->palette + 1, TRUE);
        func_020282d8(menu->windows[i], res, menu->items[i].type);
        func_0201c724(res->queue, BmpWin_GetBitmap(menu->windows[i]), TEXT_X, TEXT_Y, menu->items[i].str, res->font,
                      menu->items[i].color);
        BmpWin_Transfer(menu->windows[i]);
    }
}

// pokebw2's AppTaskMenu_SetWindowActive
static void func_02027b30(BmpWin *window, AppTaskMenuRes *res, BOOL active) {
    if (active == TRUE) {
        func_02045798(window, res->palette);
    } else if (active == FALSE) {
        func_02045798(window, res->palette + 1);
    }
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
}

// pokebw2's AppTaskMenu_PulseCursorColor
static void func_02027b60(u16 *angle, GXRgb *color, u8 bg, u8 palette) {
    fx16 t;
    u8 r;
    u8 g;
    u8 b;

    if (*angle + 0x400 >= 0x10000) {
        *angle = *angle - 0x10000 + 0x400;
    } else {
        *angle += 0x400;
    }

    t = (FX_CosIdx(*angle) + FX16_ONE) / 2;
    r = 5 + (t * 7 >> FX32_SHIFT);
    g = 10 + (t * 15 >> FX32_SHIFT);
    b = 13 + (t * 17 >> FX32_SHIFT);
    *color = GX_RGB(r, g, b);

    if (bg <= MAIN_BG_LAST) {
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, palette * 0x20 + PULSE_COLOR_OFFSET, color,
                                           sizeof(GXRgb));
    } else {
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, palette * 0x20 + PULSE_COLOR_OFFSET, color,
                                           sizeof(GXRgb));
    }
}

// pokebw2's AppTaskMenuWin_PulseColor
static void func_02027bf4(u16 *angle, GXRgb *color, u8 bg, u8 palette, GXRgb color1, GXRgb color2) {
    fx16 t;
    u8 r1 = (color1 & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT;
    u8 g1 = (color1 & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
    u8 b1 = (color1 & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
    u8 r2 = (color2 & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT;
    u8 g2 = (color2 & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
    u8 b2 = (color2 & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
    u8 r;
    u8 g;
    u8 b;

    if (*angle + 0x400 >= 0x10000) {
        *angle = *angle - 0x10000 + 0x400;
    } else {
        *angle += 0x400;
    }

    t = (FX_CosIdx(*angle) + FX16_ONE) / 2;
    r = r1 + ((r2 - r1) * t >> FX32_SHIFT);
    g = g1 + ((g2 - g1) * t >> FX32_SHIFT);
    b = b1 + ((b2 - b1) * t >> FX32_SHIFT);
    *color = GX_RGB(r, g, b);

    if (bg <= MAIN_BG_LAST) {
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, palette * 0x20 + PULSE_COLOR_OFFSET, color,
                                           sizeof(GXRgb));
    } else {
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, palette * 0x20 + PULSE_COLOR_OFFSET, color,
                                           sizeof(GXRgb));
    }
}

// pokebw2's AppTaskMenu_SetLocked
void func_02027cd8(AppTaskMenu *menu, BOOL locked) {
    menu->isLocked = locked;
    if (locked == TRUE) {
        func_020355d8(TRUE);
        func_02027b30(menu->windows[menu->cursorPos], menu->res, FALSE);
    }
}

// pokebw2's AppTaskMenu_SetCursorPos
void func_02027cfc(AppTaskMenu *menu, u32 pos) {
    int i;

    menu->cursorPos = pos;
    if (func_020355c4() == FALSE) {
        for (i = 0; i < menu->init.itemCount; i++) {
            if (i == pos) {
                func_02027b30(menu->windows[i], menu->res, TRUE);
            } else {
                func_02027b30(menu->windows[i], menu->res, FALSE);
            }
        }
    }
}

// pokebw2's AppTaskMenu_UpdateKeys
static void func_02027d44(AppTaskMenu *menu) {
    u32 pressed = GCTX_HIDGetPressedKeys();
    u32 typed = GCTX_HIDGetTypedKeys();
    u8 oldPos;

    if (menu->isLocked == TRUE) {
        return;
    }

    if (func_020355c4() == TRUE && (pressed & PAD_BUTTON_B) == 0) {
        if (pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_KEY_UP | PAD_KEY_DOWN)) {
            func_02027b30(menu->windows[menu->cursorPos], menu->res, TRUE);
            func_020355d8(FALSE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
        }
        return;
    }

    oldPos = menu->cursorPos;
    if (typed & PAD_KEY_UP) {
        if (oldPos == 0) {
            menu->cursorPos = menu->init.itemCount - 1;
        } else {
            menu->cursorPos = oldPos - 1;
        }
        menu->pulseAngle = 0;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
    } else if (typed & PAD_KEY_DOWN) {
        if (oldPos >= menu->init.itemCount - 1) {
            menu->cursorPos = 0;
        } else {
            menu->cursorPos = oldPos + 1;
        }
        menu->pulseAngle = 0;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
    } else if (pressed & PAD_BUTTON_A) {
        menu->isDecided = TRUE;
        if (menu->items[oldPos].type == APP_TASKMENU_ITEM_RETURN) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
    } else if (pressed & PAD_BUTTON_B) {
        menu->cursorPos = menu->init.itemCount - 1;
        menu->isDecided = TRUE;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        func_020355d8(FALSE);
    }

    if (oldPos != menu->cursorPos) {
        func_02027b30(menu->windows[oldPos], menu->res, FALSE);
        func_02027b30(menu->windows[menu->cursorPos], menu->res, TRUE);
    }
}

void func_02027e3c(AppTaskMenu *menu) {
    u8 x;
    u8 y;
    u8 i;
    TouchRect rects[APP_TASKMENU_ITEM_MAX + 1];
    s32 hit;

    if (menu->init.posType == APP_TASKMENU_POS_TOP_LEFT) {
        x = menu->init.x;
        y = menu->init.y;
    } else {
        x = menu->init.x - menu->init.width;
        y = menu->init.y - menu->init.height * menu->init.itemCount;
    }

    for (i = 0; i < menu->init.itemCount; i++) {
        rects[i].top = y * 8 + i * (menu->init.height * 8);
        rects[i].bottom = y * 8 + (i + 1) * (menu->init.height * 8);
        rects[i].left = x * 8;
        rects[i].right = (x + menu->init.width) * 8 - 1;
    }
    rects[i].top = TOUCH_RECT_END;

    hit = func_02035c08(rects);
    if (hit != TOUCH_RECT_NONE) {
        func_020355d8(TRUE);
        func_02027b30(menu->windows[menu->cursorPos], menu->res, FALSE);
        menu->cursorPos = hit;
        menu->isDecided = TRUE;
        func_02027b30(menu->windows[menu->cursorPos], menu->res, TRUE);
        if (menu->cursorPos == 0) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        } else if (menu->items[menu->cursorPos].type == APP_TASKMENU_ITEM_RETURN) {
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
        } else {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
    }
}


// pokebw2's AppTaskMenuRes_Create
AppTaskMenuRes *func_02027f60(u8 bg, u8 palette, Font *font, PrintQueue *queue, HeapID heapId) {
    AppTaskMenuRes *res = GFL_HeapAllocate(heapId, sizeof(AppTaskMenuRes), FALSE, "app_taskmenu.c", 710);
    u32 palType = PALTYPE_MAIN_BG;

    MI_CpuFill8(res, 0, sizeof(AppTaskMenuRes));
    res->bg = bg;
    res->palette = palette;
    res->font = font;
    res->queue = queue;
    if (bg > MAIN_BG_LAST) {
        palType = PALTYPE_SUB_BG;
    }

    res->characterFile = GFL_G2DIOReadBGNCGR(getUINarcIdx(), FRAME_CHAR_FILE, FALSE, &res->character, heapId);
    GFL_BGSysLoadNCLRDefault(getUINarcIdx(), FRAME_PLTT_FILE, palType, palette * 0x20, 2 * 0x20, heapId);
    return res;
}

// pokebw2's AppTaskMenuRes_Free
void func_02027fd8(AppTaskMenuRes *res) {
    GFL_HeapFree(res->characterFile);
    GFL_HeapFree(res);
}

AppTaskMenuWin *func_02027fec(AppTaskMenuRes *res, const AppTaskMenuItem *item, u8 x, u8 y, u8 width,
                                      HeapID heapId) {
    return func_0202800c(res, item, x, y, width, DEFAULT_HEIGHT, FALSE, FALSE, heapId);
}

AppTaskMenuWin *func_0202800c(AppTaskMenuRes *res, const AppTaskMenuItem *item, u8 x, u8 y, u8 width,
                                        u8 height, u32 fastFlash, BOOL centered, HeapID heapId) {
    AppTaskMenuWin *win = func_02028084(res, item, x, y, width, height, fastFlash, centered, heapId);
    BmpWin_Transfer(win->window);
    return win;
}

AppTaskMenuWin *func_0202804c(AppTaskMenuRes *res, const AppTaskMenuItem *item, u8 x, u8 y, u8 width,
                                              u8 height, u32 fastFlash, BOOL centered, HeapID heapId) {
    AppTaskMenuWin *win = func_02028084(res, item, x, y, width, height, fastFlash, centered, heapId);
    BmpWin_FlushMap(win->window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win->window));
    return win;
}

AppTaskMenuWin *func_02028084(AppTaskMenuRes *res, const AppTaskMenuItem *item, u8 x, u8 y, u8 width,
                                                 u8 height, u32 fastFlash, BOOL centered, HeapID heapId) {
    u8 textX = 2;
    AppTaskMenuWin *win = GFL_HeapAllocate(heapId, sizeof(AppTaskMenuWin), FALSE, "app_taskmenu.c", 823);

    MI_CpuFill8(win, 0, sizeof(AppTaskMenuWin));
    win->res = res;
    win->isUpdatingText = TRUE;
    win->flashSpeed = fastFlash + 1;
    win->window = BmpWin_CreateDynamic(res->bg, x, y, width, height, res->palette + 1, TRUE);
    func_020282d8(win->window, res, item->type);
    if (centered == TRUE) {
        textX = (width * 8 - GFL_FontGetBlockWidth(item->str, res->font, 0) - 16) / 2;
    }
    func_0201c724(res->queue, BmpWin_GetBitmap(win->window), textX + 8, TEXT_Y, item->str, res->font, item->color);
    win->color1 = GX_RGB(5, 10, 13);
    win->color2 = GX_RGB(12, 25, 30);
    return win;
}

// pokebw2's AppTaskMenuWin_Free
void func_02028148(AppTaskMenuWin *win) {
    func_020456f8(win->window);
    func_02028164(win);
}

// pokebw2's AppTaskMenuWin_FreeKeepScreen
void func_0202815c(AppTaskMenuWin *win) {
    func_02028164(win);
}

// pokebw2's AppTaskMenuWin_FreeCore
static void func_02028164(AppTaskMenuWin *win) {
    BmpWin_Free(win->window);
    GFL_HeapFree(win);
}

// pokebw2's AppTaskMenuWin_Update
void func_02028178(AppTaskMenuWin *win) {
    if (win->isUpdatingText) {
        if (func_0201c6c4(win->res->queue, BmpWin_GetBitmap(win->window)) == FALSE) {
            BmpWin_Transfer(win->window);
            win->isUpdatingText = FALSE;
        }
    }
    if (win->isFlashing) {
        if ((u8)(win->flashCount / (4 / win->flashSpeed) % 2) == 0) {
            func_02027b30(win->window, win->res, TRUE);
        } else {
            func_02027b30(win->window, win->res, FALSE);
        }
        win->flashCount++;
    }
    func_02027bf4(&win->pulseAngle, &win->pulseColor, win->res->bg, win->res->palette, win->color1,
                              win->color2);
}

// pokebw2's AppTaskMenuWin_SetActive
void func_02028218(AppTaskMenuWin *win, BOOL active) {
    win->pulseAngle = 0;
    func_02027b30(win->window, win->res, active);
}

void func_0202822c(AppTaskMenuWin *win, u32 isFlashing)
{
    win->isFlashing = isFlashing;
}


void AppTaskMenuWin_SetFlashing(AppTaskMenuWin *win, BOOL flashing) {
    win->isFlashing = flashing;
}

BOOL AppTaskMenuWin_IsFlashing(AppTaskMenuWin *win) {
    return win->isFlashing;
}

// pokebw2's AppTaskMenuWin_IsFlashFinished
BOOL func_02028230(AppTaskMenuWin *win) {
    if (win->flashCount >= 16 / win->flashSpeed) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's AppTaskMenuWin_IsTouched
BOOL func_0202824c(AppTaskMenuWin *win) {
    u32 x;
    u32 y;

    if (func_02035d20(&x, &y)) {
        u32 left = BmpWin_GetPosX(win->window) * 8;
        u32 top = BmpWin_GetPosY(win->window) * 8;
        u32 right = left + func_02045738(win->window) * 8;
        // BUG: The bottom is found from the width, so a button wider than it is tall can be touched below it
#ifdef BUGFIX
        u32 bottom = top + func_02045740(win->window) * 8;
#else
        u32 bottom = top + func_02045738(win->window) * 8;
#endif
        // Unsigned, so a point left of or above the button is out of range too
        if (x - left <= right - left && y - top <= bottom - top) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's AppTaskMenuWin_ResetFlash
void func_020282a4(AppTaskMenuWin *win) {
    win->isFlashing = FALSE;
    win->flashCount = 0;
}

// pokebw2's AppTaskMenuWin_SetPalette
void func_020282ac(AppTaskMenuWin *win, AppTaskMenuRes *res, u16 palette, u16 color1, u16 color2) {
    res->palette = palette;
    win->color1 = color1;
    win->color2 = color2;
    func_02045798(win->window, palette + 1);
    BmpWin_FlushMap(win->window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win->window));
}

// pokebw2's AppTaskMenu_DrawFrame
static void func_020282d8(BmpWin *window, AppTaskMenuRes *res, u32 type) {
    u8 x;
    u8 y;
    u32 offset;
    u8 *pixels;
    u8 *tile;

    for (y = 0; y < func_02045740(window); y++) {
        for (x = 0; x < func_02045738(window); x++) {
            pixels = GFL_BitmapGetPixelData(BmpWin_GetBitmap(window));
            offset = (x + func_02045738(window) * y) * FRAME_TILE_SIZE;

            if (y == 0 && x == 0) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_TOP_LEFT * FRAME_TILE_SIZE;
            } else if (y == 0 && x == func_02045738(window) - 1) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_TOP_RIGHT * FRAME_TILE_SIZE;
            } else if (y == 0) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_TOP * FRAME_TILE_SIZE;
            } else if (y == func_02045740(window) - 1 && x == 0) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_BOTTOM_LEFT * FRAME_TILE_SIZE;
            } else if (y == func_02045740(window) - 1 && x == func_02045738(window) - 1) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_BOTTOM_RIGHT * FRAME_TILE_SIZE;
            } else if (y == func_02045740(window) - 1) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_BOTTOM * FRAME_TILE_SIZE;
            } else if (type != 0 && x == func_02045738(window) - 3) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_ICON(type) * FRAME_TILE_SIZE;
            } else if (type != 0 && x == func_02045738(window) - 2) {
                tile = (u8 *)res->character->pRawData + (FRAME_TILE_ICON(type) + 1) * FRAME_TILE_SIZE;
            } else if (x == 0) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_LEFT * FRAME_TILE_SIZE;
            } else if (x == func_02045738(window) - 1) {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_RIGHT * FRAME_TILE_SIZE;
            } else {
                tile = (u8 *)res->character->pRawData + FRAME_TILE_CENTER * FRAME_TILE_SIZE;
            }
            MIi_CpuCopy32(tile, pixels + offset, FRAME_TILE_SIZE);
        }
    }
}

// pokebw2's AppTaskMenuWin_ClearScreen
void func_0202843c(AppTaskMenuWin *win) {
    func_020456f8(win->window);
    GFL_BGSysQueueScrLoad(win->res->bg);
}

// pokebw2's AppTaskMenuWin_FlushMap
void func_02028454(AppTaskMenuWin *win) {
    BmpWin_FlushMap(win->window);
    GFL_BGSysQueueScrLoad(win->res->bg);
}

// pokebw2's AppTaskMenuWin_IsUpdatingText
BOOL func_0202846c(AppTaskMenuWin *win) {
    return win->isUpdatingText;
}
