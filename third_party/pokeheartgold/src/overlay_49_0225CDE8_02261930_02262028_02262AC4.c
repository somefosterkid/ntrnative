#include "global.h"

#include "overlay_49.h"
#include "unk_02034354.h"
#include "unk_02037C94.h"

s32 ov49_0225A018(void *, s32);

s32 ov49_0225EF88(void *arg0);

s32 ov49_0225EF8C(void *, s32);

BOOL ov49_02262AC4(struct UnkStruct_ov70_02263344_t * arg0, struct UnkStruct_ov70_0225DEE8_t * arg1, u32 arg2) {
    s32 v0;

    v0 = ov49_0225EF88(arg0);
    switch (v0) {
    case 0:
        sub_020398D4(0, 0);
        if (sub_020392A0() == 0) {
            sub_020343E4();
            sub_0203986C();
        }
        ov49_0225A018(arg1, 1);
        ov49_0225EF8C(arg0, 1);
    default:
        break;
    case 1:
        if (sub_020392A0() == 1) {
            return 1;
        }
        break;
    }
    return 0;
}
