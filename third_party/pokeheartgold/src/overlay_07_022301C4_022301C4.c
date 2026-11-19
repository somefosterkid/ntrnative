#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "pokepic.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_battle_anim_system.h"
#include "bg_window.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;

struct CustomBgScrollContext;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

struct SysTaskQueue;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void *ov07_02222C84(const CustomBgScrollContext *ctx);

u32 ov07_02222D88(s32 x, s32 y);

typedef struct ExtrasensoryContext {
    BattleAnimSystem *battleAnimSys;
    CustomBgScrollContext *bgScroll;
    int state;
    Pokepic *defenderSprite;
    int y;
    int centerY;
    int baseAngle;
    fx32 baseAmplitude;
    int dir;
    int amplitudeStep;
    int delay;
    int count;
    u32 initValue;
} ExtrasensoryContext;

static void ov07_022301C4(ExtrasensoryContext *ctx);

static void ov07_022301C4(ExtrasensoryContext *ctx) {
    u32 *buffer = ov07_02222C84(ctx->bgScroll);

    int startY = ctx->y + (-8);
    int endY = ctx->y + (10 * 8) - (-8);

    if (startY < 0) {
        startY = 0;
    }

    if (endY > 192) {
        endY = 192;
    }

    ctx->dir *= -1;

    for (int y = startY; y < endY; y++) {

        int amplitude;
        if (y & 2) {
            amplitude = ctx->baseAmplitude + (((fx32)(((1) > 0) ? ((1) * (1 << 12) + 0.5f) : ((1) * (1 << 12) - 0.5f))) * ctx->dir);
        } else {
            amplitude = ctx->baseAmplitude - (((fx32)(((1) > 0) ? ((1) * (1 << 12) + 0.5f) : ((1) * (1 << 12) - 0.5f))) * ctx->dir);
        }

        u16 angle = ctx->baseAngle + ((((180) * 0xFFFF) / 360) / (10 * 8)) * (y - startY);
        s16 xOffset = FX_MulInline(FX_SinIdx(angle), amplitude) >> 12;
        xOffset += ((y - ctx->centerY) * ctx->amplitudeStep) / 10;
        s16 initX = ctx->initValue & 0xFFFF;
        s16 initY = ctx->initValue >> 16;

        int index = y - 1;
        if (index < 0) {
            index += 192;
        }

        buffer[index] = ov07_02222D88((u16)(initX + xOffset), (u16)initY);
    }
}

struct BgConfig;

void SetBgPriority(u8 bgLayer, u8 priority);

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

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

BOOL ov07_0221FAC8(BattleAnimSystem *system, int battler);

int ov07_0221FAE8(BattleAnimSystem *system);

enum GFBgLayer ov07_0221FAEC(BattleAnimSystem *system, int bg);

int ov07_0221FB04(BattleAnimSystem *system, int bg);

void ov07_02222C60(CustomBgScrollContext *ctx);

void ov07_02222C98(CustomBgScrollContext *ctx);

void Heap_Free(void *ptr);

enum ExtrasensoryState {
    EXTRASENSORY_STATE_SET_PARAMS = 0,
    EXTRASENSORY_STATE_SCROLL,
    EXTRASENSORY_STATE_CLEANUP,
};

void ov07_02230170(ExtrasensoryContext *ctx);

static void ov07_022302A8(SysTask *task, void *param);

static void ov07_022302A8(SysTask *task, void *param) {
    ExtrasensoryContext *ctx = param;

    switch (ctx->state) {
    case EXTRASENSORY_STATE_SET_PARAMS:
        ov07_02230170(ctx);
        ov07_022301C4(ctx);
        ctx->state++;
        break;
    case EXTRASENSORY_STATE_SCROLL:
        ctx->delay--;
        ov07_022301C4(ctx);

        if (ctx->delay < 0) {
            ctx->count++;

            if (ctx->count < 3) {
                ctx->state = EXTRASENSORY_STATE_SET_PARAMS;
            } else {
                ctx->state++;
            }
        }
        break;
    case EXTRASENSORY_STATE_CLEANUP:
        if (ov07_0221FAC8(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys)) == 0) {
            Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_HIDE, 0);
        }

        ov07_02222C98(ctx->bgScroll);
        ov07_02222C60(ctx->bgScroll);

        SetBgPriority(
            ov07_0221FAEC(ctx->battleAnimSys, 1),
            ov07_0221FB04(ctx->battleAnimSys, 1));
        SetBgPriority(GF_BG_LYR_MAIN_0, ov07_0221FAE8(ctx->battleAnimSys));

        BG_ClearCharDataRange(
            ov07_0221FAEC(ctx->battleAnimSys, 1),
            0x4000,
            0,
            ov07_0221BFD0(ctx->battleAnimSys));
        BgClearTilemapBufferAndCommit(
            ov07_0221C4A0(ctx->battleAnimSys),
            ov07_0221FAEC(ctx->battleAnimSys, 1));

        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        break;
    }
}

int ov07_0221FAF8(BattleAnimSystem *system, int bg);

CustomBgScrollContext *ov07_02222BE4(void *offsetReg, u32 initValue, enum HeapID heapID);

void *ov07_02222D90(int bgID);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_022303A4(BattleAnimSystem *system);

void ov07_022303A4(BattleAnimSystem *system) {
    ExtrasensoryContext *ctx = ov07_022324D8(system, sizeof(ExtrasensoryContext));

    ctx->battleAnimSys = system;
    ctx->defenderSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C470(system));

    s16 defenderX = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER);
    s16 defenderY = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER);
    defenderY -= Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_SHADOW_HEIGHT);

    if (ov07_0221FAC8(ctx->battleAnimSys, ov07_0221C470(ctx->battleAnimSys)) == 0) {
        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_HIDE, 1);
    }

    ctx->centerY = defenderY;
    defenderX -= ((10 * 8) / 2);
    ctx->y = defenderY - ((10 * 8) / 2);

    ctx->initValue = ov07_02222D88((u16)-defenderX, (u16)-ctx->y);
    ctx->bgScroll = ov07_02222BE4(
        ov07_02222D90(ov07_0221FAF8(ctx->battleAnimSys, 1)),
        ctx->initValue,
        ov07_0221BFD0(ctx->battleAnimSys));
    ctx->dir = 1;

    int type = ov07_02231924(ctx->battleAnimSys, ov07_0221C470(system));
    if (type == 3 || type == 4) {
        SetBgPriority(
            ov07_0221FAEC(ctx->battleAnimSys, 1),
            ov07_0221FAE8(ctx->battleAnimSys));
        SetBgPriority(GF_BG_LYR_MAIN_0, ov07_0221FAE8(ctx->battleAnimSys) + 1);
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_022302A8, ctx);
}

struct PaletteData;

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

int ov07_0221E6C8(BattleAnimSystem *system);

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BOOL ov07_022222B4(XYTransformContext *ctx);

enum BattleAnimWindow {
    BATTLE_ANIM_WINDOW_0 = 0,
    BATTLE_ANIM_WINDOW_1,
};

void ov07_02231EC0(BattleAnimSystem *system, enum BattleAnimWindow window, BOOL applyEffects);

typedef struct FakeOutCurtainContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    int delay;
    XYTransformContext curtainPos;
} FakeOutCurtainContext;

enum FakeOutCurtainState {
    FAKE_OUT_CURTAIN_STATE_INIT = 0,
    FAKE_OUT_CURTAIN_STATE_WAIT,
    FAKE_OUT_CURTAIN_STATE_MOVE_CURTAIN,
    FAKE_OUT_CURTAIN_STATE_HIDE_CURTAIN,
    FAKE_OUT_CURTAIN_STATE_WAIT_FOR_FADE,
    FAKE_OUT_CURTAIN_STATE_CLEANUP,
};

void ov07_0223049C(SysTask *task, void *param);

void ov07_0223049C(SysTask *task, void *param) {
    FakeOutCurtainContext *ctx = param;

    switch (ctx->state) {
    case FAKE_OUT_CURTAIN_STATE_INIT:
        ov07_02222268(
            &ctx->curtainPos,
            127,
            0,
            0,
            0,
            8);
        GX_SetVisibleWnd(GX_WNDMASK_W0);
        ov07_02231EC0(ctx->battleAnimSys, BATTLE_ANIM_WINDOW_0, 0);
        G2_SetWnd0Position(0, 0, 256 - 1, 192 - 1);
        ctx->delay = 7;
        ctx->state++;
        break;
    case FAKE_OUT_CURTAIN_STATE_WAIT:
        ctx->delay--;

        if (ctx->delay < 0) {
            PaletteData_BeginPaletteFade(
                ov07_0221FA78(ctx->battleAnimSys),
                (1 << PLTTBUF_MAIN_BG),
                ov07_0221E6C8(ctx->battleAnimSys),
                0,
                0,
                16,
                ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))));
            ctx->state++;
        }
        break;
    case FAKE_OUT_CURTAIN_STATE_MOVE_CURTAIN:
        if (ov07_022222B4(&ctx->curtainPos)) {
            G2_SetWnd0Position(
                (256 / 2 - 1) - ctx->curtainPos.x,
                0,
                (256 / 2) + ctx->curtainPos.x,
                192 - 1);
        } else {
            ctx->state++;
        }
        break;
    case FAKE_OUT_CURTAIN_STATE_HIDE_CURTAIN:
        if (PaletteData_GetSelectedBuffersBitmask(ov07_0221FA78(ctx->battleAnimSys)) == 0) {
            ctx->state++;
            GX_SetVisibleWnd(GX_WNDMASK_NONE);
            PaletteData_BeginPaletteFade(
                ov07_0221FA78(ctx->battleAnimSys),
                (1 << PLTTBUF_MAIN_BG),
                ov07_0221E6C8(ctx->battleAnimSys),
                0,
                16,
                0,
                ((GXRgba)(((31) << (0)) | ((31) << (5)) | ((31) << (10)) | ((1) << (15)))));
        }
        break;
    case FAKE_OUT_CURTAIN_STATE_WAIT_FOR_FADE:
        if (PaletteData_GetSelectedBuffersBitmask(ov07_0221FA78(ctx->battleAnimSys)) == 0) {
            ctx->state++;
        }
        break;
    case FAKE_OUT_CURTAIN_STATE_CLEANUP:
        Heap_Free(ctx);
        ov07_0221C448(ctx->battleAnimSys, task);
        break;
    }
}
