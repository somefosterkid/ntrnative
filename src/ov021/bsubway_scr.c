// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov033/bsubway_scr.c, ported by tools/port_file.py.
#include "types.h"
#include "battle/btl_setup.h"
#include "constants/pokemon.h"
#include "field/battle_facility.h"
#include "field/bsubway_scr.h"
#include "field/ov021_021ca544.h"
#include "field/player_state.h"
#include "gfl/heap.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/bsubway_save.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/version.h"

u16 func_ov021_021cbd90(BSubwayScrWork *bsw, u16 level, u8 index, u32 mode, u8 side);
void func_ov021_021ca4c0(BSubwayScrWork *bsw, SaveControl *save, u32 flag);

// The losses in a row that lower a Wi-Fi rank, by rank
const u8 data_ov021_021dacd0[12] = { 0, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0 };

// The Battle Points a streak of seven gives, by kind of train and by streaks done; the last row is the Wi-Fi train's,
// by rank
// clang-format off
static const u8 data_ov021_021dacdc[7][10] = {
    { 3, 3, 3, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 6, 7, 8, 9, 10, 10, 10, 10, 10 },
    { 3, 3, 3, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 6, 7, 8, 9, 10, 10, 10, 10, 10 },
    { 3, 3, 3, 0, 0, 0, 0, 0, 0, 0 },
    { 5, 6, 7, 8, 9, 10, 10, 10, 10, 10 },
    { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
};

// clang-format on

void func_ov021_021c9924(GameSystem *gsys) {
    func_02012ee4(GSYS_GetGameData(gsys), NULL);
}

BSubwayScrWork *func_ov021_021c9934(GameSystem *gsys, u16 a1, u16 a2) {
    BSubwayScrWork *bsw;
    GameData *gameData;
    PlayerInfo *playerInfo;
    SaveControl *save;
    s32 i;
    u8 value;
    u32 count;
    u32 score;
    u32 level;
    u32 teamIndex;
    u8 mode;

    gameData = GSYS_GetGameData(gsys);
    playerInfo = GetGameDataPlayerInfo(gameData);
    save = GameData_GetSaveControl(gameData);
    bsw = GFL_HeapAllocate(4, sizeof(BSubwayScrWork), TRUE, "bsubway_scr.c", 0x5f);
    bsw->heapId = 4;
    bsw->magic = 0x12345678;
    bsw->gameData = gameData;
    bsw->gender = getTrainerGender(playerInfo);
    bsw->unk70 = SaveControl_GetBlockPtr(save, SAVE_BLOCK_BSUBWAY_PLAY);
    bsw->unk74 = SaveControl_GetBlockPtr(save, SAVE_BLOCK_BSUBWAY_SCORE);
    bsw->unk78 = SaveControl_GetBlockPtr(save, SAVE_BLOCK_BSUBWAY_3B);
    func_0200d868(bsw->unk70, 0);
    func_02012ee4(gameData, bsw);
    if (a1 == 0) {
        bsw->playMode = a2;
        bsw->memberCount = func_ov021_021ca270(bsw->playMode);
        for (i = 0; i < 4; i++) {
            bsw->memberSlots[i] = 0xff;
        }
        for (i = 0; i < 14; i++) {
            bsw->unk32[i] = 0xffff;
        }
        func_0200d85c(bsw->unk70);
        func_0200da14(bsw->unk70);
        if (func_0200db44(bsw->unk74, bsw->playMode) == 1) {
            score = func_0200dac4(bsw->unk74, bsw->playMode);
            func_ov021_021ca238(bsw, score);
        }
        value = bsw->playMode;
        func_0200d914(bsw->unk70, 0, &value);
    } else {
        bsw->playMode = func_0200d884(bsw->unk70, 0, NULL);
        bsw->memberCount = func_ov021_021ca270(bsw->playMode);
        if (func_0200d884(bsw->unk70, 10, NULL) != 0) {
            func_ov021_021ca1e4(bsw);
        }
        func_0200d884(bsw->unk70, 5, bsw->memberSlots);
        func_0200d884(bsw->unk70, 8, bsw->unk32);
        if (bsw->playMode == 2 || bsw->playMode == 7) {
            bsw->unkC_5 = (u8)func_0200d884(bsw->unk70, 9, NULL);
            func_0200d884(bsw->unk70, 6, &bsw->teamConfigs[bsw->unkC_5]);
            level = 0x12f;
            if (getTrainerGender(&GameData_GetPlayerState(gameData)->playerInfo) != 0) {
                level -= 3;
            }
            teamIndex = bsw->unkC_5;
            count = func_0200d884(bsw->unk70, 7, NULL);
            func_ov021_021cbf38(bsw, &bsw->unk2C8[teamIndex], level + teamIndex, count, &bsw->teamConfigs[teamIndex],
                                bsw->heapId);
        }
        mode = bsw->playMode;
        if (func_0200db44(bsw->unk74, mode) == 1) {
            count = func_0200da54(bsw->unk70);
            score = func_0200db80(bsw->unk74, mode);
            func_ov021_021ca258(bsw, score, count);
        }
    }
    return bsw;
}


void func_ov021_021c9b24(GameSystem *gsys, BSubwayScrWork *bsw) {
    if (bsw != NULL) {
        if (bsw->allocatedBuffer != NULL) {
            GFL_HeapFree(bsw->allocatedBuffer);
            bsw->allocatedBuffer = NULL;
        }
        if (bsw->btlSetup != NULL) {
            BtlSetup_Free(bsw->btlSetup);
            bsw->btlSetup = NULL;
        }
        MI_CpuFill8(bsw, 0, sizeof(BSubwayScrWork));
        GFL_HeapFree(bsw);
    }
    func_02012ee4(GSYS_GetGameData(gsys), NULL);
}

void func_ov021_021c9b78(BSubwayScrWork *bsw) {
    u8 mode;
    u32 score;

    if (bsw->playMode == 2) {
        mode = 3;
    } else if (bsw->playMode == 7) {
        mode = 8;
    }
    bsw->playMode = mode;
    func_0200d914(bsw->unk70, 0, &mode);
    if (func_0200db44(bsw->unk74, bsw->playMode) == 1) {
        score = func_0200dac4(bsw->unk74, bsw->playMode);
        func_ov021_021ca238(bsw, score);
    } else {
        func_ov021_021ca250(bsw);
    }
}

void func_ov021_021c9bcc(BSubwayScrWork *bsw) {
    u8 value;

    value = bsw->playMode;
    func_0200d914(bsw->unk70, 0, &value);
    func_0200d914(bsw->unk70, 5, bsw->memberSlots);
    func_0200d914(bsw->unk70, 8, bsw->unk32);
    func_0200d868(bsw->unk70, 1);
    if (bsw->playMode == 2 || bsw->playMode == 7) {
        value = bsw->unkC_5;
        func_0200d914(bsw->unk70, 9, &value);
        func_0200d914(bsw->unk70, 6, &bsw->teamConfigs[bsw->unkC_5]);
        func_0200d914(bsw->unk70, 7, bsw->unk664 + bsw->unkC_5);
    }
}

void func_ov021_021c9c54(BSubwayScrWork *bsw, GameSystem *gsys) {
    PokeParty *party;
    PartyPkm *pkm;
    s32 i;

    bsw->memberCount = func_ov021_021ca270(bsw->playMode);
    func_0200d884(bsw->unk70, 5, bsw->memberSlots);
    party = func_ov021_021ca210(bsw);
    for (i = 0; i < bsw->memberCount; i++) {
        pkm = PokeParty_GetPkm(party, bsw->memberSlots[i]);
        bsw->memberSpecies[i] = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        bsw->memberItems[i] = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
    }
}

void func_ov021_021c9cac(BSubwayScrWork *bsw) {
    u8 value;

    value = bsw->playMode;
    func_0200d914(bsw->unk70, 0, &value);
    func_0200da14(bsw->unk70);
    func_0200d914(bsw->unk70, 5, bsw->memberSlots);
    func_0200d868(bsw->unk70, 1);
    if (bsw->playMode == 2 || bsw->playMode == 7) {
        value = bsw->unkC_5;
        func_0200d914(bsw->unk70, 9, &value);
        func_0200d914(bsw->unk70, 6, &bsw->teamConfigs[bsw->unkC_5]);
        func_0200d914(bsw->unk70, 7, bsw->unk664 + bsw->unkC_5);
    }
}

u16 func_ov021_021c9d30(GameSystem *gsys) {
    SaveControl *save;
    BSubwayPlayData *play;
    BSubwayScoreData *score;
    u8 mode;

    save = GameData_GetSaveControl(GSYS_GetGameData(gsys));
    play = SaveControl_GetBlockPtr(save, 0x39);
    score = SaveControl_GetBlockPtr(save, 0x3a);
    mode = func_0200d884(play, 0, NULL);
    func_0200da14(play);
    func_0200db1c(score, mode);
    return mode;
}

void func_ov021_021c9d70(GameSystem *gsys, BSubwayScrWork *bsw) {
    u16 value;
    u8 mode;
    BSubwayScoreData *score;

    value = bsw->unkE;
    mode = bsw->playMode;
    score = bsw->unk74;
    func_0200db08(score, mode, value);
    func_0200daec(score, mode, value);
    func_ov021_021ca2dc(bsw, GameData_GetSaveControl(bsw->gameData), 1, value);
    func_0200db1c(bsw->unk74, mode);
    func_0200da14(bsw->unk70);
}

u16 func_ov021_021c9db0(BSubwayScrWork *bsw) {
    u16 count;
    u16 countIndex;
    s32 index;
    u8 category;
    u8 reward;
    const u8 *row;

    if (bsw->playMode == 4) {
        reward = 0;
        if ((u16)func_0200d884(bsw->unk70, 11, NULL) == 1) {
            index = (s8)func_0200dc08(bsw->unk74);
            if (index < 0) {
                index = 0;
            } else if (index >= 10) {
                index = 9;
            }
            reward = data_ov021_021dacdc[6][index];
        } else {
            reward = 5;
        }
    } else {
        reward = 0;
        count = func_0200db80(bsw->unk74, bsw->playMode);
        switch (bsw->playMode) {
        case 0:
            category = 0;
            break;
        case 5:
            category = 1;
            reward = 1;
            break;
        case 1:
            category = 2;
            break;
        case 6:
            category = 3;
            reward = 1;
            break;
        case 2:
            category = 4;
            break;
        case 3:
            category = 4;
            break;
        case 7:
            category = 5;
            reward = 1;
            break;
        case 8:
            category = 5;
            reward = 1;
            break;
        case 4:
            category = 6;
            break;
        default:
            category = 0;
            break;
        }
        countIndex = count - 1;
        if ((s16)countIndex < 0) {
            countIndex = 0;
        } else if (countIndex >= 10) {
            countIndex = 9;
        }
        if (bsw->unkC_1 != 0) {
            if (reward == 1) {
                reward = 30;
            } else {
                reward = 10;
            }
        } else {
            row = data_ov021_021dacdc[category];
            reward = row[countIndex];
        }
    }
    if (reward == 0) {
        reward = 1;
    }
    func_0200da80(bsw->unk74, reward);
    return reward;
}


void func_ov021_021c9e8c(BSubwayScrWork *bsw) {
    u8 mode;
    u16 level;
    u16 choice;
    s32 i;

    mode = bsw->playMode;
    level = func_ov021_021ca2cc(func_ov021_021ca234(bsw));
    if (mode == 2 || mode == 3 || mode == 7 || mode == 8) {
        if (level < bsw->unk18) {
            level = bsw->unk18;
        }
        for (i = 0; i < 14; i++) {
            do {
                choice = func_ov021_021cbd90(bsw, level, (u8)(i / 2), mode, (u8)(i & 1));
            } while (func_ov021_021ca2a4(bsw->unk32, choice, i));
            bsw->unk32[i] = choice;
        }
    } else {
        for (i = 0; i < 7; i++) {
            do {
                choice = func_ov021_021cbd90(bsw, level, (u8)i, bsw->playMode, 0);
            } while (func_ov021_021ca2a4(bsw->unk32, choice, i));
            bsw->unk32[i] = choice;
        }
    }
}

u16 func_ov021_021c9f44(BSubwayScrWork *bsw, GameSystem *gsys) {
    PokeParty *party;
    PartyPkm *pkm;
    u16 i;

    if (bsw->unk84 != 0 || bsw->unk82 == 7 || bsw->unk82 == 8) {
        return 0;
    }
    party = func_ov021_021ca210(bsw);
    for (i = 0; i < bsw->memberCount; i++) {
        if (bsw->memberChoices[i] - 1 >= 6) {
            bsw->memberChoices[i] = 1;
        }
        bsw->memberSlots[i] = bsw->memberChoices[i] - 1;
        pkm = PokeParty_GetPkm(party, bsw->memberSlots[i]);
        bsw->memberSpecies[i] = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        bsw->memberItems[i] = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
    }
    return 1;
}

BOOL func_ov021_021c9fd0(BSubwayScrWork *bsw) {
    if (!bsw->unkC_0) {
        if (func_0200da54(bsw->unk70) < 7) {
            return FALSE;
        }
        bsw->unkC_0 = 1;
    }
    return TRUE;
}

void func_ov021_021c9ffc(BSubwayScrWork *bsw, GameSystem *gsys) {
    u8 mode;
    u16 value;

    mode = bsw->playMode;
    value = func_ov021_021ca234(bsw);
    func_0200db08(bsw->unk74, mode, value);
    func_0200daec(bsw->unk74, mode, func_0200dac4(bsw->unk74, mode));
    func_0200db60(bsw->unk74, mode);
    func_ov021_021ca2dc(bsw, GameData_GetSaveControl(bsw->gameData), 1, value);
    func_0200da14(bsw->unk70);
}

void func_ov021_021ca048(BSubwayScrWork *bsw, GameSystem *gsys) {
    bsw->unkC_0 = 0;
    bsw->unkC_1 = 0;
}

void func_ov021_021ca05c(BSubwayScrWork *bsw) {
    u16 ids[2];
    u16 values[2];
    s32 index;
    u32 round;
    u32 slot;

    round = func_0200da54(bsw->unk70);
    switch (bsw->playMode) {
    case 4:
        func_0200dea8(bsw->unk78, bsw->trainers, round, bsw->heapId);
        break;
    case 2:
    case 3:
    case 7:
    case 8:
        slot = round * 2;
        func_ov021_021cbed8(bsw, &bsw->trainers[0], bsw->unk32[slot], bsw->memberCount, NULL, NULL, NULL,
                            bsw->heapId);
        for (index = 0; index < bsw->memberCount; index++) {
            ids[index] = bsw->trainers[0].pokemon[index].species;
            values[index] = bsw->trainers[0].pokemon[index].item;
        }
        func_ov021_021cbed8(bsw, &bsw->trainers[1], bsw->unk32[slot + 1], bsw->memberCount, ids, values, NULL,
                            bsw->heapId);
        break;
    default:
        func_ov021_021cbed8(bsw, &bsw->trainers[0], bsw->unk32[round], bsw->memberCount, NULL, NULL, NULL,
                            bsw->heapId);
        break;
    }
}

u32 func_ov021_021ca150(BSubwayScrWork *bsw, u16 index) {
    return func_ov010_02168aec(bsw->trainers[index].trainerId);
}

u16 func_ov021_021ca164(BSubwayScoreData *score, GameSystem *gsys, u32 op) {
    u8 value;
    u32 limit;

    value = func_0200dc08(score);
    switch (op) {
    case 0:
        return value;
    case 3:
        func_0200dba0(score, 0, 2);
        if (value == 10) {
            return 0;
        }
        func_0200dbf0(score);
        return 1;
    case 4:
        limit = func_0200dc0c(score, 3);
        if (value == 1) {
            return 0;
        }
        if (limit >= data_ov021_021dacd0[value - 1]) {
            func_0200dbfc(score);
            func_0200dc0c(score, 2);
            func_0200dba0(score, 0, 2);
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

void func_ov021_021ca1e4(BSubwayScrWork *bsw) {
    if (bsw->allocatedBuffer != NULL) {
        GFL_HeapFree(bsw->allocatedBuffer);
    }
    bsw->allocatedBuffer =
        convertBoxedPokeSetToParty(getBattleBox(GameData_GetSaveControl(bsw->gameData)), HEAPID_GAMEEVENT);
}

PokeParty *func_ov021_021ca210(BSubwayScrWork *bsw) {
    if (func_0200d884(bsw->unk70, 10, NULL) == 0) {
        return GameData_GetParty(bsw->gameData);
    }
    return bsw->allocatedBuffer;
}

u16 func_ov021_021ca234(BSubwayScrWork *bsw) {
    return bsw->unkE;
}

void func_ov021_021ca238(BSubwayScrWork *bsw, u32 value) {
    bsw->unkE = value;
}

void func_ov021_021ca23c(BSubwayScrWork *bsw) {
    if (bsw->unkE < 0xffff) {
        bsw->unkE++;
    }
}

void func_ov021_021ca250(BSubwayScrWork *bsw) {
    bsw->unkE = 0;
}

void func_ov021_021ca258(BSubwayScrWork *bsw, u32 count, u32 extra) {
    u32 value;

    value = count * 7;
    value += extra;

    if (value > 0xffff) {
        value = 0xffff;
    }
    bsw->unkE = value;
}

u16 func_ov021_021ca270(u16 mode) {
    switch (mode) {
    case 0:
    case 4:
    case 5:
        return 3;
    case 1:
    case 6:
        return 4;
    case 2:
    case 3:
    case 7:
    case 8:
        return 2;
    default:
        return 0;
    }
}

BOOL func_ov021_021ca2a4(const u16 *list, u16 value, u16 count) {
    u16 i;

    for (i = 0; i < count; i++) {
        if (list[i] == value) {
            return TRUE;
        }
    }
    return FALSE;
}

u16 func_ov021_021ca2cc(s32 value) {
    return value / 7;
}

void func_ov021_021ca2dc(BSubwayScrWork *bsw, SaveControl *save, u32 a2, u32 a3) {
    u8 mode;

    switch (bsw->playMode) {
    case 0:
        func_ov021_021ca4c0(bsw, save, 0);
        return;
    case 4:
        func_ov021_021ca4c0(bsw, save, 1);
        mode = bsw->playMode;
        func_0200d914(bsw->unk70, 0, &mode);
        mode = func_0200da54(bsw->unk70) + 1;
        func_0200d914(bsw->unk70, 1, &mode);
        func_0200dc94(bsw->unk74, bsw->unk70);
        return;
    case 1:
        return;
    }
}

void func_ov021_021ca338(BSubwayScrWork *bsw, u8 variant) {
    u32 base;
    s32 index;

    base = 0x12c;
    if (variant != 0) {
        base += 3;
    }
    for (index = 0; index < 3; index++) {
        bsw->unk664[index] = func_ov021_021cbed8(bsw, &bsw->unk2C8[index], base + index, bsw->memberCount,
                                                 bsw->memberSpecies, bsw->memberItems, &bsw->teamConfigs[index],
                                                 bsw->heapId);
    }
}

void func_ov021_021ca3b4(BSubwayPokemon *dst, PartyPkm *pkm) {
    s32 i;

    dst->species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    dst->form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
    dst->item = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
    for (i = 0; i < 4; i++) {
        dst->moves[i] = PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + i, NULL);
        dst->ppUps |= PokeParty_GetParam(pkm, PKM_PARAM_MOVE1_PP_UP + i, NULL) << (2 * i);
    }
    dst->region = PokeParty_GetParam(pkm, PKM_PARAM_REGION, NULL);
    dst->id = PokeParty_GetParam(pkm, PKM_PARAM_ID, NULL);
    dst->personality = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);
    dst->ivs.all = PokeParty_GetParam(pkm, PKM_PARAM_IVS_ALL, NULL);
    for (i = 0; i < 6; i++) {
        dst->evs[i] = PokeParty_GetParam(pkm, PKM_PARAM_EV_HP + i, NULL);
    }
    dst->ability = PokeParty_GetParam(pkm, PKM_PARAM_ABILITY, NULL);
    dst->happiness = PokeParty_GetParam(pkm, PKM_PARAM_HAPPINESS, NULL);
    PokeParty_GetParam(pkm, PKM_PARAM_NICKNAME_RAW, dst->nickname);
}

void func_ov021_021ca4c0(BSubwayScrWork *bsw, SaveControl *save, u32 flag) {
    BSubwayPokemon *buffer;
    PokeParty *party;
    PartyPkm *pkm;
    s32 i;

    buffer = GFL_HeapAllocate(HEAPID_TAIL((HeapID)bsw->heapId), sizeof(BSubwayPokemon) * 3, FALSE, "bsubway_scr.c",
                              0x89a);
    MI_CpuFill8(buffer, 0, sizeof(BSubwayPokemon) * 3);
    party = func_ov021_021ca210(bsw);
    for (i = 0; i < 3; i++) {
        pkm = PokeParty_GetPkm(party, bsw->memberSlots[i]);
        func_ov021_021ca3b4(&buffer[i], pkm);
    }
    func_0200dc50(bsw->unk74, flag, buffer);
    MI_CpuFill8(buffer, 0, sizeof(BSubwayPokemon) * 3);
    GFL_HeapFree(buffer);
}

