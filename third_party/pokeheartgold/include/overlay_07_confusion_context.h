#ifndef POKEHEARTGOLD_OVERLAY_07_CONFUSION_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_CONFUSION_CONTEXT_H

#include "global.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_common.h"

struct BattleAnimSystem;
struct ManagedSprite;
struct Pokepic;
struct SpriteManager;

// sizeof(ConfusionContext) == 0x68 on the DS (heap alloc in ov07_0222C780)
typedef struct ConfusionContext {
    struct BattleAnimSystem *battleAnimSys;
    struct SpriteManager *pokemonSpriteManager;
    int state;
    int unused;
    XYTransformContext shake;
    XYTransformContext scale;
    struct ManagedSprite *monSprite;
    struct Pokepic *defenderSprite;
    s16 defenderX;
    s16 defenderY;
    s16 jitterDir;
    s16 defenderShadowHeight;
} ConfusionContext;

void ov07_0222C610(ConfusionContext *ctx);
void ov07_0222C6A8(SysTask *task, void *param);
void ov07_0222C780(struct BattleAnimSystem *system);

#endif // POKEHEARTGOLD_OVERLAY_07_CONFUSION_CONTEXT_H
