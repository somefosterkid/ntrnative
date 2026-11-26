#include "global.h"

#include "yes_no_prompt.h"

void ov65_0221F890(void);

void ov65_0221F8B4(void);

YesNoPrompt *ov65_0221FC08(BgConfig *);

s32 ov65_0221FC54(BgConfig *param_1, void *param_2, s32 *param_3, YesNoPrompt **param_4, s32 param_5);

s32 ov65_0221FC54(BgConfig *param_1, void *param_2, s32 *param_3, YesNoPrompt **param_4, s32 param_5) {
    YesNoPrompt *uVar1;
    YesNoResponse YVar2;

    YVar2 = (YesNoResponse)(~YESNORESPONSE_WAIT);
    switch (*param_3) {
    case 0:
        uVar1 = ov65_0221FC08(param_1);
        *param_4 = uVar1;
        if (param_5 != 0) {
            ov65_0221F890();
        }
        *param_3 = *param_3 + 1;
        break;
    case 1:
        YVar2 = YesNoPrompt_HandleInput(*param_4);
        if (YVar2 != YESNORESPONSE_WAIT) {
            YesNoPrompt_Destroy(*param_4);
            ov65_0221F8B4();
            *param_3 = 0;
        }
        break;
    }
    return YVar2;
}
