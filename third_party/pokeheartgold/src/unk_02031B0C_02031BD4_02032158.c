#include "global.h"

#include "string_util.h"

s32 sub_02032158(void *arg0, void *arg1);

s32 sub_02032158(void *arg0, void *arg1) {
    if (*(s32 *)arg0 != *(s32 *)arg1) {
        return 0;
    }
    if (*(u8 *)((u8 *)arg0 + 4) != *(u8 *)((u8 *)arg1 + 4)) {
        return 0;
    }
    if (*(u8 *)((u8 *)arg0 + 6) != *(u8 *)((u8 *)arg1 + 6)) {
        return 0;
    }
    if (*(u8 *)((u8 *)arg0 + 5) != *(u8 *)((u8 *)arg1 + 5)) {
        return 0;
    }
    if (StringNotEqual(arg0 + 8, arg1 + 8) == 0) {
        return 1;
    }
    return 0;
}
