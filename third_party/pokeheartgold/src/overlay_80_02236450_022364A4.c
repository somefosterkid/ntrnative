#include "global.h"

#include "error_handling.h"
#include "scrcmd_9.h"
#include "unk_0204A3F4.h"

void _s32_div_f(u16, u16);

void ov80_02229EF4(s16 *, u32, u32);

u32 ov80_02236734(FrontierFieldSystem *, void *, u16, u32, u32, u8, u8, BOOL, u32);

u32 ov80_02236AF0(u8);

BOOL ov80_022364A4(FrontierFieldSystem *param_1, u16 *param_2, u8 param_3, void *param_4, u8 param_5, u16 *param_6, u16 *param_7, u32 *param_8, u32 param_9);

BOOL ov80_022364A4(FrontierFieldSystem *param_1, u16 *param_2, u8 param_3, void *param_4, u8 param_5, u16 *param_6, u16 *param_7, u32 *param_8, u32 param_9) {
    u32 ids[4];
    u32 results[4];
    u16 b[8];
    u16 a[8];
    s32 i;
    u8 ivs;
    u32 seed;
    u32 id;
    s32 retries;
    BOOL flag;
    u32 n;
    u32 count;
    u16 r;
    u16 r1, r2;

    flag = FALSE;
    if (param_5 > 4) {
        GF_AssertFail();
    }
    n = param_5;
    count = 0;
    retries = 0;
    if (n != 0) {
        do {
            r = FrontierFieldSystem_0204B510(param_1);
            id = param_2[((u8)(r % param_2[1])) + 2];
            ov80_02229EF4((s16 *)a, id, ov80_02236AF0(param_1->towerMode));
            for (i = 0; i < (s32)count; i++) {
                ov80_02229EF4((s16 *)b, ids[i], ov80_02236AF0(param_1->towerMode));
                if (b[0] == a[0]) {
                    break;
                }
            }
            if (i != count) {
                continue;
            }
            if (param_6 != NULL) {
                for (i = 0; i < (s32)n; i++) {
                    if (a[0] == param_6[i]) {
                        break;
                    }
                }
                if (i != n) {
                    continue;
                }
            }
            if (retries < 50) {
                for (i = 0; i < (s32)count; i++) {
                    ov80_02229EF4((s16 *)b, ids[i], ov80_02236AF0(param_1->towerMode));
                    if (b[6] != 0 && b[6] == a[6]) {
                        break;
                    }
                }
                if (i != count) {
                    retries++;
                    continue;
                }
                if (param_7 != NULL) {
                    for (i = 0; i < (s32)n; i++) {
                        if (a[6] == param_7[i] && param_7[i] != 0) {
                            break;
                        }
                    }
                    if (i != n) {
                        retries++;
                        continue;
                    }
                }
            }
            ids[count] = id;
            count++;
        } while (count != n);
    }
    ivs = GetFrontierTrainerIVs(param_3);
    r1 = FrontierFieldSystem_0204B510(param_1);
    r2 = FrontierFieldSystem_0204B510(param_1);
    seed = r1 | (r2 << 16);
    if (retries >= 50) {
        flag = TRUE;
    }
    for (i = 0; i < (s32)count; i++, param_4 += 0x38) {
        results[i] = ov80_02236734(param_1, param_4, (u16)ids[i], seed, 0, ivs, (u8)i, flag, param_9);
    }
    if (param_8 == NULL) {
        return flag;
    }
    *param_8 = seed;
    for (i = 0; i < 2; i++) {
        *(u16 *)((u8 *)param_8 + i * 2 + 4) = ids[i];
        *(u32 *)((u8 *)param_8 + i * 4 + 8) = results[i];
    }
    return flag;
}
