#include "global.h"

#include "constants/heap.h"

#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_box_funcs.h"

void ov14_021F6B10(void *arg0);
void ov14_021F6B28(s32 *, s32);
void ov14_021F6A44(s32 *param_1);

extern u8 ov14_021F8B10;

void ov14_021F6A44(s32 *param_1) {
    GridInputHandler *pGVar1;
    s32 iVar2;
    s32 unaff_r5;
    switch (*((u32 *)((*param_1) + 8))) {
    case 0:
        unaff_r5 = 0;
        break;

    case 1:
        unaff_r5 = 2;
        break;

    case 2:
        unaff_r5 = 3;
        break;

    case 3:
        unaff_r5 = 6;
    }

    ov14_021F6B10(param_1);
    iVar2 = unaff_r5 * 0xc;
    pGVar1 = GridInputHandler_Create(*((TouchscreenHitbox **)((u8 *)&ov14_021F8B10 + iVar2)), *((DpadMenuBox **)((u8 *)&ov14_021F8B10 + 4 + iVar2)), *((GridCallbacks **)((u8 *)&ov14_021F8B10 + 8 + iVar2)), param_1, 1, (u8)param_1[0xb], HEAP_ID_10);
    *((GridInputHandler **)(param_1[0xd] + 0x2c)) = pGVar1;
    ov14_021F6B28(param_1, param_1[0xb]);
    return;
}
