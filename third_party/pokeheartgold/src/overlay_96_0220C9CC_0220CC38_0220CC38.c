#include "global.h"

#include "error_handling.h"
#include "math_util.h"

extern u32 ov96_0221CE14[];

extern s32 ov96_0221CE24[];

extern u32 ov96_0221CE34[];

extern s32 ov96_0221CE48[];

typedef struct {
    u32 unk_0 : 18;
    u32 key : 8;
    u32 unk_26 : 6;
} UnkStruct_ov96_0220CC38_Entry4;

typedef struct {
    u32 unk_00;
    UnkStruct_ov96_0220CC38_Entry4 entries[4];
    u32 unk_14_0 : 16;
    u32 key : 8;
    u32 unk_14_24 : 6;
    u32 idx : 2;
} UnkStruct_ov96_0220CC38_Work;

s32 ov96_0220CC38(UnkStruct_ov96_0220CC38_Work *arg0);

s32 ov96_0220CC38(UnkStruct_ov96_0220CC38_Work *arg0) {
    s32 i;
    s32 sum;
    u8 roll;
    sum = 0;
    roll = (u8)(MTRandom() % 100U);
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    for (i = 0; i < 5; i++) {
        if (arg0->entries[arg0->idx].key >= ov96_0221CE34[i]) {
            sum += ov96_0221CE48[i];
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (arg0->key >= ov96_0221CE14[i]) {
            sum += ov96_0221CE24[i];
            break;
        }
    }
    if (roll < sum) {
        return 1;
    }
    return 0;
}
