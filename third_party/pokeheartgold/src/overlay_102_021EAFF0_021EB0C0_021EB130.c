#include "global.h"

#include "yes_no_prompt.h"
#include "overlay_102_easy_chat_sentence_view.h"

u32 ov102_021EB130(EasyChatSentenceView *view) {
    YesNoResponse YVar1;
    u32 uVar2;
    YVar1 = YesNoPrompt_HandleInput(view->yesNoPrompt);
    switch (YVar1) {
    case YESNORESPONSE_YES:
        uVar2 = 1;
        break;
    case YESNORESPONSE_NO:
        uVar2 = 0;
        break;
    default:
        return 0xffffffff;
    }
    YesNoPrompt_IsInTouchMode(view->yesNoPrompt);
    YesNoPrompt_Reset(view->yesNoPrompt);
    return uVar2;
}
