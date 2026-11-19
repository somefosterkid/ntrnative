#include "global.h"

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

void ov48_0225AE3C(UnkStruct_ov69_0225ED90 *param0, const UnkStruct_ov69_0225ECF0 *param1);

void ov48_0225AE58(UnkStruct_ov69_0225ED90 *param0, BOOL param1);

void ov48_0225AE5C(UnkStruct_ov69_0225ED90 *param0, BOOL param1);

u16 ov48_0225B050(u32 param0);

extern const UnkStruct_ov69_0225ECF0 ov48_0225B330[12][3];

void ov48_0225ACD8(UnkStruct_ov69_0225EB60 *param0, u8 param1, u8 param2, u8 param3);

void ov48_0225ACD8(UnkStruct_ov69_0225EB60 *param0, u8 param1, u8 param2, u8 param3) {
    int v0;

    param0->unk_34[param1] = 1;
    param0->unk_38[param1] = ov48_0225B050(param2);

    ov48_0225AE3C(&param0->unk_00[param1], &ov48_0225B330[param0->unk_38[param1]][param1]);

    if (param3 < 24) {
        v0 = ov48_0225B050(param3);

        if (v0 == param0->unk_38[param1]) {
            if (param1 < 2) {
                ov48_0225AE5C(&param0->unk_00[param1 + 1], 1);
                ov48_0225AE58(&param0->unk_00[param1], 1);
            }
        }
    }
}
