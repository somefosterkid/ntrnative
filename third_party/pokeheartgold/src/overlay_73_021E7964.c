#include "global.h"

#include "error_handling.h"
#include "constants/heap.h"
#include "save.h"

typedef u16 charcode_t;

typedef struct RecordMixedRNG {
    charcode_t groupName[7 + 1];
    charcode_t playerName[7 + 1];
    u8 gender;
    u8 language;
    u16 dummy;
    u32 seed;
    u32 rand;
} RecordMixedRNG;

void ov73_021E7964(RecordMixedRNG *param0, const RecordMixedRNG *param1);

s32 ov73_021E7A74(void);
s32 ov73_021E7A7C(void);
s32 ov73_021E7A84(void);

extern const u8 *const ov73_021EA664[6];
BOOL ov73_021E7A8C(int param0, int param1);

void ov73_021E7964(RecordMixedRNG *param0, const RecordMixedRNG *param1) {
    int v0;

    for (v0 = 5; v0 > 2; v0--) {
        param0[v0] = param0[v0 - 1];
    }

    param0[2] = *param1;
}

void ov73_021E79A8(RecordMixedRNG *param0, int param1);

void ov73_021E79A8(RecordMixedRNG *param0, int param1) {
    int v0;
    RecordMixedRNG *v1;

    GF_ASSERT(2 <= param1 && param1 <= 5);

    for (v0 = param1 + 1; v0 <= 5; v0++) {
        param0[v0 - 1] = param0[v0];
    }

    v1 = &param0[5];

    v1->groupName[0] = 0xffff;
    v1->playerName[0] = 0xffff;
}

BOOL sub_0202C8C4(const RecordMixedRNG *entry);

BOOL sub_0202C8E4(const RecordMixedRNG *entry1, const RecordMixedRNG *entry2);

void ov73_021E79F4(int param0, int param1, RecordMixedRNG *param2, const void **param3);

void ov73_021E79F4(int param0, int param1, RecordMixedRNG *param2, const void **param3) {
    int v0, v1;
    RecordMixedRNG *v2;

    for (v0 = 0; v0 < param0; v0++) {
        v2 = (RecordMixedRNG *)param3[v0];

        if (v0 == param1) {
            continue;
        }

        if (v2 == NULL) {
            continue;
        }

        if (sub_0202C8C4(v2) == 1) {
            continue;
        }

        if (sub_0202C8E4(v2, &param2[0])) {
            continue;
        }

        for (v1 = 0; v1 < 4; v1++) {
            if (sub_0202C8E4(v2, &param2[2 + v1])) {
                ov73_021E79A8(param2, 2 + v1);
            }
        }

        ov73_021E7964(param2, v2);
    }
}

s32 ov73_021E7A74(void) {
    return 0x120;
}

s32 ov73_021E7A7C(void) {
    return 0x120;
}

s32 ov73_021E7A84(void) {
    return 0x120;
}

BOOL ov73_021E7A8C(int param0, int param1) {
    int v0;

    for (v0 = 0; ov73_021EA664[param0][v0] != 0xff; v0++) {
        if (ov73_021EA664[param0][v0] == param1) {
            return 1;
        }
    }

    return 0;
}
