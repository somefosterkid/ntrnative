#ifndef POKEHEARTGOLD_OVERLAY_07_GRUDGE_SPRITE_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_GRUDGE_SPRITE_CONTEXT_H

#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;
struct ManagedSprite;
struct SpriteManager;
struct SpriteSystem;

// Six sprites revolving round the battler while the alpha fades in and out (Grudge flames, and the same
// effect in ov07_02231300). Heap object: ov07_022324D8(system, sizeof(GrudgeSpriteContext)).
// sizeof == 0x134 on the DS
typedef struct GrudgeSpriteContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteSystem *spriteSys;
    struct SpriteManager *spriteMan;
    int state;
    int delay;
    int dir;
    struct ManagedSprite *flameSprites[6];
    XYTransformContext revs[6];
    AlphaFadeContext alpha;
    s16 attackerX;
    s16 attackerY;
} GrudgeSpriteContext;

#endif // POKEHEARTGOLD_OVERLAY_07_GRUDGE_SPRITE_CONTEXT_H
