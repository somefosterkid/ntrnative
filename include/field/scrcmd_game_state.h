// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_game_state.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_SCRCMD_GAME_STATE_H
#define BWPORT_FIELD_SCRCMD_GAME_STATE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s00CB_Random(VM *vm, FieldScriptEnv *env);
BOOL s00CC_RTGetTextFile(VM *vm, FieldScriptEnv *env);
BOOL s00D0_RTCGetDate(VM *vm, FieldScriptEnv *env);
BOOL s00D2_RTCGetSeason(VM *vm, FieldScriptEnv *env);
BOOL s00D4_TrainerCardGetBirthDate(VM *vm, FieldScriptEnv *env);
BOOL s00E1_TrainerCardGetSex(VM *vm, FieldScriptEnv *env);
BOOL s00D5_TrainerCardHasBadge(VM *vm, FieldScriptEnv *env);
BOOL s00D6_TrainerCardAddBadge(VM *vm, FieldScriptEnv *env);
BOOL s00D7_TrainerCardGetBadgeCount(VM *vm, FieldScriptEnv *env);
BOOL s0138_SaveDataGetStatus(VM *vm, FieldScriptEnv *env);
BOOL s00D9_FieldSetTeleportZone(VM *vm, FieldScriptEnv *env);
BOOL s00DB_FieldSetNextZoneHere(VM *vm, FieldScriptEnv *env);
BOOL s00DC_FieldSetNextZone(VM *vm, FieldScriptEnv *env);
BOOL s00DA_MapReplaceSetEvent(VM *vm, FieldScriptEnv *env);
BOOL s00D8_MapReplaceIsEventSet(VM *vm, FieldScriptEnv *env);
BOOL s00DE_PokeDexRegist(VM *vm, FieldScriptEnv *env);
BOOL s00DF_PokeDexIsRegist(VM *vm, FieldScriptEnv *env);
BOOL s00DD_PokeDexGetCount(VM *vm, FieldScriptEnv *env);
BOOL s00E2_SaveDataCheckRequired(VM *vm, FieldScriptEnv *env);
BOOL s00E3_GiveRunningShoes(VM *vm, FieldScriptEnv *env);
BOOL s01D7_ObjInitPointGPos(VM *vm, FieldScriptEnv *env);
BOOL s01D9_ObjInitNPCGPos(VM *vm, FieldScriptEnv *env);
BOOL s0095_TrainerFlagSet(VM *vm, FieldScriptEnv *env);
BOOL s0097_TrainerFlagGet(VM *vm, FieldScriptEnv *env);
BOOL s008C_CallTrainerLose(VM *vm, FieldScriptEnv *env);
BOOL s008D_TrainerBattleIsVictory(VM *vm, FieldScriptEnv *env);
BOOL s0177_WildBattleIsVictory(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215aa60(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215aa90(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215aaf4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215b084(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215b0b4(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215b104(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215b138(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215b158(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s00E0_GameGetVersion(VM *vm, FieldScriptEnv *env);
BOOL s00CD_RTCGetDayPart(VM *vm, FieldScriptEnv *env);
BOOL s00CF_RTCGetWeekDay(VM *vm, FieldScriptEnv *env);
BOOL s00D1_RTCGetTime(VM *vm, FieldScriptEnv *env);
BOOL s00D3_RTGetZoneID(VM *vm, FieldScriptEnv *env);
BOOL s00EA_HOFCheckIntegrity(VM *vm, FieldScriptEnv *env);
BOOL s01D8_ObjInitWarpGPos(VM *vm, FieldScriptEnv *env);
BOOL s0096_TrainerFlagReset(VM *vm, FieldScriptEnv *env);
BOOL s0176_CallWildLose(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_GAME_STATE_H
