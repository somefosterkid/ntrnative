#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task.h"
#include "error_handling.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

void ov07_022220FC(XYTransformContext *ctx, u16 sx, u16 ex, u16 sy, u16 ey, fx32 rx, fx32 ry, int steps);

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

s32 ov07_02222180(void *);
s32 ov07_022222B4(void *);
s32 ov07_02222384(void *arg0, void *arg1);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov07_02222338(XYTransformContext *linear, XYTransformContext *revs, s16 sx, s16 ex, s16 sy, s16 ey, u16 frames, fx32 arcRadius);

void ov07_02222338(XYTransformContext *linear, XYTransformContext *revs, s16 sx, s16 ex, s16 sy, s16 ey, u16 frames, fx32 arcRadius) {
    ov07_02222268(linear, sx, ex, sy, ey, frames);
    revs->x = 0;
    revs->y = 0;
    ov07_022220FC(revs, 0, 0, (((90) * 0xFFFF) / 360), (((270) * 0xFFFF) / 360), 0, arcRadius, frames);
}

s32 ov07_02222384(void *arg0, void *arg1) {
    s32 v0;
    s32 v1;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (arg1 == NULL) {
        GF_AssertFail();
    }
    v1 = ov07_022222B4(arg0);
    v0 = ov07_02222180(arg1);
    *(s16 *)arg0 = (s16)(*(s16 *)arg0 + *(s16 *)arg1);
    *(s16 *)((u8 *)arg0 + 2) = (s16)(*(s16 *)((u8 *)arg0 + 2) + *(s16 *)((u8 *)arg1 + 2));
    if ((v1 == v0) && (v1 == 0)) {
        return 0;
    }
    return 1;
}
