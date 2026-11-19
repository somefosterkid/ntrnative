#include "global.h"

#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);


BOOL ov07_0221BFC0(BattleAnimSystem *system);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

int ov07_0221FB04(BattleAnimSystem *system, int bg);

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_022222B4(XYTransformContext *ctx);

enum Battler {
    BTLSCR_ALL_BATTLERS = 0,
    BTLSCR_ATTACKER = 1,
    BTLSCR_DEFENDER = 2,
    BTLSCR_PLAYER = 3,
    BTLSCR_ENEMY = 4,
    BTLSCR_FAINTED_MON = 5,
    BTLSCR_SWITCHED_MON = 6,
    BTLSCR_SIDE_EFFECT_MON = 7,
    BTLSCR_ABILITY_MON = 8,
    BTLSCR_PLAYER_SLOT_1 = 9,
    BTLSCR_ENEMY_SLOT_1 = 10,
    BTLSCR_PLAYER_SLOT_2 = 11,
    BTLSCR_ENEMY_SLOT_2 = 12,
    BTLSCR_UNUSED_13 = 13,
    BTLSCR_MSG_ATTACKER = 14,
    BTLSCR_MSG_DEFENDER = 15,
    BTLSCR_ATTACKER_PARTNER = 16,
    BTLSCR_DEFENDER_PARTNER = 17,
    BTLSCR_FORCED_OUT = 18,
    BTLSCR_ATTACKER_ENEMY = 19,
    BTLSCR_DEFENDER_ENEMY = 20,
    BTLSCR_MSG_BATTLER_TEMP = 21,
    BTLSCR_SWITCHED_MON_AFTER = 22,
};

enum Battler ov07_0223192C(BattleAnimSystem *system, int battler);

typedef struct FrenzyPlantSpriteContext {
    BattleAnimScriptFuncCommon common;
    s16 unused;
    s16 lifetime[8];
    ManagedSprite *sprites[8];
    XYTransformContext pos;
} FrenzyPlantSpriteContext;

void ov07_0222B2A8(SysTask *task, void *param);

void ov07_0222B320(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222B320(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    FrenzyPlantSpriteContext *ctx = ov07_022324D8(system, sizeof(FrenzyPlantSpriteContext));
    ctx->unused = 10;

    ov07_02231FE4(system, &ctx->common);

    int i;

    ManagedSpriteTemplate template;

    template = ov07_0221F9E8(ctx->common.battleAnimSys);
    ctx->sprites[0] = sprite;

    for (i = 1; i < 8; i++) {
        ctx->sprites[i] = SpriteSystem_NewSprite(ctx->common.spriteSystem, ctx->common.primarySpriteManager, &template);

        if (i % 2) {
            ManagedSprite_SetFlipMode(ctx->sprites[i], 1);
        }
    }

    Pokepic *monSprite;
    s16 attackerX, defenderX, attackerY, defenderY;

    monSprite = ov07_0221FA48(ctx->common.battleAnimSys, ov07_0221C468(ctx->common.battleAnimSys));
    attackerX = Pokepic_GetAttr(monSprite, MON_SPRITE_X_CENTER);
    attackerY = Pokepic_GetAttr(monSprite, MON_SPRITE_Y_CENTER);
    monSprite = ov07_0221FA48(ctx->common.battleAnimSys, ov07_0221C470(ctx->common.battleAnimSys));
    defenderX = Pokepic_GetAttr(monSprite, MON_SPRITE_X_CENTER);
    defenderY = Pokepic_GetAttr(monSprite, MON_SPRITE_Y_CENTER);

    ov07_02222268(&ctx->pos, attackerX, defenderX, attackerY, defenderY, (8 + 2) * 2);

    BOOL active;
    int frame = 0;
    int spriteIdx = 0;
    BOOL skip = 0;
    if (ov07_0223192C(ctx->common.battleAnimSys, ov07_0221C468(ctx->common.battleAnimSys)) == BTLSCR_ENEMY) {
        skip = 1;
    }

    do {
        active = ov07_022222B4(&ctx->pos);

        if (frame % 2) {
            if (skip == 0) {
                if (spriteIdx < 8) {
                    ManagedSprite_SetPositionXY(ctx->sprites[spriteIdx], ctx->pos.x, ctx->pos.y);
                    spriteIdx++;
                }
            } else {
                skip = 0;
            }
        }

        frame++;
    } while (active == 1);

    int baseBgPriority = ov07_0221FB04(ctx->common.battleAnimSys, 1);
    int effectBgPriority = ov07_0221FB04(ctx->common.battleAnimSys, 2);

    for (i = 0; i < 8; i++) {
        ctx->lifetime[i] = (8 - i) * 4;

        if (ov07_0221BFC0(ctx->common.battleAnimSys) == 0) {
            if (ov07_0223192C(ctx->common.battleAnimSys, ov07_0221C468(ctx->common.battleAnimSys)) == 0x3) {
                if (i < 4) {
                    ManagedSprite_SetPriority(ctx->sprites[i], effectBgPriority);
                } else {
                    ManagedSprite_SetPriority(ctx->sprites[i], baseBgPriority);
                }
            } else {
                if (i >= 4) {
                    ManagedSprite_SetPriority(ctx->sprites[i], effectBgPriority);
                } else {
                    ManagedSprite_SetPriority(ctx->sprites[i], baseBgPriority);
                }
            }

            ManagedSprite_SetDrawPriority(ctx->sprites[i], 8 - i);
        } else {
            ManagedSprite_SetDrawPriority(ctx->sprites[i], i);
            ManagedSprite_SetPriority(ctx->sprites[i], effectBgPriority);
        }
    }

    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222B2A8, ctx);
}
