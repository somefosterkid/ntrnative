#include "global.h"

#include "unk_020379A0.h"
#include "yes_no_prompt.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

u8 ov80_02236DD4(u8);

s32 ov80_02237254(u8 challengeType);

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

BOOL ov81_0223F1A4(BattleFactoryApp *app);

BOOL ov81_0223F1A4(BattleFactoryApp *app) {
    u8 numMons = ov80_02236DD4(app->challengeType);

    switch (app->subState) {
    case 0:
        ov81_02242D88(app->unk_388, 0);
        ov81_02242D88(app->unk_390, 0);
        app->animationTimer = 0;
        app->subState++;
        break;
    case 1:
        app->animationTimer = 0;
        app->subState++;
        break;
    case 2:
        app->subState++;
        break;
    case 3:
        app->animationTimer = 0;
        app->subState++;
        break;
    case 4:
        ov81_022408A0(app, 0, numMons);
        app->printerID = ov81_0224086C(app, 1);
        ov81_02241BD0(app->yesNoPrompt, app->bgConfig);
        ov81_02242694(app, 1);
        app->animationTimer = 0;
        app->subState++;
        break;
    case 5:
        switch (YesNoPrompt_HandleInput(app->yesNoPrompt)) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            ov81_02242694(app, 0);
            app->subState++;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            ov81_02242694(app, 0);
            ov81_02242D88(app->unk_388, 1);
            ov81_02242D88(app->unk_390, 1);
            app->numMonsSelected--;
            app->selectedIndices[app->numMonsSelected] = 0;
            if (ov80_02237254(app->challengeType) == 1) {
                ov81_02240FA4(app, 8, 0);
            }
            return TRUE;
        }
        break;
    case 6:
        if (ov80_02237254(app->challengeType) == 0) {
            return TRUE;
        }
        if (ov81_02240FA4(app, 8, 0) == 1) {
            app->printerID = ov81_0224086C(app, 2);
            sub_02037BEC();
            sub_02037AC0(0xa4);
            app->subState++;
        }
        break;
    case 7:
        if (sub_02037B38(0xa4) == 1) {
            sub_02037BEC();
            return TRUE;
        }
        break;
    }
    return FALSE;
}
