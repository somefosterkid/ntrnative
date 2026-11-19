#include "global.h"
#include "overlay_49_unk_struct_ov70_02269800.h"
#include "constants/heap.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_02269204_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov70_0226E5A4;

struct UnkStruct_ov70_02269204_t {
    WiFiPlazaSystem *unk_00;
    UnkStruct_ov70_0225CA20 *unk_04;
    UnkStruct_ov70_02260AD4 *unk_08;
    UnkStruct_ov70_0225C894 *unk_0C;
    UnkStruct_ov70_02269800 unk_10[20];
    Field3dModel unk_10550[15];
    void *unk_10640[39];
    Field3DModelAnimation unk_106DC[16];
    NNSFndAllocator unk_1081C;
};

void ov49_02265980(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, const UnkStruct_ov70_0226E5A4 *param3);

extern const UnkStruct_ov70_0226E5A4 ov49_0226A70C[39];

UnkStruct_ov70_02269204 *ov49_022652E8(WiFiPlazaSystem *param0, UnkStruct_ov70_0225C894 *param1, UnkStruct_ov70_02260AD4 *param2, UnkStruct_ov70_0225CA20 *param3, enum HeapID heapID, u32 heapID2);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov49_02259154(void *param0);
#else
void ov49_02259154();
#endif

void sub_020182A8(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov49_02267C8C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02267C8C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    VecFx32 v0;
    int v1;

#if defined(SDK_PORT) || defined(NONMATCHING)
    ov49_02259154((struct UnkStruct_ov70_0225CA20_t *)(param1->unk_08));
#else
    ov49_02259154((struct UnkStruct_ov70_0225CA20_t *)(param1->unk_08), &v0);
#endif

    v0.z += (-FX32_CONST(10));
    v0.x += (-FX32_CONST(4));
    v0.y += (FX32_CONST(16)) + (-FX32_CONST(0));

    for (v1 = 0; v1 < 2; v1++) {
        ov49_02265980(param0, param1, 16 + v1, &ov49_0226A70C[37 + v1]);
        sub_020182A8(&param1->unk_0C[v1 + 16], v0.x + (v1 * (FX32_CONST(24))), v0.y, v0.z);
    }
}
