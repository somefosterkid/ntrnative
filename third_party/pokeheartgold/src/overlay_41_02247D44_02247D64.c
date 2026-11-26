#include "global.h"
struct UnkStruct_ov22_0225A0E4;

#include "yes_no_prompt.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

s32 ov41_02247D34(void *, s32);

void ov41_02247D64(struct UnkStruct_ov22_0225A0E4 *arg0);

void ov41_02247D64(UnkStruct_ov22_0225A0E4 *arg0) {
    YesNoPromptTemplate template;
    MI_CpuFill8(&template, 0U, sizeof(YesNoPromptTemplate));
    template.bgConfig = *((BgConfig **)&arg0->unk_40);
    template.bgId = 3;
    template.tileStart = 0;
    template.plttSlot = 5;
    template.x = 0x19;
    template.y = 4;
    template.ignoreTouchFlag = (u8)(*((s32 *)&arg0->unk_6EC));
    YesNoPrompt_Reset(arg0->unk_6B8);
    YesNoPrompt_InitFromTemplate(arg0->unk_6B8, &template);
    ov41_02247D34(arg0, 2);
    reg_G2_BG0CNT = (u16)(2 | (reg_G2_BG0CNT & (~3)));
    reg_G2_BG1CNT = (u16)(1 | (reg_G2_BG1CNT & (~3)));
    reg_G2_BG2CNT = (u16)(3 | (reg_G2_BG2CNT & (~3)));
    reg_G2_BG3CNT = (u16)(reg_G2_BG3CNT & (~3));
}
