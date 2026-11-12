// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_msg.h: the lines in use so far.
#ifndef BWPORT_FIELD_SCRCMD_MSG_H
#define BWPORT_FIELD_SCRCMD_MSG_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "field/field_script.h"
#include "struct_decls.h"

BOOL s0037_MsgSetLoadingSpinner(VM *vm, FieldScriptEnv *env);
BOOL s0036_InfoMsgClose(VM *vm, FieldScriptEnv *env);
BOOL s0041_MoneyWinClose(VM *vm, FieldScriptEnv *env);
BOOL s003B_MsgWinCloseNo(VM *vm, FieldScriptEnv *env);
BOOL s003F_MsgWinCloseAll(VM *vm, FieldScriptEnv *env);
BOOL s0033_MsgSetAutoscrolls(VM *vm, FieldScriptEnv *env);
// Close the windows when the script's sub events are finished, in FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS
BOOL func_ov021_021b0630(FinishScriptSubEventsWork *work, u32 *state);
BOOL func_ov021_021b1604(FinishScriptSubEventsWork *work, u32 *state);
BOOL func_ov021_021b0ee8(FinishScriptSubEventsWork *work, u32 *state);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s0034_SystemMsg(VM *vm, FieldScriptEnv *env);
BOOL s0035_SystemMsgAsync(VM *vm, FieldScriptEnv *env);
BOOL s0040_MoneyWinDisp(VM *vm, FieldScriptEnv *env);
BOOL s0042_MoneyWinUpdate(VM *vm, FieldScriptEnv *env);
BOOL s0038_InfoMsg(VM *vm, FieldScriptEnv *env);
BOOL s004A_ScreamMsg(VM *vm, FieldScriptEnv *env);
BOOL s0039_InfoMsgClose(VM *vm, FieldScriptEnv *env);
BOOL s003A_MultiMsg(VM *vm, FieldScriptEnv *env);
BOOL s0043_MsgPlaceSign(VM *vm, FieldScriptEnv *env);
BOOL s0044_MsgPlaceSignClose(VM *vm, FieldScriptEnv *env);
BOOL s0045_CheckerMsg(VM *vm, FieldScriptEnv *env);
BOOL s0046_CheckerMsgClose(VM *vm, FieldScriptEnv *env);
BOOL s004B_MsgWaitAdvance(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b10f4(FinishScriptSubEventsWork *work, u32 *state);
BOOL func_ov021_021b1234(FinishScriptSubEventsWork *work, u32 *state);
BOOL func_ov021_021b1334(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021b13ec(FinishScriptSubEventsWork *work, u32 *state);
BOOL func_ov021_021b14e8(FinishScriptSubEventsWork *work, u32 *state);

#endif // BWPORT_FIELD_SCRCMD_MSG_H
