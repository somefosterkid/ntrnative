#include "global.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "overlay_48.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

void ov48_0225A650(void *arg0, s32 arg1, s32 arg2);

struct G3DPipelineBuffers;
struct SpriteList;
struct SpriteResourceCollection;
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
void ov48_0225A6DC(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, u32 param2);
void ov48_0225A768(UnkStruct_ov69_0225E084 *param0);

void ov48_0225A650(void *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    s32 v1;

    v0 = arg1;
    v1 = arg2;
    if (v0 < 0) {
        v0 = 0;
    }
    if (v1 < 0) {
        v1 = 0;
    }
    *(s16 *)arg0 = 1;
    *(s16 *)((u8 *)arg0 + 4) = (s16)v0;
    *(s16 *)((u8 *)arg0 + 6) = (s16)v1;
}

void ov48_0225A668(void *arg0, s32 arg1, s32 arg2);

void ov48_0225A668(void *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    s32 v1;

    v0 = arg1;
    v1 = arg2;
    if (v0 < 0) {
        v0 = 0;
    }
    if (v1 < 0) {
        v1 = 0;
    }
    *(s16 *)((u8 *)arg0 + 2) = 1;
    *(s16 *)((u8 *)arg0 + 8) = (s16)v0;
    *(s16 *)((u8 *)arg0 + 0xA) = (s16)v1;
}

void ov48_0225A680(SysTask *param0, void *param1) {
    UnkStruct_ov69_0225E478 *v0 = param1;

    if (v0->unk_00) {
        G2S_SetWnd0Position(0, v0->unk_04, 255, v0->unk_06);
        v0->unk_00 = 0;
    }

    if (v0->unk_02) {
        G2S_SetWnd1Position(0, v0->unk_08, 255, v0->unk_0A);
        v0->unk_02 = 0;
    }
}

void ov48_0225A6DC(UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225D35C *param1, u32 param2) {
    int v0;
    int v1 = ((10 + (18 + 12)) + (27 * 4));

    for (v0 = 0; v0 < 3; v0++) {
        AddWindowParameterized(param1->unk_00, &param0->unk_168[v0], 5, 0, 0 + (6 * v0), 27, 6, 14, v1);

        v1 += (27 * 6);

        FillWindowPixelBuffer(&param0->unk_168[v0], 0);
        CopyWindowToVram(&param0->unk_168[v0]);
    }

    AddWindowParameterized(param1->unk_00, &param0->unk_198, 5, 0, 0, 27, 6, 14, (10 + (18 + 12)) + (27 * 4));
    FillWindowPixelBuffer(&param0->unk_198, 0);
}

void ov48_0225A768(UnkStruct_ov69_0225E084 *param0) {
    int v0;

    for (v0 = 0; v0 < 3; v0++) {
        RemoveWindow(&param0->unk_168[v0]);
    }

    RemoveWindow(&param0->unk_198);
}
