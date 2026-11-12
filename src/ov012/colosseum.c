// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov028/colosseum.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Union Room's colosseum (the file's name is from its assert strings): up to four players trade their info, trainer
// cards and parties over the net commands from 0x1500, then pick the teams of a battle. The work is allocated by
// func_ov012_02174f4c and kept in the UnionSystem
// The net commands and the union events are colosseum_comm.c's
#include "field/colosseum.h"
#include "types.h"
#include "field/event_colosseum_battle.h"
#include "field/field_event.h"
#include "field/union_comm.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "pml/poke_party.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "field/comm_player.h"

void func_ov010_0216f40c(TrainerCardData *card, GameData *gameData, u32 a2, u32 a3, HeapID heapId);

static void func_ov012_02175358(UnionColosseum *colosseum, BattlePlayer *player, u8 index, u32 unused);


UnionColosseum *func_ov012_02174f4c(GameData *gameData, GameSystem *gsys, const PlayerInfo *info, u32 flag) {
    NetHandle *handle = func_020399a8();
    u32 netId = func_0203d1a4(handle);
    UnionColosseum *colosseum;
    ColosseumMember *member;
    int i;

    colosseum = GFL_HeapAllocate(0x41, sizeof(UnionColosseum), TRUE, __FILE__, 0x35);
    colosseum->commPlayer = func_ov010_02167518(4, gsys, 0x41, 0);
    for (i = 0; i < 4; i++) {
        colosseum->card[i] = GFL_HeapAllocate(0x41, sizeof(TrainerCardData), TRUE, __FILE__, 0x38);
        colosseum->party[i] = PokeParty_Create(0x41);
    }
    for (i = 0; i < 4; i++) {
        colosseum->unkCC[i] = 0xff;
        colosseum->unkD0[i] = 0xff;
    }
    colosseum->unkDC = 0xff;
    colosseum->unkDD = 0xff;
    PokeParty_Copy(GameData_GetParty(gameData), colosseum->party[netId]);
    member = &colosseum->members[netId];
    func_02008494(info, &member->info);
    OS_GetMacAddress(member->mac);
    member->unk26 = 1;
    member->unk27 = (flag == 1) ? 0 : 1;
    member->unk28 = 100;
    func_ov010_0216f40c(colosseum->card[netId], gameData, 1, 0, 0x41);
    colosseum->cardReady[netId] = 1;
    return colosseum;
}


// pokebw2's Colosseum_Free
void func_ov012_0217505c(UnionColosseum *colosseum, GameSystem *gsys) {
    int i;

    for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
        GFL_HeapFree(colosseum->card[i]);
        GFL_HeapFree(colosseum->party[i]);
    }
    func_ov010_0216754c(gsys, colosseum->commPlayer);
    GFL_HeapFree(colosseum);
}

void func_ov012_02175094(UnionColosseum *colosseum, BOOL value) {
    colosseum->unkC8_0 = value;
}

void func_ov012_021750b0(UnionColosseum *colosseum, u32 value) {
    colosseum->unkDC = value;
    colosseum->unkDD = 0xff;
}

// pokebw2's Colosseum_GetUnkDC
u8 func_ov012_021750c0(const UnionColosseum *colosseum) {
    return colosseum->unkDC;
}

// pokebw2's Colosseum_SetUnkDD
void func_ov012_021750c8(UnionColosseum *colosseum, u32 value) {
    colosseum->unkDD = value;
}

u8 func_ov012_021750d0(const UnionColosseum *colosseum) {
    return colosseum->unkDD;
}

// pokebw2's Colosseum_Vote
BOOL func_ov012_021750d8(UnionColosseum *colosseum, u32 netId, u8 value) {
    int i;
    BOOL accepted;

    if (value == 0xff) {
        colosseum->unkCC[netId] = value;
        accepted = TRUE;
    } else {
        for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
            if (value == colosseum->unkCC[i]) {
                accepted = FALSE;
                break;
            }
        }
        if (i == COLOSSEUM_MEMBER_MAX) {
            colosseum->unkCC[netId] = value;
            accepted = TRUE;
        }
    }
    colosseum->unkD0[netId] = accepted;
    return accepted;
}

void func_ov012_02175110(UnionColosseum *colosseum) {
    int i;

    if (func_0203d378()) {
        for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
            u8 value = colosseum->unkD0[i];

            if (value != 0xff && func_ov012_02174d2c(colosseum, i, value) == TRUE) {
                colosseum->unkD0[i] = 0xff;
            }
        }
    }
}

void func_ov012_02175144(UnionColosseum *colosseum) {
    int count;
    int i;

    if (func_0203d378()) {
        count = 0;
        for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
            if (colosseum->unkD4[i] == 1) {
                count++;
            }
        }
        if (count == func_0203d1b4()) {
            if (func_ov012_02174ed8() == TRUE) {
                for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
                    colosseum->unkD4[i] = 0;
                    colosseum->unkD8[i] = 0;
                }
            }
        } else {
            for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
                if (colosseum->unkD8[i] == 1 && func_ov012_02174e9c(i) == TRUE) {
                    colosseum->unkD8[i] = 0;
                }
            }
        }
    }
}

// pokebw2's Colosseum_StoreChoice
void func_ov012_021751b4(UnionColosseum *colosseum, u32 netId, const ColosseumChoice *choice) {
    colosseum->choice[netId] = *choice;
    colosseum->choiceReady[netId] = TRUE;
}

u8 func_ov012_021751dc(UnionColosseum *colosseum, u32 netId, ColosseumChoice *choice) {
    u8 ready;

    *choice = colosseum->choice[netId];
    ready = colosseum->choiceReady[netId];
    colosseum->choiceReady[netId] = FALSE;
    return ready;
}

void func_ov012_02175208(UnionColosseum *colosseum) {
    colosseum->unk161 = 0;
}

BOOL func_ov012_02175214(const UnionColosseum *colosseum) {
    int count = 0;
    int i;

    for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
        if (colosseum->partyReady[i] == 1) {
            count++;
        }
    }
    if (count >= func_0203d1b4()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov012_02175240(UnionColosseum *colosseum, BOOL keepOwn) {
    int self = func_0203d1a4(func_020399a8());
    int i;

    for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
        if (!keepOwn || self != i) {
            PokeParty_Init(colosseum->party[i]);
        }
        colosseum->partyReady[i] = FALSE;
    }
}

// pokebw2's Colosseum_BuildBattlePlayers
void func_ov012_02175280(UnionColosseum *colosseum, GameSystem *gsys, ColosseumPlayers *players, u32 unused) {
    int member_max = 0;
    int i;
    int self;
    int tr_no;
    int selfTeam;
    int slot;

    MI_CpuFill8(players, 0, sizeof(ColosseumPlayers));
    players->battle.unk44 = 3;
    for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
        if (colosseum->partyReady[i] == 0) {
            players->battle.unk44 = 1;
            break;
        }
        member_max++;
    }
    GFL_ASSERT(member_max == 2 || member_max == 4);
    self = func_0203d1a4(func_020399a8());
    selfTeam = colosseum->unk15C[self];
    for (i = 0; i < member_max; i++) {
        u8 team = colosseum->unk15C[i];

        if ((team & 1) == (selfTeam & 1)) {
            slot = 0;
        } else if (players->battle.unk44 == 1) {
            slot = 1;
        } else {
            slot = 2;
        }
        tr_no = slot + (team >> 1);
        GFL_ASSERT(tr_no < member_max);
        func_ov012_02175358(colosseum, &players->battle.players[tr_no], i, unused);
    }
    players->records = GameData_GetRecords(GSYS_GetGameData(gsys));
}

void func_ov012_0217534c(ColosseumPlayers *players) {
    MI_CpuFill8(players, 0, sizeof(ColosseumPlayers));
}

// pokebw2's Colosseum_SetPlayer
static void func_ov012_02175358(UnionColosseum *colosseum, BattlePlayer *player, u8 index, u32 unused) {
    player->party = colosseum->party[index];
    player->info = &colosseum->members[index].info;
    player->unk08[1] = colosseum->members[index].unk28;
}
