#include "global.h"
#include "constants/heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"
#include "overlay_07_palette_fade.h"

struct BattleAnimSystem;

struct ManagedSprite;

struct PaletteData;

struct PaletteFadeContext;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

void Sprite_DeleteAndFreeResources(ManagedSprite *managedSprite);

void ManagedSprite_SetOamMode(ManagedSprite *managedSprite, GXOamMode mode);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

void ov07_0222A8D8(ManagedSprite *sprite);

void ov07_02222EF8(PaletteFadeContext *ctx);

PaletteFadeContext *ov07_02222F10(PaletteData *paletteData, enum HeapID heapID, enum PaletteBufferId bufferID, u16 index, u16 count, s8 stepFrames, s8 step, u8 startFrac, u8 endFrac, u16 color, int priority);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

void Heap_Free(void *ptr);

typedef struct IngrainSpriteContext {
    BattleAnimScriptFuncCommon common;
    int paletteOffset;
    s16 unused0;
    s16 delayCounters[4];
    ManagedSprite *vineSprites[4];
    PaletteFadeContext *paletteFades[6];
    int bgAlpha;
    int spriteAlpha;
} IngrainSpriteContext;

static void ov07_0222AA20(SysTask *task, void *param);

static void ov07_0222AA20(SysTask *task, void *param) {
    IngrainSpriteContext *ctx = param;
    int doneCount = 0;

    int i;
    for (i = 0; i < 4; i++) {
        ctx->delayCounters[i]++;

        if (ctx->delayCounters[i] < 20) {
            continue;
        }

        ov07_0222A8D8(ctx->vineSprites[i]);

        if (ctx->delayCounters[0] == 110) {
            ov07_02231E08(ctx->common.battleAnimSys, -1, -1);

            ManagedSprite_SetOamMode(ctx->vineSprites[0], GX_OAM_MODE_XLU);
            ManagedSprite_SetOamMode(ctx->vineSprites[1], GX_OAM_MODE_XLU);
            ManagedSprite_SetOamMode(ctx->vineSprites[2], GX_OAM_MODE_XLU);
            ManagedSprite_SetOamMode(ctx->vineSprites[3], GX_OAM_MODE_XLU);

            ctx->bgAlpha = 15;
            ctx->spriteAlpha = 0;
        }
    }

    if (ctx->delayCounters[0] == 50) {
        ctx->paletteFades[0] = ov07_02222F10(
            ctx->common.paletteData,
            ov07_0221BFD0(ctx->common.battleAnimSys),
            PLTTBUF_MAIN_OBJ,
            ((ctx->paletteOffset) * (16)),
            (16),
            (-2),
            2,
            0,
            14,
            ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
            1002);
    }

    if (ctx->delayCounters[0] == 60) {
        ctx->paletteFades[1] = ov07_02222F10(
            ctx->common.paletteData,
            ov07_0221BFD0(ctx->common.battleAnimSys),
            PLTTBUF_MAIN_OBJ,
            ((ctx->paletteOffset) * (16)),
            (16),
            (-2),
            2,
            14,
            0,
            ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
            1002);
    }

    if (ctx->delayCounters[0] == 70) {
        ctx->paletteFades[2] = ov07_02222F10(
            ctx->common.paletteData,
            ov07_0221BFD0(ctx->common.battleAnimSys),
            PLTTBUF_MAIN_OBJ,
            ((ctx->paletteOffset) * (16)),
            (16),
            (-2),
            2,
            0,
            14,
            ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
            1002);
    }

    if (ctx->delayCounters[0] == 80) {
        ctx->paletteFades[3] = ov07_02222F10(
            ctx->common.paletteData,
            ov07_0221BFD0(ctx->common.battleAnimSys),
            PLTTBUF_MAIN_OBJ,
            ((ctx->paletteOffset) * (16)),
            (16),
            (-2),
            2,
            14,
            0,
            ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
            1002);
    }

    if (ctx->delayCounters[0] == 90) {
        ctx->paletteFades[4] = ov07_02222F10(
            ctx->common.paletteData,
            ov07_0221BFD0(ctx->common.battleAnimSys),
            PLTTBUF_MAIN_OBJ,
            ((ctx->paletteOffset) * (16)),
            (16),
            (-2),
            2,
            0,
            14,
            ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
            1002);
    }

    if (ctx->delayCounters[0] == 100) {
        ctx->paletteFades[5] = ov07_02222F10(
            ctx->common.paletteData,
            ov07_0221BFD0(ctx->common.battleAnimSys),
            PLTTBUF_MAIN_OBJ,
            ((ctx->paletteOffset) * (16)),
            (16),
            (-2),
            2,
            14,
            0,
            ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))),
            1002);
    }

    if (ctx->delayCounters[0] == 110) {
        ov07_02231E08(ctx->common.battleAnimSys, -1, -1);

        ManagedSprite_SetOamMode(ctx->vineSprites[0], GX_OAM_MODE_XLU);
        ManagedSprite_SetOamMode(ctx->vineSprites[1], GX_OAM_MODE_XLU);
        ManagedSprite_SetOamMode(ctx->vineSprites[2], GX_OAM_MODE_XLU);
        ManagedSprite_SetOamMode(ctx->vineSprites[3], GX_OAM_MODE_XLU);

        ctx->bgAlpha = 15;
        ctx->spriteAlpha = 0;
    }

    if (ctx->delayCounters[0] >= 110) {
        if (ctx->bgAlpha > 0) {
            ctx->bgAlpha--;
        }

        if (ctx->spriteAlpha < 15) {
            ctx->spriteAlpha++;
        }

        G2_ChangeBlendAlpha(ctx->bgAlpha, ctx->spriteAlpha);

        if (ctx->bgAlpha == 0 && ctx->spriteAlpha == 15) {
            doneCount = 4;
        }
    }

    if (doneCount == 4) {
        for (i = 0; i < 4; i++) {
            Sprite_DeleteAndFreeResources(ctx->vineSprites[i]);
        }

        for (i = 0; i < 6; i++) {
            ov07_02222EF8(ctx->paletteFades[i]);
        }

        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}

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

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

void ManagedSprite_OffsetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

void ManagedSprite_SetFlipMode(ManagedSprite *managedSprite, BOOL mode);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

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

u16 LCRandom(void);

enum Battler ov07_0223192C(BattleAnimSystem *system, int battler);

void ov07_0222ACAC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_0222ACAC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    IngrainSpriteContext *ctx = ov07_022324D8(system, sizeof(IngrainSpriteContext));
    ctx->unused0 = 10;

    ov07_02231FE4(system, &ctx->common);

    ManagedSpriteTemplate template;
    template = ov07_0221F9E8(ctx->common.battleAnimSys);

    ctx->vineSprites[0] = sprite;
    ctx->delayCounters[0] = 0;

    int i;
    for (i = 1; i < 4; i++) {
        ctx->delayCounters[i] = (LCRandom() % 4) + i * (20 / 4);
        ctx->vineSprites[i] = SpriteSystem_NewSprite(ctx->common.spriteSystem, ctx->common.primarySpriteManager, &template);
    }

    ManagedSprite_SetFlipMode(ctx->vineSprites[1], 1);
    ManagedSprite_SetFlipMode(ctx->vineSprites[2], 1);

    int attacker = ov07_0221C468(system);
    Pokepic *attackerSprite = ov07_0221FA48(system, attacker);
    s16 vineX = Pokepic_GetAttr(attackerSprite, MON_SPRITE_X_CENTER);
    s16 vineY = Pokepic_GetAttr(attackerSprite, MON_SPRITE_Y_CENTER);
    vineY += Pokepic_GetAttr(attackerSprite, MON_SPRITE_SHADOW_HEIGHT);

    if (ov07_0223192C(system, ov07_0221C468(system)) == BTLSCR_PLAYER) {
        vineY = 140;
    } else {
        vineY = 84;
    }

    for (i = 0; i < 4; i++) {
        ManagedSprite_SetPositionXY(ctx->vineSprites[i], vineX, vineY);
    }

    if (ov07_0223192C(system, ov07_0221C468(system)) == BTLSCR_PLAYER) {
        ManagedSprite_SetDrawPriority(ctx->vineSprites[0], 10);
        ManagedSprite_SetDrawPriority(ctx->vineSprites[2], 10);
        ManagedSprite_SetDrawPriority(ctx->vineSprites[1], 18);
        ManagedSprite_SetDrawPriority(ctx->vineSprites[3], 18);
    } else {
        ManagedSprite_SetDrawPriority(ctx->vineSprites[0], 18);
        ManagedSprite_SetDrawPriority(ctx->vineSprites[2], 18);
        ManagedSprite_SetDrawPriority(ctx->vineSprites[1], 10);
        ManagedSprite_SetDrawPriority(ctx->vineSprites[3], 10);
    }

    s16 upper = 4;
    s16 lower = 0;

    ManagedSprite_OffsetPositionXY(ctx->vineSprites[0], -32, lower);
    ManagedSprite_OffsetPositionXY(ctx->vineSprites[3], -40, upper);
    ManagedSprite_OffsetPositionXY(ctx->vineSprites[2], +32, lower);
    ManagedSprite_OffsetPositionXY(ctx->vineSprites[1], +40, upper);

    ctx->paletteOffset = ManagedSprite_GetPaletteOverrideOffset(ctx->vineSprites[0]);
    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222AA20, ctx);
}
