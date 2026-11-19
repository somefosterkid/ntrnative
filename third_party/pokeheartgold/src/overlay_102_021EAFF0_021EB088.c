#include "global.h"

#include "sprite.h"
#include "overlay_102_easy_chat_sentence_view.h"

void ov102_021EB088(EasyChatSentenceView *view, s32 flag) {
    if (view->wordBoxCount != 0) {
        if (flag != 0) {
            Sprite_SetAnimCtrlSeq(view->sprites[0], 0);
            return;
        }
        Sprite_SetAnimCtrlSeq(view->sprites[0], 1);
        return;
    }
    if (0 != flag) {
        Sprite_SetAnimCtrlSeq(view->sprites[0], 0xE);
        return;
    }
    Sprite_SetAnimCtrlSeq(view->sprites[0], 0xF);
}
