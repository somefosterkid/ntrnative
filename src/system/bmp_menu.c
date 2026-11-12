// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/bmp_menu.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/ui.h"
#include "system/bmp_cursor.h"
#include "system/bmp_menu.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/printsys.h"

// Menus of options in columns, with a cursor, and the yes/no menu made from one

// The yes/no menu's messages
#define MSG_YESNO TEXT_BANK_YES_NO
#define MSG_YESNO_YES 0
#define MSG_YESNO_NO 1

static BmpMenu *func_0201f93c(const BmpMenuHeader *header, u8 x, u8 y, u8 cursorPos, HeapID heapId, u32 cancelKeys);
BmpMenu *func_0201f9d0(const BmpMenuHeader *header, u8 x, u8 y, u8 cursorPos, HeapID heapId, u32 cancelKeys);
static void func_0201f9e8(BmpMenu *menu, u8 *cursorPos);
static s32 shopDecision(BmpMenu *menu);
static BOOL func_0201fad0(BmpMenu *menu, u8 dir, u16 se);
static BOOL func_0201fb30(BmpMenu *menu, u8 dir);
static u8 func_0201fc1c(BmpMenu *menu);
static void func_0201fc5c(BmpMenu *menu);
static void func_0201fd18(BmpMenu *menu);
static void func_0201fd54(BmpMenu *menu, u8 *x, u8 *y, u8 pos);

// pokebw2's BmpMenu_AddEx
static BmpMenu *func_0201f93c(const BmpMenuHeader *header, u8 x, u8 y, u8 cursorPos, HeapID heapId, u32 cancelKeys) {
    BmpMenu *menu = GFL_HeapAllocate(heapId, sizeof(BmpMenu), TRUE, "bmp_menu.c", 97);

    menu->header = *header;
    menu->cursor = BmpCursor_Create(heapId);
    menu->cancelKeys = cancelKeys;
    menu->cursorPos = cursorPos;
    menu->maxWidth = func_0201fc1c(menu);
    menu->heapId = heapId;
    menu->x = x;
    menu->y = y;
    func_02020dc8(menu->cursor, heapId);
    menu->fontSizeX = menu->header.fontSizeX;
    menu->fontSizeY = menu->header.fontSizeY;
    func_0201fc5c(menu);
    func_0201fd18(menu);
    return menu;
}

// pokebw2's BmpMenu_Add
BmpMenu *func_0201f9d0(const BmpMenuHeader *header, u8 x, u8 y, u8 cursorPos, HeapID heapId, u32 cancelKeys) {
    return func_0201f93c(header, x, y, cursorPos, heapId, cancelKeys);
}

// pokebw2's BmpMenu_Exit
static void func_0201f9e8(BmpMenu *menu, u8 *cursorPos) {
    if (cursorPos != NULL) {
        *cursorPos = menu->cursorPos;
    }
    func_02020d3c(menu->cursor);
    GFL_HeapFree(menu);
}

static s32 shopDecision(BmpMenu *menu) {
    u32 keys = GCTX_HIDGetPressedKeys();

    menu->moveDir = 0;
    if (keys & PAD_BUTTON_A) {
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        return menu->header.options[menu->cursorPos].value;
    }
    if (keys & menu->cancelKeys) {
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return BMPMENU_CANCEL;
    }
    if (keys & PAD_KEY_UP) {
        if (func_0201fad0(menu, BMPMENU_MOVE_UP, SEQ_SE_SELECT1) == TRUE) {
            menu->moveDir = BMPMENU_MOVE_UP + 1;
        }
        return BMPMENU_NULL;
    }
    if (keys & PAD_KEY_DOWN) {
        if (func_0201fad0(menu, BMPMENU_MOVE_DOWN, SEQ_SE_SELECT1) == TRUE) {
            menu->moveDir = BMPMENU_MOVE_DOWN + 1;
        }
        return BMPMENU_NULL;
    }
    if (keys & PAD_KEY_LEFT) {
        if (func_0201fad0(menu, BMPMENU_MOVE_LEFT, SEQ_SE_SELECT1) == TRUE) {
            menu->moveDir = BMPMENU_MOVE_LEFT + 1;
        }
        return BMPMENU_NULL;
    }
    if (keys & PAD_KEY_RIGHT) {
        if (func_0201fad0(menu, BMPMENU_MOVE_RIGHT, SEQ_SE_SELECT1) == TRUE) {
            menu->moveDir = BMPMENU_MOVE_RIGHT + 1;
        }
        return BMPMENU_NULL;
    }
    return BMPMENU_NULL;
}

// Moves the cursor a step in dir, erasing it where it was and drawing it again; FALSE if it can't move
// pokebw2's BmpMenu_MoveCursor
static BOOL func_0201fad0(BmpMenu *menu, u8 dir, u16 se) {
    u8 oldPos = menu->cursorPos;
    u8 x, y;

    if (func_0201fb30(menu, dir) == FALSE) {
        return FALSE;
    }
    func_0201fd54(menu, &x, &y, oldPos);
    GFL_BitmapFillArea(BmpWin_GetBitmap(menu->header.window), x, y, menu->fontSizeX, menu->fontSizeY,
                       0xff);
    func_0201fd18(menu);
    GFL_SndSEPlay(se);
    return TRUE;
}

// Moves the cursor's position a step in dir, wrapping around if the menu loops,
// but never onto a BMPMENU_DUMMY option
// pokebw2's BmpMenu_NextCursorPos
static BOOL func_0201fb30(BmpMenu *menu, u8 dir) {
    s8 pos;

    if (dir == BMPMENU_MOVE_UP) {
        if (menu->header.yCount <= 1) {
            return FALSE;
        }
        if (menu->cursorPos % menu->header.yCount == 0) {
            if (menu->header.loop == FALSE) {
                return FALSE;
            }
            pos = menu->cursorPos + (menu->header.yCount - 1);
        } else {
            pos = menu->cursorPos - 1;
        }
    } else if (dir == BMPMENU_MOVE_DOWN) {
        if (menu->header.yCount <= 1) {
            return FALSE;
        }
        if (menu->cursorPos % menu->header.yCount == menu->header.yCount - 1) {
            if (menu->header.loop == FALSE) {
                return FALSE;
            }
            pos = menu->cursorPos - (menu->header.yCount - 1);
        } else {
            pos = menu->cursorPos + 1;
        }
    } else if (dir == BMPMENU_MOVE_LEFT) {
        if (menu->header.xCount <= 1) {
            return FALSE;
        }
        if (menu->cursorPos < menu->header.yCount) {
            if (menu->header.loop == FALSE) {
                return FALSE;
            }
            pos = menu->cursorPos + menu->header.yCount * (menu->header.xCount - 1);
        } else {
            pos = menu->cursorPos - menu->header.yCount;
        }
    } else {
        if (menu->header.xCount <= 1) {
            return FALSE;
        }
        if (menu->cursorPos >= menu->header.yCount * (menu->header.xCount - 1)) {
            if (menu->header.loop == FALSE) {
                return FALSE;
            }
            pos = menu->cursorPos % menu->header.yCount;
        } else {
            pos = menu->cursorPos + menu->header.yCount;
        }
    }
    if (menu->header.options[pos].value == BMPMENU_DUMMY) {
        return FALSE;
    }
    menu->cursorPos = pos;
    return TRUE;
}

// pokebw2's BmpMenu_GetMaxWidth
static u8 func_0201fc1c(BmpMenu *menu) {
    u8 maxWidth = 0;
    u8 i;

    for (i = 0; i < menu->header.xCount * menu->header.yCount; i++) {
        u8 width = GFL_FontGetBlockWidth(menu->header.options[i].text, menu->header.font, 0);

        if (maxWidth < width) {
            maxWidth = width;
        }
    }
    return maxWidth;
}

// pokebw2's BmpMenu_PrintOptions
static void func_0201fc5c(BmpMenu *menu) {
    u8 x, column, row, columnWidth;

    GFL_BitmapFill(BmpWin_GetBitmap(menu->header.window), 0xff);
    x = menu->x;
    columnWidth = menu->maxWidth + menu->fontSizeX * 2;
    for (column = 0; column < menu->header.xCount; column++) {
        for (row = 0; row < menu->header.yCount; row++) {
            PrintWindow *printWindow = menu->header.printWindow;
            PrintQueue *queue = menu->header.queue;
            u8 y = menu->y + (menu->fontSizeY + menu->header.lineSpacing) * row;

            func_0201c6fc(queue, BmpWin_GetBitmap(printWindow->window), x, y,
                          menu->header.options[row + column * menu->header.yCount].text, menu->header.font);
            printWindow->flushPending = TRUE;
        }
        x += columnWidth;
    }
}

// pokebw2's BmpMenu_DrawCursor
static void func_0201fd18(BmpMenu *menu) {
    u8 x, y;

    if (menu->header.cursorDisplay != BMPMENU_CURSOR_HIDE) {
        func_0201fd54(menu, &x, &y, menu->cursorPos);
        func_02020d5c(menu->cursor, x, y, menu->header.printWindow, menu->header.queue, menu->header.font);
    }
}

// pokebw2's BmpMenu_GetCursorXY
static void func_0201fd54(BmpMenu *menu, u8 *x, u8 *y, u8 pos) {
    *x = (pos / menu->header.yCount) * (menu->maxWidth + menu->fontSizeX * 2);
    *y = menu->y + (pos % menu->header.yCount) * (menu->fontSizeY + menu->header.lineSpacing);
}

BmpMenu *ShopUI_CreateConfirmDialog(const ConfirmDialogSetup *setup, u16 frameChar, u8 framePalette, u8 cursorPos,
                                    HeapID heapId) {
    BmpMenuHeader header;
    MsgData *msgData;
    ListMenuOption *options;
    BmpMenu *menu;

    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, MSG_YESNO, heapId);
    options = ListMenuCore_CreateOptionList(2, heapId);
    ListMenuCore_AppendMsgOption(options, msgData, MSG_YESNO_YES, 0, heapId);
    ListMenuCore_AppendMsgOption(options, msgData, MSG_YESNO_NO, BMPMENU_CANCEL, heapId);
    GFL_MsgDataFree(msgData);

    MI_CpuFill8(&header, 0, sizeof(header));
    header.options = options;
    header.window = BmpWin_CreateDynamic(setup->bg, setup->x, setup->y, 7, 4, setup->palette, TRUE);
    header.xCount = 1;
    header.yCount = 2;
    header.lineSpacing = 1;
    header.cursorDisplay = BMPMENU_CURSOR_SHOW;
    header.loop = FALSE;
    header.printWindow = GFL_HeapAllocate(heapId, sizeof(PrintWindow), TRUE, "bmp_menu.c", 611);
    header.font = GFL_FontCreate(ARCID_FONT, 0, 0, 0, heapId);
    header.fontSizeY = GFL_FontGetCharHeight(header.font);
    header.fontSizeX = 10;
    PrintWindow_Init(header.printWindow, header.window);
    header.queue = func_0201c440(heapId);
    header.printed = FALSE;

    menu = func_0201f9d0(&header, 14, 0, cursorPos, heapId, PAD_BUTTON_B);
    BmpWin_FlushMap(header.window);
    BmpWin_DrawFrame(header.window, WINFRAME_TRANSFER_NOW, frameChar, framePalette);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(header.window));
    BmpWin_FlushChar(header.window);
    return menu;
}

// pokebw2's ConfirmDialog_IsPrinted, which Black inlines
static inline BOOL ConfirmDialog_IsPrinted(BmpMenu *menu) {
    BOOL done = func_0201c4e4(menu->header.queue);

    if (!menu->header.printed && done) {
        BmpWin_FlushChar(menu->header.window);
        menu->header.printed = TRUE;
    }
    return menu->header.printed;
}

// pokebw2's ConfirmDialog_Update, without the wait before the cursor appears
u32 func_0201febc(BmpMenu *menu) {
    u32 result = shopDecision(menu);

    ConfirmDialog_IsPrinted(menu);
    if (result != BMPMENU_NULL) {
        func_0201fef4(menu);
    }
    return result;
}

// pokebw2's ConfirmDialog_Free
void func_0201fef4(BmpMenu *menu) {
    func_0201c6ec(menu->header.queue);
    func_0201c4c0(menu->header.queue);
    GFL_HeapFree(menu->header.printWindow);
    func_0201f7a8(menu->header.window, WINFRAME_TRANSFER_NOW);
    GFL_FontFree(menu->header.font);
    ListMenuCore_FreeOptionList(menu->header.options);
    BmpWin_Free(menu->header.window);
    func_0201f9e8(menu, NULL);
}

