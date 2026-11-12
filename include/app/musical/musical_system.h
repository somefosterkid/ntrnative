// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/musical_system.h, the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_MUSICAL_SYSTEM_H
#define BWPORT_APP_MUSICAL_MUSICAL_SYSTEM_H

// Overlay 112's musical_system.c: the musical's Pokémon and the program's data

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// The musical's data (pokebw2's Ov210Work), cut short after the fields in use
struct Ov112Work {
    // The program: 0 to 3 for the built-in ones, 4 or more for the downloaded one
    u8 program;
    void *unk4;
};

// A prop a Pokémon wears on the stage
typedef struct {
    u16 itemId;
    s16 unk2;
    // The slot it is worn on
    u8 slot;
} MusicalPokeEquip;

// A Pokémon on the stage, 0x7c bytes
struct MusicalPoke {
    // Who frees it: 0 the event, 1 the communication, 2 the stage; 4 when it is made
    u32 owner;
    // The party Pokémon it was made from, or NULL
    PartyPkm *pkm;
    u16 species;
    u8 sex;
    u8 form;
    u8 rare;
    u32 personality;
    MusicalPokeEquip equips[9];
    u16 points;
    u16 unk4C[4];
    u32 unk54[9];
    u16 unk78;
};

// Black 2's; twinned code confirms the offsets of unk4, msgArc, script, soundData, size and soundDataSize in Black
// The musical's data
struct Ov210Work {
    // The program: 0 to 3 for the built-in ones, 4 or more for the downloaded one
    u8 program;
    void *unk4;
    // The archive of the program's messages
    void *msgArc;
    // The program's script for overlay 209's stage, a table of the offsets of its scripts
    u32 *script;
    // One allocation holding the sizes of the three sound files, then the files
    u32 *soundData;
    // The program's sound data, which the stage passes to the sound functions
    void *sound[3];
    // The sizes of unk4, msgArc and script
    u32 size[3];
    // The size of soundData
    u32 soundDataSize;
};

// pokebw2's MusicalSystem_FreeProgramData
void func_ov112_021f6888(Ov210Work *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's MusicalSystem_LoadProgramData
// Loads a program's data
void func_ov112_021f68bc(Ov210Work *work, SaveControl *save, GameData *gameData, u8 program, HeapID heapId);
Ov210Work *func_ov112_021f685c(HeapID heapId);
MusicalPoke *func_ov112_021f675c(PartyPkm *pkm, HeapID heapId);
MusicalPoke *func_ov112_021f67e0(u16 species, u8 sex, u8 form, u8 rare, u32 personality, HeapID heapId);
// pokebw2's MusicalSystem_InitProgramData
// pokebw2's MusicalSystem_CanJoin
// Whether a party Pokémon can join the musical: not an egg, and not in a form it would lose in a box
BOOL func_ov112_021f6744(PartyPkm *pkm);

#endif // BWPORT_APP_MUSICAL_MUSICAL_SYSTEM_H
