// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pms_input_data.h, the lines in use so
// far.
#ifndef BWPORT_APP_PMS_INPUT_DATA_H
#define BWPORT_APP_PMS_INPUT_DATA_H

#include "types.h"
#include "struct_decls.h"

// The words that the phrase input offers, of overlay 110's pms_input_data.c: which words of each category and of each
// initial the player has unlocked. The names are Black's dsd names; pokebw2's guesses are in the comments

// The initials the words are sorted by, A to Z and the others
#define PMSI_INITIAL_COUNT 27

// Copies the unlocked words of a table to dst, and returns how many (PMSIData_CountTableWords)
u32 func_ov110_021b99d8(const PMSInputData *data, const u16 *table, u16 *dst);
// The word at a position of an initial's table, or 0 (PMSIData_GetInitialWordCode)
u16 func_ov110_021b99f0(const PMSInputData *data, u32 initial, u32 index);
void func_ov110_021b9a3c(const PMSInputData *data, u16 word, StrBuf *buf); // PMSIData_GetWordStr
u16 func_ov110_021b9a48(const PMSInputData *data); // PMSIData_GetEndWord

// The categories of words
#define PMSI_CATEGORY_COUNT 12

// The marks in the word tables: a group of words shown as one, and the end of a table
#define PMS_WORD_DUP 0xfffe

#define PMS_WORD_END 0xffff

// Black 2's, not checked against Black
// Categories 2, 4, 8, 9 and 10, whose words are always unlocked. Nothing reads them
#define PMSI_FIXED_CATEGORY_COUNT 5

// pokebw2's PMSIData_Create
PMSInputData *func_ov110_021b95a0(u32 heapId, const PMSIParam *param);
// pokebw2's PMSIData_Delete
void func_ov110_021b95f4(PMSInputData *data);
// pokebw2's PMSIData_GetCategoryWordCount
u32 func_ov110_021b999c(const PMSInputData *data, u32 category);
// pokebw2's PMSIData_GetCategoryWord
void func_ov110_021b99a4(const PMSInputData *data, u32 category, u32 index, StrBuf *buf);
// pokebw2's PMSIData_GetCategoryWordCode
u16 func_ov110_021b99c4(const PMSInputData *data, u32 category, u32 index);

#endif // BWPORT_APP_PMS_INPUT_DATA_H
