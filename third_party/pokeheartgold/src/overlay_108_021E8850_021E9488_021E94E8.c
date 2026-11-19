#include "global.h"

#include "menu_input_state.h"
#include "yes_no_prompt.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

u32 ov108_021E94E8(SafariDecorationApp *param_1) {
    YesNoResponse YVar1;
    u32 uVar3;
    YVar1 = YesNoPrompt_HandleInput(param_1->yesNoPrompt);
    switch (YVar1) {
    case YESNORESPONSE_YES:
        uVar3 = 1;
        break;
    case YESNORESPONSE_NO:
        uVar3 = 0;
        break;
    default:
        return 0xffffffff;
    }
    param_1->menuInputState = YesNoPrompt_IsInTouchMode(param_1->yesNoPrompt);
    YesNoPrompt_Reset(param_1->yesNoPrompt);
    return uVar3;
}
