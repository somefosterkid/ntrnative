#include "global.h"

#include "field_system.h"
#include "party.h"
#include "script_pokemon_util.h"

void ov01_022062CC(FieldSystem *arg0);

void ov01_022062CC(FieldSystem *arg0) {
    GetIdxOfFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(arg0->saveData));
}
