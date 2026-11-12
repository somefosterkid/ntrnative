// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/trial_house.h: the lines in use so far.
#ifndef BWPORT_SAVE_TRIAL_HOUSE_H
#define BWPORT_SAVE_TRIAL_HOUSE_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Save block 0x3f
// A Pokémon of a recorded team
struct TrialHousePokemon {
    u16 species;
    u8 form;
    u8 sex;
};

// Black 2's; twinned code confirms the offsets of valid, isDouble and pokemon in Black
// The result of a challenge
struct TrialHouseRecord {
    u8 valid;
    u8 isDouble;
    u16 points;
    TrialHousePokemon pokemon[4];
};

// Black 2's; twinned code confirms the offsets of records and bits in Black
struct TrialHouseSave {
    // The challenges against the Trainers of the game and of a downloaded challenge
    TrialHouseRecord records[2];
    u8 bits[16];
    // Copied from the downloaded challenge's save
    u8 unk38[0x22];
    u8 padding5A[2];
    // The data of the file at func_0200fe80, which that file initializes
    u8 unk5C[0x4c];
}; // size: 0xa8

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
TrialHouseSave *func_0200e8f8(SaveControl *save);
u32 func_0200e588(void);
BOOL func_0200e5a0(void *saveBuffer);
BOOL func_0200e5cc(void *extraSave);
// The Trainer of a recorded battle, 0 to 4
BSubwayTrainer *func_0200e5f8(void *save, u32 index);
u32 func_0200e5e4(void *saveBuffer);
void func_0200e608(GameData *gameData, void *saveBuffer, HeapID heapId);
void func_0200e684(void *extraSave);

#endif // BWPORT_SAVE_TRIAL_HOUSE_H
