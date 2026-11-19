#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "render_window.h"
#include "sys_task.h"
#include "unk_0200A090.h"

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    Window unk_1C;
} UnkStruct_ov69_0225DC48;

int ov48_02259E5C(const UnkStruct_ov69_0225DC48 *param0, u32 param1);
void ov48_02259E78(UnkStruct_ov69_0225DC48 *param0);
void ov48_02259E90(UnkStruct_ov69_0225DC48 *param0);

int ov48_02259E5C(const UnkStruct_ov69_0225DC48 *param0, u32 param1) {
    int v0;

    switch (param1) {
    case 0:
        v0 = param0->unk_00;
        break;
    case 1:
        v0 = param0->unk_10;
        break;
    case 2:
        v0 = param0->unk_14;
        break;
    default:
        break;
    }

    return v0;
}

void ov48_02259E78(UnkStruct_ov69_0225DC48 *param0) {
    ClearWindowPixelBuffer(&param0->unk_1C, 1);
    ClearWindowTilemapAndScheduleTransfer(&param0->unk_1C);
}

void ov48_02259E90(UnkStruct_ov69_0225DC48 *param0) {
    ScheduleWindowCopyToVram(&param0->unk_1C);
    DrawFrameAndWindow1(&param0->unk_1C, 0, 1 + (18 + 12), 0);
}
