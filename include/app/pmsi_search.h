// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pmsi_search.h.
#ifndef BWPORT_APP_PMSI_SEARCH_H
#define BWPORT_APP_PMSI_SEARCH_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The phrase input's search, of overlay 110's pmsi_search.c: the player types the first letters of a word, and the
// search lists the unlocked words that start with them. The names are Black's dsd names; pokebw2's guesses are in the
// comments

// How many letters the player can type
#define PMSI_SEARCH_INPUT_MAX 13
// No letter
#define PMSI_SEARCH_INPUT_NONE 0xff

PMSISearch *func_ov110_021baedc(const PMSInputWork *mwk, const PMSInputData *dwk, HeapID heapId); // PMSISearch_Create
void func_ov110_021baf4c(PMSISearch *ss); // PMSISearch_Delete
// Types the letter of an initial (PMSISearch_AddChar)
void func_ov110_021baf7c(PMSISearch *ss, u16 initial);
// Erases the last letter, or returns FALSE if there is none (PMSISearch_DelChar)
BOOL func_ov110_021bafac(PMSISearch *ss);
u8 func_ov110_021bafdc(PMSISearch *ss); // PMSISearch_GetInputLen
void func_ov110_021bafe4(PMSISearch *ss); // PMSISearch_Reset
// The letters typed (PMSISearch_GetInputStr)
void func_ov110_021bb00c(PMSISearch *ss, StrBuf *buf);
// Lists the words that start with the letters typed, and returns whether there are any (PMSISearch_Search)
BOOL func_ov110_021bb054(PMSISearch *ss);
u8 func_ov110_021bb1c0(PMSISearch *ss); // PMSISearch_GetResultCount
void func_ov110_021bb1cc(PMSISearch *ss, u8 index, StrBuf *buf); // PMSISearch_GetResultStr
u16 func_ov110_021bb1e4(PMSISearch *ss, u32 index); // PMSISearch_GetResultWord

#endif // BWPORT_APP_PMSI_SEARCH_H
