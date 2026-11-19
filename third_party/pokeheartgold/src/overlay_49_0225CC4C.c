#include "global.h"

struct UnkStruct_ov70_02260BB8_t;
struct UnkStruct_ov70_02260E8C_t;
struct UnkStruct_ov70_022610B8_t;
typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;
typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;
typedef void (*UnkFuncPtr_ov70_02261C60)(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *);
typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
typedef struct Easy3DModel {
    void *data;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} Easy3DModel;
typedef struct Easy3DAnim {
    void *data;
    void *anim;
    NNSG3dAnmObj *animObj;
    fx32 frame;
    BOOL dataBorrowed;
} Easy3DAnim;
typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;
void *Heap_Alloc(u32 heapID, u32 size);
typedef struct {
    BOOL unk_00;
    Easy3DObject unk_04[2];
    u32 unk_F4[5];
    fx32 unk_108[5];
} UnkStruct_ov70_022615E8;
typedef struct {
    Easy3DModel unk_00[2];
    Easy3DAnim unk_20[5];
    BOOL unk_84[5];
} UnkStruct_ov70_022615A4;
struct UnkStruct_ov70_02260E8C_t {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    Easy3DObject unk_04;
    u32 unk_7C[4];
    fx32 unk_8C[4];
    VecFx32 unk_9C;
    VecFx32 unk_A8;
};
typedef struct {
    Easy3DModel unk_00[2];
    void *unk_20[2][3];
    Easy3DAnim unk_38[2][4];
} UnkStruct_ov70_022618C8;
struct UnkStruct_ov70_022610B8_t {
    u16 unk_00;
    u16 unk_02;
    Easy3DObject unk_04;
    Easy3DAnim unk_7C[3];
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
    Easy3DModel unk_00[18];
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
void ov49_0225CE88(UnkStruct_ov70_02260BB8 *param0);
void Heap_Free(void *ptr);
void ov49_0225D7B8(UnkStruct_ov70_022615E8 *param0, UnkStruct_ov70_022615A4 *param1);
void ov49_0225DA70(UnkStruct_ov70_02260E8C *param0, UnkStruct_ov70_022618C8 *param1);
void ov49_0225DD68(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1);
void ov49_0225D804(UnkStruct_ov70_022615E8 *param0, UnkStruct_ov70_022615A4 *param1);
void ov49_0225DAFC(UnkStruct_ov70_02260E8C *param0, UnkStruct_ov70_022618C8 *param1);
void ov49_0225DD0C(UnkStruct_ov70_02261BB4 *param0, UnkStruct_ov70_022610B8 *param1);

UnkStruct_ov70_02260BB8 *ov49_0225CC4C(u32 param0, u32 param1, u32 heapID, u32 heapID2);
void ov49_0225CCC0(UnkStruct_ov70_02260BB8 *param0);
void ov49_0225CCF0(UnkStruct_ov70_02260BB8 *param0);
void ov49_0225CD58(UnkStruct_ov70_02260BB8 *param0);

UnkStruct_ov70_02260BB8 *ov49_0225CC4C(u32 param0, u32 param1, u32 heapID, u32 heapID2) {
    UnkStruct_ov70_02260BB8 *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov70_02260BB8));
    memset(v0, 0, sizeof(UnkStruct_ov70_02260BB8));

    v0->unk_11C = Heap_Alloc(heapID, sizeof(UnkStruct_ov70_02260E8C) * param0);
    v0->unk_120 = Heap_Alloc(heapID, sizeof(UnkStruct_ov70_022610B8) * param1);

    memset(v0->unk_11C, 0, sizeof(UnkStruct_ov70_02260E8C) * param0);
    memset(v0->unk_120, 0, sizeof(UnkStruct_ov70_022610B8) * param1);

    v0->unk_124 = param0;
    v0->unk_125 = param1;

    return v0;
}

void ov49_0225CCC0(UnkStruct_ov70_02260BB8 *param0) {
    if (param0->unk_128) {
        ov49_0225CE88(param0);
    }

    Heap_Free(param0->unk_11C);
    Heap_Free(param0->unk_120);
    Heap_Free(param0);
}

void ov49_0225CCF0(UnkStruct_ov70_02260BB8 *param0) {
    ov49_0225D7B8(&param0->unk_00, &param0->unk_12C);

    {
        int v0;

        for (v0 = 0; v0 < param0->unk_124; v0++) {
            ov49_0225DA70(&param0->unk_11C[v0], &param0->unk_1C4);
        }
    }

    {
        int v1;

        for (v1 = 0; v1 < param0->unk_125; v1++) {
            ov49_0225DD68(param0, &param0->unk_120[v1]);
        }
    }
}

void ov49_0225CD58(UnkStruct_ov70_02260BB8 *param0) {
    int v0;

    ((param0) ? (void)0 : GF_AssertFail());
    ((param0->unk_120) ? (void)0 : GF_AssertFail());
    ((param0->unk_11C) ? (void)0 : GF_AssertFail());

    ov49_0225D804(&param0->unk_00, &param0->unk_12C);

    for (v0 = 0; v0 < param0->unk_125; v0++) {
        ov49_0225DD0C(&param0->unk_29C, &param0->unk_120[v0]);
    }

    for (v0 = 0; v0 < param0->unk_124; v0++) {
        ov49_0225DAFC(&param0->unk_11C[v0], &param0->unk_1C4);
    }
}
