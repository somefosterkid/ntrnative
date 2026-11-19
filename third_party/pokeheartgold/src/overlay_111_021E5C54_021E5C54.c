#include "global.h"

#include "yes_no_prompt.h"
#include "overlay_111.h"

void ov111_021E5C54(BugContestSwapMon *arg0);

void ov111_021E5C54(BugContestSwapMon *arg0) {
    YesNoPromptTemplate sp0 = { 0 };

    sp0.bgConfig = arg0->bgConfig;
    sp0.bgId = 1;
    sp0.tileStart = 0xC8;
    sp0.plttSlot = 6;
    sp0.x = 0x19;
    sp0.y = 0xA;
    sp0.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(arg0->yesNoPrompt, &sp0);
}
