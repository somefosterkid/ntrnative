// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_pokemon.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_POKEMON_H
#define BWPORT_FIELD_SCRCMD_POKEMON_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL CheckGetPartyPokemon(FieldScriptEnv *env, u32 index, PartyPkm **pkm);
u32 GetScrPokeStat(FieldScriptEnv *env, u32 index, u32 param);
BOOL CheckPokeMoveLearned_NonEgg(PartyPkm *pkm, u16 move);

BOOL s011B_PokePartyGetTypes(VM *vm, FieldScriptEnv *env);
BOOL s0112_PokePartyGetEVTotal(VM *vm, FieldScriptEnv *env);
BOOL s0101_PokePartyIsFullHP(VM *vm, FieldScriptEnv *env);
BOOL s0102_PokePartyIsEgg(VM *vm, FieldScriptEnv *env);
BOOL s00FC_PokePartyGetHappiness(VM *vm, FieldScriptEnv *env);
BOOL s00FD_PokePartyAdjustHappiness(VM *vm, FieldScriptEnv *env);
BOOL s00FE_PokePartyGetSpecies(VM *vm, FieldScriptEnv *env);
BOOL s00FF_PokePartyGetForme(VM *vm, FieldScriptEnv *env);
BOOL s010D_PokePartyGetMemberByType(VM *vm, FieldScriptEnv *env);
BOOL s0103_PokePartyGetCount(VM *vm, FieldScriptEnv *env);
BOOL s0114_PokePartyGetCountBySpecies(VM *vm, FieldScriptEnv *env);
BOOL s0121_BoxGetCount(VM *vm, FieldScriptEnv *env);
BOOL s0118_PokePartyFindBySpecies(VM *vm, FieldScriptEnv *env);
BOOL s0115_PokePartyHasMove(VM *vm, FieldScriptEnv *env);
BOOL s0116_PokePartyHasMoveAny(VM *vm, FieldScriptEnv *env);
BOOL s0108_PokePartyGetMoveCount(VM *vm, FieldScriptEnv *env);
BOOL s010A_PokePartyGetMove(VM *vm, FieldScriptEnv *env);
BOOL s010B_PokePartyLearnMove(VM *vm, FieldScriptEnv *env);
BOOL s0113_PokePartyIsOriginGame(VM *vm, FieldScriptEnv *env);
BOOL s0117_PokePartySetForme(VM *vm, FieldScriptEnv *env);
BOOL s011C_PokePartyChangeRotomForme(VM *vm, FieldScriptEnv *env);
BOOL s01D5_MoveReminderCheckPkm(VM *vm, FieldScriptEnv *env);
BOOL s0119_PokePartyIsFromWhiteForest(VM *vm, FieldScriptEnv *env);
BOOL s011A_PokePartyGetMetDate(VM *vm, FieldScriptEnv *env);
BOOL s0111_PokePartySetIV(VM *vm, FieldScriptEnv *env);
BOOL s010F_PokePartyAddEgg(VM *vm, FieldScriptEnv *env);
BOOL s00F9_MoneyAdd(VM *vm, FieldScriptEnv *env);
BOOL s00FB_MoneyCheck(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s0104_PokePartyRecoverAll(VM *vm, FieldScriptEnv *env);
BOOL s0110_PokePartyGetParam(VM *vm, FieldScriptEnv *env);
BOOL s0122_BoxAdd(VM *vm, FieldScriptEnv *env);
BOOL s0123_BoxAddEx(VM *vm, FieldScriptEnv *env);
BOOL s00FA_MoneySub(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_POKEMON_H
