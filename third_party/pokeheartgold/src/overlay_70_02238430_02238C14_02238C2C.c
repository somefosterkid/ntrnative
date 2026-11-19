#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "yes_no_prompt.h"

void ov70_02238FB4(s32 arg0);

YesNoPrompt *ov70_02238C2C(BgConfig *param_1, u8 param_2, s32 param_3, u32 param_4, s32 param_5, u8 param_6);

YesNoPrompt *ov70_02238C2C(BgConfig *param_1, u8 param_2, s32 param_3, u32 param_4, s32 param_5, u8 param_6) {
    YesNoPrompt *pYVar1;
    YesNoPromptTemplate YStack_2c;

    pYVar1 = YesNoPrompt_Create(HEAP_ID_61);
    if (param_6 != 0) {
        s32 flag = 0;
        if (param_5 <= 3) {
            flag = 1;
        }
        ov70_02238FB4(flag);
    }
    YStack_2c.bgConfig = param_1;
    YStack_2c.bgId = param_5;
    YStack_2c.tileStart = param_3;
    YStack_2c.plttSlot = param_4;
    YStack_2c.x = 0x18;
    YStack_2c.y = param_2;
    YStack_2c.ignoreTouchFlag = 0;
    YStack_2c.initialCursorPos = 0;
    YStack_2c.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(pYVar1, &YStack_2c);
    return pYVar1;
}
