#include "global.h"

#include "overlay_80_022340E8.h"
#include "party.h"
#include "pokemon.h"

void ov80_02235364(Party *party, Party *playerParty, s32 a2, s32 a3) {
    s16 ptr;

    ptr = (s16)GetMonData(Party_GetMonByIndex(party, a2), 6, NULL);
    SetMonData(Party_GetMonByIndex(playerParty, a3), 6, &ptr);
}
