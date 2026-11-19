#include "global.h"

#include "sprite.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_29_number_entry_data.h"

void ov29_0225DB9C(struct NumberEntryData *param_1, u32 param_2);

void ov29_0225DB9C(NumberEntryData *param_1, u32 param_2) {
    DpadMenuBox *pDVar1;
    VecFx32 VStack_18;
    pDVar1 = (struct DpadMenuBox *)GridInputHandler_GetDpadBox(param_1->gridInput, param_2);
    VStack_18.x = ((u32)pDVar1->left) << 0xc;
    VStack_18.y = (((u32)pDVar1->top) * 0x1000) + 0x100000;
    Sprite_SetMatrix(param_1->sprites[0], &VStack_18);
    if (param_2 <= (10 - 1)) {
        Sprite_SetAnimCtrlSeq(param_1->sprites[0], 1);
        return;
    }
    Sprite_SetAnimCtrlSeq(param_1->sprites[0], 2);
    return;
}
