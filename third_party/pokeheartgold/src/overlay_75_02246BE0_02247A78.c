#include "global.h"

#include "overlay_75.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"
#include "overlay_75_unk_struct_ov98_02247704.h"
#include "unk_0203170C.h"
#include "yes_no_prompt.h"

s32 ov75_02246BCC(void *, s32, s32);

s32 ov75_02246BE0(void *, s8);

s32 ov75_02247A78(UnkStruct_ov98_02247704 *arg0) {
    ov75_02246BCC(arg0->unk_00, 4, 3);
    ov75_02246BE0(arg0->unk_00, 0x26);
    arg0->unk_08 = 0x1D;
    return 0;
}

s32 ov75_02247854(void *, s32, s32);

YesNoPrompt *ov75_02247890(BgConfig *, s32, s32);

s32 ov75_022494CC(void *, void *, s32, s32, u16);

s32 ov75_02249534(s32);

s32 ov75_02247A98(UnkStruct_ov98_02247704 *arg0) {
    enum YesNoResponse v1;
    u32 v0;

    if (ov75_02249534(arg0->unk_44) == 1) {
        return 0;
    }
    v0 = arg0->unk_94;
    switch (v0) {
    case 0:
        ov75_022494CC(arg0, arg0->unk_34, 0x19, 1, 0xF0F);
        arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        break;
    case 1:
        arg0->unk_88 = ov75_02247890(arg0->unk_04, 0x234, 0);
        arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        break;
    case 2:
        v1 = YesNoPrompt_HandleInput(arg0->unk_88);
        if (v1 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->unk_88);
            ov75_022494CC(arg0, arg0->unk_34, 0x1A, 1, 0xF0F);
            sub_02031780(arg0->unk_00->saveData, 0U, 1U);
            arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        } else if (v1 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->unk_88);
            ov75_022494CC(arg0, arg0->unk_34, 0x1B, 1, 0xF0F);
            sub_02031780(arg0->unk_00->saveData, 0U, 0U);
            arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        }
        break;
    case 3:
        ov75_02247854(arg0, 0x19, 0);
        break;
    }
    return 0;
}

s32 ov75_02247B98(UnkStruct_ov98_02247704 *arg0) {
    enum YesNoResponse v1;
    u32 v0;

    if (ov75_02249534(arg0->unk_44) == 1) {
        return 0;
    }
    v0 = arg0->unk_94;
    switch (v0) {
    case 0:
        ov75_022494CC(arg0, arg0->unk_34, 0x1C, 1, 0xF0F);
        arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        break;
    case 1:
        arg0->unk_88 = ov75_02247890(arg0->unk_04, 0x234, 1);
        arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        break;
    case 2:
        v1 = YesNoPrompt_HandleInput(arg0->unk_88);
        if (v1 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->unk_88);
            ov75_022494CC(arg0, arg0->unk_34, 0x1D, 1, 0xF0F);
            sub_02031734(arg0->unk_00->saveData);
            arg0->unk_94 = (u32)(arg0->unk_94 + 1);
        } else if (v1 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->unk_88);
            arg0->unk_08 = 0;
        }
        break;
    case 3:
        ov75_02247854(arg0, 0x19, 0);
        break;
    }
    return 0;
}
