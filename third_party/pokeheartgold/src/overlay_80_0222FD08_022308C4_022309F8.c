#include "global.h"

#include "game_stats.h"
#include "overlay_80_0222FD08.h"
#include "party.h"
#include "overlay_80_battle_factory.h"

void ov80_02230AE4(BattleFactory *factory);

void ov80_022309F8(BattleFactory *factory) {
    if (factory->selectedAppSlots[0] != 0xFF) {
        Party_SafeCopyMonToSlot_ResetAprijuiceModifiers(factory->playersParty, (s32)factory->selectedAppSlots[0], Party_GetMonByIndex(factory->opponentsParty, (s32)factory->selectedAppSlots[1]));
        factory->playerMonSetIDs[factory->selectedAppSlots[0]] = factory->initialRentalSetIDs[factory->selectedAppSlots[1]];
        ov80_02230AE4(factory);
        GameStats_Inc(Save_GameStats_Get(factory->saveData), 0x41);
    }
}
