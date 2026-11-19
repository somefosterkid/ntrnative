#include "global.h"

#include "sprite.h"
#include "overlay_102_easy_chat_sentence_view.h"

void ov102_021EAFF0(EasyChatSentenceView *view, s32 idx) {
    VecFx32 mat;
    EasyChatSentenceViewPos pos;
    f32 var_r0;
    s32 temp_r0;

    if (view->wordBoxCount != 0) {
        ov102_021EAD5C(&view->wordPos[idx], &pos);
        mat.x = pos.x << 0xC;
        mat.y = pos.y << 0xC;
        mat.z = 0;
    } else {
        mat.x = 0x80000;
        mat.y = 0x18000;
        mat.z = 0;
    }
    Sprite_SetMatrix(view->sprites[0], &mat);
    temp_r0 = 0xC0 - view->scrollY;
    if (temp_r0 > 0) {
        var_r0 = 0.5f + ((f32)(temp_r0 << 0xC));
    } else {
        var_r0 = ((f32)(temp_r0 << 0xC)) - 0.5f;
    }
    mat.y += (s32)var_r0;
    Sprite_SetMatrix(view->sprites[1], &mat);
    ov102_021EB088(view, 1);
}
