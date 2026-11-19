#include "global.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleAnimContext;
struct BattleAnimSystem;
struct BattleBgAnim;
struct BgTrackingTask;
struct ManagedSprite;
struct Pokepic;
struct PokepicDrawParam;
struct SpriteTrackingTask;
struct SysTask;
struct SysTaskQueue;
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct BattleAnimContext BattleAnimContext;
typedef struct BattleBgAnim BattleBgAnim;
typedef struct SpriteTrackingTask SpriteTrackingTask;
typedef struct BgTrackingTask BgTrackingTask;
u8 ov07_0221FA04(BattleAnimSystem *system, int battler);
int ov07_02231958(BattleAnimSystem *system, int type);
int ov07_0223197C(BattleAnimSystem *system, int battler);
int ov07_022319E0(int battler);
void ov07_02231FA0(Pokepic *sprite, Point2D *pos);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
void ov07_02232020(BattleAnimSystem *system, int targets, BattleAnimSpriteInfo *sprites, int *count);
void ov07_02231FD8(ManagedSprite *sprite, Point2D *pos);
void ov07_022323FC(BattleAnimSystem *system, int targets, BattleAnimSpriteInfo *sprites, int *count);

void ov07_02231FE4(BattleAnimSystem *system, BattleAnimScriptFuncCommon *common) {
    common->state = 0;
    common->counter1 = 0;
    common->counter2 = 0;
    common->unused_03 = 0;
    common->battleAnimSys = system;
    common->spriteSystem = ov07_0221C530(system);
    common->pokemonSpriteManager = ov07_0221C514(system);
    common->primarySpriteManager = ov07_0221C528(system);
    common->bgConfig = ov07_0221C4A0(system);
    common->paletteData = ov07_0221FA78(system);
}

static inline BOOL BattleAnimUtil_IsMaskSet(int value, int mask) {
    if ((value & mask) == mask) {
        return 1;
    }

    return 0;
}

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

void ov07_02232020(BattleAnimSystem *system, int targets, BattleAnimSpriteInfo *sprites, int *count) {
    *count = 0;

    if (BattleAnimUtil_IsMaskSet(targets, (1 << 6)) == 1) {
        int attacker = ov07_0221C468(system);

        sprites[*count].monSprite = ov07_0221FA48(system, attacker);

        if (sprites[*count].monSprite != 0L) {
            sprites[*count].battler = attacker;
            ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
            (*count)++;
        }

        int battler = ov07_0223197C(system, attacker);
        sprites[*count].monSprite = ov07_0221FA48(system, battler);

        if (sprites[*count].monSprite != 0L) {
            sprites[*count].battler = battler;
            ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
            (*count)++;
        }

        int type = ov07_0221FA04(system, attacker);
        type = ov07_022319E0(type);
        battler = ov07_02231958(system, type);

        sprites[*count].monSprite = ov07_0221FA48(system, battler);

        if (sprites[*count].monSprite != 0L) {
            sprites[*count].battler = battler;
            ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
            (*count)++;
        }

        battler = ov07_0223197C(system, battler);
        sprites[*count].monSprite = ov07_0221FA48(system, battler);

        if (sprites[*count].monSprite != 0L) {
            sprites[*count].battler = battler;
            ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
            (*count)++;
        }

        return;
    }

    if (BattleAnimUtil_IsMaskSet(targets, (1 << 5)) == 1) {
        int attacker = ov07_0221C468(system);

        int battler = ov07_0223197C(system, attacker);
        if (battler != attacker) {
            sprites[*count].monSprite = ov07_0221FA48(system, battler);

            if (sprites[*count].monSprite != 0L) {
                sprites[*count].battler = battler;
                ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
                (*count)++;
            }
        }

        int type = ov07_0221FA04(system, attacker);
        type = ov07_022319E0(type);
        battler = ov07_02231958(system, type);
        if (battler != attacker) {
            sprites[*count].monSprite = ov07_0221FA48(system, battler);

            if (sprites[*count].monSprite != 0L) {
                sprites[*count].battler = battler;
                ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
                (*count)++;
            }
        }

        battler = ov07_0223197C(system, battler);
        if (battler != attacker) {
            sprites[*count].monSprite = ov07_0221FA48(system, battler);

            if (sprites[*count].monSprite != 0L) {
                sprites[*count].battler = battler;
                ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
                (*count)++;
            }
        }

        return;
    }

    if (BattleAnimUtil_IsMaskSet(targets, (1 << 1)) == 1) {
        sprites[*count].monSprite = ov07_0221FA48(system, ov07_0221C468(system));
        if (sprites[*count].monSprite != 0L) {
            sprites[*count].battler = ov07_0221C468(system);
            ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
            (*count)++;
        }
    }

    if (ov07_0221FAB0(system) == 1) {
        if (BattleAnimUtil_IsMaskSet(targets, (1 << 2)) == 1) {
            sprites[*count].monSprite = ov07_0221FA48(system, ov07_0223197C(system, ov07_0221C468(system)));
            if (sprites[*count].monSprite != 0L) {
                sprites[*count].battler = ov07_0223197C(system, ov07_0221C468(system));
                ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
                (*count)++;
            }
        }
    }

    if (BattleAnimUtil_IsMaskSet(targets, (1 << 3)) == 1) {
        sprites[*count].monSprite = ov07_0221FA48(system, ov07_0221C470(system));
        if (sprites[*count].monSprite != 0L) {
            sprites[*count].battler = ov07_0221C470(system);
            ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
            (*count)++;
        }
    }

    if (ov07_0221FAB0(system) == 1) {
        if (BattleAnimUtil_IsMaskSet(targets, (1 << 4)) == 1) {
            sprites[*count].monSprite = ov07_0221FA48(system, ov07_0223197C(system, ov07_0221C470(system)));
            if (sprites[*count].monSprite != 0L) {
                sprites[*count].battler = ov07_0223197C(system, ov07_0221C470(system));
                ov07_02231FA0(sprites[*count].monSprite, &sprites[*count].pos);
                (*count)++;
            }
        }
    }
}

void ov07_022323FC(BattleAnimSystem *system, int targets, BattleAnimSpriteInfo *sprites, int *count) {
    *count = 0;

    if (BattleAnimUtil_IsMaskSet(targets, (1 << 1)) == 1) {
        sprites[*count].hwSprite = ov07_0221C4E8(system, *count);
        ov07_02231FD8(sprites[*count].hwSprite, &sprites[*count].pos);
        (*count)++;
    }

    if (BattleAnimUtil_IsMaskSet(targets, (1 << 2)) == 1) {
        sprites[*count].hwSprite = ov07_0221C4E8(system, *count);
        ov07_02231FD8(sprites[*count].hwSprite, &sprites[*count].pos);
        (*count)++;
    }

    if (BattleAnimUtil_IsMaskSet(targets, (1 << 3)) == 1) {
        sprites[*count].hwSprite = ov07_0221C4E8(system, *count);
        ov07_02231FD8(sprites[*count].hwSprite, &sprites[*count].pos);
        (*count)++;
    }

    if (BattleAnimUtil_IsMaskSet(targets, (1 << 4)) == 1) {
        sprites[*count].hwSprite = ov07_0221C4E8(system, *count);
        ov07_02231FD8(sprites[*count].hwSprite, &sprites[*count].pos);
        (*count)++;
    }
}
