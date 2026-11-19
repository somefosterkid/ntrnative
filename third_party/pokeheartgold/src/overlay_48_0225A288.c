#include "global.h"

#include "overlay_48.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "sprite.h"


struct SpriteList;
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
void ov48_0225A2EC(UnkStruct_ov69_0225E084 *param0);
void ov48_0225A2A0(UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1);
void ov48_0225A2EC(UnkStruct_ov69_0225E084 *param0);
BOOL ov48_0225A30C(const UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225E0C0 *param1, u32 param2);

void ov48_0225A288(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    *(s16 *)((u8 *)arg0 + 0x36) = 0;
    *(s32 *)((u8 *)arg0 + 0x38) = 0;
}

void ov48_0225A294(void *arg0);

void ov48_0225A294(void *arg0) {
    *(s16 *)((u8 *)arg0 + 0x34) = 0;
    *(s16 *)((u8 *)arg0 + 0x36) = 0;
    *(s32 *)((u8 *)arg0 + 0x38) = 0;
}

void ov48_0225A2A0(UnkStruct_ov69_0225E084 *param0, const UnkStruct_ov69_0225E0C0 *param1) {
    if (((param0->unk_36 + 1) % 4) == param0->unk_34) {
        ov48_0225A2EC(param0);
    }

    param0->unk_04[param0->unk_36] = *param1;
    param0->unk_36 = (param0->unk_36 + 1) % 4;
    param0->unk_38++;
}

void ov48_0225A2EC(UnkStruct_ov69_0225E084 *param0) {
    if (param0->unk_36 == param0->unk_34) {
        return;
    }

    param0->unk_34 = (param0->unk_34 + 1) % 4;
    param0->unk_38--;
}

BOOL ov48_0225A30C(const UnkStruct_ov69_0225E084 *param0, UnkStruct_ov69_0225E0C0 *param1, u32 param2) {
    s32 v0;

    if (param2 >= param0->unk_38) {
        return 0;
    }

    v0 = (param0->unk_36 - 1) - param2;

    if (v0 < 0) {
        v0 += 4;
    }

    *param1 = param0->unk_04[v0];

    return 1;
}
