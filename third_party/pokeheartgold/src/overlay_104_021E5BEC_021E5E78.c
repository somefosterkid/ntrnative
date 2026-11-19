#include "global.h"

s32 ov104_021E5E78(void *arg0, s32 arg1);

s32 ov104_021E5E78(void *arg0, s32 arg1) {
    void *temp_r2;

    if ((s32)(arg1 + 1) >= 3) {
        if ((*(s32 *)(*(void **)((u8 *)arg0 + 0x64)) + 0x1000) == 0xCD000) {
            return 1;
        }
    } else {
        temp_r2 = *(void **)((u8 *)arg0 + 0x64);
        if ((*(s32 *)temp_r2 + 0x1000) == (*(u16 *)((u8 *)(*(void **)((u8 *)temp_r2 + 8)) + 4) << 0xC)) {
            return 1;
        }
    }
    return 0;
}
