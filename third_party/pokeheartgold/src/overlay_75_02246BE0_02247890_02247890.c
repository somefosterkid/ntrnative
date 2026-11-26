#include "global.h"

#include "yes_no_prompt.h"

YesNoPrompt *ov75_02247890(BgConfig *arg0, s32 arg1, u32 arg2);

YesNoPrompt *ov75_02247890(BgConfig *arg0, s32 arg1, u32 arg2) {
    YesNoPromptTemplate sp0;
    YesNoPrompt *temp_r0;

    s32 bgId = 0;
    temp_r0 = YesNoPrompt_Create(HEAP_ID_116);
    sp0.bgConfig = (BgConfig *)arg0;
    sp0.bgId = bgId;
    sp0.tileStart = arg1;
    sp0.plttSlot = 0xE;
    sp0.x = 0x19;
    sp0.y = 0xA;
    sp0.ignoreTouchFlag = 0;
    sp0.initialCursorPos = arg2;
    sp0.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(temp_r0, &sp0);
    return temp_r0;
}
