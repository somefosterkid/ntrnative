#include "global.h"
#include "billboard.h"
#include "filesystem.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct Field3dModel;

struct Field3dObject;

struct NARC;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234798_t;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;

void ov45_022308C0(UnkStruct_ov66_02234798 *param0, const UnkStruct_ov63_0222CC3C *param1);

s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);

UnkStruct_ov63_0222CC3C ov42_022282F4(const UnkStruct_ov63_0222BEC0 *param0);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

int sub_02023EF4(const Billboard *billboard);

fx32 sub_02023F70(const Billboard *billboard);

typedef struct {
    Field3dModel unk_00;
    u32 unk_10;
} UnkStruct_ov66_02234958;

void ov45_02230B64(UnkStruct_ov66_02234798 *param0);

void ov45_02230B8C(UnkStruct_ov66_02234798 *param0);

void ov45_02230BFC(UnkStruct_ov66_02234798 *param0);

void ov45_02230C40(UnkStruct_ov66_02234798 *param0);

BOOL ov45_02230DC4(u32 param0);

void ov45_022309E8(UnkStruct_ov66_02234958 *param0, NARC *param1, u32 param2, enum HeapID heapID);

void ov45_02230ACC(UnkStruct_ov66_02234798 *param0);

void ov45_02230ACC(UnkStruct_ov66_02234798 *param0) {
    u32 v0;
    u32 v1;
    UnkStruct_ov63_0222CC3C v2;
    static void (*v3[12])(UnkStruct_ov66_02234798 *) = {
        ov45_02230B64,
        ov45_02230BFC,
        ov45_02230B8C,
        ov45_02230C40,
        ov45_02230B64,
        ov45_02230B8C,
        ov45_02230B8C,
        ov45_02230B8C,
        ov45_02230B8C,
        ov45_02230B8C,
        ov45_02230B8C
    };

    if (param0->unk_00_0 == 0) {
        return;
    }

    v0 = ov42_02228188(param0->unk_04, 5);
    v1 = ov42_02228188(param0->unk_04, 8);

    if ((param0->unk_86 > v1) || (param0->unk_84 != v0)) {
        if (ov45_02230DC4(param0->unk_84) == 1) {
            param0->unk_85 = sub_02023EF4(param0->unk_08);
            param0->unk_88 = sub_02023F70(param0->unk_08);
        }

        param0->unk_84 = v0;
    }

    param0->unk_86 = v1;

    v3[v0](param0);

    v2 = ov42_022282F4(param0->unk_04);
    ov45_022308C0(param0, &v2);
}
