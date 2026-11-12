// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/bmp_menulist.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_BMP_MENULIST_H
#define BWPORT_SYSTEM_BMP_MENULIST_H

#include "types.h"
#include "stddef.h"
#include "struct_decls.h"
#include "system/printsys.h"

// Frees the list, giving back where it was
void BmpMenuList_Free(BmpMenuList *list, u16 *listTop, u16 *cursorRow);

// Black 2's, not checked against Black
// Called as the cursor moves to an option (init is TRUE when the list is made), and to print more of an option at a
// row of the window
typedef void (*BmpMenuListCursorCallback)(BmpMenuList *list, intptr_t value, u8 init);

// Black 2's, not checked against Black
typedef void (*BmpMenuListPrintCallback)(BmpMenuList *list, intptr_t value, u8 y);

#define BMPMENULIST_CANCEL (-2)

// Black's layout (size 0x34), worked out from the assembly: Black 2's, with window added after printCallback and
// without the trailing wait
typedef struct {
    ListMenuOption *options;
    BmpMenuListCursorCallback cursorCallback;
    BmpMenuListPrintCallback printCallback;
    BmpWin *window;
    u16 count;
    // The rows the window shows
    u16 maxShown;
    u8 labelX;
    u8 itemX;
    u8 cursorX;
    // The y of the first row
    u8 y : 4;
    u8 fgColor : 4;
    u8 bgColor : 4;
    u8 shadowColor : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pageSkip : 2;
    u16 fontId : 6;
    u16 cursorDisplay : 1;
    // For the callbacks, which BmpMenuList_GetWork returns
    void *work;
    u16 fontSizeX;
    u16 fontSizeY;
    MsgData *msgData;
    PrintWindow *printWindow;
    PrintQueue *queue;
    Font *font;
} BmpMenuListHeader;

// pokebw2's BmpMenuList_Create: makes the list from a copy of the header, the cursor starting at cursorRow of the
// rows from listTop
BmpMenuList *BmpMenuList_Create(const BmpMenuListHeader *header, u32 listTop, u16 cursorRow, HeapID heapId);
// pokebw2's BmpMenuList_LoadCursor
void func_02020cf8(BmpMenuList *list, u32 heapId);
// pokebw2's BmpMenuList_SetCancelDisabled
void func_02020d10(BmpMenuList *list, u8 disabled);

// Black 2's, not checked against Black
// Called as the cursor moves to an option (init is TRUE when the list is made), and to print more of an option at a
// row of the window

// Black 2's, not checked against Black

// BmpMenuList_Update's results when nothing was chosen and when the menu was cancelled, and the value of a label: an
// option printed at labelX that the cursor skips
#define BMPMENULIST_NULL (-1)

// Black 2's, not checked against Black
// The keys that move the cursor a page
#define BMPMENULIST_SKIP_NONE 0

// Black 2's, not checked against Black
#define BMPMENULIST_SKIP_LR_KEY 1
#define BMPMENULIST_CURSOR_SHOW 0

// Black 2's, not checked against Black

// pokebw2's BmpMenuList_GetCursorIndex
// The index of the option under the cursor
void func_020202ec(BmpMenuList *list, u16 *index);

// pokebw2's BmpMenuList_GetPos
void func_020202fc(BmpMenuList *list, u16 *listTop, u16 *cursorRow);

// Black 2's, not checked against Black
// BmpMenuList_GetParam's parameters, in the order of the header's fields from the callbacks on. 4 and 18 give -1
enum {
    BMPMENULIST_PARAM_CURSOR_CALLBACK,
    BMPMENULIST_PARAM_PRINT_CALLBACK,
    BMPMENULIST_PARAM_COUNT,
    BMPMENULIST_PARAM_MAX_SHOWN,
    BMPMENULIST_PARAM_4,
    BMPMENULIST_PARAM_LABEL_X,
    BMPMENULIST_PARAM_ITEM_X,
    BMPMENULIST_PARAM_CURSOR_X,
    BMPMENULIST_PARAM_Y,
    BMPMENULIST_PARAM_ROW_HEIGHT,
    BMPMENULIST_PARAM_FG_COLOR,
    BMPMENULIST_PARAM_BG_COLOR,
    BMPMENULIST_PARAM_SHADOW_COLOR,
    BMPMENULIST_PARAM_LETTER_SPACING,
    BMPMENULIST_PARAM_LINE_SPACING,
    BMPMENULIST_PARAM_PAGE_SKIP,
    BMPMENULIST_PARAM_FONT_ID,
    BMPMENULIST_PARAM_CURSOR_DISPLAY,
    BMPMENULIST_PARAM_18,
    BMPMENULIST_PARAM_WORK,
};

// pokebw2's BmpMenuList_GetOptionValue
intptr_t func_02020344(BmpMenuList *list, u16 index);

// Black 2's, not checked against Black
#define BMPMENULIST_LABEL (-3)
#define BMPMENULIST_SKIP_LR_BUTTON 2
#define BMPMENULIST_CURSOR_HIDE 1

// pokebw2's BmpMenuList_SetOverrideColors
// Prints the options in these colors from then on
void func_02020298(BmpMenuList *list, u8 fgColor, u8 bgColor, u8 shadowColor);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Reads the keys and moves the cursor; returns the chosen option's value (a number or a pointer, as the option
// holds it), BMPMENULIST_CANCEL or BMPMENULIST_NULL
intptr_t BmpMenuList_Update(BmpMenuList *list);
// pokebw2's BmpMenuList_GetWork
void *func_02020d0c(BmpMenuList *list);
// pokebw2's BmpMenuList_GetParam
// A field of the list's header, or -1; a pointer one (the callbacks, the window, the work) as an intptr_t
intptr_t func_02020350(BmpMenuList *list, u8 param);
// pokebw2's BmpMenuList_SetColors
void func_02020268(BmpMenuList *list, u8 fgColor, u8 bgColor, u8 shadowColor);
// pokebw2's BmpMenuList_GetRowY
u16 func_02020324(BmpMenuList *list, u16 row);
// pokebw2's BmpMenuList_GetCursorY
u16 func_02020314(BmpMenuList *list);

// pokebw2's BmpMenuList_Redraw
void func_02020234(BmpMenuList *list);

#endif // BWPORT_SYSTEM_BMP_MENULIST_H
