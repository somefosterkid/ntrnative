// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_fade.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_FADE_H
#define BWPORT_FIELD_SCRCMD_FADE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s00B3_FadeEx(VM *vm, FieldScriptEnv *env);
BOOL s01A8_FadeInBlackQ_(VM *vm, FieldScriptEnv *env);
BOOL s01A4_FadeOutBlackQ(VM *vm, FieldScriptEnv *env);
BOOL s01A5_FadeInWhiteQ(VM *vm, FieldScriptEnv *env);
BOOL s01A6_FadeOutWhiteQ(VM *vm, FieldScriptEnv *env);
BOOL s01AB_FadeInBlack(VM *vm, FieldScriptEnv *env);
BOOL s01AC_FadeOutBlack(VM *vm, FieldScriptEnv *env);
BOOL s01AD_FadeInWhite(VM *vm, FieldScriptEnv *env);
BOOL s01AE_FadeOutWhite(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s01AA_CallSeasonBanner(VM *vm, FieldScriptEnv *env);
BOOL s01A3_FadeInBlackQ(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_FADE_H
