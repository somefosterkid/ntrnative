#include "global.h"

#include "overlay_27.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"

u32 ov01_021EF00C(struct FieldMenuManager *menu, s32);

// asm table of 7 pointers (the .word symbols), one hitbox list per menu size 2..8
extern TouchscreenHitbox *ov27_0225D49C[];

s32 ov27_0225CA98(Ov27TaskData *data) {
    s32 iVar1;
    u32 uVar2;

    s32 table = data->nMenuWindows - 2;

    iVar1 = TouchscreenHitbox_FindRectAtTouchNew(ov27_0225D49C[table]);
    if (iVar1 != -1) {
        data->selection = iVar1;
        ov27_0225CD18(data, data->nMenuWindows, data->selection);
        PlaySE(0x5dc);
        data->state = 9;
        ov27_0225CD74(data, data->nMenuWindows);
        if (data->callback != NULL) {
            data->callback(data->callbackMenu, data->selection);
        }
    } else {
        if ((gSystem.newKeys & 0x40) != 0) {
            iVar1 = ov27_0225CA68(&data->selection, data->nMenuWindows, 0);
            if (iVar1 != 0) {
                ov27_0225CD18(data, data->nMenuWindows, data->selection);
                PlaySE(0x5dc);
                if (data->callback != NULL) {
                    data->callback(data->callbackMenu, data->selection);
                }
            }
        } else {
            if ((gSystem.newKeys & 0x80) != 0) {
                iVar1 = ov27_0225CA68(&data->selection, data->nMenuWindows, 1);
                if (iVar1 != 0) {
                    ov27_0225CD18(data, data->nMenuWindows, data->selection);
                    PlaySE(0x5dc);
                    if (data->callback != NULL) {
                        data->callback(data->callbackMenu, data->selection);
                    }
                }
            } else {
                if ((gSystem.newKeys & 0x20) != 0) {
                    iVar1 = ov27_0225CA68(&data->selection, data->nMenuWindows, 2);
                    if (iVar1 != 0) {
                        ov27_0225CD18(data, data->nMenuWindows, data->selection);
                        PlaySE(0x5dc);
                        if (data->callback != NULL) {
                            data->callback(data->callbackMenu, data->selection);
                        }
                    }
                } else {
                    if ((gSystem.newKeys & 0x10) != 0) {
                        iVar1 = ov27_0225CA68(&data->selection, data->nMenuWindows, 3);
                        if (iVar1 != 0) {
                            ov27_0225CD18(data, data->nMenuWindows, data->selection);
                            PlaySE(0x5dc);
                            if (data->callback != NULL) {
                                data->callback(data->callbackMenu, data->selection);
                            }
                        }
                    } else {
                        if ((gSystem.newKeys & 1) != 0) {
                            ov27_0225CD74(data, data->nMenuWindows);
                            data->state = 9;
                            PlaySE(0x5dc);
                        } else {
                            if (((gSystem.newKeys & 2) != 0) && (uVar2 = ov01_021EF00C(data->menu, 2), uVar2 != 0)) {
                                data->selection = data->nMenuWindows + -1;
                                ov27_0225CD18(data, data->nMenuWindows, data->selection);
                                ov27_0225CD74(data, data->nMenuWindows);
                                data->state = 9;
                                PlaySE(0x5dc);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
