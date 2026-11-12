// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov285/b_app_tool.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/b_app_tool.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "system/cursor_move.h"
#include "system/printsys.h"

// pokebw2's BAppCursor_Create
BAppCursor *func_ov196_0220af40(HeapID heapId) {
    BAppCursor *cursor = GFL_HeapAllocate(heapId, sizeof(BAppCursor), FALSE, "b_app_tool.c", 43);
    cursor->heapId = heapId;
    cursor->visible = TRUE;
    return cursor;
}

// pokebw2's BAppCursor_Delete
void func_ov196_0220af64(BAppCursor *cursor) {
    GFL_HeapFree(cursor);
}

// pokebw2's BAppCursor_CreateActors
void func_ov196_0220af6c(BAppCursor *cursor, ClActUnit *unit, u32 chars, u32 palette, u32 cellAnims) {
    ClActorSetup setup;
    u32 i;

    setup.x = 0;
    setup.y = 0;
    setup.priority = 0;
    setup.bgPriority = 1;
    for (i = 0; i < 4; i++) {
        setup.sequence = i;
        cursor->corners[i] = func_0204b294(unit, chars, palette, cellAnims, &setup, 1, cursor->heapId);
        func_0204b6f4(cursor->corners[i], 0, 1);
        func_0204b92c(cursor->corners[i], TRUE);
    }
}

// pokebw2's BAppCursor_DeleteActors
void func_ov196_0220afc4(BAppCursor *cursor) {
    u32 i;

    for (i = 0; i < 4; i++) {
        func_0204b3b4(cursor->corners[i]);
    }
}

// pokebw2's BAppCursor_SetVisible
void func_ov196_0220afdc(BAppCursor *cursor, BOOL visible) {
    u32 i;

    if (cursor->visible == visible) {
        return;
    }
    for (i = 0; i < 4; i++) {
        func_0204b3dc(cursor->corners[i], visible);
    }
    cursor->visible = visible;
}

// pokebw2's BAppCursor_SetPos
void func_ov196_0220b000(BAppCursor *cursor, const CursorMoveData *pos) {
    ClActorPos actorPos;
    u8 halfWidth = pos->width / 2;
    u8 halfHeight = pos->height / 2;

    actorPos.x = pos->x - halfWidth;
    actorPos.y = pos->y - halfHeight;
    func_0204b404(cursor->corners[0], &actorPos, 1);
    actorPos.x = pos->x - halfWidth;
    actorPos.y = pos->y + halfHeight;
    func_0204b404(cursor->corners[1], &actorPos, 1);
    actorPos.x = pos->x + halfWidth;
    actorPos.y = pos->y - halfHeight;
    func_0204b404(cursor->corners[2], &actorPos, 1);
    actorPos.x = pos->x + halfWidth;
    actorPos.y = pos->y + halfHeight;
    func_0204b404(cursor->corners[3], &actorPos, 1);
}

// pokebw2's BAppTool_QueueWindowScreen
void func_ov196_0220b07c(PrintWindow *printWindow) {
    BmpWin_FlushMap(printWindow->window);
    GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(printWindow->window));
}

// pokebw2's BAppTool_QueueWindowScreens
void func_ov196_0220b094(PrintWindow *printWindows, const u8 *list) {
    u32 i;

    i = 0;
    while (TRUE) {
        if (list[i] == 0xff) {
            break;
        }
        func_ov196_0220b07c(&printWindows[list[i]]);
        i++;
    }
}

// pokebw2's BAppTool_FlushPrintWindows
void func_ov196_0220b0b0(PrintWindow *printWindows, PrintQueue *queue, u32 count) {
    u32 i;

    func_0201c4e4(queue);
    for (i = 0; i < count; i++) {
        PrintWindow_Flush(&printWindows[i], queue);
    }
}
