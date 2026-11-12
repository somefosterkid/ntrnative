// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/pml/met_data.h, the lines in use so far.
#ifndef BWPORT_PML_MET_DATA_H
#define BWPORT_PML_MET_DATA_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// Where, when and by whom a Pokémon was met. Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// Marks the Pokémon as met in a fateful encounter, at the location and on the date
void setFatefulEncounterPkmData(BoxPkm *pkm, u16 location, u32 year, u32 month, u32 day);

// Records how and where a party Pokémon was met, by kind: 0 caught or received, as met at placeName with the player
// as its Trainer; 1 an in-game trade; 2 hatched, an egg of the player's keeping where it was received as its egg's
// place; 5 an egg received at placeName; 6 a link trade, at placeName; 7 one of N's Pokémon. 3 and 4 change nothing
void PokeParty_SetupMetData(PartyPkm *pkm, u32 kind, PlayerInfo *playerInfo, u32 placeName, HeapID heapId);

// Sets the date the Pokémon was met to today
void setMetCurrentDateTime(BoxPkm *pkm);

// Black 2's, not checked against Black
// The message files of the place names, as MetLocation_GetNameFile returns them
#define PLACE_FILE_UNOVA 0x59

// Black 2's, not checked against Black
#define PLACE_FILE_EVENT 0x5a
#define PLACE_FILE_EXTERNAL 0x5b
#define PLACE_FILE_SPECIAL 0x5c

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void PML_UtilSetupMetData(BoxPkm *pkm, u32 kind, PlayerInfo *playerInfo, u32 placeName, HeapID heapId);
BOOL special_transfers(BoxPkm *pkm, u32 kind);
// pokebw2's PokeParty_IsSpecialTransfer
// Whether the Pokémon came by one of the kinds of special event for the player: 0 to 3 the Celebi and the beasts
// whose event hasn't run or has, 4 Keldeo, 5 Meloetta, 6 Genesect, 7 Shaymin
BOOL func_0202f488(PartyPkm *pkm, u32 kind);
// pokebw2's PokeParty_SetSpecialTransferUsed
// Marks the event of an event Celebi or beast as run
void func_0202f630(PartyPkm *pkm, u32 kind);
// pokebw2's MetLocation_GetNameFile
// The message file of a location's name, PLACE_FILE_*, and the name's index in it
u32 func_0202f688(u32 location);
// pokebw2's MetLocation_GetNameIndex
u32 func_0202f6d8(u32 location);

#endif // BWPORT_PML_MET_DATA_H
