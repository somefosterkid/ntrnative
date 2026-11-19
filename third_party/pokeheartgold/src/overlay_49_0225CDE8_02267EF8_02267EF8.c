#include "global.h"
#include "overlay_49_unk_struct_ov70_02269800.h"
#include "constants/heap.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_02269204_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

void sub_020182A0(Field3dObject *obj, BOOL visible);

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

typedef struct {
    s32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    fx32 unk_18;
    fx32 unk_1C;
    fx32 unk_20;
    fx32 unk_24;
} UnkStruct_ov70_022692B4;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
    fx32 unk_08;
} UnkStruct_ov70_0226949C;

typedef struct {
    s16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov70_022692B4 unk_08[4];
    UnkStruct_ov70_0226949C unk_A8[4];
    Field3dObject *unk_D8[4];
    const UnkStruct_ov70_0225CC54 *unk_E8;
} UnkStruct_ov70_0226BD64;

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

void ov49_02267F94(UnkStruct_ov70_0226BD64 *param0, const UnkStruct_ov70_0225CC54 *param1);

void ov49_0226813C(UnkStruct_ov70_0226BD64 *param0, const UnkStruct_ov70_0225CC54 *param1);

void ov49_022682D4(UnkStruct_ov70_0226BD64 *param0, const UnkStruct_ov70_0225CC54 *param1);

UnkStruct_ov70_02269204 *ov49_022652E8(WiFiPlazaSystem *param0, UnkStruct_ov70_0225C894 *param1, UnkStruct_ov70_02260AD4 *param2, UnkStruct_ov70_0225CA20 *param3, enum HeapID heapID, u32 heapID2);

void ov49_02267EF8(UnkStruct_ov70_0226BD64 *param0, u32 param1);

void ov49_02267EF8(UnkStruct_ov70_0226BD64 *param0, u32 param1) {
    int v0;

    extern void (*const ov49_0226A4CC[3])(UnkStruct_ov70_0226BD64 *, const UnkStruct_ov70_0225CC54 *);

    for (v0 = 0; v0 < param1; v0++) {
        sub_020182A0(param0->unk_D8[v0], 1);
    }

    param0->unk_00 = 0;
    param0->unk_06 = 1;
    param0->unk_04 = param1;

    ov49_0226A4CC[param0->unk_02](param0, param0->unk_E8);
}

BOOL ov49_022680B4(UnkStruct_ov70_0226BD64 *param0);

BOOL ov49_02268230(UnkStruct_ov70_0226BD64 *param0);

BOOL ov49_02268334(UnkStruct_ov70_0226BD64 *param0);

BOOL ov49_02267F40(UnkStruct_ov70_0226BD64 *param0);

BOOL ov49_02267F40(UnkStruct_ov70_0226BD64 *param0) {
    int v0;
    BOOL v1;

    extern BOOL (*const ov49_0226A484[3])(UnkStruct_ov70_0226BD64 *);

    if (param0->unk_06 == 0) {
        return 1;
    }

    v1 = ov49_0226A484[param0->unk_02](param0);

    param0->unk_00++;

    if (v1 == 1) {
        for (v0 = 0; v0 < param0->unk_04; v0++) {
            sub_020182A0(param0->unk_D8[v0], 0);
        }

        param0->unk_06 = 0;
    }

    return v1;
}
