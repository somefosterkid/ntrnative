#include "global.h"

#include "list_menu_items.h"
#include "overlay_27.h"
#include "sprite.h"

ListMenuItem *ov01_021EEF58(struct FieldMenuManager *menu);

extern const u16 ov27_0225D118[][2];
extern const u16 ov27_0225D11A[][2];

s32 ov27_0225CC90(Ov27TaskData *data) {
    if (Sprite_IsAnimated(data->cursor) == 0) {
        Sprite_SetDrawFlag(data->cursor, 0);
        ov27_0225C6F8(data);
        data->state = 0xA;
    }
    return 0;
}

s32 ov27_0225CCBC(Ov27TaskData *data) {
    ListMenuItem *iVar1;

    iVar1 = ov01_021EEF58(data->menu);
    *data->result = (s16)iVar1[data->selection].value;
    data->state = 1;
    return 0;
}

void ov27_0225CCE0(Ov27TaskData *data, s32 param_2) {
    VecFx32 VStack_10;

    VStack_10.x = (u32)ov27_0225D118[param_2][0] << 0xc;
    VStack_10.y = (ov27_0225D11A[param_2][0] + 0x100) * 0x1000;
    VStack_10.z = 0;
    Sprite_SetMatrix(data->cursor, &VStack_10);
    return;
}
