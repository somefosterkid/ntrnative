#include "global.h"
struct NamePickerData;

#include "sprite.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_32_name_picker_data.h"

void ov32_0225DD24(struct NamePickerData *param_1, s32 param_2);

void ov32_0225DD24(NamePickerData *param_1, s32 param_2) {
    DpadMenuBox *pDVar1;
    VecFx32 VStack_18;

    pDVar1 = (struct DpadMenuBox *)(GridInputHandler_GetDpadBox(param_1->gridInput, param_2));
    VStack_18.x = (u32)pDVar1->left << 0xc;
    VStack_18.y = (u32)pDVar1->top * 0x1000 + 0x100000;
    Sprite_SetMatrix(param_1->sprites[0], &VStack_18);
    if (param_2 == 8) {
        Sprite_SetAnimCtrlSeq(param_1->sprites[0], 1);
        return;
    }
    Sprite_SetAnimCtrlSeq(param_1->sprites[0], 0);
    return;
}
