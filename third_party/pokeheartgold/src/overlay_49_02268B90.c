#include "global.h"

#include "error_handling.h"
#include "unk_02005D10.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_49_unk_struct_ov70_0226cbb4.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_0226C914_t;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_0226C914_t UnkStruct_ov70_0226C914;

BOOL ov49_0225E85C(UnkStruct_ov70_02261E10 *param0, u32 param1, BOOL param2, fx32 param3);

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

struct UnkStruct_ov70_0226C914_t {
    const WiFiPlazaSystem *unk_00;
    UnkStruct_ov70_02261E10 *unk_04;
    u8 unk_08;
    u8 unk_09;
    u16 unk_0A;
    UnkStruct_ov70_0226C914_sub1 unk_0C;
};

void ov49_02268D0C(u32 param0, UnkStruct_ov70_0226CBB4 *param1);

extern const u8 ov49_0226A83C[7][2];

void ov49_02268B90(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_0225E6E0(UnkStruct_ov70_02261E10 *param0, fx32 param1);
void ov49_0225E894(UnkStruct_ov70_02261E10 *param0, fx32 param1);
typedef struct {
    u32 unk_00;
    fx32 unk_04;
} UnkStruct_ov70_0226E6FC;
extern const UnkStruct_ov70_0226E6FC ov49_0226A864[3];
void ov49_02268C2C(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268B90(UnkStruct_ov70_0226C914 *param0, u32 param1) {
    int v0;
    u32 v1, v2;
    UnkStruct_ov70_0226CBB4 v3;
    BOOL v4;

    if ((param1 % 6) == 0) {
        v1 = ov49_0226A83C[param0->unk_0C.val2.unk_00][1];
        v2 = ov49_0226A83C[param0->unk_0C.val2.unk_00][0];

        ov49_02268D0C(v1, &v3);

        for (v0 = 0; v0 < v3.unk_04; v0++) {
            v4 = ov49_0225E85C(param0->unk_04, v3.unk_00[v0], 0, (FX32_ONE));
            GF_ASSERT(v4 == 1);
        }

        ov49_02268D0C(v2, &v3);

        for (v0 = 0; v0 < v3.unk_04; v0++) {
            v4 = ov49_0225E85C(param0->unk_04, v3.unk_00[v0], 1, (FX32_ONE));
            GF_ASSERT(v4 == 1);
        }

        param0->unk_0C.val2.unk_00 = (param0->unk_0C.val2.unk_00 + 1) % 7;
    }
}

void ov49_02268C2C(UnkStruct_ov70_0226C914 *param0, u32 param1) {
    if (param0->unk_0C.val3.unk_00 < 3) {
        if (ov49_0226A864[param0->unk_0C.val3.unk_00].unk_00 < param1) {
            ov49_0225E894(param0->unk_04, ov49_0226A864[param0->unk_0C.val3.unk_00].unk_04);
            ov49_0225E6E0(param0->unk_04, ov49_0226A864[param0->unk_0C.val3.unk_00].unk_04);
            PlaySE(1460);

            param0->unk_0C.val3.unk_00++;
        }
    }
}
