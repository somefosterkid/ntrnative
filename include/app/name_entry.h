// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/name_entry.h, the lines in use so far,
// with Black's layout and names.
#ifndef BWPORT_APP_NAME_ENTRY_H
#define BWPORT_APP_NAME_ENTRY_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "struct_decls.h"

// The name entry

// What the name entry is for
#define NAME_ENTRY_POKEMON 1

// Black's is 0x2c bytes, without the three fields that follow gameInfo in Black 2's
typedef struct {
    u32 mode;
    // The gender, or the Pokémon's species
    u16 gender;
    // The Pokémon's form, and its sex in the high byte
    u16 unk6;
    PartyPkm *pkm;
    // A StrBuf that the Pokémon name entry formats and prints once the name is given, if not NULL, and the box data and
    // the number of the box, which it names
    StrBuf *unkC;
    BoxSaveAccessor *unk10;
    u32 unk14;
    u32 maxLength;
    // 1 when the name is to be left as it was
    u32 unk1C;
    // The name, which starts as a default
    StrBuf *name;
    // The save data that the new game creates in the background
    SaveControlIntr *saveTask;
    TrainerGameInfoSave *gameInfo;
} NameEntryParam;

// Overlay 10's namein_setup.c
// The name starts as a copy of name, if given
NameEntryParam *setupNameEntry(HeapID heapId, u32 mode, u32 a2, u32 a3, u32 maxLength, const StrBuf *name,
                               TrainerGameInfoSave *gameInfo);
// The name entry for a Pokémon's nickname
NameEntryParam *setupPokemonNameEntry(HeapID heapId, PartyPkm *pkm, u32 maxLength, const StrBuf *name,
                                      TrainerGameInfoSave *gameInfo);
extern const GameProcFunctions data_ov194_021f5580;
NameEntryParam *pokemonNameEntry(HeapID heapId, PartyPkm *pkm, u32 maxLength, const StrBuf *name, StrBuf *unkC,
                                 BoxSaveAccessor *unk10, u32 unk14, TrainerGameInfoSave *gameInfo);
void func_ov010_0216c0c0(NameEntryParam *param);
// Copies the entered name
void func_ov010_0216c0d4(NameEntryParam *param, StrBuf *dest);
BOOL func_ov010_0216c0e4(NameEntryParam *param);

void func_0200c0fc(TrainerGameInfoSave *info, u32 index, u8 value);
// The keyboard that each mode last used, a byte of the trainer's info save (not declared in save_control.h)
u8 func_0200c104(TrainerGameInfoSave *info, u32 index);
// The name entry's proc functions (its overlay is 194; the ROM's table is named for ov074's address)
extern const GameProcFunctions data_ov194_021f5580;
// The sound sequences the name entry preloads: their count and the list
extern const u32 data_ov194_021f557c;
extern const u32 data_ov194_021f558c[];

// Black 2's, not checked against Black
#define NAME_ENTRY_RIVAL 3

#define NAME_ENTRY_FRIEND 4

#endif // BWPORT_APP_NAME_ENTRY_H
