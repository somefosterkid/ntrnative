#include "global.h"

#include "bg_window.h"
#include "easy_chat.h"
#include "font.h"
#include "text.h"
#include "overlay_102_easy_chat_sentence_view.h"

BgConfig *ov102_021EA268(struct UnkStruct_ov20_021D2128_t *parent);

s32 sub_0200E948(Window *, u32, s32);

void ov102_021EAD98(EasyChatSentenceView *view, Window *window, EasyChatSentenceViewPos *pos, u16 ecWord) {
    EasyChatSentenceViewPos v0;

    if (ecWord != 0xFFFF) {
        ov102_021EAD48(pos, &v0);
        GetECWordIntoStringByIndex(ecWord, view->string);
        FontID_String_GetWidth(FONT_MESSAGE, view->string, 0U);
        AddTextPrinterParameterizedWithColor(window, FONT_MESSAGE, view->string, (u32) v0.x, (u32) v0.y, 0xFFU, 0x5060EU, NULL);
    }
}

s32 ov102_021EADF0(const EasyChatSentenceView *view) {
    return view->wordBoxCount;
}

void ov102_021EADF8(EasyChatSentenceView *view, s32 idx) {
    Window *window;

    FillBgTilemapRect(ov102_021EA268(view->parent), 0U, 0U, 0U, 0x14U, 0x18U, 4U, 0x11U);
    ClearWindowTilemapAndCopyToVram(view->textWindow);
    window = &view->windows[2] + idx;
    view->textWindow = window;
    sub_0200E948(window, view->frameTile, 0xF);
}
