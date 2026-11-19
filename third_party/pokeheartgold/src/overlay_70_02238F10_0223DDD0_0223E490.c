#include "global.h"

#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"

s32 ov70_0223E490(s32 arg0);

s32 ov70_0223E490(s32 arg0) {
    if (arg0 == 0x12) {
        return 1;
    }
    return 0;
}

BoxPokemon *ov70_0223E49C(Party *arg0, PCStorage *arg1, u32 arg2, s32 arg3);

BoxPokemon *ov70_0223E49C(Party *party, PCStorage *storage, u32 boxno, s32 arg3) {
    if (ov70_0223E490(boxno) != 0) {
        if (arg3 > (s32)(Party_GetCount(party) - 1)) {
            return NULL;
        }
        return Mon_GetBoxMon(Party_GetMonByIndex(party, arg3));
    }
    return PCStorage_GetMonByIndexPair(storage, boxno, (u32)arg3);
}
