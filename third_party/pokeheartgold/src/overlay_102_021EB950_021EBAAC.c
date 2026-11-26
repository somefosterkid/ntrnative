#include "global.h"
#include "bg_window.h"
#include "overlay_102_easy_chat_categories_context.h"

s32 ov102_021EC298(void *, BgConfig *, s32, s32, s32, s32);

void ov102_021EBAAC(EasyChatCategoriesContext *arg0);

void ov102_021EBAAC(EasyChatCategoriesContext *arg0) {
    arg0->unk_14 = 0;
    ov102_021EC298(&arg0->unk_3C, arg0->bgConfig, 1, 1, 0, 6);
}
