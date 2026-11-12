// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/pmsi_param.h, the lines in use so
// far. pokebw2's names, its own guesses, are in comments.
#ifndef BWPORT_SYSTEM_PMSI_PARAM_H
#define BWPORT_SYSTEM_PMSI_PARAM_H

#include "types.h"
#include "struct_decls.h"

// The parameters of the sentence input screen (pmsi_param.c): what it edits, one word, two words or a sentence, the
// starting value and, once it closes, the result

// PMSIParam_Create: sentence is the starting sentence, or NULL for an empty one; unused is always 0
PMSIParam *func_02024044(u32 mode, u32 unused, const PMSData *sentence, BOOL flag3, SaveControl *save, u32 heapId);
// PMSIParam_Free
void func_020240fc(PMSIParam *param);
// PMSIParam_SetSentence
void func_02024108(PMSIParam *param, const PMSData *sentence);
// PMSIParam_IsCanceled
BOOL func_0202411c(const PMSIParam *param);
// PMSIParam_GetWord
u16 func_02024124(const PMSIParam *param);
// PMSIParam_GetWords
void func_02024128(const PMSIParam *param, u16 *words);
// PMSIParam_GetSentence
void func_02024134(const PMSIParam *param, PMSData *sentence);

enum {
    PMSI_MODE_WORD,
    PMSI_MODE_DOUBLE_WORD,
    PMSI_MODE_SENTENCE,
};

// pokebw2's PMSIParam_SetWord
void func_02024104(PMSIParam *param, u16 word);
BOOL func_02024150(const PMSIParam *param);
// pokebw2's PMSIParam_HasStartSentence
BOOL func_02024158(const PMSIParam *param);
// pokebw2's PMSIParam_HasNumbers
BOOL func_02024160(const PMSIParam *param);
// pokebw2's PMSIParam_GetResult
// The result, in words or sentence by the mode
void func_02024168(const PMSIParam *param, u16 *words, PMSData *sentence);
// pokebw2's PMSIParam_SetResult
// Sets the result and whether it differs from the starting value
void func_020241e0(PMSIParam *param, const u16 *words, const PMSData *sentence);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PMSIParam_IsChanged
BOOL func_02024120(const PMSIParam *param);
// pokebw2's PMSIParam_GetMode
u32 func_02024144(const PMSIParam *param);
// pokebw2's PMSIParam_GetPokeDex
void *func_02024148(const PMSIParam *param);
// pokebw2's PMSIParam_GetWordSave
PMSWordSave *func_0202414c(const PMSIParam *param);
BOOL func_0202421c(const PMSIParam *param);
void func_02024224(PMSIParam *param, BOOL touch);

#endif // BWPORT_SYSTEM_PMSI_PARAM_H
