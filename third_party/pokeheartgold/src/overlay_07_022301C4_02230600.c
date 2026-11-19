#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimContext;

struct BattleAnimScriptLoop;

struct BattleAnimSystem;

struct BattleBackgroundReference;

struct BattleBgAnim;

struct BgTrackingTask;

struct CustomBgScrollContext;

struct ExtrasensoryContext;

struct FakeOutCurtainContext;

struct ManagedSprite;

struct Pokepic;

struct PokemonSpriteDrawContext;

struct PokepicShadow;

struct PokepicDrawParam;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

struct XYTransformContext;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;

typedef struct BattleAnimContext BattleAnimContext;

typedef struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
} BattleAnimScriptLoop;

typedef struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
} PokemonSpriteDrawContext;

typedef struct BattleBgAnim BattleBgAnim;

typedef struct SpriteTrackingTask SpriteTrackingTask;

typedef struct BgTrackingTask BgTrackingTask;

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

typedef struct FakeOutCurtainContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    int delay;
    XYTransformContext curtainPos;
} FakeOutCurtainContext;

void ov07_0223049C(SysTask *task, void *param);

void ov07_02230600(BattleAnimSystem *system);

void ov07_02230600(BattleAnimSystem *system) {
    // BUG: Should be sizeof(FakeOutCurtainContext), but since sizeof(ExtrasensoryContext) is larger, it works anyway.
    FakeOutCurtainContext *ctx = ov07_022324D8(system, sizeof(ExtrasensoryContext));
    ctx->battleAnimSys = system;

    ov07_0221C410(ctx->battleAnimSys, ov07_0223049C, ctx);
}
