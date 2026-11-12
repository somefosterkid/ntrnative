// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/event_battle.h, the lines in use so
// far.
#ifndef BWPORT_FIELD_EVENT_BATTLE_H
#define BWPORT_FIELD_EVENT_BATTLE_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// A wild battle. Black 2 adds the index of the special Pokémon met, in EventWildBattleCall_CreateCore
GameEvent *EventWildBattleCall_Create(GameSystem *gsys, Field *field, BtlSetup *setup, u8 a3, u32 a4);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// A battle against a trainer of the field, with flags for its kind
GameEvent *EventTrainerBattleCall_Create(GameSystem *gsys, Field *field, u32 a2, u32 a3, u32 trainerId, u32 a5,
                                         u32 flags);
// The capture demonstration's battle
GameEvent *EventCaptureDemo_Create(GameSystem *gsys, Field *field, HeapID heapId);
// A battle against a trainer, who sends out the Pokémon of the in-game trade of the index once it is done
GameEvent *LoadTradedPokemonBattleStats(GameSystem *gsys, Field *field, u32 a2, u32 a3, u32 trainerId, u32 a5,
                                        u32 flags, u32 index);
GameEvent *CreateTrialHouseBattleEvent(GameSystem *gsys, Field *field, BtlSetup *setup);
GameEvent *func_ov010_0216e8cc(GameSystem *gsys, Field *field, BtlSetup *setup);

#endif // BWPORT_FIELD_EVENT_BATTLE_H
