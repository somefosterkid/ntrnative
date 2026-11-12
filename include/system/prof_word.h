// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/prof_word.h: the lines in use so far.
#ifndef BWPORT_SYSTEM_PROF_WORD_H
#define BWPORT_SYSTEM_PROF_WORD_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

#define PROF_WORD_LIST_COUNT 7

// The length of a name and of a word in the lists, terminator included
#define PROF_WORD_LEN 32

// Black 2's; twinned code confirms the offsets of lists in Black
// The lists, decrypted from archive 216, kept loaded to check several names
struct ProfWordLists {
    u16 *lists[PROF_WORD_LIST_COUNT];
    u32 sizes[PROF_WORD_LIST_COUNT];
};

// pokebw2's ProfWord_FreeLists
void func_ov194_021f5354(ProfWordLists *lists);
// pokebw2's ProfWord_HasManyDigits
// Returns TRUE if the first len characters of the name hold more than 4 digits, which could be a phone number
BOOL func_ov194_021f54cc(const u16 *name, u32 len, HeapID unused);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ProfWord_LoadLists
ProfWordLists *func_ov194_021f5304(HeapID heapId);
// pokebw2's ProfWord_Check
// Returns TRUE if the name, with its case and kana folded, is in a list; lists may be NULL to read them one at a time
BOOL func_ov194_021f5370(ProfWordLists *lists, const u16 *name, HeapID heapId);

#endif // BWPORT_SYSTEM_PROF_WORD_H
