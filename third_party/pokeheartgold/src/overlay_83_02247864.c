#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"

#include "overlay_83_battle_castle_yes_no_prompt.h"

void ov83_02247988(u16 *, u8 *);

void ov83_02247864(BattleCastleYesNoPrompt *param_1, BgConfig *param_2);

void ov83_022478B4(BattleCastleYesNoPrompt *arg0);

void ov83_02247864(BattleCastleYesNoPrompt *param_1, BgConfig *param_2) {
    u16 uStack_26;
    u8 auStack_28[2];
    YesNoPromptTemplate *new_var;
    YesNoPromptTemplate YStack_24;
    param_1->bgConfig = param_2;
    ov83_02247988(&uStack_26, auStack_28);
    YStack_24.bgConfig = param_2;
    YStack_24.bgId = 0;
    YStack_24.tileStart = (s32)uStack_26;
    YStack_24.plttSlot = 0xb;
    YStack_24.x = 0x19;
    YStack_24.y = 0xa;
    YStack_24.ignoreTouchFlag = 0;
    YStack_24.initialCursorPos = 0;
    YStack_24.shapeParam = 0;
    new_var = &YStack_24;
    YesNoPrompt_InitFromTemplate(param_1->prompt, new_var);
    param_1->isActive = TRUE;
    return;
}

void ov83_022478B4(BattleCastleYesNoPrompt *arg0) {
    if (arg0->isActive != 0) {
        YesNoPrompt_Reset(arg0->prompt);
        BgCommitTilemapBufferToVram(arg0->bgConfig, 0U);
        arg0->isActive = 0;
    }
}
