// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_place_name.c, which is the same
// code in Black 2/White 2's overlay 36.
// The script command that shows the place name banner. The ROM has no name for the file; scrcmd_place_name.c is
// descriptive, and the command could also end the build model commands before it. Function name from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/scrcmd_place_name.h"
#include "system/game_system.h"

BOOL s023F_CallPlaceNameDisp(VM *vm, FieldScriptEnv *env) {
    Field *field = GSYS_GetField(FieldScriptEnv_GetGameSystem(env));
    PlaceName *placeName = Field_GetPlaceName(field);

    BeginForcePlaceNameDisp(placeName, Field_GetPlayerStateZoneID(field));
    return FALSE;
}
