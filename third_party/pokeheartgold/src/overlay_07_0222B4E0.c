#include "global.h"

#include "battle/battler_sprite_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_resources_header.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
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
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_palette_fade.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

struct SysTaskQueue;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

NNSG2dImagePaletteProxy *Sprite_GetPaletteProxy(Sprite *paletteProxy);

void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

void ManagedSprite_TickFrame(ManagedSprite *managedSprite);

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

void ManagedSprite_SetAffineScale(ManagedSprite *managedSprite, f32 xScale, f32 yScale);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

BOOL ov07_02222EE8(PaletteFadeContext *ctx);

void ov07_02222EF8(PaletteFadeContext *ctx);

PaletteFadeContext *ov07_02222F10(PaletteData *paletteData, enum HeapID heapID, enum PaletteBufferId bufferID, u16 index, u16 count, u8 stepFrames, s8 step, u8 startFrac, u8 endFrac, u16 color, int priority);

void Heap_Free(void *ptr);

u32 ObjPlttTransfer_GetPaletteVramOffset(const NNSG2dImagePaletteProxy *paletteProxy, NNS_G2D_VRAM_TYPE vramType);

enum RolePlaySprite {
    ROLE_PLAY_SPRITE_ATTACKER_0 = 0,
    ROLE_PLAY_SPRITE_ATTACKER_1,
    ROLE_PLAY_SPRITE_DEFENDER,
    ROLE_PLAY_SPRITE_COUNT
};

typedef struct RolePlayContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprites[ROLE_PLAY_SPRITE_COUNT];
    int blendStep;
    f32 scaleX;
    f32 scaleY;
    u8 spriteAlpha;
    u8 bgAlpha;
    s8 scaleDirX;
    PaletteFadeContext *palFades[2];
} RolePlayContext;

enum RolePlayState {
    ROLE_PLAY_STATE_SCALE = 0,
    ROLE_PLAY_STATE_WAIT_FOR_FADE,
    ROLE_PLAY_STATE_CLEANUP,
};

void ov07_0222B4E0(SysTask *task, void *param);

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};
void ManagedSprite_SetPriority(ManagedSprite *managedSprite, int priority);
void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);
void ManagedSprite_GetPositionXY(ManagedSprite *managedSprite, s16 *outX, s16 *outY);
BOOL ov07_0221BFC0(BattleAnimSystem *system);
/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov07_0221FA10(BattleAnimSystem *system, int battler);
u16 ov07_0221FA1C(BattleAnimSystem *system, int battler);
u8 ov07_0221FA2C(BattleAnimSystem *system, int battler);
#else
int ov07_0221FA10(BattleAnimSystem *system, int battler);
int ov07_0221FA1C(BattleAnimSystem *system, int battler);
int ov07_0221FA2C(BattleAnimSystem *system, int battler);
#endif
int ov07_0221FA38(BattleAnimSystem *system, int battler);
BOOL ov07_0221FB78(BattleAnimSystem *system, int battlerRole);
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
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov07_02231924(BattleAnimSystem *system, int battler);
#else
int ov07_02231924(BattleAnimSystem *system, int battler);
#endif
enum Battler ov07_0223192C(BattleAnimSystem *system, int battler);
void ov07_02231A20(int battler, BOOL isDoubles, Point2D *pos);
void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);
u8 GetMonPicHeightBySpeciesGenderForm(u16 species, u8 gender, u8 face, u8 form, u32 personality);
void ov07_0222B640(BattleAnimSystem *system);
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
void ov07_0221C7B8(BattleAnimSystem *system, BattlerSpriteContext *battlerSpriteCtx, int animParticipantMode);
void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);
BOOL ov07_022222B4(XYTransformContext *ctx);
void ov07_0223475C(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);
void ov07_0223494C(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);
typedef struct SnatchContext {
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo spriteInfo;
    XYTransformContext pos;
    s16 dir;
    BOOL doRev;
    Point2D battlerPositions[2];
    int unk_64;
    int iteration;
    int xpos[3];
    int ypos[3];
    int zpos[3];
} SnatchContext;
enum SnatchState {
    SNATCH_STATE_INIT = 0,
    SNATCH_STATE_MOVE,
};
void ov07_0222B898(SysTask *task, void *param);
void ov07_02231A50(BattleAnimSystem *system, int battler, Point2D *pos);
void ov07_02232020(BattleAnimSystem *system, int battler, BattleAnimSpriteInfo *sprites, int *count);
void ov07_0222B988(BattleAnimSystem *system);

void ov07_0222B4E0(SysTask *task, void *param) {
    RolePlayContext *ctx = param;

    switch (ctx->common.state) {
    case ROLE_PLAY_STATE_SCALE:
        if (ctx->blendStep > 14) {
            ctx->scaleY -= (f32)0.2;
            ctx->scaleX += (f32)0.2;

            if (ctx->scaleY <= 0.2) {
                ctx->common.state++;
                ManagedSprite_SetDrawFlag(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], 0);
            } else {
                ManagedSprite_SetAffineScale(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], ctx->scaleX * ctx->scaleDirX, ctx->scaleY);
            }
        } else {
            ctx->spriteAlpha++;
            ctx->bgAlpha--;
            G2_ChangeBlendAlpha(ctx->spriteAlpha, ctx->bgAlpha);
            ctx->blendStep++;
        }
        break;
    case ROLE_PLAY_STATE_WAIT_FOR_FADE:
        if (ov07_02222EE8(ctx->palFades[1]) == 0) {
            ov07_02222EF8(ctx->palFades[0]);
            ov07_02222EF8(ctx->palFades[1]);

            int palIndex = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(ctx->sprites[ROLE_PLAY_SPRITE_DEFENDER]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);

            ctx->palFades[1] = ov07_02222F10(
                ctx->common.paletteData,
                ov07_0221BFD0(ctx->common.battleAnimSys),
                PLTTBUF_MAIN_OBJ,
                ((palIndex) * (16)),
                (16),
                0,
                1,
                15,
                0,
                ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
                1100);
            ctx->common.state++;
        }
        break;
    case ROLE_PLAY_STATE_CLEANUP:
    default:
        if (ov07_02222EE8(ctx->palFades[1]) == 0) {
            ManagedSprite_TickFrame(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_1]);
            ov07_02222EF8(ctx->palFades[1]);
            ov07_0221C448(ctx->common.battleAnimSys, task);
            Heap_Free(ctx);
            return;
        }
        break;
    }

    ManagedSprite_TickFrame(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0]);
    ManagedSprite_TickFrame(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_1]);
    ManagedSprite_TickFrame(ctx->sprites[ROLE_PLAY_SPRITE_DEFENDER]);
    SpriteSystem_DrawSprites(ctx->common.pokemonSpriteManager);
}

void ov07_0222B640(BattleAnimSystem *system) {
    RolePlayContext *ctx = ov07_022324D8(system, sizeof(RolePlayContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0] = ov07_0221C4E8(ctx->common.battleAnimSys, 0);
    ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_1] = ov07_0221C4E8(ctx->common.battleAnimSys, 1);
    ctx->sprites[ROLE_PLAY_SPRITE_DEFENDER] = ov07_0221C4E8(ctx->common.battleAnimSys, 2);
    ctx->blendStep = 0;

    s16 xOffset = (-32);
    s16 dir = ov07_02222004(system, ov07_0221C468(system));
    xOffset *= dir;

    ctx->scaleX = 1.0f;
    ctx->scaleY = 1.0f;

    if (ov07_0221FB78(system, 1) == 1) {
        ctx->scaleDirX = -1;
    } else {
        ctx->scaleDirX = +1;
    }

    s16 posX, posY;
    ManagedSprite_GetPositionXY(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_1], &posX, &posY);
    ManagedSprite_SetPositionXY(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], posX + xOffset, posY);
    ManagedSprite_SetAffineOverwriteMode(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], AFFINE_OVERWRITE_MODE_DOUBLE);
    ManagedSprite_SetAffineScale(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], ctx->scaleX * ctx->scaleDirX, ctx->scaleY);

    int palIndex = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    ctx->palFades[0] = ov07_02222F10(
        ctx->common.paletteData,
        ov07_0221BFD0(system),
        PLTTBUF_MAIN_OBJ,
        ((palIndex) * (16)),
        (16),
        0,
        1,
        0,
        15,
        ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
        1100);

    palIndex = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(ctx->sprites[ROLE_PLAY_SPRITE_DEFENDER]->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    ctx->palFades[1] = ov07_02222F10(
        ctx->common.paletteData,
        ov07_0221BFD0(system),
        PLTTBUF_MAIN_OBJ,
        ((palIndex) * (16)),
        (16),
        0,
        1,
        0,
        15,
        ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
        1100);

    u8 attacker = ov07_0221C468(ctx->common.battleAnimSys);
    u8 attackerType = ov07_02231924(ctx->common.battleAnimSys, attacker);

    Point2D attackerDefaultPos;
    Point2D attackerPos;
    ov07_02231A20(attackerType, ov07_0221BFC0(ctx->common.battleAnimSys), &attackerDefaultPos);
    ManagedSprite_GetPositionXY(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], &attackerPos.x, &attackerPos.y);

    int face;
    if (ov07_0223192C(ctx->common.battleAnimSys, attacker) == BTLSCR_PLAYER) {
        face = 0;
        ManagedSprite_SetPriority(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], 1);
        ManagedSprite_SetDrawPriority(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], 0);
    } else {
        ManagedSprite_SetPriority(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], 2);
        ManagedSprite_SetDrawPriority(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], 0);
        face = 2;
    }

    u8 defender = ov07_0221C470(ctx->common.battleAnimSys);
    u8 defenderYOffset = GetMonPicHeightBySpeciesGenderForm(
        ov07_0221FA1C(ctx->common.battleAnimSys, defender),
        ov07_0221FA10(ctx->common.battleAnimSys, defender),
        face,
        ov07_0221FA2C(ctx->common.battleAnimSys, defender),
        ov07_0221FA38(ctx->common.battleAnimSys, defender));

    ManagedSprite_SetPositionXY(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], attackerPos.x, attackerDefaultPos.y + defenderYOffset);
    ManagedSprite_SetDrawFlag(ctx->sprites[ROLE_PLAY_SPRITE_ATTACKER_0], 1);

    ctx->spriteAlpha = 0;
    ctx->bgAlpha = 15;

    ov07_02231E08(ctx->common.battleAnimSys, -1, -1);
    G2_ChangeBlendAlpha(ctx->spriteAlpha, ctx->bgAlpha);

    SysTask *task = ov07_0221C410(ctx->common.battleAnimSys, ov07_0222B4E0, ctx);
    ov07_0222B4E0(task, ctx);
}

void ov07_0222B898(SysTask *task, void *param) {
    SnatchContext *ctx = param;

    switch (ctx->common.state) {
    case SNATCH_STATE_INIT:
        ov07_02222268(
            &ctx->pos,
            ctx->spriteInfo.pos.x,
            ctx->xpos[ctx->iteration],
            ctx->ypos[ctx->iteration],
            ctx->ypos[ctx->iteration],
            15);

        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Z_CENTER, ctx->zpos[ctx->iteration]);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y, 1);
        ctx->common.state++;
    case SNATCH_STATE_MOVE:
        if (ov07_022222B4(&ctx->pos) == 0) {
            ctx->iteration++;

            if (ctx->iteration > (3 - 1)) {
                ctx->common.state++;
            } else {
                BattlerSpriteContext battlerSpriteCtx;

                if (ctx->doRev == 0) {
                    ov07_0221C7B8(ctx->common.battleAnimSys, &battlerSpriteCtx, 3);

                    if (ctx->iteration == 2) {
                        ov07_0223494C(&battlerSpriteCtx, ov07_0221BFD0(ctx->common.battleAnimSys));
                    } else {
                        ov07_0223475C(&battlerSpriteCtx, ov07_0221BFD0(ctx->common.battleAnimSys));
                    }
                }

                ctx->common.state = SNATCH_STATE_INIT;
            }

            ctx->spriteInfo.pos.x = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_X_CENTER);
        }

        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_X_CENTER, ctx->pos.x);
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER, ctx->pos.y);
        break;
    default:
        Pokepic_SetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y, 0);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}

void ov07_0222B988(BattleAnimSystem *system) {
    SnatchContext *ctx = ov07_022324D8(system, sizeof(SnatchContext));
    ov07_02231FE4(system, &ctx->common);

    int attacker = ov07_0221C468(ctx->common.battleAnimSys);

    ov07_02231A50(
        ctx->common.battleAnimSys,
        ov07_0221C468(ctx->common.battleAnimSys),
        &ctx->battlerPositions[0]);

    ov07_02231A50(
        ctx->common.battleAnimSys,
        ov07_0221C470(ctx->common.battleAnimSys),
        &ctx->battlerPositions[1]);

    if (ov07_0223192C(ctx->common.battleAnimSys, attacker) == BTLSCR_PLAYER) {
        ctx->dir = +1;
    } else {
        ctx->dir = -1;
    }

    int attackerSide = ov07_0223192C(ctx->common.battleAnimSys, ov07_0221C468(ctx->common.battleAnimSys));
    int defenderSide = ov07_0223192C(ctx->common.battleAnimSys, ov07_0221C470(ctx->common.battleAnimSys));

    ctx->doRev = attackerSide == defenderSide;
    ctx->iteration = 0;

    int count;
    ov07_02232020(system, ov07_0221C4A8(system, 0), &ctx->spriteInfo, &count);

    int height = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Y_CENTER) - ctx->battlerPositions[0].y;

    if (ctx->dir > 0) {
        ctx->xpos[0] = 256 + (10 * 8) - 1;
        ctx->xpos[1] = -(10 * 8);
        ctx->xpos[2] = ctx->battlerPositions[0].x;

        ctx->ypos[0] = ctx->battlerPositions[0].y + height;
        ctx->ypos[1] = ctx->battlerPositions[1].y + height;
        ctx->ypos[2] = ctx->battlerPositions[0].y + height;

        ctx->zpos[0] = 10;
        ctx->zpos[1] = (-650);
        ctx->zpos[2] = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Z_CENTER);
    } else {
        ctx->xpos[0] = -(10 * 8);
        ctx->xpos[1] = 256 + (10 * 8) - 1;
        ctx->xpos[2] = ctx->battlerPositions[0].x;

        ctx->ypos[0] = ctx->battlerPositions[0].y + height;
        ctx->ypos[1] = ctx->battlerPositions[1].y + height;
        ctx->ypos[2] = ctx->battlerPositions[0].y + height;

        ctx->zpos[0] = (-650);
        ctx->zpos[1] = 10;
        ctx->zpos[2] = Pokepic_GetAttr(ctx->spriteInfo.monSprite, MON_SPRITE_Z_CENTER);
    }

    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222B898, ctx);
}
