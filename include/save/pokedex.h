// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/pokedex.h, the lines in use so far.
#ifndef BWPORT_SAVE_POKEDEX_H
#define BWPORT_SAVE_POKEDEX_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

#define POKEDEX_SPECIES_BYTES 0x54
#define POKEDEX_FORM_BYTES 9
// The species that the Pokédex has entries in other languages of
#define POKEDEX_LANGUAGE_SPECIES_MAX 493
#define POKEDEX_LANGUAGE_COUNT 7
#define POKEDEX_LANGUAGE_BYTES 0x1b0
#define POKEDEX_MAGIC 0xbeefcafe

// The lists of species seen and shown: by sex, and then the same of shiny Pokémon
enum {
    POKEDEX_KIND_MALE,
    POKEDEX_KIND_FEMALE,
    POKEDEX_KIND_RARE_MALE,
    POKEDEX_KIND_RARE_FEMALE,
    POKEDEX_KIND_COUNT,
};

u32 getSizeofPokedexData(void);
// Clears the save's Pokédex and sets its magic
void func_0200c9f4(PokeDexSave *pokedex);
// The save's Pokédex
PokeDexSave *func_0200ca10(SaveControl *save);
// Gives the player the National Pokédex, and whether the player has it
void func_0200ca28(PokeDexSave *pokedex);
BOOL PokeDex_IsNationalObtained(PokeDexSave *pokedex);
// Gives the player the Pokédex, and whether the player has it (pokebw2's givePlayerPokedex, PokeDex_IsGiven)
void func_0200ca40(PokeDexSave *pokedex);
u32 func_0200ca54(PokeDexSave *pokedex);
// Which Pokédex the list shows and the detail screen counts (0 the regional one, 1 the national one), and sets it
void func_0200ca5c(PokeDexSave *pokedex, u32 mode);
u32 func_0200ca70(PokeDexSave *pokedex);
// The species that the Pokédex's list was last on, and sets it
void func_0200ca78(PokeDexSave *pokedex, u16 species);
u16 func_0200ca8c(PokeDexSave *pokedex);
// Sets the sex, shininess and form that the Pokédex shows of a species
void addToDex(PokeDexSave *pokedex, u16 species, u32 sex, BOOL rare, u32 form);
// The forms of a species, 1 for one without any
u16 getNumberOfForms(u16 species);
// The sex, shininess and form that the Pokédex shows of a species
void func_0200cc40(PokeDexSave *pokedex, u16 species, u32 *sex, u32 *rare, u32 *form, HeapID heapId);
void addPkmToDex(PokeDexSave *pokedex, PartyPkm *pkm);
BOOL PokeDex_IsCaught(PokeDexSave *pokedex, u16 species);
// The species caught, in the National Pokédex once the player has it (else the regional one's)
u32 func_0200cf08(PokeDexSave *pokedex, HeapID heapId);
u16 PokeDex_GetCaughtNoNational(PokeDexSave *pokedex);
void PokeDex_RegistPkm(PokeDexSave *pokedex, PartyPkm *pkm);
BOOL PokeDex_IsSeen(PokeDexSave *pokedex, u16 species);
// The species seen, in the National Pokédex once the player has it (else the regional one's)
u32 func_0200d0ac(PokeDexSave *pokedex, HeapID heapId);
u16 PokeDex_GetSeenNoNational(PokeDexSave *pokedex);
// The regional Pokédex's species caught and seen, counted over the regional dex table
u16 func_0200cf54(PokeDexSave *pokedex, HeapID heapId);
u16 func_0200d0f8(PokeDexSave *pokedex, HeapID heapId);
// Whether the Pokédex has seen the species in that sex, shininess and form
BOOL func_0200d144(PokeDexSave *pokedex, u16 species, u32 sex, BOOL rare, u32 form);
// The personality of the Spinda the Pokédex shows, for index 0
u32 func_0200d288(PokeDexSave *pokedex, BOOL flag);
// Whether all the species are caught but the mythical ones, in the regional Pokédex too, and whether all of the
// regional one are seen
u32 PokeDex_IsCompleteNational(PokeDexSave *pokedex);
u32 PokeDex_IsCompleteUnova(PokeDexSave *pokedex, HeapID heapId);
u32 PokeDex_IsSeenAllUnova(PokeDexSave *pokedex, HeapID heapId);
// Whether the Pokédex has the species' entry in a language
BOOL PokeDex_HasLanguage(PokeDexSave *pokedex, u16 species, u32 language);

// The Pokédex's save block 0x37 (func_0200ca10), pokebw2's struct with Black's layout: no habitat list, a 10-bit
// given and a 1-bit mode, POKEDEX_FORM_BYTES 9. The lists of bits are by species - 1, and the ones of forms by the
// form's place in pokemonFormList
struct PokeDexSave {
    u32 magic;
    u32 nationalObtained : 1;
    // Set to 1 when the player is given the Pokédex (func_0200ca40)
    u32 given : 10;
    // Which Pokédex the list shows and the detail screen counts: 0 the regional one, 1 the national one
    u32 mode : 1;
    // The species that the Pokédex's list was last on
    u32 lastSpecies : 10;
    u32 unused4_22 : 10;
    u8 caught[POKEDEX_SPECIES_BYTES];
    // A list of POKEDEX_SPECIES_BYTES for each kind
    u8 seen[POKEDEX_KIND_COUNT * POKEDEX_SPECIES_BYTES];
    // Which of them the Pokédex shows of a species: one bit set among the four lists
    u8 shown[POKEDEX_KIND_COUNT * POKEDEX_SPECIES_BYTES];
    // The forms seen and the form shown: a list of POKEDEX_FORM_BYTES for the normal ones, then one for the shiny
    u8 seenForms[2 * POKEDEX_FORM_BYTES];
    u8 shownForms[2 * POKEDEX_FORM_BYTES];
    // The languages a species was caught in, POKEDEX_LANGUAGE_COUNT bits a species
    u8 languages[POKEDEX_LANGUAGE_BYTES];
    // The personality of the first Spinda seen
    u32 spindaPersonality;
}; // size: 0x4d4

#endif // BWPORT_SAVE_POKEDEX_H
