#include "global.h"

#include "error_handling.h"
#include "camera.h"

s32 ov45_0222A5E8(s32, s32);

s32 ov49_02259FE8(void *);

void *ov49_02259FEC(Camera const *camera);

s32 ov49_0225A010(void *);

u8 ov49_0225A02C(void *);

s32 ov49_0225EF98(s32, s32, void *, s32);

extern s32 ov49_02269BE0;

void ov49_0225F1A8(void *arg0);

void ov49_0225F1A8(void *arg0) {
    s32 v2;
    s32 v1;
    void *v0;

    v0 = ov49_02259FEC(arg0);
    v2 = ov49_0225A010(arg0);
    v1 = ov49_0225A02C(arg0);
    ov45_0222A5E8(ov49_02259FE8(arg0), 1);
    if ((u32) * (u16 *)((u8 *)v0 + 6) >= 4U) {
        GF_AssertFail();
    }
    ov49_0225EF98(v2, v1, &ov49_02269BE0 + *(u16 *)((u8 *)v0 + 6) * 2, 0);
}
