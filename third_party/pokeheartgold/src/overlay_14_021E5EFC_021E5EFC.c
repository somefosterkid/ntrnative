#include "global.h"

#include "bg_window.h"
#include "yes_no_prompt.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E5EFC(u8 *param_1, s8 param_2);

void ov14_021E5EFC(u8 *param_1, s8 param_2) {
    YesNoPromptTemplate YStack_28;

    YStack_28.bgConfig = ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_14;
    YStack_28.bgId = 0;
    YStack_28.tileStart = 0x3ac;
    YStack_28.plttSlot = 8;
    YStack_28.x = 0x19;
    YStack_28.y = 0xc;
    YStack_28.ignoreTouchFlag = 0;
    YStack_28.initialCursorPos = param_2;
    YStack_28.shapeParam = 0;
    YesNoPrompt_InitFromTemplate(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_434, &YStack_28);
    return;
}
