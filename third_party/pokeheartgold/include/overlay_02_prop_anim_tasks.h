#ifndef POKEHEARTGOLD_OVERLAY_02_PROP_ANIM_TASKS_H
#define POKEHEARTGOLD_OVERLAY_02_PROP_ANIM_TASKS_H

#include "global.h"

// Environment of the Pokemon Center healing machine tasks (PokecenterAnimCreate / ov02_0224CDB0 and their run functions). DS 0x18.
typedef struct PokecenterHealingAnimation {
    VecFx32 position;                // 0x00
    u8 pokeballCount;                // 0x0C
    u8 currentPokeballIndex;         // 0x0D
    u8 pokeballTicks;                // 0x0E
    u8 state;                        // 0x0F
    u8 pokeballLoadedPropIDs[6];     // 0x10
} PokecenterHealingAnimation;

// Environment of ov02_0224BE24 (created by ov02_0224BDE8): plays the one-shot animation of map prop 0xD0. DS 0x4.
typedef struct MapPropAnimTask {
    u8 length;     // 0x00
    u8 direction;  // 0x01
    u8 state;      // 0x02
    u8 padding_3;  // 0x03
} MapPropAnimTask;

#endif // POKEHEARTGOLD_OVERLAY_02_PROP_ANIM_TASKS_H
