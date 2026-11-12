// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_menu.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_MENU_H
#define BWPORT_FIELD_SCRCMD_MENU_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s0047_YesNoWin(VM *vm, FieldScriptEnv *env);
BOOL s00AF_ListMenuAdd(VM *vm, FieldScriptEnv *env);
BOOL s00B0_ListMenuShow(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s00AD_ListMenuInitCommon(VM *vm, FieldScriptEnv *env);
BOOL s00AE_ListMenu_AnchorTopLeft(VM *vm, FieldScriptEnv *env);
BOOL s00B2_ListMenu_AnchorTopRight(VM *vm, FieldScriptEnv *env);
BOOL s00B1_ListMenuShow2(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_MENU_H
