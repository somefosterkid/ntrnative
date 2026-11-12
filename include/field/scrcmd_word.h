// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_word.h, the commands Black has.
#ifndef BWPORT_FIELD_SCRCMD_WORD_H
#define BWPORT_FIELD_SCRCMD_WORD_H

// Overlay 21's scrcmd_word.c: the script commands that set the words of messages. Names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), whose numbers are the commands' in Black 2; Black's entry in
// its command table, FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS, follows each

#include "types.h"
#include "struct_decls.h"

BOOL s004C_WordSetPlayerName(VM *vm, FieldScriptEnv *env); // 0x5B
BOOL s004D_WordSetItemName(VM *vm, FieldScriptEnv *env); // 0x5C
BOOL s004F_WordSetItemNameWithArticle(VM *vm, FieldScriptEnv *env); // 0x5E
BOOL s004E_WordSetItemNameEx(VM *vm, FieldScriptEnv *env); // 0x5D
BOOL s0050_WordSetTMMoveName(VM *vm, FieldScriptEnv *env); // 0x5F
BOOL s0051_WordSetMoveName(VM *vm, FieldScriptEnv *env); // 0x60
BOOL s0056_WordSetPokeTypeName(VM *vm, FieldScriptEnv *env); // 0x65
BOOL s0057_WordSetPokeSpecies(VM *vm, FieldScriptEnv *env); // 0x66
BOOL s0058_WordSetPokeSpeciesWithArticle(VM *vm, FieldScriptEnv *env); // 0x67
BOOL s0052_WordSetItemPocketName(VM *vm, FieldScriptEnv *env); // 0x61
BOOL s0053_WordSetPartyPokeSpecies(VM *vm, FieldScriptEnv *env); // 0x62
BOOL s0054_WordSetPartyPokeName(VM *vm, FieldScriptEnv *env); // 0x63
BOOL s0055_WordSetDaycarePokeSpecies(VM *vm, FieldScriptEnv *env); // 0x64
BOOL s005B_WordSetDaycarePokeName(VM *vm, FieldScriptEnv *env); // 0x6A
BOOL s005C_WordSetNumber(VM *vm, FieldScriptEnv *env); // 0x6B
BOOL s0059_WordSetPlaceName(VM *vm, FieldScriptEnv *env); // 0x68
BOOL s005A_WordSetTrendName(VM *vm, FieldScriptEnv *env); // 0x69
BOOL s005E_WordSetCountry(VM *vm, FieldScriptEnv *env); // 0x6D
BOOL s005F_WordSetHobbyName(VM *vm, FieldScriptEnv *env); // 0x6E
BOOL s0063_WordSetSurveyAnswer(VM *vm, FieldScriptEnv *env); // 0x72
BOOL s0060_WordSetPassPowerName(VM *vm, FieldScriptEnv *env); // 0x6F
BOOL s0061_WordSetTrainerClassName(VM *vm, FieldScriptEnv *env); // 0x70
BOOL s0062_WordSetTrainerClassNameWithArticle(VM *vm, FieldScriptEnv *env); // 0x71
// pokebw2's func_ov036_021a7c64 and func_ov036_021a7cb4
BOOL func_ov021_021afc38(VM *vm, FieldScriptEnv *env); // 0x229
BOOL func_ov021_021afc88(VM *vm, FieldScriptEnv *env); // 0x22A

#endif // BWPORT_FIELD_SCRCMD_WORD_H
