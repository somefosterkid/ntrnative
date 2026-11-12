// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/pms_data.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_PMS_DATA_H
#define BWPORT_SYSTEM_PMS_DATA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Sentences: a sentence from one of the message files of sentence types, with up to two words filled into its word
// set commands, as trainers' greetings and the mail use them

// Up to two words fill a sentence
#define PMS_SENTENCE_WORD_MAX 2

struct PMSData {
    // PMS_WORD_NULL when the sentence is empty
    u16 type;
    u16 id;
    u16 words[PMS_SENTENCE_WORD_MAX];
};

// pokebw2's PMSData_ToString
StrBuf *func_0202435c(const PMSData *data, u32 heapId);

// pokebw2's PMSData_Init
void func_020242a4(PMSData *data, u16 type);
// pokebw2's PMSData_Copy
void func_02024570(PMSData *dest, const PMSData *src);
// pokebw2's PMSData_Equals
BOOL func_02024534(const PMSData *data, const PMSData *other);

// pokebw2's PMSData_Clear
void func_0202428c(PMSData *data);

// The sentence types, the message file each one's sentences are in
#define PMS_SENTENCE_TYPE_COUNT 7

// pokebw2's PMSData_ToStringWithWords
// The sentence with its first wordCount words filled in
StrBuf *func_02024368(const PMSData *data, u32 heapId, int wordCount);
// pokebw2's PMSData_GetSentenceString
// The sentence without its words filled in
StrBuf *func_0202441c(const PMSData *data, u32 heapId);
// pokebw2's PMSData_IsComplete
// Whether every word the sentence has a command for is filled in
BOOL func_02024464(const PMSData *data, HeapID heapId);
u16 func_020244d4(const PMSData *data, u32 index);
// pokebw2's PMSData_GetWord
u16 func_020244f0(const PMSData *data, u32 index);
BOOL func_020244f8(const PMSData *data, u32 index);
// pokebw2's PMSWord_IsNumber
BOOL func_02024508(u16 word);
// pokebw2's PMSWord_GetNumber
int func_02024520(u16 word);
// pokebw2's PMSData_GetSentenceCount
// The number of sentences of a type
u32 func_02024584(u32 type);
// pokebw2's PMSData_SetWord
void func_020245a0(PMSData *data, u32 index, u16 word);
// pokebw2's PMSData_ClearUnusedWords
// Empties the words the sentence has no command for
void func_020245ac(PMSData *data, HeapID heapId);
// pokebw2's PMSData_IsValid
BOOL func_020245d4(const PMSData *data, u32 heapId);
// pokebw2's PMSData_Validate
// Replaces an invalid sentence or word with a default one, and returns whether all of it was valid
BOOL func_0202465c(PMSData *data, BOOL allowEmpty, HeapID heapId);
// pokebw2's PMSWord_Validate
BOOL func_020246c8(u16 *word, BOOL allowEmpty, BOOL allowNumber);
// pokebw2's PMSNumber_Validate
BOOL func_0202473c(int *number, BOOL allowZero);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSData_IsNotEmpty
BOOL func_02024450(const PMSData *data);
// pokebw2's PMSData_InitWithSentence
void func_020242c0(PMSData *data, u16 type, u16 id);
// pokebw2's PMSData_GetType
u16 func_0202452c(const PMSData *data);
// pokebw2's PMSData_GetID
u16 func_02024530(const PMSData *data);
// pokebw2's PMSData_SetSentence
void func_02024598(PMSData *data, u32 type, u32 id);

void func_02024344(PMSData *data);
// One of four fixed sentences
void func_020242d8(PMSData *data, u32 preset);

#endif // BWPORT_SYSTEM_PMS_DATA_H
