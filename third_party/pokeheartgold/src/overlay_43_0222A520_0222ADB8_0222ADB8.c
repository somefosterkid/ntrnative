#include "global.h"

#include "brightness.h"
#include "yes_no_prompt.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"

void ov43_0222ADB8(PalPadShared *arg0, PalPadGfx *arg1, u32 arg2) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0U, sizeof(YesNoPromptTemplate));
    template.bgConfig = arg1->bgConfig;
    template.bgId = 1;
    template.tileStart = 0xA;
    template.plttSlot = 0xD;
    template.x = 0x19;
    template.y = 9;
    template.ignoreTouchFlag = (u8)arg0->inputState;
    template.initialCursorPos = (u8)arg2;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(arg1->yesNoPrompt, &template);
    StartBrightnessTransition(4U, 8, 0, (GXBlendPlaneMask)(0x3D), 1U);
}
