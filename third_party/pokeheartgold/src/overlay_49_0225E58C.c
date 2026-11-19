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

u16 ov49_0225D1C0(const UnkStruct_ov70_022610B8 *param0);

BOOL ov49_0225E9D0(const UnkStruct_ov70_022610B8 *param0, u8 param1, u8 param2);

void ov49_0225EA70(UnkStruct_ov70_02261E10 *param0, UnkStruct_ov70_022610B8 *param1);

void ov49_0225EAB4(UnkStruct_ov70_02261E10 *param0, UnkStruct_ov70_022610B8 *param1);

void ov49_0225E58C(UnkStruct_ov70_02261E10 *param0, u8 param1, u8 param2);

s32 ov49_0225ECD4(void *, s32);
void ov49_0225E6E0(UnkStruct_ov70_02261E10 *arg0, s32 arg1);

void ov49_0225E714(UnkStruct_ov70_02261E10 *param0);
typedef void (*UnkFuncPtr_ov70_02261C60)(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *);
void ov49_0225D224(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1, int param2, int param3, UnkFuncPtr_ov70_02261C60 param4);
void ov49_0225EA10(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1);
void ov49_0225EA40(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1);
void ov49_0225E760(UnkStruct_ov70_02261E10 *param0, int param1);
BOOL ov49_0225E824(const UnkStruct_ov70_02261E10 *param0);
s32 ov49_0225E85C(UnkStruct_ov70_02261E10 *arg0, u32 arg1, s32 arg2, s32 arg3);
void ov49_0225E82C(UnkStruct_ov70_02261E10 *param0, BOOL param1, fx32 param2);
s32 ov49_0225EB54(void *, UnkStruct_ov70_02260BB8 *, s32);
s32 ov49_0225EB84(void *);
void ov49_0225E894(UnkStruct_ov70_02261E10 *arg0, s32 arg1);

void ov49_0225E58C(UnkStruct_ov70_02261E10 *param0, u8 param1, u8 param2) {
    int v0;
    BOOL v1[2];
    u32 v2;

    for (v0 = 0; v0 < param0->unk_612; v0++) {
        v1[0] = ov49_0225E9D0(param0->unk_08[v0], param1 - 1, param2 - 1);
        v1[1] = ov49_0225E9D0(param0->unk_08[v0], param1, param2 - 1);

        if ((v1[0] == 1) || (v1[1] == 1)) {
            v2 = ov49_0225D1C0(param0->unk_08[v0]);

            switch (v2) {
            case 1:
            case 2:
            case 3:
            case 4:
                if (param0->unk_611 == 4) {
                    ov49_0225EAB4(param0, param0->unk_08[v0]);
                } else {
                    ov49_0225EA70(param0, param0->unk_08[v0]);
                }
                break;
            default:
                break;
            }
        }
    }
}

void ov49_0225D214(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1, u8 param2, int param3);

void ov49_0225D328(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1, int param2);

void ov49_0225D494(UnkStruct_ov70_022610B8 *param0, BOOL param1);

void ov49_0225D4A0(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1, u32 param2);

void ov49_0225D4C8(UnkStruct_ov70_022610B8 *param0, fx32 param1);

void ov49_0225E624(UnkStruct_ov70_02261E10 *param0, BOOL param1);

void ov49_0225E624(UnkStruct_ov70_02261E10 *param0, BOOL param1) {
    int v0;
    u32 v1;

    for (v0 = 0; v0 < param0->unk_612; v0++) {
        v1 = ov49_0225D1C0(param0->unk_08[v0]);

        switch (v1) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (param1 == 1) {
                ov49_0225D4A0(param0->unk_04, param0->unk_08[v0], 1 << 0);
            }
            break;
        case 15:
            if (param1 == 1) {
                ov49_0225D4C8(param0->unk_08[v0], ((fx32)0x0000000000000800L));
                ov49_0225D214(param0->unk_04, param0->unk_08[v0], 0, 6);
                ov49_0225D214(param0->unk_04, param0->unk_08[v0], 1, 6);
                ov49_0225D214(param0->unk_04, param0->unk_08[v0], 2, 6);
                ov49_0225D494(param0->unk_08[v0], 1);
            } else {
                ov49_0225D328(param0->unk_04, param0->unk_08[v0], 0);
                ov49_0225D328(param0->unk_04, param0->unk_08[v0], 1);
                ov49_0225D328(param0->unk_04, param0->unk_08[v0], 2);
                ov49_0225D494(param0->unk_08[v0], 0);
            }
            break;
        default:
            break;
        }
    }
}

void ov49_0225E6E0(UnkStruct_ov70_02261E10 *arg0, s32 arg1) {
    s32 v0;

    v0 = 0;
    if ((s32)arg0->unk_22C.unk_3DF > 0) {
        do {
            ov49_0225ECD4(&arg0->unk_22C.unk_2BC[v0], arg1);
            v0 += 1;
        } while (v0 < (s32)arg0->unk_22C.unk_3DF);
    }
}

void ov49_0225E714(UnkStruct_ov70_02261E10 *param0) {
    int v0;
    u32 v1;

    for (v0 = 0; v0 < param0->unk_612; v0++) {
        v1 = ov49_0225D1C0(param0->unk_08[v0]);

        switch (v1) {
        case 1:
        case 2:
        case 3:
        case 4:
            ov49_0225D4A0(param0->unk_04, param0->unk_08[v0], 1 << 0);
            break;
        default:
            break;
        }
    }
}

void ov49_0225E760(UnkStruct_ov70_02261E10 *param0, int param1) {
    int v0;
    u32 v1;

    for (v0 = 0; v0 < param0->unk_612; v0++) {
        v1 = ov49_0225D1C0(param0->unk_08[v0]);

        switch (v1) {
        case 11:
        case 12:
            switch (param1) {
            case 0:
                ov49_0225D214(param0->unk_04, param0->unk_08[v0], 0, 2);
                param0->unk_22C.unk_18 = 0;
                break;
            case 1:
                ov49_0225D224(param0->unk_04, param0->unk_08[v0], 0, 1, ov49_0225EA10);
                param0->unk_22C.unk_18 = 1;
                break;
            case 2:
                ov49_0225D224(param0->unk_04, param0->unk_08[v0], 0, 4, ov49_0225EA40);
                ov49_0225D328(param0->unk_04, param0->unk_08[v0], 1);
                param0->unk_22C.unk_18 = 0;
                break;
            case 3:
                ov49_0225D214(param0->unk_04, param0->unk_08[v0], 1, 0);
                ov49_0225D328(param0->unk_04, param0->unk_08[v0], 0);
                param0->unk_22C.unk_18 = 1;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

BOOL ov49_0225E824(const UnkStruct_ov70_02261E10 *param0) {
    return param0->unk_22C.unk_18;
}

void ov49_0225E82C(UnkStruct_ov70_02261E10 *param0, BOOL param1, fx32 param2) {
    int v0;

    for (v0 = 0; v0 < param0->unk_22C.unk_3DC; v0++) {
        ov49_0225E85C(param0, v0, param1, param2);
    }
}

s32 ov49_0225E85C(UnkStruct_ov70_02261E10 *arg0, u32 arg1, s32 arg2, s32 arg3) {
    void *v3;

    if (arg1 < (u32)arg0->unk_22C.unk_3DC) {
        v3 = &arg0->unk_22C.unk_1C[arg1];
        if (arg2 != 0) {
            ov49_0225EB54(v3, arg0->unk_04, arg3);
        } else {
            ov49_0225EB84(v3);
        }
        return 1;
    }
    return 0;
}

void ov49_0225E894(UnkStruct_ov70_02261E10 *arg0, s32 arg1) {
    s32 v2;

    v2 = 0;
    if ((s32)arg0->unk_22C.unk_3DD > 0) {
        do {
            ov49_0225ECD4(&arg0->unk_22C.unk_13C[v2], arg1);
            v2 += 1;
        } while (v2 < (s32)arg0->unk_22C.unk_3DD);
    }
}
