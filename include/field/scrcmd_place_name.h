// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/scrcmd_place_name.h.
#ifndef BWPORT_FIELD_SCRCMD_PLACE_NAME_H
#define BWPORT_FIELD_SCRCMD_PLACE_NAME_H

// Overlay 21's scrcmd_place_name.c: the script command that shows the place name banner. Name from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0), whose 0x23F is the command's number in Black 2; in Black it
// is entry 0x252 of the command table, data_ov010_0217062c

#include "types.h"
#include "struct_decls.h"

BOOL s023F_CallPlaceNameDisp(VM *vm, FieldScriptEnv *env);

#endif // BWPORT_FIELD_SCRCMD_PLACE_NAME_H
