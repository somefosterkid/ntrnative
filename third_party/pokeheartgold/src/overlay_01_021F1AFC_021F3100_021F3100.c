#include "global.h"

#include "field_system.h"
#include "party.h"
#include "pokemon_types_def.h"

Pokemon *ov01_021F3100(FieldSystem *arg0, s32 arg1);

Pokemon *ov01_021F3100(FieldSystem *arg0, s32 slot) {
    return Party_GetMonByIndex(SaveArray_Party_Get(arg0->saveData), slot);
}
