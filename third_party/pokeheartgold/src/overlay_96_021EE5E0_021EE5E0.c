#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "field/overlay_01_021FB878.h"

typedef struct UnkStruct_ov6_02240774_t UnkStruct_ov6_02240774;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    s32 unk_0C;
} UnkStruct_ov6_0223FD0C;

typedef struct {
    u32 unk_00;
    Field3dObject unk_04;
    UnkStruct_ov6_0223FD0C unk_7C;
    int unk_8C;
} UnkStruct_ov6_022407B8;

struct UnkStruct_ov6_02240774_t {
    Field3dModel unk_00;
    UnkStruct_ov6_022407B8 unk_10[3];
    u32 unk_1C0;
    s32 unk_1C4;
    s32 unk_1C8;
    fx32 unk_1CC;
    fx32 unk_1D0;
    fx32 unk_1D4;
    u16 unk_1D8;
    u16 unk_1DA;
    u16 unk_1DC;
};

void ov96_021EE5E0(UnkStruct_ov6_02240774 *param0);

void ov96_021EE5E0(UnkStruct_ov6_02240774 *param0) {
    s32 var_r4;
    UnkStruct_ov6_022407B8 *var_r5;

    var_r4 = 0;
    var_r5 = param0->unk_10;
    do {
        ClearWindowTilemapAndCopyToVram((Window *)var_r5);
        RemoveWindow((Window *)var_r5);
        var_r4 += 1;
        var_r5 = (void *)((u8 *)var_r5 + (0x10));
    } while (var_r4 < 2);
    NARC_Delete((NARC *)param0->unk_00.texture);
    Heap_Free(param0);
}
