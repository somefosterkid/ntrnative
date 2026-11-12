// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/shooter_item.h.
#ifndef BWPORT_SYSTEM_SHOOTER_ITEM_H
#define BWPORT_SYSTEM_SHOOTER_ITEM_H

#include "types.h"

// The items of the Wonder Launcher (the shooter) and their costs in its energy. The names are Black's dsd names;
// pokebw2's guesses are in the comments

#define SHOOTER_ITEM_COUNT 46

// Whether the item at index isn't set in the bit field of disabled items (ShooterItem_IsEnabled)
BOOL func_0202f114(const u8 *disabled, u32 index);
// ShooterItem_GetItem
u32 func_0202f138(u32 index);
// An item's cost, 0 if it isn't a shooter item (ShooterItem_GetCost)
u32 func_0202f144(u16 item);
// An item's index in the list, 0 if it isn't in it (ShooterItem_GetIndex)
u32 func_0202f170(u16 item);

#endif // BWPORT_SYSTEM_SHOOTER_ITEM_H
