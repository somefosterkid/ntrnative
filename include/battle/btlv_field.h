// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_field.h, the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_FIELD_H
#define BWPORT_BATTLE_BTLV_FIELD_H

// Overlay 94's btlv_field.c (named by its string), the battle view's field: the ground and weather model around the
// stage. BtlvField_Create is swan's name (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); the others are dsd's

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

BtlvField *BtlvField_Create(BOOL arg0, u8 fieldId, u8 variant, HeapID heapId);
void func_ov094_021f6cf0(BtlvField *field);
void func_ov094_021f6d5c(BtlvField *field);
void func_ov094_021f6dd4(BtlvField *field);
void func_ov094_021f6dec(BtlvField *field, u8 evy, u8 targetEvy, u8 waitFrames, u16 color);
BOOL func_ov094_021f6e1c(BtlvField *field);
void func_ov094_021f6e2c(BtlvField *field, BOOL hide);

#endif // BWPORT_BATTLE_BTLV_FIELD_H
