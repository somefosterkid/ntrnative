// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_tables.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_TABLES_H
#define BWPORT_BATTLE_BTL_TABLES_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"

// func_02013de0 is a zone test (include/field/zone.h), not an item table

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL data_ov095_0689c945(u16 ability);
BOOL data_ov095_0689c9b1(u16 ability);
// pokebw2's IsPledgeMove
BOOL data_ov095_0689c8ed(u16 move); // The Pledge moves
// pokebw2's IsPressureFieldMove
BOOL data_ov095_0689c8dd(u16 move); // Pressure also takes extra PP for it
// pokebw2's IsDelayedAttackMove
BOOL data_ov095_0689c8cd(u16 move); // Future Sight, Doom Desire (and no move)
// pokebw2's IsMimicExcludedMove
BOOL data_ov095_0689ca7d(u16 move);         // Mimic can't copy it
// pokebw2's IsEncoreExcludedMove
// Moves
BOOL data_ov095_0689c8ad(u16 move); // Encore can't repeat it
// pokebw2's IsRolePlayExcludedAbility
// Abilities
BOOL data_ov095_0689c91d(u16 ability);
// pokebw2's GetMetronomeExcludedMoves
const u16 *data_ov095_0689ca8d(u32 *count); // The moves Metronome can't call
// pokebw2's IsAssistExcludedMove
BOOL data_ov095_0689ca3d(u16 move);         // Assist can't call it
// pokebw2's IsSleepTalkExcludedMove
BOOL data_ov095_0689ca1d(u16 move);         // Sleep Talk can't call it
// pokebw2's IsMeFirstExcludedMove
BOOL data_ov095_0689c8bd(u16 move); // Me First can't use it
// pokebw2's IsCopycatExcludedMove
BOOL data_ov095_0689ca5d(u16 move);         // Copycat can't call it
BOOL data_ov095_0689c93d(u16 ability);
BOOL data_ov095_0689c9d5(u16 effect);
u32 data_ov095_0689c9f9(u32 index);
// Black's Thumb stubs (bx pc) of the two above
BOOL func_ov093_021d3558(u16 effect);
u32 func_ov093_021d3a10(u32 index);
// pokebw2's IsSkillSwapExcludedAbility
BOOL data_ov095_0689c955(u16 ability);
// pokebw2's IsEscapeItem
BOOL data_ov095_0689c90d(u16 item); // Fluffy Tail, Poke Doll and Poke Toy
BOOL data_ov095_0689c9a1(u16 background);
// The nth condition of the lists that the battle walks through, or 0 past their ends
u32 data_ov095_0689c9e5(u32 index);

// pokebw2's IsReactiveHeldItem
BOOL data_ov095_0689c8fd(u16 item);
// pokebw2's IsArceusPlate
BOOL data_ov095_0689c981(u16 item); // Arceus plates
// pokebw2's IsGenesectDrive
BOOL data_ov095_0689c991(u16 item); // Genesect drives
BOOL IsAbilityMultitype(u16 ability);
BOOL IsArceus(u16 species);

#endif // BWPORT_BATTLE_BTL_TABLES_H
