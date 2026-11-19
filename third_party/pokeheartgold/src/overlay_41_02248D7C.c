#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"

struct UnkStruct_ov22_02259560_t;
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov22_0225886C;
static void ov41_02248DC8(UnkStruct_ov22_0225886C *param0);
void ov41_02249AF4(UnkStruct_ov22_02259560 *param0, int param1, int param2);
void ov41_02249B44(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);
typedef struct UnkStruct_ov22_02258A48_t {
    void *unk_00;
    void (*unk_04)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_08)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_0C)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_10)(struct UnkStruct_ov22_02258A48_t *param0);
    s16 unk_14;
    s16 unk_16;
    u8 unk_18;
    u8 padding[3];
} UnkStruct_ov22_02258A48;
void ov41_02248E80(UnkStruct_ov22_02258A48 *param0);

UnkStruct_ov22_0225886C *ov41_02248D7C(UnkStruct_ov22_0225886C *param0, int param1);
void ov41_02248DA4(UnkStruct_ov22_0225886C *param0, int param1);
static void ov41_02248DC8(UnkStruct_ov22_0225886C *param0);
int ov41_02248E10(int param0, int param1);
void ov41_02248E28(UnkStruct_ov22_02258A48 *param0);

UnkStruct_ov22_0225886C *ov41_02248D7C(UnkStruct_ov22_0225886C *param0, int param1) {
    int v0;

    for (v0 = 0; v0 < param1; v0++) {
        if (param0[v0].unk_00 == 0L) {
            return param0 + v0;
        }
    }

    return 0L;
}

void ov41_02248DA4(UnkStruct_ov22_0225886C *param0, int param1) {
    int v0;

    for (v0 = 0; v0 < param1; v0++) {
        if (param0[v0].unk_00) {
            ov41_02248DC8(param0 + v0);
        }
    }
}

static void ov41_02248DC8(UnkStruct_ov22_0225886C *param0) {
    int v0, v1;

    ov41_02249B44(param0->unk_00, &v0, &v1);
    v1 += param0->unk_08;
    ov41_02249AF4(param0->unk_00, v0, v1);

    param0->unk_04--;

    if (param0->unk_04 <= 0) {
        memset(param0, 0, sizeof(UnkStruct_ov22_0225886C));
    }
}

int ov41_02248E10(int param0, int param1) {
    param0 += param1 - (param0 % param1);
    return param0 / param1;
}

void ov41_02248E28(UnkStruct_ov22_02258A48 *param0) {
    memset(param0, 0, sizeof(UnkStruct_ov22_02258A48));

    param0->unk_04 = ov41_02248E80;
    param0->unk_08 = ov41_02248E80;
    param0->unk_0C = ov41_02248E80;
}
