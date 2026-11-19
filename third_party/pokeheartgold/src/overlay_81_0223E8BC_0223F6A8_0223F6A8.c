#include "global.h"

#include "overlay_81.h"
#include "yes_no_prompt.h"
#include "overlay_81_battle_factory_app.h"

s32 ov80_02237254(u8);

s32 ov81_0223F754(void *);

void ov81_02242E08(BattleFactoryAppPanelSprite *panelSprite, int palIndex);

s32 ov81_0223F6A8(void *arg0) {
    enum YesNoResponse v0;
    u8 v1;

    v1 = ((BattleFactoryApp *)arg0)->subState;
    switch (v1) {
    case 0:
        *((s8 *)&((BattleFactoryApp *)arg0)->printerID) = ov81_0224086C(arg0, 0xA);
        ov81_02241BD0(((BattleFactoryApp *)arg0)->yesNoPrompt, ((BattleFactoryApp *)arg0)->bgConfig);
        ov81_02242694(arg0, 1);
        ((BattleFactoryApp *)arg0)->subState = (u8)(((BattleFactoryApp *)arg0)->subState + 1);
    default:
        break;
    case 1:
        v0 = YesNoPrompt_HandleInput(((BattleFactoryApp *)arg0)->yesNoPrompt);
        switch (v0) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(((BattleFactoryApp *)arg0)->yesNoPrompt);
            ov81_02242694(arg0, 0);
            if (ov80_02237254(((BattleFactoryApp *)arg0)->challengeType) == 1) {
                *((s8 *)&((BattleFactoryApp *)arg0)->printerID) = ov81_0224086C(arg0, 2);
            }
            ((BattleFactoryApp *)arg0)->subState = (u8)(((BattleFactoryApp *)arg0)->subState + 1);
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(((BattleFactoryApp *)arg0)->yesNoPrompt);
            ov81_02242694(arg0, 0);
            ov81_0223F754(arg0);
            ov81_02242E08(((BattleFactoryApp *)arg0)->unk_388, 0);
            ((BattleFactoryApp *)arg0)->subState = (u8)(((BattleFactoryApp *)arg0)->subState + 1);
            break;
        }
        break;
    case 2:
        return 1;
    }
    return 0;
}
