#include "global.h"

#include "overlay_40.h"
#include "touch_hitbox_controller.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_0223A034_v0 {
    u8 unk_0[0xC];
    s32 unk_C;
    u8 unk_10[0x4];
    s32 unk_14;
    u8 unk_18[0xC4];
    TouchHitboxController *unk_DC;
    u8 unk_E0[0x668];
    void *unk_748;
} UnkStruct_ov40_0223A034_v0;

s32 ov40_0222F09C(void *, void *, s32, s32, String *);

s32 ov40_0222F6D0(void *, s32);

s32 ov40_0222FA88(void *);

s32 ov40_0223A034(UnkStruct_0208C06C *arg0) {
    UnkStruct_ov40_0223A034_v0 *v0;

    v0 = arg0->unk_860;
    ov40_0222FA88(&arg0->unk_14.unk_46C);
    ov40_0222F6D0(&arg0->unk_14.unk_48C, arg0->unk_14.unk_46C.unk_10);
    ov40_0222F09C(&arg0->unk_14.unk_48C, arg0, v0->unk_C, v0->unk_14, v0->unk_748);
    TouchHitboxController_IsTriggered(v0->unk_DC);
    return 0;
}
