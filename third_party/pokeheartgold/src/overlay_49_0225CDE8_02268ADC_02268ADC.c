#include "global.h"
#include "overlay_49_unk_struct_ov70_0226_c914_t.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_0226C914_t;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_0226C914_t UnkStruct_ov70_0226C914;

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

void ov70_0226C9AC(UnkStruct_ov70_0226C914 *param0, u32 param1);
void ov49_02268B04(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov70_0226C9B0(UnkStruct_ov70_0226C914 *param0, u32 param1);
void ov49_02268B08(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov70_0226C9B4(UnkStruct_ov70_0226C914 *param0, u32 param1);
void ov49_02268B0C(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov70_0226CA38(UnkStruct_ov70_0226C914 *param0, u32 param1);
void ov49_02268B90(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov70_0226CAD4(UnkStruct_ov70_0226C914 *param0, u32 param1);
void ov49_02268C2C(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268ADC(UnkStruct_ov70_0226C914 *param0, u32 param1, u32 param2);

void ov49_02268ADC(UnkStruct_ov70_0226C914 *param0, u32 param1, u32 param2) {
    static void (*const v0[])(UnkStruct_ov70_0226C914 *, u32) = {
        ov49_02268B04,
        ov49_02268B04,
        ov49_02268B08,
        ov49_02268B0C,
        ov49_02268B90,
        ov49_02268C2C
    };

    ((param1 < 6) ? (void)0 : GF_AssertFail());

    if (param1 < 6) {
        v0[param1](param0, param2);
    }
}
