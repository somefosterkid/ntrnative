#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "render_window.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_48_unk_struct_ov69_0225ef54.h"
#include "msgdata.h"

struct G3DPipelineBuffers;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
} UnkStruct_ov69_0225ECF0;
typedef struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
} UnkStruct_ov69_0225E0C0;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04[4];
    void *unk_08[2];
    NNSG2dScreenData *unk_10[2];
    u16 unk_18;
    u16 unk_1A;
    void *unk_1C;
    NNSG2dPaletteData *unk_20;
    u8 unk_24;
    u8 unk_25;
    u8 unk_26;
    u8 unk_27;
} UnkStruct_ov69_0225E7DC;
typedef struct {
    u8 unk_00;
    s8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov69_0225E084_sub1;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    const UnkStruct_ov69_0225ECF0 *unk_0C;
} UnkStruct_ov69_0225ED90;
typedef struct {
    UnkStruct_ov69_0225ED90 unk_00[3];
    s16 unk_30;
    s16 unk_32;
    u8 unk_34[4];
    u8 unk_38[4];
    Sprite *unk_3C[12];
    SpriteResource *unk_6C[12][4];
} UnkStruct_ov69_0225EB60;
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    SysTask *unk_0C;
} UnkStruct_ov69_0225E478;
typedef struct {
    u32 unk_00;
    UnkStruct_ov69_0225E0C0 unk_04[4];
    u16 unk_34;
    u16 unk_36;
    u32 unk_38;
    u16 unk_3C;
    u16 unk_3E;
    UnkStruct_ov69_0225E084_sub1 unk_40[18];
    u32 unk_88[3];
    void *unk_94;
    NNSG2dScreenData *unk_98;
    u16 unk_9C;
    u16 unk_9E;
    UnkStruct_ov69_0225E7DC unk_A0[5];
    Window unk_168[3];
    Window unk_198;
    Window unk_1A8;
    UnkStruct_ov69_0225EB60 unk_1B8;
    UnkStruct_ov69_0225E478 unk_2E4;
} UnkStruct_ov69_0225E084;
void ov48_0225A338(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1);
void ov48_0225A354(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225EF54 *param1, UnkStruct_ov69_0225D35C *param2);
BOOL ov48_0225A41C(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1);
void ov48_0225A428(UnkStruct_ov69_0225E084 *param0);
BOOL ov48_0225A430(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1);
void ov48_0225AD54(UnkStruct_ov69_0225EB60 *param0, UnkStruct_ov69_0225E478 *param1);
void ov48_0225A2A0(UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1);
BOOL ov48_0225A30C(const UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225E0C0 *param1, u32 param2);

void ov48_0225A158(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225EF54 *param1, UnkStruct_ov69_0225D35C *param2);
void ov48_0225A1D0(UnkStruct_ov69_0225E084 *param0);
void ov48_0225A1EC(UnkStruct_ov69_0225E084 *param0);
BOOL ov48_0225A20C(UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1);
BOOL ov48_0225A244(const UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1);

void ov48_0225A158(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225EF54 *param1, UnkStruct_ov69_0225D35C *param2) {
    BOOL v0;

    switch (param0->unk_00) {
    case 0:
        ov48_0225A338(param0, param2);
        ov48_0225AD54(&param0->unk_1B8, &param0->unk_2E4);
        break;
    case 1:
        ov48_0225A354(param0, param1, param2);
        param0->unk_00 = 2;
        break;
    case 2:
        v0 = ov48_0225A41C(param0, param2);

        if (v0 == 1) {
            param0->unk_00 = 0;
        }
        break;
    case 3:
        ov48_0225A428(param0);
        ov48_0225A430(param0, param2);
        param0->unk_00 = 4;
        break;
    case 4:
        v0 = ov48_0225A430(param0, param2);

        if (v0 == 1) {
            param0->unk_00 = 0;
        }
        break;
    }
}

void ov48_0225A1D0(UnkStruct_ov69_0225E084 *param0) {
    ClearFrameAndWindow2(&param0->unk_1A8, 1);
    ClearWindowTilemapAndScheduleTransfer(&param0->unk_1A8);
}

void ov48_0225A1EC(UnkStruct_ov69_0225E084 *param0) {
    ScheduleWindowCopyToVram(&param0->unk_1A8);
    DrawFrameAndWindow2(&param0->unk_1A8, 1, 10, 10);
}

BOOL ov48_0225A20C(UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1) {
    switch (param0->unk_00) {
    case 0:
        param0->unk_00 = 1;
        break;
    case 2:
        param0->unk_00 = 3;
        return 0;
    case 3:
    case 4:
    case 1:
        return 0;
    }

    ov48_0225A2A0(param0, param1);

    return 1;
}

BOOL ov48_0225A244(const UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1) {
    int v0;
    UnkStruct_ov69_0225E0C0 v1;
    BOOL v2;

    for (v0 = 0; v0 < 3; v0++) {
        v2 = ov48_0225A30C(param0, &v1, v0);

        if (v2 == 0) {
            return 0;
        }

        if ((v1.unk_04 == param1->unk_04) && (v1.unk_08 == param1->unk_08)) {
            return 1;
        }
    }

    return 0;
}
