#include "global.h"

typedef struct {
    void *unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C_0 : 3;
    u32 unk_0C_3 : 2;
    u32 unk_0C_5 : 2;
    u32 unk_0C_7 : 4;
    u32 unk_0C_11 : 8;
    u32 unk_0C_19 : 13;
    u8 unk_10;
} UnkStruct_ov96_02210980;

void ov96_0221090C(UnkStruct_ov96_02210980 *param0);

void ov96_02210980(UnkStruct_ov96_02210980 *param0, void *param1, int param2);

void ov96_02210980(UnkStruct_ov96_02210980 *param0, void *param1, int param2) {
    u8 v0 = param2 / 3;
    u8 v1 = param2 % 3;

    param0->unk_00 = param1;
    param0->unk_0C_3 = v0;
    param0->unk_0C_5 = v1;
    param0->unk_0C_11 = v0 * 2 + v1 * 8;
    param0->unk_10 = v0 * 2 + v1 * 8;
    ov96_0221090C(param0);
}
