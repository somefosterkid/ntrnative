#include "global.h"
#include "gf_gfx_planes.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "bg_window.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_earthquake_context.h"

struct BattleAnimSpriteInfo;

struct BattleAnimSystem;

struct BgConfig;

struct EarthquakeContext;

struct PaletteData;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct SpriteManager;

struct SysTask;

struct SysTaskQueue;

struct XYTransformContext;

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

void PaletteData_BlendPalettes(PaletteData *paletteData, enum PaletteBufferId bufferID, u16 toBlend, u8 fraction, u16 target);

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

int ov07_0221E6C8(BattleAnimSystem *system);

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

BOOL ov07_022227D8(XYTransformContext *ctx);

void Heap_Free(void *ptr);

enum EarthquakeState {
    EARTHQUAKE_STATE_INIT = 0,
    EARTHQUAKE_STATE_FADE_BG,
    EARTHQUAKE_STATE_FLASH,
    EARTHQUAKE_STATE_SHAKE,
};

extern const s16 ov07_02236722[8];

void ov07_02228B38(SysTask *task, void *param);

void ov07_02228B38(SysTask *task, void *param) {
    EarthquakeContext *ctx = param;

    switch (ctx->state) {
    case EARTHQUAKE_STATE_INIT:
        PaletteData_BeginPaletteFade(
            ov07_0221FA78(ctx->battleAnimSys),
            (1 << PLTTBUF_MAIN_BG),
            ov07_0221E6C8(ctx->battleAnimSys),
            1,
            0,
            10,
            (((0) << 10) | ((0) << 5) | (0)));
        ctx->state++;
        break;
    case EARTHQUAKE_STATE_FADE_BG:
        if (PaletteData_GetSelectedBuffersBitmask(ov07_0221FA78(ctx->battleAnimSys)) != 0) {
            break;
        }

        ctx->state++;
    case EARTHQUAKE_STATE_FLASH:
        ov07_022227A8(
            &ctx->shake,
            ov07_02236722[ctx->shakeCount],
            0,
            0,
            5);

        if (ctx->flashFlag == 0) {
            PaletteData_BlendPalettes(
                ov07_0221FA78(ctx->battleAnimSys),
                PLTTBUF_MAIN_BG,
                0xFF,
                10,
                (((0) << 10) | ((0) << 5) | (0)));
        } else {
            PaletteData_BlendPalettes(
                ov07_0221FA78(ctx->battleAnimSys),
                PLTTBUF_MAIN_BG,
                0xFF,
                10,
                (((31) << 10) | ((31) << 5) | (31)));
        }

        ctx->flashDuration = 3;
        ctx->flashFlag ^= 1;
        ctx->shakeCount++;
        ctx->state++;
    case EARTHQUAKE_STATE_SHAKE:
        if (ov07_022227D8(&ctx->shake) == 0) {
            if (ctx->shakeCount >= (sizeof(ov07_02236722) / sizeof(*(ov07_02236722)))) {
                ctx->state++;
            } else {
                ctx->state--;
            }
        }

        if (ctx->flashDuration > 0) {
            ctx->flashDuration--;

            if (ctx->flashDuration == 0) {
                PaletteData_BlendPalettes(
                    ov07_0221FA78(ctx->battleAnimSys),
                    PLTTBUF_MAIN_BG,
                    0xFF,
                    0,
                    (((0) << 10) | ((0) << 5) | (0)));
            }
        }

        for (int i = 0; i < 4; i++) {
            if (ctx->sprites[i].monSprite == 0L) {
                continue;
            }

            Pokepic_SetAttr(ctx->sprites[i].monSprite, MON_SPRITE_X_CENTER, ctx->sprites[i].basePos.x + ctx->shake.x);
        }

        BgSetPosTextAndCommit(ctx->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, ctx->shake.x);
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
