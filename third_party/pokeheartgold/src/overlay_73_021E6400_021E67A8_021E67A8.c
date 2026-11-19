#include "global.h"

#include "bg_window.h"
#include "unk_02035900.h"
#include "yes_no_prompt.h"

s32 ov73_021E71E4(void *, s32, s8);

void ov73_021E67A8(void *arg0);

void ov73_021E67A8(void *arg0) {
    YesNoPromptTemplate template;

    MI_CpuFill8(&template, 0U, sizeof(YesNoPromptTemplate));
    template.bgConfig = *(BgConfig **)arg0;
    template.bgId = 0;
    template.tileStart = 0x1F;
    template.plttSlot = 0xE;
    template.x = 0x19;
    template.y = 6;
    template.ignoreTouchFlag = (u8) * (s32 *)((u8 *)arg0 + 0x314);
    template.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(*(YesNoPrompt **)((u8 *)arg0 + 0x2E8), &template);
    *(s8 *)((u8 *)arg0 + 0x4A15) = 1;
    if (sub_0203769C() == 0) {
        ov73_021E71E4(arg0, 0, 2);
    }
    ov73_021E71E4(arg0, 1, 2);
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)arg0, 1U);
}
