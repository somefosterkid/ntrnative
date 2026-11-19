#include "global.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D848_t;
struct UnkStruct_ov66_02234548_t;
struct UnkStruct_ov66_02234798_t;
struct UnkStruct_ov70_0225C894_t;
struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;
typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;
void ov45_0223089C(UnkStruct_ov66_02234548 *param0);
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0225D93C;
typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;
struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};
void ov45_022308C0(UnkStruct_ov66_02234548 *param0);
void ov45_022308E4(UnkStruct_ov66_02234548 *param0);
void ov45_02230908(UnkStruct_ov66_02234548 *param0);
void ov45_02230920(UnkStruct_ov66_02234548 *param0);
void ov45_0223093C(UnkStruct_ov66_02234798 *param0, int param1);
void ov45_02230968(UnkStruct_ov66_02234798 *param0);
void ov45_022308B0(UnkStruct_ov66_02234548 *param0);
void ov45_02230978(UnkStruct_ov66_02234548 *param0);
void ov45_02230994(UnkStruct_ov66_02234548 *param0);

void ov49_02259130(UnkStruct_ov70_0225CA20 *param0);
void ov49_0225913C(UnkStruct_ov70_0225CA20 *param0);
void ov49_02259148(UnkStruct_ov70_0225CA20 *param0);
void ov49_02259154(UnkStruct_ov70_0225CA20 *param0);
void ov49_02259160(UnkStruct_ov70_0225CA20 *param0);
void ov49_0225916C(UnkStruct_ov70_0225CC54 *param0, BOOL param1);
void ov49_02259184(UnkStruct_ov70_0225CC54 *param0, BOOL param1);
void ov49_0225919C(UnkStruct_ov70_0225CC54 *param0, BOOL param1);
void ov49_022591B4(UnkStruct_ov70_0225CA20 *param0);
void ov49_022591C0(UnkStruct_ov70_0225CA20 *param0);
void ov49_022591CC(UnkStruct_ov70_0225CA20 *param0);

void ov49_02259130(UnkStruct_ov70_0225CA20 *param0) {
    ov45_0223089C(param0->unk_04);
}

void ov49_0225913C(UnkStruct_ov70_0225CA20 *param0) {
    ov45_022308C0(param0->unk_04);
}

void ov49_02259148(UnkStruct_ov70_0225CA20 *param0) {
    ov45_022308E4(param0->unk_04);
}

void ov49_02259154(UnkStruct_ov70_0225CA20 *param0) {
    ov45_02230908(param0->unk_04);
}

void ov49_02259160(UnkStruct_ov70_0225CA20 *param0) {
    ov45_02230920(param0->unk_04);
}

void ov49_0225916C(UnkStruct_ov70_0225CC54 *param0, BOOL param1) {
    if (param1) {
        ov45_0223093C(param0->unk_04, 1);
    } else {
        ov45_02230968(param0->unk_04);
    }
}

void ov49_02259184(UnkStruct_ov70_0225CC54 *param0, BOOL param1) {
    if (param1) {
        ov45_0223093C(param0->unk_04, 0);
    } else {
        ov45_02230968(param0->unk_04);
    }
}

void ov49_0225919C(UnkStruct_ov70_0225CC54 *param0, BOOL param1) {
    if (param1) {
        ov45_0223093C(param0->unk_04, 2);
    } else {
        ov45_02230968(param0->unk_04);
    }
}

void ov49_022591B4(UnkStruct_ov70_0225CA20 *param0) {
    ov45_022308B0(param0->unk_04);
}

void ov49_022591C0(UnkStruct_ov70_0225CA20 *param0) {
    ov45_02230978(param0->unk_04);
}

void ov49_022591CC(UnkStruct_ov70_0225CA20 *param0) {
    ov45_02230994(param0->unk_04);
}
