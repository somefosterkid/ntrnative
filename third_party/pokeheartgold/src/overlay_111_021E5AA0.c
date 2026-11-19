#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "yes_no_prompt.h"
#include "overlay_111.h"

void ov111_021E5BE4(BugContestSwapMon *param_1);

void ov111_021E5C54(BugContestSwapMon *param_1);

void ov111_021E5D08(BugContestSwapMon *, u32);

BOOL ov111_021E5AA0(BugContestSwapMon *swapMon);

BOOL ov111_021E5AA0(BugContestSwapMon *swapMon) {
    s32 iVar1;
    YesNoResponse YVar2;
    BOOL BVar3;
    s32 iVar4;
    iVar4 = -1;
    ov111_021E6A2C(swapMon->text);
    iVar1 = ov111_021E6888(swapMon->text);
    if (iVar1 != 0) {
        return 0;
    }
    switch (swapMon->substate) {
    case 0:
        iVar4 = 1;
        ov111_021E5BE4(swapMon);
        swapMon->substate = iVar4;
        break;

    case 1:
        ov111_021E5C54(swapMon);
        swapMon->substate = 2;
        break;

    case 2:
        YVar2 = YesNoPrompt_HandleInput(swapMon->yesNoPrompt);
        switch (YVar2) {
        case YESNORESPONSE_YES:
            YesNoPrompt_Reset(swapMon->yesNoPrompt);
            ManagedSprite_SetDrawFlag(swapMon->cursorSprite, 0);
            ov111_021E6770(swapMon->panels, 0, 1, 0x48);
            ov111_021E6770(swapMon->panels, 1, 1, 0xf2);
            ov111_021E5D08(swapMon, 1);
            swapMon->substate = 3;
            break;

        case YESNORESPONSE_NO:
            YesNoPrompt_Reset(swapMon->yesNoPrompt);
            ManagedSprite_SetDrawFlag(swapMon->cursorSprite, 0);
            ov111_021E6770(swapMon->panels, 0, 1, 0xffffffce);
            ov111_021E6770(swapMon->panels, 1, 1, 0x48);
            ov111_021E5D08(swapMon, 2);
            swapMon->substate = 4;
            break;
        }
        break;

    case 3:
        ov111_021E6784(swapMon->panels);
        iVar1 = ov111_021E67A4(swapMon->panels);
        if (iVar1 != 0) {
            iVar4 = 2;
            swapMon->substate = 5;
        }
        break;

    case 4:
        ov111_021E6784(swapMon->panels);
        iVar1 = ov111_021E67A4(swapMon->panels);
        if (iVar1 != 0) {
            iVar4 = 3;
            swapMon->substate = 5;
        }
        break;

    case 5:
        BVar3 = System_GetTouchNew();
        if ((BVar3 != 0) || ((gSystem.newKeys & 1) != 0)) {
            PlaySE(0x5dc);
            swapMon->substate = 6;
        }
        break;

    case 6:
        return 1;

    default:
        GF_AssertFail();
        break;
    }

    if (iVar4 != -1) {
        ov111_021E68FC(swapMon->text, iVar4);
    }
    return 0;
}
