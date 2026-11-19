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

void ov49_02265B3C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3, fx32 param4);

UnkStruct_ov70_02269204 *ov49_022652E8(WiFiPlazaSystem *param0, UnkStruct_ov70_0225C894 *param1, UnkStruct_ov70_02260AD4 *param2, UnkStruct_ov70_0225CA20 *param3, enum HeapID heapID, u32 heapID2);

void ov49_02265B14(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3);

void ov49_02265B14(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3) {
    ov49_02265B3C(param0, param1, param2, param3, (FX32_ONE * 2));
}

BOOL ov49_02265B94(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3, fx32 param4);

void ov49_02265B28(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3);

void ov49_02265B28(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3) {
    ov49_02265B94(param0, param1, param2, param3, (FX32_ONE * 2));
}
