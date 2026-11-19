#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96rival.h"

void ov96_0220CF50(Ov96Rival *, s32, PokeathlonCourseData *);

void ov96_0220C9CC(Ov96RivalSet *arg0);

void ov96_0220D200(u32 *, s32, PokeathlonCourseData *);
void ov96_0220C9F4(Ov96Rival *arg0, s32 arg1, PokeathlonCourseData *arg2);
s32 ov96_0220CAC4(void *);
s32 ov96_0220CBC0(void *);
s32 ov96_0220CBEC(void *);
s32 ov96_0220CC18(Ov96Rival *);
s32 ov96_0220D07C(void *);
s32 ov96_0220D2AC(void *, u32, int);
s32 ov96_0220CA28(Ov96Rival *arg0);

void ov96_0220C9CC(Ov96RivalSet *arg0) {
    u32 v1;
    Ov96Rival *v0;
    unsigned int new_var;
    v1 = 0;
    new_var = 0U;
    if (arg0->count > new_var) {
        v0 = &arg0->rivals[0];
        do {
            ov96_0220CF50(v0, 0, arg0->course);
            v1 += 1;
            v0++;
        } while (v1 < arg0->count);
    }
}

void ov96_0220C9F4(Ov96Rival *dest, s32 arg1, PokeathlonCourseData *arg2) {
    MI_CpuFill8(dest, 0U, sizeof(Ov96Rival));
    dest->course = arg2;
    dest->unk_18 = (s32)((dest->unk_18 & 0x3FFFFFFF) | (arg1 << 0x1E));
    dest->stat[3] = 0x64;
    ov96_0220D200(&dest->stat[0], arg1, arg2);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov96_0220CA28(Ov96Rival *arg0) {
    s32 v1;
    s32 v2;
    u32 v0;

    v2 = 0;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    v0 = (u32)(arg0->unk_14 * 4) >> 0x1A;
    switch (v0) {
    case 0:
        v2 = ov96_0220CAC4(arg0);
        break;
    case 1:
        v2 = ov96_0220CBC0(arg0);
        break;
    case 2:
        v2 = ov96_0220CC18(arg0);
        break;
    case 3:
        v2 = ov96_0220CBEC(arg0);
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (((u32)(arg0->unk_14 * 4) >> 0x1A) != 3) {
        ov96_0220D07C(arg0);
    }
    ov96_0220D2AC(&arg0->stat[0], (u32)(arg0->unk_18 << 0x15) >> 0x15, (u8)((u32)arg0->unk_14 >> 0x1E));
    v1 = arg0->unk_18;
    arg0->unk_18 = (s32)((v1 & 0xFFFFF800) | ((0xFFFFF800U >> 0x15) & (((u32)(v1 << 0x15) >> 0x15) + 1)));
    return v2;
}
#else
s32 ov96_0220CA28(Ov96Rival *arg0) {
    s32 v1;
    s32 v2;
    u32 v0;

    v2 = 0;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    v0 = (u32)(*(u32 *)((u8 *)arg0 + 0x14) * 4) >> 0x1A;
    switch (v0) {
    case 0:
        v2 = ov96_0220CAC4(arg0);
        break;
    case 1:
        v2 = ov96_0220CBC0(arg0);
        break;
    case 2:
        v2 = ov96_0220CC18(arg0);
        break;
    case 3:
        v2 = ov96_0220CBEC(arg0);
        break;
    default:
        GF_AssertFail();
        break;
    }
    if (((u32)(*(u32 *)((u8 *)arg0 + 0x14) * 4) >> 0x1A) != 3) {
        ov96_0220D07C(arg0);
    }
    ov96_0220D2AC((u8 *)arg0 + 4, (u32)(*(s32 *)((u8 *)arg0 + 0x18) << 0x15) >> 0x15, (u8)((u32) * (u32 *)((u8 *)arg0 + 0x14) >> 0x1E));
    v1 = *(s32 *)((u8 *)arg0 + 0x18);
    *(s32 *)((u8 *)arg0 + 0x18) = (s32)((v1 & 0xFFFFF800) | ((0xFFFFF800U >> 0x15) & (((u32)(v1 << 0x15) >> 0x15) + 1)));
    return v2;
}
#endif
