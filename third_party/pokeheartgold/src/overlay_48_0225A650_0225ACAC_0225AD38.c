#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02005D10.h"

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

void ov48_0225AD38(UnkStruct_ov69_0225EB60 *param0);

void ov48_0225AD38(UnkStruct_ov69_0225EB60 *param0) {
    int v0;

    for (v0 = 0; v0 < 12; v0++) {
        Sprite_SetDrawFlag(param0->unk_3C[v0], FALSE);
    }
}

s32 ov48_0225ADBC(void *, u32, void *);

s32 ov48_0225ADF8(void *, u32, void *);

void ov48_0225AD54(UnkStruct_ov69_0225EB60 *arg0, void *arg1);

void ov48_0225AD54(UnkStruct_ov69_0225EB60 *arg0, void *arg1) {
    s16 v2;
    s32 v1;

    v2 = arg0->unk_32;
    if ((s32)v2 > 0) {
        arg0->unk_32 = (s16)(v2 - 1);
        if ((arg0->unk_32 == 0) && (ov48_0225ADBC(arg0, arg0->unk_30, arg1) == 1)) {
            PlaySE(0x5D8U);
        }
    } else if (ov48_0225ADF8(arg0, arg0->unk_30, arg1) == 1) {
        v1 = arg0->unk_30 - 1;
        if (v1 >= 0) {
            arg0->unk_30 = (s16)v1;
            ov48_0225ADBC(arg0, arg0->unk_30, arg1);
            return;
        }
        StopSE(0x5D8U, 0);
        arg0->unk_30 = 2;
        arg0->unk_32 = 0x80;
    }
}
