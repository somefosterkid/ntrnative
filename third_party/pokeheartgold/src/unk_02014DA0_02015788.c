#include "global.h"

#include "heap.h"
#include "pm_string.h"
#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "text.h"

extern u16 _020F60DC;

struct ListMenuCursor *ListMenuCursorNew(enum HeapID heapID);

typedef u32 TextColor;
typedef struct ListMenuCursor {
    TextColor color;
    String *string;
} ColoredArrow;
void DestroyListMenuCursorObj(ColoredArrow *arrow);
void ListMenuCursorSetColor(ColoredArrow *arrow, TextColor color);
void ListMenuUpdateCursorObj(const ColoredArrow *arrow, Window *window, u32 xOffset, u32 yOffset);

struct ListMenuCursor *ListMenuCursorNew(enum HeapID heapID) {
    String *dest;
    struct ListMenuCursor *listMenuCursor;

    listMenuCursor = Heap_Alloc(heapID, sizeof(ColoredArrow));
    if (listMenuCursor != NULL) {
        listMenuCursor->color = 0x1020F;
        dest = String_New(4U, heapID);
        listMenuCursor->string = dest;
        CopyU16ArrayToString(dest, &_020F60DC);
    }
    return listMenuCursor;
}

void DestroyListMenuCursorObj(ColoredArrow *arrow) {
    ((arrow != 0L) ? (void)0 : GF_AssertFail());

    if (arrow) {
        if (arrow->string) {
            String_Delete(arrow->string);
        }

        Heap_Free(arrow);
    }
}

void ListMenuCursorSetColor(ColoredArrow *arrow, TextColor color) {
    ((arrow != 0L) ? (void)0 : GF_AssertFail());

    if (arrow) {
        arrow->color = color;
    }
}

void ListMenuUpdateCursorObj(const ColoredArrow *arrow, Window *window, u32 xOffset, u32 yOffset) {
    AddTextPrinterParameterizedWithColor(window, FONT_SYSTEM, arrow->string, xOffset, yOffset, 0xFF, arrow->color, 0L);
    CopyWindowPixelsToVram_TextMode(window);
}
