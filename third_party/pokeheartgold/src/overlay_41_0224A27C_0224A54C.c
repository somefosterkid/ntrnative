#include "global.h"

#include "error_handling.h"
#include "touch_hitbox_controller.h"

s32 ov41_0224A448(void *);

s32 ov41_0224A4EC(void *);

void ov41_0224A54C(void *arg0);

void ov41_0224A54C(void *arg0) {
    if (*(TouchHitboxController **)((u8 *)arg0 + 0x68) == NULL) {
        GF_AssertFail();
    }
    if (ov41_0224A4EC(arg0) == 0) {
        if (**(s32 **)((u8 *)arg0 + 0x80) == 0) {
            ov41_0224A448(arg0);
            return;
        }
        TouchHitboxController_IsTriggered(*(TouchHitboxController **)((u8 *)arg0 + 0x68));
    }
}

s32 ov41_0224A1EC(u8 *, s32, s32);

s32 ov41_0224A8B0(u8 *, s32);

s32 ov41_0224A8D4(u8 *, s32);

void ov41_0224A580(u8 *arg0);

void ov41_0224A580(u8 *arg0) {
    ov41_0224A8B0(arg0 + 0x40, 3);
    ov41_0224A8D4(arg0 + 0x40, 3);
    ov41_0224A1EC(arg0, 2, 3);
}

s32 ov41_0224A9BC(void *, s32, s32);

s32 ov41_0224A9F8(u8 *, s32, s32);

void ov41_0224A5A4(u8 *arg0, s32 arg1, s32 arg2);

void ov41_0224A5A4(u8 *arg0, s32 arg1, s32 arg2) {
    u8 *v0;
    u8 *v1;
    s32 v2;
    u8 *v3;

    v0 = arg0;
    v2 = 0;
    v3 = arg0;
    do {
        ov41_0224A9BC(v3, arg1, arg2);
        v2 += 1;
        v3 += 0x10;
    } while (v2 < 4);
    v1 = v0 + 0x40;
    v0 = v1;
    ov41_0224A9F8(v1, arg1, arg2);
}

s32 ov41_0224A238(void *, s32, void *, s32);

void ov41_0224A5D4(u8 *arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4);

void ov41_0224A5D4(u8 *arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    if (arg0 == 0) {
        GF_AssertFail();
    }
    if (arg1 < 4) {
        ov41_0224A238(arg0 + (arg1 * 0x10), arg2, arg3, arg4);
        return;
    }
    ov41_0224A238(arg0 + 0x40, arg2, arg3, arg4);
}
