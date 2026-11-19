#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "constants/heap.h"
#include "library/spl_emitter.h"
#include "bag_types_def.h"
#include "filesystem.h"
#include "heap.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "seal_case.h"
#include "sys_task.h"
#include "particle_system.h"
#include "pokemon_types_def.h"
#include "msgdata.h"
#include "pokepic.h"
#include "pokemon_summary_app.h"
#include "pokedex.h"
#include "game_stats.h"
#include "evolution_data.h"

void sub_020771A0(BgConfig *arg0);

struct OverlayManager;
struct G3DPipelineBuffers;
struct GameStats;
struct MsgData;
struct PaletteAnimator;
struct ParticleSystem;
struct Party;
struct Pokedex;
struct PokemonAnimManager;
struct Pokepic;
struct PokepicManager;
struct PokepicDrawParam;
struct PokemonSummary;
struct Poketch;
struct MessageFormat;
typedef struct {
    enum HeapID heapID;
    int unk_04;
} UnkStruct_0207C894;
typedef struct {
    UnkStruct_0207C894 unk_00;
    SPLEmitter *unk_08;
    ParticleSystem *unk_0C;
} UnkStruct_0207C8C4;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
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
typedef struct PokemonAnimManager PokemonAnimManager;
Pokemon *AllocMonZeroed(enum HeapID heapID);
void CalcMonLevelAndStats(Pokemon *mon);
void SetMonData(Pokemon *mon, int param, const void *value);
void GetPokemonSpriteCharAndPlttNarcIds(PokepicTemplate *spriteTemplate, Pokemon *mon, u8 face);
void CopyPokemonToPokemon(Pokemon *src, Pokemon *dest);
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct Poketch Poketch;
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};
void sub_020771E8(EvolutionData *evolutionData);

void sub_020771A0(BgConfig *bgConfig) {
    GfGfx_EngineATogglePlanes(1U, 0U);
    GfGfx_EngineATogglePlanes(2U, 0U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 2U);
    FreeBgTilemapBuffer(bgConfig, 3U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 6U);
}

void sub_020771E8(EvolutionData *evolutionData) {
    PokepicTemplate monSpriteTemplate;
    Pokemon *mon;

    GetPokemonSpriteCharAndPlttNarcIds(&monSpriteTemplate, evolutionData->mon, 2);

    evolutionData->monSprites[0] = PokepicManager_CreatePokepic(evolutionData->monSpriteMan, &monSpriteTemplate, 128, 80, 0, 0, 0L, 0L);
    mon = AllocMonZeroed(evolutionData->heapID);

    CopyPokemonToPokemon(evolutionData->mon, mon);
    SetMonData(mon, MON_DATA_SPECIES, (u8 *)&evolutionData->targetSpecies);
    CalcMonLevelAndStats(mon);
    GetPokemonSpriteCharAndPlttNarcIds(&monSpriteTemplate, mon, 2);
    Heap_Free(mon);

    evolutionData->monSprites[1] = PokepicManager_CreatePokepic(evolutionData->monSpriteMan, &monSpriteTemplate, 128, 80, 0, 0, 0L, 0L);

    Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_X, 0);
    Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_Y, 0);
}
