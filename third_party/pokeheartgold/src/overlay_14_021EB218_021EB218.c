#include "global.h"

#include "yes_no_prompt.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"

extern u32 ov14_021F7D74;

extern u32 ov14_021F7D78;

u32 ov14_021EB218(u8 *param_1);

u32 ov14_021EB218(u8 *param_1) {
    YesNoResponse YVar1;
    u32 uVar2;

    YVar1 = YesNoPrompt_HandleInput(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_434);
    switch (YVar1) {
    case YESNORESPONSE_YES:
        YesNoPrompt_Reset(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_434);
        uVar2 = (**(u32 (**)(u8 *))(&ov14_021F7D74 + (u32) ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_438 * 2))(param_1);
        return uVar2;
    case YESNORESPONSE_NO:
        YesNoPrompt_Reset(((UnkStruct_ov14_boxapp *)param_1)->sys->unk_434);
        uVar2 = (**(u32 (**)(u8 *))(&ov14_021F7D78 + (u32) ((UnkStruct_ov14_boxapp *)param_1)->sys->unk_438 * 2))(param_1);
        return uVar2;
    default:
        break;
    }
    return 7;
}
