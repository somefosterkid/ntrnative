#include "global.h"

#include "bg_window.h"
#include "list_menu_cursor.h"
#include "overlay_102_easy_chat_sentence_view.h"

void ov102_021EB170(EasyChatSentenceView *view, u32 idx) {
    FillWindowPixelRect(&view->windows[4], 9U, 0U, 0U, 0xEU, 0x20U);
    ListMenuUpdateCursorObj(view->cursor, &view->windows[4], 0U, idx << 4);
}
