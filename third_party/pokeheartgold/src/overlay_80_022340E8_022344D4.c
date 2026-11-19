#include "global.h"

#include "overlay_80_022340E8.h"
#include "party.h"
#include "pokemon.h"
#include "overlay_80_battle_arcade.h"

u32 sub_02030F34(void *, u8, u8, u8, void *);

void ov80_022344D4(struct BattleArcade *arcade) {
    s16 ptr;
    Pokemon *mon;
    s32 slot;
    u8 v0;

    v0 = (u8)Party_GetCount(arcade->playersParty);
    slot = 0;
    if ((s32)v0 > 0) {
        do {
            mon = Party_GetMonByIndex(arcade->playersParty, slot);
            ptr = sub_02030F34(arcade->arcadeSave, 4, (u8)slot, 0, 0);
            SetMonData(mon, 6, &ptr);
            slot += 1;
        } while (slot < (s32)v0);
    }
}
