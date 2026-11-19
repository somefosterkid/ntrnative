#include "global.h"

#include "save.h"
#include "unk_0205BB1C.h"
#include "pokemon_types_def.h"

struct Party;
struct Pokemon;
int Party_GetCount(const Party *party);
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
Party *SaveArray_Party_Get(SaveData *saveData);
u32 GetMonData(Pokemon *mon, int param, void *dest);

u16 Save_GetPartyLead(SaveData *saveData) {
    u16 i, partyCount = Party_GetCount(SaveArray_Party_Get(saveData));

    for (i = 0; i < partyCount; i++) {
        Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(saveData), i);

        if (GetMonData(mon, MON_DATA_IS_EGG, 0L) == 0) {
            return i;
        }
    }

    return 0;
}
