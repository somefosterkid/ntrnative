#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"

void ov82_0223FDC8(YesNoPrompt *param_1, BgConfig *param_2, u8 param_3);

void ov82_0223FDC8(YesNoPrompt *param_1, BgConfig *param_2, u8 param_3) {
    YesNoPromptTemplate YStack_28;
    MI_CpuFill8(&YStack_28, 0, sizeof(YesNoPromptTemplate));
    YStack_28.bgConfig = param_2;
    YStack_28.bgId = 0;
    YStack_28.tileStart = 0x6d;
    YStack_28.plttSlot = 0xe;
    YStack_28.x = 0x18;
    YStack_28.y = 0xa;
    YStack_28.ignoreTouchFlag = param_3;
    YStack_28.initialCursorPos = 0;
    YStack_28.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(param_1, &YStack_28);
    return;
}
