#include "global.h"
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

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

PaletteFadeContext *ov07_02222F10(PaletteData *paletteData, enum HeapID heapID, enum PaletteBufferId bufferID, u16 index, u16 count, s8 stepFrames, s8 step, u8 startFrac, u8 endFrac, u16 color, int priority);

void ov07_022323FC(BattleAnimSystem *system, int pokemon, BattleAnimSpriteInfo *sprites, int *count);

u32 ObjPlttTransfer_GetPaletteVramOffset(const NNSG2dImagePaletteProxy *paletteProxy, NNS_G2D_VRAM_TYPE vramType);

typedef struct FadePokemonSpriteContext {
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo spriteInfo;
    PaletteFadeContext *paletteFade;
} FadePokemonSpriteContext;

void ov07_02225810(SysTask *task, void *param);

void ov07_02225838(BattleAnimSystem *system);

void ov07_02225838(BattleAnimSystem *system) {
    FadePokemonSpriteContext *ctx = ov07_022324D8(system, sizeof(FadePokemonSpriteContext));
    ov07_02231FE4(system, &ctx->common);

    int target = ov07_0221C4A8(system, 0);
    int stepFrames = ov07_0221C4A8(system, 1);
    int step = ov07_0221C4A8(system, 2);
    int startFrac = ov07_0221C4A8(system, 3);
    int endFrac = ov07_0221C4A8(system, 4);
    int color = ov07_0221C4A8(system, 5);

    int count;
    ov07_022323FC(system, target, &ctx->spriteInfo, &count);

    int palDest = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(ctx->spriteInfo.hwSprite->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    ctx->paletteFade = ov07_02222F10(
        ctx->common.paletteData,
        ov07_0221BFD0(system),
        PLTTBUF_MAIN_OBJ,
        ((palDest) * (16)),
        (16),
        stepFrames,
        step,
        startFrac,
        endFrac,
        color,
        1100);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02225810, ctx);
}
