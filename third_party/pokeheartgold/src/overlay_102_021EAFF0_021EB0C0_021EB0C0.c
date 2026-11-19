#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_102_easy_chat_sentence_view.h"

BgConfig *ov102_021EA268(struct UnkStruct_ov20_021D2128_t *parent, u8 arg1, s32 arg2);

void ov102_021EB0C0(EasyChatSentenceView *view, u8 param_2, s32 arg2);

void ov102_021EB0C0(EasyChatSentenceView *view, u8 param_2, s32 arg2) {
    BgConfig *pBVar1;
    YesNoPromptTemplate YStack_28;

    pBVar1 = ov102_021EA268(view->parent, param_2, arg2);
    MI_CpuFill8(&YStack_28, 0, sizeof(YesNoPromptTemplate));
    YStack_28.bgConfig = pBVar1;
    YStack_28.bgId = 0;
    YStack_28.tileStart = (s32) view->yesNoTile;
    YStack_28.plttSlot = 3;
    YStack_28.x = 0x19;
    YStack_28.y = 0xc;
    YStack_28.ignoreTouchFlag = *view->ignoreTouchFlag;
    YStack_28.initialCursorPos = (u16)param_2;
    YesNoPrompt_InitFromTemplate(view->yesNoPrompt, &YStack_28);
    return;
}
