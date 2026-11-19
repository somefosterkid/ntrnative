#include "global.h"

#include "field_types_def.h"

u8 ov01_022055B0(LocalMapObject *arg0);

s32 sub_02023F30(void *);

u32 ov01_021F8FC0(s32 param_1, LocalMapObject *param_2, void *param_3);

u32 ov01_021F8FC0(s32 param_1, LocalMapObject *param_2, void *param_3) {
    u8 uVar1;
    s32 iVar2;

    iVar2 = sub_02023F30(param_3);
    iVar2 = (iVar2 / 0x1000);
    switch (param_1) {
    case 1:
        iVar2 = iVar2 + -0x14;
        break;
    case 2:
        iVar2 = iVar2 + -0x28;
        break;
    case 3:
        iVar2 = iVar2 + -0x3c;
        break;
    case 0:
        break;
    }
    uVar1 = ov01_022055B0(param_2);
    if (uVar1 != 0) {
        if (param_1 == 1) {
            if ((iVar2 < 5) || (iVar2 >= 15)) {
                return 1;
            }
        } else if (iVar2 < 10) {
            return 1;
        }
    } else {
        if (((iVar2 >= 5) && (iVar2 < 10)) || (iVar2 >= 15)) {
            return 1;
        }
    }
    return 0;
}
