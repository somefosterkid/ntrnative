// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/condition_effects.h: the lines in use
// so far.
#ifndef BWPORT_BATTLE_CONDITION_EFFECTS_H
#define BWPORT_BATTLE_CONDITION_EFFECTS_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "battle/btl_pokeparam.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Condition_IsPassedByBatonPass
// Whether Baton Pass passes a condition on
BOOL data_ov095_0689c869(u32 condition);
// pokebw2's Condition_CheckTrapped
// Whether the condition of a Pokemon keeps it from being switched out, in the event
void data_ov095_0689c68d(BtlServerFlow *flow, BattleMon *mon);
// pokebw2's Condition_SetAddMessage
// Sets the message of a condition that was inflicted, or clears the string when it has none
void data_ov095_0689c365(u32 condition, BattleCondition value, BattleMon *mon, BattleHandlerString *string);
// pokebw2's Condition_CheckIngrainForceSwitch
// Ingrain stops a Pokemon from being forced out, with its message
void data_ov095_0689c7a5(BtlServerFlow *flow, BattleMon *mon);
// pokebw2's Condition_EndTurnEffect
// What a condition does at the end of a turn, or when it is cured: the damage or recovery, the countdown and the
// messages. A cured condition gets its own message
void data_ov095_0689b7a9(BattleMon *mon, u32 condition, BattleCondition prev, BOOL cured, BtlServerFlow *flow);
// pokebw2's Condition_SetDamageMessage
// Sets the message of the damage of a condition (poison, burn, Curse and Nightmare). Always returns 0
BOOL data_ov095_0689b8cd(BattleHandlerString *string, BattleMon *mon, u32 condition);
// Whether the conditions of a Pokemon ground it, or give it a type chart check, in the event
void Condition_CheckFloating(BtlServerFlow *flow, BattleMon *mon);
// pokebw2's Condition_CheckTypeImmunity
void data_ov095_0689c541(BtlServerFlow *flow, BattleMon *target);

#endif // BWPORT_BATTLE_CONDITION_EFFECTS_H
