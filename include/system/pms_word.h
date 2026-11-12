// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/pms_word.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SYSTEM_PMS_WORD_H
#define BWPORT_SYSTEM_PMS_WORD_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Loads one word, or clears the string for PMS_WORD_NULL
void loadSayingToString(u16 saying, StrBuf *strbuf, HeapID heapId);

// No word
#define PMS_WORD_NULL 0xffff

PMSWordSave *getDexBlkAddress(SaveControl *save);

// pokebw2's PMSWord_GetMessage
// The category and message index of a word, FALSE if it is past the last category
BOOL func_020248d8(u32 word, u32 *category, u32 *index);
// pokebw2's PMSWord_FromMessage
// The word that message index of message file fileId is, or PMS_WORD_NULL
u16 func_02024888(u16 fileId, u16 index);
#define PMS_WORD_INDEX_MASK 0x7ff

// The word categories, each with a message file
#define PMS_WORD_CATEGORY_COUNT 13

// Black 2's, not checked against Black
// The save block of sentences
#define SAVE_BLOCK_PMS 0x27

// pokebw2's PMSWordBank_Create
// The message files of the word categories, loaded for a while
PMSWordBank *func_020247a8(u32 heapId);
// pokebw2's PMSWordBank_Free
void func_020247f0(PMSWordBank *bank);
// pokebw2's PMSWordBank_LoadWord
void func_02024810(PMSWordBank *bank, u16 word, StrBuf *strbuf);
// pokebw2's PMSWord_FromMessage
// The word that message index of message file fileId is, or PMS_WORD_NULL
// pokebw2's PMSWord_GetMessage
// The category and message index of a word, FALSE if it is past the last category
// pokebw2's PMSWordSave_GetSentence
PMSData *func_02024910(PMSWordSave *save, int index);
// pokebw2's PMSWordSave_SetSentence
void func_02024918(PMSWordSave *save, int index, const PMSData *sentence);
// pokebw2's PMSWordSave_Init
void func_02024938(PMSWordSave *save);
// pokebw2's PMSWordSave_GetLanguageFlag
// A flag for each of seven languages, by their order in the save's language table; a new save sets the game's own
BOOL func_02024994(PMSWordSave *save, u32 bit);
// pokebw2's PMSWord_AreEquivalent
// Whether two words are the same word, or two of a group of words that count as the same
BOOL func_020249b0(u16 word, u16 other);

// A word with this bit set is a number, in its low bits, rather than a message
#define PMS_WORD_NUMBER_SHIFT 11

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSWordSave_GetSize
u32 func_02024934(void);
// pokebw2's PMSWordSave_SetLanguageFlag
void func_020249a0(PMSWordSave *save, u32 bit);

#endif // BWPORT_SYSTEM_PMS_WORD_H
