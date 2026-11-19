#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system.h"
#include "menu_input_state.h"
#include "overlay_31.h"
#include "yes_no_prompt.h"




void ov31_0225E700(MartSubScreen *param_1) {
    MenuInputState MVar1;
    YesNoPrompt *pYVar2;
    YesNoPromptTemplate YStack_20;

    MI_CpuFill8(&YStack_20, 0, sizeof(YesNoPromptTemplate));
    YStack_20.bgConfig = param_1->bgConfig;
    YStack_20.bgId = 5;
    YStack_20.tileStart = 0x60;
    YStack_20.plttSlot = 8;
    YStack_20.x = 0x1a;
    YStack_20.y = 6;
    MVar1 = MenuInputStateMgr_GetState(&param_1->fieldSystem->menuInputState);
    YStack_20.ignoreTouchFlag = MVar1;
    YStack_20.initialCursorPos = 0;
    YStack_20.shapeParam = 0;
    BgClearTilemapBufferAndCommit(param_1->bgConfig, 5);
    pYVar2 = YesNoPrompt_Create(HEAP_ID_8);
    param_1->yesNoPrompt = pYVar2;
    YesNoPrompt_InitFromTemplate(param_1->yesNoPrompt, &YStack_20);
    return;
}

s32 ov31_0225E774(MartSubScreen *arg0) {
    YesNoPrompt *yesNoPrompt;
    enum YesNoResponse v0;

    yesNoPrompt = arg0->yesNoPrompt;
    if (yesNoPrompt == NULL) {
        return 0;
    }
    v0 = YesNoPrompt_HandleInputForSave(yesNoPrompt);
    switch (v0) {
    case YESNORESPONSE_YES:
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        arg0->yesNoPrompt = NULL;
        arg0->martData->queuedButtonPress = 0;
        return 1;
    case YESNORESPONSE_NO:
        YesNoPrompt_Destroy(arg0->yesNoPrompt);
        arg0->yesNoPrompt = NULL;
        arg0->martData->queuedButtonPress = 1;
        return 1;
    default:
        return 0;
    }
}
