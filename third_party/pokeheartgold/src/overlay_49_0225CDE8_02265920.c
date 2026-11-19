#include "global.h"

#include "error_handling.h"

s32 ov49_02265958(u8 *);

s32 ov49_02266AF0(void *, u8 *);

s32 ov49_02265920(void *arg0, u8 *arg1);

s32 ov49_02265920(void *arg0, u8 *arg1) {
    if (ov49_02265958(arg1) == 0) {
        return 0;
    }
    if ((u32)*arg1 >= 0x1BU) {
        GF_AssertFail();
    }
    return ov49_02266AF0(arg0, arg1);
}
