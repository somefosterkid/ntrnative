// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/bsubway_savedata.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/bsubway_save.h"
#include "field/bsubway_scr.h"
#include "gfl/heap.h"
#include "gfl/std.h"

// The Battle Subway's three save blocks: the play saved in the middle of (0x38), the scores with the Battle Points
// (0x39) and the Wi-Fi train's downloaded data (0x3a). The file's name is the ROM's, which func_0200df58 passes to
// the heap.

static u32 func_0200dac0(u32 mode);
static void func_0200dd8c(BSubwayWifiData *data);
static BOOL func_0200dda8(const RTCDate *date1, const RTCDate *date2);

// pokebw2's BSubwayPlayData_GetSize
u32 func_0200d858(void) {
    return sizeof(BSubwayPlayData);
}

// pokebw2's BSubwayPlayData_Init
void func_0200d85c(BSubwayPlayData *data) {
    MI_CpuFill8(data, 0, sizeof(BSubwayPlayData));
}

// pokebw2's BSubwayPlayData_SetSaveFlag
void func_0200d868(BSubwayPlayData *data, u32 value) {
    data->saved = value;
}

// pokebw2's BSubwayPlayData_GetSaveFlag
u32 func_0200d87c(BSubwayPlayData *data) {
    return data->saved;
}

// pokebw2's BSubwayPlayData_GetData
u32 func_0200d884(BSubwayPlayData *data, u32 id, void *buffer) {
    switch (id) {
    case BSUBWAY_PLAY_MODE:
        return data->playMode;
    case BSUBWAY_PLAY_ROUND:
        return data->round;
    case BSUBWAY_PLAY_WIFI_DOWN:
        return data->wifiDown;
    case BSUBWAY_PLAY_WIFI_TURN:
        return data->wifiTurn;
    case BSUBWAY_PLAY_WIFI_DAMAGE:
        return data->wifiDamage;
    case BSUBWAY_PLAY_UNK8:
        MI_CpuCopy8(data->unk8, buffer, sizeof(data->unk8));
        return 0;
    case BSUBWAY_PLAY_UNK28:
        MI_CpuCopy8(data->unk28, buffer, sizeof(data->unk28));
        return 0;
    case BSUBWAY_PLAY_UNK0_0:
        return data->unk0_0;
    case BSUBWAY_PLAY_UNKC:
        MI_CpuCopy8(data->unkC, buffer, sizeof(data->unkC));
        return 0;
    case BSUBWAY_PLAY_UNK0_6:
        return data->unk0_6;
    case BSUBWAY_PLAY_UNK0_9:
        return data->unk0_9;
    case BSUBWAY_PLAY_UNK0_10:
        return data->unk0_10;
    }
    return 0;
}

// pokebw2's BSubwayPlayData_SetData
void func_0200d914(BSubwayPlayData *data, u32 id, const void *value) {
    switch (id) {
    case BSUBWAY_PLAY_MODE:
        data->playMode = *(const u8 *)value;
        break;
    case BSUBWAY_PLAY_ROUND:
        data->round = *(const u8 *)value;
        break;
    case BSUBWAY_PLAY_WIFI_DOWN:
        data->wifiDown = *(const u8 *)value;
        break;
    case BSUBWAY_PLAY_WIFI_TURN:
        data->wifiTurn = *(const u16 *)value;
        break;
    case BSUBWAY_PLAY_WIFI_DAMAGE:
        data->wifiDamage = *(const u16 *)value;
        break;
    case BSUBWAY_PLAY_UNK8:
        MI_CpuCopy8(value, data->unk8, sizeof(data->unk8));
        break;
    case BSUBWAY_PLAY_UNK28:
        MI_CpuCopy8(value, data->unk28, sizeof(data->unk28));
        break;
    case BSUBWAY_PLAY_UNK0_0:
        data->unk0_0 = *(const u8 *)value;
        break;
    case BSUBWAY_PLAY_UNKC:
        MI_CpuCopy8(value, data->unkC, sizeof(data->unkC));
        break;
    case BSUBWAY_PLAY_UNK0_6:
        data->unk0_6 = *(const u8 *)value;
        break;
    case BSUBWAY_PLAY_UNK0_9:
        data->unk0_9 = *(const u8 *)value;
        break;
    case BSUBWAY_PLAY_UNK0_10:
        data->unk0_10 = *(const u8 *)value;
        break;
    }
}

// pokebw2's BSubwayPlayData_AddWifiRecord
void func_0200d9e8(BSubwayPlayData *data, u8 down, u8 turn, u16 damage) {
    if (data->wifiDown + down < 0xff) {
        data->wifiDown += down;
    }
    if (data->wifiTurn + turn < 0xffff) {
        data->wifiTurn += turn;
    }
    if (data->wifiDamage + damage < 0xffff) {
        data->wifiDamage += damage;
    }
}

// pokebw2's BSubwayPlayData_ResetRound
void func_0200da14(BSubwayPlayData *data) {
    u8 round = 0;

    func_0200d914(data, BSUBWAY_PLAY_ROUND, &round);
}

// pokebw2's BSubwayPlayData_IncRound
void func_0200da28(BSubwayPlayData *data) {
    u8 round = func_0200d884(data, BSUBWAY_PLAY_ROUND, NULL);

    if (round < 0xff) {
        round++;
    }
    func_0200d914(data, BSUBWAY_PLAY_ROUND, &round);
}

// pokebw2's BSubwayPlayData_GetRound
u16 func_0200da54(BSubwayPlayData *data) {
    return func_0200d884(data, BSUBWAY_PLAY_ROUND, NULL);
}

// pokebw2's BSubwayScoreData_GetSize
u32 func_0200da64(void) {
    return sizeof(BSubwayScoreData);
}

// pokebw2's BSubwayScoreData_Init
void func_0200da6c(BSubwayScoreData *score) {
    MI_CpuFill8(score, 0, sizeof(BSubwayScoreData));
    score->wifiRank = 1;
}

// pokebw2's BSubwayScoreData_AddBattlePoints
void func_0200da80(BSubwayScoreData *score, u16 amount) {
    if (score->battlePoints + amount > BSUBWAY_BATTLE_POINTS_MAX) {
        score->battlePoints = BSUBWAY_BATTLE_POINTS_MAX;
    } else {
        score->battlePoints += amount;
    }
}

// pokebw2's BSubwayScoreData_SubBattlePoints
void func_0200da98(BSubwayScoreData *score, u16 amount) {
    s16 rest = (s16)score->battlePoints - (s16)amount;

    if (rest >= 0) {
        score->battlePoints -= amount;
    } else {
        score->battlePoints = 0;
    }
}

// pokebw2's BSubwayScoreData_GetBattlePoints
u16 func_0200dabc(BSubwayScoreData *score) {
    return score->battlePoints;
}

// The index of a play mode's scores
// pokebw2's BSubwayScoreData_GetModeIndex
static u32 func_0200dac0(u32 mode) {
    return mode;
}

// pokebw2's BSubwayScoreData_GetStreak
u16 func_0200dac4(BSubwayScoreData *score, u32 mode) {
    return score->streaks[func_0200dac0(mode)];
}

// pokebw2's BSubwayScoreData_GetMaxStreak
u16 func_0200dad8(BSubwayScoreData *score, u32 mode) {
    return score->maxStreaks[func_0200dac0(mode)];
}

// pokebw2's BSubwayScoreData_UpdateMaxStreak
void func_0200daec(BSubwayScoreData *score, u32 mode, u32 value) {
    u32 index = func_0200dac0(mode);

    if (score->maxStreaks[index] < value) {
        score->maxStreaks[index] = value;
    }
}

// pokebw2's BSubwayScoreData_SetStreak
void func_0200db08(BSubwayScoreData *score, u32 mode, u32 value) {
    score->streaks[func_0200dac0(mode)] = value;
}

// pokebw2's BSubwayScoreData_ResetStage
void func_0200db1c(BSubwayScoreData *score, u32 mode) {
    score->stages[func_0200dac0(mode)] = 0;
}

// pokebw2's BSubwayScoreData_StartStage
void func_0200db30(BSubwayScoreData *score, u32 mode) {
    score->stages[func_0200dac0(mode)] = 1;
}

// pokebw2's BSubwayScoreData_HasStage
BOOL func_0200db44(BSubwayScoreData *score, u32 mode) {
    if (score->stages[func_0200dac0(mode)] != 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BSubwayScoreData_IncStage
void func_0200db60(BSubwayScoreData *score, u32 mode) {
    u32 index = func_0200dac0(mode);

    if (score->stages[index] < 0xfffe) {
        score->stages[index]++;
    }
}

// pokebw2's BSubwayScoreData_GetStage
u16 func_0200db80(BSubwayScoreData *score, u32 mode) {
    u16 stage = score->stages[func_0200dac0(mode)] - 1;

    if ((s16)stage < 0) {
        stage = 0;
    }
    return stage;
}

// pokebw2's BSubwayScoreData_SetFlag
BOOL func_0200dba0(BSubwayScoreData *score, int id, u32 op) {
    u32 i;
    u32 flag = 1;

    if (id >= 11) {
        return FALSE;
    }
    for (i = 0; i < id; i++) {
        flag <<= 1;
    }
    switch (op) {
    case BSUBWAY_FLAG_RESET:
        score->flags &= flag ^ 0xffffffff;
        break;
    case BSUBWAY_FLAG_SET:
        score->flags |= flag;
        break;
    case BSUBWAY_FLAG_GET:
        return (score->flags >> id) & 1;
    }
    return FALSE;
}

// pokebw2's BSubwayScoreData_IncWifiRank
void func_0200dbf0(BSubwayScoreData *score) {
    if (score->wifiRank < BSUBWAY_WIFI_RANK_MAX) {
        score->wifiRank++;
    }
}

// pokebw2's BSubwayScoreData_DecWifiRank
void func_0200dbfc(BSubwayScoreData *score) {
    if (score->wifiRank > 1) {
        score->wifiRank--;
    }
}

// pokebw2's BSubwayScoreData_GetWifiRank
u32 func_0200dc08(BSubwayScoreData *score) {
    return score->wifiRank;
}

// pokebw2's BSubwayScoreData_SetWifiLoseCount
u32 func_0200dc0c(BSubwayScoreData *score, u32 op) {
    switch (op) {
    case BSUBWAY_FLAG_RESET:
        func_0200dba0(score, 0, BSUBWAY_FLAG_RESET);
        score->wifiLoseCount = 0;
        break;
    case BSUBWAY_FLAG_INC:
        if (func_0200dba0(score, 0, BSUBWAY_FLAG_GET)) {
            if (score->wifiLoseCount < 0xff) {
                score->wifiLoseCount++;
            }
        } else {
            func_0200dba0(score, 0, BSUBWAY_FLAG_SET);
            score->wifiLoseCount = 1;
        }
        break;
    }
    return score->wifiLoseCount;
}

// pokebw2's BSubwayScoreData_SetTeam
void func_0200dc50(BSubwayScoreData *score, u32 flag, const void *team) {
    if (flag == 0) {
        MI_CpuCopy8(team, score->team0, sizeof(score->team0));
    } else {
        MI_CpuCopy8(team, score->team1, sizeof(score->team1));
    }
}

// pokebw2's BSubwayScoreData_GetTeam
void func_0200dc74(BSubwayScoreData *score, u32 flag, void *team) {
    if (flag == 0) {
        MI_CpuCopy8(score->team0, team, sizeof(score->team0));
    } else {
        MI_CpuCopy8(score->team1, team, sizeof(score->team1));
    }
}

// pokebw2's BSubwayScoreData_CalcWifiScore
void func_0200dc94(BSubwayScoreData *score, BSubwayPlayData *play) {
    u16 roundScore, recordScore, damageScore;
    u16 turnScore, downScore;

    roundScore = (play->round - 1) * 1000;
    turnScore = play->wifiTurn * 10;
    downScore = play->wifiDown * 20;
    if (turnScore + downScore > 950) {
        recordScore = 0;
    } else {
        recordScore = 950 - (turnScore + downScore);
    }
    if (play->wifiDamage > 970) {
        damageScore = 0;
    } else {
        damageScore = (1000 - play->wifiDamage) / 30;
    }
    score->wifiScore = roundScore + recordScore + damageScore;
}

// pokebw2's BSubwayScoreData_GetWifiScore
u16 func_0200dcf8(BSubwayScoreData *score) {
    return score->wifiScore;
}

// pokebw2's BSubwayScoreData_GetWifiScoreRounds
u8 func_0200dcfc(BSubwayScoreData *score) {
    return score->wifiScore / 1000;
}

// pokebw2's BSubwayWifiData_GetSize
u32 func_0200dd10(void) {
    return sizeof(BSubwayWifiData);
}

// pokebw2's BSubwayWifiData_Init
void func_0200dd18(BSubwayWifiData *data) {
    MI_CpuFill8(data, 0, sizeof(BSubwayWifiData));
}

// pokebw2's BSubwayWifiData_SetRoomFlag
void func_0200dd28(BSubwayWifiData *data, u8 rank, u8 room, const RTCDate *date) {
    u16 id;
    u8 offset, bit;
    u8 flag = 1;

    if (room == 0 || room > BSUBWAY_WIFI_ROOM_MAX) {
        return;
    }
    if (rank == 0 || rank > BSUBWAY_WIFI_RANK_MAX) {
        return;
    }
    id = (room - 1) + (rank - 1) * BSUBWAY_WIFI_ROOM_MAX;
    offset = id / 8;
    bit = id % 8;
    flag <<= bit;
    data->roomFlags[offset] |= flag;
    data->date = (date->year << 24) | ((date->month & 0xff) << 16) | ((date->day & 0xff) << 8) | date->week;
}

// pokebw2's BSubwayWifiData_ClearRoomFlags
static void func_0200dd8c(BSubwayWifiData *data) {
    MI_CpuFill8(data->roomFlags, 0, sizeof(data->roomFlags));
    MI_CpuFill8(&data->date, 0, sizeof(data->date));
}

// Whether a date is after another, by its year, its month or its day
// pokebw2's BSubwayWifiData_IsDateAfter
static BOOL func_0200dda8(const RTCDate *date1, const RTCDate *date2) {
    if (date1->year > date2->year) {
        return TRUE;
    }
    if (date1->month > date2->month) {
        return TRUE;
    }
    if (date1->day > date2->day) {
        return TRUE;
    }
    return FALSE;
}

// Whether the room was cleared today. Clears every room's flag when the day has changed
// pokebw2's BSubwayWifiData_GetRoomFlag
BOOL func_0200ddd0(BSubwayWifiData *data, u8 rank, u8 room, const RTCDate *today) {
    u16 id;
    u8 offset, bit;
    u8 flag = 1;
    RTCDate date;

    if (room > BSUBWAY_WIFI_ROOM_MAX || rank > BSUBWAY_WIFI_RANK_MAX) {
        return FALSE;
    }
    date.year = (data->date >> 24) & 0xff;
    date.month = (data->date >> 16) & 0xff;
    date.day = (data->date >> 8) & 0xff;
    date.week = data->date & 0xff;
    if (func_0200dda8(today, &date)) {
        func_0200dd8c(data);
        return FALSE;
    }
    id = (room - 1) + (rank - 1) * BSUBWAY_WIFI_ROOM_MAX;
    offset = id / 8;
    bit = id % 8;
    flag <<= bit;
    if (data->roomFlags[offset] & flag) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BSubwayWifiData_HasPlayers
u16 func_0200de5c(BSubwayWifiData *data) {
    return data->hasPlayers;
}

// pokebw2's BSubwayWifiData_HasLeaders
u16 func_0200de64(BSubwayWifiData *data) {
    return data->hasLeaders;
}

// pokebw2's BSubwayWifiData_SetPlayers
void func_0200de6c(BSubwayWifiData *data, const void *players, u8 rank, u8 room) {
    MI_CpuCopy8(players, data->players, sizeof(data->players));
    data->playerRank = rank;
    data->playerRoom = room;
    data->hasPlayers = TRUE;
}

// pokebw2's BSubwayWifiData_GetPlayerRank
u16 func_0200de94(BSubwayWifiData *data) {
    return data->playerRank;
}

// pokebw2's BSubwayWifiData_GetPlayerRoom
u16 func_0200dea0(BSubwayWifiData *data) {
    return data->playerRoom;
}

// pokebw2's BSubwayWifiData_GetTrainer
void func_0200dea8(BSubwayWifiData *data, BSubwayTrainer *trainer, u8 round, u16 id) {
    BSubwayWifiPlayer *player = &data->players[round];

    trainer->unk00 = 0xffff;
    trainer->trainerId = player->trainerClass;
    MI_CpuCopy8(player->name, trainer->name, sizeof(player->name));
    MI_CpuCopy8(player->message, &trainer->message, sizeof(player->message));
    MI_CpuCopy8(player->winWords, &trainer->winWords, sizeof(player->winWords));
    MI_CpuCopy8(player->loseWords, &trainer->loseWords, sizeof(player->loseWords));
    MI_CpuCopy8(player->pokemon, trainer->pokemon, sizeof(player->pokemon));
}

// pokebw2's BSubwayWifiData_SetLeaders
void func_0200df10(BSubwayWifiData *data, const void *leaders, u8 rank, u8 room) {
    MI_CpuCopy8(leaders, data->leaders, sizeof(data->leaders));
    data->leaderRank = rank;
    data->leaderRoom = room;
    data->hasLeaders = TRUE;
}

// pokebw2's BSubwayWifiData_GetLeaderRank
u16 func_0200df40(BSubwayWifiData *data) {
    return data->leaderRank;
}

// pokebw2's BSubwayWifiData_GetLeaderRoom
u16 func_0200df4c(BSubwayWifiData *data) {
    return data->leaderRoom;
}

// pokebw2's BSubwayWifiData_AllocLeaders
void *func_0200df58(BSubwayWifiData *data, HeapID heapId) {
    BSubwayWifiLeader *leaders = GFL_HeapAllocate(heapId, sizeof(data->leaders), TRUE, "bsubway_savedata.c", 1288);

    MI_CpuCopy8(data->leaders, leaders, sizeof(data->leaders));
    return leaders;
}

// How many leaders a list has
// pokebw2's BSubwayWifiLeader_CountList
int func_0200df94(void *list) {
    BSubwayWifiLeader *leaders = list;
    int i;
    int count = 0;

    for (i = 0; i < BSUBWAY_WIFI_LEADER_MAX; i++) {
        if (leaders[i].name[0] != 0) {
            count++;
        }
    }
    return count;
}

// pokebw2's BSubwayWifiLeader_GetID
u32 func_0200dfb4(void *entry) {
    BSubwayWifiLeader *leader = entry;

    return leader->id[0] | (leader->id[1] << 8) | (leader->id[2] << 16) | (leader->id[3] << 24);
}
