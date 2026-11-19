#include "global.h"

#include "constants/heap.h"

#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"

#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "pokemon_types_def.h"
#include "overlay_07_battle_anim_system.h"
#include "pokepic.h"

struct BattleAnimSystem;

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

typedef struct SpriteExampleContext {
    u8 state;
    BattleAnimSystem *battleAnimSystem;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMgr;
    ManagedSprite *mainSprite;
    ManagedSprite *sprites[100];
    Sprite *unused[10];
} SpriteExampleContext;

void ov07_0222314C(SysTask *task, void *param);

void ov07_022231BC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

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
void ov07_02223224(Pokepic *sprite);

void ov07_022231BC(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;

    SpriteExampleContext *ctx = Heap_Alloc(HEAP_ID_BATTLE, sizeof(SpriteExampleContext));
    ((ctx != 0L) ? (void)0 : GF_AssertFail());

    ctx->state = 0;
    ctx->spriteSys = spriteSys;
    ctx->spriteMgr = spriteMan;
    ctx->mainSprite = sprite;
    ctx->battleAnimSystem = system;

    template = ov07_0221F9E8(system);
    ctx->sprites[0] = sprite;

    for (int i = 1; i < 3; i++) {
        ctx->sprites[i] = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMgr, &template);
    }

    ov07_0221C3F4(system, ov07_0222314C, ctx, 0x1000);
}

void ov07_02223224(Pokepic *sprite) {
    Pokepic_SetAttr(sprite, MON_SPRITE_SCALE_X, (1 << 8));
    Pokepic_SetAttr(sprite, MON_SPRITE_SCALE_Y, (1 << 8));
}
