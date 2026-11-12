// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_sound.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SCRCMD_SOUND_H
#define BWPORT_FIELD_SCRCMD_SOUND_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "field/field_script.h"
#include "struct_decls.h"

BOOL s0098_BGMPlay(VM *vm, FieldScriptEnv *env);
BOOL s0235_BGMPlayEx(VM *vm, FieldScriptEnv *env);
BOOL s0245_BGMFadeOut(VM *vm, FieldScriptEnv *env);
BOOL s009B_BGMIsPlaying(VM *vm, FieldScriptEnv *env);
BOOL s009E_BGMChangeMap(VM *vm, FieldScriptEnv *env);
BOOL s0238_BGMChangeMapEx(VM *vm, FieldScriptEnv *env);
BOOL s009F_BGMPlayPush(VM *vm, FieldScriptEnv *env);
BOOL s00A1_BGMPush(VM *vm, FieldScriptEnv *env);
BOOL s00A2_BGMPop(VM *vm, FieldScriptEnv *env);
BOOL FieldScriptSubEventFinish_MEPlayback(FinishScriptSubEventsWork *work, u32 *state);
BOOL s024C_BGMAmbienceResume(VM *vm, FieldScriptEnv *env);
BOOL FieldScriptSubEventFinish_BGMPlayback(FinishScriptSubEventsWork *work, u32 *state);
BOOL s00A6_SEPlay(VM *vm, FieldScriptEnv *env);
BOOL s00AB_PVPlay(VM *vm, FieldScriptEnv *env);
BOOL s00A3_ISSSwitchEnable(VM *vm, FieldScriptEnv *env);
BOOL s00A5_ISSSwitchQuery(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021aef10(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021aef60(VM *vm, FieldScriptEnv *env);
BOOL func_ov021_021aefa4(FinishScriptSubEventsWork *work, u32 *state);
// The same as s00AB_PVPlay, with the save's Chatot recording
BOOL func_ov021_021af3d8(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s00A0_BGMWait(VM *vm, FieldScriptEnv *env);
BOOL s00A7_SEStop(VM *vm, FieldScriptEnv *env);
BOOL s00A8_SEWait(VM *vm, FieldScriptEnv *env);
BOOL s00AA_MEWait(VM *vm, FieldScriptEnv *env);
BOOL s00AC_PVWait(VM *vm, FieldScriptEnv *env);
BOOL s00A4_ISSSwitchDisable(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_SOUND_H
