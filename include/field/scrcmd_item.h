// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_item.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_FIELD_SCRCMD_ITEM_H
#define BWPORT_FIELD_SCRCMD_ITEM_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

BOOL s00B5_ItemAdd(VM *vm, FieldScriptEnv *env);
BOOL s00B9_ItemGetCount(VM *vm, FieldScriptEnv *env);
BOOL s00BA_ItemIsTMHM(VM *vm, FieldScriptEnv *env);
BOOL s00BB_ItemGetPocket(VM *vm, FieldScriptEnv *env);
BOOL s00BD_ItemGetClass(VM *vm, FieldScriptEnv *env);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
BOOL s00B6_ItemSub(VM *vm, FieldScriptEnv *env);
BOOL s00B7_ItemCheckSpace(VM *vm, FieldScriptEnv *env);
BOOL s00B8_ItemCheckAmount(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_ITEM_H
