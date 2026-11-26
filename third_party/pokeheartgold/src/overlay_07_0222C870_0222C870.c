#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_bg_scroll_context.h"
#include "pokepic.h"
#include "overlay_07_acid_armor_context.h"

struct BattleAnimSystem;

struct BgScrollContext;

struct Pokepic;

struct PokepicDrawParam;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

BOOL ov07_022222B4(XYTransformContext *ctx);

void *ov07_02222D60(const BgScrollContext *ctx);

u32 ov07_02222D88(s32 x, s32 y);

static BOOL ov07_0222C870(AcidArmorContext *ctx);

static BOOL ov07_0222C870(AcidArmorContext *ctx) {
    u32 *writeBuffer = ov07_02222D60(ctx->bgScroll);
    ov07_0222C850(ctx, writeBuffer);

    BOOL active = ov07_022222B4(&ctx->pos);

    for (int y = ctx->scrollStartY; y <= ctx->scrollEndY; y++) {
        s16 xOff = writeBuffer[y] & 0xFFFF;
        s16 yOff = writeBuffer[y] >> 16;

        yOff -= ((ctx->scrollEndY - y) * ctx->pos.y) / 100;
        xOff -= ((ctx->scrollStartY + (10 * 8) / 2 - y) * ctx->pos.x) / 100;

        int prevY = y - 1;
        if (prevY < 0) {
            prevY += 192;
        }

        writeBuffer[prevY] = ov07_02222D88((u16)xOff, (u16)yOff);
    }

    return active;
}

struct BgConfig;

struct SysTask;

struct SysTaskQueue;

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

enum BgOffsetUpdateOp {
    BG_OFFSET_UPDATE_SET_X = 0,
    BG_OFFSET_UPDATE_ADD_X,
    BG_OFFSET_UPDATE_SUB_X,
    BG_OFFSET_UPDATE_SET_Y,
    BG_OFFSET_UPDATE_ADD_Y,
    BG_OFFSET_UPDATE_SUB_Y,
};

typedef struct BgConfig BgConfig;

void SetBgPriority(u8 bgLayer, u8 priority);

void BgSetPosTextAndCommit(BgConfig *bgConfig, u8 bgLayer, int bgOffsetUpdateOp, int val);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

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

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

int ov07_0221FAE8(BattleAnimSystem *system);

enum BgLayer ov07_0221FAEC(BattleAnimSystem *system, int bg);

int ov07_0221FB04(BattleAnimSystem *system, int bg);

void ov07_02222D3C(BgScrollContext *ctx);

void ov07_02222D74(BgScrollContext *ctx);

void Heap_Free(void *ptr);

enum AcidArmorState {
    ACID_ARMOR_STATE_SCROLL = 0,
    ACID_ARMOR_STATE_FADE,
    ACID_ARMOR_STATE_WAIT,
    ACID_ARMOR_STATE_FADE_BACK,
    ACID_ARMOR_STATE_CLEANUP,
};

void ov07_0222C918(SysTask *task, void *param);

void ov07_0222C918(SysTask *task, void *param) {
    AcidArmorContext *ctx = param;

    switch (ctx->state) {
    case ACID_ARMOR_STATE_SCROLL:
        if (ov07_0222C870(ctx) == 0) {
            ctx->state++;
            ov07_02231E74(ctx->battleAnimSys, 31, 0);
            ov07_02222AC4(
                &ctx->alphaFade,
                31,
                0,
                0,
                31,
                16);
        }
        break;
    case ACID_ARMOR_STATE_FADE:
        ov07_0222C870(ctx);

        if (ov07_02222AF4(&ctx->alphaFade)) {
            ctx->state++;

            ov07_02222D74(ctx->bgScroll);
            ctx->delay = 0;
        }
        break;
    case ACID_ARMOR_STATE_WAIT:
        BgSetPosTextAndCommit(ov07_0221C4A0(ctx->battleAnimSys), BG_LAYER_MAIN_2, BG_OFFSET_UPDATE_SET_X, -ctx->attackerX);
        BgSetPosTextAndCommit(ov07_0221C4A0(ctx->battleAnimSys), BG_LAYER_MAIN_2, BG_OFFSET_UPDATE_SET_Y, -ctx->attackerY);

        ctx->delay++;

        if (ctx->delay > 8) {
            ov07_02222AC4(
                &ctx->alphaFade,
                0,
                31,
                31,
                0,
                16);
            ctx->state++;
        }
        break;
    case ACID_ARMOR_STATE_FADE_BACK:
        if (ov07_02222AF4(&ctx->alphaFade)) {
            Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_HIDE, 0);
            ctx->state++;
        }
        break;
    case ACID_ARMOR_STATE_CLEANUP:
        ov07_02222D3C(ctx->bgScroll);

        SetBgPriority(
            ov07_0221FAEC(ctx->battleAnimSys, 1),
            ov07_0221FB04(ctx->battleAnimSys, 1));
        SetBgPriority(BG_LAYER_MAIN_0, ov07_0221FAE8(ctx->battleAnimSys));

        BG_ClearCharDataRange(
            ov07_0221FAEC(ctx->battleAnimSys, 1),
            0x4000,
            0,
            ov07_0221BFD0(ctx->battleAnimSys));

        BgClearTilemapBufferAndCommit(
            ov07_0221C4A0(ctx->battleAnimSys),
            ov07_0221FAEC(ctx->battleAnimSys, 1));

        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    default:
        break;
    }
}
