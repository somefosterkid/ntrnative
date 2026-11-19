#include "global.h"
struct WifiBattleTowerAppState;

#include "yes_no_prompt.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

enum YesNoResponse ov73_021E83D4(YesNoPrompt *arg0);

enum YesNoResponse ov73_021E83D4(YesNoPrompt *arg0) {
    enum YesNoResponse v0;

    v0 = YesNoPrompt_HandleInput(arg0);
    if (v0 != YESNORESPONSE_WAIT) {
        YesNoPrompt_Destroy(arg0);
    }
    return v0;
}

void ov73_021E83EC(struct WifiBattleTowerAppState *arg0, s32 arg1, s32 arg2);

void ov73_021E83EC(WifiBattleTowerAppState *arg0, s32 arg1, s32 arg2) {
    *((s32 *)&arg0->state) = arg1;
    *((s32 *)&arg0->nextState) = arg2;
}
