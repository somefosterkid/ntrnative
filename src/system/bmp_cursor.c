// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/bmp_cursor.c, which is the same code in
// Black 2/White 2's main module. The names are swan's or Black's dsd names; pokebw2's guesses are in the comments.
#include "types.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "system/bmp_cursor.h"
#include "system/printsys.h"

// The cursor menus draw before their selected option: an 8x8 bitmap, or a string, which nothing in the ROM sets

#define BMPCURSOR_BITMAP 0
#define BMPCURSOR_STRING 1

struct BmpCursor {
    u32 type;
    StrBuf *string;
    GFLBitmap *bitmap;
};

// The cursor's 16-color pixels, a triangle pointing right
static const u8 pixelData[] = {
#include "asset/bmp_cursor_pixels.inc"
};

BmpCursor *BmpCursor_Create(u32 heapId) {
    BmpCursor *cursor = GFL_HeapAllocate(heapId, sizeof(BmpCursor), TRUE, "bmp_cursor.c", 58);

    if (cursor != NULL) {
        cursor->type = BMPCURSOR_BITMAP;
    }
    return cursor;
}

// BmpCursor_Free
void func_02020d3c(BmpCursor *cursor) {
    if (cursor->string != NULL) {
        GFL_StrBufFree(cursor->string);
    }
    if (cursor->bitmap != NULL) {
        GFL_BitmapFree(cursor->bitmap);
    }
    GFL_HeapFree(cursor);
}

// BmpCursor_Print
void func_02020d5c(BmpCursor *cursor, u16 x, u16 y, PrintWindow *printWindow, PrintQueue *queue, Font *font) {
    if (cursor->type == BMPCURSOR_STRING) {
        func_0201c6fc(queue, BmpWin_GetBitmap(printWindow->window), x, y, cursor->string, font);
        printWindow->flushPending = TRUE;
    } else if (cursor->bitmap != NULL) {
        GFL_BitmapCopyArea(cursor->bitmap, BmpWin_GetBitmap(printWindow->window), 0, 0, x + 2, y + 2, 8, 8, 0xf);
        BmpWin_FlushChar(printWindow->window);
    }
}

// BmpCursor_LoadBitmap
void func_02020dc8(BmpCursor *cursor, u32 heapId) {
    cursor->bitmap = GFL_BitmapWrap((void *)pixelData, 1, 1, 0x20, heapId);
}
