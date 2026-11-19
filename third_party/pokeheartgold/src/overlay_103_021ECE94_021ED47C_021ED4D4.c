#include "global.h"

#include "yes_no_prompt.h"
#include "overlay_103_mail_box_view.h"
#include "overlay_103_mail_box_work.h"


u32 ov103_021ED4D4(MailBoxWork *param_1);

u32 ov103_021ED4D4(MailBoxWork *param_1) {
    YesNoResponse YVar1;
    u32 uVar2;
    u32 ret;

    YVar1 = YesNoPrompt_HandleInput(param_1->view->yesNoPrompt);
    switch (YVar1) {
    case YESNORESPONSE_YES:
        YesNoPrompt_Reset(param_1->view->yesNoPrompt);
        uVar2 = ov103_021EEC68[param_1->view->yesNoIndex][0](param_1);
        ret = uVar2;
        break;
    case YESNORESPONSE_NO:
        YesNoPrompt_Reset(param_1->view->yesNoPrompt);
        uVar2 = ov103_021EEC68[param_1->view->yesNoIndex][1](param_1);
        ret = uVar2;
        break;
    default:
        ret = 4;
        break;
    }
    return ret;
}
