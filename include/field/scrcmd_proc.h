// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_proc.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SCRCMD_PROC_H
#define BWPORT_FIELD_SCRCMD_PROC_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s014A_FieldOpen(VM *vm, FieldScriptEnv *env);
BOOL s014B_FieldClose(VM *vm, FieldScriptEnv *env);
void CreateScrCmdOverlayProcess(VM *vm, FieldScriptEnv *env, s32 overlayId, const GameProcFunctions *functions,
                                void *resource, void (*cleanup)(ScriptOverlayWork *), void *data);
BOOL s0154_Call3DDemo(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215c69c(VM *vm, void *data);
// Called before the Pokédex diplomas
void func_ov010_0215c7c4(void *arg);
void func_ov010_0215c870(void *arg);
BOOL func_ov010_0215cc38(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s014C_RTFreeUserHeap(VM *vm, FieldScriptEnv *env);
BOOL s014E_CallBag(VM *vm, FieldScriptEnv *env);
BOOL s0150_CallMailbox(VM *vm, FieldScriptEnv *env);
BOOL s01D6_MoveReminderCallMoveSelect(VM *vm, FieldScriptEnv *env);
BOOL s0160_NetConnectWiFiBattle(VM *vm, FieldScriptEnv *env);
BOOL s0161_NetConnectBattleVideo(VM *vm, FieldScriptEnv *env);
void func_ov010_0215c700(ScriptOverlayWork *work);
BOOL func_ov010_0215c95c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215cd3c(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215cd80(VM *vm, FieldScriptEnv *env);
BOOL func_ov010_0215cdb8(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_PROC_H
