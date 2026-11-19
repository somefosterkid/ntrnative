#include "global.h"

#include "field_roamer.h"
#include "field_system.h"
#include "field_types_def.h"
#include "roamer.h"
#include "unk_0206793C.h"

void sub_02067BE8(FieldSystem *fieldSystem) {
    Save_RandomizeRoamersLocation(Save_Roamers_Get(fieldSystem->saveData));
}
