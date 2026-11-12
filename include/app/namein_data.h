// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/namein_data.h: the lines in use so far.
#ifndef BWPORT_APP_NAMEIN_DATA_H
#define BWPORT_APP_NAMEIN_DATA_H

#include "types.h"
#include "gfl/heap.h"

// The characters that can be entered are in keyboards, a grid each. 0x3000 (an ideographic space) fills the unused
// places of a grid, a word or a row
#define NAMEIN_EMPTY_CHAR 0x3000

// The most characters of one of a word's spellings
#define NAMEIN_WORD_CHARS 3

#define NAMEIN_KEYBOARD_COUNT 5

// A grid of characters, width by height, row by row
typedef struct {
    u16 width;
    u16 height;
    u16 chars[];
} NameinKeyboard;

typedef struct {
    NameinKeyboard *keyboards[NAMEIN_KEYBOARD_COUNT];
} NameinKeyboardSet;

// A word as it is spelled in four ways (the member of the table that it is read with picks one)
typedef struct {
    u16 forms[4][NAMEIN_WORD_CHARS];
} NameinWord;

typedef struct {
    u16 count;
    NameinWord words[];
} NameinWordTable;

// The characters that change into each other: the rows of NAMEIN_WORD_CHARS characters, whose unused places are
// NAMEIN_EMPTY_CHAR
typedef struct {
    u16 count;
    u16 chars[][NAMEIN_WORD_CHARS];
} NameinConvTable;

// pokebw2's NameinData_LoadKeyboards
NameinKeyboardSet *func_ov194_021f4d3c(HeapID heapId);
// pokebw2's NameinData_FreeKeyboards
void func_ov194_021f4d78(NameinKeyboardSet *set);
// pokebw2's NameinKeyboard_GetChar
u16 func_ov194_021f4da4(const NameinKeyboard *keyboard, u16 x, u16 y);
// pokebw2's NameinWordTable_GetForm0
// The four spellings of a word, copied to dest with at most n characters. len gets how many characters before the first
// NAMEIN_EMPTY_CHAR. All but the first return whether there are any
void func_ov194_021f4ddc(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len);
// pokebw2's NameinWordTable_GetForm1
BOOL func_ov194_021f4e18(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len);
// pokebw2's NameinWordTable_GetForm2
BOOL func_ov194_021f4e58(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len);
// pokebw2's NameinWordTable_GetForm3
BOOL func_ov194_021f4e98(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len);
// pokebw2's NameinWordTable_Find
// The index of the word whose first spelling is str, or 0xffff
u16 func_ov194_021f4ed8(const NameinWordTable *table, const u16 *str);
// pokebw2's NameinConvTable_GetNext
// Writes to out the character that follows in's in its row; FALSE if in has none
BOOL func_ov194_021f4f7c(const NameinConvTable *table, const u16 *in, u16 *out);
// pokebw2's NameinData_IsNameValid
// Whether every character of name is on a keyboard or in the conversion table
BOOL func_ov194_021f5004(const u16 *name, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's NameinData_GetKeyboard
NameinKeyboard *func_ov194_021f4d94(const NameinKeyboardSet *set, u32 index);
// pokebw2's NameinKeyboard_GetWidth
u16 func_ov194_021f4d9c(const NameinKeyboard *keyboard);
// pokebw2's NameinKeyboard_GetHeight
u16 func_ov194_021f4da0(const NameinKeyboard *keyboard);
// pokebw2's NameinData_LoadWordTable
NameinWordTable *func_ov194_021f4dbc(u32 index, HeapID heapId);
// pokebw2's NameinData_FreeWordTable
void func_ov194_021f4dd0(NameinWordTable *table);
// pokebw2's NameinData_LoadConvTable
NameinConvTable *func_ov194_021f4f50(HeapID heapId);
// pokebw2's NameinData_FreeConvTable
void func_ov194_021f4f60(NameinConvTable *table);

#endif // BWPORT_APP_NAMEIN_DATA_H
