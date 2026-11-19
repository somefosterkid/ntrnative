#include "global.h"

#include "math_util.h"

u32 ov96_02215E68(void *, s32);

static s32 ov96_0221A3AC(void *arg0);

static s32 ov96_0221A3AC(void *arg0) {
    s32 v2;
    u32 v0;
    u32 v1;

    v1 = MTRandom() % 100U;
    v2 = 0;
    v0 = ov96_02215E68(*(void **)arg0, (u8)((u32)(*(s32 *)((u8 *)arg0 + 8) << 0x1E) >> 0x1E));
    if (v0 < 0xAU) {
        v2 = 0x64;
    } else if (v0 < 0x14U) {
        v2 = 0x46;
    } else if (v0 < 0x1EU) {
        v2 = 0x32;
    } else if (v0 < 0x28U) {
        v2 = 0x1E;
    } else if (v0 < 0x32U) {
        v2 = 0xA;
    }
    if ((s32)v1 < v2) {
        return 1;
    }
    return 0;
}

s32 ov96_02215DBC(void *, s32);

void *ov96_02215DD4(void *, s32);

s32 ov96_02215E94(void *, s32);

s32 ov96_02215EB0(void *, s32);

s32 ov96_02215ECC(void *, s32);

s32 ov96_02215F80(void *, s32, s32 *);

s32 ov96_0221A08C(void *);

s32 ov96_0221A314(void *);

void ov96_0221A400(void *arg0);

void ov96_0221A400(void *arg0) {
    s32 v0;
    s32 v4;
    s32 v5;
    s32 v6;
    s8 v3;
    void *v1;
    void *v2;

    if (ov96_02215E94(*(void **)arg0, (u8)((u32)(*(s32 *)((u8 *)arg0 + 8) << 0x1E) >> 0x1E)) != 0) {
        return;
    }
    v4 = *(s32 *)((u8 *)arg0 + 8);
    if ((((u32)(v4 << 0x1A) >> 0x1F) != 0) && (((u32)(v4 << 0x1B) >> 0x1F) == 0)) {
        ov96_02215DBC(*(void **)arg0, (u8)((u32)(v4 << 0x1E) >> 0x1E));
        if ((ov96_02215DBC(*(void **)arg0, *(u8 *)((u8 *)arg0 + 7)) == 3) || (ov96_02215E94(*(void **)arg0, *(u8 *)((u8 *)arg0 + 7)) != 0) || (ov96_02215EB0(*(void **)arg0, (u8)((u32)(*(s32 *)((u8 *)arg0 + 8) << 0x1E) >> 0x1E)) != 0)) {
            v1 = ov96_02215DD4(*(void **)arg0, (u8)((u32)(*(s32 *)((u8 *)arg0 + 8) << 0x1E) >> 0x1E));
            *(s16 *)(&v0) = (s16)((s32) * (s32 *)v1 >> 0xC);
            *(s16 *)((u8 *)(&v0) + 2) = (s16)((s32) * (s32 *)((u8 *)v1 + 4) >> 0xC);
            ov96_02215F80(*(void **)arg0, (u8)((u32)(*(s32 *)((u8 *)arg0 + 8) << 0x1E) >> 0x1E), &v0);
            *(s32 *)((u8 *)arg0 + 8) = (s32)(*(s32 *)((u8 *)arg0 + 8) & ~0x20);
        } else {
            v2 = ov96_02215DD4(*(void **)arg0, *(u8 *)((u8 *)arg0 + 7));
            *(s16 *)(&v0) = (s16)((s32) * (s32 *)v2 >> 0xC);
            *(s16 *)((u8 *)(&v0) + 2) = (s16)((s32) * (s32 *)((u8 *)v2 + 4) >> 0xC);
            ov96_02215F80(*(void **)arg0, (u8)((u32)(*(s32 *)((u8 *)arg0 + 8) << 0x1E) >> 0x1E), &v0);
        }
    }
    v5 = *(s32 *)((u8 *)arg0 + 8);
    if (((u32)(v5 << 0x1B) >> 0x1F) != 0) {
        if (ov96_02215ECC(*(void **)arg0, (u8)((u32)(v5 << 0x1E) >> 0x1E)) != 0) {
            v6 = ov96_02215DBC(*(void **)arg0, (u8)((u32)(*(s32 *)((u8 *)arg0 + 8) << 0x1E) >> 0x1E));
            ov96_0221A314(arg0);
            if (v6 == 3) {
                *(s32 *)((u8 *)arg0 + 8) = (s32)(*(s32 *)((u8 *)arg0 + 8) & ~0x10);
            }
        }
    } else {
        *(s8 *)((u8 *)arg0 + 4) = (s8)(*(s8 *)((u8 *)arg0 + 4) - 1);
        v3 = *(s8 *)((u8 *)arg0 + 4);
        if ((s32)v3 <= 0) {
            if ((s32)v3 < 0) {
                *(s8 *)((u8 *)arg0 + 4) = 0;
            }
            *(s8 *)((u8 *)arg0 + 6) = (s8)(*(s8 *)((u8 *)arg0 + 6) - 1);
            if ((s32) * (s8 *)((u8 *)arg0 + 6) <= 0) {
                *(s8 *)((u8 *)arg0 + 6) = 3;
                if (ov96_02215ECC(*(void **)arg0, (u8)((u32)(*(s32 *)((u8 *)arg0 + 8) << 0x1E) >> 0x1E)) != 0) {
                    ov96_0221A08C(arg0);
                    *(s8 *)((u8 *)arg0 + 5) = (s8)(*(s8 *)((u8 *)arg0 + 5) - 1);
                    if ((s32) * (s8 *)((u8 *)arg0 + 5) <= 0) {
                        if (ov96_0221A3AC(arg0) != 0) {
                            *(s32 *)((u8 *)arg0 + 8) = (s32)(0x10 | *(s32 *)((u8 *)arg0 + 8));
                        }
                        *(s8 *)((u8 *)arg0 + 5) = 0;
                    }
                }
            }
        }
    }
}
