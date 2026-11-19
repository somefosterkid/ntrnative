#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "yes_no_prompt.h"

void ov59_02238F58(Ov59ApricornApp *arg0);

void ov59_02238F58(Ov59ApricornApp *arg0) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0U, sizeof(YesNoPromptTemplate));
    template.bgConfig = *((BgConfig **)&arg0->bgConfig);
    template.bgId = 0;
    template.tileStart = 0x6A;
    template.plttSlot = 0xE;
    template.x = 0x19;
    template.y = 5;
    template.ignoreTouchFlag = (u8) *((s32 *)&arg0->menuInputState);
    template.initialCursorPos = 1;
    YesNoPrompt_InitFromTemplate(*((YesNoPrompt * *)&arg0->yesNoPrompt), &template);
}
