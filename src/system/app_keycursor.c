// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/app_keycursor.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/touchpanel.h"
#include "field/player_action.h"
#include "system/app_keycursor.h"
#include "system/printsys.h"

// The cursor that bobs in the bottom right corner of a window while its message waits for a button

// The cursor's size and where it goes, from the window's bottom right corner
#define CURSOR_WIDTH 10
#define CURSOR_HEIGHT 7
#define CURSOR_RIGHT 11
#define CURSOR_BOTTOM 9

// The cursor moves down a pixel every 8 frames, and back up after 3
#define CURSOR_FRAME_TIME 8
#define CURSOR_FRAMES 3

struct KeyCursor {
    GFLBitmap *bitmap;
    u8 frame;
    u8 timer;
    u8 erased;
    u8 keys : 1;
    u8 touch : 1;
    u8 showWhenDone : 1;
    u16 bgColor;
};

// The cursor: a down arrow at the top left of 2 by 2 tiles of 4 bits per pixel, 0x20 bytes each
// pokebw2's sCursorBitmap
static const u8 data_0209fd10[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x21, 0x22, 0x22,
    0x22, 0x10, 0x22, 0x22, 0x12, 0x00, 0x21, 0x22, 0x11, 0x00, 0x10, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// pokebw2's KeyCursor_Create
KeyCursor *func_02028590(u16 bgColor, BOOL keys, BOOL touch, HeapID heapId) {
    return func_020285a4(bgColor, keys, touch, heapId, data_0209fd10);
}

// pokebw2's KeyCursor_CreateEx
KeyCursor *func_020285a4(u16 bgColor, BOOL keys, BOOL touch, HeapID heapId, const u8 *bitmap) {
    KeyCursor *cursor = GFL_HeapAllocate(heapId, sizeof(KeyCursor), TRUE, "app_keycursor.c", 149);

    cursor->bgColor = bgColor;
    cursor->keys = keys;
    cursor->touch = touch;
    cursor->bitmap = GFL_BitmapWrap((void *)bitmap, 2, 2, 0x20, heapId);
    return cursor;
}

// pokebw2's KeyCursor_Free
void func_02028604(KeyCursor *cursor) {
    GFL_BitmapFree(cursor->bitmap);
    GFL_HeapFree(cursor);
}

// pokebw2's KeyCursor_Erase
void func_02028618(KeyCursor *cursor, GFLBitmap *bitmap, u16 bgColor) {
    u16 x = GFL_BitmapGetWidth(bitmap) - CURSOR_RIGHT;
    u16 y = GFL_BitmapGetHeight(bitmap) - CURSOR_BOTTOM;

    GFL_BitmapFillArea(bitmap, x, y + cursor->frame, CURSOR_WIDTH, CURSOR_HEIGHT, bgColor);
}

// pokebw2's KeyCursor_Draw
void func_0202865c(KeyCursor *cursor, GFLBitmap *bitmap, u16 bgColor) {
    u16 x;
    u16 y;

    func_02028618(cursor, bitmap, bgColor);
    cursor->timer++;
    if (cursor->timer >= CURSOR_FRAME_TIME) {
        cursor->timer = 0;
        cursor->frame++;
        cursor->frame %= CURSOR_FRAMES;
    }

    x = GFL_BitmapGetWidth(bitmap) - CURSOR_RIGHT;
    y = GFL_BitmapGetHeight(bitmap) - CURSOR_BOTTOM;
    GFL_BitmapCopyArea(cursor->bitmap, bitmap, 0, 2, x, y + cursor->frame, CURSOR_WIDTH, CURSOR_HEIGHT, 0);
}

void func_020286c4(KeyCursor *cursor, PrintStream *stream, BmpWin *window) {
    BOOL pressed;

    switch (func_0201ce30(stream)) {
    case PRINT_STREAM_DONE:
        cursor->erased = FALSE;
        break;
    case PRINT_STREAM_PAUSED:
        pressed = FALSE;
        if (cursor->keys) {
            pressed = GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B);
        }
        if (pressed == FALSE && cursor->touch) {
            pressed = func_02035c60();
        }

        if (pressed) {
            cursor->erased = TRUE;
            func_02028618(cursor, BmpWin_GetBitmap(window), cursor->bgColor);
            BmpWin_FlushChar(window);
        } else if (cursor->erased == FALSE) {
            func_0202865c(cursor, BmpWin_GetBitmap(window), cursor->bgColor);
            BmpWin_FlushChar(window);
        }
        break;
    case PRINT_STREAM_RUNNING:
        cursor->erased = FALSE;
        break;
    }
}

void func_02028748(PlayerActionPerms *perms, u32 action, u32 blocked) {
    perms->blocked |= blocked << action;
}

BOOL func_02028754(PlayerActionPerms *perms, u32 action) {
    return (perms->blocked >> action) & 1;
}
