#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "menu_input_state.h"
#include "yes_no_prompt.h"

u32 ov59_02238FB4(Ov59ApricornApp *param_1);

u32 ov59_02238FB4(Ov59ApricornApp *param_1) {
    u32 ret;
    switch (YesNoPrompt_HandleInput(param_1->yesNoPrompt)) {
    case YESNORESPONSE_YES:
        ret = 1;
        break;
    case YESNORESPONSE_NO:
        ret = 0;
        break;
    default:
        return -1;
    }
    param_1->menuInputState = YesNoPrompt_IsInTouchMode(param_1->yesNoPrompt);
    YesNoPrompt_Reset(param_1->yesNoPrompt);
    return ret;
}
