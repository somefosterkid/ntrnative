// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_ov169.h: the lines in use so far.
#ifndef BWPORT_BATTLE_BTL_OV169_H
#define BWPORT_BATTLE_BTL_OV169_H

#include "types.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Whether an item is an Arceus plate, and a Genesect drive
BOOL data_ov095_0689c981(u16 item);
BOOL data_ov095_0689c991(u16 item);
BattleEventItem *FieldEffectEventAdd(u32 effect, u8 monId);
void data_ov095_06898081(BattleEventItem *item);

// The target screen's key cursor: the stops each column of the target screen has, by position of the Pokemon choosing
// (double battle, then triple), and the stops themselves
extern const u8 data_ov095_0689e090[2][15];
extern const u8 data_ov095_0689e0b0[3][15];
extern const BtlvInputKeyStop *const data_ov095_0689e560[2][15];
extern const BtlvInputKeyStop *const data_ov095_0689e704[3][15];

#endif // BWPORT_BATTLE_BTL_OV169_H
