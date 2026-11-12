// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/save_outside.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_SAVE_OUTSIDE_H
#define BWPORT_SAVE_SAVE_OUTSIDE_H

#include "types.h"
#include "gfl/heap.h"
#include "save/mystery_gift.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of receivedFlags, cards and checksum in Black
// The mystery gifts kept outside the save, laid out like the start of MysteryGiftSaveData and encrypted the same
// way
typedef struct {
    u8 receivedFlags[MG_ID_MAX / 8];
    MysteryGift cards[MG_CARD_MAX_OUTSIDE];
    u16 checksum;
    u16 key;
} SaveOutsideGifts;

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's SaveOutside_StartSave
// Starts writing the work's blocks back, and steps it until it returns TRUE
void func_ov228_021dfdc0(SaveOutside *work);
// pokebw2's SaveOutside_Save
BOOL func_ov228_021dfe00(SaveOutside *work);
// pokebw2's SaveOutside_Load
// Loads both blocks, and returns the work that keeps them. Copies that can't be read leave empty data
SaveOutside *func_ov228_021dfc20(HeapID heapId);
// pokebw2's SaveOutside_IsGiftsLoaded
// Whether the gifts were loaded, and whether both of their copies were broken
BOOL func_ov228_021dfe90(SaveOutside *work);
// pokebw2's SaveOutside_Free
void func_ov228_021dfdb8(SaveOutside *work);
// pokebw2's SaveOutside_GetGifts
SaveOutsideGifts *func_ov228_021dfeac(SaveOutside *work);

// pokebw2's SaveOutside_IsGiftsBroken
BOOL func_ov228_021dfe98(SaveOutside *work);
// pokebw2's SaveOutside_CopyGiftsToSave
// Copies the gifts into the save's mystery gift block
void func_ov228_021dfeb0(SaveOutside *work, SaveControl *save);

#endif // BWPORT_SAVE_SAVE_OUTSIDE_H
