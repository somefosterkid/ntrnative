#include "global.h"

#include "overlay_01_021FC66C.h"

s32 ov01_021FCAE8(void);

s32 ov01_021FC8E8(void *arg0, void *arg1, void *arg2) {
    *(s32 *)((u8 *)arg0 + 0x18) = (s32)(*(s32 *)((u8 *)arg0 + 0x18) - 1);
    if (ov01_021FCAE8() == 1) {
        *(s32 *)((u8 *)arg0 + 0xC) = 6;
        return 1;
    }
    if ((s32) * (s32 *)((u8 *)arg0 + 0x18) > 0) {
        return 0;
    }
    *(s32 *)((u8 *)arg0 + 0xC) = 0xB;
    return 0;
}
