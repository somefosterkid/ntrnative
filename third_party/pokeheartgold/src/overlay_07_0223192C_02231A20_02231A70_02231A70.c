#include "global.h"

typedef struct {
    u32 a;
    u32 b;
    u32 c;
} UnkStruct_ov07_02231A70_Trio;

typedef struct {
    UnkStruct_ov07_02231A70_Trio v[8][24];
} UnkStruct_ov07_02231A70_Table;

extern UnkStruct_ov07_02231A70_Table ov07_0223687C;

void ov07_02231A70(u32 param_1, UnkStruct_ov07_02231A70_Trio *param_2, u32 param_3, s32 param_4, s32 param_5);

void ov07_02231A70(u32 param_1, UnkStruct_ov07_02231A70_Trio *param_2, u32 param_3, s32 param_4, s32 param_5) {
    UnkStruct_ov07_02231A70_Table tbl = ov07_0223687C;
    UnkStruct_ov07_02231A70_Trio *row = tbl.v[param_1];
    u32 a, b, c;
    param_4 += param_5 * 2;
    c = row[param_4].c;
    b = row[param_4].b;
    a = row[param_4].a;
    param_2->a = a;
    param_2->b = b;
    param_2->c = c;
}
