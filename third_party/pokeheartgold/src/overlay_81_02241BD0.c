#include "global.h"

#include "yes_no_prompt.h"
#include "pokepic.h"
#include "unk_02019BA4.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02241CEC(void *);
void ov81_02241C84(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3);

void ov81_02241BD0(YesNoPrompt *yesNoPrompt, BgConfig *bgConfig) {
    YesNoPromptTemplate template;

    template.bgConfig = bgConfig;
    template.bgId = 4;
    template.tileStart = 0x380;
    template.plttSlot = 0xE;
    template.x = 0x19;
    template.y = 0xA;
    template.ignoreTouchFlag = 0;
    template.initialCursorPos = 0;
    template.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(yesNoPrompt, &template);
}

void ov81_02241C0C(BattleFactoryApp *app) {
    u16 v2;
    u32 v0;
    u8 v1;

    v0 = (u32)GridInputHandler_GetNextInput(app->inputHandler);
    if (v0 < 6U) {
        v1 = app->numMonsSelected;
        v2 = 0;
        if ((s32)v1 > 0) {
loop_3:
            if (v0 != *((u16 *)&app->selectedIndices[v2])) {
                v2 += 1;
                if ((s32)v2 < (s32)v1) {
                    goto loop_3;
                }
            }
        }
        if ((v1 != 0) && (v2 != v1)) {
            Pokepic_SetAttr(((Pokepic *)app->monSprites[v1]), 6, 1);
        } else {
            ov81_02241E68(app, v1, v0, 0);
            Pokepic_SetAttr(((Pokepic *)app->monSprites[(app->numMonsSelected)]), 6, 0);
        }
        ov81_02241CEC(app);
    }
}

void ov81_02241C84(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    s32 v0;

    if (arg1 == 3) {
        v0 = (0x50 * arg0) + 0x30;
    } else {
        v0 = (0x70 * arg0) + 0x48;
    }
    *arg2 = v0;
    *arg3 = 0x90;
}
