#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

#include "bg_window.h"
#include "options.h"
#include "pokepic.h"
#include "unk_02005D10.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "yes_no_prompt.h"

s32 ov80_02237254(u8);

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

void ov81_02242E08(BattleFactoryAppPanelSprite *panelSprite, int palIndex);

u32 ov81_0223F770(BattleFactoryApp *app, u32 param_2, u32 param_3, u32 param_4);

u32 ov81_0223F770(BattleFactoryApp *app, u32 param_2, u32 param_3, u32 param_4) {
    u8 uVar1;
    u32 uVar2;
    u32 uVar3;
    YesNoResponse YVar4;
    s32 iVar5;

    switch (app->subState) {
    case 0:
        uVar2 = Options_GetFrame(app->options);
        ov81_02243028(&app->windows[7], uVar2);
        uVar1 = ov81_0224086C(app, 0xd);
        app->printerID = uVar1;
        app->subState = 1;
        break;
    case 1:
        uVar3 = GridInputHandler_HandleInput_NoHold(app->inputHandler);
        switch (uVar3) {
        case 0:
        case 1:
        case 2:
        case 3:
            PlaySE(0x5dc);
            app->unk_468 = uVar3;
            ov81_02242218(app, app->receivableParty, app->unk_468, 4);
            sub_020196E8(app->unk_474, 0, 7, 0);
            ov81_02241D94(app);
            ov81_022424AC(app, app->unk_468);
            ov81_02241980(app);
            ov81_02242E08(app->unk_388, 1);
            break;
        case 4:
            PlaySE(0x5dc);
            ov81_02242D88(app->monIconSprites[0], 0);
            ov81_02242D88(app->monIconSprites[1], 0);
            Pokepic_SetAttr(app->monSprites[app->selectedIndices[0]], 6, 1);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[app->selectedIndices[0] + 2]);
            sub_0201980C(app->unk_474, 0);
            ov81_022425D8(app, 2, 0xf, 5);
            app->subState = 4;
            break;
        case 5:
            PlaySE(0x5dc);
            ov81_02242D88(app->monIconSprites[0], 0);
            ov81_02242D88(app->monIconSprites[1], 0);
            Pokepic_SetAttr(app->monSprites[app->selectedIndices[0]], 6, 1);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[app->selectedIndices[0] + 2]);
            sub_0201980C(app->unk_474, 0);
            ov81_022425D8(app, 0x17, 0xf, 6);
            app->subState = 4;
            break;
        case 6:
            PlaySE(0x5dc);
            ov81_022425C4(app, 2, 0xf, 7);
            app->subState = 4;
            break;
        case 7:
            PlaySE(0x5dc);
            ov81_022425C4(app, 0x12, 0xf, 8);
            app->subState = 4;
            break;
        case 0xfffffffd:
            PlaySE(0x5dc);
            iVar5 = GridInputHandler_GetNextInput(app->inputHandler);
            if (iVar5 - 4U <= 1) {
                ov81_02242D88(app->monIconSprites[0], 0);
                ov81_02242D88(app->monIconSprites[1], 0);
                Pokepic_SetAttr(app->monSprites[app->selectedIndices[0]], 6, 1);
                ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
                ClearWindowTilemapAndScheduleTransfer(&app->windows[app->selectedIndices[0] + 2]);
                sub_0201980C(app->unk_474, 0);
            } else if ((iVar5 != 6) && (iVar5 != 7)) {
                ov81_02242218(app, app->receivableParty, iVar5, 4);
                ScheduleWindowCopyToVram(&app->windows[8]);
                sub_020196E8(app->unk_474, 0, 7, 0);
                ov81_02241D94(app);
                ov81_022424AC(app, iVar5);
            }
            break;
        case 0xfffffffe:
            uVar3 = GridInputHandler_GetNextInput(app->inputHandler);
            if (uVar3 >= 6) {
                PlaySE(0x5dc);
                ov81_022419E0(app);
                ov81_02242E08(app->unk_388, 0);
            } else {
                PlaySE(0x5dc);
                ov81_02241A7C(app);
                ov81_02242E08(app->unk_388, 1);
                ov81_02240F38(app, 1);
                return 1;
            }
            break;
        }
        break;
    case 2:
        YVar4 = YesNoPrompt_HandleInput(app->yesNoPrompt);
        switch (YVar4) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(app->yesNoPrompt);
            ov81_02242694(app, 0);
            ov81_0223FB64(app);
            iVar5 = ov80_02237254(app->challengeType);
            if (iVar5 == 1) {
                uVar1 = ov81_0224086C(app, 2);
                app->printerID = uVar1;
            }
            app->subState = app->subState + 1;
            break;
        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(app->yesNoPrompt);
            ov81_02242694(app, 0);
            ov81_0223FB88(app);
            ov81_02241A38(app);
            ov81_02242E08(app->unk_388, 0);
            app->subState = 1;
            break;
        }
        break;
    case 3:
        return 1;
    case 4:
        ov81_022425EC(app);
        break;
    case 5:
        ov81_02241A7C(app);
        ov81_0223FB3C(app);
        return 1;
    case 6:
        ov81_02241A7C(app);
        ov81_02240F38(app, 1);
        return 1;
    case 7:
        ov81_022419E0(app);
        ov81_02241A7C(app);
        ov81_02241BD0(app->yesNoPrompt, app->bgConfig);
        ov81_02242694(app, 1);
        uVar1 = ov81_0224086C(app, 0xe);
        app->printerID = uVar1;
        app->subState = 2;
        break;
    case 8:
        ov81_022419E0(app);
        ov81_02242E08(app->unk_388, 0);
        app->subState = 1;
    }
    return 0;
}
