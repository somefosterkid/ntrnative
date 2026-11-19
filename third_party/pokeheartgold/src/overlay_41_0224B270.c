#include "global.h"

#include "unk_02005D10.h"
#include "bg_window.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"

s32 ov41_0224B374(void *, void *);

void ov41_0224B270(void *arg0);

typedef struct {
    u32 unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_02095B28;
typedef struct {
    UnkStruct_02095B28 unk_00;
    UnkStruct_02095B28 unk_08;
    u8 unk_10[4];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStruct_02095C60;
typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov22_0225AD68;
typedef struct {
    BOOL unk_00;
    Sprite *unk_04[2];
    VecFx32 unk_0C[2];
    UnkStruct_ov22_0225AD68 unk_24;
    UnkStruct_ov22_0225AD68 unk_38;
    UnkStruct_ov22_0225AD68 unk_4C;
} UnkStruct_ov22_0225AE9C;
void ov41_0224B298(UnkStruct_ov22_0225AB54 *param0);

void ov41_0224B270(void *arg0) {
    s32 v0;

    v0 = *((s32 *)((UnkStruct_ov22_0225AB54 *)arg0)->unk_2C);
    if (*((s32 *)&((UnkStruct_ov22_0225AB54 *)arg0)->unk_1C) != v0) {
        *((s32 *)&((UnkStruct_ov22_0225AB54 *)arg0)->unk_1C) = v0;
        if (v0 <= 0xA) {
            ov41_0224B374(arg0, arg0 + 0x30);
            PlaySE(0x682U);
        }
    }
}

void ov41_0224B298(UnkStruct_ov22_0225AB54 *param0) {
    int v0;
    int v1;
    int v2 = param0->unk_1C;
    int v3 = 1;

    for (v0 = 1; v0 < 2; v0++) {
        v3 *= 10;
    }

    for (v0 = 0; v0 < 2; v0++) {
        v1 = v2 / v3;

        ((v1 <= 10) ? (void)0 : GF_AssertFail());
        Sprite_SetAnimCtrlSeq(param0->unk_10[v0], 0 + v1);

        v2 -= v1 * v3;
        v3 /= 10;

        if (param0->unk_90 == 0) {
            if (param0->unk_1C <= 10) {
                Sprite_SetPalIndexRespectVramOffset(param0->unk_10[v0], 1);

                if (v0 == 2 - 1) {
                    param0->unk_90 = 1;
                }
            }
        }
    }
}
