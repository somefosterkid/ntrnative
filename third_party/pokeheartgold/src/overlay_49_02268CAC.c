#include "global.h"
#include "overlay_49_unk_struct_ov70_0226_c914_t.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_0226C914_t;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_0226C914_t UnkStruct_ov70_0226C914;

void ov49_0225E82C(UnkStruct_ov70_02261E10 *param0, BOOL param1, fx32 param2);

typedef union {
    struct {
        u32 unk_00;
    } val1;
    struct {
        u32 unk_00;
    } val2;
    struct {
        u32 unk_00;
    } val3;
} UnkStruct_ov70_0226C914_sub1;

void ov49_02268CAC(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268CBC(UnkStruct_ov70_0226C914 *param0, u32 param1);
void ov49_02268CCC(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268CDC(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268CAC(UnkStruct_ov70_0226C914 *param0, u32 param1) {
    ov49_0225E82C(param0->unk_04, 0, FX32_ONE);
}

void ov49_02268CBC(UnkStruct_ov70_0226C914 *param0, u32 param1) {
    ov49_0225E82C(param0->unk_04, 1, FX32_ONE);
}

void ov49_02268CCC(UnkStruct_ov70_0226C914 *param0, u32 param1) {
    ov49_0225E82C(param0->unk_04, 0, FX32_ONE);
}

void ov49_02268CDC(UnkStruct_ov70_0226C914 *param0, u32 param1) {
    ov49_0225E82C(param0->unk_04, 0, FX32_ONE);
}
