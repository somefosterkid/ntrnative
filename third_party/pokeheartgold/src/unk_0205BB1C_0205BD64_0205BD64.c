#include "global.h"

#include "party.h"
#include "pokemon.h"

u16 Save_GetPartyLeadAlive(SaveData *saveData);

u16 Save_GetPartyLeadAlive(SaveData *saveData) {
    Pokemon *temp_r0;
    u16 var_r5;
    u16 temp_r6;
    temp_r6 = (u16)Party_GetCount(SaveArray_Party_Get(saveData));
    var_r5 = 0;
    if (temp_r6 != 0) {
        do {
            temp_r0 = Party_GetMonByIndex(SaveArray_Party_Get(saveData), var_r5);

            if ((GetMonData(temp_r0, 0x4C, 0L) == 0) && (GetMonData(temp_r0, 0xA3, 0L) != 0)) {
                return (u16)var_r5;
            }
            var_r5 = (u16)(var_r5 + 1);
        } while (var_r5 < temp_r6);
    }
    return 0U;
}
