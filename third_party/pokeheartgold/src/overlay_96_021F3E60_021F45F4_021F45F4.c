#include "global.h"

#include "error_handling.h"
#include "unk_02005D10.h"

s32 ov96_021F43EC(void *);

s32 ov96_021F46BC(void *, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_021F47F0(void *arg0);
#else
s32 ov96_021F47F0();
#endif

s32 ov96_021F480C(void *, void *);

s32 ov96_021F48A8(void *, void *, s32);

s32 ov96_021F4A60(void *, void *);

void ov96_021F45F4(void *arg0, s32 arg1, s32 arg2);

void ov96_021F45F4(void *arg0, s32 arg1, s32 arg2) {
    s32 (*temp_r1)(void *);
    u8 temp_r7;
    s32 *new_var;
    void *temp_r4;
    temp_r4 = arg0 + 0x68;
    new_var = (s32 *)(((u8 *)arg0) + 0x68);
    temp_r7 = ov96_021F46BC(temp_r4, arg2);
    if ((arg1 == 0) && ((*((s32 *)(((u8 *)temp_r4) + 4))) != 0)) {
        ov96_021F43EC(arg0);
    }
    if ((*new_var) != 0) {
        if (arg1 != 0) {
            if ((*((s32 *)(((u8 *)temp_r4) + 4))) == 0) {
                *((s32 *)(((u8 *)temp_r4) + 4)) = 1;
#if defined(SDK_PORT) || defined(NONMATCHING)
                if (ov96_021F47F0(temp_r4) != 1) {
#else
                if (ov96_021F47F0(temp_r4, 1) != 1) {
#endif
                    GF_AssertFail();
                }
                ov96_021F480C(temp_r4, *((void **)(((u8 *)arg0) + 8)));
            }
#if defined(SDK_PORT) || defined(NONMATCHING)
        } else if ((temp_r7 != 0xC) && (ov96_021F47F0(temp_r4) != 0)) {
#else
        } else if ((temp_r7 != 0xC) && (ov96_021F47F0(temp_r4, 2) != 0)) {
#endif
            PlaySE(0x89EU);
            ov96_021F48A8(temp_r4, *((void **)(((u8 *)arg0) + 8)), temp_r7);
        }
    } else {
        ov96_021F4A60(temp_r4, *((void **)(((u8 *)arg0) + 8)));
    }
    temp_r1 = *((s32(**)(void *))(((u8 *)temp_r4) + 0x14));
    if (temp_r1 != 0L) {
        temp_r1(arg0);
        return;
    }
    GF_AssertFail();
}
