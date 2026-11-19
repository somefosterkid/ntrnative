#include "global.h"

#include "error_handling.h"

extern s32 _02103754;

extern s32 _021037B8;

s32 sub_02087FD4(s32 arg0);

s32 sub_02087FD4(s32 arg0) {
    if (arg0 >= 7) {
        if (arg0 >= 7) {
            GF_AssertFail();
        }
        return *(s32 *)((u8 *)(&_02103754) + 0x64);
    }
    return *(s32 *)(((u8 *)&_021037B8) + (0x24 * arg0));
}
