// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_pokemon_center.h: the prototypes
// of Black's named functions and the types they need.
#ifndef BWPORT_FIELD_EVENT_POKEMON_CENTER_H
#define BWPORT_FIELD_EVENT_POKEMON_CENTER_H

#include "types.h"
#include "system/game_event.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

GameEvent *EventPokeCenHeal_Create(GameSystem *gsys, GameEvent *parent, u8 ballCount);
GameEventReturnCode EventPokeCenHeal_Callback(GameEvent *event, u32 *state, void *data);
void EventPokeCenHeal_Init(EventPokeCenHealData *work, GameSystem *gsys, u8 ballCount);

// Black 2's; twinned code confirms the offsets of heapId, field, basePosition, ballCount, animationCount, frameCounter,
// propSystem, ballHandles and centerProp, and its size in Black
// Partial layout for the healing machine's animation state.
struct EventPokeCenHealData {
    u16 heapId;
    u16 unk02;
    Field *field;
    VecFx32 basePosition;
    u8 ballCount;
    u8 animationCount;
    u16 frameCounter;
    FieldPropSystem *propSystem;
    FieldPropHandle *ballHandles[6];
    FieldChunkPropHolder *centerProp;
};

void EventPokeCenHeal_End(EventPokeCenHealData *work);
void EventPokeCenHeal_ChangeState(EventPokeCenHealData *work, u32 *state, u32 newState);
void EventPokeCenHeal_StartAnimations(EventPokeCenHealData *work);
void EventPokeCenHeal_PauseAnimation(EventPokeCenHealData *work);
BOOL EventPokeCenHeal_IsAnimationDone(EventPokeCenHealData *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void EventPokeCenHeal_SpawnMonsBall(EventPokeCenHealData *work);

#endif // BWPORT_FIELD_EVENT_POKEMON_CENTER_H
