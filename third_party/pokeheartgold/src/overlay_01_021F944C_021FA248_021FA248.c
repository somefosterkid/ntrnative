#include "global.h"

#include "error_handling.h"
#include "overlay_01_021F944C.h"

extern const BillboardSpriteInfo ov01_02207318[];

extern u16 ov01_022074A8;

const BillboardSpriteInfo *sub_021FA248(s32 arg0) {
    u16 *v1;
    u32 v0;

    v1 = &ov01_022074A8;
loop_1:
    if (*(u16 *)v1 == arg0) {
        v0 = (u32)(*(u16 *)((u8 *)v1 + 4) << 0x10) >> 0x1A;
        if (v0 == 0xFF) {
            if (v0 == 0xFF) {
                GF_AssertFail();
            }
            return &ov01_02207318[0];
        }
        return &ov01_02207318[v0];
    }
    v1 += 3;
    if (*v1 == 0xFFFF) {
        GF_AssertFail();
        return NULL;
    }
    goto loop_1;
}
