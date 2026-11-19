#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "yes_no_prompt.h"

YesNoPrompt *ov39_02228AC0(BgConfig *param_1, s32 param_2);

YesNoPrompt *ov39_02228AC0(BgConfig *param_1, s32 param_2) {
    YesNoPrompt *pYVar1;
    YesNoPromptTemplate YStack_28;

    pYVar1 = YesNoPrompt_Create(HEAP_ID_124);
    YStack_28.bgConfig = param_1;
    YStack_28.bgId = 0;
    YStack_28.tileStart = param_2;
    YStack_28.plttSlot = 0xe;
    YStack_28.x = 0x19;
    YStack_28.y = 0xa;
    YStack_28.ignoreTouchFlag = 0;
    YStack_28.initialCursorPos = 0;
    YStack_28.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(pYVar1, &YStack_28);
    return pYVar1;
}
