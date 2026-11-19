#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "script_pokemon_util.h"

s32 ov02_02250780(FieldSystem *fieldSystem, u8 a1);

s32 ov02_02250780(FieldSystem *fieldSystem, u8 a1) {
    Pokemon *mon;
    u32 v0;
    u32 v1;

    mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
    v1 = GetMonData(mon, 0xB1, NULL);
    v0 = GetMonData(mon, 0xB2, NULL);
    if ((v1 == a1) || (v0 == a1)) {
        return 1;
    }
    return 0;
}
