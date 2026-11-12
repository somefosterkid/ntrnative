// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/app_taskmenu.h, the lines in use so
// far, with Black's names: pokebw2's AppTaskMenu_ and AppTaskMenuRes_ names are its own guesses.
#ifndef BWPORT_SYSTEM_APP_TASKMENU_H
#define BWPORT_SYSTEM_APP_TASKMENU_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/printsys.h"

// The menus' task menus: a column of buttons, one window each, picked with the keys or the touch screen

// An item of a task menu. type 0 is a plain button
typedef struct {
    StrBuf *str;
    u16 color;
    u32 type;
} AppTaskMenuItem;

// Where a task menu's x and y put it: its top left corner, or its bottom right
#define APP_TASKMENU_POS_TOP_LEFT 0
#define APP_TASKMENU_POS_BOTTOM_RIGHT 1

typedef struct {
    u32 heapId;
    u8 itemCount;
    AppTaskMenuItem *items;
    u32 posType;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} AppTaskMenuInit;

AppTaskMenu *func_02027778(AppTaskMenuInit *init, AppTaskMenuRes *res); // pokebw2's AppTaskMenu_Create
// pokebw2's AppTaskMenu_CreateFastFlash, a menu whose picked item flashes twice as fast
AppTaskMenu *func_0202784c(AppTaskMenuInit *init, AppTaskMenuRes *res);
// pokebw2's AppTaskMenu_Free, AppTaskMenu_Update, AppTaskMenu_IsFlashFinished and AppTaskMenu_GetCursorPos
void func_02027858(AppTaskMenu *menu);
void func_02027974(AppTaskMenu *menu);
BOOL func_020279e8(AppTaskMenu *menu);
u8 func_02027a04(AppTaskMenu *menu);
// pokebw2's AppTaskMenuRes_Create and AppTaskMenuRes_Free: what the buttons share
AppTaskMenuRes *func_02027f60(u8 bg, u8 palette, Font *font, PrintQueue *queue, HeapID heapId);
void func_02027fd8(AppTaskMenuRes *res);

// pokebw2's AppTaskMenuWin_Update
void func_02028178(AppTaskMenuWin *win);
BOOL func_0202846c(AppTaskMenuWin *win);
// pokebw2's AppTaskMenuWin_CreateEx
// A single button with the text centered if `centered`; fastFlash makes it flash twice as fast
AppTaskMenuWin *func_0202800c(AppTaskMenuRes *res, const AppTaskMenuItem *item, u8 x, u8 y, u8 width,
                                        u8 height, u32 fastFlash, BOOL centered, HeapID heapId);
// pokebw2's AppTaskMenuWin_SetActive
void func_02028218(AppTaskMenuWin *win, BOOL active);
// pokebw2's AppTaskMenuWin_IsFlashFinished
BOOL func_02028230(AppTaskMenuWin *win);

#define APP_TASKMENU_ITEM_RETURN 1

// pokebw2's AppTaskMenuWin_Create
// A single button at x and y, width tiles wide and 3 tall
AppTaskMenuWin *func_02027fec(AppTaskMenuRes *res, const AppTaskMenuItem *item, u8 x, u8 y, u8 width,
                                      HeapID heapId);

// pokebw2's AppTaskMenuWin_ClearScreen
void func_0202843c(AppTaskMenuWin *win);
// pokebw2's AppTaskMenuWin_CreateExNoChar
// AppTaskMenuWin_CreateEx that only puts the window on the screen, its characters loaded later
AppTaskMenuWin *func_0202804c(AppTaskMenuRes *res, const AppTaskMenuItem *item, u8 x, u8 y, u8 width,
                                              u8 height, u32 fastFlash, BOOL centered, HeapID heapId);
// pokebw2's AppTaskMenuWin_Create
// A single button at x and y, width tiles wide and 3 tall

// pokebw2's AppTaskMenuWin_ClearScreen
// pokebw2's AppTaskMenuWin_Create
// A single button at x and y, width tiles wide and 3 tall

// pokebw2's AppTaskMenu_SetCursorPos
void func_02027cfc(AppTaskMenu *menu, u32 pos);

// pokebw2's AppTaskMenuWin_ResetFlash
void func_020282a4(AppTaskMenuWin *win);
// pokebw2's AppTaskMenuWin_Create
// A single button at x and y, width tiles wide and 3 tall

// pokebw2's AppTaskMenuWin_IsTouched
// Whether the button was touched this frame
BOOL func_0202824c(AppTaskMenuWin *win);
// pokebw2's AppTaskMenu_UpdateText
// Shows the windows once their text is printed. AppTaskMenu_Update calls it
void func_020278f4(AppTaskMenu *menu);

// pokebw2's AppTaskMenu_SetLocked
// While locked, the keys do nothing. Locking it hides the cursor
void func_02027cd8(AppTaskMenu *menu, BOOL locked);

// pokebw2's AppTaskMenu_SetCursorActive
// Shows the cursor's button as the cursor's, or as the others
void func_02027a08(AppTaskMenu *menu, BOOL active);

// Black 2's, not checked against Black
// The most items a task menu can have
#define APP_TASKMENU_ITEM_MAX 8

// pokebw2's AppTaskMenuWin_SetPalette
// Moves the buttons of res to another pair of palettes, and sets the colors the button pulses between
void func_020282ac(AppTaskMenuWin *win, AppTaskMenuRes *res, u16 palette, u16 color1, u16 color2);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's AppTaskMenuWin_Free
// Frees the button and clears it from the screen, or leaves it there
void func_02028148(AppTaskMenuWin *win);
// pokebw2's AppTaskMenuWin_SetFlashing
void func_0202822c(AppTaskMenuWin *win, u32 isFlashing);
// pokebw2's AppTaskMenuWin_FreeKeepScreen
void func_0202815c(AppTaskMenuWin *win);
// pokebw2's AppTaskMenuWin_FlushMap
void func_02028454(AppTaskMenuWin *win);

// pokebw2's AppTaskMenu_IsDecided
BOOL func_02027a20(AppTaskMenu *menu);

#endif // BWPORT_SYSTEM_APP_TASKMENU_H
