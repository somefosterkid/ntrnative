#include "global.h"

#include "error_handling.h"

void *ov96_021E94EC(s32 *, u32);

extern void *ov96_0221D9C8;

void ov96_0221A57C(void *arg0, s32 arg1);

void ov96_0221A57C(void *arg0, s32 arg1) {
    s16 (*v1)(u16);
    u8 v2;
    void *v0;

    v1 = (short (*)(unsigned short))(*(void **)(&ov96_0221D9C8 + arg1));
    if (v1 != NULL) {
        v2 = 0;
        do {
            v0 = ov96_021E94EC(arg0, v2);
            *(s16 *)((u8 *)v0 + 0xA) = v1((u16) * (s32 *)((u8 *)v0 + 0xC));
            v2 += 1;
        } while ((u32)v2 < 4U);
        return;
    }
    GF_AssertFail();
}
