#include "global.h"

#include "follow_mon.h"
#include "map_header.h"
#include "message_format.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "script_pokemon_util.h"

typedef struct UnkStruct_ov02_02250504 {
    u8 unk_0[0xC];
    SaveData *unk_C;
    u8 unk_10[0xF8];
    FieldSystemUnkSub108 *unk_108;
    u8 unk_10C[0x14];
    void *unk_120;
} UnkStruct_ov02_02250504;

void FollowMon_PlaceholdersSet(FieldSystem *arg0, MessageFormat *arg1);

void FollowMon_PlaceholdersSet(FieldSystem *arg0, MessageFormat *messageFormat) {
    BoxPokemon *boxmon;
    Pokemon *mon;

    mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(arg0->saveData));
    boxmon = Mon_GetBoxMon(mon);
    BufferBoxMonNickname(messageFormat, 0U, boxmon);
    BufferBoxMonSpeciesName(messageFormat, 1U, boxmon);
    BufferPlayersName(messageFormat, 2U, Save_PlayerData_GetProfile(arg0->saveData));
    BufferLocationName(messageFormat, 3U, MapHeader_GetMapSec(**((u32 * *)&arg0->location)));
    BufferItemName(messageFormat, 4U, GetMonData(mon, 6, NULL));
}

void ov02_02250504(UnkStruct_ov02_02250504 *arg0);

void ov02_02250504(UnkStruct_ov02_02250504 *arg0) {
    s32 v0;
    Pokemon *mon;
    s32 v1;
    s32 v3;
    s32 v4;
    u32 v2;

    mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(arg0->unk_C));
    v1 = (s32)FieldSystem_UnkSub108_GetMonMood(arg0->unk_108);
    v0 = v1;
    v4 = v1 + *(s8 *)((u8 *)(arg0->unk_120) + 0x815);
    v0 = v4;
    if (v4 > 0x7F) {
        v0 = 0x7F;
    } else if (v4 < -0x7F) {
        v0 = -0x7F;
    }
    FieldSystem_UnkSub108_SetMonMood(arg0->unk_108, (s8)v0);
    v2 = GetMonData(mon, 9, NULL);
    v0 = v2;
    v3 = v2 + *(s8 *)((u8 *)(arg0->unk_120) + 0x814);
    v0 = v3;
    if (v3 > 0xFF) {
        v0 = 0xFF;
    } else if (v3 < 0) {
        v0 = 0;
    }
    SetMonData(mon, 9, &v0);
}
