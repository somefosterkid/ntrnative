// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_camera.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_CAMERA_H
#define BWPORT_FIELD_SCRCMD_CAMERA_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "field/field_script.h"
#include "struct_decls.h"

BOOL s013F_EvCameraInit(VM *vm, FieldScriptEnv *env);
BOOL s0140_EvCameraEnd(VM *vm, FieldScriptEnv *env);
BOOL s0143_EvCameraMoveTo(VM *vm, FieldScriptEnv *env);
BOOL s0144_EvCameraReturn(VM *vm, FieldScriptEnv *env);
BOOL s0147_EvCameraMoveToDefault(VM *vm, FieldScriptEnv *env);
BOOL s0145_EvCameraWait(VM *vm, FieldScriptEnv *env);
// FIELD_SCRIPT_SUB_EVENT_FINISH_FUNCS's entry for the event camera
BOOL FieldScriptSubEventFinish_EvCamera(FinishScriptSubEventsWork *work, u32 *state);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s0141_EvCameraUnbind(VM *vm, FieldScriptEnv *env);
BOOL s0142_EvCameraRebind(VM *vm, FieldScriptEnv *env);
BOOL s0146_EvCameraMoveToCommon(VM *vm, FieldScriptEnv *env);
BOOL s0148_EvCameraShake(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_CAMERA_H
