#include "global.h"

#include "sound_02004A44.h"
#include "unk_02005D10.h"
#include "map_object.h"

void ov02_0224FDF8(u8 *param_1, u16 param_2, s32 param_3, s32 param_4);

s32 ov02_0224FE70(void *, LocalMapObject *, s32);
void ov02_0224FE40(void *arg0, u8 *arg1, LocalMapObject *arg2);

void ov02_0224FDF8(u8 *param_1, u16 param_2, s32 param_3, s32 param_4) {
    s32 iVar1;

    if ((*(u8 *)(param_1 + 5) != 0) && (param_2 != 0)) {
        if (0x94a < param_2) {
            if (param_2 == 0x94b) {
                iVar1 = 0;
            } else {
                iVar1 = 0xb;
            }
            PlayCryEx(iVar1, param_3, 0x1ff, 0x1ff, 0x1ff, param_4);
            return;
        }
        PlaySE((u16)param_2);
    }
    return;
}

void ov02_0224FE40(void *arg0, u8 *arg1, LocalMapObject *object) {
    u8 v0;

    if (*arg1 != 0) {
        v0 = (u8)MapObject_GetFacingDirection(object);
        MapObject_SetFacingDirectionDirect(object, *arg1 - 1);
        ov02_0224FE70(arg0, object, v0);
    }
}
