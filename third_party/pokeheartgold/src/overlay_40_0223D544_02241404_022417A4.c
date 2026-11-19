#include "global.h"

#include "overlay_40.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_0223077C(void *, void *, s32, s32);

s32 sub_020879E0(void *, s32);

s32 sub_02087A08(void *, s32, s32);

s32 ov40_022417A4(void *arg0) {
    ov40_0223077C(arg0, *(void **)((u8 *)arg0 + 0x6F0), 0x80, 0x60);
    sub_020879E0(*(void **)((u8 *)arg0 + 0x6F0), 1);
    sub_02087A08(*(void **)((u8 *)arg0 + 0x6F0), 0x18, 0x18);
    ov40_0222BF80(arg0, 3);
    return 0;
}
