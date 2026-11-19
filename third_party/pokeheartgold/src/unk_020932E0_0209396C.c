#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "unk_020932E0.h"
#include "yes_no_prompt.h"

void sub_0209396C(UnkStruct_020932E0 *param_1) {
    YesNoPromptTemplate YStack_20;

    MI_CpuFill8(&YStack_20, 0, sizeof(YesNoPromptTemplate));
    YStack_20.bgConfig = param_1->unk_0;
    YStack_20.bgId = 0;
    YStack_20.tileStart = 0x200;
    YStack_20.plttSlot = 8;
    YStack_20.x = 0x19;
    YStack_20.y = 0xf;
    YesNoPrompt_InitFromTemplate(param_1->unk_8, &YStack_20);
    sub_0209501C(param_1, 4, FONT_SYSTEM);
    sub_02095780();
    param_1->unk_14 = 5;
    return;
}

void sub_020939B8(UnkStruct_020932E0 *arg0) {
    enum YesNoResponse v0;

    v0 = YesNoPrompt_HandleInput(arg0->unk_8);
    switch (v0) {
    case YESNORESPONSE_YES:
        sub_020950F8(&arg0->unk_4660, 1);
        YesNoPrompt_Reset(arg0->unk_8);
        sub_02094F5C(arg0, arg0->unk_4684);
        Sprite_SetAnimCtrlSeq(arg0->unk_8C0, 0x2D);
        sub_02094A90(arg0, 2);
        arg0->unk_46BC = 0;
        sub_02095794();
        sub_020956B8(arg0);
        arg0->unk_14 = 0;
        return;
    case YESNORESPONSE_NO:
        sub_020950F8(&arg0->unk_4660, 1);
        YesNoPrompt_Reset(arg0->unk_8);
        sub_02094A90(arg0, 0);
        sub_02095794();
        arg0->unk_14 = 0;
        return;
    }
}

void sub_02093A40(UnkStruct_020932E0 *arg0) {
    SpriteList *spriteList;

    spriteList = arg0->unk_18;
    if (spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(spriteList);
    }
}
