#include "global.h"

#include "sprite.h"
#include "overlay_102_easy_chat_sentence_view.h"

s32 ov102_021E8F6C(struct EasyChatApp *app);

void ov102_021EAF5C(EasyChatSentenceView *view, s32 flag) {
    Sprite_SetDrawFlag(view->sprites[0], flag);
    Sprite_SetDrawFlag(view->sprites[1], flag);
    ov102_021EB088(view, 1);
}

void ov102_021EAF7C(EasyChatSentenceView *view) {
    if (ov102_021E8F6C(view->app) == 2) {
        Sprite_SetDrawFlag(view->sprites[2], 0);
        Sprite_SetDrawFlag(view->sprites[3], 0);
        Sprite_SetDrawFlag(view->sprites[5], 0);
        Sprite_SetDrawFlag(view->sprites[4], 0);
    }
}

void ov102_021EAFAC(EasyChatSentenceView *view) {
    if (ov102_021E8F6C(view->app) == 2) {
        Sprite_SetDrawFlag(view->sprites[2], 1);
        Sprite_SetDrawFlag(view->sprites[3], 1);
        Sprite_SetDrawFlag(view->sprites[5], 1);
        Sprite_SetDrawFlag(view->sprites[4], 1);
        Sprite_SetAnimCtrlSeq(view->sprites[2], 0x13);
        Sprite_SetAnimCtrlSeq(view->sprites[3], 0x15);
    }
}
