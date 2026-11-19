#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "pokepic.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_flash_context.h"
#include "overlay_07_palette_fade.h"

struct BattleAnimSystem;

struct PaletteData;

struct PaletteFadeContext;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

struct SysTaskQueue;

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);

BOOL Pokepic_ResumePaletteFade(Pokepic *monSprite);

int ov07_0221E6C8(BattleAnimSystem *system);

enum FlashState {
    FLASH_STATE_SETUP_FADE_OUT = 0,
    FLASH_STATE_FADE_OUT,
    FLASH_STATE_HOLD,
    FLASH_STATE_SETUP_FADE_IN,
    FLASH_STATE_FADE_IN,
};

void ov07_022283D8(SysTask *task, void *param);

void ov07_022283D8(SysTask *task, void *param) {
    FlashContext *ctx = param;

    switch (ctx->state) {
    case FLASH_STATE_SETUP_FADE_OUT:
        PaletteData_BeginPaletteFade(
            ctx->paletteData,
            (1 << PLTTBUF_MAIN_BG),
            ov07_0221E6C8(ctx->battleAnimSys),
            (-2),
            0,
            16,
            (((31) << 10) | ((31) << 5) | (31)));

        Pokepic_StartPaletteFade(
            ctx->attackerSprite,
            0,
            16,
            0,
            (((0) << 10) | ((0) << 5) | (0)));
        ctx->state++;
        break;
    case FLASH_STATE_FADE_OUT:
        if (PaletteData_GetSelectedBuffersBitmask(ctx->paletteData) == 0 && Pokepic_ResumePaletteFade(ctx->attackerSprite) == 0) {
            ctx->state++;
        }
        break;
    case FLASH_STATE_HOLD:
        if (++ctx->delay > 5) {
            ctx->state++;
        }
        break;
    case FLASH_STATE_SETUP_FADE_IN:
        PaletteData_BeginPaletteFade(
            ctx->paletteData,
            (1 << PLTTBUF_MAIN_BG),
            ov07_0221E6C8(ctx->battleAnimSys),
            (-2),
            16,
            0,
            (((31) << 10) | ((31) << 5) | (31)));

        Pokepic_StartPaletteFade(
            ctx->attackerSprite,
            16,
            0,
            0,
            (((0) << 10) | ((0) << 5) | (0)));
        ctx->state++;
        break;
    case FLASH_STATE_FADE_IN:
        if (PaletteData_GetSelectedBuffersBitmask(ctx->paletteData) == 0 && Pokepic_ResumePaletteFade(ctx->attackerSprite) == 0) {
            ctx->state++;
        }
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
