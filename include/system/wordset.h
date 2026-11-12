// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/wordset.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_WORDSET_H
#define BWPORT_SYSTEM_WORDSET_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Word sets (wordset.c): the words that fill the placeholders in messages

WordSet *GFL_WordSetSystemCreateDefault(HeapID heapId);
void GFL_WordSetSystemFree(WordSet *wordSet);
void GFL_WordSetFormatStrbuf(WordSet *wordSet, StrBuf *dest, const StrBuf *src);
void WordSet_LoadSpeciesName(WordSet *wordSet, u32 index, u32 species);
void loadCountryToStrbuf(WordSet *wordSet, u32 index, u32 country);
void loadCountryAreaToStrbuf(WordSet *wordSet, u32 index, u32 country, u32 area);
void loadHobbyNameToStrbuf(WordSet *wordSet, u32 index, u8 hobby);
// Puts a Pokémon's species name in a word set
void setPartyPokemonSpeciesNameToStrbuf(WordSet *wordSet, u32 index, PartyPkm *pkm);
void loadPokemonNicknameToStrbuf(WordSet *wordSet, u32 index, PartyPkm *pkm);
void loadPokemonTextNameToStrbuf(WordSet *wordSet, u32 index, u32 species);
void loadMoveNameToStrbuf(WordSet *wordSet, u32 index, u32 move);
void loadItemNameToStrbuf(WordSet *wordSet, u32 index, u32 item);
void loadPassPowerToStrbuf(WordSet *wordSet, u32 index, s32 passPower);
void loadBagPocketNameToStrbuf(WordSet *wordSet, u32 index, u32 pocket);
// Puts the player's name in a word set
void copyVarForText(WordSet *wordSet, u32 index, PlayerInfo *playerInfo);
// Puts a place name, from the place names' message file, in a word set
void loadLocationNameToStrbuf(WordSet *wordSet, u32 index, u32 placeNameId);
// Formats a number as GFL_WordSetFormatNumber does
void WordSetNumber(WordSet *wordSet, u32 index, s32 number, u32 digits, u32 pad, BOOL ascii);
// pokebw2's func_0202437c
void func_0201ee0c(WordSet *wordSet, u32 index, const StrBuf *strbuf, u32 a3, u32 a4, u32 a5);

void loadItemTextNameToStrbuf(WordSet *wordSet, u32 index, u32 item);
void loadItemsNameToStrbuf(WordSet *wordSet, u32 index, u32 item);
void loadSayingForDisplay(WordSet *wordSet, u32 index, u16 saying);
void loadTypeTextToStrbuf(WordSet *wordSet, u32 index, u32 type);
void loadTrainerTypeText(WordSet *wordSet, u32 index, u8 trainerType);
void loadTrainerTypeWithArticleToStrbuf(WordSet *wordSet, u32 index, u8 trainerType);
void loadQuestionnaireAnswerToStrbuf(WordSet *wordSet, u32 index, u8 answer);

void GFL_WordSetClearAll(WordSet *wordSet);

void loadTrainerTypeToStrbuf(WordSet *wordSet, u32 index, u32 trainerId);

// An item's name: the plural when plural is set, else the one in message file 481 when a4 is set
void loadItemText(WordSet *wordSet, u32 index, u32 item, BOOL plural, BOOL a4);

void loadBoxNameForDisplay(WordSet *wordSet, u32 index, void *boxData, u32 box);

void func_0201effc(WordSet *wordSet, u32 index, u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// A word set of count words of up to length characters
WordSet *GFL_WordSetSystemCreate(u32 count, u32 length, HeapID heapId);
void loadPokemonSpeciesTextNameToStrbuf(WordSet *wordSet, u32 index, PartyPkm *pkm);
void loadAbilityNameToStrbuf(WordSet *wordSet, u32 index, u32 ability);
void loadStatNameToStrbuf(WordSet *wordSet, u32 index, u8 stat);
void loadJobAnswerToStrbuf(WordSet *wordSet, u32 index, u8 job);
void loadBoxPokemonNameToStrbuf(WordSet *wordSet, u32 index, BoxPkm *pkm);
void loadMonthToStrbuf(WordSet *wordSet, u32 index, u32 month);
void setBoxPokemonSpeciesNameToStrbuf(WordSet *wordSet, u32 index, BoxPkm *pkm);
void loadNatureToStrbuf(WordSet *wordSet, u32 index, u32 nature);
void loadTrainerNamesToStrbuf(WordSet *wordSet, u32 index, u32 trainerId);

#endif // BWPORT_SYSTEM_WORDSET_H
