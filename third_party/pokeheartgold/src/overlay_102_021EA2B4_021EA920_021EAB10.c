#include "global.h"

#include "overlay_102_easy_chat_sentence_view.h"

void ov102_021EAB10(const EasyChatSentenceView *view, TouchscreenHitbox *hitbox, s32 idx) {
    hitbox->rect.left = (u8)(view->wordPos[idx].x - 0x18);
    hitbox->rect.right = (s8)(hitbox->rect.left + 0x60);
    hitbox->rect.top = (u8) view->wordPos[idx].y;
    hitbox->rect.bottom = (s8)(hitbox->rect.top + 0x10);
}
