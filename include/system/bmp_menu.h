// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/bmp_menu.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_BMP_MENU_H
#define BWPORT_SYSTEM_BMP_MENU_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "system/printsys.h"

// Menus of options laid out in columns in a window, with a cursor that moves between them (bmp_menu.c). The ROM only
// has its yes/no menu, which ShopUI_CreateConfirmDialog makes. ShopUI_CreateConfirmDialog is swan's name; the others
// are Black's dsd names, with pokebw2's guesses in the comments

// The result of a menu's update when it is cancelled
#define BMPMENU_CANCEL (-2)

typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 palette;
    u16 unk4;
} ConfirmDialogSetup;

// A yes/no menu in a window of its own, with the frame at frameChar and framePalette and the cursor on cursorPos
BmpMenu *ShopUI_CreateConfirmDialog(const ConfirmDialogSetup *setup, u16 frameChar, u8 framePalette, u8 cursorPos,
                                    HeapID heapId);
// Returns 0 for yes and BMPMENU_CANCEL for no or B, freeing the menu, and -1 until then (ConfirmDialog_Update)
u32 func_0201febc(BmpMenu *menu);
void func_0201fef4(BmpMenu *menu); // ConfirmDialog_Free

// The results of a menu's update while nothing is chosen and when it is cancelled, and the value of an option the
// cursor skips
#define BMPMENU_NULL (-1)

// The results of a menu's update while nothing is chosen and when it is cancelled, and the value of an option the
// cursor skips

// Black 2's, not checked against Black
#define BMPMENU_DUMMY (-3)

// Black 2's, but for the window, which Black's keeps here (Black's is 0x24 bytes, from bmp_menu.c's code)
typedef struct {
    ListMenuOption *options;
    // The yes/no menu's window (Black 2 has it only in the print window)
    BmpWin *window;
    // Frames before the cursor appears, which the yes/no menu shortens to 2 for 0
    u8 wait;
    u8 xCount;
    u8 yCount;
    u8 lineSpacing : 4;
    // BMPMENU_CURSOR_SHOW or BMPMENU_CURSOR_HIDE
    u8 cursorDisplay : 2;
    // Whether the cursor wraps around at the edges
    u8 loop : 2;
    u16 fontSizeX;
    u16 fontSizeY;
    u32 unkC;
    PrintWindow *printWindow;
    PrintQueue *queue;
    Font *font;
    // Set once the options are printed and the window's characters are copied
    BOOL printed;
} BmpMenuHeader;

// Black 2's, not checked against Black
#define BMPMENU_CURSOR_SHOW 0
#define BMPMENU_CURSOR_HIDE 1

// Black 2's, not checked against Black
// The directions the cursor moves in
enum {
    BMPMENU_MOVE_UP,
    BMPMENU_MOVE_DOWN,
    BMPMENU_MOVE_LEFT,
    BMPMENU_MOVE_RIGHT,
};

// Black 2's, its fields 4 bytes further on for the header's window (0x38 bytes in Black)
struct BmpMenu {
    BmpMenuHeader header;
    BmpCursor *cursor;
    // The keys that cancel the menu
    u32 cancelKeys;
    u8 unk28;
    u8 cursorPos;
    // The width of the widest option
    u8 maxWidth;
    u8 x;
    u8 y;
    u8 fontSizeX;
    u8 fontSizeY;
    // The direction the cursor last moved, plus 1, or 0 if it didn't
    u8 moveDir;
    HeapID heapId;
    // Frames until the cursor is drawn
    u8 wait;
};

#endif // BWPORT_SYSTEM_BMP_MENU_H
