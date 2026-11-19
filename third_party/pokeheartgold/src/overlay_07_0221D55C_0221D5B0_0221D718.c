#include "global.h"

#include "bg_window.h"
#include "overlay_07_battle_anim_system.h"
#include "gf_gfx_planes.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleAnimContext;

struct BattleAnimSystem;

struct BattleBgAnim;

struct BgTrackingTask;

struct SpriteTrackingTask;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

enum BattleAnimTrackingTask {
    BATTLE_ANIM_TRACKING_TASK_SPRITE_0 = 0,
    BATTLE_ANIM_TRACKING_TASK_SPRITE_1,
    BATTLE_ANIM_TRACKING_TASK_SPRITE_2,
    BATTLE_ANIM_TRACKING_TASK_SPRITE_3,
    BATTLE_ANIM_TRACKING_TASK_BG,

    BATTLE_ANIM_TRACKING_TASK_COUNT,
};

typedef struct BattleAnimContext BattleAnimContext;

typedef struct BattleBgAnim BattleBgAnim;

typedef struct SpriteTrackingTask SpriteTrackingTask;

typedef struct BgTrackingTask BgTrackingTask;

void ov07_0221D55C(BattleAnimSystem *system, enum BattleAnimTrackingTask task);

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

static inline void BattleAnimScript_Next(BattleAnimSystem *system);

static inline void BattleAnimScript_Next(BattleAnimSystem *system) {
    system->scriptPtr += 1;
}

void ov07_0221D718(BattleAnimSystem *system);

void ov07_0221D718(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);
    BattleAnimScript_Next(system);

    u8 *bgTiles = BgGetCharPtr(GF_BG_LYR_MAIN_2);
    MI_CpuFill8(bgTiles, 0, 10 * 10 * 2 * TILE_SIZE_4BPP);

    ov07_0221D55C(system, BATTLE_ANIM_TRACKING_TASK_BG);
}
