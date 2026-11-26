#include "global.h"

#include "constants/heap.h"
#include "overlay_49_unk_struct_ov70_02261_e10_t.h"
#include "overlay_49_unk_struct_ov70_022629_fc.h"

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_02260E8C_t;

struct UnkStruct_ov70_022610B8_t;

struct UnkStruct_ov70_02261E10_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov49_0225D214(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1, u8 param2, int param3);
#else
void ov49_0225D214(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1, int param2, int param3);
#endif

typedef struct {
    UnkStruct_ov70_022610B8 *unk_00[3];
    UnkStruct_ov70_022610B8 *unk_0C[3];
    BOOL unk_18;
    UnkStruct_ov70_022629FC unk_1C[24];
    UnkStruct_ov70_022629FC unk_13C[24];
    UnkStruct_ov70_022610B8 *unk_25C[24];
    UnkStruct_ov70_022629FC unk_2BC[24];
    u8 unk_3DC;
    u8 unk_3DD;
    u8 unk_3DE;
    u8 unk_3DF;
    u32 unk_3E0;
} UnkStruct_ov70_02261E10_sub1;

void ov49_0225EAE0(UnkStruct_ov70_02261E10 *param0, UnkStruct_ov70_022610B8 *param1);

void ov49_0225EB00(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_022610B8 *param1);
void ov49_0225EBA8(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1);
void ov49_0225EBE4(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1);
void ov49_0225EC28(UnkStruct_ov70_022629FC *param0);
BOOL ov49_0225EC30(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1);
void ov49_0225EB08(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1);
void ov49_0225EB54(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1, fx32 param2);

void ov49_0225EAE0(UnkStruct_ov70_02261E10 *param0, UnkStruct_ov70_022610B8 *param1) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        ov49_0225D214(param0->unk_04, param1, v0, 1);
    }
}

void ov49_0225EB00(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_022610B8 *param1) {
    param0->unk_00 = param1;
    param0->unk_04 = 0;
}

void ov49_0225EB08(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1) {
    switch (param0->unk_04) {
    case 0:
        break;
    case 1:
        ov49_0225EBA8(param0, param1);
        param0->unk_04 = 2;
        break;
    case 2:
        break;
    case 3:
        ov49_0225EC28(param0);
        param0->unk_04 = 4;
        break;
    case 4:
        if (ov49_0225EC30(param0, param1) == 1) {
            ov49_0225EBE4(param0, param1);
            param0->unk_04 = 0;
        }
        break;
    default:
        break;
    }
}

void ov49_0225EB54(UnkStruct_ov70_022629FC *param0, UnkStruct_ov70_02260BB8 *param1, fx32 param2) {
    if ((param0->unk_04 == 1) || (param0->unk_04 == 2)) {
        return;
    }

    if ((param0->unk_04 == 3) || (param0->unk_04 == 4)) {
        ov49_0225EBE4(param0, param1);
    }

    param0->unk_04 = 1;
    param0->unk_08 = param2;
}
