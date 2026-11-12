// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/hidden_item.h.
#ifndef BWPORT_FIELD_HIDDEN_ITEM_H
#define BWPORT_FIELD_HIDDEN_ITEM_H

// Overlay 10's hidden_item.c (a descriptive name): the table of the items hidden in the field, by their script ID.
// Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

typedef struct {
    // The item's flag, from HIDDEN_ITEM_FLAG_START
    u16 index;
    u8 unk2;
    // Whether the item comes back after it is found
    u8 respawn;
    u16 unk4;
    u16 unk6;
    u16 unk8;
} HiddenItem;

u16 GetHiddenItemNoBySCRID(u16 scrId);
u16 GetHiddenItemEventFlagNoBySCRID(u16 scrId);
void TryClearRepeatableHiddenItemFlags(EventWork *eventWork);
// The table and its length (pokebw2's func_ov012_0215f2c4)
const HiddenItem *func_ov010_0216476c(u16 *count);

// Black 2's, not checked against Black
// The event flag of the first hidden item, which HiddenItem.index is added to
#define HIDDEN_ITEM_FLAG_START 0x384

#endif // BWPORT_FIELD_HIDDEN_ITEM_H
