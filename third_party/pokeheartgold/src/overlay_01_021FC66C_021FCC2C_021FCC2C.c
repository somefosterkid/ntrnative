#include "global.h"

#include "follow_mon.h"
#include "math_util.h"
#include "overlay_01_021FC66C.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "overlay_01_fishing_task.h"

static s32 ov01_021FCC74(u32 arg0);

s32 ov01_021FCC2C(void *arg0) {
    if (PlayerAvatar_GetState((((FishingTask *)arg0)->fieldSystem)->playerAvatar) == 2) {
        return 0;
    }
    if ((FollowMon_IsActive(((FishingTask *)arg0)->fieldSystem) != 0) && (ov01_021FCC74((u8)GetMonData(GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get((((FishingTask *)arg0)->fieldSystem)->saveData)), 9, NULL)) != 0)) {
        return 1;
    }
    return 0;
}

static s32 ov01_021FCC74(u32 arg0) {
    s32 v0;

    if (arg0 <= 0x63U) {
        return 0;
    }
    if (arg0 <= 0x95U) {
        v0 = 0x14;
    } else if (arg0 <= 0xC7U) {
        v0 = 0x1E;
    } else if (arg0 <= 0xF9U) {
        v0 = 0x28;
    } else {
        v0 = 0x32;
    }
    if ((s32)((s32)LCRandom() % 100) < v0) {
        return 1;
    }
    return 0;
}
