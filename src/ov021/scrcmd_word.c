// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_word.c, adapted to Black: it has
// no rival name, medal, ability, nature or Join Avenue commands, its item pocket command names the pocket it is given,
// and its WordSetItemNameEx has no return value.

// The script commands that set the words of messages: names of the player, items, moves, Pokémon, places and the
// rest, and numbers. The ROM has no name for the file; scrcmd_word.c is descriptive. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field.h"
#include "field/field_daycare.h"
#include "field/field_script.h"
#include "field/scrcmd_word.h"
#include "field/zone.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "save/save_control.h"
#include "save/trainer_card.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "system/wordset.h"

typedef void (*WordSetLoadFunc)(WordSet *wordSet, u32 index, u32 value);
// The trainer class loaders take the class as a u8
typedef void (*WordSetLoadByteFunc)(WordSet *wordSet, u32 index, u8 value);

static BOOL loadAllItemText(VM *vm, FieldScriptEnv *env, WordSetLoadFunc load);
static BOOL loadAllPkmNameText(VM *vm, FieldScriptEnv *env, WordSetLoadFunc load);
static BOOL func_ov021_021afbec(VM *vm, FieldScriptEnv *env, WordSetLoadByteFunc load);

BOOL s004C_WordSetPlayerName(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptWork_GetFieldWork(work);
    PlayerInfo *playerInfo = GetGameDataPlayerInfo(FieldScriptEnv_GetGameData(env));

    WordSet *wordSet = ScriptWork_GetWordSet(work);

    copyVarForText(wordSet, VM_Read8(vm), playerInfo);
    return FALSE;
}

static BOOL loadAllItemText(VM *vm, FieldScriptEnv *env, WordSetLoadFunc load) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    load(wordSet, index, ScriptReadAny(vm, env));
    return FALSE;
}

BOOL s004D_WordSetItemName(VM *vm, FieldScriptEnv *env) {
    return loadAllItemText(vm, env, loadItemNameToStrbuf);
}

BOOL s004F_WordSetItemNameWithArticle(VM *vm, FieldScriptEnv *env) {
    return loadAllItemText(vm, env, loadItemTextNameToStrbuf);
}

// The item's name, plural when there are more than one, or with its article
BOOL s004E_WordSetItemNameEx(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);
    u16 item = ScriptReadAny(vm, env);
    u16 count = ScriptReadAny(vm, env);
    u8 withArticle = VM_Read8(vm);

    if (count > 1) {
        loadItemsNameToStrbuf(wordSet, index, item);
    } else if (withArticle) {
        loadItemTextNameToStrbuf(wordSet, index, item);
    } else {
        loadItemNameToStrbuf(wordSet, index, item);
    }
    // BUG: Nothing is returned, so the command returns whatever the word set function left in r0, and a nonzero
    // value makes VM_Run return before the next command
#ifdef BUGFIX
    return FALSE;
#endif
}

BOOL s0050_WordSetTMMoveName(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    loadMoveNameToStrbuf(wordSet, index, PML_ItemGetTMWazaID(ScriptReadAny(vm, env)));
    return FALSE;
}

BOOL s0051_WordSetMoveName(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    loadMoveNameToStrbuf(wordSet, index, ScriptReadAny(vm, env));
    return FALSE;
}

BOOL s0056_WordSetPokeTypeName(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    loadTypeTextToStrbuf(wordSet, index, ScriptReadAny(vm, env));
    return FALSE;
}

static BOOL loadAllPkmNameText(VM *vm, FieldScriptEnv *env, WordSetLoadFunc load) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    load(wordSet, index, ScriptReadAny(vm, env));
    return FALSE;
}

BOOL s0057_WordSetPokeSpecies(VM *vm, FieldScriptEnv *env) {
    return loadAllPkmNameText(vm, env, WordSet_LoadSpeciesName);
}

BOOL s0058_WordSetPokeSpeciesWithArticle(VM *vm, FieldScriptEnv *env) {
    return loadAllPkmNameText(vm, env, loadPokemonTextNameToStrbuf);
}

BOOL s0052_WordSetItemPocketName(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    loadBagPocketNameToStrbuf(wordSet, index, ScriptReadAny(vm, env));
    return FALSE;
}

BOOL s0053_WordSetPartyPokeSpecies(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    u8 index = VM_Read8(vm);
    u16 slot = ScriptReadAny(vm, env);

    setPartyPokemonSpeciesNameToStrbuf(wordSet, index, PokeParty_GetPkm(GameData_GetParty(gameData), slot));
    return FALSE;
}

BOOL s0054_WordSetPartyPokeName(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    u8 index = VM_Read8(vm);
    u16 slot = ScriptReadAny(vm, env);

    loadPokemonNicknameToStrbuf(wordSet, index, PokeParty_GetPkm(GameData_GetParty(gameData), slot));
    return FALSE;
}

BOOL s0055_WordSetDaycarePokeSpecies(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);
    u16 slot = ScriptReadAny(vm, env);

    setPartyPokemonSpeciesNameToStrbuf(
        wordSet, index, DayCare_GetPkm(Field_GetDayCare(GSYS_GetField(FieldScriptEnv_GetGameSystem(env))), slot));
    return FALSE;
}

BOOL s005B_WordSetDaycarePokeName(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);
    u16 slot = ScriptReadAny(vm, env);

    loadPokemonNicknameToStrbuf(
        wordSet, index, DayCare_GetPkm(Field_GetDayCare(GSYS_GetField(FieldScriptEnv_GetGameSystem(env))), slot));
    return FALSE;
}

BOOL s005C_WordSetNumber(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);
    u16 number = ScriptReadAny(vm, env);
    u16 digits = ScriptReadAny(vm, env);

    WordSetNumber(wordSet, index, number, digits, 0, TRUE);
    return FALSE;
}

BOOL s0059_WordSetPlaceName(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    loadLocationNameToStrbuf(wordSet, index, ZoneData_GetPlaceNameID(ScriptReadAny(vm, env)));
    return FALSE;
}

BOOL s005A_WordSetTrendName(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    loadSayingForDisplay(wordSet, index, ScriptReadAny(vm, env));
    return FALSE;
}

BOOL s005E_WordSetCountry(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    loadCountryToStrbuf(wordSet, index, ScriptReadAny(vm, env));
    return FALSE;
}

BOOL s005F_WordSetHobbyName(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);

    loadHobbyNameToStrbuf(wordSet, index, ScriptReadAny(vm, env));
    return FALSE;
}

BOOL s0063_WordSetSurveyAnswer(VM *vm, FieldScriptEnv *env) {
    WordSet *wordSet = ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env));
    u8 index = VM_Read8(vm);
    u16 answer = ScriptReadAny(vm, env);

    loadQuestionnaireAnswerToStrbuf(wordSet, index, answer);
    return FALSE;
}

BOOL s0060_WordSetPassPowerName(VM *vm, FieldScriptEnv *env) {
    u8 index = VM_Read8(vm);
    u16 passPower = ScriptReadAny(vm, env);

    loadPassPowerToStrbuf(ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env)), index, passPower);
    return FALSE;
}

static BOOL func_ov021_021afbec(VM *vm, FieldScriptEnv *env, WordSetLoadByteFunc load) {
    u8 index = VM_Read8(vm);
    u16 value = ScriptReadAny(vm, env);

    load(ScriptWork_GetWordSet(FieldScriptEnv_GetScriptWork(env)), index, value);
    return FALSE;
}

BOOL s0061_WordSetTrainerClassName(VM *vm, FieldScriptEnv *env) {
    return func_ov021_021afbec(vm, env, loadTrainerTypeText);
}

BOOL s0062_WordSetTrainerClassNameWithArticle(VM *vm, FieldScriptEnv *env) {
    return func_ov021_021afbec(vm, env, loadTrainerTypeWithArticleToStrbuf);
}

BOOL func_ov021_021afc38(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    TrainerGameInfoSave *info = getTrainerGameInfoAddress(GameData_GetSaveControl(FieldScriptEnv_GetGameData(env)));
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    u8 index = VM_Read8(vm);

    WordSetNumber(wordSet, index, func_0200c1dc(info), 9, 1, TRUE);
    return FALSE;
}

BOOL func_ov021_021afc88(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    TrainerGameInfoSave *info = getTrainerGameInfoAddress(GameData_GetSaveControl(FieldScriptEnv_GetGameData(env)));
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    u8 index = VM_Read8(vm);

    WordSetNumber(wordSet, index, func_0200c1c4(info), 9, 1, TRUE);
    return FALSE;
}
