#include "global.h"

#include "field_system.h"
#include "follow_mon.h"
#include "map_header.h"
#include "message_format.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "script_pokemon_util.h"

void FollowMon_PlaceholdersSet(FieldSystem *arg0, MessageFormat *arg1);

void FollowMon_PlaceholdersSet(FieldSystem *arg0, MessageFormat *messageFormat) {
    BoxPokemon *boxmon;
    Pokemon *mon;

    mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(arg0->saveData));
    boxmon = Mon_GetBoxMon(mon);
    BufferBoxMonNickname(messageFormat, 0U, boxmon);
    BufferBoxMonSpeciesName(messageFormat, 1U, boxmon);
    BufferPlayersName(messageFormat, 2U, Save_PlayerData_GetProfile(arg0->saveData));
    BufferLocationName(messageFormat, 3U, MapHeader_GetMapSec(arg0->location->mapId));
    BufferItemName(messageFormat, 4U, GetMonData(mon, 6, NULL));
}

void ov02_02250504(FieldSystem *arg0);

void ov02_02250504(FieldSystem *arg0) {
    s32 v0;
    Pokemon *mon;
    s32 v1;
    s32 v3;
    s32 v4;
    u32 v2;

    mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(arg0->saveData));
    v1 = (s32)FieldSystem_UnkSub108_GetMonMood(arg0->unk108);
    v0 = v1;
    v4 = v1 + arg0->unk120->unk815;
    v0 = v4;
    if (v4 > 0x7F) {
        v0 = 0x7F;
    } else if (v4 < -0x7F) {
        v0 = -0x7F;
    }
    FieldSystem_UnkSub108_SetMonMood(arg0->unk108, (s8)v0);
    v2 = GetMonData(mon, 9, NULL);
    v0 = v2;
    v3 = v2 + arg0->unk120->unk814;
    v0 = v3;
    if (v3 > 0xFF) {
        v0 = 0xFF;
    } else if (v3 < 0) {
        v0 = 0;
    }
    SetMonData(mon, 9, &v0);
}
