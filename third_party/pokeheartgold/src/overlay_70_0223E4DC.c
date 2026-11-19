#include "global.h"

#include "party.h"
#include "pokemon.h"

struct PCBoxes;

typedef struct PCBoxes PCBoxes;

BOOL ov70_0223E490(int boxID);

BOOL ov70_0223E4DC(Party *party, PCBoxes *sLotoId, int boxID, int unused2);

extern const u16 ov70_02245700[10];
int ov70_0223E4FC(BoxPokemon *boxMon);

BOOL ov70_0223E4DC(Party *party, PCBoxes *sLotoId, int boxID, int unused2) {
    if (ov70_0223E490(boxID)) {
        if (Party_GetCount(party) < 2) { // BUG? includes eggs, so a party of [valid mon, egg] passes the < 2 guard. The last non-egg PokÃ©mon can be deposited, leaving only an egg.
            return FALSE;
        }
    }

    return TRUE;
}

int ov70_0223E4FC(BoxPokemon *boxMon) {
    int count = 0;
    int reencrypt = AcquireBoxMonLock(boxMon);

    for (int i = 0; i < ((int)(sizeof(ov70_02245700) / sizeof(*(ov70_02245700)))); i++) {
        count += GetBoxMonData(boxMon, ov70_02245700[i], 0L);
    }

    ReleaseBoxMonLock(boxMon, reencrypt);

    if (count) {
        return 1;
    }

    return 0;
}
