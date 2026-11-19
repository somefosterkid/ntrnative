#include "global.h"

#include "bg_window.h"
#include "unk_020932E0.h"
#include "yes_no_prompt.h"

void sub_0209389C(UnkStruct_020932E0 *param_1) {
    YesNoPromptTemplate YStack_20;
    unsigned char new_var;
    MI_CpuFill8(&YStack_20, 0, sizeof(YesNoPromptTemplate));
    YStack_20.bgConfig = param_1->unk_0;
    YStack_20.bgId = 0;
    YStack_20.tileStart = 0x200;
    YStack_20.plttSlot = 8;
    YStack_20.x = 0x19;
    YStack_20.y = 0xf;
    YesNoPrompt_InitFromTemplate(param_1->unk_8, &YStack_20);
    new_var = param_1->unk_D;
    if (new_var == 1) {
        sub_020950F8(&param_1->unk_4660, 0);
        sub_0209501C(param_1, 3, FONT_MESSAGE);
    } else {
        sub_0209501C(param_1, 2, FONT_MESSAGE);
    }
    sub_02095780();
    param_1->unk_14 = 2;
    return;
}
