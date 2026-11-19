#include "global.h"

#include "menu_input_state.h"
#include "yes_no_prompt.h"

s32 ov56_021E60F4(u8 *param_1);

s32 ov56_021E60F4(u8 *param_1) {
    YesNoResponse YVar1;
    MenuInputState MVar2;
    s32 sVar3;

    YVar1 = YesNoPrompt_HandleInput(*(YesNoPrompt **)(param_1 + 0xac));
    switch (YVar1) {
    case YESNORESPONSE_YES:
        sVar3 = 1;
        break;
    case YESNORESPONSE_NO:
        sVar3 = 0;
        break;
    default:
        return -1;
    }
    MVar2 = YesNoPrompt_IsInTouchMode(*(YesNoPrompt **)(param_1 + 0xac));
    if (MVar2 != *(u8 *)(param_1 + 10)) {
        if (*(u8 *)(param_1 + 10) == MENU_INPUT_STATE_BUTTONS) {
            (**(void (**)(u8 *))(param_1 + 0x34))(param_1);
        } else {
            (**(void (**)(u8 *))(param_1 + 0x38))(param_1);
        }
        *(s8 *)(param_1 + 10) = (s8)MVar2;
    }
    YesNoPrompt_Reset(*(YesNoPrompt **)(param_1 + 0xac));
    return sVar3;
}
