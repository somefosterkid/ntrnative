#include "global.h"

#include "error_handling.h"
#include "overlay_96_app_022078b0.h"

s32 ov96_021EAF8C(void *);

s32 ov96_02207990(void *, s32 *, s32, s32);

s32 ov96_022078B0(void *arg0, void *arg1, s8 *arg2);

s32 ov96_022078B0(void *arg0, void *arg1, s8 *arg2) {
    s32 v2[3];
    void *v1;
    s8 *v0;
    s32 v5;
    void *v3;
    void *v4;

    v5 = 0;
    *arg2 = 0;
    v1 = arg1;
    v0 = arg2;
    v4 = arg0;
    v1 = arg1 + 0x58;
loop_1:
    if (*(s32 *)v4 != 0) {
        v2[0] = *(u16 *)((u8 *)(arg0 + (*(s32 *)((u8 *)v4 + 0xC) * 4)) + 0x64) << 0xC;
        v2[1] = *(u16 *)((u8 *)(arg0 + (*(s32 *)((u8 *)v4 + 0xC) * 4)) + 0x66) << 0xC;
        v2[2] = 0;
        if (ov96_02207990(v1, &v2[0], ov96_021EAF8C(*(void **)(arg1 + (*((u8 *)&((Ov96App_022078B0 *)arg1)->unk_0[177]) * 4))) << 0xC, 0x8000) != 0) {
            if ((u32) *((u8 *)&((Ov96App_022078B0 *)arg1)->unk_0[170]) >= 9U) {
                v3 = arg1 + 0xB3;
                if (*(u8 *)(v3 + v5) == 0) {
                    *v0 = 1;
                    *(u8 *)(v3 + v5) = 1;
                }
                return 0;
            }
            *(s8 *)((u8 *)(arg1 + v5) + 0xB3) = 1;
            *(s32 *)(arg0 + (0x14 * v5)) = 0;
            *((u8 *)&((Ov96App_022078B0 *)arg1)->unk_0[170]) = (u8)(*((u8 *)&((Ov96App_022078B0 *)arg1)->unk_0[170]) + 1);
            if (*(u16 *)((u8 *)arg0 + 0xB6) == 0) {
                GF_AssertFail();
            }
            *(u16 *)((u8 *)arg0 + 0xB6) = (u16)(*(u16 *)((u8 *)arg0 + 0xB6) - 1);
            return 1;
        }
        *(s8 *)((u8 *)(arg1 + v5) + 0xB3) = 0;
        goto block_12;
    }
    *(s8 *)((u8 *)(arg1 + v5) + 0xB3) = 0;
block_12:
    v5 += 1;
    v4 += 0x14;
    if (v5 >= 5) {
        return 0;
    }
    goto loop_1;
}
