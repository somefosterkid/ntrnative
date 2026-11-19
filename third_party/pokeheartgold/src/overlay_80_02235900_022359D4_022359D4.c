#include "global.h"

#include "frontier/frontier.h"

#include "unk_02035900.h"

u8 sub_02037B5C(s32);

s32 ov80_022359D4(void **arg0);

s32 ov80_022359D4(void **arg0) {
    void *v0;

    v0 = Frontier_GetData(*(void **)(*arg0));
    if ((u32) * (u8 *)((u8 *)v0 + 0x6F) >= 2U) {
        *(u8 *)((u8 *)v0 + 0x6F) = 0U;
        return 1;
    }
    if (sub_02037B5C(sub_0203769C() ^ 1) == 0xAF) {
        *(s8 *)((u8 *)(*arg0) + 0x39) = 1;
        return 1;
    }
    return 0;
}
