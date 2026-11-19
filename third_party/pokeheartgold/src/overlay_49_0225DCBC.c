#include "global.h"

#include "heap.h"
#include "field/overlay_01_021FB878.h"
#include "math_util.h"
#include "overlay_49_unk_struct_ov70_022615e8.h"

typedef struct UnkStruct_ov49_0225DD0C_arg1 {
    u16 unk_0;
    u16 unk_2;
    u8 unk_4;
    u8 unk_5[0xDB];
    u8 unk_E0;
    u8 unk_E1;
    u8 unk_E2;
} UnkStruct_ov49_0225DD0C_arg1;

s32 ov49_0225D574(void *);

void ov49_0225DCBC(void *arg0);

struct UnkStruct_ov70_02260BB8_t;
struct UnkStruct_ov70_02260E8C_t;
struct UnkStruct_ov70_022610B8_t;
typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;
typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;
typedef void (*UnkFuncPtr_ov70_02261C60)(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *);
typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;
void ov49_0225D328(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1, int param2);
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
void sub_02018198(Field3DModelAnimation *anim, fx32 frame);
typedef struct {
    Field3dModel unk_00[2];
    Field3DModelAnimation unk_20[5];
    BOOL unk_84[5];
} UnkStruct_ov70_022615A4;
struct UnkStruct_ov70_02260E8C_t {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    Field3dObject unk_04;
    u32 unk_7C[4];
    fx32 unk_8C[4];
    VecFx32 unk_9C;
    VecFx32 unk_A8;
};
typedef struct {
    Field3dModel unk_00[2];
    void *unk_20[2][3];
    Field3DModelAnimation unk_38[2][4];
} UnkStruct_ov70_022618C8;
struct UnkStruct_ov70_022610B8_t {
    u16 unk_00;
    u16 unk_02;
    Field3dObject unk_04;
    Field3DModelAnimation unk_7C[3];
    u8 unk_B8[3];
    u8 unk_BB;
    u8 unk_BC[3];
    u8 unk_BF;
    fx32 unk_C0[3];
    u8 unk_CC;
    u8 unk_CD[3];
    UnkFuncPtr_ov70_02261C60 unk_D0[3];
    fx32 unk_DC;
    u8 unk_E0;
    u8 unk_E1;
    u8 unk_E2;
    u8 unk_E3;
};
typedef struct {
    Field3dModel unk_00[18];
    void *unk_120[18][3];
} UnkStruct_ov70_02261BB4;
struct UnkStruct_ov70_02260BB8_t {
    UnkStruct_ov70_022615E8 unk_00;
    UnkStruct_ov70_02260E8C *unk_11C;
    UnkStruct_ov70_022610B8 *unk_120;
    u8 unk_124;
    u8 unk_125;
    u8 unk_126;
    u8 unk_127;
    BOOL unk_128;
    UnkStruct_ov70_022615A4 unk_12C;
    UnkStruct_ov70_022618C8 unk_1C4;
    UnkStruct_ov70_02261BB4 unk_29C;
    NNSFndAllocator unk_494;
};
void ov49_0225D57C(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2);
BOOL ov49_0225D5A0(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2);
void ov49_0225D5C8(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2);
BOOL ov49_0225D5E4(fx32 *param0, const Field3DModelAnimation *param1, fx32 param2);
void ov49_0225DD68(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1);

void ov49_0225DCBC(void *arg0) {
    s32 v2;
    void *v1;
    void *v0;
    s32 v3;
    s32 v4;
    void *ptr;
    void *v5;

    v0 = arg0;
    v2 = 0;
    v1 = arg0;
    do {
        v5 = v0;
        v4 = 0;
loop_2:
        ptr = *(void **)((u8 *)v5 + 0x120);
        if (ptr != NULL) {
            Heap_Free(ptr);
            *(void **)((u8 *)v5 + 0x120) = NULL;
        }
        v4 += 1;
        v5 += 4;
        if (v4 < 3) {
            goto loop_2;
        }
        ov49_0225D574(v1);
        v0 += 0xC;
        v1 += 0x10;
        v3 = v2 + 1;
        v2 = v3;
    } while (v3 < 0x12);
}

s32 ov49_022588A0(void *, void *);

s32 sub_020181EC(void *);

void ov49_0225DD0C(u8 *arg0, UnkStruct_ov49_0225DD0C_arg1 *arg1);

void ov49_0225DD0C(u8 *arg0, UnkStruct_ov49_0225DD0C_arg1 *arg1) {
    if ((arg1->unk_0 != 0) && (ov49_022588A0(arg0 + (arg1->unk_2 * 0x10), (void *)&arg1->unk_4) != 0)) {
        if (arg1->unk_E0 == 1) {
            NNS_G3dMdlSetMdlAlphaAll(*(NNSG3dResMdl **)((u8 *)(arg0 + (arg1->unk_2 * 0x10)) + 8), (s32) arg1->unk_E1);
        }
        sub_020181EC((void *)&arg1->unk_4);
        if (arg1->unk_E0 == 1) {
            NNS_G3dMdlSetMdlAlphaAll(*(NNSG3dResMdl **)((u8 *)(arg0 + (arg1->unk_2 * 0x10)) + 8), (s32) arg1->unk_E2);
        }
    }
}

void ov49_0225DD68(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1) {
    int v0;
    BOOL v1;
    UnkFuncPtr_ov70_02261C60 v2;

    for (v0 = 0; v0 < 3; v0++) {
        if (param1->unk_B8[v0]) {
            switch (param1->unk_BC[v0]) {
            case 0:
                ov49_0225D57C(&param1->unk_C0[v0], &param1->unk_7C[v0], param1->unk_DC);
                sub_02018198(&param1->unk_7C[v0], param1->unk_C0[v0]);
                break;
            case 1:
                v1 = ov49_0225D5A0(&param1->unk_C0[v0], &param1->unk_7C[v0], param1->unk_DC);
                sub_02018198(&param1->unk_7C[v0], param1->unk_C0[v0]);

                if (v1 == 1) {
                    v2 = param1->unk_D0[v0];

                    ov49_0225D328(param0, param1, v0);

                    if (v2) {
                        v2(param0, param1);
                    }
                }
                break;
            case 3:
                ov49_0225D5C8(&param1->unk_C0[v0], &param1->unk_7C[v0], param1->unk_DC);
                sub_02018198(&param1->unk_7C[v0], param1->unk_C0[v0]);
                break;
            case 4:
                v1 = ov49_0225D5E4(&param1->unk_C0[v0], &param1->unk_7C[v0], param1->unk_DC);
                sub_02018198(&param1->unk_7C[v0], param1->unk_C0[v0]);

                if (v1 == 1) {
                    v2 = param1->unk_D0[v0];

                    ov49_0225D328(param0, param1, v0);

                    if (v2) {
                        v2(param0, param1);
                    }
                }
                break;
            case 5:
                if (param1->unk_CD[v0] > 0) {
                    param1->unk_CD[v0]--;
                } else {
                    v1 = ov49_0225D5A0(&param1->unk_C0[v0], &param1->unk_7C[v0], param1->unk_DC);

                    if (v1 == 1) {
                        param1->unk_CD[v0] = MTRandom() % param1->unk_CC;
                        param1->unk_C0[v0] = 0;
                    }

                    sub_02018198(&param1->unk_7C[v0], param1->unk_C0[v0]);
                }
                break;
            case 6:
                if (param1->unk_CD[v0] > 0) {
                    param1->unk_CD[v0]--;
                } else {
                    ov49_0225D57C(&param1->unk_C0[v0], &param1->unk_7C[v0], param1->unk_DC);
                    sub_02018198(&param1->unk_7C[v0], param1->unk_C0[v0]);
                }
                break;
            case 2:
            default:
                break;
            }
        }
    }
}
