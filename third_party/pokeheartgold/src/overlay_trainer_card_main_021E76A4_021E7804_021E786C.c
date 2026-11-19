#include "global.h"

#include "bg_window.h"
#include "menu_input_state.h"
#include "render_window.h"
#include "trainer_card_main.h"
#include "yes_no_prompt.h"

int ov51_021E786C(TrainerCardMain *cardMain, int param_2) {
    YesNoResponse YVar1;
    MenuInputState MVar2;
    s32 iVar3;

    YVar1 = YesNoPrompt_HandleInput(cardMain->yesNoPrompt);
    switch (YVar1) {
    case YESNORESPONSE_YES:
        iVar3 = 1;
        break;
    case YESNORESPONSE_NO:
        iVar3 = 0;
        break;
    default:
        return -1;
    }
    MVar2 = YesNoPrompt_IsInTouchMode(cardMain->yesNoPrompt);
    cardMain->inTouchMode = MVar2;
    YesNoPrompt_Reset(cardMain->yesNoPrompt);
    if ((iVar3 == 0) || (param_2 != 0)) {
        ClearFrameAndWindow2(&cardMain->windows[11], 0);
        ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, 4);
    }
    return iVar3;
}
