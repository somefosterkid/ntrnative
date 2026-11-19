#ifndef POKEHEARTGOLD_OVERLAY_07_FLASH_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_FLASH_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "sys_task.h"

struct PaletteData;
struct PaletteFadeContext;
struct Pokepic;

// sizeof == 0x14 on the DS
typedef struct FlashContext {
    u8 state;
    u8 delay;
    struct PaletteData *paletteData;
    struct BattleAnimSystem *battleAnimSys;
    struct PaletteFadeContext *unused;
    struct Pokepic *attackerSprite;
} FlashContext;

void ov07_022283D8(SysTask *task, void *param);

#endif // POKEHEARTGOLD_OVERLAY_07_FLASH_CONTEXT_H
