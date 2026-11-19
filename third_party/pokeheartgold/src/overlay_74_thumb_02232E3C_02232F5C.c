#include "global.h"
struct GBAMigrator;

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_74_gbamigrator.h"

void ov74_02232F5C(struct GBAMigrator *param_1);

void ov74_02232F5C(GBAMigrator *param_1) {
    YesNoPromptTemplate YStack_18;

    YStack_18.bgConfig = param_1->bgConfig;
    YStack_18.tileStart = 0x200;
    YStack_18.plttSlot = 8;
    YStack_18.bgId = 0;
    YStack_18.x = 0x19;
    YStack_18.y = 7;
    YStack_18.shapeParam = 0;
    YStack_18.initialCursorPos = 1;
    YesNoPrompt_InitFromTemplate(*((YesNoPrompt * *)&param_1->yesNoTouchMenu), &YStack_18);
    return;
}
