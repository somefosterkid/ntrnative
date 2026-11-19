#include "global.h"

#include "constants/heap.h"
#include "overlay_49_unk_struct_ov70_022629_fc.h"

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_022610B8_t;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

u8 ov49_0225D3BC(const UnkStruct_ov70_02260BB8 *param0, const UnkStruct_ov70_022610B8 *param1, int param2);

void ov49_0225D494(UnkStruct_ov70_022610B8 *param0, BOOL param1);

void ov49_0225D4E8(UnkStruct_ov70_022610B8 *param0);

void ov49_0225D4F0(UnkStruct_ov70_022610B8 *param0, fx32 param1, fx32 param2, fx32 param3);

void ov49_0225EC28(UnkStruct_ov70_022629FC *param0);

BOOL ov49_0225EC30(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1);

void ov49_0225EE10(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1);

void ov49_0225ED98(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1);

void ov49_0225ED98(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1) {
    switch (param0->unk_04) {
    case 0:
        break;
    case 1:
        ov49_0225EE10(param0, param1);
        param0->unk_04 = 2;
        break;
    case 2:
        if (ov49_0225D3BC(param1, param0->unk_00, 0) == 0) {
            param0->unk_04 = 3;
        }
        break;
    case 3:
        ov49_0225EC28(param0);
        param0->unk_04 = 4;
        break;
    case 4:
        if (ov49_0225EC30(param0, param1) == 1) {
            ov49_0225D494(param0->unk_00, 0);
            ov49_0225D4E8(param0->unk_00);
            ov49_0225D4F0(param0->unk_00, ((fx32)0x0000000000001000L), ((fx32)0x0000000000001000L), ((fx32)0x0000000000001000L));

            param0->unk_04 = 0;
        }
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }
}
