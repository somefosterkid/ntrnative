#include "global.h"

#include "constants/heap.h"

#include "sprite.h"
#include "sprite_resources_header.h"
#include "overlay_102_easy_chat_sentence_view.h"

Sprite *ov102_021EA2B4(struct UnkStruct_ov20_021D2128_t *parent, SpriteResourcesHeader *header, u32 x, u32 y, u32 arg4, int arg5);

s32 ov102_021E8F6C(struct EasyChatApp *app);

s32 ov102_021EA270(struct UnkStruct_ov20_021D2128_t *parent, SpriteResourcesHeader *header, s32 arg2, s32 arg3);

void ov102_021EA920(EasyChatSentenceView *view) {
    SpriteResourcesHeader sp30;
    SpriteResourcesHeader spC;
    EasyChatSentenceViewPos sp8;
    Sprite *temp_r0;
    Sprite *temp_r0_4;
    Sprite *temp_r0_5;
    Sprite *temp_r0_3;
    Sprite *temp_r0_2;

    if (view->wordBoxCount != 0) {
        ov102_021EAD5C(&view->wordPos[0], &sp8);
    } else {
        sp8.x = 0x80;
        sp8.y = 0x18;
    }
    ov102_021EA270(view->parent, &sp30, 0, 0);
    ov102_021EA270(view->parent, &spC, 1, 0);
    view->sprites[0] = ov102_021EA2B4(view->parent, &sp30, (u32) sp8.x, (u32) sp8.y, (u32)(0U), 1);
    temp_r0 = ov102_021EA2B4(view->parent, &spC, (u32) sp8.x, sp8.y - view->scrollY, 0U, 2);
    view->sprites[1] = temp_r0;
    Sprite_SetAnimCtrlSeq(temp_r0, 1);
    ov102_021EB088(view, 1);
    temp_r0_2 = ov102_021EA2B4(view->parent, &sp30, 8U, 0x40U, 1U, 1);
    view->sprites[2] = temp_r0_2;
    Sprite_SetAnimCtrlSeq(temp_r0_2, 0x13);
    temp_r0_3 = ov102_021EA2B4(view->parent, &sp30, 0xF8U, 0x40U, 1U, 1);
    view->sprites[3] = temp_r0_3;
    Sprite_SetAnimCtrlSeq(temp_r0_3, 0x15);
    temp_r0_4 = ov102_021EA2B4(view->parent, &sp30, 0x1CU, 0x40U, 1U, 1);
    view->sprites[5] = temp_r0_4;
    Sprite_SetAnimCtrlSeq(temp_r0_4, 0x12);
    temp_r0_5 = ov102_021EA2B4(view->parent, &sp30, 0x80U, 0x40U, 1U, 1);
    view->sprites[4] = temp_r0_5;
    Sprite_SetAnimCtrlSeq(temp_r0_5, 0x17);
    if (ov102_021E8F6C(view->app) != 2) {
        Sprite_SetDrawFlag(view->sprites[3], 0);
        Sprite_SetDrawFlag(view->sprites[2], 0);
        Sprite_SetDrawFlag(view->sprites[4], 0);
        Sprite_SetDrawFlag(view->sprites[5], 0);
    }
}
