#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "overlay_92.h"
#include "palette.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct G3DPipelineBuffers;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct {
    NARC *unk_00;
    NARC *unk_04;
    SpriteSystem *unk_08;
    SpriteManager *unk_0C;
    BgConfig *unk_10;
    PaletteData *unk_14;
    G3DPipelineBuffers *unk_18;
    Camera *camera;
    u8 padding_20[4];
    NNSFndAllocator unk_24;
    UnkStruct_ov116_022649E4_sub1 unk_2C;
} UnkStruct_ov116_022649E4;

typedef struct {
    int unk_00;
    BOOL unk_04;
    void *unk_08[8];
    int unk_28[10];
    BOOL unk_50[10];
} UnkStruct_ov116_02265470;

typedef struct {
    int unk_00;
    int *unk_04;
    UnkStruct_ov116_02265470 unk_08[5];
    SysTask *unk_80;
    UnkStruct_ov116_022649E4 *unk_84;
} UnkStruct_ov116_0226534C;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

void ov92_02261118(UnkStruct_ov116_02265470 *param0);

void ov92_02261208(UnkStruct_ov116_02265470 *param0);

void ov92_0226135C(UnkStruct_ov116_02265470 *param0);

void ov92_022613F0(UnkStruct_ov116_02265470 *param0);

void ov92_02261448(SysTask *param0, void *param1) {
    UnkStruct_ov116_0226534C *v0 = param1;
    UnkStruct_ov116_022649E4 *v1 = v0->unk_84;

    if ((IsPaletteFadeFinished() == 0) || (v1->unk_2C.unk_00 == 1)) {
        SysTask_Destroy(param0);

        return;
    }

    switch (*v0->unk_04) {
    case 5:
        if (v0->unk_08[4].unk_04 == 0) {
            v0->unk_08[4].unk_04 = 1;
            v0->unk_08[4].unk_50[0] = 0;
        }
    case 4:
        if (v0->unk_08[3].unk_04 == 0) {
            PlaySE(1550);
            v0->unk_08[3].unk_04 = 1;
            v0->unk_08[3].unk_50[0] = 0;
        }
    case 3:
        if (v0->unk_08[2].unk_04 == 0) {
            v0->unk_08[2].unk_04 = 1;
            v0->unk_08[2].unk_50[0] = 0;
        }
    case 2:
        if (v0->unk_08[1].unk_04 == 0) {
            PlaySE(1417);
            v0->unk_08[1].unk_04 = 1;
            v0->unk_08[1].unk_50[0] = 0;
        }
    case 1:
        if (v0->unk_08[0].unk_04 == 0) {
            v0->unk_08[0].unk_04 = 1;
            v0->unk_08[0].unk_50[0] = 0;
        }
        break;
    default:
        if (*v0->unk_04 != v0->unk_00) {
            v0->unk_08[0].unk_50[0] = 1;
            v0->unk_08[1].unk_50[0] = 1;
            v0->unk_08[2].unk_50[0] = 1;
            v0->unk_08[3].unk_50[0] = 1;
            v0->unk_08[4].unk_50[0] = 1;
            StopSE(1417, 0);
        }
        break;
    }

    ov92_02261118(&v0->unk_08[0]);
    ov92_02261208(&v0->unk_08[1]);
    ov92_0226135C(&v0->unk_08[2]);
    ov92_02261208(&v0->unk_08[3]);
    ov92_022613F0(&v0->unk_08[4]);

    v0->unk_00 = *v0->unk_04;
}
