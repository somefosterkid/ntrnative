#include "global.h"
#include "sprite_resources_header.h"

#include "heap.h"
#include "sys_task_api.h"
#include "overlay_07_battle_anim_system.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

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

typedef struct BattleAnimSoundContext {
    u8 type;

    u8 unused01;
    u8 unused02;

    u8 applyInterval;
    u8 tickCount;

    int startPan;
    int endPan;
    int panStep;
    int curPan;

    u8 repeatCount;
    u16 effectID;

    u32 unused1C[7];

    BattleAnimSystem *system;
} BattleAnimSoundContext;

enum BattleAnimSoundTaskType {
    BATTLE_ANIM_SOUND_TASK_NONE = 0,
    BATTLE_ANIM_SOUND_TASK_PAN,
    BATTLE_ANIM_SOUND_TASK_PAN_2,
    BATTLE_ANIM_SOUND_TASK_PAN_3,
    BATTLE_ANIM_SOUND_TASK_REPEAT,
    BATTLE_ANIM_SOUND_TASK_DELAY,

    BATTLE_ANIM_SOUND_TASK_COUNT,
};

extern BOOL (*const ov07_02234C08[BATTLE_ANIM_SOUND_TASK_COUNT])(BattleAnimSoundContext *);

void ov07_0221C584(SysTask *task, void *param);

void ov07_0221C584(SysTask *task, void *param) {
    BattleAnimSoundContext *ctx = param;

    BOOL result = ov07_02234C08[ctx->type](ctx);
    if (result == FALSE) {
        if (ctx->system->activeSoundTasks != 0) {
            ctx->system->activeSoundTasks--;
        }

        Heap_Free(ctx);
        SysTask_Destroy(task);
    }
}
