#include "global.h"

#include "error_handling.h"
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

void ov70_0226CB54(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268CBC(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov70_0226CB74(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268CAC(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov70_0226CB94(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268C74(UnkStruct_ov70_0226C914 *param0, u32 param1, u32 param2);

void ov49_02268C74(UnkStruct_ov70_0226C914 *param0, u32 param1, u32 param2) {
    extern void (*const ov49_0226A87C[])(UnkStruct_ov70_0226C914 *, u32);

    GF_ASSERT(param1 < 6);

    if (param1 < 6) {
        memset(&param0->pad_C, 0, sizeof(UnkStruct_ov70_0226C914_sub1));

        ov49_0226A87C[param1](param0, param2);
        param0->unk_08 = param1;
    }
}
