#include "global.h"

#include "error_handling.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

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

extern const u8 ov49_0226A8B4[17];

void ov49_02268B0C(UnkStruct_ov70_0226C914 *param0, u32 param1);

void ov49_02268B0C(UnkStruct_ov70_0226C914 *param0, u32 param1) {
    BOOL v0;
    u32 v1;
    BOOL v2;
    BOOL v3;
    u32 v4;
    static const u32 v5[4][2] = {
        { 0, 2 },
        { 1, 3 },
        { 2, 0 },
        { 3, 1 },
    };

    if ((param1 % 14) == 0) {
        v1 = 0;

        do {
            v3 = 0;

            if ((v1 % 4) == v5[param0->unk_0C.val1.unk_00][0]) {
                v2 = 1;
                v3 = 1;
            } else if ((v1 % 4) == v5[param0->unk_0C.val1.unk_00][1]) {
                v2 = 0;
                v3 = 1;
            }

            if (v3 == 1) {
                if (v1 < NELEMS(ov49_0226A8B4)) {
                    v4 = ov49_0226A8B4[v1];
                    v0 = ov49_0225E85C(param0->unk_04, v4, v2, (FX32_ONE));
                    GF_ASSERT(v0 == 1);
                } else {
                    v0 = 0;
                }
            } else {
                v0 = 1;
            }

            v1++;
        } while (v0 == 1);

        param0->unk_0C.val1.unk_00 = (param0->unk_0C.val1.unk_00 + 1) % 4;
    }
}
