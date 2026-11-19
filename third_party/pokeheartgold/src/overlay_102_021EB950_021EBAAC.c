#include "global.h"
#include "overlay_102_easy_chat_categories_context.h"

s32 ov102_021EC298(void *, void *, s32, s32, s32, s32);

void ov102_021EBAAC(void *arg0);

void ov102_021EBAAC(void *arg0) {
    *((s32 *)&((EasyChatCategoriesContext *)arg0)->unk_14) = 0;
    ov102_021EC298(arg0 + 0x3C, *((void **)&((EasyChatCategoriesContext *)arg0)->bgConfig), 1, 1, 0, 6);
}
