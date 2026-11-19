#include "global.h"

#include "overlay_88_wifi_plaza_news.h"

void ov88_02258EF8(void);

void ov88_02258EF8(void) {
}

void ov45_0222D594(const UnkStruct_ov66_02231428 *param0, const UnkStruct_ov66_02231428 *param1, UnkStruct_ov66_02231428 *param2);

s32 ov45_0222DCF4(struct UnkStruct_ov66_0223177C_t *arg0, UnkStruct_ov66_02231428 *arg1);

BOOL ov88_02258EFC(WiFiPlazaNewsCountdown *countdown, struct UnkStruct_ov66_0223177C_t *timeSource, const UnkStruct_ov66_02231428 *closeTime);

BOOL ov88_02258EFC(WiFiPlazaNewsCountdown *countdown, struct UnkStruct_ov66_0223177C_t *timeSource, const UnkStruct_ov66_02231428 *closeTime) {
    UnkStruct_ov66_02231428 v2;
    UnkStruct_ov66_02231428 v1;
    UnkStruct_ov66_02231428 v0;
    s32 v3;

    v3 = 0;
    if (ov45_0222DCF4(timeSource, &v2) == 0) {
        return 0;
    }
    if (countdown->waitingForTimeSync == 1) {
        countdown->waitingForTimeSync = 0U;
        v3 = 1;
        countdown->needsRedraw = 1;
    }
    ov45_0222D594(closeTime, &v2, &v1);
    ov45_0222D594(&v1, &countdown->remainingTime, &v0);
    if (((u32)v0.unk_00_val2_unk_02 >= countdown->secondsPerSegment) || (v0.unk_00_val2_unk_01 != 0) || (v0.unk_00_val2_unk_00 != 0)) {
        v3 = 1;
        countdown->needsRedraw = 1;
        countdown->remainingTime.unk_00_val1 = v1.unk_00_val1;
    }
    return v3;
}
