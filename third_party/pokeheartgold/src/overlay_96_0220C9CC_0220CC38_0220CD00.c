#include "global.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_96_ov96rival.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
extern u16 ov96_0221CDE8;

extern u8 ov96_0221CDEE;

extern u16 ov96_0221CDF4;

extern u8 ov96_0221CDFE;

s32 ov96_0220CD00(Ov96Rival *arg0);

s32 ov96_0220CD00(Ov96Rival *arg0) {
    s32 var_r4;
    s32 var_r0;
    u16 *var_r1;
    u16 *var_r2;
    u8 temp_r6;

    var_r4 = 0;
    temp_r6 = (MTRandom() % 100U);
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    var_r1 = &ov96_0221CDF4;
    var_r0 = 0;
loop_3:
    if ((u32)((u32)(arg0->stat[arg0->unk_14 >> 0x1E] << 6) >> 0x18) < *var_r1) {
        var_r4 += *(s16 *)(&ov96_0221CDFE + (var_r0 * 2));
    } else {
        var_r0 += 1;
        var_r1 += 1;
        if (var_r0 < 5) {
            goto loop_3;
        }
    }
    var_r2 = &ov96_0221CDE8;
    var_r0 = 0;
loop_7:
    if ((u32)((u32)(arg0->unk_14 << 8) >> 0x18) >= (u32)*var_r2) {
        var_r4 += *(s16 *)(&ov96_0221CDEE + (var_r0 * 2));
    } else {
        var_r0 += 1;
        var_r2 += 1;
        if (var_r0 < 3) {
            goto loop_7;
        }
    }
    if ((s32)temp_r6 < (s32)var_r4) {
        return 1;
    }
    return 0;
}
#else
extern u16 ov96_0221CDE8;

extern u8 ov96_0221CDEE;

extern u16 ov96_0221CDF4;

extern u8 ov96_0221CDFE;

s32 ov96_0220CD00(Ov96Rival *arg0);

s32 ov96_0220CD00(Ov96Rival *arg0) {
    s32 var_r4;
    s32 var_r0;
    u16 *var_r1;
    u16 *var_r2;
    u8 temp_r6;

    var_r4 = 0;
    temp_r6 = (MTRandom() % 100U);
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    var_r1 = &ov96_0221CDF4;
    var_r0 = 0;
loop_3:
    if ((u32)((u32)(*(s32 *)((u8 *)((u8 *)arg0 + (((u32) * (u32 *)((u8 *)arg0 + 0x14) >> 0x1E) * 4)) + 4) << 6) >> 0x18) < *var_r1) {
        var_r4 += *(s16 *)(&ov96_0221CDFE + (var_r0 * 2));
    } else {
        var_r0 += 1;
        var_r1 += 1;
        if (var_r0 < 5) {
            goto loop_3;
        }
    }
    var_r2 = &ov96_0221CDE8;
    var_r0 = 0;
loop_7:
    if ((u32)((u32)(*(s32 *)((u8 *)arg0 + 0x14) << 8) >> 0x18) >= (u32)*var_r2) {
        var_r4 += *(s16 *)(&ov96_0221CDEE + (var_r0 * 2));
    } else {
        var_r0 += 1;
        var_r2 += 1;
        if (var_r0 < 3) {
            goto loop_7;
        }
    }
    if ((s32)temp_r6 < (s32)var_r4) {
        return 1;
    }
    return 0;
}
#endif
