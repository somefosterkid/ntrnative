// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_monset.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_MONSET_H
#define BWPORT_BATTLE_BTL_MONSET_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define BTL_MONSET_MAX 6
// What a mon of the set has taken, for BtlMonSet.damageKind
#define BTL_MONSET_NO_DAMAGE 0

// Black 2's, not checked against Black
#define BTL_MONSET_HP_DAMAGE 1
#define BTL_MONSET_SUBSTITUTE_DAMAGE 2

// Black 2's, not checked against Black
struct BtlMonSet {
    BattleMon *mons[BTL_MONSET_MAX];
    u16 damage[BTL_MONSET_MAX];
    u16 substituteDamage[BTL_MONSET_MAX];
    u8 damageKind[BTL_MONSET_MAX];
    u16 speed[BTL_MONSET_MAX];
    u8 count;
    u8 countMax;
    u8 getIndex;
    u8 numTargets;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlMonSet_GetCountMax
u32 data_ov095_0689cd41(BtlMonSet *set);
// pokebw2's BtlMonSet_GetCount
u32 data_ov095_0689cd39(BtlMonSet *set);
// pokebw2's BtlMonSet_Get
BattleMon *data_ov095_0689cc71(BtlMonSet *set, u32 index);
// pokebw2's BtlMonSet_Clear
// Empties the set
void data_ov095_0689cb3d(BtlMonSet *set);
// pokebw2's BtlMonSet_Add
// Adds a mon unless it is in the set already or the set is full
void data_ov095_0689cb49(BtlMonSet *set, BattleMon *mon);
// Sorts the set by speed, fastest first, with a coin flip for ties
void SortBySpeed(BtlMonSet *set, BtlServerFlow *flow);
// pokebw2's BtlMonSet_ResetIndex
// Restarts BtlMonSet_GetNext
void data_ov095_0689cc85(BtlMonSet *set);
// pokebw2's BtlMonSet_GetNext
// The next mon of the set, or NULL after the last
BattleMon *data_ov095_0689cc8d(BtlMonSet *set);
// pokebw2's BtlMonSet_RemoveFainted
// Removes the fainted mons
void data_ov095_0689cee5(BtlMonSet *set);
// pokebw2's BtlMonSet_CopyNotFainted
// Copies the mons that haven't fainted into dest and returns how many there are
u32 data_ov095_0689cd79(BtlMonSet *set, BtlMonSet *dest);
// pokebw2's BtlMonSet_SetNumTargets
void data_ov095_0689cd49(BtlMonSet *set, u8 numTargets);
// pokebw2's BtlMonSet_IsEmptyAfterTargeting
// Whether the move had targets (it was aimed at some mon) and every one of them is gone
BOOL data_ov095_0689cd51(BtlMonSet *set);
// pokebw2's BtlMonSet_Remove
// Removes a mon from the set
void data_ov095_0689cc15(BtlMonSet *set, BattleMon *mon);
// pokebw2's BtlMonSet_CopyAllies
// Copies the mons that are allies of a mon (it too) into out, and returns how many there are
u32 data_ov095_0689cdcd(BtlMonSet *set, BattleMon *mon, BtlMonSet *out);
// pokebw2's BtlMonSet_CopyOpponents
// Copies the mons that are not allies of a mon into out, and returns how many there are
u32 data_ov095_0689ce59(BtlMonSet *set, BattleMon *mon, BtlMonSet *out);
// pokebw2's BtlMonSet_AddDamage
// Adds a mon and records damage it took, to its substitute if that flag is set
void data_ov095_0689cbb9(BtlMonSet *set, BattleMon *mon, u16 damage, BOOL substitute);
// pokebw2's BtlMonSet_GetHpDamage
// The HP damage a mon took; FALSE when it took none
BOOL data_ov095_0689ccf9(const BtlMonSet *set, BattleMon *mon, u32 *damage);
// pokebw2's BtlMonSet_GetTotalDamage
// The damage a mon took, HP and substitute together; FALSE when it took none
BOOL data_ov095_0689ccb5(const BtlMonSet *set, BattleMon *mon, u32 *damage);

#endif // BWPORT_BATTLE_BTL_MONSET_H
