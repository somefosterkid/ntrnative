#include "global.h"

#include "pm_string.h"
#include "sprite_system.h"
#include "overlay_102_easy_chat_sentence_view.h"

s32 ov102_021E8FB4(struct EasyChatApp *app);

String *ov102_021E8FA8(struct EasyChatApp *app, s32 id);

void ov102_021EAC20(EasyChatSentenceView *view) {
    Sprite *sprite;
    s32 v0;

    v0 = ov102_021E8FB4(view->app);
    sprite = view->sprites[5];
    if (sprite != NULL) {
        Sprite_SetPositionXY(sprite, (s16)((v0 * 2) + 0x1C), 0x40);
    }
}

void ov102_021EAC44(EasyChatTemplateReader *reader, EasyChatSentenceView *view) {
    String *str;
    u16 *v0;

    str = ov102_021E8FA8(view->app, 0x23);
    reader->string = str;
    v0 = String_cstr(str);
    reader->cur = v0;
    if (*v0 == 0xFFFE) {
        reader->state = 1;
        return;
    }
    reader->state = 0;
}
