#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "yes_no_prompt.h"

YesNoPrompt *ov73_021E8390(BgConfig *param_1, u8 param_2, s32 param_3);

YesNoPrompt *ov73_021E8390(BgConfig *param_1, u8 param_2, s32 param_3) {
    YesNoPrompt *pYVar1;
    YesNoPromptTemplate YStack_28;

    pYVar1 = YesNoPrompt_Create(HEAP_ID_150);
    YStack_28.bgConfig = param_1;
    YStack_28.bgId = 0;
    YStack_28.plttSlot = 8;
    YStack_28.tileStart = param_3;
    YStack_28.x = 0x17;
    YStack_28.y = param_2;
    YStack_28.ignoreTouchFlag = 0;
    YStack_28.initialCursorPos = 0;
    YStack_28.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(pYVar1, &YStack_28);
    return pYVar1;
}
